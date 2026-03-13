/*
 * File      : runlog.c
 */

#include <rthw.h>
#include <rtthread.h>
#include <irqdesc.h>
#include <fh_pmu.h>
#include <mmu.h>

#ifndef __packed
#define __packed __attribute__((packed))
#endif

#ifndef BIT
#define BIT(x) (1 << (x))
#endif

#ifndef FIELD_SIZEOF
#define FIELD_SIZEOF(t, f) (sizeof(((t *)0)->f))
#endif

#ifndef RT_USING_SMP
#define NR_CPUS             1
#else
#define NR_CPUS             RT_CPUS_NR
#endif

#define unlikely(x) (x)

#define SPIN_LOCK_UNLOCKED (spinlock_t) {0}
#define DEFINE_SPINLOCK(x) spinlock_t x = SPIN_LOCK_UNLOCKED
typedef struct {volatile int count; } spinlock_t;
#define spin_lock_irqsave(lock, flags)      \
    do                                      \
    {                                       \
        flags = rt_hw_interrupt_disable();  \
    } while (0)
#define spin_unlock_irqrestore(lock, flags) \
    do                                      \
    {                                       \
        rt_hw_interrupt_enable(flags);      \
    } while (0)



#define RUNLOG_VERSION      (0x524c0100)   /* rtos version */

#define TRACE_MAGIC         0xa579
#define TRACE_TYPE_SCHED    0
#define TRACE_TYPE_FUNC     1
#define TRACE_TYPE_IRQ      2
#define TRACE_TYPE_USR      3
#define TRACE_TYPE_MAX      3

#define TRACE_ACTION_ENTER  1
#define TRACE_ACTION_EXIT   0

struct trace_head
{
    int   version;
    int   pos;    /* current pos */
    int   end;
    int   size;
    int   cpu;
    int   total_cpu;
    int   sys_time_offset;
    char  name[64-28];
} __packed;

struct trace_sched
{
    unsigned int pid_pre;
    unsigned int pid;
    char name[16];
} __packed;
struct trace_func
{
    void *f;
    short line;
    short action;
} __packed;
struct trace_irq
{
    short irq_no;
    short action;
} __packed;
struct trace_usr
{
    int action;
    char des[16];
} __packed;

struct trace_node
{
    unsigned int magic:16;
    unsigned int type:2;
    unsigned int cpu:2;
    unsigned int pts_hi:12;
    unsigned int pts_lo;
    void *data[0];
} __packed;

#define MAX_TRACE_SIZE  (sizeof(struct trace_node) + sizeof(struct trace_sched))

#define RECORD_EN       BIT(0)
#define RECORD_DUMP     BIT(1)
#define RECORD_BIN      BIT(2)
#define RECORD_STS_OK   RECORD_EN

extern void mmu_clean_dcache(rt_uint32_t buffer, rt_uint32_t size);
#ifdef RT_RUNLOG_COHERENT_MEM
#define flush_cache(ptr, size)  mmu_clean_dcache((rt_uint32_t)ptr, (rt_uint32_t)size);
#else
#define flush_cache(ptr, size)
#endif

#define RUNLOG_SIZE     (RT_RUNLOG_SIZE_KB * 1024)
#ifdef SCHED_RUNTIME_WARN_US
static unsigned int warn_runtime_us = SCHED_RUNTIME_WARN_US;
#else
static unsigned int warn_runtime_us = 0;
#endif
static int trace_sts;
DEFINE_SPINLOCK(trace_lock);

#define NEXT_NODE(n) \
    ((n) = (typeof(n))((void *)(n) + sizeof(struct trace_node) + type_len(((struct trace_node *)(n))->type)))

#ifndef RT_RUNLOG_MEM_PERCPU
static unsigned int g_trace_pos = sizeof(struct trace_head);
static unsigned long g_trace_mem_vaddr;

#define CHECK_TRACE_VALID() do { \
    if (!g_trace_mem_vaddr) \
        return; \
    if ((trace_sts & 0x3) != RECORD_STS_OK) \
        return; \
} while (0)

