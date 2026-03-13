#include "uvc_rtt_config.h"

// Feature config header
#include "feature_config.h"

#if ENABLE_OSD_CROSS
#include "dsp_ext/FHAdv_OSD_mpi.h"
#include "overlay_gbox.h"
#include "sample_common_overlay.h"
#endif

static char *model_name = MODEL_TAG_UVC_CONFIG;

extern void *fh_dma_memcpy(void *dst, const void *src, int len);

// Draw cross on YUV data
#if ENABLE_OSD_CROSS
static void draw_cross_yuv(FH_UINT8 *ybuf, FH_UINT8 *uvbuf, FH_SINT32 width, FH_SINT32 height)
{
    FH_SINT32 center_x = width / 2;
    FH_SINT32 center_y = height / 2;
    FH_SINT32 line_width = 10;
    FH_SINT32 i, j;
    
    // Red color in YUV: Y=81
    FH_UINT8 red_y = 81;
    
    // Draw horizontal line (red, full width)
    for (j = center_y - line_width/2; j < center_y + line_width/2 && j < height; j++)
    {
        if (j < 0) continue;
        for (i = 0; i < width; i++)
        {
            ybuf[j * width + i] = red_y;
        }
    }
    
    // Draw vertical line (red, full height)
    for (j = 0; j < height; j++)
    {
        for (i = center_x - line_width/2; i < center_x + line_width/2 && i < width; i++)
        {
            if (i < 0) continue;
            ybuf[j * width + i] = red_y;
        }
    }
}
#endif

extern void *fh_dma_memcpy(void *dst, const void *src, int len);

FH_SINT32 getVPUStreamToUVCList(FH_SINT32 grp_id, FH_SINT32 chn_id, struct uvc_dev_app *pDev)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;
    FH_VIDEO_FRAME stream;
    FH_UINT32 handle_lock;
    struct yuv_buf *yuv_buf = NULL;

    SDK_CHECK_NULL_PTR(model_name, pDev);

    ret = lock_vpu_stream(grp_id, chn_id, &stream, 1000, &handle_lock, 0);
    SDK_FUNC_ERROR(model_name, ret);

    for (int i = 0; i < UVC_YUV_BUF_NUM; i++)
    {
        yuv_buf = &pDev->yuv_buf[pDev->w_index];
        if (!yuv_buf->is_lock && !yuv_buf->is_full)
        {
            yuv_buf->is_lock = 1;
            memset(yuv_buf->data, 0, yuv_buf->bufsize);

            if (pDev->fcc == V4L2_PIX_FMT_IR)
            {
                yuv_buf->ysize = pDev->width * pDev->height;
                yuv_buf->uvsize = pDev->width * pDev->height / 2;
                fh_dma_memcpy(yuv_buf->data, stream.frm_scan.luma.data.vbase, stream.frm_scan.luma.data.size);
            }
            else if (pDev->fcc == V4L2_PIX_FMT_NV12)
            {
                yuv_buf->ysize = pDev->width * pDev->height;
                yuv_buf->uvsize = pDev->width * pDev->height / 2;
                fh_dma_memcpy(yuv_buf->data, stream.frm_scan.luma.data.vbase, stream.frm_scan.luma.data.size);
                fh_dma_memcpy(yuv_buf->data + stream.frm_scan.luma.data.size, stream.frm_scan.chroma.data.vbase, stream.frm_scan.chroma.data.size);
#if ENABLE_OSD_CROSS
                draw_cross_yuv(yuv_buf->data, yuv_buf->data + yuv_buf->ysize, pDev->width, pDev->height);
#endif
            }
            else if (pDev->fcc == V4L2_PIX_FMT_YUY2 && stream.data_format == VPU_VOMODE_YUYV)
            {
                yuv_buf->ysize = pDev->width * pDev->height;
                yuv_buf->uvsize = pDev->width * pDev->height;
                fh_dma_memcpy(yuv_buf->data, stream.frm_yuyv.data.vbase, stream.frm_yuyv.data.size);
            }
            else if (pDev->fcc == V4L2_PIX_FMT_YUY2 && stream.data_format == VPU_VOMODE_SCAN)
            {
                yuv_buf->ysize = pDev->width * pDev->height;
                yuv_buf->uvsize = pDev->width * pDev->height;
                fh_dma_memcpy(pDev->trans_buffer.data, stream.frm_scan.luma.data.vbase, yuv_buf->ysize);
                fh_dma_memcpy(pDev->trans_buffer.data + yuv_buf->ysize, stream.frm_scan.chroma.data.vbase, yuv_buf->uvsize / 2);
                nv12_to_yuy2((FH_CHAR *)pDev->trans_buffer.data, (FH_CHAR *)pDev->trans_buffer.data + yuv_buf->ysize, (FH_CHAR *)yuv_buf->data, pDev->width, pDev->height);
            }

            yuv_buf->time_stamp = stream.time_stamp;
            yuv_buf->is_full = 1;
            yuv_buf->is_lock = 0;

            pDev->w_index++;
            if (pDev->w_index == UVC_YUV_BUF_NUM)
            {
                pDev->w_index = 0;
            }

            ret = unlock_vpu_stream(grp_id, chn_id, handle_lock, &stream);
            SDK_FUNC_ERROR(model_name, ret);

            return ret;
        }

        pDev->w_index++;
        if (pDev->w_index == UVC_YUV_BUF_NUM)
        {
            pDev->w_index = 0;
        }
    }

    ret = unlock_vpu_stream(grp_id, chn_id, handle_lock, &stream);
    SDK_FUNC_ERROR(model_name, ret);

    SDK_ERR_PRT(model_name, "all buffer is full!\n");
