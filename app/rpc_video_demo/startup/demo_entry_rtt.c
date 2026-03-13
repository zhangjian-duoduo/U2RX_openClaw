/*
 * File      : sample_vlcview.c
 * This file is part of SOCs BSP for RT-Thread distribution.
 *
 * Copyright (c) 2017 Shanghai Fullhan Microelectronics Co., Ltd.
 * All rights reserved
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *  Visit http://www.fullhan.com to get contact with Fullhan.
 *
 * Change Logs:
 * Date           Author       Notes
 */
#include "../sample_vlcview.h"
#include <optparse.h>

#ifdef __RTTHREAD_OS__

AOV_CONFIG g_aov_config = {
    .port = 1234,
    .aov_enable = 0,
    .aov_sleep = 0,
    .md_enable = 1,
    .aovnn_enable = 1,
    .interval_ms = AOV_INTERVAL,
    .frame_num = AOV_NN_FRAME_NUM,
    .venc_mempool_pct = AOV_VENC_MEM_POOL_PCT,
    .aovnn_threshold = AOV_NN_THRE,
    .aovmd_threshold = AOV_MD_THRE,
};

/* 帮助信息 */
static void usage(const char *progname)
{
    fprintf(stderr, "Usage: %s --ip <IP> --port <PORT> --aov-enable --interval <MS> --frame <NUM> "
                    "--venc-mempool <PCT> --aovnn-enable <0/1> --aovnn-threshold <VAL> --aovmd-enable <0/1> --aovmd-threshold <VAL> --aov-sleep <0/1>\n",
            progname);
    fprintf(stderr, "Example:\n");
    fprintf(stderr, "  %s --ip 192.168.1.10 --aov-enable 1 --port 8080 --interval 1000 --frame 30 "
                    "--venc-mempool 50 --aovnn-enable 1 --aovnn-threshold 500 --aovmd-enable 1 --aovmd-threshold 50 --aov-sleep 1\n",
            progname);
}

int rpc_video_demo(int argc, char **argv)
{
    int ret;
    pthread_t g_thread_vlcview = 0;
    pthread_attr_t attr;
    /* 定义长选项结构 */
    struct optparse_long longopts[] = {
        {"help", 'h', OPTPARSE_NONE},
        {"ip", 'i', OPTPARSE_REQUIRED},
        {"port", 'p', OPTPARSE_REQUIRED},
        {"aov-enable", 'a', OPTPARSE_REQUIRED},
        {"interval", 'v', OPTPARSE_REQUIRED},
        {"frame", 'f', OPTPARSE_REQUIRED},
        {"venc-mempool", 'm', OPTPARSE_REQUIRED},
        {"aovnn-enable", 'e', OPTPARSE_REQUIRED},
        {"aovmd-enable", 'd', OPTPARSE_REQUIRED},
        {"aovmd-threshold", 'n', OPTPARSE_REQUIRED},
        {"aovnn-threshold", 't', OPTPARSE_REQUIRED},
        {"aov-sleep", 's', OPTPARSE_REQUIRED},
        {"quit", 'q', OPTPARSE_NONE},
        {0}};

    /* 解析命令行参数 */
    int opt;
    struct optparse options;
    optparse_init(&options, argv);

    while ((opt = optparse_long(&options, longopts, NULL)) != -1)
    {
        switch (opt)
        {
        case 'i':                             /* 新增：处理 --ip */
            // g_aov_config.ip = strdup(options.optarg); /* 动态分配内存保存IP字符串 */
            strncpy(g_aov_config.ip, options.optarg, strlen(options.optarg) <= (sizeof(g_aov_config.ip) - 1) ? strlen(options.optarg): (sizeof(g_aov_config.ip) - 1));
            break;
        case 'p':
            g_aov_config.port = strtol(options.optarg, NULL, 0);
            break;
        case 'a':
            g_aov_config.aov_enable = strtol(options.optarg, NULL, 0);
            break;
        case 'v':
            g_aov_config.interval_ms = strtol(options.optarg, NULL, 0);
            break;
        case 'f':
            g_aov_config.frame_num = strtol(options.optarg, NULL, 0);
            break;
        case 'm':
            g_aov_config.venc_mempool_pct = strtol(options.optarg, NULL, 0);
            break;
        case 'e':
            g_aov_config.aovnn_enable = strtol(options.optarg, NULL, 0);
            break;
        case 't':
            g_aov_config.aovnn_threshold = strtol(options.optarg, NULL, 0);
            break;
        case 'd':
            g_aov_config.md_enable = strtol(options.optarg, NULL, 0);
            break;
        case 'n':
            g_aov_config.aovmd_threshold = strtol(options.optarg, NULL, 0);
            break;
        case 's':
            g_aov_config.aov_sleep = strtol(options.optarg, NULL, 0);
            break;
        case 'q':
            _vlcview_exit();
            return 0;
        case 'h':
            usage(argv[0]);
            return 0;
        default:
            usage(argv[0]);
            return -1;
        }
    }

    /* 检查必需参数是否提供 */
    if (g_aov_config.ip == NULL)
    {
        usage(argv[0]);
        return -1;
    }

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    ret = pthread_create(&g_thread_vlcview, &attr, _vlcview, NULL);
    if (ret != 0)
    {
        return -1;
    }

    return 0;
}

void user_main(void)
{
}
#include <rttshell.h>
SHELL_CMD_EXPORT(rpc_video_demo, rpc_video_demo());
#endif /*__RTTHREAD_OS__ end*/