#define TRACE_START(typ) \
    unsigned long flags; \
    struct trace_node *n; \
    struct trace_head *h = (struct trace_head *)g_trace_mem_vaddr; \
                            \
    CHECK_TRACE_VALID(); \
    spin_lock_irqsave(&trace_lock, flags); \
    h->pos = g_trace_pos; \
    CUR_NODE(n, 0); \
    tn = (__typeof__(tn))n->data; \
    n->type = typ; \
    n->magic = TRACE_MAGIC; \
    n->cpu = rt_hw_cpu_id(); \
    set_pts(n)

#define TRACE_END() do { \
    g_trace_pos += sizeof(struct trace_node) + sizeof(*tn); \
    if (g_trace_pos + MAX_TRACE_SIZE >= RUNLOG_SIZE) \
    { \
        h->end = g_trace_pos; \
        g_trace_pos = sizeof(struct trace_head); \
    } \
    flush_cache(n, MAX_TRACE_SIZE); \
    flush_cache(h, sizeof(*h)); \
    spin_unlock_irqrestore(&trace_lock, flags); \
} while (0)

#define FIRST_NODE(n, cpu) \
    ((n) = (typeof(n))(g_trace_mem_vaddr + sizeof(struct trace_head)))

#define CUR_NODE(n, cpu) \
    ((n) = (typeof(n))(g_trace_mem_vaddr + g_trace_pos))

#else

static unsigned int g_trace_pos[NR_CPUS] = {sizeof(struct trace_head)};
static unsigned int g_trace_mem_vaddr[NR_CPUS];

#define CHECK_TRACE_VALID() do { \
    if (!g_trace_mem_vaddr[0]) \
        return; \
    if ((trace_sts & 0x3) != RECORD_STS_OK) \
        return; \
} while (0)

#define TRACE_START(typ) \
    unsigned long flags; \
    int cpu = 0; \
    struct trace_node *n; \
    struct trace_head *h; \
                            \
    CHECK_TRACE_VALID(); \
    spin_lock_irqsave(&trace_lock, flags); \
    cpu = rt_hw_cpu_id(); \
    h = (struct trace_head *)g_trace_mem_vaddr[cpu]; \
    h->pos = g_trace_pos[cpu]; \
    CUR_NODE(n, cpu); \
    tn = (__typeof__(tn))n->data; \
    n->type = typ; \
    n->magic = TRACE_MAGIC; \
    n->cpu = cpu; \
    set_pts(n)

#define TRACE_END() do { \
    g_trace_pos[cpu] += sizeof(struct trace_node) + sizeof(*tn); \
    if (g_trace_pos[cpu] + MAX_TRACE_SIZE >= RUNLOG_SIZE) \
    { \
        h->end = g_trace_pos[cpu]; \
        g_trace_pos[cpu] = sizeof(struct trace_head); \
    } \
    flush_cache(n, MAX_TRACE_SIZE); \
    flush_cache(h, sizeof(*h)); \
    spin_unlock_irqrestore(&trace_lock, flags); \
} while (0)

#define FIRST_NODE(n, cpu) \
    ((n) = (typeof(n))(g_trace_mem_vaddr[cpu] + sizeof(struct trace_head)))

#define CUR_NODE(n, cpu) \
    ((n) = (typeof(n))(g_trace_mem_vaddr[cpu] + g_trace_pos[cpu]))

#endif

static void sched_check_reinit(void);

static void set_pts(struct trace_node *n)
{
    unsigned long long pts = fh_get_pts64(); /* get PTS */

    n->pts_lo = (unsigned int)pts;
    n->pts_hi = (unsigned short)(pts >> 32);
}

void add_trace_node_func_ext(void *func, int line, int enter)
{
    struct trace_func *tn;

    TRACE_START(TRACE_TYPE_FUNC);

    tn->f = (void *)func;
    tn->line = line;
    tn->action = enter;

    TRACE_END();
}
RTM_EXPORT(add_trace_node_func_ext);

void add_trace_node_irq(int irq, int enter)
{
    struct trace_irq *tn;

    TRACE_START(TRACE_TYPE_IRQ);

    tn->irq_no = irq;
    tn->action = enter;

    TRACE_END();
}
RTM_EXPORT(add_trace_node_irq);