Exit:
    return FH_SDK_FAILED;
}

FH_SINT32 getVPUStreamToUVC(FH_SINT32 grp_id, FH_SINT32 chn_id, struct uvc_dev_app *pDev)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;

    SDK_CHECK_NULL_PTR(model_name, pDev);
#ifndef ENABLE_PSRAM
    struct yuv_buf *yuv_buf = NULL;
    for (int i = 0; i < UVC_YUV_BUF_NUM; i++)
    {
        yuv_buf = &pDev->yuv_buf[pDev->r_index];
        if (!yuv_buf->is_lock && yuv_buf->is_full)
        {
            yuv_buf->is_lock = 1;

            fh_uvc_stream_pts(pDev->stream_id, yuv_buf->time_stamp);
            if (pDev->fcc == V4L2_PIX_FMT_IR)
            {
                fh_uvc_stream_enqueue(pDev->stream_id, yuv_buf->data, yuv_buf->ysize);
            }
            else
            {
                fh_uvc_stream_enqueue(pDev->stream_id, yuv_buf->data, yuv_buf->ysize + yuv_buf->uvsize);
            }

            yuv_buf->is_full = 0;
            yuv_buf->is_lock = 0;

            pDev->r_index++;
            if (pDev->r_index == UVC_YUV_BUF_NUM)
            {
                pDev->r_index = 0;
            }

            return ret;
        }
    }

    usleep(10 * 1000);
#else
    FH_VIDEO_FRAME stream;
    FH_UINT32 handle_lock;

    ret = lock_vpu_stream(grp_id, chn_id, &stream, 1000, &handle_lock, 0);
    SDK_FUNC_ERROR(model_name, ret);

    fh_uvc_stream_pts(pDev->stream_id, stream.time_stamp);
    fh_uvc_stream_enqueue(pDev->stream_id, stream.frm_yuyv.data.vbase, pDev->width * pDev->height * 2);

    ret = unlock_vpu_stream(grp_id, chn_id, handle_lock, &stream);
    SDK_FUNC_ERROR(model_name, ret);
#endif

    return FH_SDK_SUCCESS;
Exit:
    return FH_SDK_FAILED;
}

