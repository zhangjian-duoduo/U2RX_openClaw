#include "nna_app/include/sample_common_nna.h"
#include "sample_nna_lpr.h"
#include "overlay/include/overlay_tosd.h"

#define DRAW_BOX_CHAN (0)
#define CLEAR_FRAME_NUM 40
#define LPR_WIDTH 160
#define LPR_HEIGHT 32

static char *model_name = MODEL_TAG_DEMO_NNA_LPR;

static FH_UINT32 g_nna_grpid[MAX_GRP_NUM] = {0};
static FH_SINT32 g_nna_running[MAX_GRP_NUM] = {0};
static FH_SINT32 g_nna_stop[MAX_GRP_NUM] = {0};

typedef struct lpr_box
{
    FH_SINT32 id;
    FH_SINT32 last_frame;
    OSD_GBOX gbox;
} LPR_BOX;

LPR_BOX lpr_box[MAX_GRP_NUM][BOX_MAX_NUM];

static FH_SINT32 clean_box(FH_SINT32 grp_id, FH_SINT32 chn_id, NN_RESULT lpr_out)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;

    for (int i = 0; i < BOX_MAX_NUM; i++)
    {
        if (lpr_box[grp_id][i].last_frame % CLEAR_FRAME_NUM == 0)
        {
            memset(&lpr_box[grp_id][i], 0, sizeof(lpr_box[grp_id][i]));

            ret = sample_set_gbox(grp_id, chn_id, &lpr_box[grp_id][i].gbox);
            SDK_FUNC_ERROR(model_name, ret);
        }

        lpr_box[grp_id][i].last_frame++;
    }

    return ret;
}

static FH_SINT32 draw_box(FH_SINT32 grp_id, FH_UINT32 enable, FH_UINT32 box_id, FH_SINT32 chn_id, FH_UINT32 x, FH_UINT32 y, FH_UINT32 w, FH_UINT32 h)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;

    if (x < 0 || y < 0 || w < 0 || h < 0)
    {
        return FH_SDK_FAILED;
    }

    FH_UINT32 chn_w = 0;
    FH_UINT32 chn_h = 0;

    chn_w = get_vpu_chn_w(grp_id, chn_id);
    chn_h = get_vpu_chn_h(grp_id, chn_id);

    if ((x + w) > chn_w)
    {
        w = chn_w - x - 1;
    }

    if ((y + h) > chn_h)
    {
        h = chn_h - y - 1;
    }

    for (int i = 0; i < BOX_MAX_NUM; i++)
    {
        lpr_box[grp_id][i].id = i;
        lpr_box[grp_id][i].last_frame = 1;
        lpr_box[grp_id][i].gbox.gbox_id = i;
        lpr_box[grp_id][i].gbox.color.fAlpha = 255;
        lpr_box[grp_id][i].gbox.color.fRed = 255;
        lpr_box[grp_id][i].gbox.color.fGreen = 0;
        lpr_box[grp_id][i].gbox.color.fBlue = 0;
        lpr_box[grp_id][i].gbox.x = x;
        lpr_box[grp_id][i].gbox.y = y;
        lpr_box[grp_id][i].gbox.w = w;
        lpr_box[grp_id][i].gbox.h = h;
        lpr_box[grp_id][i].gbox.enable = 1;

        ret = sample_set_gbox(grp_id, chn_id, &lpr_box[grp_id][i].gbox);
        SDK_FUNC_ERROR(model_name, ret);

        return ret;
    }
    return ret;
}

