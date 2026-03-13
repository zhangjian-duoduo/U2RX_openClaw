#ifndef __RUNLOG_H__
#define __RUNLOG_H__

#define FUNC_SIRQ       ((void *)-1)
#define FUNC_WQ         ((void *)-2)
#define FUNC_IRQ_ENTER  ((void *)-3)
#define FUNC_IRQ_EXIT   ((void *)-4)

#define _RET_IP_        (unsigned long)__builtin_return_address(0)
#define _THIS_IP_  ({ __label__ __here; __here: (unsigned long)&&__here; })

void add_trace_node_func_ext(void *func, int line, int enter);
#define add_trace_node_func(line, enter) \
    add_trace_node_func_ext((void *)_THIS_IP_, (line), (enter))

void add_trace_node_irq(int irq, int enter);
void add_trace_node_usr(const char *des, int enter);
void add_trace_node_usr_printf(int enter, const char *fmt, ...);
void add_trace_node_sched(struct rt_thread *prev, struct rt_thread *next);
void dump_trace_log(void);
void set_trace_status(int status);
void start_trace_log(void);
void stop_trace_log(void);

#endif
