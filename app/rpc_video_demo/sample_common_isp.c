#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <pthread.h>
#include "types/type_def.h"
#include "dbi_over_tcp.h"
#include "sample_opts.h"

static struct dbi_tcp_config g_dbi_tcp_conf;
static volatile int g_coolview_is_running;
static volatile int g_stop_coolview;

FH_SINT32 sample_common_start_coolview(FH_VOID *arg)
{
    FH_SINT32 ret;
    pthread_t thread_dbg;
    pthread_attr_t attr;
    struct sched_param param;

    if (g_coolview_is_running)
    {
        printf("Error: coolview is already running!\n");
        return -1;
    }

    g_stop_coolview = 0;
    g_dbi_tcp_conf.cancel = &g_stop_coolview;
    g_dbi_tcp_conf.port = 8888;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize(&attr, 3 * 1024);
    pthread_attr_setschedparam(&attr, &param);
    ret = pthread_create(&thread_dbg, &attr, (void *(*)(void *))libtcp_dbi_thread, &g_dbi_tcp_conf);
    if (!ret)
    {
        g_coolview_is_running = 1;
    }
    else
    {
        printf("Error: create coolview thread failed!\n");
    }

    return ret;
}

FH_SINT32 sample_common_stop_coolview(FH_VOID)
{
    if (g_coolview_is_running)
    {
        /*wait for coolview thread to exit*/
        g_stop_coolview = 1;
        while (g_stop_coolview)
        {
            usleep(20*1000);
        }
        g_coolview_is_running = 0;
    }

    return 0;
}



