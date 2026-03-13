#include "nna_app/include/sample_common_nna.h"
#include "fhalg_yolox_post.h"

#define AINR_ON_FORMAT 0x1200               // low fps
#define AINR_OFF_FORMAT FORMAT_2688X1520P20 // high fps 0x1202

static char *model_name = MODEL_TAG_DEMO_NNA_AINR;

static FH_UINT32 g_nna_grpid[MAX_GRP_NUM] = {0};
static FH_SINT32 g_nna_running[MAX_GRP_NUM] = {0};
static FH_SINT32 g_nna_stop[MAX_GRP_NUM] = {0};

static FH_SINT32 g_switch_ainr_running = 0;
static FH_SINT32 g_switch_ainr_stop = 0;
static FH_SINT32 g_ainr_on_format[MAX_GRP_NUM] = {0};
static FH_SINT32 g_ainr_off_format[MAX_GRP_NUM] = {0};

static FH_SINT32 draw_box(FH_SINT32 grp_id, FH_SINT32 chn_id, NN_RESULT nn_result, FH_FLOAT conf)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;
    FH_UINT32 chn_w = 0;
    FH_UINT32 chn_h = 0;
    static OSD_GBOX gbox[BOX_MAX_NUM];
    FH_SINT32 i = 0;
    static NN_RESULT last_result = {0};

    if (nn_result.result_det.nn_result_num == 0)
    {
        for (i = 0; i < last_result.result_det.nn_result_num; i++)
        {
            gbox[i].enable = 0;

            ret = sample_set_gbox(grp_id, chn_id, &gbox[i]);
            SDK_FUNC_ERROR_CONTINUE(model_name, ret);
        }

        last_result.result_det.nn_result_num = 0;

        return FH_SDK_SUCCESS;
    }

    SDK_CHECK_MAX_VALID(model_name, nn_result.result_det.nn_result_num, BOX_MAX_NUM, "nn_result_num:[%d] > BOX_MAX_NUM:[%d]\n", nn_result.result_det.nn_result_num, BOX_MAX_NUM);

    chn_w = get_vpu_chn_w(grp_id, chn_id);
    chn_h = get_vpu_chn_h(grp_id, chn_id);

    for (i = 0; i < nn_result.result_det.nn_result_num; i++)
    {
        gbox[i].gbox_id = i;
        gbox[i].color.fAlpha = 255;
        gbox[i].color.fRed = 255;
        gbox[i].color.fGreen = 0;
        gbox[i].color.fBlue = 0;
        gbox[i].x = nn_result.result_det.nn_result_info[i].box.x * chn_w;
        gbox[i].y = nn_result.result_det.nn_result_info[i].box.y * chn_h;
        gbox[i].w = nn_result.result_det.nn_result_info[i].box.w * chn_w;
        gbox[i].h = nn_result.result_det.nn_result_info[i].box.h * chn_h;

        if (nn_result.result_det.nn_result_info[i].conf < conf)
        {
            gbox[i].enable = 0;
        }
        else
        {
            gbox[i].enable = 1;
        }

        ret = sample_set_gbox(grp_id, chn_id, &gbox[i]);
        SDK_FUNC_ERROR_CONTINUE(model_name, ret);
    }

    if (last_result.result_det.nn_result_num > nn_result.result_det.nn_result_num)
    {
        for (; i < last_result.result_det.nn_result_num; i++)
        {
            gbox[i].enable = 0;

            ret = sample_set_gbox(grp_id, chn_id, &gbox[i]);
            SDK_FUNC_ERROR_CONTINUE(model_name, ret);
        }
    }

    last_result.result_det.nn_result_num = nn_result.result_det.nn_result_num;

    return ret;
}