static FH_SINT32 lp_show_on_osd(FH_SINT32 channel, char *text_data, FH_UINT32 chn_w, FH_UINT32 chn_h)
{
    FH_SINT32 ret;
    FHT_OSD_CONFIG_t osd_cfg;
    FHT_OSD_Layer_Config_t pOsdLayerInfo[1];
    FHT_OSD_TextLine_t text_line_cfg[1];
    FHT_OSD_Ex_TextLine_t TextCfg[1];

    memset(&osd_cfg, 0, sizeof(osd_cfg));
    memset(&text_line_cfg[0], 0, sizeof(FHT_OSD_TextLine_t));
    memset(&pOsdLayerInfo[0], 0, sizeof(FHT_OSD_Layer_Config_t));
    memset(&TextCfg[0], 0, sizeof(FHT_OSD_Ex_TextLine_t));

    /* 我们的demo中只演示了一个行块 */
    osd_cfg.nOsdLayerNum = 1;
    osd_cfg.pOsdLayerInfo = &pOsdLayerInfo[0];

    /* 不旋转 */
    osd_cfg.osdRotate = 0;

    /* 设置字符大小,像素单位 */
    pOsdLayerInfo[0].osdSize = OSD_FONT_DISP_SIZE;

    pOsdLayerInfo[0].layerStartX = 0;
    pOsdLayerInfo[0].layerStartY = 0;
    pOsdLayerInfo[0].layerFlag = FH_OSD_LAYER_USE_TWO_BUF; // layer双buffer，可避免乱码

    /* 设置字符颜色为白色 */
    pOsdLayerInfo[0].normalColor.fAlpha = 255;
    pOsdLayerInfo[0].normalColor.fRed = 255;
    pOsdLayerInfo[0].normalColor.fGreen = 255;
    pOsdLayerInfo[0].normalColor.fBlue = 255;

    /* 设置字符反色颜色为黑色 */
    pOsdLayerInfo[0].invertColor.fAlpha = 255;
    pOsdLayerInfo[0].invertColor.fRed = 0;
    pOsdLayerInfo[0].invertColor.fGreen = 0;
    pOsdLayerInfo[0].invertColor.fBlue = 0;

    /* 设置字符钩边颜色为黑色 */
    pOsdLayerInfo[0].edgeColor.fAlpha = 255;
    pOsdLayerInfo[0].edgeColor.fRed = 0;
    pOsdLayerInfo[0].edgeColor.fGreen = 0;
    pOsdLayerInfo[0].edgeColor.fBlue = 0;

    /* 不显示背景 */
    pOsdLayerInfo[0].bkgColor.fAlpha = 0;

    /* 钩边像素为1 */
    pOsdLayerInfo[0].edgePixel = 1;

    /* 反色控制 */
    pOsdLayerInfo[0].osdInvertEnable = FH_OSD_INVERT_DISABLE; /*disable反色功能*/
    pOsdLayerInfo[0].osdInvertThreshold.high_level = 180;
    pOsdLayerInfo[0].osdInvertThreshold.low_level = 160;

    /*layer id*/
    pOsdLayerInfo[0].layerId = 1;

    text_line_cfg[0].enable = 1;
    text_line_cfg[0].textInfo = text_data;
    text_line_cfg[0].textEnable = 1;                                     /* 使能自定义text */
    text_line_cfg[0].timeOsdEnable = 0;                                  /* 使能时间显示 */
    text_line_cfg[0].textLineWidth = (OSD_FONT_DISP_SIZE / 2) * 36;      /* 每行最多显示36个像素宽度为16的字符, 超过后自动换行 */
    text_line_cfg[0].linePositionX = chn_w / 2 - OSD_FONT_DISP_SIZE * 3; /* 左上角起始点宽度像素位置 */
    text_line_cfg[0].linePositionY = OSD_FONT_DISP_SIZE;                 /* 左上角起始点高度像素位置 */

    ret = FHAdv_Osd_Ex_SetText(channel, channel, &osd_cfg);
    if (ret != FH_SUCCESS)
    {
        if (channel == 0) /*TOSD可能位于分通道前,这样channel 1可能没有,因此就不打印出错*/
        {
            printf("FHAdv_Osd_Ex_SetText failed with %d\n", ret);
        }
        return ret;
    }

    TextCfg[0].LineNum = 1;
    TextCfg[0].Reconfig = 1;
    TextCfg[0].pLineCfg = &text_line_cfg[0];

    ret = FHAdv_Osd_Ex_SetTextLine(channel, 0, pOsdLayerInfo[0].layerId, &TextCfg[0]);
    if (ret != FH_SUCCESS)
    {
        if (channel == 0) /*TOSD可能位于分通道前,这样channel 1可能没有,因此就不打印出错*/
        {
            printf("FHAdv_Osd_Ex_SetText failed with %d\n", ret);
        }
        return ret;
    }

    return 0;
}