FH_SINT32 getViseStreamToUVCList(FH_SINT32 vise_id, FH_SINT32 chn_id, struct uvc_dev_app *pDev)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;
    FH_VIDEO_FRAME stream;
    struct yuv_buf *yuv_buf = NULL;

    SDK_CHECK_NULL_PTR(model_name, pDev);

    ret = vise_get_stream(vise_id, &stream);
    SDK_FUNC_ERROR(model_name, ret);

    for (int i = 0; i < UVC_YUV_BUF_NUM; i++)
    {
        yuv_buf = &pDev->yuv_buf[pDev->w_index];
        if (!yuv_buf->is_lock && !yuv_buf->is_full)
        {
            yuv_buf->is_lock = 1;
            memset(yuv_buf->data, 0, yuv_buf->bufsize);

            if (pDev->fcc == V4L2_PIX_FMT_IR)
            {
                yuv_buf->ysize = pDev->width * pDev->height;
                yuv_buf->uvsize = pDev->width * pDev->height / 2;
                fh_dma_memcpy(yuv_buf->data, stream.frm_scan.luma.data.vbase, stream.frm_scan.luma.data.size);
            }
            else if (pDev->fcc == V4L2_PIX_FMT_NV12)
            {
                yuv_buf->ysize = pDev->width * pDev->height;
                yuv_buf->uvsize = pDev->width * pDev->height / 2;
                fh_dma_memcpy(yuv_buf->data, stream.frm_scan.luma.data.vbase, stream.frm_scan.luma.data.size);
                fh_dma_memcpy(yuv_buf->data + stream.frm_scan.luma.data.size, stream.frm_scan.chroma.data.vbase, stream.frm_scan.chroma.data.size);
            }
            else if (pDev->fcc == V4L2_PIX_FMT_YUY2 && stream.data_format == VPU_VOMODE_YUYV)
            {
                yuv_buf->ysize = pDev->width * pDev->height;
                yuv_buf->uvsize = pDev->width * pDev->height;
                fh_dma_memcpy(yuv_buf->data, stream.frm_yuyv.data.vbase, stream.frm_yuyv.data.size);
            }
            else if (pDev->fcc == V4L2_PIX_FMT_YUY2 && stream.data_format == VPU_VOMODE_SCAN)
            {
                yuv_buf->ysize = pDev->width * pDev->height;
                yuv_buf->uvsize = pDev->width * pDev->height;
                fh_dma_memcpy(pDev->trans_buffer.data, stream.frm_scan.luma.data.vbase, yuv_buf->ysize);
                fh_dma_memcpy(pDev->trans_buffer.data + yuv_buf->ysize, stream.frm_scan.chroma.data.vbase, yuv_buf->uvsize / 2);
                nv12_to_yuy2((FH_CHAR *)pDev->trans_buffer.data, (FH_CHAR *)pDev->trans_buffer.data + yuv_buf->ysize, (FH_CHAR *)yuv_buf->data, pDev->width, pDev->height);
            }

            yuv_buf->time_stamp = stream.time_stamp;
            yuv_buf->is_full = 1;
            yuv_buf->is_lock = 0;

            pDev->w_index++;
            if (pDev->w_index == UVC_YUV_BUF_NUM)
            {
                pDev->w_index = 0;
            }

            ret = vise_release_stream(vise_id, &stream);
            SDK_FUNC_ERROR(model_name, ret);

            return ret;
        }

        pDev->w_index++;
        if (pDev->w_index == UVC_YUV_BUF_NUM)
        {
            pDev->w_index = 0;
        }
    }

    ret = vise_release_stream(vise_id, &stream);
    SDK_FUNC_ERROR(model_name, ret);

    SDK_ERR_PRT(model_name, "all buffer is full!\n");
Exit:
    return FH_SDK_FAILED;
}

FH_SINT32 getViseStreamToUVC(FH_SINT32 grp_id, FH_SINT32 chn_id, struct uvc_dev_app *pDev)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;
    struct yuv_buf *yuv_buf = NULL;

    SDK_CHECK_NULL_PTR(model_name, pDev);

    for (int i = 0; i < UVC_YUV_BUF_NUM; i++)
    {
        yuv_buf = &pDev->yuv_buf[pDev->r_index];
        if (!yuv_buf->is_lock && yuv_buf->is_full)
        {
            yuv_buf->is_lock = 1;

            fh_uvc_stream_pts(pDev->stream_id, yuv_buf->time_stamp);
            fh_uvc_stream_enqueue(pDev->stream_id, yuv_buf->data, yuv_buf->ysize + yuv_buf->uvsize);

            yuv_buf->is_full = 0;
            yuv_buf->is_lock = 0;

            pDev->r_index++;
            if (pDev->r_index == UVC_YUV_BUF_NUM)
            {
                pDev->r_index = 0;
            }

            return ret;
        }
    }

    usleep(10 * 1000);

    return FH_SDK_SUCCESS;