static FH_SINT32 switch_ainr_run(FH_SINT32 grp_id)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;
    ISP_INFO *isp_info;
    FH_CHAR *param;
    FH_SINT32 day_flag = 0;
    FH_SINT32 night_flag = 0;
    FH_SINT32 flag = 0;
    FH_SINT32 wdr_mode = 0;
    FH_SINT32 hex_file_len = 0;

    if (grp_id < 0)
    {
        return FH_SDK_SUCCESS;
    }

    isp_info = get_isp_config(grp_id);
    if (!isp_info->enable)
    {
        SDK_ERR_PRT(model_name, "switch_ainr_run Failed! ISP[%d] not enable!\n", grp_id);
        return FH_SDK_FAILED;
    }

    if (!isp_info->ainr_2d_enable)
    {
        SDK_ERR_PRT(model_name, "AIISP[%d] not enable !\n", grp_id);
        return FH_SDK_FAILED;
    }

    isp_info->aiisp_enable = !isp_info->aiisp_enable;

    if (isp_info->aiisp_enable) // night[on]
    {
        isp_info->isp_format = g_ainr_on_format[grp_id];
        wdr_mode = get_sensorFormat_wdr_mode(isp_info->isp_format);
        if (wdr_mode)
        {
            night_flag = SAMPLE_SENSOR_FLAG_WDR_NIGHT;
        }
        else
        {
            night_flag = SAMPLE_SENSOR_FLAG_NIGHT;
        }
        flag = night_flag;

        param = getSensorHex(grp_id, isp_info->sensor->name, flag, &hex_file_len);
        if (param)
        {
            ret = API_ISP_LoadIspParam(grp_id, param);
            SDK_FUNC_ERROR(model_name, ret);
            freeSensorHex(param);
        }
        else
        {
            SDK_PRT(model_name, "Error: Cann't load sensor hex file!\n");
        }

        // only switch aiisp
        ret = change_ai_isp_status(grp_id, isp_info->aiisp_enable);
        SDK_FUNC_ERROR(model_name, ret);

        // change format and switch aiisp
        // ret = change_ai_isp_format(grp_id, isp_info->aiisp_enable);
        // SDK_FUNC_ERROR(model_name, ret);

        ret = ctrl_fps_from_AE(grp_id, -1);
        SDK_FUNC_ERROR(model_name, ret);

        SDK_PRT(model_name, "AINR ON!\n");
    }
    else // day[off]
    {
        isp_info->isp_format = g_ainr_off_format[grp_id];
        wdr_mode = get_sensorFormat_wdr_mode(isp_info->isp_format);
        if (wdr_mode)
        {
            day_flag = SAMPLE_SENSOR_FLAG_WDR;
        }
        else
        {
            day_flag = SAMPLE_SENSOR_FLAG_NORMAL;
        }
        flag = day_flag;

        param = getSensorHex(grp_id, isp_info->sensor->name, flag, &hex_file_len);
        if (param)
        {
            ret = API_ISP_LoadIspParam(grp_id, param);
            SDK_FUNC_ERROR(model_name, ret);
            freeSensorHex(param);
        }
        else
        {
            SDK_PRT(model_name, "Error: Cann't load sensor hex file!\n");
        }

        // only switch aiisp
        ret = change_ai_isp_status(grp_id, isp_info->aiisp_enable);
        SDK_FUNC_ERROR(model_name, ret);

        // change format and switch aiisp
        // ret = change_ai_isp_format(grp_id, isp_info->aiisp_enable);
        // SDK_FUNC_ERROR(model_name, ret);

        ret = ctrl_fps_from_AE(grp_id, -1);
        SDK_FUNC_ERROR(model_name, ret);

        SDK_PRT(model_name, "AINR OFF!\n");
    }

    return ret;
}

FH_VOID *switch_ainr(FH_VOID *args)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;
    FH_CHAR cmd[20];
    FH_SINT32 grp_id = -1;

    if (g_switch_ainr_running)
    {
        return NULL;
    }
    g_switch_ainr_running = 1;

    for (int i = 0; i < MAX_GRP_NUM; i++)
    {
        g_ainr_on_format[i] = AINR_ON_FORMAT;
        g_ainr_off_format[i] = AINR_OFF_FORMAT;
    }
    prctl(PR_SET_NAME, "switch_ainr");
    sleep(1);
    while (!g_switch_ainr_stop)
    {
        printf("Enter '0','1','exit' to switch GRP0 or GRP1 ainr!!!\n");
        scanf("%s", cmd);
        if (strcmp(cmd, "0") == 0)
        {
            grp_id = 0;
        }
        else if (strcmp(cmd, "1") == 0)
        {
            grp_id = 1;
        }
        else if (strcmp(cmd, "exit") == 0)
        {
            grp_id = -1;
            break;
        }
        else
        {
            grp_id = -1;
            SDK_PRT(model_name, "Print Cmd Error!\n");
        }
        ret = switch_ainr_run(grp_id);
        SDK_FUNC_ERROR_PRT(model_name, ret);
    }

    SDK_PRT(model_name, "ainr switch_ainr[%d] End!\n", grp_id);
    g_switch_ainr_running = 0;

    return NULL;
}

