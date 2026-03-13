#ifndef __SAMPLE_VLCVIEW_H__
#define __SAMPLE_VLCVIEW_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>
#include "libpes/include/libpes.h"
#include "librecord/include/librecord.h"
#include <fh_rpc_customer_mpi.h>
#include "types/type_def.h"
#include "dsp/fh_system_mpi.h"
#include "dsp/fh_vpu_mpi.h"
#include "dsp/fh_venc_mpi.h"
#include "isp/isp_enum.h"
#include "aov_xbus.h"
#include "sample_opts.h"

#define MAX_GRP_NUM 2
#define MAX_VPU_CHN_NUM 4
#define MAX_PES_CHANNEL_COUNT 36

typedef struct
{
    char ip[20];
    int port;
    int aov_enable;
    int aov_sleep;
    int md_enable;
    int aovnn_enable;
    int interval_ms;
    int frame_num;
    int venc_mempool_pct;
    int aovnn_threshold;
    int aovmd_threshold;
} AOV_CONFIG;

void *_vlcview(void *arg);
void _vlcview_exit(void);
#endif