/* 加载字库 */
static FH_SINT32 load_font_lib(FHT_OSD_FontType_e type, FH_UINT8 *font_array, FH_SINT32 font_array_size)
{
    FH_SINT32 ret;
    FHT_OSD_FontLib_t font_lib;

    font_lib.pLibData = font_array;
    font_lib.libSize = font_array_size;
    ret = FHAdv_Osd_LoadFontLib(type, &font_lib);
    SDK_FUNC_ERROR_PRT(model_name, ret);

    return 0;
}

static FH_SINT32 sample_init_font(FH_VOID)
{
    FH_SINT32 ret;

    /* 加载gb2312字库 */
    ret = load_font_lib(FHEN_FONT_TYPE_CHINESE, gb2312_lpr, sizeof(gb2312_lpr));
    SDK_FUNC_ERROR_PRT(model_name, ret);

    /* 加载asc字库 */
    ret = load_font_lib(FHEN_FONT_TYPE_ASC, asc16_lpr, sizeof(asc16_lpr));
    SDK_FUNC_ERROR_PRT(model_name, ret);

    return ret;
}

FH_SINT32 lprec_result_is_correct(NN_RESULT *lpr_out, FH_CHAR *licensePlate)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;

    memset(licensePlate, 0, 128);

    if (lpr_out->result_lp_rec.plate_len < 3)
    {
        SDK_ERR_PRT(model_name, "lp_plate_len[%d] < 3\n", lpr_out->result_lp_rec.plate_len);
        return FH_SDK_FAILED;
    }
    else
    {
        for (int i = 0; i < lpr_out->result_lp_rec.plate_len; i++)
        {
            int index = lpr_out->result_lp_rec.val_dec[i];
            strncat(licensePlate, lpr_char_1[index], 2);
            if (lpr_out->result_lp_rec.prob_dec[i] < 0.8) // 0.8
            {
                SDK_ERR_PRT(model_name, "prob[%d] %f < 0.8\n", i, lpr_out->result_lp_rec.prob_dec[i]);
                return FH_SDK_FAILED;
            }
        }
    }

    return ret;
}

FH_SINT32 get_lprec_input(FHA_LPRECTIFY_PROCESSOR processor, NN_INPUT_DATA lpd_input, NN_INPUT_DATA *lprec_input, NN_RESULT det_out, FH_UINT32 chn_w, FH_UINT32 chn_h, int i, FH_SINT32 grp_id)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;

    FHA_IMAGE_s algo_src;
    FHA_IMAGE_s algo_dst;
    FHA_LPD_LANDMARK_s input_pts;

    FH_SINT32 dt_x = 0;
    FH_SINT32 dt_y = 0;
    FH_SINT32 dt_w = 0;
    FH_SINT32 dt_h = 0;
    FH_FLOAT chn = 0.0f;
    FH_UINT32 stride_coef = 0;

    ret = nna_algo_format(lpd_input.imageType, &algo_src.imageType, &chn, &stride_coef);
    SDK_FUNC_ERROR(model_name, ret);

    if (lprec_input->data.vbase == NULL)
    {
        ret = buffer_malloc_withname(&lprec_input->data, LPR_WIDTH * LPR_HEIGHT * chn, 1024, "lprec_input_buffer");
        SDK_FUNC_ERROR_GOTO(model_name, ret);
    }

    memset(lprec_input->data.vbase, 0, lprec_input->data.size);

    algo_src.width = lpd_input.width;
    algo_src.height = lpd_input.height;
    algo_src.stride = lpd_input.width * stride_coef;
    algo_src.data = (FH_UINT8 *)lpd_input.data.vbase;

    algo_dst.width = LPR_WIDTH;
    algo_dst.height = LPR_HEIGHT;
    algo_dst.stride = LPR_WIDTH * stride_coef;
    algo_dst.imageType = algo_src.imageType;
    algo_dst.data = (FH_UINT8 *)lprec_input->data.vbase;

    for (int j = 0; j < FH_LP_LANDMARK_PTS_NUM; j++)
    {
        input_pts.point[j].x = det_out.result_lp_det.detBBox[i].landmark[j].x * lpd_input.width;
        input_pts.point[j].y = det_out.result_lp_det.detBBox[i].landmark[j].y * lpd_input.height;
    }

    dt_x = det_out.result_lp_det.detBBox[i].box.x * chn_w;
    dt_y = det_out.result_lp_det.detBBox[i].box.y * chn_h;
    dt_w = det_out.result_lp_det.detBBox[i].box.w * chn_w;
    dt_h = det_out.result_lp_det.detBBox[i].box.h * chn_h;

    ret = FHA_PLATE_RECTIFY_Process(processor, &algo_src, &input_pts, &algo_dst);
    SDK_FUNC_ERROR_PRT(model_name, ret);

    draw_box(grp_id, 1, (FH_LP_LANDMARK_PTS_NUM + 1) - 1, 0, dt_x, dt_y, dt_w, dt_h);

    lprec_input->width = LPR_WIDTH;
    lprec_input->height = LPR_HEIGHT;
    lprec_input->stride = LPR_WIDTH;
    lprec_input->imageType = algo_dst.imageType;
    lprec_input->frame_id = lpd_input.frame_id;
    lprec_input->pool_id = -1;
    lprec_input->timestamp = algo_dst.timestamp;

    return ret;