FH_VOID *query_cv(FH_VOID *args)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;
    FH_SINT32 grp_id = *((FH_SINT32 *)args);
    FH_CHAR thread_name[20];
    NN_RESULT nn_result;
    VPU_INFO *vpu_info;
    VPU_CHN_INFO *vpu_chn_info;
    ISP_INFO *isp_info;
    FH_AIISP_CV_PARAMS params;
    FH_AIISP_CV_OUT out;
    FH_VOID *nn_handle;
    FH_VIDEO_FRAME pstVpuframeinfo;
    FH_UINT32 handle_lock;
    FH_UINT64 time_start = 0;
    FH_UINT64 time_now = 0;
    FH_UINT64 time_bak = 0;
    FH_UINT64 time_lapse = 0;
    FH_FLOAT fps = 0;
    T_TY_ModelDesc model_desc;
    FH_VOID *nn_post_handle;
    NnPostAnchorfreeInitParam nn_post_cfg;
    FH_ANCHORFREE_DET_BBOXES_T outdet;

    vpu_info = get_vpu_config(grp_id);

    if (vpu_info->enable)
    {
        vpu_chn_info = get_vpu_chn_config(grp_id, NN_CHN);
        if (!vpu_chn_info->enable)
        {
            SDK_ERR_PRT(model_name, "VPU[%d] CHN[%d] not enable !\n", grp_id, NN_CHN);
            goto Exit;
        }
    }
    else
    {
        SDK_ERR_PRT(model_name, "VPU GRP[%d] not enable !\n", grp_id);
        goto Exit;
    }

    isp_info = get_isp_config(grp_id);
    if (!isp_info->enable)
    {
        SDK_ERR_PRT(model_name, "ISP GRP[%d] not enable !\n", grp_id);
        goto Exit;
    }

    if (!isp_info->ainr_cv_enable)
    {
        SDK_ERR_PRT(model_name, "AIISP[%d] CV not enable !\n", grp_id);
        goto Exit;
    }

    params.model_cv_path = isp_info->ainr_cv_model;
    params.type = AIISP_C2DET;
    ret = FH_CV_Create(&nn_handle, params);
    SDK_FUNC_ERROR_GOTO(model_name, ret);

    ret = FH_CV_Get_ModuleDesc(nn_handle, &model_desc);
    SDK_FUNC_ERROR_GOTO(model_name, ret);

    nn_post_cfg.num_of_fm_scales = 3;         // 3
    nn_post_cfg.conf_thresh = 0.4;            // 置信度
    nn_post_cfg.nms_thresh = 0.45;            // 0.45
    nn_post_cfg.num_of_classes = 2;           // 2分类
    nn_post_cfg.src_h = vpu_chn_info->height; // AI CHN宽高
    nn_post_cfg.src_w = vpu_chn_info->width;
    nn_post_cfg.rotate = FN_POST_ROT0;
    // nn_post_cfg.net_type = FN_DET_TYPE_C2;
    nn_post_cfg.net_type = FN_DET_TYPE_C2_CPU;
    nn_post_cfg.model_desc = &model_desc;

    ret = FH_NNPost_ANCHORFREE_Create(&nn_post_handle, nn_post_cfg);
    SDK_FUNC_ERROR_GOTO(model_name, ret);

    SDK_PRT(model_name, "CV Set FPS %d\n", isp_info->ainr_cv_fps);
    time_lapse = 1000 / isp_info->ainr_cv_fps;
    g_nna_running[grp_id] = 1;

    time_start = time_bak = time_now = get_ms();

    memset(thread_name, 0, sizeof(thread_name));
    sprintf(thread_name, "query_cv_%d", grp_id);
    prctl(PR_SET_NAME, thread_name);
    while (!g_nna_stop[grp_id])
    {
        ret = lock_vpu_stream(grp_id, NN_CHN, &pstVpuframeinfo, 500, &handle_lock, 1);
        SDK_FUNC_ERROR_CONTINUE(model_name, ret);
        time_now = get_ms();
        if (time_now - time_bak > time_lapse)
        {
            time_bak += time_lapse;
            fps++;

            ret = FH_CV_Process(nn_handle, &pstVpuframeinfo, &out, 1 * 1000);
            if (ret)
            {
                SDK_FUNC_ERROR_PRT(model_name, ret);

                ret = unlock_vpu_stream(grp_id, NN_CHN, handle_lock, &pstVpuframeinfo);
                SDK_FUNC_ERROR_CONTINUE(model_name, ret);
                continue;
            }

            ret = unlock_vpu_stream(grp_id, NN_CHN, handle_lock, &pstVpuframeinfo);
            SDK_FUNC_ERROR_CONTINUE(model_name, ret);

            ret = FH_NNPost_ANCHORFREE_Process(nn_post_handle, out.taskOutVec, &outdet);
            SDK_FUNC_ERROR_PRT(model_name, ret);

            nn_result.result_det.nn_result_num = outdet.bbox_num;
            nn_result.net_type = PERSON_VEHICLE_DET;
            if (nn_result.result_det.nn_result_num > NN_RESULT_NUM)
            {
                SDK_ERR_PRT(model_name, "nn_result.result_det.nn_result_num[%d] > NN_RESULT_NUM[%d!\n", nn_result.result_det.nn_result_num, NN_RESULT_NUM);
            }
            else
            {
                for (int i = 0; i < outdet.bbox_num; i++)
                {
                    nn_result.result_det.nn_result_info[i].class_id = outdet.bbox[i].class_id;
                    nn_result.result_det.nn_result_info[i].box.x = outdet.bbox[i].x;
                    nn_result.result_det.nn_result_info[i].box.y = outdet.bbox[i].y;
                    nn_result.result_det.nn_result_info[i].box.w = outdet.bbox[i].w;
                    nn_result.result_det.nn_result_info[i].box.h = outdet.bbox[i].h;
                    nn_result.result_det.nn_result_info[i].conf = outdet.bbox[i].conf;
#if NN_DEBUG
                    SDK_PRT(model_name, "[%d] class_id = %d\n", i, nn_result.result_det.nn_result_info[i].class_id);
                    SDK_PRT(model_name, "[%d] x = %f\n", i, nn_result.result_det.nn_result_info[i].box.x);
                    SDK_PRT(model_name, "[%d] y = %f\n", i, nn_result.result_det.nn_result_info[i].box.y);
                    SDK_PRT(model_name, "[%d] w = %f\n", i, nn_result.result_det.nn_result_info[i].box.w);
                    SDK_PRT(model_name, "[%d] h = %f\n", i, nn_result.result_det.nn_result_info[i].box.h);
                    SDK_PRT(model_name, "[%d] conf = %f\n", i, nn_result.result_det.nn_result_info[i].conf);
#endif
                }

                if (!ret)
                {
                    ret = draw_box(grp_id, 0, nn_result, 0.4f);
                    SDK_FUNC_ERROR_PRT(model_name, ret);
                }
            }

            if (time_now - time_start > 10 * 1000)
            {
                SDK_PRT(model_name, "NN[%d] Det FPS: %f\n", grp_id, fps / 10);
                fps = 0;
                time_start = time_now;
            }

            ret = FH_CV_Release(nn_handle, &out);
            SDK_FUNC_ERROR_CONTINUE(model_name, ret);
        }
        else
        {
            ret = unlock_vpu_stream(grp_id, NN_CHN, handle_lock, &pstVpuframeinfo);
            SDK_FUNC_ERROR_CONTINUE(model_name, ret);
        }
    }