Exit:
    return FH_SDK_FAILED;
}

FH_SINT32 getENCStreamToUVC(FH_SINT32 grp_id, FH_SINT32 chn_id, struct uvc_dev_app *pDev, FH_STREAM_TYPE streamType)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;
    FH_VENC_STREAM stream;
    FH_SINT32 enc_chn = 0;
    ENC_INFO *enc_info;

    SDK_CHECK_NULL_PTR(model_name, pDev);

    enc_info = get_enc_config(grp_id, chn_id);
    enc_chn = enc_info->channel;

    ret = enc_get_chn_stream(enc_chn, &stream);
    SDK_FUNC_ERROR(model_name, ret);

    if (stream.stmtype == FH_STREAM_H264 && streamType == FH_STREAM_H264)
    {
        if (pDev->g_h264_delay > 1 && enc_chn == pDev->stream_id) // delay未实�?TODO
        {
            if (pDev->g_h264_delay == 5)
                FH_VENC_RequestIDR(enc_chn);
            pDev->g_h264_delay--;
            if (pDev->g_h264_delay == 1)
            {
                pDev->g_h264_delay = 0;
                FH_VENC_RequestIDR(enc_chn);
            }
        }

        fh_uvc_stream_pts(pDev->stream_id, stream.h264_stream.time_stamp);
        fh_uvc_stream_enqueue(pDev->stream_id, stream.h264_stream.start, stream.h264_stream.length);
    }
    else if (stream.stmtype == FH_STREAM_H265 && streamType == FH_STREAM_H265)
    {
        if (pDev->g_h265_delay > 1 && enc_chn == pDev->stream_id) // delay未实�?TODO
        {
            pDev->g_h265_delay--;
            if (pDev->g_h265_delay == 1)
            {
                pDev->g_h265_delay = 0;
                FH_VENC_RequestIDR(enc_chn);
            }
        }

        fh_uvc_stream_pts(pDev->stream_id, stream.h265_stream.time_stamp);
        fh_uvc_stream_enqueue(pDev->stream_id, stream.h265_stream.start, stream.h265_stream.length);
    }

    ret = enc_release_stream(&stream);
    SDK_FUNC_ERROR(model_name, ret);

    return ret;
Exit:
    return FH_SDK_FAILED;
}

FH_SINT32 getMJPEGStreamToUVC(FH_SINT32 grp_id, FH_SINT32 chn_id, struct uvc_dev_app *pDev, FH_STREAM_TYPE streamType, FH_SINT32 still_image)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;
    FH_VENC_STREAM stream;
    FH_SINT32 mjpeg_chn = 0;
    ENC_INFO *mjpeg_info;

    SDK_CHECK_NULL_PTR(model_name, pDev);

    mjpeg_info = get_mjpeg_config(grp_id, chn_id);
    mjpeg_chn = mjpeg_info->channel;

    ret = enc_get_chn_stream(mjpeg_chn, &stream);
    SDK_FUNC_ERROR(model_name, ret);

    if (stream.stmtype == FH_STREAM_MJPEG && streamType == FH_STREAM_MJPEG)
    {
        if (still_image)
        {
            fh_uvc_stream_pts(pDev->stream_id, UVC_STILL_IMAGE_MAGIC_FLAG);
            fh_uvc_stream_enqueue(pDev->stream_id, stream.mjpeg_stream.start, stream.mjpeg_stream.length);
        }
        else
        {
            if (stream.mjpeg_stream.start[0] == 0xff)
            {
                if (uvc_get_flash_mode() == IsocDataMode_Flash)
                {
                    uvc_get_flash_data(&stream.mjpeg_stream.start, &stream.mjpeg_stream.length);
                    uvc_set_flash_mode(IsocDataMode_stream);
                }
            }

            fh_uvc_stream_pts(pDev->stream_id, stream.mjpeg_stream.time_stamp);
            fh_uvc_stream_enqueue(pDev->stream_id, stream.mjpeg_stream.start, stream.mjpeg_stream.length);
        }
    }
    else
    {
        SDK_ERR_PRT(model_name, "streamType = %d,stream.stype=%d\n", streamType, stream.stmtype);
    }

    ret = enc_release_stream(&stream);
    SDK_FUNC_ERROR(model_name, ret);

    return ret;