Exit:

    return FH_SDK_FAILED;
}

FH_VOID *nna_lp_rec(FH_VOID *args)
{
    FH_SINT32 ret;
    FH_UINT32 chn_w = 0;
    FH_UINT32 chn_h = 0;
    NN_HANDLE detHandle = NULL;
    NN_HANDLE lprHandle = NULL;
    FH_CHAR model_path[256];
    VPU_INFO *vpu_info = NULL;
    VPU_CHN_INFO *vpu_chn_info = NULL;
    VPU_CHN_INFO *vpu_chn0_info = NULL;
    NN_TYPE nn_type;
    NN_INPUT_DATA lpd_input;
    NN_INPUT_DATA lpr_input;
    NN_RESULT det_out;
    NN_RESULT lpr_out;
    FH_VIDEO_FRAME rgbStr;
    FH_SINT32 grp_id = *((FH_SINT32 *)args);
    FH_UINT32 handle_lock = 0;
    FH_CHAR thread_name[20];
    FH_CHAR licensePlate[128];
    FHA_LPRECTIFY_PROCESSOR processor = NULL;

    vpu_info = get_vpu_config(grp_id);

    if (vpu_info->enable)
    {
        vpu_chn_info = get_vpu_chn_config(grp_id, NN_CHN);
        if (!vpu_chn_info->enable)
        {
            SDK_ERR_PRT(model_name, "VPU[%d] CHN[%d] not enable!\n", grp_id, NN_CHN);
            goto Exit;
        }
    }
    else
    {
        SDK_ERR_PRT(model_name, "VPU GRP[%d] not enable!\n", grp_id);
        goto Exit;
    }
    if (vpu_info->enable)
    {
        vpu_chn0_info = get_vpu_chn_config(grp_id, DRAW_BOX_CHAN);
        if (!vpu_chn_info->enable)
        {
            SDK_ERR_PRT(model_name, "VPU[%d] CHN[%d] not enable!\n", grp_id, DRAW_BOX_CHAN);
            goto Exit;
        }
    }
    else
    {
        SDK_ERR_PRT(model_name, "VPU GRP[%d] not enable!\n", grp_id);
        goto Exit;
    }

    chn_w = vpu_chn0_info->width;
    chn_h = vpu_chn0_info->height;

    ret = sample_init_font();
    SDK_FUNC_ERROR_GOTO(model_name, ret);

    nn_type = LP_DET;
    memset(&model_path, 0, sizeof(model_path));
    strcpy(model_path, "./models/lpdet.nbg");

    ret = nna_init(grp_id, &detHandle, nn_type, model_path, vpu_chn_info->width, vpu_chn_info->height);
    SDK_FUNC_ERROR_GOTO(model_name, ret);

    nn_type = LP_REC;
    memset(&model_path, 0, sizeof(model_path));
    strcpy(model_path, "./models/lprec.nbg");

    ret = nna_init(grp_id, &lprHandle, nn_type, model_path, LPR_WIDTH, LPR_HEIGHT);
    SDK_FUNC_ERROR_GOTO(model_name, ret);

    memset(&lpd_input, 0, sizeof(lpd_input));
    lpd_input.width = vpu_chn_info->width;
    lpd_input.height = vpu_chn_info->height;
    lpd_input.stride = vpu_chn_info->width;

    processor = FHA_PLATE_RECTIFY_Create();
    SDK_CHECK_NULL_PTR(model_name, processor);

    g_nna_running[grp_id] = 1;

    sprintf(thread_name, "nna_lp_rec_%d", grp_id);
    prctl(PR_SET_NAME, thread_name);
    while (!g_nna_stop[grp_id])
    {
        ret = lock_vpu_stream(grp_id, NN_CHN, &rgbStr, 1000, &handle_lock, 0);
        SDK_FUNC_ERROR_CONTINUE(model_name, ret);

        ret = getVPUStreamToNN(rgbStr, &lpd_input);
        if (!ret)
        {
            ret = get_lpdet_result(detHandle, lpd_input, &det_out);
            SDK_FUNC_ERROR_CONTINUE(model_name, ret);

            for (int i = 0; i < det_out.result_lp_det.boxNum; i++)
            {
                ret = get_lprec_input(processor, lpd_input, &lpr_input, det_out, chn_w, chn_h, i, grp_id);
                SDK_FUNC_ERROR_CONTINUE(model_name, ret);

                ret = get_lprec_result(lprHandle, lpr_input, &lpr_out);
                SDK_FUNC_ERROR_CONTINUE(model_name, ret);

                if (!lprec_result_is_correct(&lpr_out, licensePlate))
                {
                    ret = lp_show_on_osd(DRAW_BOX_CHAN, licensePlate, chn_w, chn_h);
                    SDK_FUNC_ERROR_PRT(model_name, ret);
                }
                else
                {
                    memset(licensePlate, 0, 128);
                    ret = lp_show_on_osd(DRAW_BOX_CHAN, licensePlate, chn_w, chn_h);
                    SDK_FUNC_ERROR_PRT(model_name, ret);
                }
            }

            ret = clean_box(grp_id, DRAW_BOX_CHAN, lpr_out);
            SDK_FUNC_ERROR_CONTINUE(model_name, ret);
        }

        ret = unlock_vpu_stream(grp_id, NN_CHN, handle_lock, &rgbStr);
        SDK_FUNC_ERROR_CONTINUE(model_name, ret);
    }

Exit:
    ret = nna_release(grp_id, &detHandle);
    SDK_FUNC_ERROR_PRT(model_name, ret);

    ret = nna_release(grp_id, &lprHandle);
    SDK_FUNC_ERROR_PRT(model_name, ret);

    if (processor)
    {
        ret = FHA_PLATE_RECTIFY_Destory(processor);
        SDK_FUNC_ERROR_PRT(model_name, ret);
    }

    SDK_PRT(model_name, "nna_lp_rec[%d] End!\n", grp_id);
    g_nna_running[grp_id] = 0;
    return NULL;
}