Exit:
    ret = FH_NNPost_ANCHORFREE_Destroy(nn_post_handle);
    SDK_FUNC_ERROR_PRT(model_name, ret);
    nn_post_handle = NULL;

    ret = FH_CV_Destory(&nn_handle);
    SDK_FUNC_ERROR_PRT(model_name, ret);

    SDK_PRT(model_name, "ainr query_cv[%d] End!\n", grp_id);
    g_nna_running[grp_id] = 0;

    return NULL;
}

FH_SINT32 sample_fh_ainr_start(FH_SINT32 grp_id)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;
    pthread_t thread;
    pthread_attr_t attr;

    if (g_nna_running[grp_id])
    {
        SDK_PRT(model_name, "ainr[%d] already running!\n", grp_id);
        return FH_SDK_SUCCESS;
    }

    g_nna_grpid[grp_id] = grp_id;
    g_nna_stop[grp_id] = 0;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
#ifdef __RTTHREAD_OS__
    pthread_attr_setstacksize(&attr, 10 * 1024);
#endif
    if (pthread_create(&thread, &attr, query_cv, &g_nna_grpid[grp_id]))
    {
        SDK_ERR_PRT(model_name, "pthread_create failed\n");
        goto Exit;
    }

    g_switch_ainr_stop = 0;
    if (pthread_create(&thread, &attr, switch_ainr, NULL))
    {
        SDK_ERR_PRT(model_name, "pthread_create failed\n");
        goto Exit;
    }

    return ret;

Exit:
    return -1;
}

FH_SINT32 sample_fh_ainr_stop(FH_SINT32 grp_id)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;

    if (g_nna_running[grp_id])
    {
        g_nna_stop[grp_id] = 1;

        while (g_nna_running[grp_id])
        {
            usleep(10 * 1000);
        }
    }
    else
    {
        SDK_PRT(model_name, "ainr[%d] not running!\n", grp_id);
    }

    if (g_switch_ainr_running)
    {
        g_switch_ainr_stop = 1;

        while (g_switch_ainr_running)
        {
            printf("Watting Enter 'exit'\n");
            sleep(1);
        }
    }

    return ret;
}
