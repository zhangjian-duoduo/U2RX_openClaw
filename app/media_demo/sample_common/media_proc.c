#include "media_proc.h"

#ifdef __LINUX_OS__
#include <sys/ioctl.h>
static int write_proc(char *path, char *s)
{
    int fd;

    fd = open(path, O_RDWR);
    if (fd < 0)
    {
        printf("Error: open %s!\n", path);
        return -1;
    }

    write(fd, s, strlen(s));
    close(fd);
    return 0;
}

void media_write_proc_fast(char *s)
{
#define XBUS_IOC_CMD_FAST_TRACE (0x07 << 28)

    struct ioc_fast_trace
    {
        void *str;
        unsigned int len; /*length include the trailing zero*/
    };

    static int mfd = -1;

    int ret;
    struct ioc_fast_trace ioc;

    if (!s)
        return;

    if (mfd < 0)
    {
        mfd = open("/dev/rtxbus", O_RDWR);
        if (mfd < 0)
        {
            printf("Error: cann't open xbus dev file!\n");
            return;
        }
    }

    ioc.str = s;
    ioc.len = strlen(s) + 1;

    ret = ioctl(mfd, XBUS_IOC_CMD_FAST_TRACE, &ioc);
    if (ret < 0)
    {
        printf("Error: xbus fast trace failed %08x\n", ret);
    }
}

static int vpu_write_proc(char *s)
{
    return write_proc("/proc/driver/vpu", s);
}

#if !defined CONFIG_ARCH_MC632X
static int enc_write_proc(char *s)
{
    return write_proc("/proc/driver/enc", s);
}

static int jpeg_write_proc(char *s)
{
    return write_proc("/proc/driver/jpeg", s);
}
#endif

#ifdef BGM_SUPPORT
static int bgm_write_proc(char *s)
{
    return write_proc("/proc/driver/bgm", s);
}
#endif

static int isp_write_proc(char *s)
{
    return write_proc("/proc/driver/isp", s);
}

static int mipi_write_proc(char *s)
{
    return write_proc("/proc/driver/mipi", s);
}

static int vicap_write_proc(char *s)
{
    return write_proc("/proc/driver/vicap", s);
}

static int media_trace_write_proc(char *s)
{
    return write_proc("/proc/driver/trace", s);
}
#else
#ifdef RPC_MEDIA
extern int media_proc_write(int argc, char *argv[]);
extern int media_proc_read(int argc, char *argv[]);

int vpu_write_proc(char *s)
{
    char *argv[3];
    argv[0] = "media";
    argv[1] = "vpu";
    argv[2] = s;
    return media_proc_write(3, argv);
}

int enc_write_proc(char *s)
{
    char *argv[3];
    argv[0] = "media";
    argv[1] = "enc";
    argv[2] = s;
    return media_proc_write(3, argv);
}

int jpeg_write_proc(char *s)
{
    char *argv[3];
    argv[0] = "media";
    argv[1] = "jpeg";
    argv[2] = s;
    return media_proc_write(3, argv);
}

int bgm_write_proc(char *s)
{
    char *argv[3];
    argv[0] = "media";
    argv[1] = "bgm";
    argv[2] = s;
    return media_proc_write(3, argv);
}

int vicap_write_proc(char *s)
{
    char *argv[3];
    argv[0] = "media";
    argv[1] = "vicap";
    argv[2] = s;
    return media_proc_write(3, argv);
}

int isp_write_proc(char *s)
{
    char *argv[3];
    argv[0] = "media";
    argv[1] = "isp";
    argv[2] = s;
    return media_proc_write(3, argv);
}

int mipi_write_proc(char *s)
{
    char *argv[3];
    argv[0] = "media";
    argv[1] = "mipi";
    argv[2] = s;
    return media_proc_write(3, argv);
}

int media_trace_write_proc(char *s)
{
    char *argv[3];
    argv[0] = "media";
    argv[1] = "trace";
    argv[2] = s;
    return media_proc_write(3, argv);
}
#else
extern int vpu_write_proc(char *s);
#if !defined CONFIG_ARCH_MC632X
extern int enc_write_proc(char *s);
extern int jpeg_write_proc(char *s);
#endif
#ifdef BGM_SUPPORT
extern int bgm_write_proc(char *s);
#endif
extern int vicap_write_proc(char *s);
extern int isp_write_proc(char *s);
extern int mipi_write_proc(char *s);
extern int media_trace_write_proc(char *s);
#endif
#endif

void write_media_proc(char *mod, char *config)
{
    // printf("[Write Proc]\"echo %s > %s\"\n", config, mod);
    if (strcmp(mod, "/proc/driver/vpu") == 0)
    {
        vpu_write_proc(config);
    }
    if (strcmp(mod, "/proc/driver/enc") == 0)
    {
#if !defined CONFIG_ARCH_MC632X
        enc_write_proc(config);
#else
        printf("not supported on this platform!\n");
#endif
    }
    if (strcmp(mod, "/proc/driver/jpeg") == 0)
    {
#if !defined CONFIG_ARCH_MC632X
        jpeg_write_proc(config);
#else
        printf("not supported on this platform!\n");
#endif
    }
    if (strcmp(mod, "/proc/driver/bgm") == 0)
    {
#ifdef BGM_SUPPORT
        bgm_write_proc(config);
#else
        printf("not supported on this platform!\n");
#endif
    }
    if (strcmp(mod, "/proc/driver/isp") == 0)
    {
        isp_write_proc(config);
    }
    if (strcmp(mod, "/proc/driver/mipi") == 0)
    {
        mipi_write_proc(config);
    }
    if (strcmp(mod, "/proc/driver/vicap") == 0)
    {
        vicap_write_proc(config);
    }
    if (strcmp(mod, "/proc/driver/trace") == 0)
    {
        media_trace_write_proc(config);
    }
}
