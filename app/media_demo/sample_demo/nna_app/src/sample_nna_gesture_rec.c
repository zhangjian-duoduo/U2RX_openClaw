#include "nna_app/include/sample_common_nna.h"

#define CLEAR_FRAME_NUM 40
#define GES_REC_W 64
#define GES_REC_H 64

static char *model_name = MODEL_TAG_DEMO_NNA_GES_REC;

static FH_UINT32 g_nna_grp_id[MAX_GRP_NUM] = {0};
static FH_UINT32 g_nna_running[MAX_GRP_NUM] = {0};
static FH_UINT32 g_nna_stop[MAX_GRP_NUM] = {0};

typedef struct gesture_box
{
    FH_SINT32 box_id;
    FH_SINT32 last_frame;
    OSD_GBOX gbox;
} GES_REC_BOX;

GES_REC_BOX ges_box[MAX_GRP_NUM][BOX_MAX_NUM];

static FH_SINT32 draw_box(FH_SINT32 grp_id, FH_SINT32 chn_id, NN_RESULT gesdet_out)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;
    FH_UINT32 chn_w = 0;
    FH_UINT32 chn_h = 0;

    chn_w = get_vpu_chn_w(grp_id, chn_id);
    chn_h = get_vpu_chn_h(grp_id, chn_id);

    for (int i = 0; i < BOX_MAX_NUM; i++)
    {
        ges_box[grp_id][i].box_id = i;
        ges_box[grp_id][i].last_frame = 1;
        ges_box[grp_id][i].gbox.gbox_id = i;
        ges_box[grp_id][i].gbox.color.fAlpha = 255;
        ges_box[grp_id][i].gbox.color.fRed = 255;
        ges_box[grp_id][i].gbox.color.fGreen = 0;
        ges_box[grp_id][i].gbox.color.fBlue = 0;
        ges_box[grp_id][i].gbox.x = gesdet_out.result_gesture_det.nn_result_info[i].box.x * chn_w;
        ges_box[grp_id][i].gbox.y = gesdet_out.result_gesture_det.nn_result_info[i].box.y * chn_h;
        ges_box[grp_id][i].gbox.w = gesdet_out.result_gesture_det.nn_result_info[i].box.w * chn_w;
        ges_box[grp_id][i].gbox.h = gesdet_out.result_gesture_det.nn_result_info[i].box.h * chn_h;
        ges_box[grp_id][i].gbox.enable = 1;

        ret = sample_set_gbox(grp_id, chn_id, &ges_box[grp_id][i].gbox);
        SDK_FUNC_ERROR(model_name, ret);

        return ret;
    }

    return ret;
}

static FH_SINT32 clean_box(FH_SINT32 grp_id, FH_UINT32 chn_id)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;

    for (int i = 0; i < BOX_MAX_NUM; i++)
    {
        if (ges_box[grp_id][i].last_frame % CLEAR_FRAME_NUM == 0)
        {
            memset(&ges_box[grp_id][i], 0, sizeof(ges_box[grp_id][i]));

            ret = sample_set_gbox(grp_id, chn_id, &ges_box[grp_id][i].gbox);
            SDK_FUNC_ERROR(model_name, ret);
        }

        ges_box[grp_id][i].last_frame++;
    }

    return ret;
}

static FH_SINT32 chooseFormat(NN_IMAGE_TYPE nn_image_type, FHA_IMAGE_TYPE_e *imageType, FH_SINT32 *chn)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;

    *imageType = 0;
    *chn = 0;

    switch (nn_image_type)
    {
    case IMAGE_TYPE_RGB888:
        *imageType = FH_IMAGE_FORMAT_RGBA;
        *chn = 4;
        break;
    case IMAGE_TYPE_RRGGBB:
        *imageType = FH_IMAGE_FORMAT_RGB_PLANE;
        *chn = 3;
        break;
    default:
        SDK_ERR_PRT(model_name, "NNA imageType[%d] not found!\n", *imageType);
        break;
    }

    if (*imageType == 0)
    {
        ret = FH_SDK_FAILED;
    }

    return ret;
}