void add_trace_node_usr(const char *des, int enter)
{
    struct trace_usr *tn;

    TRACE_START(TRACE_TYPE_USR);

    tn->action = enter;
    rt_strncpy(tn->des, des, sizeof(tn->des));
    tn->des[sizeof(tn->des)-1] = '\0';

    TRACE_END();
}
RTM_EXPORT(add_trace_node_usr);

void add_trace_node_usr_printf(int enter, const char *fmt, ...)
{
    char buf[FIELD_SIZEOF(struct trace_usr, des)];
    va_list args;

    va_start(args, fmt);
    rt_vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    add_trace_node_usr(buf, enter);
}
RTM_EXPORT(add_trace_node_usr_printf);

static void sched_check(struct rt_thread *prev, struct rt_thread *next);

void add_trace_node_sched(struct rt_thread *prev, struct rt_thread *next)
{
    struct trace_sched *tn;

    TRACE_START(TRACE_TYPE_SCHED);

    tn->pid = (unsigned int)next;
    tn->pid_pre = (unsigned int)prev;
    rt_strncpy(tn->name, next->name, sizeof(tn->name));
    tn->name[sizeof(tn->name)-1] = '\0';

    TRACE_END();
    if (warn_runtime_us > 0)
        sched_check(prev, next);
}
RTM_EXPORT(add_trace_node_sched);

static unsigned long type_len(int type)
{
    static const unsigned long ts[] = {
        [TRACE_TYPE_USR] = sizeof(struct trace_usr),
        [TRACE_TYPE_FUNC] = sizeof(struct trace_func),
        [TRACE_TYPE_IRQ] = sizeof(struct trace_irq),
        [TRACE_TYPE_SCHED] = sizeof(struct trace_sched),
    };
    if (unlikely(type > TRACE_TYPE_MAX))
        rt_kprintf("bad trace type: %x", type);
    return ts[type];
}

static char type_str(int type)
{
    static const char s[] = {
        [TRACE_TYPE_USR] = 'U',
        [TRACE_TYPE_FUNC] = 'F',
        [TRACE_TYPE_IRQ] = 'I',
        [TRACE_TYPE_SCHED] = 'S',
    };
    return s[type];
}

static int dump_trace_node(void *sfile, struct trace_node *n, int i)
{
    static char buf0[32], buf1[128];

#ifndef RT_RUNLOG_MEM_PERCPU
    long trace_mem_vaddr = g_trace_mem_vaddr;
#else
    long trace_mem_vaddr = g_trace_mem_vaddr[0];
#endif

    if (unlikely(n->magic != TRACE_MAGIC))
    {
        rt_kprintf("err magic: 0x%x in %p\n", n->magic, n);
        int *ptr = (int *)(n-1);
        int count = 16;
        do
        {
            if (count % 4 == 0)
                rt_kprintf("\n0x%08x: ", ptr);
            rt_kprintf("%08x ", *ptr++);
        } while (--count);
        rt_kputs("\n");
        return -EIO;
    }
    if (n->pts_lo)
    {
        unsigned long long pts = (unsigned long long)n->pts_lo|((unsigned long long)n->pts_hi<<32);

        rt_snprintf(buf0, sizeof(buf0), "%lX|%c|%u|%llu|", (long)n - trace_mem_vaddr, type_str(n->type), n->cpu, pts);
        switch (n->type)
        {
        case TRACE_TYPE_SCHED:
        {
            struct trace_sched *tn = (__typeof__(tn))n->data;

            rt_snprintf(buf1, sizeof(buf1), "%X,%X,%s\n", tn->pid_pre, tn->pid, tn->name);
            break;
        }
        case TRACE_TYPE_FUNC:
        {
            struct trace_func *tn = (__typeof__(tn))n->data;

            rt_snprintf(buf1, sizeof(buf1), "%x,%d,%x\n", tn->f, tn->line, tn->action);
            break;
        }
        case TRACE_TYPE_IRQ:
        {
            struct trace_irq *tn = (__typeof__(tn))n->data;
            struct rt_irq_desc *d = irq_to_desc(tn->irq_no);
            const char *name = "irq";

            #ifdef RT_USING_INTERRUPT_INFO
                name = d->name;
            #endif
            rt_snprintf(buf1, sizeof(buf1), "%s,%d,%x\n", name, tn->irq_no, tn->action);
            break;
        }
        case TRACE_TYPE_USR:
        {
            struct trace_usr *tn = (__typeof__(tn))n->data;
            rt_snprintf(buf1, sizeof(buf1), "%s,%x\n", tn->des, tn->action);
            break;
        }
        default:
            RT_ASSERT(0);
            break;
        }
        rt_kputs(buf0);
        rt_kputs(buf1);
    }
    return 0;
}