FH_SINT32 sample_fh_nna_lp_rec_start(FH_SINT32 grp_id)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;

    pthread_t nn_thread;
    pthread_attr_t nn_attr;

    if (g_nna_running[grp_id])
    {
        SDK_PRT(model_name, "nna_lpr[%d] already running!\n", grp_id);

        return FH_SDK_SUCCESS;
    }

    g_nna_grpid[grp_id] = grp_id;
    g_nna_stop[grp_id] = 0;

    pthread_attr_init(&nn_attr);
    pthread_attr_setdetachstate(&nn_attr, PTHREAD_CREATE_DETACHED);
#ifdef __RTTHREAD_OS__
    pthread_attr_setstacksize(&nn_attr, 256 * 1024);
#endif
    if (pthread_create(&nn_thread, &nn_attr, nna_lp_rec, &g_nna_grpid[grp_id]))
    {
        SDK_PRT(model_name, "nn_thread_create failed!\n");
        goto Exit;
    }

    return ret;
Exit:
    return FH_SDK_FAILED;
}

FH_SINT32 sample_fh_nna_lp_rec_stop(FH_SINT32 grp_id)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;

    if (g_nna_running[grp_id])
    {
        g_nna_stop[grp_id] = 1;

        while (g_nna_running[grp_id])
        {
            usleep(10);
        }
    }
    else
    {
        SDK_PRT(model_name, "nna_lpr_detect nna[%d] not running!\n", grp_id);
    }

    return ret;
}