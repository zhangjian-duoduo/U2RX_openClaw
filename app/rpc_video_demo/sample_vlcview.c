#include "sample_vlcview.h"

extern AOV_CONFIG g_aov_config;
static FH_BOOL g_stop_running = FH_FALSE;
static pthread_t g_thread_stream = 0;
#ifdef FH_USING_AOV_DEMO
static pthread_t g_thread_status = 0;
pthread_mutex_t g_aov_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

void _vlcview_exit(void)
{
    if (!g_thread_stream)
    {
        printf("rpc video demo is not running!\n");
        return;
    }
    g_stop_running = FH_TRUE;
#ifdef FH_USING_STREAM_UDP
    libpes_uninit();
#endif
#ifdef OPEN_RECORD
    librecord_uninit();
#endif
#ifdef OPEN_COOLVIEW
    sample_common_stop_coolview();
#endif
    while (g_stop_running)
    {
        sleep(1);
    }
    printf("demo normal exit!\n");
}

#ifdef FH_USING_AOV_DEMO
int linux_aov_exit(void)
{
    AOV_XBUS_CMD_S aov_cmd;

    memset(&aov_cmd, 0, sizeof(aov_cmd));
    aov_cmd.cmd = AOV_CMD_EXIT_AOV;
    aov_cmd.data.aov.aov_enable = 0;
    printf("%s: AOV_CMD_EXIT_AOV!\n", __func__);
    return FH_RPC_Customer_Command(RPC_AOV_CMD_3, &aov_cmd, sizeof(aov_cmd));
}

void *check_event_status_proc(void *arg)
{
    int aov_event_status = 0;

    while (1)
    {
        pthread_mutex_lock(&g_aov_mutex);
        aov_event_status = check_event_status();
        if ((aov_event_status & AOV_WAKEUP_USER))
        {
            linux_aov_exit();
        }
        pthread_mutex_unlock(&g_aov_mutex);
        sleep(1);
    }
}

int check_event_status(void)
{
    AOV_XBUS_CMD_S aov_cmd;
    int aov_event_status = 0;
    int ret = 0;

    memset(&aov_cmd, 0, sizeof(aov_cmd));
    aov_cmd.cmd = AOV_CMD_QUERY_STATE;

    ret = FH_RPC_Customer_Command(RPC_AOV_CMD_3 | RPC_CUSTOMER_FLAG_NEED_CopyBack, &aov_cmd, sizeof(aov_cmd));
    if (0 == ret)
    {
        printf("GRPID %d AOV_CMD_QUERY_STATE = 0x%x\n", aov_cmd.data.wake_source.grpid, aov_cmd.data.wake_source.event_status);
    }
    aov_event_status = aov_cmd.data.wake_source.event_status;

    return aov_event_status;
}

#endif