#define print(m, fmt, ...) \
    rt_kprintf(fmt, ##__VA_ARGS__)

static struct trace_node *find_next_valid_node(struct trace_node *n, int size)
{
    int j = 0;

    while (n->magic != TRACE_MAGIC && j < size/4)
    {
        n = (struct trace_node *)((void *)n + 4);
        j++;
    }
    return n;
}

static int _dump_trace_log(void *sfile, int cpu)
{
    int i = 0;
    unsigned long flags;
    struct trace_node *n;

#ifndef RT_RUNLOG_MEM_PERCPU
    long trace_mem_vaddr = g_trace_mem_vaddr;
    int trace_pos = g_trace_pos;
#else
    RT_ASSERT(cpu < NR_CPUS);
    long trace_mem_vaddr = g_trace_mem_vaddr[cpu];
    int trace_pos = g_trace_pos[cpu];
#endif

    struct trace_head *h = (struct trace_head *)trace_mem_vaddr;

    if (!trace_mem_vaddr)
        return 0;

    spin_lock_irqsave(&trace_lock, flags);
    trace_sts |= RECORD_DUMP;
    fh_pmu_wdt_pause();

    print(f, "###### ver:0x%x %s nr_cpus:%u, pts-printk_time=%dus\n", h->version,
        #ifdef RT_RUNLOG_MEM_PERCPU
            "use percpu mem"
        #else
            ""
        #endif
            , NR_CPUS, h->sys_time_offset);
    print(sfile, "#########g_trace_pos=0x%x/0x%x/0x%x. addr=%lx\n", trace_pos, h->end, RUNLOG_SIZE, trace_mem_vaddr);

    if (h->end)
    {
        CUR_NODE(n, cpu);
find_next:
        n = find_next_valid_node(n, MAX_TRACE_SIZE);
        while ((unsigned long)n < trace_mem_vaddr + h->end)
        {
            if (dump_trace_node(sfile, n, i++))
                goto find_next;
            NEXT_NODE(n);
        }
    }

    FIRST_NODE(n, cpu);
    while ((unsigned long)n < trace_mem_vaddr + trace_pos)
    {
        dump_trace_node(sfile, n, i++);
        NEXT_NODE(n);
    }

    trace_sts &= ~RECORD_DUMP;
    sched_check_reinit();
    fh_pmu_wdt_resume();
    spin_unlock_irqrestore(&trace_lock, flags);
    return 0;
}

void dump_trace_log(void)
{
#ifndef RT_RUNLOG_MEM_PERCPU
    _dump_trace_log(NULL, 0);
#else
    int i = 0;

    for (i = 0; i < NR_CPUS; i++)
    {
        _dump_trace_log(NULL, i);
    }
#endif
}
RTM_EXPORT(dump_trace_log);

void set_trace_status(int status)
{
    unsigned long flags;

    spin_lock_irqsave(&trace_lock, flags);
    trace_sts = status;
    spin_unlock_irqrestore(&trace_lock, flags);
}
RTM_EXPORT(set_trace_status);

void stop_trace_log(void)
{
    set_trace_status(0);
}
RTM_EXPORT(stop_trace_log);

void start_trace_log(void)
{
    set_trace_status(RECORD_EN);
}
RTM_EXPORT(start_trace_log);

/**************************** sched info *****************************/
static void dump_process_info(struct rt_thread *tsk, unsigned long long pts, unsigned int runtime)
{
#ifdef RT_USING_SMP
    int task_cpu = tsk->oncpu;
#else
    int task_cpu = 0;
#endif
    rt_kprintf("### [%llu] cpu:%u task:%s[0x%x] cost %uus\n", pts, task_cpu, tsk->name, tsk, runtime);
    extern long list_thread(void);
    list_thread();
}

static unsigned long long last_pts[NR_CPUS];

static void sched_check(struct rt_thread *prev, struct rt_thread *next)
{
    static struct rt_thread *last[NR_CPUS];
    unsigned long long pts;
    int cpu = rt_hw_cpu_id();

    pts = fh_get_pts64();
    if (last[cpu] == prev && last_pts[cpu])
    {
        unsigned int runtime = pts - last_pts[cpu];

        /* filter out idle task */
        if (runtime >= warn_runtime_us && rt_strncmp(prev->name, "tidle", 5))
            dump_process_info(prev, pts, runtime);
    }

    last[cpu] = next;
    last_pts[cpu] = fh_get_pts64();
}

static void sched_check_reinit(void)
{
    rt_memset(last_pts, 0, sizeof(last_pts));
}

static void init_trace_head(long head, int size, int cpu, int offset)
{
    struct trace_head *h = (struct trace_head *)head;

    h->version = RUNLOG_VERSION;
    h->pos = 0;
    h->end = 0;
    h->size = size;
    h->cpu = cpu;
    h->total_cpu = NR_CPUS;
    /* delta time of pts and printk */
    h->sys_time_offset = offset;
    flush_cache(h, sizeof(*h));
}

/**************************runlog dev ops *****************************/
static rt_err_t runlog_open(rt_device_t dev, rt_uint16_t oflag)
{
    unsigned long flags;

    spin_lock_irqsave(&trace_lock, flags);
    trace_sts |= RECORD_DUMP;
    spin_unlock_irqrestore(&trace_lock, flags);
    return 0;
}

static rt_err_t runlog_close(rt_device_t dev)
{
    unsigned long flags;

    spin_lock_irqsave(&trace_lock, flags);
    trace_sts &= ~RECORD_DUMP;
    sched_check_reinit();
    spin_unlock_irqrestore(&trace_lock, flags);

    return RT_EOK;
}


static rt_size_t runlog_read(rt_device_t dev, rt_off_t pos, void *buffer,
                               rt_size_t size)
{
#ifndef RT_RUNLOG_MEM_PERCPU
    long trace_mem_vaddr = g_trace_mem_vaddr;
    int runlog_size = RUNLOG_SIZE;
#else
    long trace_mem_vaddr = g_trace_mem_vaddr[0];
    int runlog_size = RUNLOG_SIZE * NR_CPUS;
#endif
    if (pos >= runlog_size)
        return 0;
    if (size > runlog_size - pos)
        size = runlog_size - pos;
    rt_memcpy(buffer, (unsigned char *)trace_mem_vaddr + pos, size);
    return size;
}

#ifdef RT_USING_DEVICE_OPS
static struct rt_device_ops runlog_dev_ops = {
    .open = runlog_open,
    .read = runlog_read,
    .close = runlog_close,
};
#endif

static struct rt_device runlog_dev = {
    .type            = RT_Device_Class_Char,
#ifdef RT_USING_DEVICE_OPS
    .ops = &runlog_dev_ops;
#else
    .open            = runlog_open,
    .read            = runlog_read,
    .close           = runlog_close,
#endif
};


int runlog_init(void)
{
    unsigned long flags;
    unsigned long paddr;
    unsigned long long sys_clock =  rt_tick_get_millisecond() * 1000ull;
    int sys_time_offset = fh_get_pts64()-sys_clock;

#ifndef RT_RUNLOG_MEM_PERCPU
    long trace_mem_vaddr = g_trace_mem_vaddr;
    int runlog_size = RUNLOG_SIZE;
#else
    long trace_mem_vaddr = g_trace_mem_vaddr[0];
    int runlog_size = RUNLOG_SIZE * NR_CPUS;
#endif

    if (!trace_mem_vaddr)
    {
#ifdef RT_RUNLOG_USE_CUSTOM_MEM
        paddr = RT_RUNLOG_CUSTOM_MEM_ADDR;
        trace_mem_vaddr = (int)paddr;
#else
        trace_mem_vaddr = (unsigned long)rt_malloc(runlog_size);
        paddr = trace_mem_vaddr;
#endif
        if (!trace_mem_vaddr)
        {
            rt_kprintf("runlog_init: failed!!!!\n");
            return -ENOMEM;
        }
        else
        {
            rt_kprintf("########## runlog_init: ver:0x%x va(%08lx) pa(%08lx). %scached\n", RUNLOG_VERSION, trace_mem_vaddr, (long)paddr,
            #ifdef RT_RUNLOG_COHERENT_MEM
                "non"
            #else
                ""
            #endif
                );
            rt_kprintf("\t\tsize:0x%x%s, nr_cpus:%u\n", RUNLOG_SIZE,
            #ifdef RT_RUNLOG_MEM_PERCPU
                "xN (use percpu mem)"
            #else
                ""
            #endif
                , NR_CPUS);
            rt_kprintf("\t\tpts-printk_time=%dus\n\n", sys_time_offset);
            rt_memset((void *)trace_mem_vaddr, 0, runlog_size);
#ifndef RT_RUNLOG_MEM_PERCPU
            g_trace_mem_vaddr = trace_mem_vaddr;
            init_trace_head(g_trace_mem_vaddr, runlog_size, NR_CPUS, sys_time_offset);
#else
            {
                int i;
                for (i = 0; i < NR_CPUS; i++)
                {
                    g_trace_mem_vaddr[i] = trace_mem_vaddr + i*RUNLOG_SIZE;
                    g_trace_pos[i] = g_trace_pos[0];
                    init_trace_head(g_trace_mem_vaddr[i], runlog_size, i, sys_time_offset);
                }
            }
#endif
        }
    }

    spin_lock_irqsave(&trace_lock, flags);
#ifdef RT_RUNLOG_TRACE_WHEN_STARTUP
    trace_sts = RECORD_STS_OK;
#else
    trace_sts = 0;
#endif
    spin_unlock_irqrestore(&trace_lock, flags);

    int ret = rt_device_register(&runlog_dev, "runlog", RT_DEVICE_FLAG_RDONLY);
    if (ret)
    {
        rt_kprintf("i2s device register error %d\n", ret);
        return ret;
    }
    return 0;
}

static void runlog_cmd(int argc, char **argv)
{
    char *cmd;

    if (argc < 2)
    {
        rt_kprintf("usage: \n");
        rt_kprintf("    %s 1        (re)start record\n", argv[0]);
        rt_kprintf("    %s 0        stop record\n", argv[0]);
        rt_kprintf("    %s dump     dump trace log use printf\n", argv[0]);
        rt_kprintf("    %s warn [time]  set schedule warnning time in us\n", argv[0]);
        return;
    }

    cmd = argv[1];

    if (cmd[0] == '1')
    {
        if (!(trace_sts & RECORD_EN))
            sched_check_reinit();
        start_trace_log();
    }
    else if (cmd[0] == '0')
        stop_trace_log();
    else if (rt_strncmp("dump", cmd, 4) == 0)
    {
        dump_trace_log();
        return;
    } else if (rt_strncmp("warn", cmd, 4) == 0)
    {
        int val;

        if (argc < 3)
            rt_kprintf("current SCHED_RUNTIME_WARN_US = %uus\n", warn_runtime_us);
        else
        {
            val = rt_atoi(argv[2]);
            rt_kprintf("set warn_runtime_us = %d\n", val);
            warn_runtime_us = val;
        }
    }
    rt_kprintf("sts:%x en:%d fmt:%s dump:%d\n", trace_sts, !!(trace_sts&RECORD_EN),
        (trace_sts&RECORD_BIN)?"bin":"txt", !!(trace_sts&RECORD_DUMP));
}

MSH_CMD_EXPORT_ALIAS(runlog_cmd, runlog, dump & control runlog);