Exit:
    return FH_SDK_FAILED;
}

// ==================== OSD Cross Feature ====================

#if ENABLE_OSD_CROSS


static FH_BOOL s_osd_cross_init = FH_FALSE;

/**
 * @brief Init OSD cross
 */
FH_SINT32 osd_cross_init(FH_SINT32 grp_id, FH_SINT32 chn_id, FH_UINT32 width, FH_UINT32 height)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;
    FHT_OSD_CONFIG_t osd_cfg;
    FHT_OSD_Layer_Config_t layer_cfg;
    
    if (s_osd_cross_init)
    {
        return FH_SDK_SUCCESS;
    }
    
    // Start OSD subsystem first
    ret = sample_fh_overlay_start(grp_id);
    if (ret != FH_SDK_SUCCESS)
    {
        SDK_ERR_PRT(model_name, "sample_fh_overlay_start failed, ret=%d\n", ret);
        return ret;
    }
    
    // Init OSD config
    memset(&osd_cfg, 0, sizeof(osd_cfg));
    memset(&layer_cfg, 0, sizeof(layer_cfg));
    
    // Config layer params
    layer_cfg.layerStartX = 0;
    layer_cfg.layerStartY = 0;
    layer_cfg.layerMaxWidth = width;
    layer_cfg.layerMaxHeight = height;
    layer_cfg.osdSize = 16;
    
    // Set transparent
    layer_cfg.normalColor.fRed = 0;
    layer_cfg.normalColor.fGreen = 0;
    layer_cfg.normalColor.fBlue = 0;
    layer_cfg.normalColor.fAlpha = 0;
    
    osd_cfg.osdRotate = 0;
    osd_cfg.nOsdLayerNum = 1;
    osd_cfg.pOsdLayerInfo = &layer_cfg;
    
    ret = FHAdv_Osd_SetText(grp_id, &osd_cfg);
    if (ret != FH_SDK_SUCCESS)
    {
        SDK_ERR_PRT(model_name, "FHAdv_Osd_SetText failed, ret=%d\n", ret);
        return ret;
    }
    
    s_osd_cross_init = FH_TRUE;
    
    return ret;
}

/**
 * @brief ����Ƶ�������Ļ��ƺ�ɫʮ��
 * @param grp_id VPU��ID
 * @param chn_id ͨ��ID
 * @param width ��Ƶ����
 * @param height ��Ƶ�߶�
 */
FH_SINT32 osd_cross_draw(FH_SINT32 grp_id, FH_SINT32 chn_id, FH_UINT32 width, FH_UINT32 height)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;
    OSD_GBOX gbox;
    
    // Draw a simple red rectangle in top-left corner for test
    memset(&gbox, 0, sizeof(gbox));
    gbox.enable = 1;
    gbox.gbox_id = 0;
    gbox.x = 100;
    gbox.y = 100;
    gbox.w = 100;
    gbox.h = 100;
    gbox.color.fRed = 255;
    gbox.color.fGreen = 0;
    gbox.color.fBlue = 0;
    gbox.color.fAlpha = 255;
    
    ret = sample_set_gbox(grp_id, chn_id, &gbox);
    if (ret != FH_SDK_SUCCESS)
    {
        SDK_ERR_PRT(model_name, "sample_set_gbox failed, ret=%d\n", ret);
        return ret;
    }
    
    return ret;
}

/**
 * @brief Close OSD cross
 */
FH_SINT32 osd_cross_close(FH_SINT32 grp_id, FH_SINT32 chn_id)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;
    OSD_GBOX gbox;
    
    memset(&gbox, 0, sizeof(gbox));
    gbox.enable = 0;
    gbox.gbox_id = 0;
    ret = sample_set_gbox(grp_id, chn_id, &gbox);
    
    gbox.gbox_id = 1;
    ret = sample_set_gbox(grp_id, chn_id, &gbox);
    
    s_osd_cross_init = FH_FALSE;
    
    return ret;
}

#endif // ENABLE_OSD_CROSS