void *sample_vlcview_get_stream_proc(void *arg)
{
    FH_SINT32 ret, i;
    FH_VENC_STREAM stream;
    unsigned int chan;
    FH_SINT32 subtype;

    struct vlcview_enc_stream_element stream_element;
    while (!g_stop_running)
    {
        memset(&stream_element, 0, sizeof(stream_element));
        memset(&stream, 0, sizeof(stream));
#ifdef FH_USING_AOV_DEMO
        pthread_mutex_lock(&g_aov_mutex);
#endif

        ret = FH_VENC_GetStream_Timeout(FH_STREAM_H265 | FH_STREAM_H264 | FH_STREAM_JPEG, &stream, 10 * 1000);
        if (ret == RETURN_OK)
        {
            if (stream.stmtype == FH_STREAM_H265)
            {
                stream_element.enc_type = VLCVIEW_ENC_H265;
                stream_element.frame_type = stream.h265_stream.frame_type == FH_FRAME_I ? VLCVIEW_ENC_I_FRAME : VLCVIEW_ENC_P_FRAME;
                stream_element.frame_len = stream.h265_stream.length;
                stream_element.time_stamp = stream.h265_stream.time_stamp;
                stream_element.nalu_count = stream.h265_stream.nalu_cnt;
                chan = stream.chan;
                for (i = 0; i < stream_element.nalu_count; i++)
                {
                    stream_element.nalu[i].start = stream.h265_stream.nalu[i].start;
                    stream_element.nalu[i].len = stream.h265_stream.nalu[i].length;
#ifdef OPEN_RECORD
                    subtype = stream.h265_stream.frame_type == FH_FRAME_I ? DMC_MEDIA_SUBTYPE_IFRAME : DMC_MEDIA_SUBTYPE_PFRAME;
                    librecord_save(stream.chan, DMC_MEDIA_TYPE_H265, subtype, stream.h265_stream.nalu[i].start, stream.h265_stream.nalu[i].length);
#endif
                }
            }
            else if (stream.stmtype == FH_STREAM_H264)
            {
                stream_element.enc_type = VLCVIEW_ENC_H264;
                stream_element.frame_type = stream.h264_stream.frame_type == FH_FRAME_I ? VLCVIEW_ENC_I_FRAME : VLCVIEW_ENC_P_FRAME;
                stream_element.frame_len = stream.h264_stream.length;
                stream_element.time_stamp = stream.h264_stream.time_stamp;
                stream_element.nalu_count = stream.h264_stream.nalu_cnt;
                chan = stream.chan;
                for (i = 0; i < stream_element.nalu_count; i++)
                {
                    stream_element.nalu[i].start = stream.h264_stream.nalu[i].start;
                    stream_element.nalu[i].len = stream.h264_stream.nalu[i].length;
#ifdef OPEN_RECORD
                    subtype = stream.h264_stream.frame_type == FH_FRAME_I ? DMC_MEDIA_SUBTYPE_IFRAME : DMC_MEDIA_SUBTYPE_PFRAME;
                    librecord_save(stream.chan, DMC_MEDIA_TYPE_H264, subtype, stream.h264_stream.nalu[i].start, stream.h264_stream.nalu[i].length);
#endif
                }
            } else if (stream.stmtype == FH_STREAM_JPEG)
            {
                printf("[chan %d]get jpeg start %p length %d\n", stream.chan, stream.jpeg_stream.start, stream.jpeg_stream.length);
            }
#ifdef FH_USING_STREAM_UDP
            libpes_stream_pack(chan, stream_element);
#endif
            FH_VENC_ReleaseStream(&stream);
        }
#ifdef FH_USING_AOV_DEMO
        pthread_mutex_unlock(&g_aov_mutex);
        usleep(1);
#endif
    }

    return NULL;
}

#ifdef FH_USING_AOV_DEMO
static pthread_t g_thread_aov = 0;
static int g_aov_resume_reason = 0;
int linux_aov_config(void)
{
    AOV_XBUS_CMD_S aov_cmd;

    if (g_aov_config.aov_enable == 0)
        printf("stop aov.........\n");
    else
        printf("start aov.........\n");

    memset(&aov_cmd, 0, sizeof(aov_cmd));
    aov_cmd.cmd = AOV_CMD_START_AOV;
    aov_cmd.data.aov.aov_enable = g_aov_config.aov_enable;
    aov_cmd.data.aov.md_enable = g_aov_config.md_enable;
    aov_cmd.data.aov.aovnn_enable = g_aov_config.aovnn_enable;
    /* aov_cmd.data.video.AovChnNum = 1; */
    aov_cmd.data.aov.aov_interval = g_aov_config.interval_ms;
    aov_cmd.data.aov.frame_num = g_aov_config.frame_num; /* 帧号好计数一些，时间不好控制 */
    aov_cmd.data.aov.venc_mempool_pct = g_aov_config.venc_mempool_pct;
    aov_cmd.data.aov.aovnn_threshold = g_aov_config.aovnn_threshold / 1000.0f;
    aov_cmd.data.aov.aovmd_threshold = g_aov_config.aovmd_threshold;

    return FH_RPC_Customer_Command(RPC_AOV_CMD_3, &aov_cmd, sizeof(aov_cmd));
}