static FH_UINT8 ges_rec_is_correct(NN_RESULT gesrec_out)
{
    FH_UINT32 ret = FH_SDK_SUCCESS;

    if (gesrec_out.result_gesture_rec.conf < 0.7)
    {
        SDK_PRT(model_name, "rec's conf is too low! conf is %f\n", gesrec_out.result_gesture_rec.conf);
        return ret;
    }
    else
    {
        switch (gesrec_out.result_gesture_rec.type_info)
        {
        case GESTURE_UNDEFINED:
        case GESTURE_FOUR:
        case GESTURE_PEACE:
            SDK_PRT(model_name, "it's undefined symbol!\n");
            break;
        case GESTURE_FIST:
            SDK_PRT(model_name, "it's fist!\n");
            break;
        case GESTURE_OK:
            SDK_PRT(model_name, "it's ok!\n");
            break;
        case GESTURE_STOP:
            SDK_PRT(model_name, "it's stop!\n");
            break;
        default:
            break;
        }
    }

    return ret;
}

static FH_SINT32 get_ges_rec_input(FHA_FACECROP_PROCESSOR processor, NN_RESULT_GESTURE_DET_INFO nn_result_info, NN_INPUT_DATA image_src, NN_INPUT_DATA *gesrec_input)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;

    FHA_IMAGE_s crop_src;
    FHA_IMAGE_s crop_dst;
    FHA_BOX_s crop_box = {0};

    FH_SINT32 chn = 0;

    if (gesrec_input->data.vbase == NULL)
    {
        ret = buffer_malloc_withname(&gesrec_input->data, GES_REC_W * GES_REC_H * 4, 1024, "ges_rec_input");
        SDK_FUNC_ERROR(model_name, ret);
    }

    crop_src.width = image_src.width;
    crop_src.height = image_src.height;

    ret = chooseFormat(image_src.imageType, &crop_src.imageType, &chn);
    SDK_FUNC_ERROR(model_name, ret);

    crop_src.stride = crop_src.width * chn;
    crop_src.data = (FH_UINT8 *)image_src.data.vbase;

    crop_dst.width = GES_REC_W;
    crop_dst.height = GES_REC_H;
    crop_dst.imageType = crop_src.imageType;
    crop_dst.stride = crop_dst.width * chn;
    crop_dst.data = (FH_UINT8 *)gesrec_input->data.vbase;

    crop_box.x = nn_result_info.box.x * crop_src.width;
    crop_box.y = nn_result_info.box.y * crop_src.height;
    crop_box.w = nn_result_info.box.w * crop_src.width;
    crop_box.h = nn_result_info.box.h * crop_src.height;

    ret = FHA_IMGCROP_Process(processor, &crop_src, &crop_box, &crop_dst);
    SDK_FUNC_ERROR(model_name, ret);

    gesrec_input->frame_id = image_src.frame_id;
    gesrec_input->width = crop_dst.width;
    gesrec_input->height = crop_dst.height;
    gesrec_input->stride = crop_dst.stride;
    gesrec_input->imageType = image_src.imageType;
    gesrec_input->pool_id = -1;
    gesrec_input->timestamp = image_src.timestamp;

    return ret;
}

