#ifndef _ISP_API_H_
#define _ISP_API_H_


#include "isp_common.h"
#include "isp_sensor_if.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#define lift_shift_bit_num(bit_num)			(1<<bit_num)
// #define DEBUG_API 1

#define CHECK_VALIDATION(x, a, b)                                                                                                  \
    do                                                                                                                             \
    {                                                                                                                              \
        int r;                                                                                                                     \
        if ((x < a) || (x > b))                                                                                                    \
        {                                                                                                                          \
            if (x < a)                                                                                                             \
                r = a;                                                                                                             \
            else                                                                                                                   \
                r = b;                                                                                                             \
            fh_printf("[WARNING]parameter out of range @%s %s= %d | range=[%d,%d] | auto clip to %d\n", __func__, #x, x, a, b, r); \
            x = r;                                                                                                                 \
        }                                                                                                                          \
    } while (0)

#define FUNC_DEP __attribute__((deprecated))

enum ISP_HW_MODULE_LIST
{
    HW_MODUL_LUT              = lift_shift_bit_num(0),
    HW_MODUL_CLP              = lift_shift_bit_num(1),
    HW_MODUL_HMIR             = lift_shift_bit_num(2),
    HW_MODUL_DPC              = lift_shift_bit_num(3),
    HW_MODUL_GB               = lift_shift_bit_num(4),
    HW_MODUL_LSC              = lift_shift_bit_num(5),
    HW_MODUL_GAIN             = lift_shift_bit_num(6),
    HW_MODUL_CRC              = lift_shift_bit_num(7),
    HW_MODUL_NR3D             = lift_shift_bit_num(8),
    HW_MODUL_OVB              = lift_shift_bit_num(9),

    HW_MODUL_POSTGAIN         = lift_shift_bit_num(10),
    HW_MODUL_HLR              = lift_shift_bit_num(11),
    HW_MODUL_RGGB             = lift_shift_bit_num(12),
    HW_MODUL_CGAMMA           = lift_shift_bit_num(13),
    HW_MODUL_YNR              = lift_shift_bit_num(14),
    HW_MODUL_CNR              = lift_shift_bit_num(15),
    HW_MODUL_APC              = lift_shift_bit_num(16),
    HW_MODUL_YGAMMA           = lift_shift_bit_num(17),
    HW_MODUL_YIE              = lift_shift_bit_num(18),
    HW_MODUL_CHROMA           = lift_shift_bit_num(19),
    HW_MODUL_CIE              = lift_shift_bit_num(20),
    HW_MODUL_CFA              = lift_shift_bit_num(21),
    HW_MODUL_NR2D             = lift_shift_bit_num(22),
    HW_MODUL_DRC              = lift_shift_bit_num(23),
    HW_MODUL_HSV              = lift_shift_bit_num(24),
    HW_MODUL_LCE              = lift_shift_bit_num(25),
    HW_MODUL_SUBDC            = lift_shift_bit_num(26),
    HW_MODUL_MATCHSUBDC       = lift_shift_bit_num(27),
    HW_MODUL_MATCHPOSTGAIN    = lift_shift_bit_num(28),

    HW_MODUL_STAT_AE          = lift_shift_bit_num(29),
    HW_MODUL_STAT_AWB         = lift_shift_bit_num(30),
    HW_MODUL_STAT_HIST        = lift_shift_bit_num(31),

    ISP_HW_MODULE_LIST_DUMMY=0xffffffff,
};


enum ISP_IRSTATUS_E {
    ISP_NIGHT = 0,
    ISP_DAY   = 1,
    IRSTATUS_DUMMY = 0xffffffff,
};

enum ISP_LEVEL_E {
    LEVEL_0 = 0,
    LEVEL_1 = 1,
    LEVEL_2 = 2,
    LEVEL_3 = 3,
    LEVEL_DUMMY = 0xffffffff,
};

/**SYSTEM_CONTROL*/
/*
*   Name: API_ISP_SetModuleCfg
*            ISP相关工作模式配置
*
*   Parameters:
*
*       [IN] FH_UINT32 u32IspDevId
*            ISP的设备号，通过它选择配置不同的ISP
*
*       [IN] ISP_CTRL_CFG *pstDrvCtrlCfg
*            工作模式配置的参数指针，具体描述见ISP_CTRL_CFG结构体
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*   Note:
*       1. WDR的开关只影响内存分配，不影响硬件开关
*       2. 该接口不支持实时调用，调用位置需要在API_ISP_MemInit之前
*/
FH_SINT32 API_ISP_SetModuleCfg(FH_UINT32 u32IspDevId, ISP_CTRL_CFG *pstDrvCtrlCfg);
/*
*   Name: API_ISP_GetModuleCfg
*            获取当前ISP的配置模式
*
*   Parameters:
*
*       [IN] FH_UINT32 u32IspDevId
*            ISP的设备号，通过它选择配置不同的ISP
*
*       [OUT] ISP_CTRL_CFG *pstDrvCtrlCfg
*            工作模式配置的参数指针，具体描述见ISP_CTRL_CFG结构体
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetModuleCfg(FH_UINT32 u32IspDevId, ISP_CTRL_CFG *pstDrvCtrlCfg);
/*
*   Name: API_ISP_GetBuffSize
*            获取ISP中分配的buffer大小
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN]  ISP_MEM_INIT* stMemInit
*            ISP初始化内存的参数配置
*
*   Return:
*            ISP中分配的buffer大小
*   Note:
*       只用于统计内存时调用,isp正常运行程序不能调用,否则会出异常
*/
FH_SINT32 API_ISP_GetBuffSize(FH_UINT32 u32IspDevId, ISP_MEM_INIT* stMemInit);
/*
*   Name: API_ISP_MemInit
*            ISP内部使用的memory分配与初始化
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN]  ISP_MEM_INIT* stMemInit
*            ISP初始化内存的参数配置
*
*   Return:
*            0(正确)
*           -1(ISP设备驱动打开失败)
*   Note:
*       仅使用ISP_MEM_INIT中的幅面信息stPicConf和ISP输出的方式enIspOutMode
*/
FH_SINT32 API_ISP_MemInit(FH_UINT32 u32IspDevId, ISP_MEM_INIT* stMemInit);
/*
*   Name: API_ISP_GetBinAddr
*            获取ISP的param参数的地址和大小
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_PARAM_CONFIG* param_conf
*            param的地址和大小
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetBinAddr(FH_UINT32 u32IspDevId, ISP_PARAM_CONFIG* param_conf);
/*
*   Name: API_ISP_SetCisClk
*            配置供给sensor的时钟
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] FH_UINT32 cisClk
*            需要配置时钟频率值
*
*   Return:
*            正确则返回0,错误则返回ERROR_ISP_SET_CIS_CLK
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetCisClk(FH_UINT32 u32IspDevId, FH_UINT32 cisClk);
/*
*   Name: API_ISP_CisClkEn
*            供给sensor的时钟使能配置
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] FH_BOOL bEn
*            1表示使能时钟，0表示关闭时钟。
*
*   Return:
*            0(正确)，错误(ERROR_ISP_SET_CIS_CLK)
*   Note:
*       无
*/
FH_SINT32 API_ISP_CisClkEn(FH_UINT32 u32IspDevId, FH_BOOL bEn);
/*
*   Name: API_ISP_SetViAttr
*            配置vi相关的一些幅面信息
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const ISP_VI_ATTR_S *pstViAttr
*            结构体ISP_VI_ATTR_S的指针
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetViAttr(FH_UINT32 u32IspDevId, const ISP_VI_ATTR_S *pstViAttr);
/*
*   Name: API_ISP_GetViAttr
*            获取当前幅面相关信息
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_VI_ATTR_S *pstViAttr
*            结构体ISP_VI_ATTR_S的指针
*
*   Return:
*            0(正确)，
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetViAttr(FH_UINT32 u32IspDevId, ISP_VI_ATTR_S *pstViAttr);
/*
*   Name: API_ISP_Init
*            初始化ISP硬件寄存器，并启动ISP
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*   Return:
*            0(正确)
*        -1003(ISP初始化异常)
*   Note:
*       无
*/
FH_SINT32 API_ISP_Init(FH_UINT32 u32IspDevId);
/*
*   Name: API_ISP_LoadIspParam
*            加载指定参数到DRV寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] char *isp_param_buff
*            指定参数的指针
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_LoadIspParam(FH_UINT32 u32IspDevId, char *isp_param_buff);
/*
*   Name: API_ISP_Pause
*            暂停ISP对图像的处理与输出
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*   Return:
*            0(正确)
*   Note:
*       无
*/
FH_SINT32 API_ISP_Pause(FH_UINT32 u32IspDevId);
/*
*   Name: API_ISP_Resume
*            恢复ISP对图像处理与输出
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*   Return:
*            0(正确)
*   Note:
*       无
*/
FH_SINT32 API_ISP_Resume(FH_UINT32 u32IspDevId);
/*
*   Name: API_ISP_SetRunPara
*            配置API_ISP_Run的运行参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN]  ISP_RUN_PARA_S* pstRunPara
*            类型为ISP_RUN_PARA_S的结构体指针，详细成员变量请查看ISP_RUN_PARA_S结构体定义。
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       1. bAeAwbOnly该接口配置无效
*/
FH_SINT32 API_ISP_SetRunPara(FH_UINT32 u32IspDevId, ISP_RUN_PARA_S* pstRunPara);
/*
*   Name: API_ISP_GetRunPara
*            获取API_ISP_Run的运行参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN]  ISP_RUN_PARA_S* pstRunPara
*            类型为ISP_RUN_PARA_S的结构体指针，详细成员变量请查看ISP_RUN_PARA_S结构体定义。
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       1. bAeAwbOnly该接口配置无效
*/
FH_SINT32 API_ISP_GetRunPara(FH_UINT32 u32IspDevId, ISP_RUN_PARA_S* pstRunPara);
/*
*   Name: API_ISP_Run
*            ISP策略处理
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*   Return:
*            0(正确)，
*           -1(图像丢失)
*   Note:
*       无
*/
FH_SINT32 API_ISP_Run(FH_UINT32 u32IspDevId);
/*
*   Name: API_ISP_AE_AWB_Run
*            ISP AE&AWB策略处理
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*   Return:
*            0(正确)
*            图像丢失(ERROR_ISP_WAIT_PICEND_FAILED)
*   Note:
*       无
*/
FH_SINT32 API_ISP_AE_AWB_Run(FH_UINT32 u32IspDevId);
/*
*   Name: API_ISP_Exit
*            ISP线程退出，清理状态
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*   Return:
*            0(正确)
*   Note:
*       无
*/
FH_SINT32 API_ISP_Exit(FH_UINT32 u32IspDevId);
/*
*   Name: API_ISP_SensorRegCb
*            拷贝sensor的回调函数到目标地址。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] FH_UINT32 u32SensorId
*            无用
*
*       [IN]  struct isp_sensor_if* pstSensorFunc
*            类型为isp_sensor_if的结构体指针，详细成员变量请查看isp_sensor_if结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SensorRegCb(FH_UINT32 u32IspDevId, FH_UINT32 u32SensorId, struct isp_sensor_if* pstSensorFunc);
/*
*   Name: API_ISP_SensorUnRegCb
*            注销ISP中注册的sensor的回调函数。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] FH_UINT32 u32SensorId
*            无用
*
*   Return:
*            0(正确)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SensorUnRegCb(FH_UINT32 u32IspDevId, FH_UINT32 u32SensorId);
/*
*   Name: API_ISP_Set_HWmodule_cfg
*            配置ISP模块硬件使能
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const FH_UINT32 u32modulecfg
*            每1bit对应一个硬件使能位0表示关闭，1表示打开，详见枚举ISP_HW_MODULE_LIST。
*
*   Return:
*            0(正确)
*   Note:
*       当需要一次开关多个模块时,使用此函数,若只需控制一个模块,则可使用API_ISP_Set_Determined_HWmodule函数实现
*/
FH_SINT32 API_ISP_Set_HWmodule_cfg(FH_UINT32 u32IspDevId, const FH_UINT32 u32modulecfg);
/*
*   Name: API_ISP_Get_HWmodule_cfg
*            获取当前ISP模块硬件状态
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] FH_UINT32 *u32modulecfg
*            每1bit对应一个硬件使能位0表示关闭，1表示打开，详见枚举ISP_HW_MODULE_LIST。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_Get_HWmodule_cfg(FH_UINT32 u32IspDevId, FH_UINT32 *u32modulecfg);
/*
*   Name: API_ISP_Set_Determined_HWmodule
*            配置ISP某个模块硬件使能
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const HW_MODULE_CFG *pstModuleCfg
*            传入要控制的某个模块枚举变量以及开关状态，详见HW_MODULE_CFG。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       一次只能传入一个模块,否则会出错
*/
FH_SINT32 API_ISP_Set_Determined_HWmodule(FH_UINT32 u32IspDevId, const HW_MODULE_CFG *pstModuleCfg);
/*
*   Name: API_ISP_Get_Determined_HWmodule
*           获取ISP某个模块硬件使能状态
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [INOUT] HW_MODULE_CFG *pstModuleCfg
*            传入要控制的某个模块枚举变量,获取该模块开关状态，详见HW_MODULE_CFG。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       一次只能传入一个模块,否则会出错
*/
FH_SINT32 API_ISP_Get_Determined_HWmodule(FH_UINT32 u32IspDevId, HW_MODULE_CFG *pstModuleCfg);
/*
*   Name: API_ISP_Param_Init
*           初始化ISP效果档位参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       第一次调用API_ISP_SET_SHARPNESS_LEVEL、API_ISP_SET_DENOISE3D_LEVEL前需调用此接口，位置建议放在加载完ISP效果参数之后。
*/
FH_SINT32 API_ISP_Param_Init(FH_UINT32 u32DevId);
/*
*   Name: API_ISP_SET_SHARPNESS_LEVEL
*           设置ISP清晰度等级
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] FH_UINT32 sharpLevel
*            传入要要设置的清晰度等级，详见ISP_LEVEL_E枚举。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       第一次调用前需调用API_ISP_Param_Init(),初始化ISP效果档位参数。
*/
FH_SINT32 API_ISP_SET_SHARPNESS_LEVEL(FH_UINT32 u32IspDevId, FH_UINT32 sharpLevel);
/*
*   Name: API_ISP_SET_DENOISE3D_LEVEL
*           设置ISP NR3D降噪等级
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] FH_UINT32 nr3dLevel
*            传入要要设置的NR3D降噪等级，详见ISP_LEVEL_E枚举。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       第一次调用前需调用API_ISP_Param_Init(),初始化ISP效果档位参数。
*/
FH_SINT32 API_ISP_SET_DENOISE3D_LEVEL(FH_UINT32 u32IspDevId, FH_UINT32 nr3dLevel);
/*
*   Name: API_ISP_GetAlgCtrl
*            获取软件控制开关寄存器状态
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] FH_UINT32 *u32AlgCtrl
*            每1bit对应一个软件使能位0表示关闭，1表示打开
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetAlgCtrl(FH_UINT32 u32IspDevId, FH_UINT32 *u32AlgCtrl);
/*
*   Name: API_ISP_SetAlgCtrl
*            设置软件控制开关寄存器状态
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] FH_UINT32 u32AlgCtrl
*            每1bit对应一个软件使能位0表示关闭，1表示打开
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetAlgCtrl(FH_UINT32 u32IspDevId, FH_UINT32 u32AlgCtrl);
/*
*   Name: API_ISP_GetRawSize
*         获取ISP_NR3D处的raw数据的大小, 供应用层分配空间
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT]  FH_UINT32* raw_size
*            raw数据大小,单位byte
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       与API_ISP_GetRaw配合使用
*/
FH_SINT32 API_ISP_GetRawSize(FH_UINT32 u32IspDevId, FH_UINT32 *raw_size);
/*
*   Name: API_ISP_GetRawCfg
*            获取raw数据的配置, 供应用层分配空间
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_RAW_BUF *raw_buf
*            结构体包含分块模式,短帧raw数据的大小
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       与API_ISP_GetRaw配合使用
*/
FH_SINT32 API_ISP_GetRawCfg(FH_UINT32 u32IspDevId, ISP_RAW_BUF *raw_buf);
/*
*   Name: API_ISP_GetRaw
*            获取ISP　NR3D处的raw数据
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] FH_BOOL strategy_en
*            关闭硬件影响图像效果模块(0)，打开硬件影响图像效果模块(1)
*
*       [OUT]  FH_VOID* pRawBuff
*            存放raw数据的地址
*
*       [IN]  FH_UINT32 u32Size
*            存放raw数据buffer大小
*
*       [IN]  FH_UINT32 u32FrameCnt
*            需要导出的帧数，导出帧数不连续
*
*       [IN]  FRAME_TYPE type
*            帧类型
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetRaw(FH_UINT32 u32IspDevId, FH_BOOL strategy_en, FH_VOID* pRawBuff, FH_UINT32 u32Size, FH_UINT32 u32FrameCnt, FRAME_TYPE type);
/*
*   Name: API_ISP_SetRawBuf
*            连续导出多帧raw功能, 配置相关信息
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] FH_BOOL strategy_en
*            关闭硬件影响图像效果模块(0)，打开硬件影响图像效果模块(1)
*
*       [IN]  ISP_RAW_BUF *raw_buf
*            类型为ISP_RAW_BUF的结构体指针，详细成员变量请查看ISP_RAW_BUF结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       应用层需申请足够的vmm内存
*/
FH_SINT32 API_ISP_SetRawBuf(FH_UINT32 u32IspDevId, FH_BOOL strategy_en, ISP_RAW_BUF *raw_buf);
/*
*   Name: API_ISP_GetViData
*            获取VI处的raw数据
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN/OUT] ISP_RAW_BUF_CFG_S *pstRawBufInfo
*            配置的信息，以及返回获取到的数据地址
*            类型为ISP_RAW_BUF_CFG_S的结构体指针，详细成员变量请查看ISP_RAW_BUF_CFG_S结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*
*/
FH_SINT32 API_ISP_GetViData(FH_UINT32 u32IspDevId, ISP_RAW_BUF_CFG_S *pstRawBufInfo);
/*
*   Name: API_ISP_ReleaseViData
*            释放VI处的raw数据
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN/OUT] ISP_RAW_BUF_CFG_S *pstRawBufInfo
*            配置的信息，以及返回获取到的数据地址
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*
*   Note:
*        无
*/
FH_SINT32 API_ISP_ReleaseViData(FH_UINT32 u32IspDevId, ISP_RAW_BUF_CFG_S *pstRawBufInfo);
/*
*   Name: API_ISP_GetViBlkSize
*            获取VI处的raw内存大小
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN]  FH_UINT32 u32Width
*            输入的幅面宽度
*
*       [IN]  FH_UINT32 u32Height
*            输入的幅面高度
*
*       [OUT] FH_UINT32 *blk_size
*            每块内存需要的Byte
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*
*   Note:
*        无
*/
FH_SINT32 API_ISP_GetViBlkSize(FH_UINT32 u32IspDevId, FH_UINT32 u32Width, FH_UINT32 u32Height, FH_UINT32 *blk_size);
/*
*   Name: API_ISP_Open
*            打开isp驱动设备
*
*   Parameters:
*
*       [IN]  FH_VOID
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*
*   Note:
*        需要与API_ISP_Close配对使用
*/
FH_SINT32 API_ISP_Open(FH_VOID);
/*
*   Name: API_ISP_Close
*            关闭isp驱动设备
*
*   Parameters:
*
*       [IN]  FH_VOID
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*
*   Note:
*        需要与API_ISP_Open配对使用
*/
FH_SINT32 API_ISP_Close(FH_VOID);
/*
*   Name: API_ISP_PowerSuspend
*            ISP SYS掉电前的操作
*
*   Parameters:
*
*       [IN]  ISP_SUSPEND_S* pstSuspendPara
*            类型为ISP_SUSPEND_S的结构体指针，详细成员变量请查看ISP_SUSPEND_S结构体定义。
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*
*   Note:
*        该接口为保留接口暂时无用
*/
FH_SINT32 API_ISP_PowerSuspend(FH_UINT32 u32IspDevId, ISP_SUSPEND_S* pstSuspendPara);
/*
*   Name: API_ISP_PowerResume
*            ISP SYS掉电后的恢复操作
*
*   Parameters:
*
*       [IN]  ISP_RESUME_S* pstResumePara
*            类型为ISP_RESUME_S的结构体指针，详细成员变量请查看ISP_RESUME_S结构体定义。
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*
*   Note:
*        该接口为保留接口暂时无用
*/
FH_SINT32 API_ISP_PowerResume(FH_UINT32 u32IspDevId, ISP_RESUME_S* pstResumePara);
/*
*   Name: API_ISP_SetAovWkMode
*            配置AOV模式参数
*
*   Parameters:
*
*       [IN]  ISP_AOV_MODE_CFG_S* pstAvoCfg
*            类型为ISP_AOV_MODE_CFG_S的结构体指针，详细成员变量请查看ISP_AOV_MODE_CFG_S结构体定义。
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*
*   Note:
*        无
*/
FH_SINT32 API_ISP_SetAovWkMode(FH_UINT32 u32IspDevId, ISP_AOV_MODE_CFG_S* pstAvoCfg);
/*
*   Name: API_ISP_GetAovWkMode
*            配置AOV模式参数
*
*   Parameters:
*
*       [IN]  ISP_AOV_MODE_CFG_S* pstAovCfg
*            类型为ISP_AOV_MODE_CFG_S的结构体指针，详细成员变量请查看ISP_AOV_MODE_CFG_S结构体定义。
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*
*   Note:
*        无
*/
FH_SINT32 API_ISP_GetAovWkMode(FH_UINT32 u32IspDevId, ISP_AOV_MODE_CFG_S* pstAovCfg);
/*
*   Name: API_ISP_SetColorbar
*            设置isp输出彩条
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN]  FH_UINT32 u32Fps
*            彩条输出帧率,配0表示关闭彩条输出
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*
*   Note:
*        fps有最小值,具体以isp实际输出为准
*/
FH_SINT32 API_ISP_SetColorbar(FH_UINT32 u32IspDevId, FH_UINT32 u32Fps);
/**SENSOR_CONTROL*/

/*
*   Name: API_ISP_SensorInit
*            sensor预初始化，并未配置sensor寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN]  Sensor_Init_t* initCfg
*            sensor的初始化信息
*
*   Return:
*            0(正确)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SensorInit(FH_UINT32 u32IspDevId, Sensor_Init_t* initCfg);
/*
*   Name: API_ISP_SetSensorFmt
*            初始化sensor
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] FH_UINT32 format
*            传入的时枚举FORMAT中的枚举值，选择幅面
*
*   Return:
*            0(正确)
*        -3002(sensor初始化异常)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetSensorFmt(FH_UINT32 u32IspDevId, FH_UINT32 format);

/*
*   Name: API_ISP_SensorKick
*            启动sensor输出，有的sensor需要用到
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*   Return:
*            0(正确返回)
*           -1(sensor相关的回调函数未被注册)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SensorKick(FH_UINT32 u32IspDevId);
/*
*   Name: API_ISP_SetSensorIntt
*            配置sensor的曝光值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] FH_UINT32 intt
*            传入的曝光值
*
*   Return:
*            0(正确返回)
*           -1(sensor相关的回调函数未被注册)。
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetSensorIntt(FH_UINT32 u32IspDevId, FH_UINT32 intt);
/*
*   Name: API_ISP_SetSensorGain
*            配置sensor增益值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] FH_UINT32 gain
*            增益值
*
*   Return:
*            0(正确返回)
*           -1(sensor相关的回调函数未被注册)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetSensorGain(FH_UINT32 u32IspDevId, FH_UINT32 gain);
/*
*   Name: API_ISP_MapInttGain
*            不同幅面切换时根据当前幅面曝光值和增益值转换成目标幅面曝光值和增益值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_SENSOR_COMMON_CMD_DATA0 *pstMapInttGainCfg
*            类型为ISP_SENSOR_COMMON_CMD_DATA0的结构体指针，详细成员变量请查看ISP_SENSOR_COMMON_CMD_DATA0结构体定义。
*
*   Return:
*            0(正确返回)
*           -1(sensor相关的回调函数未被注册)
*   Note:
*       无
*/
FH_SINT32 API_ISP_MapInttGain(FH_UINT32 u32IspDevId, ISP_SENSOR_COMMON_CMD_DATA0 *pstMapInttGainCfg);
/*
*   Name: API_ISP_SetSensorReg
*            配置sensor寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] FH_UINT16 addr
*            sensor寄存器地址
*
*       [IN] FH_UINT16 data
*            配置的值
*
*   Return:
*            0(正确返回)
*           -1(sensor相关的回调函数未被注册)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetSensorReg(FH_UINT32 u32IspDevId, FH_UINT16 addr,FH_UINT16 data);
/*
*   Name: API_ISP_GetSensorReg
*            读取sensor寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] FH_UINT16 addr
*            sensor寄存器地址
*
*       [OUT]  FH_UINT16 *data
*            读取的值
*
*   Return:
*            0(正确返回)
*           -1(sensor相关的回调函数未被注册)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetSensorReg(FH_UINT32 u32IspDevId, FH_UINT16 addr, FH_UINT16 *data);
/*
*   Name: API_ISP_ChangeSnsSlaveAddr
*            修改sensor对应的i2c地址
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] FH_UINT32 u32i2cAddr
*            新的sensor i2caddr
*
*   Return:
*           0
*
*   Note:
*       1. 修改sensor对应的i2c地址 需要在API_ISP_SensorInit调用之前调用
*/
FH_SINT32 API_ISP_ChangeSnsSlaveAddr(FH_UINT32 u32IspDevId, FH_UINT32 u32i2cAddr);
/*
*   Name: API_ISP_SensorPause
*            暂停sensor输出
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*   Return:
*           0
*
*   Note:
*       1. 用于暂停sensor的输出流
*/
FH_SINT32 API_ISP_SensorPause(FH_UINT32 u32IspDevId);
/*
*   Name: API_ISP_SensorResume
*            恢复sensor输出
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*   Return:
*           0
*
*   Note:
*       1. 用于恢复sensor的输出流
*/
FH_SINT32 API_ISP_SensorResume(FH_UINT32 u32IspDevId);
/*
*   Name: API_ISP_SetSensorAttr
*            配置sensor的属性
*
*   Parameters:
*
*       [IN] ISP_SNS_ATTR_CFG_S *pstSnsCfg
*            类型为ISP_SNS_ATTR_CFG_S的结构体指针，详细成员变量请查看ISP_SNS_ATTR_CFG_S结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*   Note:
*       在不注册sensor库的时候可以使用该接口替代一部分sensor功能
*/
FH_UINT32 API_ISP_SetSensorAttr(ISP_SNS_ATTR_CFG_S *pstSnsCfg);
/*
*   Name: API_ISP_GetSensorAttr
*            获取sensor的属性
*
*   Parameters:
*
*       [IN] ISP_SNS_ATTR_CFG_S *pstSnsCfg
*            类型为ISP_SNS_ATTR_CFG_S的结构体指针，详细成员变量请查看ISP_SNS_ATTR_CFG_S结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*   Note:
*       在不注册sensor库的时候可以使用该接口替代一部分sensor功能
*/
FH_UINT32 API_ISP_GetSensorAttr(ISP_SNS_ATTR_CFG_S *pstSnsCfg);
/**SMART_AE**/
/*
*   Name: API_ISP_SetSmartAECfg
*            设置smart_ae相关参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] SMART_AE_CFG *pstSmartAeCfg
*            类型为SMART_AE_CFG的结构体指针，详细成员变量请查看SMART_AE_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetSmartAECfg(FH_UINT32 u32IspDevId, SMART_AE_CFG *pstSmartAeCfg);
/*
*   Name: API_ISP_GetSmartAECfg
*            获取smart_ae相关参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] SMART_AE_CFG *pstSmartAeCfg
*            类型为SMART_AE_CFG的结构体指针，详细成员变量请查看SMART_AE_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetSmartAECfg(FH_UINT32 u32IspDevId, SMART_AE_CFG *pstSmartAeCfg);
/*
*   Name: API_ISP_GetSmartAeStatus
*            获取smart_ae状态值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] SMART_AE_STATUS *pstSmartAeStatus
*            类型为SMART_AE_STATUS的结构体指针，详细成员变量请查看SMART_AE_STATUS结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetSmartAeStatus(FH_UINT32 u32IspDevId, SMART_AE_STATUS *pstSmartAeStatus);
/** AE **/
/*
*   Name: API_ISP_SetAeDefaultCfg
*            设置ae相关参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] AE_DEFAULT_CFG *pstAeDefaultCfg
*            类型为AE_DEFAULT_CFG的结构体指针，详细成员变量请查看AE_DEFAULT_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetAeDefaultCfg(FH_UINT32 u32IspDevId, AE_DEFAULT_CFG *pstAeDefaultCfg);
/*
*   Name: API_ISP_GetAeDefaultCfg
*            获取ae相关参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] AE_DEFAULT_CFG *pstAeDefaultCfg
*            类型为AE_DEFAULT_CFG的结构体指针，详细成员变量请查看AE_DEFAULT_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetAeDefaultCfg(FH_UINT32 u32IspDevId, AE_DEFAULT_CFG *pstAeDefaultCfg);
/*
*   Name: API_ISP_SetAeRouteCfg
*            设置ae路线模式相关参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] AE_ROUTE_CFG *pstAeRouteCfg
*            类型为AE_ROUTE_CFG的结构体指针，详细成员变量请查看AE_ROUTE_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetAeRouteCfg(FH_UINT32 u32IspDevId, AE_ROUTE_CFG *pstAeRouteCfg);
/*
*   Name: API_ISP_GetAeRouteCfg
*            获取ae路线模式相关参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] AE_ROUTE_CFG *pstAeRouteCfg
*            类型为AE_ROUTE_CFG的结构体指针，详细成员变量请查看AE_ROUTE_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetAeRouteCfg(FH_UINT32 u32IspDevId, AE_ROUTE_CFG *pstAeRouteCfg);
/*
*   Name: API_ISP_SetAeInfo
*            设置ae曝光时间及增益参数至sensor及硬件寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const ISP_AE_INFO *pstAeInfo
*            类型为ISP_AE_INFO的结构体指针，详细成员变量请查看ISP_AE_INFO结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetAeInfo(FH_UINT32 u32IspDevId, const ISP_AE_INFO *pstAeInfo);
/*
*   Name: API_ISP_GetAeInfo
*           从sensor及硬件寄存器获取ae曝光时间及增益参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_AE_INFO *pstAeInfo
*            类型为ISP_AE_INFO的结构体指针，详细成员变量请查看ISP_AE_INFO结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetAeInfo(FH_UINT32 u32IspDevId, ISP_AE_INFO *pstAeInfo);
/*
*   Name: API_ISP_SetAeInfo2
*            设置ae曝光时间及增益参数至sensor及硬件寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const ISP_AE_INFO_2 *pstAeInfo2
*            类型为ISP_AE_INFO_2的结构体指针，详细成员变量请查看ISP_AE_INFO_2结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       与API_ISP_SetAeInfo的区别在于isp gain分为pre和post两部分配置
*/
FH_SINT32 API_ISP_SetAeInfo2(FH_UINT32 u32IspDevId, const ISP_AE_INFO_2 *pstAeInfo2);
/*
*   Name: API_ISP_GetAeInfo2
*           从sensor及硬件寄存器获取ae曝光时间及增益参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_AE_INFO_2 *pstAeInfo2
*            类型为ISP_AE_INFO_2的结构体指针，详细成员变量请查看ISP_AE_INFO_2结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       与API_ISPGetAeInfo的区别在于isp gain分为pre和post两部分获取
*/
FH_SINT32 API_ISP_GetAeInfo2(FH_UINT32 u32IspDevId, ISP_AE_INFO_2 *pstAeInfo2);
/*
*   Name: API_ISP_SetAeInfo3
*            设置ae曝光时间(us)及增益参数至sensor及硬件寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const ISP_AE_INFO_3 *pstAeInfo3
*            类型为ISP_AE_INFO_3的结构体指针，详细成员变量请查看ISP_AE_INFO_2结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       与API_ISP_SetAeInfo的区别在于intt以us为单位
*/
FH_SINT32 API_ISP_SetAeInfo3(FH_UINT32 u32IspDevId, const ISP_AE_INFO_3 *pstAeInfo3);
/*
*   Name: API_ISP_GetAeInfo3
*           从sensor及硬件寄存器获取ae曝光时间及增益参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_AE_INFO_3 *pstAeInfo3
*            类型为ISP_AE_INFO_2的结构体指针，详细成员变量请查看ISP_AE_INFO_3结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       与API_ISPGetAeInfo的区别在于intt以us为单位
*/
FH_SINT32 API_ISP_GetAeInfo3(FH_UINT32 u32IspDevId, ISP_AE_INFO_3 *pstAeInfo3);
/*
*   Name: API_ISP_GetAeStatus
*            获取ae状态值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_AE_STATUS *pstAeStatus
*            类型为ISP_AE_STATUS的结构体指针，详细成员变量请查看ISP_AE_STATUS结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetAeStatus(FH_UINT32 u32IspDevId, ISP_AE_STATUS *pstAeStatus);
/*
*   Name: API_ISP_SetAeUserStatus
*            设置ae用户状态值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] AE_USER_STATUS *pstAeUserStatus
*            类型为AE_USER_STATUS的结构体指针，详细成员变量请查看AE_USER_STATUS结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       若用户使用自定义ae策略,则需要调用该函数配置ae相关状态值,提供给isp的其他策略使用,否则其他策略可能运行会有异常
*/
FH_SINT32 API_ISP_SetAeUserStatus(FH_UINT32 u32IspDevId, AE_USER_STATUS *pstAeUserStatus);

/** AWB **/
/*
*   Name: API_ISP_SetAwbDefaultCfg
*            设置awb相关参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] AWB_DEFAULT_CFG *pstAwbDefaultCfg
*            类型为AWB_DEFAULT_CFG的结构体指针，详细成员变量请查看AWB_DEFAULT_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetAwbDefaultCfg(FH_UINT32 u32IspDevId, AWB_DEFAULT_CFG *pstAwbDefaultCfg);
/*
*   Name: API_ISP_GetAwbDefaultCfg
*            获取awb相关参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] AWB_DEFAULT_CFG *pstAwbDefaultCfg
*            类型为AWB_DEFAULT_CFG的结构体指针，详细成员变量请查看AWB_DEFAULT_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetAwbDefaultCfg(FH_UINT32 u32IspDevId, AWB_DEFAULT_CFG *pstAwbDefaultCfg);
/*
*   Name: API_ISP_GetAwbStatus
*            获取awb状态值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_AWB_STATUS *pstAwbStatus
*            类型为ISP_AWB_STATUS的结构体指针，详细成员变量请查看ISP_AWB_STATUS结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetAwbStatus(FH_UINT32 u32IspDevId, ISP_AWB_STATUS *pstAwbStatus);
/*
*   Name: API_ISP_SetAwbGain
*            设置awb增益至硬件寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_AWB_GAIN *pstAwbGain
*            类型为ISP_AWB_GAIN的结构体指针，详细成员变量请查看ISP_AWB_GAIN结构体定义。
*
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetAwbGain(FH_UINT32 u32IspDevId, const ISP_AWB_GAIN *pstAwbGain);
/*
*   Name: API_ISP_GetAwbGain
*            从硬件寄存器获取awb增益值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] const ISP_AWB_GAIN *pstAwbGain
*            类型为ISP_AWB_GAIN的结构体指针，详细成员变量请查看ISP_AWB_GAIN结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)

*
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetAwbGain(FH_UINT32 u32IspDevId, ISP_AWB_GAIN *pstAwbGain);
/**BLC*/
/*
*   Name: API_ISP_SetBlcCfg
*            配置BLC的DRV寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_BLC_CFG *pstBlcCfg
*            类型为ISP_BLC_CFG的结构体指针，详细成员变量请查ISP_BLC_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetBlcCfg(FH_UINT32 u32IspDevId, ISP_BLC_CFG *pstBlcCfg);
/*
*   Name: API_ISP_GetBlcCfg
*            获取当前配置的BLC的DRV寄存器值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_BLC_Cfg * pstBlcCfg
*            类型为ISP_BLC_Cfg的结构体指针，详细成员变量请查看ISP_BLC_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetBlcCfg(FH_UINT32 u32IspDevId, ISP_BLC_CFG *pstBlcCfg);
/*
*   Name: API_ISP_GetBlcStatus
*            获取当前BLC的status寄存器值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_BLC_STAT *pstBlcStatus
*            类型为ISP_BLC_STAT的结构体指针，详细成员变量请查看ISP_BLC_STAT结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetBlcStatus(FH_UINT32 u32IspDevId, ISP_BLC_STAT *pstBlcStatus);
/** GB*/
/*
*   Name: API_ISP_SetGbCfg
*            配置GB的DRV寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_GB_CFG *pstGbCfg
*            类型为ISP_GB_CFG的结构体指针，详细成员变量请查看ISP_GB_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetGbCfg(FH_UINT32 u32IspDevId, ISP_GB_CFG *pstGbCfg);
/*
*   Name: API_ISP_GetGbCfg
*            获取当前配置的GB的DRV寄存器值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_GB_CFG *pstGbCfg
*            类型为ISP_GB_CFG的结构体指针，详细成员变量请查看ISP_GB_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetGbCfg(FH_UINT32 u32IspDevId, ISP_GB_CFG *pstGbCfg);
/** DPC*/
/*
*   Name: API_ISP_SetDpcCfg
*            配置动态DPC的DRV寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_DPC_CFG *pstDpcCfg
*            类型为ISP_DPC_CFG的结构体指针，详细成员变量请查看ISP_DPC_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetDpcCfg(FH_UINT32 u32IspDevId, ISP_DPC_CFG *pstDpcCfg);
/*
*   Name: API_ISP_GetDpcCfg
*            获取当前配置的动态DPC的DRV寄存器值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_DPC_CFG *pstDpcCfg
*            类型为ISP_DPC_CFG的结构体指针，详细成员变量请查看ISP_DPC_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetDpcCfg(FH_UINT32 u32IspDevId, ISP_DPC_CFG *pstDpcCfg);
/** LSC*/
/*
*   Name: API_ISP_SetLscCfg
*            配置lsc参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_LSC_CFG *pstLscCfg
*            类型为ISP_LSC_CFG的结构体指针，详细成员变量请查看ISP_LSC_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetLscCfg(FH_UINT32 u32IspDevId, ISP_LSC_CFG *pstLscCfg);
/** NR3D*/
/*
*   Name: API_ISP_SetNr3dCfg
*            配置NR3D的DRV寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_NR3D_CFG *pstNr3dCfg
*            类型为ISP_NR3D_CFG的结构体指针，详细成员变量请查看ISP_NR3D_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetNr3dCfg(FH_UINT32 u32IspDevId, ISP_NR3D_CFG *pstNr3dCfg);
/*
*   Name: API_ISP_GetNr3dCfg
*            获取当前配置的NR3D的DRV寄存器值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_NR3D_CFG *pstNr3dCfg
*            类型为ISP_NR3D_CFG的结构体指针，详细成员变量请查看ISP_NR3D_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetNr3dCfg(FH_UINT32 u32IspDevId, ISP_NR3D_CFG *pstNr3dCfg);
/*
*   Name: API_ISP_SetNr3dCoeffCfg
*            该函数可以配置NR3D的Coeff曲线，通过配置四对坐标可以配置Coeff曲线。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const ISP_NR3D_COEFF_CFG *pstNr3dCoeffCfg
*            类型为ISP_NR3D_COEFF_CFG的结构体指针，详细成员变量请查看ISP_NR3D_COEFF_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetNr3dCoeffCfg(FH_UINT32 u32IspDevId, ISP_NR3D_COEFF_CFG *pstNr3dCoeffCfg);
/** NR3D_MD*/
/*
*   Name: API_ISP_GetNr3dMdStat
*            获取Nr3d_MD模块的数据
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_NR3D_MD_STAT* pstNr3dMdStat
*            类型为ISP_NR3D_MD_STAT的结构体指针，详细成员变量请查看ISP_NR3D_MD_STAT结构体定义。
*
*   Return:
*           0
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetNr3dMdStat(FH_UINT32 u32IspDevId, ISP_NR3D_MD_STAT* pstNr3dMdStat);
/*
*   Name: API_ISP_SetNr3dMdStatCfg
*            配置Nr3d_MD模块的统计阈值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_NR3D_MD_STAT_CFG* pstNr3dMdStatCfg
*            类型为ISP_NR3D_MD_STAT_CFG的结构体指针，详细成员变量请查看ISP_NR3D_MD_STAT_CFG结构体定义。
*
*   Return:
*           0
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetNr3dMdStatCfg(FH_UINT32 u32IspDevId, ISP_NR3D_MD_STAT_CFG* pstNr3dMdStatCfg);
/*
*   Name: API_ISP_GetNr3dMdStatCFG
*            获取Nr3d_MD模块的统计阈值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_NR3D_MD_STAT_CFG* pstNr3dMdStatCfg
*            类型为ISP_NR3D_MD_STAT_CFG的结构体指针，详细成员变量请查看ISP_NR3D_MD_STAT_CFG结构体定义。
*
*   Return:
*           0
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetNr3dMdStatCfg(FH_UINT32 u32IspDevId, ISP_NR3D_MD_STAT_CFG* pstNr3dMdStatCfg);
/*
*   Name: API_ISP_SetNr3dMdStatCfg2
*         配置Nr3d_MD模块的统计阈值，可用mdstatType控制类型
*
*   Parameters:
*
*       [IN] FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] NR3D_MDSTAT_TYPE mdstatType
*            NR3D MdStat 类型，详细成员请查看枚举NR3D_MDSTAT_TYPE的定义
*
*       [IN] ISP_NR3D_MD_STAT_CFG* pstNr3dMdStatCfg
*            类型为ISP_NR3D_MD_STAT_CFG的结构体指针，详细成员变量请查看ISP_NR3D_MD_STAT_CFG结构体定义。
*
*   Return:
*           0
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetNr3dMdStatCfg2(FH_UINT32 u32IspDevId, NR3D_MDSTAT_TYPE mdstatType, ISP_NR3D_MD_STAT_CFG* pstNr3dMdStatCfg);
/*
*   Name: API_ISP_GetNr3dMdStatCfg2
*            配置Nr3d_MD模块的统计阈值，可用mdstatType控制类型
*
*   Parameters:
*
*       [IN] FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] NR3D_MDSTAT_TYPE mdstatType
*            NR3D MdStat 类型，详细成员请查看枚举NR3D_MDSTAT_TYPE的定义
*
*       [OUT] ISP_NR3D_MD_STAT_CFG* pstNr3dMdStatCfg
*            类型为ISP_NR3D_MD_STAT_CFG的结构体指针，详细成员变量请查看ISP_NR3D_MD_STAT_CFG结构体定义。
*
*   Return:
*           0
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetNr3dMdStatCfg2(FH_UINT32 u32IspDevId, NR3D_MDSTAT_TYPE mdstatType, ISP_NR3D_MD_STAT_CFG* pstNr3dMdStatCfg);
/** NR2D*/
/*
*   Name: API_ISP_SetNr2dCfg
*            配置NR2D的DRV寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_NR2D_CFG *pstNr2dCfg
*            类型为ISP_NR2D_CFG的结构体指针，详细成员变量请查看ISP_NR2D_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetNr2dCfg(FH_UINT32 u32IspDevId, ISP_NR2D_CFG *pstNr2dCfg);
/*
*   Name: API_ISP_GetNr2dCfg
*            获取当前配置的NR2D的DRV寄存器值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_NR2D_CFG *pstNr2dCfg
*            类型为ISP_NR2D_CFG的结构体指针，详细成员变量请查看ISP_NR2D_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetNr2dCfg(FH_UINT32 u32IspDevId, ISP_NR2D_CFG *pstNr2dCfg);
/*
*   Name: API_ISP_SetNr2dDpcCfg
*            配置NR2D DPC的DRV寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_NR2D_DPC_CFG *pstNr2dDpcCfg
*            类型为ISP_NR2D_DPC_CFG的结构体指针，详细成员变量请查看ISP_NR2D_DPC_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetNr2dDpcCfg(FH_UINT32 u32IspDevId, ISP_NR2D_DPC_CFG *pstNr2dDpcCfg);
/*
*   Name: API_ISP_GetNr2dDpcCfg
*            配置NR2D DPC的DRV寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_NR2D_DPC_CFG *pstNr2dDpcCfg
*            类型为ISP_NR2D_DPC_CFG的结构体指针，详细成员变量请查看ISP_NR2D_DPC_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetNr2dDpcCfg(FH_UINT32 u32IspDevId, ISP_NR2D_DPC_CFG *pstNr2dDpcCfg);
/** MD*/
/*
*   Name: API_ISP_GetMdStat
*            获取表征MD模块的运动静止程度的数值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_MD_STAT* pstMdStat
*            类型为ISP_MD_STAT的结构体指针，详细成员变量请查看ISP_MD_STAT结构体定义。
*
*   Return:
*           0
*
*   Note:
*          该接口为保留接口,暂时无用
*       1. 运动块和静止块的数值总和固定，图像划分成384个区域
*       2. 运动块越多，说明图像当前运动的可能性越大，反之则越小
*/
FH_SINT32 FUNC_DEP API_ISP_GetMdStat(FH_UINT32 u32IspDevId, ISP_MD_STAT* pstMdStat);
/** DRC **/
/*
*   Name: API_ISP_SetDrcCfg
*            配置动态范围压缩的DRV寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_DRC_CFG *pstDrcCfg
*            类型为ISP_DRC_CFG的结构体指针，详细成员变量请查看ISP_DRC_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetDrcCfg(FH_UINT32 u32IspDevId, ISP_DRC_CFG *pstDrcCfg);
/*
*   Name: API_ISP_GetDrcCfg
*            获取当前配置的动态范围压缩的DRV寄存器值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_DRC_CFG *pstDrcCfg
*            类型为ISP_DRC_CFG的结构体指针，详细成员变量请查看ISP_DRC_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetDrcCfg(FH_UINT32 u32IspDevId, ISP_DRC_CFG *pstDrcCfg);
/**HLR*/
/*
*   Name: API_ISP_SetHlrCfg
*            配置HLR的DRV寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_HLR_CFG *pstHlrCfg
*            类型为ISP_HLR_CFG的结构体指针，详细成员变量请查看ISP_HLR_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetHlrCfg(FH_UINT32 u32IspDevId, ISP_HLR_CFG *pstHlrCfg);
/*
*   Name: API_ISP_GetHlrCfg
*            获取当前配置的HLR的DRV寄存器值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_HLR_CFG *pstHlrCfg
*            类型为ISP_HLR_CFG的结构体指针，详细成员变量请查看ISP_HLR_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetHlrCfg(FH_UINT32 u32IspDevId, ISP_HLR_CFG *pstHlrCfg);
/**IE*/
/*
*   Name: API_ISP_SetContrastCfg
*            配置对比度相关的DRV寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_CONTRAST_CFG *pstContrastCfg
*            类型为ISP_CONTRAST_CFG的结构体指针，详细成员变量请查看ISP_CONTRAST_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetContrastCfg(FH_UINT32 u32IspDevId, ISP_CONTRAST_CFG *pstContrastCfg);
/*
*   Name: API_ISP_GetContrastCfg
*            获取当前配置的对比度相关的DRV寄存器值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_CONTRAST_CFG *pstContrastCfg
*            类型为ISP_CONTRAST_CFG的结构体指针，详细成员变量请查看ISP_CONTRAST_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetContrastCfg(FH_UINT32 u32IspDevId, ISP_CONTRAST_CFG *pstContrastCfg);
/*
*   Name: API_ISP_SetBrightnessCfg
*            配置亮度相关的DRV寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_BRIGHTNESS_CFG *pstBrightnessCfg
*            类型为ISP_BRIGHTNESS_CFG的结构体指针，详细成员变量请查看ISP_BRIGHTNESS_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetBrightnessCfg(FH_UINT32 u32IspDevId, ISP_BRIGHTNESS_CFG *pstBrightnessCfg);
/*
*   Name: API_ISP_GetBrightnessCfg
*            获取当前配置的亮度相关的DRV寄存器值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_BRIGHTNESS_CFG *pstBrightnessCfg
*            类型为ISP_BRIGHTNESS_CFG的结构体指针，详细成员变量请查看ISP_BRIGHTNESS_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetBrightnessCfg(FH_UINT32 u32IspDevId, ISP_BRIGHTNESS_CFG *pstBrightnessCfg);
/**CE*/
/*
*   Name: API_ISP_SetSaturation
*            配置对比度相关的DRV寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_SAT_CFG *pstCeCfg
*            类型为ISP_SAT_CFG的结构体指针，详细成员变量请查看ISP_SAT_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetSaturation(FH_UINT32 u32IspDevId, ISP_SAT_CFG *pstCeCfg);
/*
*   Name: API_ISP_GetSaturation
*            获取当前配置的对比度相关的DRV寄存器值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_SAT_CFG *pstCeCfg
*            类型为ISP_SAT_CFG的结构体指针，详细成员变量请查看ISP_SAT_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetSaturation(FH_UINT32 u32IspDevId, ISP_SAT_CFG *pstCeCfg);
/**APC*/
/*
*   Name: API_ISP_SetApcCfg
*            配置锐度相关的DRV寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_APC_CFG *pstApcCfg
*            类型为ISP_APC_CFG的结构体指针，详细成员变量请查看ISP_APC_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetApcCfg(FH_UINT32 u32IspDevId, ISP_APC_CFG *pstApcCfg);
/*
*   Name: API_ISP_GetApcCfg
*            获取当前配置的锐度相关的DRV寄存器值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_APC_CFG *pstApcCfg
*            类型为ISP_APC_CFG的结构体指针，详细成员变量请查看ISP_APC_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetApcCfg(FH_UINT32 u32IspDevId, ISP_APC_CFG *pstApcCfg);
/*
*   Name: API_ISP_SetApcMtCfg
*            配置apc与nr3d的联动参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_APC_MT_CFG *pstApcMtCfg
*            类型为ISP_APC_MT_CFG的结构体指针，详细成员变量请查看ISP_APC_MT_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetApcMtCfg(FH_UINT32 u32IspDevId, ISP_APC_MT_CFG *pstApcMtCfg);
/*
*   Name: API_ISP_GetApcMtCfg
*            获取APC与nr3d的联动参数。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_APC_MT_CFG *pstApcMtCfg
*             类型为ISP_APC_MT_CFG的结构体指针，详细成员变量请查看ISP_APC_MT_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetApcMtCfg(FH_UINT32 u32IspDevId, ISP_APC_MT_CFG *pstApcMtCfg);
/*
*   Name: API_ISP_SetSharpenStatCfg
*            配置SHARPEN统计窗
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] SHARPEN_STAT_CFG *pstSharpenStatCfg
*            类型为SHARPEN_STAT_CFG的结构体指针，详细成员变量请查看SHARPEN_STAT_CFG结构体定义。
*
*   Return:
*            0(正确)
*            空指针异常(ERROR_ISP_NULL_POINTER)，传入参数超过限制范围(ERROR_ISP_PARA_OUTOF_RANGE)，0(正确)
*/
FH_SINT32 API_ISP_SetSharpenStatCfg(FH_UINT32 u32IspDevId, SHARPEN_STAT_CFG *pstSharpenStatCfg);
/*
*   Name: API_ISP_GetSharpenStatCfg
*            获取当前SHARPEN配置的统计窗口信息
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] SHARPEN_STAT_CFG *pstSharpenStatCfg
*            类型为SHARPEN_STAT_CFG的结构体指针，详细成员变量请查看SHARPEN_STAT_CFG结构体定义。
*
*   Return:
*            0(正确)
*            空指针异常(ERROR_ISP_NULL_POINTER)
*/
FH_SINT32 API_ISP_GetSharpenStatCfg(FH_UINT32 u32IspDevId, SHARPEN_STAT_CFG *pstSharpenStatCfg);
/*
*   Name: API_ISP_GetSharpenStat
*            获取SHARPEN统计信息即锐度统计信息
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] SHARPEN_STAT *pstSharpenStat
*            类型为SHARPEN_STAT的结构体指针，详细成员变量请查看SHARPEN_STAT结构体定义。
*
*   Return:
*            0(正确)
*            空指针异常(ERROR_ISP_NULL_POINTER)，
*/
FH_SINT32 API_ISP_GetSharpenStat(FH_UINT32 u32IspDevId, SHARPEN_STAT *pstSharpenStat);
/**GAMMA*/
/*
*   Name: API_ISP_SetGammaCfg
*            配置gamma相关的DRV寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_GAMMA_CFG *pstGammaCfg
*            类型为ISP_GAMMA_CFG的结构体指针，详细成员变量请查看ISP_GAMMA_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetGammaCfg(FH_UINT32 u32IspDevId, ISP_GAMMA_CFG *pstGammaCfg);
/*
*   Name: API_ISP_GetGammaCfg
*            获取当前配置的gamma相关的DRV寄存器值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_GAMMA_CFG *pstGammaCfg
*            类型为ISP_GAMMA_CFG的结构体指针，详细成员变量请查看ISP_GAMMA_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetGammaCfg(FH_UINT32 u32IspDevId, ISP_GAMMA_CFG *pstGammaCfg);

/**YCNR*/
/*
*   Name: API_ISP_SetYnrCfg
*            配置YNR相关的DRV寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_YNR_CFG *pstYnrCfg
*            类型为ISP_YNR_CFG的结构体指针，详细成员变量请查看ISP_YNR_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetYnrCfg(FH_UINT32 u32IspDevId, ISP_YNR_CFG *pstYnrCfg);
/*
*   Name: API_ISP_GetYnrCfg
*            获取当前配置的YNR相关的DRV寄存器值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_YNR_CFG *pstYnrCfg
*            类型为ISP_YNR_CFG的结构体指针，详细成员变量请查看ISP_YNR_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetYnrCfg(FH_UINT32 u32IspDevId, ISP_YNR_CFG *pstYnrCfg);
/*
*   Name: API_ISP_SetCnrCfg
*            配置CNR相关的DRV寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_CNR_CFG *pstCnrCfg
*            类型为ISP_CNR_CFG的结构体指针，详细成员变量请查看ISP_CNR_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetCnrCfg(FH_UINT32 u32IspDevId, ISP_CNR_CFG *pstCnrCfg);
/*
*   Name: API_ISP_GetCnrCfg
*            获取当前配置的CNR相关的DRV寄存器值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_CNR_CFG *pstCnrCfg
*            类型为ISP_CNR_CFG的结构体指针，详细成员变量请查看ISP_CNR_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetCnrCfg(FH_UINT32 u32IspDevId, ISP_CNR_CFG *pstCnrCfg);
/**PURPLE*/
/*
*   Name: API_ISP_SetAntiPurpleBoundary
*            配置去紫边相关的DRV寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_PURPLEFRI_CFG *pstPurplefriCfg
*            类型为ISP_PURPLEFRI_CFG的结构体指针，详细成员变量请查看ISP_PURPLEFRI_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetAntiPurpleBoundary(FH_UINT32 u32IspDevId, ISP_PURPLEFRI_CFG *pstPurplefriCfg);
/*
*   Name: API_ISP_GetAntiPurpleBoundary
*            获取当前配置的去紫边的DRV寄存器值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_PURPLEFRI_CFG *pstPurplefriCfg
*            类型为ISP_PURPLEFRI_CFG的结构体指针，详细成员变量请查看ISP_PURPLEFRI_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetAntiPurpleBoundary(FH_UINT32 u32IspDevId, ISP_PURPLEFRI_CFG *pstPurplefriCfg);
/**Debug Interface**/
/*
*   Name: API_ISP_ReadMallocedMem
*            读取指定偏移的VMM分配的内存的值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] FH_SINT32 intMemSlot
*            分配的内存的类型，决定基地址。
*
*       [IN]  FH_UINT32 offset
*            偏移地址，选定的内存会给定其基地址。
*
*       [OUT]  FH_UINT32 *pstData
*            存放读取到数据的地址。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_ReadMallocedMem(FH_UINT32 u32IspDevId, FH_SINT32 intMemSlot, FH_UINT32 offset, FH_UINT32 *pstData);
/*
*   Name: API_ISP_WriteMallocedMem
*            写指定偏移的VMM分配的内存的值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] FH_SINT32 intMemSlot
*            分配的内存的类型，决定基地址。
*
*       [IN]  FH_UINT32 offset
*            偏移地址，选定的内存会给定其基地址。
*
*       [IN]  FH_UINT32 *pstData
*            目标值，将该值写入目标地址。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_WriteMallocedMem(FH_UINT32 u32IspDevId, FH_SINT32 intMemSlot, FH_UINT32 offset, FH_UINT32 *pstData);
/*
*   Name: API_ISP_ImportMallocedMem
*            导入指定大小的数据到指定偏移的VMM分配的内存
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] FH_SINT32 intMemSlot
*            分配的内存的类型，决定基地址。
*
*       [IN]  FH_UINT32 offset
*            偏移地址，选定的内存会给定其基地址。
*
*       [IN]  FH_UINT32 *pstSrc
*            导入数据的起始地址
*
*       [IN]  FH_UINT32 size
*            需要导入的数据大小
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_ImportMallocedMem(FH_UINT32 u32IspDevId, FH_SINT32 intMemSlot, FH_UINT32 offset, FH_UINT32 *pstSrc, FH_UINT32 size);
/*
*   Name: API_ISP_ExportMallocedMem
*            从指定偏移的VMM分配的内存导出指定大小的数据
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] FH_SINT32 intMemSlot
*            分配的内存的类型，决定基地址。
*
*       [IN]  FH_UINT32 offset
*            偏移地址，选定的内存会给定其基地址。
*
*       [OUT]  FH_UINT32 *pstDst
*            存放导出数据的起始地址
*
*       [IN]  FH_UINT32 size
*            需要导出的数据大小
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_ExportMallocedMem(FH_UINT32 u32IspDevId, FH_SINT32 intMemSlot, FH_UINT32 offset, FH_UINT32 *pstDst, FH_UINT32 size);
/*
*   Name: API_ISP_GetVIState
*            获取当前ISP的一些运行状态信息
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_VI_STAT_S *pstStat
*            类型为ISP_VI_STAT_S的结构体指针，详细成员变量请查看ISP_VI_STAT_S结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetVIState(FH_UINT32 u32IspDevId, ISP_VI_STAT_S *pstStat);
/*
*   Name: API_ISP_SetSensorFrameRate
*            配置SENSOR的垂直消隐至指定倍率
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] int m
*            垂直消隐的倍率,U.8,也即0x100表示一倍
*
*   Return:
*            0(正确)
*   Note:
*       当ae策略关闭时可使用该函数,策略打开时的帧消隐由策略参数控制,支持降帧+升帧（注意不能超过初始配置帧率，且需要检查intt是否满足切换后的帧率）
*/
FH_SINT32 API_ISP_SetSensorFrameRate(FH_UINT32 u32IspDevId, int m);
/*
*   Name: API_ISP_Dump_Param
*            拷贝所有DRV寄存器值到指定地址
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] FH_UINT32 *addr
*            存放DRV参数的地址
*
*       [IN] FH_UINT32 *size
*            拷贝数据的大小
*
*   Return:
*            0(正确)
*   Note:
*       无
*/
FH_SINT32 API_ISP_Dump_Param(FH_UINT32 u32IspDevId, FH_UINT32 *addr,FH_UINT32 *size);
/**MIRROR*/
/*
*   Name: API_ISP_MirrorEnable
*            ISP的MIRROR模块，可以实现镜像,镜像后的pattern不需要配置,软件会自动转换。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] MIRROR_CFG_S *pMirror
*            类型为MIRROR_CFG_S的结构体指针，详细成员变量请查看MIRROR_CFG_S结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_MirrorEnable(FH_UINT32 u32IspDevId, MIRROR_CFG_S *pMirror);
/*
*   Name: API_ISP_SetMirrorAndflip
*            配置SENSOR的镜像和水平翻转。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] FH_BOOL mirror
*            镜像(mirror=1)，正常(mirror=0)
*
*       [IN]  FH_BOOL flip
*            水平翻转(flip=1)，正常(flip=0)
*
*   Return:
*            0(正确)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetMirrorAndflip(FH_UINT32 u32IspDevId, FH_BOOL mirror, FH_BOOL flip);
/*
*   Name: API_ISP_SetMirrorAndflipEx
*            配置SENSOR的镜像和水平翻转，同时会更改ISP中相关bayer配置。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] FH_BOOL mirror
*            镜像(mirror=1)，正常(mirror=0)
*
*       [IN]  FH_BOOL flip
*            水平翻转(flip=1)，正常(flip=0)
*
*       [IN] FH_UINT32 bayer
*            镜像和水平翻转影响之后的BAYER格式。
*
*   Return:
*            0(正确)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetMirrorAndflipEx(FH_UINT32 u32IspDevId, FH_BOOL mirror, FH_BOOL flip,FH_UINT32 bayer);
/*
*   Name: API_ISP_GetMirrorAndflip
*            获取当前SENSOR镜像和水平翻转的状态
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] FH_BOOL *mirror
*            镜像(mirror=1)，正常(mirror=0)
*
*       [OUT]  FH_BOOL *flip
*            水平翻转(flip=1)，正常(flip=0)
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetMirrorAndflip(FH_UINT32 u32IspDevId, FH_BOOL *mirror, FH_BOOL *flip);

/**POST_GAIN*/
/*
*   Name: API_ISP_SetPostGain
*            设置postgain参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const ISP_POST_GAIN_CFG *pstPostGainCfg
*            类型为ISP_POST_GAIN_CFG的结构体指针，详细成员变量请查看ISP_POST_GAIN_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       post_gain模块目前作为一部分数字增益使用,不建议随意配置增益,防止ae策略出现一些异常
*/
FH_SINT32 API_ISP_SetPostGain(FH_UINT32 u32IspDevId, const ISP_POST_GAIN_CFG *pstPostGainCfg);
/*
*   Name: API_ISP_GetPostGain
*           获取postgain参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_POST_GAIN_CFG *pstPostGainCfg
*            类型为ISP_POST_GAIN_CFG的结构体指针，详细成员变量请查看ISP_POST_GAIN_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetPostGain(FH_UINT32 u32IspDevId, ISP_POST_GAIN_CFG *pstPostGainCfg);
/**SMART IR**/
/*
*   Name: API_ISP_GetIrStatus
*            获取日夜状态值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] FH_UINT32 *u32irStatus
*            详见枚举ISP_IRSTATUS_E。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetIrStatus(FH_UINT32 u32IspDevId, FH_UINT32 *u32irStatus);
/*
*   Name: API_ISP_SetSmartIrCfg
*            设置智能切换日夜的参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] SMART_IR_CFG *pstSmartIrCfg
*            类型为SMART_IR_CFG的结构体指针，详细成员变量请查看SMART_IR_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetSmartIrCfg(FH_UINT32 u32IspDevId, SMART_IR_CFG *pstSmartIrCfg);
/*
*   Name: API_ISP_GetSmartIrCfg
*            获取智能切换日夜的参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] SMART_IR_CFG *pstSmartIrCfg
*            类型为SMART_IR_CFG的结构体指针，详细成员变量请查看SMART_IR_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetSmartIrCfg(FH_UINT32 u32IspDevId, SMART_IR_CFG *pstSmartIrCfg);
/**OTHERS*/
/*
*   Name: API_ISP_SetCropInfo
*            配置输入幅面的水平和垂直的处理偏移。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] int offset_x
*            水平方向的偏移
*
*       [IN] int offset_y
*            垂直方向的偏移。
*
*   Return:
*            0(正确)
*        -3004(偏移值超过输入幅面或者为奇数)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetCropInfo(FH_UINT32 u32IspDevId, int offset_x,int offset_y);
/*
*   Name: *FH_ISP_Version
*            获取、打印ISP版本号。
*
*   Parameters:
*
*       [IN] FH_UINT32 print_enable
*            打印ISP库版本号(1)，不打印ISP库版本号(0)
*
*   Return:
*            ISP库版本号的字符串。
*   Note:
*       无
*/
FH_CHAR *FH_ISP_Version(FH_UINT32 print_enable);
/*
*   Name: *FH_ISPCORE_Version
*            获取、打印ISP版本号。
*
*   Parameters:
*
*       [IN]  void
*
*   Return:
*            0(正确)
*   Note:
*       无
*/
FH_SINT32 FH_ISPCORE_Version(void);
/**FAST_BOOT**/
/*
*   Name: API_ISP_RegisterPicStartCallback
*            在ISP PicStartB的时候的回调函数，可以用在RTT下统计时间。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ispIntCallback cb
*            void型的函数指针，指向回调函数的位置。
*
*   Return:
*            0(正确)
*   Note:
*       无
*/
FH_SINT32 API_ISP_RegisterPicStartCallback(FH_UINT32 u32IspDevId, ispIntCallback cb);
/*
*   Name: API_ISP_RegisterPicEndCallback
*            在ISP PicEndP的时候的回调函数，可以用在RTT下统计时间。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ispIntCallback cb
*            void型的函数指针，指向回调函数的位置。
*
*   Return:
*            0(正确)
*   Note:
*       无
*/
FH_SINT32 API_ISP_RegisterPicEndCallback(FH_UINT32 u32IspDevId, ispIntCallback cb);
/*
*   Name: API_ISP_RegisterIspInitCfgCallback
*            ISP硬件初始化回调函数，其执行位置在API_ISP_Init之前，此时ISP未启动，流程中仅执行一次。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ispInitCfgCallback cb
*            void型的函数指针，指向回调函数的位置。
*
*   Return:
*            0(正确)
*   Note:
*       无
*/
FH_SINT32 API_ISP_RegisterIspInitCfgCallback(FH_UINT32 u32IspDevId, ispInitCfgCallback cb);
/*
*   Name: API_ISP_SetStrategyInitCfg
*            初始化所有效果策略参数值（除AE/AWB外），调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const STRATEGY_INIT_CFG *pstStrategyInitCfg
*            类型为STRATEGY_INIT_CFG的结构体指针，详细成员变量请查看STRATEGY_INIT_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetStrategyInitCfg(FH_UINT32 u32IspDevId, STRATEGY_INIT_CFG *pstStrategyInitCfg);
/*
*   Name: API_ISP_SetBlcInitCfg
*            初始化BLC所有通道的减DC值，调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const BLC_INIT_CFG *pstBlcInitCfg
*            类型为BLC_INIT_CFG的结构体指针，详细成员变量请查看BLC_INIT_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetBlcInitCfg(FH_UINT32 u32IspDevId, const BLC_INIT_CFG *pstBlcInitCfg);
/*
*   Name: API_ISP_SetWbInitCfg
*            初始化WB三个通道增益值，调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const WB_INIT_CFG *pstWbInitCfg
*            类型为WB_INIT_CFG的结构体指针，详细成员变量请查看WB_INIT_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetWbInitCfg(FH_UINT32 u32IspDevId, const WB_INIT_CFG *pstWbInitCfg);
/*
*   Name: API_ISP_SetCcmInitCfg
*            初始化CCM矩阵硬件寄存器，调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] CCM_INIT_CFG *pstCcmInitCfg
*            类型为DPC_INIT_CFG的结构体指针，详细成员变量请查看DPC_INIT_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetCcmInitCfg(FH_UINT32 u32IspDevId, const CCM_INIT_CFG *pstCcmInitCfg);
/*
*   Name: API_ISP_SetDpcInitCfg
*            初始化DPC硬件寄存器，调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const DPC_INIT_CFG *pstDpcInitCfg
*            类型为DPC_INIT_CFG的结构体指针，详细成员变量请查看DPC_INIT_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetDpcInitCfg(FH_UINT32 u32IspDevId, const DPC_INIT_CFG *pstDpcInitCfg);
/*
*   Name: API_ISP_SetApcInitCfg
*            初始化APC硬件寄存器，调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const APC_INIT_CFG *pstApcInitCfg
*            类型为APC_INIT_CFG的结构体指针，详细成员变量请查看APC_INIT_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetApcInitCfg(FH_UINT32 u32IspDevId, const APC_INIT_CFG *pstApcInitCfg);
/*
*   Name: API_ISP_SetYnrInitCfg
*            初始化YNR硬件寄存器，调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const YNR_INIT_CFG *pstYnrInitCfg
*            类型为YNR_INIT_CFG的结构体指针，详细成员变量请查看YNR_INIT_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       不支持
*/
FH_SINT32 FUNC_DEP API_ISP_SetYnrInitCfg(FH_UINT32 u32IspDevId, const YNR_INIT_CFG *pstYnrInitCfg);
/*
*   Name: API_ISP_SetCtrInitCfg
*            配置对比度值到硬件寄存器，调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const CTR_INIT_CFG *pstCtrInitCfg
*            类型为CTR_INIT_CFG的结构体指针，详细成员变量请查看CTR_INIT_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetCtrInitCfg(FH_UINT32 u32IspDevId, const CTR_INIT_CFG *pstCtrInitCfg);
/*
*   Name: API_ISP_SetSatInitCfg
*            配置饱和度值到硬件寄存器，调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const SAT_INIT_CFG *pstSatInitCfg
*            SAT_INIT_CFG的结构体指针，详细参数查看结构体SAT_INIT_CFG注释。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetSatInitCfg(FH_UINT32 u32IspDevId, const SAT_INIT_CFG *pstSatInitCfg);
/*
*   Name: API_ISP_SetGammaModeInitCfg
*            根据传入的gamma mode值，配置相应硬件寄存器，调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const GAMMA_MODE_INIT_CFG *pstGammaModeInitCfg
*            目标配置的gamma mode。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetGammaModeInitCfg(FH_UINT32 u32IspDevId, const GAMMA_MODE_INIT_CFG *pstGammaModeInitCfg);
/*
*   Name: API_ISP_SetCGammaInitCfg
*            配置cgamma table到硬件寄存器，调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const GAMMA_INIT_CFG *pstCGammaInitCfg
*            结构体GAMMA_INIT_CFG的指针，目标配置cgamma表。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       暂不支持
*/
FH_SINT32 FUNC_DEP API_ISP_SetCGammaInitCfg(FH_UINT32 u32IspDevId, const GAMMA_INIT_CFG *pstCGammaInitCfg);
/*
*   Name: API_ISP_SetYGammaInitCfg
*            配置ygamma table到硬件寄存器，调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const GAMMA_INIT_CFG *pstYGammaInitCfg
*            结构体GAMMA_INIT_CFG的指针，目标配置的ygamma表。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetYGammaInitCfg(FH_UINT32 u32IspDevId, const GAMMA_INIT_CFG *pstYGammaInitCfg);
/*
*   Name: API_ISP_SetIspGainInitCfg
*            配置Isp Gain到硬件寄存器，调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const ISP_GAIN_INIT_CFG *pstIspGainInitCfg
*            结构体ISP_GAIN_INIT_CFG的指针，目标配置的Isp Gain。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetIspGainInitCfg(FH_UINT32 u32IspDevId, const ISP_GAIN_INIT_CFG *pstIspGainInitCfg);
/*
*   Name: API_ISP_GetFirstStageParam
*            获取第一阶段传给小图isp效果参数以及拉高gpio延时时间等
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_SENSOR_COMMON_CMD_DATA0 *pstFirstStageParamCfg
*            类型为ISP_SENSOR_COMMON_CMD_DATA0的结构体指针，详细成员变量请查看ISP_SENSOR_COMMON_CMD_DATA0结构体定义。
*
*   Return:
*        0(正确返回)
*        -1(sensor相关的回调函数未被注册)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetFirstStageParam(FH_UINT32 u32IspDevId, ISP_SENSOR_COMMON_CMD_DATA0 *pstFirstStageParamCfg);
/**CCM**/
/*
*   Name: API_ISP_SetCcmCfg
*            配置色彩空间转换的配置。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] CCM_TABLE *pstCcmCfg
*             类型为CCM_TABLE的结构体指针，详细成员变量请查看CCM_TABLE结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetCcmCfg(FH_UINT32 u32IspDevId, CCM_TABLE *pstCcmCfg);
/*
*   Name: API_ISP_GetCcmCfg
*           获取色彩空间转换的配置。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] CCM_TABLE *pstCcmCfg
*             类型为CCM_TABLE的结构体指针，详细成员变量请查看CCM_TABLE结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetCcmCfg(FH_UINT32 u32IspDevId, CCM_TABLE *pstCcmCfg);
/*
*   Name: API_ISP_SetCcmHwCfg
*            配置ccm参数至硬件寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const ISP_CCM_CFG *pstCcmCfg
*             类型为ISP_CCM_CFG的结构体指针，详细成员变量请查看ISP_CCM_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetCcmHwCfg(FH_UINT32 u32IspDevId, const ISP_CCM_CFG *pstCcmCfg);
/*
*   Name: API_ISP_GetCcmHwCfg
*            从硬件寄存器获取ccm参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_CCM_CFG *pstCcmCfg
*            类型为ISP_CCM_CFG的结构体指针，详细成员变量请查看ISP_CCM_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetCcmHwCfg(FH_UINT32 u32IspDevId, ISP_CCM_CFG *pstCcmCfg);
/*
*   Name: API_ISP_SetAcrCfg
*            设置ccm自动弱化参数。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_ACR_CFG *pstAcrCfg
*            类型为ISP_ACR_CFG的结构体指针，详细成员变量请查看ISP_ACR_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetAcrCfg(FH_UINT32 u32IspDevId, ISP_ACR_CFG *pstAcrCfg);
/*
*   Name: API_ISP_GetAcrCfg
*            获取ccm自动弱化参数。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_ACR_CFG *pstAcrCfg
*            类型为ISP_ACR_CFG的结构体指针，详细成员变量请查看ISP_ACR_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetAcrCfg(FH_UINT32 u32IspDevId, ISP_ACR_CFG *pstAcrCfg);
/**LCE**/
/*
*   Name: API_ISP_SetLceCfg
*           设置局部对比度增强参数。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] ISP_LCE_CFG *pstLceCfg
*             类型为ISP_LCE_CFG的结构体指针，详细成员变量请查看ISP_LCE_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetLceCfg(FH_UINT32 u32IspDevId, ISP_LCE_CFG *pstLceCfg);
/*
*   Name: API_ISP_GetLceCfg
*           获得局部对比度增强参数。
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_LCE_CFG *pstLceCfg
*             类型为ISP_LCE_CFG的结构体指针，详细成员变量请查看ISP_LCE_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetLceCfg(FH_UINT32 u32IspDevId, ISP_LCE_CFG *pstLceCfg);
/**YCGAIN**/
/*
*   Name: API_ISP_SetYCGainCfg
*           设置rgb2yuv配置矩阵
*
*   Parameters:
*
*       [IN] FH_UINT32 u32IspDevId
*            ISP的设备号，通过它选择配置不同的ISP
*
*       [OUT] ISP_YC_GAIN_CFG *pstYcGainCfg
*             类型为ISP_YC_GAIN_CFG的结构体指针，详细成员变量请查看ISP_YC_GAIN_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetYCGainCfg(FH_UINT32 u32IspDevId, ISP_YC_GAIN_CFG *pstYcGainCfg);
/*
*   Name: API_ISP_GetYCGainCfg
*           获取rgb2yuv配置矩阵
*
*   Parameters:
*
*       [IN] FH_UINT32 u32IspDevId
*            ISP的设备号，通过它选择配置不同的ISP
*
*       [OUT] ISP_YC_GAIN_CFG *pstYcGainCfg
*             类型为ISP_YC_GAIN_CFG的结构体指针，详细成员变量请查看ISP_YC_GAIN_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetYCGainCfg(FH_UINT32 u32IspDevId, ISP_YC_GAIN_CFG *pstYcGainCfg);
/** STATISTICS **/
/*
*   Name: API_ISP_GetAwbAdvStat
*            获取awb高级统计信息
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_AWB_ADV_STAT * pstAwbAdvStat
*            类型为ISP_AWB_ADV_STAT的结构体指针，详细成员变量请查看ISP_AWB_ADV_STAT结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       统计窗个数默认16*16,也可通过API_ISP_SetAwbAdvStatCfg进行重新配置,最大支持32*32
*       使用示例:
*              ISP_AWB_ADV_STAT awbAdvStat[256];  // 256是默认窗口配置,若改动的话通过API_ISP_GetAwbAdvStatCfg获取当前统计窗个数
*              API_ISP_GetAwbAdvStat(awbAdvStat);
*/
FH_SINT32 API_ISP_GetAwbAdvStat(FH_UINT32 u32IspDevId, ISP_AWB_ADV_STAT * pstAwbAdvStat);
/*
*   Name: API_ISP_SetAwbAdvStatCfg
*            设置awb高级统计配置相关参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const ISP_AWB_ADV_STAT_CFG *pstAwbStatCfg
*            类型为ISP_AWB_ADV_STAT_CFG的结构体指针，详细成员变量请查看ISP_AWB_ADV_STAT_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetAwbAdvStatCfg(FH_UINT32 u32IspDevId, const ISP_AWB_ADV_STAT_CFG *pstAwbStatCfg);
/*
*   Name: API_ISP_GetAwbAdvStatCfg
*            获取awb高级统计配置相关参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_AWB_ADV_STAT_CFG *pstAwbStatCfg
*            类型为ISP_AWB_ADV_STAT_CFG的结构体指针，详细成员变量请查看ISP_AWB_ADV_STAT_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetAwbAdvStatCfg(FH_UINT32 u32IspDevId, ISP_AWB_ADV_STAT_CFG *pstAwbStatCfg);
/*
*   Name: API_ISP_GetHistStat
*            获取hist统计结果
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_HIST_STAT *psthistStat
*            类型为ISP_HIST_STAT的结构体指针，详细成员变量请查看ISP_HIST_STAT结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetHistStat(FH_UINT32 u32IspDevId, ISP_HIST_STAT *psthistStat);
/*
*   Name: API_ISP_SetHistStatCfg
*            设置hist统计配置相关参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const ISP_HIST_STAT_CFG *pstHistStatCfg
*            类型为ISP_HIST_STAT_CFG的结构体指针，详细成员变量请查看ISP_HIST_STAT_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetHistStatCfg(FH_UINT32 u32IspDevId, const ISP_HIST_STAT_CFG *pstHistStatCfg);
/*
*   Name: API_ISP_GetHistStatCfg
*            获取hist统计配置相关参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_HIST_STAT_CFG *pstHistStatCfg
*            类型为ISP_HIST_STAT_CFG的结构体指针，详细成员变量请查看ISP_HIST_STAT_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetHistStatCfg(FH_UINT32 u32IspDevId, ISP_HIST_STAT_CFG *pstHistStatCfg);
/*
*   Name: API_ISP_GetGlobeStat
*            获取global统计信息
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] GLOBE_STAT *pstGlobeStat
*            类型为GLOBE_STAT的结构体指针，详细成员变量请查看GLOBE_STAT结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       使用示例:
*               GLOBE_STAT glStat[256];  // 256是默认窗口配置,若改动的话通过API_ISP_GetGlobeStatCfg获取当前统计窗个数
*               API_ISP_GetGlobeStat(glStat);
*/
FH_SINT32 API_ISP_GetGlobeStat(FH_UINT32 u32IspDevId, GLOBE_STAT *pstGlobeStat);
/*
*   Name: API_ISP_SetGlobeStatCfg
*            设置global统计配置相关参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [IN] const GLOBE_STAT_CFG *pstGlobeStatCfg
*            类型为GLOBE_STAT_CFG的结构体指针，详细成员变量请查看GLOBE_STAT_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetGlobeStatCfg(FH_UINT32 u32IspDevId, const GLOBE_STAT_CFG *pstGlobeStatCfg);
/*
*   Name: API_ISP_GetGlobeStatCfg
*            获取global统计配置相关参数
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] GLOBE_STAT_CFG *pstGlobeStatCfg
*            类型为GLOBE_STAT_CFG的结构体指针，详细成员变量请查看GLOBE_STAT_CFG结构体定义。
*
*   Return:
*           0(成功)
*           非0(失败，详见错误码)
*
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetGlobeStatCfg(FH_UINT32 u32IspDevId, GLOBE_STAT_CFG *pstGlobeStatCfg);
/**FC**/
/*
*   Name: API_ISP_SetFcCfg
*            配置去假色相关的DRV寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] FC_CFG *pstFcCfg
*            类型为FC_CFG的结构体指针，详细成员变量请查看FC_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetFcCfg(FH_UINT32 u32IspDevId, FC_CFG *pstFcCfg);
/*
*   Name: API_ISP_GetFcCfg
*            获取当前配置的去假色的DRV寄存器值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] FC_CFG *pstFcCfg
*            类型为FC_CFG的结构体指针，详细成员变量请查看FC_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetFcCfg(FH_UINT32 u32IspDevId, FC_CFG *pstFcCfg);
/**HSV**/
/*
*   Name: API_ISP_SetHsvCfg
*            配置色相和饱和度的自适应转换相关的DRV寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_HSV_CFG *pstHsvCfg
*            类型为ISP_HSV_CFG的结构体指针，详细成员变量请查看ISP_HSV_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetHsvCfg(FH_UINT32 u32IspDevId, ISP_HSV_CFG *pstHsvCfg);
/*
*   Name: API_ISP_GetHsvCfg
*            获取当前配置的颜色校正和饱和度调整的DRV寄存器值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_HSV_CFG *pstHsvCfg
*            类型为ISP_HSV_CFG的结构体指针，详细成员变量请查看ISP_HSV_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetHsvCfg(FH_UINT32 u32IspDevId, ISP_HSV_CFG *pstHsvCfg);
/**LDC**/
/*
*   Name: API_ISP_SetLdcCfg
*            配置LDC矫正相关的DRV寄存器
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_LDC_CFG *pstLdcCfg
*            类型为ISP_LDC_CFG的结构体指针，详细成员变量请查看ISP_LDC_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_SetLdcCfg(FH_UINT32 u32IspDevId, ISP_LDC_CFG *pstLdcCfg);
/*
*   Name: API_ISP_GetLdcCfg
*            获取当前配置的LDC矫正的DRV寄存器值
*
*   Parameters:
*
*       [IN]  FH_UINT32 u32IspDevId
*            ISP的设备号
*
*       [OUT] ISP_LDC_CFG *pstLdcCfg
*            类型为ISP_LDC_CFG的结构体指针，详细成员变量请查看ISP_LDC_CFG结构体定义。
*
*   Return:
*            0(正确)
*            非0(失败，详见错误码)
*   Note:
*       无
*/
FH_SINT32 API_ISP_GetLdcCfg(FH_UINT32 u32IspDevId, ISP_LDC_CFG *pstLdcCfg);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*_ISP_API_H_*/