void *aov_main(void *arg)
{
    FH_SINT32 ret = 0;
    static AOV_XBUS_CMD_S aov_cmd;
    int aov_event_status = 0;

    ret = FH_RPC_Customer_Init();
    if (ret != 0)
    {
        printf("FH_RPC_Customer_Init: failed, error=%08x\n", ret);
        return NULL;
    }
    pthread_create(&g_thread_status, NULL, check_event_status_proc, NULL);

    ret = linux_aov_config();
    if (ret)
        printf("linux linux_aov_config failed ret %x ....\n", ret);
    sleep(2);
    while (g_aov_config.aov_enable && g_aov_config.aov_sleep && (!g_stop_running))
    {
        aov_event_status = check_event_status();
        if ((aov_event_status & AOV_WAKEUP_USER))
        {
            linux_aov_exit();
            sleep(30);
        }
        pthread_mutex_lock(&g_aov_mutex);
        ret = linux_aov_config();
        if (ret)
            printf("linux linux_aov_config failed ret %x ....\n", ret);
        memset(&aov_cmd, 0, sizeof(aov_cmd));
        aov_cmd.cmd = AOV_CMD_SLEEP;
        ret = FH_RPC_Customer_Command(RPC_AOV_CMD_3, &aov_cmd, sizeof(aov_cmd));
        if (ret < 0)
        {
            printf("FH_RPC_Customer_Command: failed, error=%08x\n", ret);
        }

#ifdef __LINUX_OS__
        printf("[%s-%d]: ARM will slp...\n", __func__, __LINE__);
        int fd = -1;

        fd = open("/sys/power/state", O_RDWR);
        if (fd < 0)
        {
            printf("open fd faild fd = %d\n", fd);
        }
        write(fd, "mem\n", 5);
        printf("[%s-%d]: ARM wake up...\n", __func__, __LINE__);
        close(fd);
#endif

#ifdef __RTTHREAD_OS__
        extern void fh_aov_arm_sleep(void);
        fh_aov_arm_sleep();
#endif
        aov_cmd.cmd = AOV_CMD_WAKE_REASON;
        ret = FH_RPC_Customer_Command(RPC_AOV_CMD_3 | RPC_CUSTOMER_FLAG_NEED_CopyBack, &aov_cmd, sizeof(aov_cmd));
        if (ret == 0)
        {
            printf("GRPID %d AOV_CMD_WAKE_REASON = 0x%x\n", aov_cmd.data.wake_source.grpid, aov_cmd.data.wake_source.wakeup_reason);
        }
        g_aov_resume_reason = aov_cmd.data.wake_source.wakeup_reason;
        pthread_mutex_unlock(&g_aov_mutex);
        if ((g_aov_resume_reason & AOV_WAKEUP_USER))
        {
            printf("[%s-%d]: wake up by user!!!!\n", __func__, __LINE__);
            linux_aov_exit();
            sleep(30);
        }
        else
        {
            sleep(10);
        }

    }
    FH_RPC_Customer_DeInit();
}
#endif /* FH_USING_AOV_DEMO */

void *_vlcview(void *arg)
{
    FH_SINT32 ret;
    FH_SINT32 i;
    int index = 0;
    int code = 0;
    int curgrp = 0;
    int pesgrp = 0;

    ret = FH_SYS_Init();
    if (ret != RETURN_OK)
    {
        printf("Error: FH_SYS_Init failed with %x\n", ret);
        goto err_exit;
    }

#ifdef FH_USING_STREAM_UDP
    ret = libpes_init();
    if (ret != 0)
    {
        printf("Error: libpes_init failed with %d\n", ret);
        goto err_exit;
    }
#endif

#ifdef OPEN_RECORD
    ret = librecord_init();
    if (ret != 0)
    {
        printf("Error: librecord_init failed with %d\n", ret);
        goto err_exit;
    }
#endif

    pthread_create(&g_thread_stream, NULL, sample_vlcview_get_stream_proc, NULL);

#ifdef FH_USING_STREAM_UDP
    while (index < MAX_GRP_NUM)
    {
        pesgrp |= 1 << index;
        index++;
    }

    code = 0x1;

    if (pesgrp > MAX_PES_CHANNEL_COUNT)
    {
        printf("Error: channel num is larger than %d\n", MAX_PES_CHANNEL_COUNT);
        return -1;
    }

    while (curgrp <= MAX_GRP_NUM)
    {
        if (pesgrp & code)
        {
            for (i = curgrp * MAX_VPU_CHN_NUM; i < (curgrp + 1) * MAX_VPU_CHN_NUM; i++)
            {
                libpes_send_to_vlc(i, g_aov_config.ip, g_aov_config.port + i);
            }
        }
        code <<= 1;
        curgrp++;
    }
#endif

#ifdef FH_USING_AOV_DEMO
    pthread_create(&g_thread_aov, NULL, aov_main, NULL);
#endif

#ifdef OPEN_COOLVIEW
    extern FH_SINT32 sample_common_start_coolview(FH_VOID * arg);
    sample_common_start_coolview(NULL);
#endif
    printf("rpc video demo runing\n");

    if (g_thread_stream)
    {
        pthread_join(g_thread_stream, NULL);
        g_thread_stream = 0;
    }
#ifdef FH_USING_AOV_DEMO
    if (g_thread_aov)
    {
        pthread_join(g_thread_aov, NULL);
        g_thread_aov = 0;
    }
#endif

    g_stop_running = FH_FALSE;

    return NULL;
err_exit:
    g_stop_running = FH_TRUE;
#ifdef FH_USING_STREAM_UDP
    libpes_uninit();
#endif
#ifdef OPEN_RECORD
    librecord_uninit();
#endif

    return NULL;
}