FH_VOID *nna_gesture_rec(FH_VOID *args)
{
    FH_SINT32 ret;
    FH_SINT32 grp_id = *((FH_SINT32 *)args);
    NN_TYPE nn_type;
    FH_CHAR model_path[256];
    VPU_INFO *vpu_info = NULL;
    VPU_CHN_INFO *vpu_chn_info = NULL;
    FH_UINT32 handle_lock = 0;
    FH_CHAR thread_name[20];
    NN_HANDLE gesRecHandle = NULL;
    NN_HANDLE gesDetHandle = NULL;
    NN_INPUT_DATA gesdet_input;
    NN_RESULT gesdet_out;
    NN_INPUT_DATA gesrec_input;
    NN_RESULT gesrec_out;
    FH_VIDEO_FRAME rgbStr;
    FHA_FACECROP_PROCESSOR processor = NULL;
    FHA_FACECROP_PARAM_s param;

    vpu_info = get_vpu_config(grp_id);

    if (vpu_info->enable)
    {
        vpu_chn_info = get_vpu_chn_config(grp_id, NN_CHN);
        if (!vpu_chn_info->enable)
        {
            SDK_ERR_PRT(model_name, "VPU{%d] CHN[%d] not enable!\n", grp_id, NN_CHN);
            goto Exit;
        }
    }
    else
    {
        SDK_ERR_PRT(model_name, "VPU GRP[%d] not enable!\n", grp_id);
        goto Exit;
    }

    nn_type = GES_DET;
    memset(&model_path, 0, sizeof(model_path));
    strcpy(model_path, "./models/gesturedet.nbg");

    ret = nna_init(grp_id, &gesDetHandle, nn_type, model_path, vpu_chn_info->width, vpu_chn_info->height);
    SDK_FUNC_ERROR_GOTO(model_name, ret);

    nn_type = GES_REC;
    memset(&model_path, 0, sizeof(model_path));
    strcpy(model_path, "./models/gesturerec.nbg");

    ret = nna_init(grp_id, &gesRecHandle, nn_type, model_path, GES_REC_W, GES_REC_H);
    SDK_FUNC_ERROR_GOTO(model_name, ret);

    memset(&gesdet_input, 0, sizeof(gesdet_input));
    gesdet_input.width = vpu_chn_info->width;
    gesdet_input.height = vpu_chn_info->height;
    gesdet_input.stride = vpu_chn_info->width;

    param.expan_coef = (float)0;
    processor = FHA_IMGCROP_Create(&param);
    SDK_FUNC_ERROR_PRT(model_name, ret);

    g_nna_running[grp_id] = 1;

    memset(thread_name, 0, sizeof(thread_name));
    sprintf(thread_name, "nna_ges_rec_%d", grp_id);
    prctl(PR_SET_NAME, thread_name);
    sleep(1);

    while (!g_nna_stop[grp_id])
    {
        ret = lock_vpu_stream(grp_id, NN_CHN, &rgbStr, 1000, &handle_lock, 0);
        SDK_FUNC_ERROR_CONTINUE(model_name, ret);

        ret = getVPUStreamToNN(rgbStr, &gesdet_input);
        SDK_FUNC_ERROR_CONTINUE(model_name, ret);

        ret = nna_get_ges_det_result(gesDetHandle, gesdet_input, &gesdet_out);
        SDK_FUNC_ERROR_CONTINUE(model_name, ret);

        for (int i = 0; i < gesdet_out.result_gesture_det.nn_result_num; i++)
        {
            draw_box(grp_id, 0, gesdet_out);

            ret = get_ges_rec_input(processor, gesdet_out.result_gesture_det.nn_result_info[i], gesdet_input, &gesrec_input);
            SDK_FUNC_ERROR_CONTINUE(model_name, ret);

            ret = nna_get_ges_rec_result(gesRecHandle, gesrec_input, &gesrec_out);
            SDK_FUNC_ERROR_CONTINUE(model_name, ret);

            ret = ges_rec_is_correct(gesrec_out);
            SDK_FUNC_ERROR_CONTINUE(model_name, ret);
        }

        ret = clean_box(grp_id, 0);
        SDK_FUNC_ERROR_CONTINUE(model_name, ret);

        ret = unlock_vpu_stream(grp_id, NN_CHN, handle_lock, &rgbStr);
        SDK_FUNC_ERROR_CONTINUE(model_name, ret);
    }

Exit:
    ret = nna_release(grp_id, &gesDetHandle);
    SDK_FUNC_ERROR_PRT(model_name, ret);

    ret = nna_release(grp_id, &gesRecHandle);
    SDK_FUNC_ERROR_PRT(model_name, ret);

    ret = FHA_IMGCROP_Destory(processor);
    SDK_FUNC_ERROR_PRT(model_name, ret);

    g_nna_running[grp_id] = 0;

    return NULL;
}

FH_SINT32 sample_fh_nna_gesture_rec_start(FH_SINT32 grp_id)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;

    pthread_t nn_thread;
    pthread_attr_t nn_attr;

    if (g_nna_running[grp_id])
    {
        SDK_PRT(model_name, "nna_gesture_rec[%d] already running!\n", grp_id);

        return FH_SDK_SUCCESS;
    }

    g_nna_grp_id[grp_id] = grp_id;
    g_nna_stop[grp_id] = 0;

    pthread_attr_init(&nn_attr);
    pthread_attr_setdetachstate(&nn_attr, PTHREAD_CREATE_DETACHED);
#ifdef __RTTHREAD_OS__
    pthread_attr_setstacksize(&nn_attr, 256 * 1024);
#endif
    if (pthread_create(&nn_thread, &nn_attr, nna_gesture_rec, &g_nna_grp_id[grp_id]))
    {
        SDK_PRT(model_name, "nn_thread_create failed!\n");
        goto Exit;
    }

    return ret;

Exit:
    return FH_SDK_FAILED;
}

FH_SINT32 sample_fh_nna_gesture_rec_stop(FH_SINT32 grp_id)
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
        SDK_PRT(model_name, "nna_gesture_rec nna[%d] not running!\n", grp_id);
    }

    return ret;
}