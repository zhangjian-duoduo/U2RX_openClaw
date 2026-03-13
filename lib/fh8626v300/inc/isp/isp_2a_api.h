#ifndef _ISP_2A_API_H_
#define _ISP_2A_API_H_

#include "isp_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */


FH_SINT32 API_ISP_AeAwbInit(FH_UINT32 u32IspDevId, struct isp_sensor_if* pstSensorFunc);
FH_SINT32 API_ISP_AeAwbRun(FH_UINT32 u32IspDevId);
FH_SINT32 API_ISP_AeAwbDeinit(FH_UINT32 u32IspDevId);

#endif