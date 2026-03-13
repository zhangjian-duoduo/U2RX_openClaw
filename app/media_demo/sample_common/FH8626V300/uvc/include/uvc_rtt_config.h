#ifndef __UVC_RTT_CONFIG_H__
#define __UVC_RTT_CONFIG_H__
#include "sample_common_media.h"
#include "vpu_config.h"
#include "enc_config.h"
#include "jpeg_config.h"
#include "vise_config.h"
#include "libusb_rtt/usb_video/include/usb_video.h"

FH_SINT32 getVPUStreamToUVCList(FH_SINT32 grp_id, FH_SINT32 chn_id, struct uvc_dev_app *pDev);

FH_SINT32 getVPUStreamToUVC(FH_SINT32 grp_id, FH_SINT32 chn_id, struct uvc_dev_app *pDev);

FH_SINT32 getViseStreamToUVCList(FH_SINT32 vise_id, FH_SINT32 chn_id, struct uvc_dev_app *pDev);

FH_SINT32 getViseStreamToUVC(FH_SINT32 vise_id, FH_SINT32 chn_id, struct uvc_dev_app *pDev);

FH_SINT32 getENCStreamToUVC(FH_SINT32 grp_id, FH_SINT32 chn_id, struct uvc_dev_app *pDev, FH_STREAM_TYPE streamType);

FH_SINT32 getMJPEGStreamToUVC(FH_SINT32 grp_id, FH_SINT32 chn_id, struct uvc_dev_app *pDev, FH_STREAM_TYPE streamType, FH_SINT32 still_image);

// OSD十字标绘功能
#if ENABLE_OSD_CROSS
FH_SINT32 osd_cross_init(FH_SINT32 grp_id, FH_SINT32 chn_id, FH_UINT32 width, FH_UINT32 height);
FH_SINT32 osd_cross_draw(FH_SINT32 grp_id, FH_SINT32 chn_id, FH_UINT32 width, FH_UINT32 height);
FH_SINT32 osd_cross_close(FH_SINT32 grp_id, FH_SINT32 chn_id);
#endif

#endif // __UVC_RTT_CONFIG_H__
