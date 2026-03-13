#ifndef _ISP_COMMON_H_
#define _ISP_COMMON_H_

#include "type_def.h"
#include "bufCtrl.h"
#include "fh_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
#pragma pack(4)

#define MALLOCED_MEM_BASE_ISP (0)

#define MALLOCED_MEM_ISPFPKG    (1)
#define MALLOCED_MEM_ISPFPKG_L  (2)
#define MALLOCED_MEM_ISPBPKG    (4)
#define MALLOCED_MEM_ISPBPKG_L  (5)
#define MALLOCED_MEM_ISPPPKG    (6)
#define MALLOCED_MEM_ISPPPKG_L  (7)

#define MALLOCED_MEM_DRV (3)

#define MALLOCED_ISP_OUT_LUMA   (0x10)

#define GAIN_NODES 12
#define INTT_NODES 4

typedef int (*ispInitCfgCallback)(FH_UINT32 u32Id);
typedef int (*ispIntCallback)(void);
int isp_core_trace_interface(FH_UINT32 u32Id, const char* func);

/**|SYSTEM CONTROL|**/
typedef enum _ISP_BAYER_TYPE_S_ {
    BAYER_RGGB = 0x0, // 色彩模式RGGB
    BAYER_GRBG = 0x1, // 色彩模式GRBG
    BAYER_BGGR = 0x2, // 色彩模式BGGR
    BAYER_GBRG = 0x3, // 色彩模式GBRG
    ISP_BAYER_TYPE_DUMMY=0xffffffff,
} ISP_BAYER_TYPE;

typedef enum _TAB_COLOR_CODE {
    CC_R  = 0,  // 色彩模式R通道
    CC_GR = 1,  // 色彩模式Gr通道
    CC_B  = 2,  // 色彩模式B通道
    CC_GB = 3,  // 色彩模式Gb通道
    TAB_COLOR_CODE_DUMMY =0xffffffff,
} TAB_COLOR_CODE;

typedef enum _CURVE_TYPE_ {
    CURVE_BUILTIN     = 0x0, // 选用内定曲线
    CURVE_USER_DEFINE = 0x1, // 选用用户自定义曲线
    CURVE_TYPE_DUMMY  =0xffffffff,
} CURVE_TYPE;

typedef enum _FRAME_TYPE_ {
    FRAME_LINEAR = 0x0,      //frame类型为线性
    FRAME_WDR_SHORT = 0x1,   //frame类型为WDR的短帧
    FRAME_WDR_LONG  = 0x2,   //frame类型为WDR的长帧
    FRAME_WDR_MERGED = 0x3,  //frame类型为WDR merge
    FRAME_TYPE_DUMMY = 0xffffffff,
} FRAME_TYPE;

typedef enum _DUMP_TYPE_ {
    DUMP_LONG = 0x0,
    DUMP_SHORT = 0x1,
    DUMP_WDR_MERGE  = 0x2,
    DUMP_NR3D = 0x3,
    DUMP_DRC = 0x4,
    DUMP_CFA = 0x5,
    DUMP_YUV444 = 0x6,
    DUMP_YUV422 = 0x7,
    DUMP_TYPE_DUMMY = 0xffffffff,
} DUMP_TYPE;

typedef enum _NR3D_MDSTAT_TYPE_ {
    MDSTAT_8x8    =  0x0,
    MDSTAT_16x16  =  0x1,
    MDSTAT_32x32  =  0x2,
    MDSTAT_DUMMY  =  0xffffffff,
} NR3D_MDSTAT_TYPE;

typedef enum _DUMP_RAW_FORMAT_ {
    RAW_FORMAT = 0x0,
    RGB_FORMAT = 0x1,
    YUV422_FORMAT  = 0x2,
    YUV444_FORMAT = 0x3,
    DUMP_RAW_FORMAT_DUMMY = 0xffffffff,
} DUMP_RAW_FORMAT;

typedef enum _OFFLINE_MODE_
{
    ISP_OFFLINE_MODE_DISABLE  = 0,  // ISP工作模式为在线模式
    ISP_OFFLINE_MODE_LINEAR   = 1,  // ISP工作为离线线性模式
    ISP_OFFLINE_MODE_WDR      = 2,  // ISP工作为离线款动态模式
    ISP_ONLINE_ONLINE_MODE    = 3,  // ISP工作为双路在线模式
    ISP_ONLINE_SWITCH_MODE    = 4,  // ISP工作模式为SWITCH模式
    ISP_CLOSE_MODE_SNS_TO_DDR = 5,  // ISP不工作,sensor数据直接写入DDR模式
    ISP_MODE_DUMMY = 0xffffffff,
} ISP_OFFLINE_MODE;

typedef enum _ISP_OUT_MODE_
{
    ISP_OUT_TO_VPU  = (1 << 0),// ISP输出到VPU
    ISP_OUT_TO_DDR  = (1 << 1),// ISP输出到DDR，不支持
    ISP_OUT_TO_ENG  = (1 << 2),// ISP输出到ENG，不支持
    ISP_OUT_DUMMY = 0xffffffff,
}ISP_OUT_MODE_E;

typedef enum _ISP_OUT_FORMAT_
{
    ISP_OUT_TO_DDR_YUV420_8BIT  = 0,// ISP输出格式为YUV420_8BIT
    ISP_OUT_TO_DDR_YUV422_8BIT  = 1,// ISP输出格式为YUV422_8BIT
    ISP_OUT_TO_DDR_YUV420_10BIT = 2,// ISP输出格式为YUV420_10BIT
    ISP_OUT_TO_DDR_YUV422_10BIT = 3,// ISP输出格式为YUV422_10BIT
    ISP_OUT_TO_DDR_DUMMY = 0xffffffff,
}ISP_OUT_FORMAT_E;

typedef enum _LUT2D_WORK_MODE_E_
{
    ISP_LUT2D_BYPASS  = 0,// 2dlut 未启用
    ISP_LUT2D_ONLINE  = 1,// 2dlut 在线模式
    ISP_LUT2D_OFFLINE = 2,// 2dlut 离线模式
    ISP_LUT2D_DUMMY = 0xffffffff,
}LUT2D_WORK_MODE_E;

typedef enum _ISP_RUN_POS_SEL_E
{
    ISP_RUN_POS_START = 0,  // API_ISP_Run在帧头唤醒
    ISP_RUN_POS_END = 1,    // API_ISP_Run在帧尾唤醒
    ISP_RUN_POS_SEL_DUMMY = 0xffffffff,
}ISP_RUN_POS_SEL_E;

typedef enum __ISP_SNS_ATTR_E__
{
    ISP_SNS_ATTR_EXPOSURE_RATIO = 0,
    ISP_SNS_ATTR_WDR_FLAG  = 1,
    ISP_SNS_ATTR_DCG_FLAG  = 2,
    ISP_SNS_ATTR_LANE_MAX  = 3,
    ISP_SNS_ATTR_AGAIN_MAX = 4,
    ISP_SNS_ATTR_EXPOSURE_RATIO_CURR = 5,
    ISP_SNS_ATTR_AGAIN = 6,
    ISP_SNS_ATTR_INTT = 7,
    ISP_SNS_ATTR_DUMMY = 0xffffffff,
}ISP_SNS_ATTR_E;

typedef struct _ISP_PIC_SIZE_S_
{
    FH_UINT32 u32Width;     // 输入幅面宽度 | [0~0xffff]
    FH_UINT32 u32Height;    // 输入幅面高度 | [0~0xffff]
} ISP_PIC_SIZE;

typedef struct _ISP_MEM_INIT_
{
    ISP_OFFLINE_MODE  enOfflineWorkMode; // 当前ISP工作模式 | [OFFLINE_MODE]
    ISP_OUT_MODE_E    enIspOutMode;      // ISP输出的方式 | [ISP_OUT_MODE_E]
    ISP_OUT_FORMAT_E  enIspOutFmt;       // ISP离线输出的格式，不支持 | [ISP_OUT_FORMAT]
    LUT2D_WORK_MODE_E enLut2dWorkMode;   // Lut缓存分配控制，仅影响内存分配，不支持 | [0~1]
    ISP_PIC_SIZE      stPicConf;         // 幅面配置 | [ISP_PIC_SIZE]
} ISP_MEM_INIT;

typedef struct _ISP_RAW_BUF_S
{
    MEM_DESC raw_mem;     // raw数据的地址信息 | [MEM_DESC]
    MEM_DESC coeff_mem;   // coeff数据的地址信息 | [MEM_DESC]
    FRAME_TYPE raw_type;  // raw数据类型 | [FRAME_TYPE]
    FH_UINT32  frameCnt;  // 预期导出数据帧数 | [0~0xffffffff]
    FH_UINT32   u32BlkMod;     // 是否分块  | [0~1]
    FH_UINT32   u32RawSizeSf;  // Sf单帧raw的大小 | [0~0xffffffff]
    FH_UINT32   u32RawSizeLf;  // Lf单帧raw的大小,不支持 | [0~0xffffffff]
} ISP_RAW_BUF;

typedef struct _ISP_PARAM_CONFIG_S_
{
    FH_UINT32 u32BinAddr;// ISP参数存放的当前地址 | [0~0xffffffff]
    FH_UINT32 u32BinSize;// ISP参数的大小 | [0~0xffffffff]
} ISP_PARAM_CONFIG;

typedef struct _HW_MODULE_CFG_S{
    FH_UINT32 moduleCfg;  // 需要配置的模块 | [ISP_HW_MODULE_LIST]
    FH_BOOL enable;  // 使能开关,0表示关闭,1表示打开 | [0~1]
} HW_MODULE_CFG;

typedef struct _ISP_VERSION_S_
{
    FH_UINT32 u32SdkVer;        // sdk版本号 | [0~0xffffffff]
    FH_UINT32 FH_UINT32ChipVer; // 芯片版本号 | [0~0xffffffff]
    FH_UINT8  u08SdkSubVer;     // sdk sub版本号 | [0~0xff]
    FH_UINT8  u08BuildTime[21]; // sdk构建时间 | [0~0xff]
} ISP_VERSION;

typedef struct _ISP_VI_ATTR_
{
    FH_UINT16      u16WndHeight;   ///<sensor幅面高度 | [0~0xffff]
    FH_UINT16      u16WndWidth;    /// <sensor幅面宽度 | [0~0xffff]
    FH_UINT16      u16InputHeight; ///<sensor输入图像高度 | [0~0xffff]
    FH_UINT16      u16InputWidth;  ///<sensor输入图像宽度 | [0~0xffff]
    FH_UINT16      u16OffsetX;     ///<裁剪水平偏移 | [0~0xffff]
    FH_UINT16      u16OffsetY;     ///<裁剪垂直偏移 | [0~0xffff]
    FH_UINT16      u16PicHeight;   ///<处理的图像高度 | [0~0xffff]
    FH_UINT16      u16PicWidth;    ///<处理的图像宽度 | [0~0xffff]
    FH_UINT16      u16FrameRate;   ///<帧率 | [0~0xffff]
    ISP_BAYER_TYPE enBayerType;    ///bayer数据格式 | [0~0x3]
} ISP_VI_ATTR_S;

typedef struct _ISP_VI_STAT_S
{
    FH_UINT64 u64ISPS0IntCnt;   // ISPS0中断计数 | [0~0xffffffff]
    FH_UINT64 u64ISPS1IntCnt;   // ISPS1中断计数 | [0~0xffffffff]
    FH_UINT32 u32FrmRate;     // 当前帧率 | [0~0xffffffff]
    FH_UINT32 u32PicWidth;    // 当前处理图像宽度 | [0~0xffffffff]
    FH_UINT32 u32PicHeight;   // 当前处理图像高度 | [0~0xffffffff]
    FH_UINT32 u32ISPOverFlow; // ISP Vi溢出中断计数 | [0~0xffffffff]
    FH_UINT32 u32IspErrorSt;  // 出错标志，Vi溢出终端计数大于阈值会被值1 | [0~1]
} ISP_VI_STAT_S;

typedef struct _ISP_CTRL_CFG_S
{
    FH_UINT32  u32Id;                 // Isp通道号 | [0~1]
    FH_BOOL    bNr3dEn;               // Nr3d模块使能，默认开启，影响缓存分配 | [0~1]
    FH_BOOL    bNr3dStraightEn;       // Nr3d模块直通使能 | [0~1]
} ISP_CTRL_CFG;

typedef struct _ISP_SUSPEND_S_
{
    FH_UINT32 u32Reserved;  // 保留参数 | [0~0xffffffff]
}ISP_SUSPEND_S;

typedef struct _ISP_RESUME_S_
{
    FH_UINT32 u32Reserved; // 保留参数 | [0~0xffffffff]
}ISP_RESUME_S;

typedef struct __ISP_AOV_MODE_CFG_S
{
    FH_BOOL bEn;                // aov模式开启使能 | [FH_BOOL]
    FH_UINT32 u32RunCnt;        // 运行多少帧之后停止流水 | [0~0xffffffff]
}ISP_AOV_MODE_CFG_S;

typedef struct _ISP_RUN_PARA_S_
{
    FH_UINT32 u32TimeOut;       // 超时时间，单位是ms | [0~0xffffffff]
    ISP_RUN_POS_SEL_E enRunPos; // 策略执行的时机点 | [ISP_RUN_POS_SEL_E]
    FH_BOOL bAeAwbOnly;         // 只运行AE/AWB | [FH_BOOL]
    FH_BOOL bStrategyRun;       // 是否运行策略，使能时正常运行策略，否则该接口只起阻塞作用 | [FH_BOOL]
    FH_BOOL bRunForce;          // 使能时调用完立即运行一次策略，不等待信号量 | [FH_BOOL]
}ISP_RUN_PARA_S;

typedef struct __ISP_SNS_ATTR_CFG__
{
    FH_UINT32 u32IspDevId; // Isp通道号 | [0~1]
    FH_UINT32 u32InValue;  // 输入值 | [0~0xffffffff]
    FH_UINT32 u32OutValue; // 输出值 | [0~0xffffffff]
    ISP_SNS_ATTR_E enCmd;  // sensor配置命令 | [ISP_SNS_ATTR_E]
}ISP_SNS_ATTR_CFG_S;

typedef struct _ISP_RAW_BUF_COM_S_
{
    FH_UINT32  u32Base;      // 数据物理地址 | [0~0xffffffff]
    FH_UINT32  u32Vbase;     // 数据虚拟地址 | [0~0xffffffff]
    FH_UINT64  u64TimeStamp; // 数据获取的时间戳 | [0~0xffffffffffffffff]
    FH_UINT32  u32Size;      // 数据大小 | [0~0xffffffff]
    FH_UINT32  u32Stride;    // 一行的数据大小 | [0~0xffffffff]
}ISP_RAW_BUF_COM_S;

typedef struct _ISP_RAW_BUF_CFG_S_
{
    FH_BOOL    bLock;        // 是否加锁 | [0~1]
    FH_UINT32  u32TimeOut;   // 超时时间，阻塞和非阻塞都可以配置超时时间，单位是ms | [0~0xffffffff]
    FH_UINT64  u64FrmCnt;    // 帧号 | [0~0xffffffff]
    FH_UINT32  u32PoolId;    // 释放FIFO需要的id号 | [0~0xffffffff]
    ISP_PIC_SIZE stPic;      // 图像幅面相关的配置 | [ISP_PIC_SIZE]
    ISP_RAW_BUF_COM_S stSf;  // 短帧的RAW数据 | [ISP_RAW_BUF_COM_S]
    ISP_RAW_BUF_COM_S stLf;  // 长帧的RAW数据，不支持 | [ISP_RAW_BUF_COM_S]
}ISP_RAW_BUF_CFG_S;

/**|MIRROR|**/
typedef struct mirror_cfg_s
{
    FH_BOOL             bEN;  // 镜像使能开关 | [0~1]
    ISP_BAYER_TYPE      normal_bayer;  // 无效成员变量 | [0~3]
    ISP_BAYER_TYPE      mirror_bayer;  //　无效成员变量 | [0~3]
}MIRROR_CFG_S;

/**|STAT POS|**/
typedef enum _STAT_SEL_MODULE
{
    STAT_SEL_AWB_ADV = 0,
    STAT_SEL_GL   = 1,
    STAT_SEL_HIST = 2,
    STAT_SEL_MD   = 3,
    STAT_SEL_DRC  = 4,
    STAT_SEL_AF   = 5,   // FH8862V300不支持
    STAT_SEL_MODULE_DUMMY = 0xffffffff,
} STAT_SEL_MODULE;

/**|SMART_AE|**/
typedef struct _SMART_AE_ROI_S_
{
    FH_UINT16 u16RectX;     // ROI检测框左上角横坐标 | [0x0~0xffff]
    FH_UINT16 u16RectY;     // ROI检测框左上角纵坐标 | [0x0~0xffff]
    FH_UINT16 u16RectW;     // ROI检测框横向宽度 | [0x0~0xffff]
    FH_UINT16 u16RectH;     // ROI检测框纵向高度 | [0x0~0xffff]
    FH_UINT8 u08RectWeight; // ROI检测框权重 | [0x0~0xf]
} SMART_AE_ROI;

typedef struct _SMART_AE_CFG_S_
{
    FH_BOOL bSmartAeEn;         // SmartAe控制使能 | [0x0~0x1]
    FH_UINT16 u16RatioMin;      // 智能曝光系数最小值： U.10，值越小，允许整体亮度越低 | [0x0~0xffff]
    FH_UINT16 u16RatioMax;      // 智能曝光系数最大值： U.10，值越大，允许整体亮度越高 | [0x0~0xffff]
    FH_UINT16 u16DelayNum;      // 智能曝光延迟恢复帧数：这里的帧数表示智能检测的帧数，值越大，检测物体离开场景后恢复的时间越长 | [0x0~0xfff]
    FH_UINT8 u08TargetLuma;     // 智能曝光目标亮度： 值越大，ROI检测框内亮度越大，但目标亮度不能过大或过小，受smartRatioMin/Max限制 | [0x0~0xff]
    FH_UINT8 u08Speed;          // 智能曝光速度： 值越小曝光调整速度越快，0是最快速度 | [0x0~0xff]
    FH_UINT8 u08Interval;       // 智能曝光间隔： 值为0时表示每帧运行，值为1时表示间隔1帧运行一次 | [0x0~0xf]
    FH_UINT8 u08RoiNum;         // Roi检测框有效数量，最多支持4个 | [0x0~0x4]
    SMART_AE_ROI stDetectRoi[4];//智能曝光检测框坐标配置 | [SMART_AE_ROI]
} SMART_AE_CFG;

typedef struct _SMART_AE_STATUS_S_
{
    FH_UINT8 u08CurrLuma;     // 当前亮度(智能曝光调整时显示ROI检测框内亮度) | [0x0~0xff]
    FH_UINT8 u08SmartOpStatus;// 智能曝光调整状态 0：退出智能曝光 1：曝光调整状态 2：曝光保持状态，维持上一帧曝光参数 | [0x0~0x3]
} SMART_AE_STATUS;

/**|AE|**/
typedef struct _AE_ENABLE_CFG_S_
{
    FH_BOOL bAecEn;          // 自动曝光策略总开关 0：关闭AE策略 1：打开AE策略 | [0x0~0x1]
    FH_UINT8 u08AeMode;      // 自动曝光模式选择： （更新时需置位refresh位） 0：自动 1：半自动(允许固定曝光时间或增益中的一个，另一个走自动模式) 2：手动 | [0x0~0x2]
    FH_BOOL bAscEn;          // 自动快门使能 (aeMode为0时生效，更新时需置位refresh位)  0：关闭自动曝光时间，曝光时间维持上一帧的数值 1：使能自动曝光时间 | [0x0~0x1]
    FH_BOOL bMscEn;          // 手动快门使能 (aeMode为1时生效，更新时需置位refresh位) 0：关闭手动曝光时间，则曝光时间自动走 1：使能手动曝光时间，则曝光时间由mIntt决定 | [0x0~0x1]
    FH_BOOL bAgcEn;          // 自动增益使能 (aeMode为0时生效，更新时需置位refresh位)  0：关闭自动增益，增益维持上一帧的数值 1：使能自动增益 | [0x0~0x1]
    FH_BOOL bMgcEn;          // 手动增益使能(aeMode为1时生效，更新时需置位refresh位) 0：关闭手动增益，则增益自动走 1：使能手动增益，则增益由mAgain,mDgain决定 | [0x0~0x1]
    FH_BOOL bSensUpEn;       // 慢快门使能(更新时需置位refresh位) 0：禁止降低帧率 1：允许降低帧率，增加快门时间 | [0x0~0x1]
    FH_BOOL bIrisEn;         // 自动光圈使能(暂不支持) 0：关闭自动光圈 1：打开自动光圈 | [0x0~0x1]
    FH_BOOL bInttFineEn;     // 精细快门使能 0：只能调整整数行曝光时间 1：允许调整小数行曝光时间(采用数字增益内插) | [0x0~0x1]
    FH_BOOL bGainFineEn;     // 精细增益使能 0：增益精度跟随sensor 1：允许1/256增益精度(采用数字增益内插) | [0x0~0x1]
    FH_BOOL bAspeedEn;       // 自动速度控制使能 0：关闭自动速度，速度由initSpeed，speed及UExpSpeedBias决定 1：打开自动速度 | [0x0~0x1]
    FH_BOOL bAdaptiveEn;     // 自适应参考亮度 0：关闭自适应参考亮度，亮度只由lumaRef决定 1：打开自适应参考亮度，亮度跟随环境光线变化 | [0x0~0x1]
    FH_BOOL bAntiFlickerEn;  // 自动抗闪使能 0：关闭抗闪模式 1：打开抗闪模式 | [0x0~0x1]
    FH_BOOL bARouteEn;       // 自动曝光路线使能（更新时需置位refresh位） 0：使用默认曝光路线 1：使用手动曝光路线 | [0x0~0x1]
    FH_BOOL bInttUnitSel;    // 曝光时间单位选择： 0: 曝光时间单位使用us 1: 曝光时间单位使用行 | [0x0~0x1]
    FH_BOOL bInttFineMode;   // Intt内插模式选择 0：使用dGain内插 1：使用aGain内插 | [0x0~0x1]
    FH_BOOL bSfInttFineEn;   // 短帧精细快门使能(仅限WDR) 0：只能调整整数行曝光时间 1：允许调整小数行曝光时间(采用数字增益内插) | [0x0~0x1]
} AE_ENABLE_CFG;

typedef struct _AE_INIT_CFG_S_
{
    FH_BOOL bInitAeFlag;       // 快速启动AE开关控制：0：关闭快速启动AE 1：开启快速启动AE | [0x0-0x1]
    FH_UINT8 u08InitSpeed;     // 初始化曝光速度 | [0x0~0xff]
    FH_UINT8 u08InitTolerance; // 初始化曝光的稳定区间，U.4精度 | [0x0-0xff]
    FH_UINT32 u32InitExpTime;  // 初始化曝光时间，单位由inttUnitSel决定，暂不支持 | [0x0~0xffff]
    FH_UINT16 u16InitAgain;    // 初始化sensor增益，U.6，暂不支持 | [0x40~0xffff]
    FH_UINT16 u16InitDgain;    // 初始化isp增益，U.6，暂不支持 | [0x40~0x3ff]
} AE_INIT_CFG;

typedef struct _AE_SPEED_CFG_S_
{
    FH_UINT8 u08Tolerance;        // 一般不超过lumaRef的1%,U.4精度 | [0x0~0xff]
    FH_UINT8 u08LightChangeZone;  // 较小亮度变化区间 | [0x0~0xff]
    FH_UINT8 u08GreatChangeZone;  // 较大亮度变化区间 | [0x0~0xff]
    FH_UINT8 u08UnderExpLDlyFrm;  // 欠曝较小调整时延时调整的帧数 | [0x0~0xff]
    FH_UINT8 u08UnderExpGDlyFrm;  // 欠曝较大调整时延时调整的帧数 | [0x0~0xff]
    FH_UINT8 u08OverExpLDlyFrm;   // 过曝较小调整时延时调整的帧数 | [0x0~0xff]
    FH_UINT8 u08OverExpGDlyFrm;   // 过曝较大调整时延时调整的帧数 | [0x0~0xff]
    FH_UINT8 u08RunInterval;      // AE策略运行间隔，值为0时表示每帧运行，值为1时表示隔1帧运行一次，依次类推，一般配成0 | [0x0~0xf]
    FH_UINT8 u08Speed;            // 曝光速度，值越小速率越快，0是最快速率 | [0x0~0xff]
    FH_UINT8 u08UExpSpeedBias;    // 欠曝调整时速度偏移量 | [0x0~0xff]
} AE_SPEED_CFG;

typedef struct _AE_METERING_CFG_S_
{
    FH_UINT8 u08StatSel;         // 统计模式： 0：3*3分块统计(不支持) 1：16*16分块统计 2：hist统计 | [0x0~0x2]
    FH_UINT8 u08LightMode;       // 亮度测光模式(statSel=1或2时生效)： 0：正常模式 1：高光优先 2：低光优先 | [0x0~0x2]
    FH_UINT8 u08BlockMode;       // 分块测光模式(statSel=0或1时生效)： 0：全局测光 1：前光补偿(仅3*3统计生效) 2：背光补偿(仅3*3统计生效) 3：用户自定义 | [0x0~0x3]
    FH_UINT8 u08MeteringBlock;   // 当blockMode = 1/2时生效，表示 0：最强，8最弱 | [0x0~0x8]
    FH_UINT8 u08LumaBoundary;    // 高光或低光亮度截断值(仅在lightMode下有效) | [0x0~0xff]
    FH_UINT8 u08RoiWeight;       // 高光或低光区域对平均亮度影响程度，值越大，对平均亮度影响越大，值为0时不影响(仅在lightMode下有效) | [0x0~0xff]
    FH_UINT16 u16Sensitivity;    // 亮度敏感度，值越大，对高光或低光区域越敏感, U.7(仅在lightMode下有效) | [0x0~0xffff]
    FH_UINT8 u08WeightBlk[256];  // 第0块权值（仅meteringMode=5或7，更新时需置位refresh位） | [0x0~0xf]
} AE_METERING_CFG;

typedef struct _AE_TIMING_CFG_S_
{
    FH_UINT8 u08Intt0Delay;   // 配置T1 intt时，延迟的帧数，更新时需置位refresh位 | [0x0~0x3]
    FH_UINT8 u08Intt1Delay;   // 当至少两帧合成时配置T2 intt时，延迟的帧数，更新时需置位refresh位 | [0x0~0x3]
    FH_UINT8 u08Intt2Delay;   // 当至少三帧合成时配置T3 intt时，延迟的帧数，更新时需置位refresh位 | [0x0~0x3]
    FH_UINT8 u08Intt3Delay;   // 当至少四帧合成时配置T4 intt时，延迟的帧数，更新时需置位refresh位 | [0x0~0x3]
    FH_UINT8 u08Again0Delay;  // 配置T1 sensor gain时，延迟的帧数，更新时需置位refresh位 | [0x0~0x3]
    FH_UINT8 u08Again1Delay;  // 当至少两帧合成时配置T2 again时，延迟的帧数，更新时需置位refresh位 | [0x0~0x3]
    FH_UINT8 u08Again2Delay;  // 当至少三帧合成时配置T3 again时，延迟的帧数，更新时需置位refresh位 | [0x0~0x3]
    FH_UINT8 u08Again3Delay;  // 当至少四帧合成时配置T4 again时，延迟的帧数，更新时需置位refresh位 | [0x0~0x3]
    FH_UINT8 u08DgainDelay;   // 配置isp gain时，延迟的帧数，更新时需置位refresh位 | [0x0~0xf]
    FH_UINT8 u08SensUpDelay;  // 配置帧结构时，延迟的帧数，更新时需置位refresh位 | [0x0~0xf]
    FH_UINT8 u08SfDgainDelay; // 补偿短帧配置isp gain时，延迟的帧数，更新时需置位refresh位 | [0x0~0xf]
} AE_TIMING_CFG;

typedef struct _AE_MANUAL_CFG_S_
{
    FH_UINT32 u32MIntt;   // 手动模式下的intt值，单位由inttUnitSel决定 | [0x0~0x3ffff]
    FH_UINT16 u16MAgain;  // 手动模式下的again值，U.6 | [0x40~0xffff]
    FH_UINT16 u16MDgain;  // 手动模式下的Dgain值，U.6 | [0x40~0x3ff]
} AE_MANUAL_CFG;

typedef struct _AE_ANTIFLICKER_CFG_S_
{
    FH_BOOL bAntiFlickerMode; // 抗闪模式选择： 1:强抗闪模式，完全根据频闪周期配置曝光时间 0:弱抗闪模式，当环境光较亮时通过限制亮度并取消抗闪来避免过曝严重 | [0x0~0x1]
    FH_UINT16 u16Frequency;   // 抗闪频率值 | [0x0~0xfff]
    FH_UINT8 u08StabZone;     // 抗闪最大亮度差异设置，当stabZone+targetLuma<CurrLuma时取消抗闪设置 | [0x0~0xff]
} AE_ANTIFLICKER_CFG;

typedef struct _AE_LUMA_CFG_S_
{
    FH_UINT8 u08LumaRef;     // 参考亮度，归一化至0~255 | [0x0~0xff]
    FH_UINT8 u08LumaRefLow;  // 低光状态下的参考亮度，仅当adaptiveEn为1且aRouteEn为0时生效，归一化至0~255 | [0x0~0xff]
    FH_UINT8 u08LumaRefHigh; // 高光状态下的参考亮度，仅当adaptiveEn为1且aRouteEn为0时生效，归一化至0~255 | [0x0~0xff]
    FH_UINT16 u16EvLow;      // 低光状态环境光强，用于aRouteEn为0时的参考亮度内插 | [0x0~0xffff]
    FH_UINT16 u16EvNormL;    // 正常光线状态环境光强下阈值，用于aRouteEn为0时的参考亮度内插 | [0x0~0xffff]
    FH_UINT16 u16EvNormH;    // 正常光线状态环境光强上阈值，用于aRouteEn为0时的参考亮度内插 | [0x0~0xffff]
    FH_UINT16 u16EvHigh;     // 高光状态环境光强，用于aRouteEn为0时的参考亮度内插 | [0x0~0xffff]
} AE_LUMA_CFG;

typedef struct _AE_RANGE_CFG_S_
{
    FH_UINT16 u16InttMin;   // 最小曝光时间，单位由inttUnitSel决定，更新时需置位refresh位 | [0x0~0xffff]
    FH_UINT32 u32InttMax;   // 最大曝光时间，单位由inttUnitSel决定，更新时需置位refresh位 | [0x0~0xfffff]
    FH_UINT16 u16AgainMin;  // sensor增益最小值，U.6，更新时需置位refresh位 | [0x40~0xffff]
    FH_UINT16 u16AgainMax;  // sensor增益最大值，U.6，更新时需置位refresh位 | [0x40~0xffff]
    FH_UINT16 u16DgainMin;  // isp增益最小值，U.6，更新时需置位refresh位 | [0x40~0x3ff]
    FH_UINT16 u16DgainMax;  // isp增益最大值，U.6，更新时需置位refresh位 | [0x40~0xfff]
} AE_RANGE_CFG;

typedef struct _AE_DEFAULT_CFG_S_ {
    AE_ENABLE_CFG stAeEnableCfg;            // ae使能配置 | [AE_ENABLE_CFG]
    AE_INIT_CFG stAeInitCfg;                // ae初始化配置 | [AE_INIT_CFG]
    AE_SPEED_CFG stAeSpeedCfg;              // ae速率配置 | [AE_SPEED_CFG]
    AE_METERING_CFG stAeMeteringCfg;        // ae测光配置 | [AE_METERING_CFG]
    AE_TIMING_CFG stAeTimingCfg;            // ae时序配置 | [AE_TIMING_CFG]
    AE_MANUAL_CFG stAeManualCfg;            // ae手动值配置 | [AE_MANUAL_CFG]
    AE_ANTIFLICKER_CFG stAeAntiflickerCfg;  // ae抗闪配置 | [AE_ANTIFLICKER_CFG]
    AE_LUMA_CFG stAeLumaCfg;                // ae亮度配置 | [AE_LUMA_CFG]
    AE_RANGE_CFG stAeRangeCfg;              // 最小最大范围配置,更新时需置位refresh位 | [AE_RANGE_CFG]
    FH_BOOL bNodeChMode;                    // AE节点变化模式选择： 0：跨节点变化模式 1：节点连续变化模式 | [0x0~0x1]
    FH_BOOL bSensUpMode;                    // 慢快门模式选择： 0：无极连续降帧； 1：非连续直接降帧； | [0x0~0x1]
    FH_UINT8 u08SensUpPrec;                 // 慢快门非连续降帧精度，值越大降帧分段越多越精确 | [0x0~0xf]
    FH_UINT16 u16SensUpEnableGain;          // 降帧启动增益，U.6 (aRouteEn为0时生效，更新时需置位refresh位） | [0x40~0xffff]
    FH_UINT16 u16SensUpDisableGain;         // 降帧关闭增益，U.6 (aRouteEn为0时生效，更新时需置位refresh位） | [0x40~0xffff]
    FH_UINT8 u08InttPrec;                   // 曝光精度，固定以行为单位 | [0x0~0xff]
    FH_UINT8 u08InttOffset;                 // 当sensor intt与again存在偏移关系时需要配置曝光时间的偏移，固定以μs为单位 | [0x0~0xff]
} AE_DEFAULT_CFG;

typedef struct _AE_ROUTE_CFG_S_
{
    FH_UINT16 u16Piris[16];  // 该节点对应光圈参数，更新时需置位refresh位 | [0x0~0xfff]
    FH_UINT32 u32Intt[16];   // 该节点对应曝光时间参数，单位由inttUnitSel决定，更新时需置位refresh位 | [0x0~0xfffff]
    FH_UINT16 u16AGain[16];  // 该节点对应模拟增益，U.6，更新时需置位refresh位 | [0x40~0xffff]
    FH_UINT16 u16DGain[16];  // 该节点对应数字增益，U.6，更新时需置位refresh位 | [0x40~0x3ff]
    FH_UINT8 u08LumaRef[16]; // 第一段线对应的参考亮度，更新时需置位refresh位 | [0x0~0xff]
} AE_ROUTE_CFG;

typedef struct _ISP_AE_INFO_S_
{
    FH_UINT32   u32Intt;          // sensor曝光行数 | [0~0xffffffff]
    FH_UINT32   u32IspGain;       // isp增益值U4.8, 0x100表示一倍 | [0~0xfff]
    FH_UINT32   u32SensorGain;    // sensor增益值U.6, 0x40表示一倍 |[0~0xffffffff]
    FH_UINT32   u32TotalGain;     // 总增益U.6 , 0x40表示一倍 | [0~0xffffffff]
} ISP_AE_INFO;

typedef struct _ISP_AE_INFO_2_S_
{
    FH_UINT32   u32Intt;          // sensor曝光行数 | [0~0xffffffff]
    FH_UINT32   u32PreGain;       // 前置isp增益值U4.8, 0x100表示一倍 | [0~0xfff]
    FH_UINT32   u32PostGain;      // 后置isp增益值U4.8, 0x100表示一倍 | [0~0xfff]
    FH_UINT32   u32SensorGain;    // sensor增益值U.6, 0x40表示一倍 |[0~0xffffffff]
    FH_UINT32   u32TotalGain;     // 总增益U.6 , 0x40表示一倍 | [0~0xffffffff]
} ISP_AE_INFO_2;

typedef struct _ISP_AE_INFO_3_S_
{
    FH_UINT32   u32InttUs;        // sensor曝光时间us | [0~0xffffffff]
    FH_UINT32   u32IspGain;       // isp增益值U4.8, 0x100表示一倍 | [0~0xfff]
    FH_UINT32   u32SensorGain;    // sensor增益值U.6, 0x40表示一倍 |[0~0xffffffff]
    FH_UINT32   u32TotalGain;     // 总增益U.6 , 0x40表示一倍 | [0~0xffffffff]
} ISP_AE_INFO_3;

typedef struct _ISP_AE_STATUS_S_
{
    FH_UINT8 u08TarLuma;         // 目标亮度(智能曝光调整时显示ROI检测框内目标亮度) | [0x0~0xff]
    FH_UINT8 u08CurrLuma;        // 当前亮度(智能曝光调整时显示ROI检测框内亮度) | [0x0~0xff]
    FH_SINT16 s16ErrorLuma;      // 当前帧亮度与目标值差异，S.4，tar-curr | [0~8191]
    FH_UINT16 u16CurrPiris;      // 当前光圈值 | [0x0~0xfff]
    FH_UINT32 u32CurrIntt;       // 当前曝光时间，单位固定是μs | [0x0~0xfffff]
    FH_UINT16 u16CurrAgain;      // 当前sensor增益，U.6 | [0x0~0xffff]
    FH_UINT16 u16CurrDgain;      // 当前isp增益，U.6 | [0x0~0xffff]
    FH_UINT32 u32CurrTotalGain;  // 当前总增益，U.6 | [0x0~0xffffffff]
    FH_UINT16 u16CurrInttLine;   // 当前曝光时间，单位固定为行 | [0x0~0xffff]
    FH_UINT16 u16CurrFrameHeight;// 当前帧高度 | [0x0~0xffff]
    FH_UINT16 u16Ev;             // 当前环境光强 | [0x0~0xffff]
    FH_UINT8 u08CurrNodeId;      // 当前节点ID(仅aRouteEn为1时有效) | [0x0~0xf]
    FH_BOOL bOpStatus;           // Ae当前运行状态 0：停止 1：运行 | [0x0~0x1]
    FH_UINT32 u32ExpParam;       // 当前总曝光参数，曝光时间以行数计算，增益均以.6精度 | [0x0~0xffffffff]
    FH_UINT16 u16Piris[16];      // 该节点对应光圈参数 | [0x0~0xfff]
    FH_UINT32 u32Intt[16];       // 该节点对应曝光时间参数，单位由inttUnitSel决定 | [0x0~0xfffff]
    FH_UINT16 u16AGain[16];      // 该节点对应模拟增益，U.6 | [0x0~0xffff]
    FH_UINT16 u16DGain[16];      // 该节点对应数字增益，U.6 | [0x0~0xffff]
    FH_UINT16 u16ActInttMax;     // 策略运行生效的inttmax，以行为单位 | [0x0~0xffff]
    FH_UINT16 u16ActAgainMax;    // 策略运行生效的againmax，U.6 | [0x0~0xffff]
    FH_UINT16 u16ActDgainMax;    // 策略运行生效的dgainmax，U.6 | [0x0~0xffff]
    FH_UINT16 u16SensorInttMax;  // Sensor支持的inttmax，以行为单位，从sensor库获取 | [0x0~0xffff]
    FH_UINT16 u16SensorInttMin;  // Sensor支持的inttMin，以行为单位，从sensor库获取 | [0x0~0xffff]
    FH_UINT16 u16SensorAgainMax; // Sensor支持的againMax，U.6，从sensor库获取 | [0x0~0xffff]
    FH_UINT16 u16SensorAgainMin; // Sensor支持的againMin，U.6，从sensor库获取 | [0x0~0xffff]
    FH_UINT16 u16DgainMax;       // ISP支持的DgainMax，U.6，由硬件位宽决定 | [0x0~0xffff]
    FH_UINT16 u16DgainMin;       // ISP支持的DgainMin，U.6，由硬件位宽决定 | [0x0~0xffff]
    FH_UINT8 u08SmartOpStatus;   // 智能曝光调整状态 0：退出智能曝光 1：曝光调整状态 2：曝光保持状态，维持上一帧曝光参数 | [0x0~0x3]
    FH_UINT8 u08LumaRef[16];     // 该节点对应参考亮度 | [0x0~0xff]
} ISP_AE_STATUS;

typedef struct _AE_USER_STATUS_S_
{
    FH_UINT8 u08TarLuma;         // 当前目标亮度 | [0x0-0xff]
    FH_UINT32 u32CurrTotalGain;  // 当前总增益，U.6 | [0x0-0xffffffff]
    FH_BOOL bOpStatus;           // Ae当前运行状态  0：停止 1：运行 | [0x0-0x1]
    FH_UINT32 u32CurrIntt;       // 当前曝光时间，单位固定是μs | [0x0-0xfffff]
    FH_UINT16 u16CurrInttLine;   // 当前曝光时间，单位固定为行 | [0x0-0xffff]
    FH_UINT16 u16ActInttMax;     // 策略运行生效的inttmax，以行为单位,非慢快门下不大于u16SensorInttMax | [0x0-0xffff]
    FH_UINT16 u16SensorInttMax;  // Sensor支持的inttmax，以行为单位，不考虑慢快门情况,表示的是一帧之内的最大曝光值 | [0x0-0xffff]
} AE_USER_STATUS;

/**|AWB|**/
typedef struct _ISP_AWB_GAIN_S_ {
    FH_UINT16 u16Rgain;// 手动R通道增益，U.9 | [0x0~0x1fff]
    FH_UINT16 u16Ggain;// 手动G通道增益，U.9 | [0x0~0x1fff]
    FH_UINT16 u16Bgain;// 手动B通道增益，U.9 | [0x0~0x1fff]
} ISP_AWB_GAIN;
typedef struct _AWB_ENABLE_CFG_S_
{
    FH_BOOL bAwbEn;// Awb总使能 | [0x0~0x1]
    FH_BOOL bMwbEn;// 手动wb使能，当为1时配置手动R,G,B增益值，当为0时根据统计实时调整 | [0x0~0x1]
    FH_BOOL bStatSel;// 0：Gray World 1：硬件筛选统计 | [0x0~0x1]
    FH_BOOL bStatMappingEn;// 0:硬件统计配置固定，和室内配置一样 1:硬件统计配置跟着亮度场景变化 | [0x0~0x1]
    FH_BOOL bZoneSel;// 0:软件策略白点检测区域自动生成 1:软件策略白点检测区域用户配置 | [0x0~0x1]
    FH_BOOL bZoneWeightMappingEn;// 0:软件筛选框权重固定，由IndoorZoneWeight决定 1:软件筛选框权重跟随判断的亮度场景联动 | [0x0~0x1]
    FH_BOOL bWinWeightEn;// 统计分块权重使能 | [0x0~0x1]
    FH_BOOL bLightWeightEn;// 亮度是否影响权重使能 | [0x0~0x1]
    FH_BOOL bInOutEn;// 室内外场景判断使能 | [0x0~0x1]
    FH_BOOL bPureColorEn;// 单色物体判断使能 | [0x0~0x1]
    FH_BOOL bMultiCTEn;// 混合色温使能 | [0x0~0x1]
    FH_BOOL bSpecWPEn;// 特殊色温点使能 | [0x0~0x1]
    FH_BOOL bFineTuneEn;// 精细调整使能，目前主要用于肤色保护（仅在室内模式打开） | [0x0~0x1]
    FH_BOOL bOutdoorEnhanceEn;// 仅在室外场景下生效，针对蓝天场景以及绿植场景 | [0x0~0x1]
    FH_BOOL bCompEn;// 增益补偿使能 | [0x0~0x1]
    FH_BOOL bMinMaxSel;// 0：增益计算采用最小值 1：增益计算采用最大值 | [0x0~0x1]
    FH_BOOL bMinNormEn;// 增益计算采用最小值时，是否做亮度归一化的使能 | [0x0~0x1]
    FH_BOOL bMaxNormEn;// 增益计算采用最大值时，是否做亮度归一化的使能（暂不支持） | [0x0~0x1]
    FH_BOOL bRangeWpEn;// 0:超色温上下限的场景增益不做clip 1:对超色温上下限的场景增益进行限制 | [0x0~0x1]
    FH_BOOL bWdrSpecCtrlEn;// 0:长短帧标定白点位置相同 1:长短帧标定白点位置不同 | [0x0~0x1]
    FH_UINT8 u08AwbAlgType;// 0：Global Alg 1：Advanced Alg 2: low cost Alg | [0x0~0x2]
} AWB_ENABLE_CFG;

typedef struct _AWB_WHITE_POINT_S_
{
    FH_UINT16 u16BOverG;  // 标定白点的B/G分量 | [0~0xffff]
    FH_UINT16 u16ROverG;  // 标定白点的R/G分量 | [0~0xffff]
} AWB_WHITE_POINT;

typedef struct _AWB_WHITE_POINT_UV_S_
{
    FH_UINT16 u16U;  // 标定白点的u分量 | [0~0xfff]
    FH_UINT16 u16V;  // 标定白点的v分量 | [0~0xfff]
} AWB_WHITE_POINT_UV;

typedef struct _STAT_WHITE_POINT_S_
{
    FH_UINT16 u16Coordinate_w;  // awblib白框横坐标U4.8 | [0~0xfff]
    FH_UINT16 u16Coordinate_h;  // awblib白框纵坐标U4.8 | [0~0xfff]
} STAT_WHITE_POINT;

typedef struct _AWB_CALIB_CFG_S_
{
    FH_SINT16 s16Matrix[9];           // 标定用3*3转换矩阵参数 | [-32767, 32768]
    FH_SINT16 s16StandardWPM[6];      // 6个标记白点的参数M | [-32767~32768]
    FH_UINT16 u16StandardWPK[6];      // 6个标记白点的参数K | [0x0~0xffff]
    FH_UINT16 u16StandardWPCCT[6];      // 6个标记白点的计算色温 | [0x0~0xffff]
    AWB_WHITE_POINT_UV stWhitePointUV[6];  // 标定白点的UV结构体 | [AWB_WHITE_POINT_UV]
    AWB_WHITE_POINT stWhitePointLF[6];  // 长帧标定白点的结构体,默认都配置长帧白点 | [AWB_WHITE_POINT]
    AWB_WHITE_POINT stWhitePointSF[6];  // 短帧标定白点的结构体,仅wdrSpecCtrlEn开启后生效 | [AWB_WHITE_POINT]
} AWB_CALIB_CFG;

typedef struct _AWB_LIGHT_CFG_S_
{
    FH_BOOL bLightWeightThMode;    // 0：内置门限值表   1：用户自定义门限值 | [0x0~0x1]
    FH_UINT8 u08LightWeightTh[6];  // 最暗处阈值 | [0x0~0xff]
    FH_UINT8 u08LightWeight[6];    // 最暗处权重 | [0x0~0xff]
} AWB_LIGHT_CFG;

typedef struct _AWB_WP_POINT_CFG_S_
{
    FH_UINT16 u16WpPointX;// 顶点的横坐标，U4.12 | [0x0~0xffff]
    FH_UINT16 u16WpPointY;// 顶点的纵坐标，U4.12 | [0x0~0xffff]
} AWB_WP_POINT_CFG;

typedef struct _AWB_SPEC_CT_CFG_S_
{
    FH_BOOL bSpecWpEn;// 特殊白点使能 | [0x0~0x1]
    FH_BOOL bSpecWpMode;// 0:新增白点 1:删除白点 | [0x0~0x1]
    FH_BOOL bSpecWpType;// 0:三角形 1:矩形 | [0x0~0x1]
    AWB_WP_POINT_CFG stPoint[3];  // 3个顶点的横纵坐标X,Y U4.12 | [AWB_WP_POINT_CFG]
} AWB_SPEC_CT_CFG;

typedef struct _AWB_RANGE_WP_CFG_S_
{
    FH_BOOL bRangeWpType;// 0:超色温上下限时按照色温标定点映射到最高最低色温点上 1：手动配置低色温和高色温下的增益值 | [0x0~0x1]
    FH_UINT16 u16LowCCT;// 低色温下限，色温取值范围建议为[wp0,wp1]，单位为K | [0x0~0xbb8]
    FH_UINT16 u16HighCCT;// 高色温上限，色温取值范围建议为[wp4,wp5]，单位为K | [0x0~0x3a98]
    FH_UINT16 u16BgLimitLowCT;// 低色温bg分量下限 | [0x0~0xffff]
    FH_UINT16 u16RgLimitLowCT;// 低色温rg分量上限 | [0x0~0xffff]
    FH_UINT16 u16BgLimitHighCT;// 高色温bg分量上限 | [0x0~0xffff]
    FH_UINT16 u16RgLimitHighCT;// 高色温rg分量下限 | [0x0~0xffff]
} AWB_RANGE_WP_CFG;
typedef struct _AWB_MANUAL_CFG_S_
{
    ISP_AWB_GAIN stAwbGain;  // wb手动增益值 | [ISP_AWB_GAIN]
} AWB_MANUAL_CFG;

typedef struct _AWB_COMP_CFG_S_
{
    FH_UINT8 u08REnhance;// R通道增益补偿，U.6 | [0x0~0xff]
    FH_UINT8 u08GEnhance;// G通道增益补偿，U.6 | [0x0~0xff]
    FH_UINT8 u08BEnhance;// B通道增益补偿，U.6 | [0x0~0xff]
} AWB_COMP_CFG;

typedef struct _AWB_STAT_TH_CFG_S_
{
    FH_UINT8 u08StatThh;// 统计像素亮度阈值上限，亮度按255归一化 | [0x0~0xff]
    FH_UINT8 u08StatThl;// 统计像素亮度阈值下限，亮度按255归一化 | [0x0~0xff]
    FH_UINT8 u08StatThlOutdoor;// 室外时的统计像素亮度阈值下限，亮度按255归一化 | [0x0~0xff]
    FH_UINT8 u08StatThhOutdoor;// 室外时的统计像素亮度阈值上限，亮度按255归一化 | [0x0~0xff]
    FH_UINT8 u08StatThlLowlight;// 低亮时统计阈值下限，按255归一化 | [0x0~0xff]
    FH_UINT8 u08StatThhLowlight;// 低亮时统计阈值上限，按255归一化 | [0x0~0xff]
} AWB_STAT_TH_CFG;
typedef struct _AWB_LUX_SCENE_CFG_S_
{
    FH_BOOL bInOutType;// 室内外场景判断类型： 0：自动 1：手动 | [0x0~0x1]
    FH_UINT8 u08ManualType;// 手动模式下亮度场景类型： 0：室内 1：室外 2：低照度 | [0x0~0x2]
    FH_UINT16 u16InDoorThresh;// 室内场景判断曝光时间的阈值，单位是us，大于该曝光时间认为是室内的概率是100% | [0x0~0xffff]
    FH_UINT16 u16OutDoorThresh;// 室外场景判断曝光时间的阈值，单位是us，不大于该曝光时间认为是室外的概率是100% | [0x0~0xffff]
    FH_UINT16 u16NormalLightThresh;// 正常亮度时增益阈值，U12.4 | [0x0~0xffff]
    FH_UINT16 u16LowLightThresh;// 低亮场景时增益阈值,U12.4 | [0x0~0xffff]
} AWB_LUX_SCENE_CFG;

typedef struct _AWB_AUTO_ZONE_CFG_S_
{
    FH_BOOL bAutoZoneWidthType;// zonesel为0时生效 0:软件自动生成色温框的宽度不同色温固定,且左边界和右边界相同 1: 软件自动生成色温框宽度不同色温自定义配置 | [0x0~0x1]
    FH_UINT8 u08ColorTempNum;// 软件自动生成色温框的个数，范围4~16,仅zonesel为0时生效 | [0x4~0x10]
    FH_UINT8 u08AutoZoneWidthThl;// autoZoneWidthType为0时生效，色温框的宽度下阈值，U.6 | [0x0~0xff]
    FH_UINT8 u08AutoZoneWidthThh;// autoZoneWidthType为0时生效，色温框的宽度上阈值，U.6 | [0x0~0xff]
    FH_UINT8 u08ZoneLWidthThl[6];// 第一个标记白点软件生成框宽度低增益左边界阈值，U.6 | [0x0~0xff]
    FH_UINT8 u08ZoneLWidthThh[6];// 第一个色温点软件生成框宽度高增益左边界阈值，U.6 | [0x0~0xff]
    FH_UINT8 u08ZoneRWidthThl[6];// 第一个标记白点软件生成框宽度低增益右边界阈值，U.6 | [0x0~0xff]
    FH_UINT8 u08ZoneRWidthThh[6];// 第一个色温点软件生成框宽度高增益右边界阈值，U.6 | [0x0~0xff]
    FH_UINT16 u16ZoneGainThl;// 控制软件筛选框宽度的增益下阈值(U12.4)，zonesel为0时生效 | [0x0~0xffff]
    FH_UINT16 u16ZoneGainThh;// 控制软件筛选框宽度的增益上阈值(U12.4)，zonesel为0时生效 | [0x0~0xffff]
}AWB_AUTO_ZONE_CFG;

typedef struct _AWB_DZONE_S_
{
    FH_UINT8 u08DZtype;              // 每个检测框的类型 0:三角形 1:矩形 2:任意四边形 | [0x0~0x2]
    AWB_WHITE_POINT stPoint[4];   // 每个检测框最多4个顶点,x,y U4.12 | [AWB_WHITE_POINT]
} AWB_DZONE;

typedef struct _AWB_MZONE_CFG_S_
{
    FH_BOOL bZoneType;                // 手动配置软件筛选框的模式 0:库里内置 1:外部传入手动配置 | [0x0~0x1]
    FH_UINT8 u08ZoneNum;              // 手动配置软件筛选框的个数,最大16个,zonetype为0时,这个参数只读 | [0x1~0x10]
    AWB_DZONE stDZone[16];          // 手动配置的软件筛选框 | [AWB_DZONE]
} AWB_MZONE_CFG;

typedef struct _AWB_ZONE_WEIGHT_CFG_S_
{
    FH_UINT8 u08IndoorZoneWeight[16];// 室内软件筛选框0的weight | [0x0~0xff]
    FH_UINT8 u08OutdoorZoneWeight[16];// 室外软件筛选框0的weight | [0x0~0xff]
    FH_UINT8 u08LowNightZoneWeight[16];// 低照度软件筛选框0的weight | [0x0~0xff]
} AWB_ZONE_WEIGHT_CFG;

typedef struct _AWB_MULTI_CT_CFG_S_
{
    FH_BOOL  bCTWeightType;// 0:固定CT权重，权重从indoorCTWeight里读取 1：CT权重mapping | [0x0~0x1]
    FH_UINT8 u08MultiSatScaler;// 混合色温下饱和度调整比例，增益系数计算为multiSatScaler/0xff,值越小，混合色温下饱和度越低。 | [0x0~0xff]
    FH_UINT8 u08IndoorCTWeight[10];// 室内正常照度混合色温下色温点0权重 | [0x0~0xff]
    FH_UINT8 u08OutWeight[10];// 室外色温点0权重 | [0x0~0xff]
    FH_UINT8 u08LowLightCTWeight[10];// 低亮时混合色温下色温点0权重 | [0x0~0xff]
} AWB_MULTI_CT_CFG;

typedef struct _AWB_PURE_CFG_S_
{
    FH_UINT8 u08LumaDiffRatioTh;    // 单色亮度差异比例门限值，差异值与平均值的比例小于此值则亮度符合纯色判断，反之则不符合 | [0x0~0x64]
    FH_UINT8 u08ChromaDiffRatioTh;  // 单色色度差异比例门限值，差异值与平均值的比例小于此值则色度符合纯色判断，反之则不符合 | [0x0~0x64]
} AWB_PURE_CFG;

typedef struct _AWB_DEFAULT_CFG_S_
{
    AWB_ENABLE_CFG stAwbEnableCfg;  // awb使能配置 | [AWB_ENABLE_CFG]
    FH_UINT16 u16Speed;             // 值越大，调整速度越快 | [0x0~0xfff]
    FH_UINT8 u08fineTuneStr;        // 精细调整强度,值越大保护越强,过大导致不稳定,建议暂时配置为0 | [0x0~0xff]
    STAT_WHITE_POINT stPoint[6];      //  awb白框点,A，B，C，D，E，F六个点，Ax=Bx By=Cy Dy=Ey Ex=Fx | [STAT_WHITE_POINT]
    AWB_STAT_TH_CFG stAwbStatThCfg; // awb统计硬件阈值设置 | [AWB_STAT_TH_CFG]
    FH_UINT8 u08WeightBlk[64];     // 统计分块权重设置（仅winWeightEn =1生效） | [0x0~0xff]
    AWB_LIGHT_CFG stAwbLightCfg;    // awb亮度阈值及权重配置 | [AWB_LIGHT_CFG]
    AWB_LUX_SCENE_CFG stAwbLuxSceneCfg;    // awb亮度场景判断配置 | [AWB_LUX_SCENE_CFG]
    AWB_AUTO_ZONE_CFG stAwbAutoZoneCfg;     // awb自动生成软件筛选框配置 | [AWB_AUTO_ZONE_CFG]
    AWB_MZONE_CFG stAwbMZoneCfg;     // awb手动配置软件筛选框配置 | [AWB_MZONE_CFG]
    AWB_ZONE_WEIGHT_CFG stAwbZoneWeightCfg;// awb软件筛选框权重配置 | [AWB_ZONE_WEIGHT_CFG]
    AWB_MULTI_CT_CFG stAwbMultiCTCfg;// awb混色温配置 | [AWB_MULTI_CT_CFG]
    AWB_PURE_CFG stAwbPureCfg;       // awb纯色配置 | [AWB_PURE_CFG]
    AWB_SPEC_CT_CFG stAwbSpecCtCfg[4];   // 最多4组awb特殊色温参数配置 | [AWB_SPEC_CT_CFG]
    AWB_RANGE_WP_CFG stAwbRangeWPCfg;   // 超色温限制配置 | [AWB_RANGE_WP_CFG]
    AWB_MANUAL_CFG stAwbManualCfg;  // awb手动配置 | [AWB_MANUAL_CFG]
    AWB_WHITE_POINT stManualWhitePoint[6]; // 手动CT白点 | [AWB_WHITE_POINT]
    AWB_COMP_CFG stAwbCompCfg;      // awb补偿增益配置 | [AWB_COMP_CFG]
    AWB_CALIB_CFG stAwbCalibCfg;    // awb标定配置 | [AWB_CALIB_CFG]
} AWB_DEFAULT_CFG;

typedef struct _ISP_AWB_STATUS_S_
{
    FH_UINT16 u16RGain;// 红色增益（U4.6） | [0x0~0x3ff]
    FH_UINT16 u16GGain;// 绿色增益（U4.6） | [0x0~0x3ff]
    FH_UINT16 u16BGain;// 蓝色增益（U4.6） | [0x0~0x3ff]
    FH_UINT16 u16CurrBOverG;// 当前B/G(水平坐标，U4.12) | [0x0~0xffff]
    FH_UINT16 u16CurrROverG;// 当前R/G(垂直坐标，U4.12) | [0x0~0xffff]
    FH_UINT16 u16RAvg;// 当前统计红色平均值（U10） | [0x0~0x3ff]
    FH_UINT16 u16GAvg;// 当前统计绿色平均值（U10） | [0x0~0x3ff]
    FH_UINT16 u16BAvg;// 当前统计蓝色平均值（U10） | [0x0~0x3ff]
    FH_UINT8 u08Pos;// 当前色温位置 0：最靠近左侧统计点 255：最靠近右侧统计点 | [0x0~0xff]
    FH_UINT8 u08CurrLeft;// 当前色温左侧统计点index | [0x0~0xf]
    FH_UINT8 u08CurrRight;// 当前色温右侧统计点index | [0x0~0xf]
    FH_UINT16 u16CurrCCT;// 当前计算CCT值 | [0x0~0xffff]
    FH_UINT8 u08UnknownProb;// 当前为不确定场景的概率，单位为百分比 | [0x0~0x64]
    FH_UINT8 u08PureColorProb;// 当前为纯色场景的概率，单位为百分比 | [0x0~0x64]
    FH_UINT8 u08OutdoorProb;// 当前为室外场景的概率，单位为百分比 | [0x0~0x64]
    FH_UINT8 u08LowLightProb;// 当前为低光场景的概率，单位为百分比 | [0x0~0x64]
    FH_UINT16 u16Ls0CCT;// 当前主色温的CCT值 | [0x0~0xffff]
    FH_UINT8 u08Ls0Area;// 当前主色温的面积 | [0x0~0x64]
    FH_UINT16 u16Ls1CCT;// 当前次色温的CCT值 | [0x0~0xffff]
    FH_UINT8 u08Ls1Area;// 当前次色温的面积 | [0x0~0x64]
    FH_UINT8 u08IllumiantSingleProb;// 当前为单色温场景的概率，单位为百分比 | [0x0~0x64]
    FH_UINT8 u08IllumiantMultiProb;// 当前为混合色温场景的概率，单位为百分比 | [0x0~0x64]
    FH_UINT8 u08BlueSkyProb;// 当前为蓝天场景权重，仅室外时判断 | [0x0~0x64]
    FH_UINT8 u08GreenTreeProb;// 当前为绿树场景权重，仅室外时判断 | [0x0~0x64]
    FH_UINT16 u16CtNum[16];// 色温0的blk数量 | [0x0~0xffff]
    FH_UINT16 u16CtSpecNum[4];// 特殊白点0的blk数量 | [0x0~0xffff]
    FH_UINT16 u16X[64];// 色温框坐标x0,U4.12(工具显示色温框使用) | [0x0~0xffff]
    FH_UINT16 u16Y[64];// 色温框坐标y0 U4.12 (工具显示色温框使用) | [0x0~0xffff]
    FH_UINT16 u16WpX[64];// 白块0坐标x0 | [0x0~0xffff]
    FH_UINT16 u16WpY[64];// 白块0坐标y0 | [0x0~0xffff]
    FH_UINT16 u16Cnt[64];// 有效白块0数量，归一化到65535 | [0x0~0xffff]
} ISP_AWB_STATUS;

/**|CCM|**/
typedef struct _CCM_TABLE_S_
{
    FH_SINT16 s16CcmTable[4][12];  // ccm矩阵表 | [-4096~0xfff]
} CCM_TABLE;

typedef struct _ISP_CCM_CFG_S_ {
    FH_SINT16 s16CcmTable[12];  // ccm矩阵表 | [-4096~0xfff]
} ISP_CCM_CFG;

typedef struct _ISP_ACR_CFG_S_
{
    FH_BOOL bAcrEn;// Acr控制使能 | [0x0~0x1]
    FH_UINT8 u08AcrMap[12];// 0db时，色彩校正弱化增益U.6，0x40表示全用ccm矩阵 | [0x0~0x40]
} ISP_ACR_CFG;

/**|GAMMA|**/
typedef enum IPB_GAMMA_MODE_S_ {
    GAMMA_OFF       = 0, // 关闭GAMMA
    GAMMA_MODE_YC   = 1, // GAMMA采用YC模式
    GAMMA_MODE_RGB  = 2, // GAMMA采用RGB模式
    GAMMA_MODE_RGBY = 3, // GAMMA采用RGBY模式
    IPB_GAMMA_MODE_DUMMY =0xffffffff,
} IPB_GAMMA_MODE;

typedef enum _GAMMA_BUILTIN_IDX_ {
    GAMMA_CURVE_10 = 0, // GAMMA曲线1.0
    GAMMA_CURVE_12 = 1, // GAMMA曲线1.2
    GAMMA_CURVE_14 = 2, // GAMMA曲线1.4
    GAMMA_CURVE_16 = 3, // GAMMA曲线1.6
    GAMMA_CURVE_18 = 4, // GAMMA曲线1.8
    GAMMA_CURVE_20 = 5, // GAMMA曲线2.0
    GAMMA_CURVE_22 = 6, // GAMMA曲线2.2
    GAMMA_CURVE_24 = 7, // GAMMA曲线2.4
    GAMMA_CURVE_26 = 8, // GAMMA曲线2.6
    GAMMA_CURVE_28 = 9, // GAMMA曲线2.8
    GAMMA_BUILTIN_IDX_DUMMY =0xffffffff,
} GAMMA_BUILTIN_IDX;

typedef struct _ISP_GAMMA_CFG_S_
{
    FH_BOOL        bGammaEn;    // gamma控制使能 | [0~1]
    IPB_GAMMA_MODE u8GammaMode; // GAMMA模块输入选择	0: gamma off	1: YC mode	2: RGB mode	3: RGBY mode |[0-0x3]
    CURVE_TYPE        eCCurveType;       // c gamma控制模式:0 内置gamma表;1 用户自定义gamma表 | [0~1]
    FH_UINT16         u16CGamma[80];    // 用户自定义cgamma表 | [0~0x3ff]
    CURVE_TYPE        eYCurveType;       // y gamma控制模式:0 内置gamma表;1 用户自定义gamma表 | [0~1]
    FH_UINT16         u16YGamma[80];    // 用户自定义ygamma表 | [0~0x3ff]
} ISP_GAMMA_CFG;
/**|BLC|**/
typedef struct _ISP_BLC_CFG_S_
{
    FH_BOOL bBlcEn;// Blc控制使能 | [0x0~0x1]
    FH_BOOL bMode;// 0: manual 1: gain mapping | [0x0~0x1]
    FH_BOOL bChanelModeEn;// 0:不分通道减dc,仅R通道参数生效 1: 分通道减dc | [0x0~0x1]
    FH_BOOL bLocationSel;// 0: blc在nr3d前减 1: blc在nr3d后减 | [0x0~0x1]
    FH_SINT16 s16BlcManuR;// 手动R通道dc偏移 | [-512~511]
    FH_SINT16 s16BlcManuG;// 手动G通道dc偏移 | [-512~511]
    FH_SINT16 s16BlcManuB;// 手动B通道dc偏移 | [-512~511]
    FH_SINT8 s08BlcDeltaR;// 手动R通道dc值偏移 | [-8~7]
    FH_SINT8 s08BlcDeltaG;// 手动G通道dc值偏移 | [-8~7]
    FH_SINT8 s08BlcDeltaB;// 手动B通道dc值偏移 | [-8~7]
    FH_UINT16 u16BlcRMap[12];// 增益为0dB的R通道BLC值 | [0x0~0x3ff]
    FH_UINT16 u16BlcGMap[12];// 增益为0dB的G通道BLC值 | [0x0~0x3ff]
    FH_UINT16 u16BlcBMap[12];// 增益为0dB的B通道BLC值 | [0x0~0x3ff]
    FH_SINT8 s08BlcDeltaRMap[12];// 增益为0dB的R通道精细BLC补偿（locationSel =1生效） | [-128~127]
    FH_SINT8 s08BlcDeltaGMap[12];// 增益为0dB的G通道精细BLC补偿（locationSel =1生效） | [-128~127]
    FH_SINT8 s08BlcDeltaBMap[12];// 增益为0dB的B通道精细BLC补偿（locationSel =1生效） | [-128~127]
} ISP_BLC_CFG;
/**|GB|**/
typedef struct _ISP_GB_CFG_S_
{
    FH_BOOL bGbEn;// Gb控制使能 | [0x0~0x1]
    FH_UINT16 u16THL;// 下阈值 | [0x0~0xff]
    FH_UINT16 u16THH;// 上阈值 | [0x0~0xff]
} ISP_GB_CFG;
/**|DPC|**/
typedef struct _ISP_DPC_CFG_S_
{
    FH_BOOL bDpcEn;// Dpc控制使能 | [0x0~0x1]
    FH_BOOL bMode;// 0: manual 1: gain mapping | [0x0~0x1]
    FH_UINT8 u08CtrlMode;// 白天去坏点控制模式： 0~3，值越大越强 | [0x0~0x3]
    FH_UINT8 u08Enable;// 0：关闭去坏点 1：打开去白点功能 2：打开去黑点功能 3：同时打开去白点黑点功能 | [0x0~0x3]
    FH_UINT8 u08Str;// 手动DPC强度 | [0x0~0x7]
    FH_UINT8 u08WDc;// 手动白点门限值 | [0x0~0xff]
    FH_UINT8 u08BDc;// 手动黑点门限值 | [0x0~0xff]
    FH_UINT8 u08StrMap[12];// DPC强度（增益0dB时） | [0x0~0xf]
    FH_UINT8 u08WDcMap[12];// w_dc值（增益0dB时） | [0x0~0xff]
    FH_UINT8 u08BDcMap[12];// b_dc值（增益0dB时） | [0x0~0xff]
    FH_UINT8 u08CtrlModeMap[12];// 去坏点模式（增益0dB时） | [0x0~0xf]
} ISP_DPC_CFG;

/**|LSC|**/
typedef struct _ISP_LSC_CFG_S_
{
    FH_BOOL   bLscEn;       // 镜头阴影矫正控制使能 | [0~1]
    FH_UINT32 u32Coff[299]; // 补偿系数 | [0~0xffffffff]
} ISP_LSC_CFG;

/**|NR3D|**/
typedef struct _NR3D_STAT_CFG_S_
{
    FH_UINT8 u08WinNumH;// 统计垂直方向统计窗个数 | [0x0~0x1f]
    FH_UINT8 u08WinNumW;// 统计水平方向统计窗个数 | [0x0~0x1f]
    FH_UINT8 u08WinNumD;// 统计值域方向统计窗个数 | [0x0~0x10]
} NR3D_STAT_CFG;
typedef struct _ISP_NR3D_CFG_S_
{
    FH_BOOL bNr3dEn;// Nr3d控制使能 | [0x0~0x1]
    FH_BOOL bNr3dMode;// NR3D control mode 0: manual 1: gain mapping | [0x0~0x1]
    FH_BOOL bNr3dHwEnable;// nr3d硬件模块开关使能 | [0x0~0x1]
    FH_BOOL bCoeffPresetMap;// 0: 采用内置coeff映射表（由coeffMapIdx决定） 1：采用自定义coeff映射表 | [0x0~0x1]
    FH_BOOL bCoeffMappingEn;// NR3D coeff映射线以及处理方式控制模式： 0: 手动模式 1: gain mapping | [0x0~0x1]
    FH_UINT8 u08CoeffMapIdx;// Coeff映射线（建议白天配置0或1）： 0~7：过渡越来越平滑，但同时强度变弱 | [0x0~0x7]
    FH_UINT8 u08RefRatio;// 参考帧运动信息迭代因子，越大衰减越快 | [0x0~0x20]
    NR3D_STAT_CFG stNr3dStatCfg;// Nr3d统计窗口配置 | [NR3D_STAT_CFG]
    FH_UINT8 u08MdFltCof;// 运动信息滤波比例，值越大越倾向于判断成运动。 | [0x0~0xff]
    FH_UINT8 u08RefCoeffTh;// 参考帧运动信息阈值，参考帧coeff会被限制在 refcoeffTh之上， 值越大，nr3d 强度越弱，噪声越大。 | [0x0~0xff]
    FH_UINT16 u16MdGain;// 运动强度增益 | [0x0~0xffff]
    FH_UINT8 u08MdCoeffTh;// 运动coeff门限值，小于此门限认为是静止，coeff曲线以mdCoeffTh作为分界点，位宽只有8bit | [0x0~0xff]
    FH_UINT16 u16LumaX0;// 亮度映射x0（x0>0） | [0x0~0x3ff]
    FH_UINT16 u16LumaY0;// 亮度映射y0 | [0x0~0x3ff]
    FH_UINT16 u16LumaX1;// 亮度映射x1 | [0x0~0x3ff]
    FH_UINT16 u16LumaY1;// 亮度映射y1 | [0x0~0x3ff]
    FH_UINT16 u16LumaX2;// 亮度映射x2 | [0x0~0x3ff]
    FH_UINT16 u16LumaY2;// 亮度映射y3 | [0x0~0x3ff]
    FH_UINT16 u16LumaYOffset;// 亮度映射x=0时对应y值 | [0x0~0x3ff]
    FH_UINT16 u16LumaY3;// 亮度映射y4，x=1023时对应的y值 | [0x0~0x3ff]
    FH_UINT16 u16Slope;// 噪声标定估计斜率U10.4 | [0x0~0x3fff]
    FH_UINT16 u16Offset;// 噪声标定估计偏移U16 | [0x0~0xffff]
    FH_UINT8 u08OffsetShift;// 噪声估计偏移精度，默认为0，最大为4 | [0x0~0x4]
    FH_UINT16 u16MdGainMap[12];// 0db，运动估计强度 | [0x0~0xffff]
    FH_UINT8 u08MdFltCofMap[12];// 0db时，运动信息滤波比例 | [0x0~0xff]
    FH_UINT8 u08RefCoeffThMap[12];// 0db时，参考帧运动coeff最小值 | [0x0~0xff]
    FH_UINT8 u08RefRatioMap[12];// 0db时，参考帧运动信息迭代因子，越大衰减越快 | [0x0~0x20]
    FH_UINT8 u08Nr3dCoeffIdxMap[12];// nr3d coeff映射线（增益0dB时） | [0x0~0xe]
    FH_UINT16 u16SlopeMap[12];// 0db，噪声标定估计斜率U10.4 | [0x0~0xffff]
    FH_UINT16 u16OffsetStrMap[12];// 0db，噪声标定估计偏移强度 | [0x0~0xffff]
} ISP_NR3D_CFG;

typedef struct _ISP_NR3D_COEFF_CFG_
{
    FH_UINT8 u8CoeffY[25]; // NR3D Coeff 26段映射线的折点纵坐标。Y0<=Y1...<=Y24 | [0~0xff]
} ISP_NR3D_COEFF_CFG;

/**|NR3D_MD|**/
typedef struct _ISP_NR3D_MD_STAT_
{
    FH_UINT32    u32Base;       // 数据物理地址 | [0~0xffffffff]
    FH_UINT32    u32Vbase;      // 数据虚拟地址 | [0~0xffffffff]
    FH_UINT32    u32Width;      // 8倍下采样的幅面宽 | [0x0~0xffff]
    FH_UINT32    u32Height;     // 8倍下采样的幅面高 | [0x0~0xffff]
    FH_UINT64    u64TimeStamp;  // 时间戳  | [0x0~0xfffffff]
    FH_UINT64    u64FrameId;    // 帧号 | [0x0~0xffffffffffffffff]
} ISP_NR3D_MD_STAT;

typedef struct _ISP_NR3D_MD_STAT_CFG_
{
    FH_UINT8     u08MdStatTh;          // NR3D_MD_STAT 运动占比阈值(归一化至255)，在一个统计窗口内累计运动超过此比例则MD数据为1 | [0x0~0xff]
    FH_UINT8     u08MdCountTh;         // NR3D_MD_STAT 运动判断阈值，超过此阈值判断为运动 | [0x0~0xff]
} ISP_NR3D_MD_STAT_CFG;
/**|LTM|**/
typedef struct _ISP_LTM_CFG_S_
{
    FH_BOOL    bLtmEn;               // 线性增益模式:0 手动,1 gainMapping | [0~1]
    CURVE_TYPE eCurveType;           //曲线预设类型 | [1~2]
    FH_UINT32  u32TonemapCurve[128]; // ltm曲线设置表格 | [1~0xffffffff]
    FH_UINT32  u32LtmIdx;            //当mode为normal模式时：	0~14: 内部表系数	15:用户定义 | [0-0xf]
    FH_BOOL bMode;                   // 0：宽动态模式	1：非宽动态模式 | [0~1]
    FH_BOOL  bLtmHwEn;               // Ltm硬件模块开关 | [0-0x1]
    FH_BOOL  bYPreScalerEn;          //高亮时正常亮度和高光处比例系数设置使能 | [0-0x1]
    FH_UINT8 u08HlYScaler;           //高光处的亮度比例系数，值越大越亮，U.5 | [0-0x3f]
    FH_UINT8 u08NlYScaler;           //正常亮度处的亮度比例系数，值越大越亮，U.5， | [0-0x3f]
    FH_UINT8 u08YMaxRatio;           //统计得到的像素最大值的补偿比例因子，U.6 | [0-0xff]
    FH_UINT8 u08YMeanRatio;          //统计得到的像素平均值的补偿比例因子，U.6 | [0-0xff]
    FH_UINT8 u08YMinRatio;           //统计得到的像素最小值的补偿比例因子，U.6 | [0-0xff]
    FH_UINT8 u08NlCtr;   //正常亮度处的局部对比度，值越大对比度越大，但噪声也越大 | [0-0xff]
    FH_UINT8 u08Weight1; //正常亮度处新旧LTM融合比例，值越小，新LTM越多 | [0-0xff]
    FH_UINT8 u08Weight3; //高亮处新旧LTM融合比例，值越小，新LTM越多 | [0-0xff]
    FH_UINT8 u08Weight0; //低亮处新旧LTM融合比例，值越小，新LTM越多 | [0-0xff]
    FH_UINT8 u08Weight2; //较亮处新旧LTM融合比例，值越小，新LTM越多 | [0-0xff]
    FH_UINT8 u08NlYScalerMap[GAIN_NODES]; //正常亮度比例系数 | [0-0xff]
    FH_UINT8 u08HlYScalerMap[GAIN_NODES]; //高光处比例系数 | [0-0xff]
    FH_UINT8 u08NlCtrMap[GAIN_NODES];     //局部对比度 | [0-0xff]
    FH_UINT8 u08LtmWinIdxMap[GAIN_NODES]; //window idx选择 | [0-0xf]
    FH_UINT8 u08EpsWdr;                   //宽动态场景下滤波器参数值 | [0-0xf]
    FH_UINT8 u08EpsLinear;                //普通场景下滤波器参数值 | [0-0xf]
    FH_UINT8 u08NlYPreScaler[4];             //(1/16|1/8|1/4|1/2 inttMax)时正常亮度处的亮度比例系数，值越大越亮，U.5， | [0-0x3f]
    FH_UINT8 u08HlYPreScaler[4];             //(1/16|1/8|1/4|1/2 inttMax)时高亮处的亮度比例系数，值越大越亮，U.5， | [0-0x3f]
} ISP_LTM_CFG;
/**|MD|**/
typedef struct _ISP_MD_STAT_S_
{
    FH_UINT32         u32MdEn;    //模块使能状态，状态为1时获取到的值为有效 | [0~1]
    FH_UINT32         u32MdMove;  //模块运动状态统计 | [0~0x180]
    FH_UINT32         u32MdStill; //模块静止状态统计 | [0~0x180]
} ISP_MD_STAT;
/**|NR2D|**/
typedef struct _ISP_LUT_POINT_S_
{
    FH_UINT16 u16X;// 坐标系中X轴坐标值 | [0x0~0xffff]
    FH_UINT16 u16Y;// 坐标系中Y轴坐标值 | [0x0~0xffff]
} ISP_LUT_POINT;
typedef struct _ISP_NR2D_CFG_S_
{
    FH_BOOL bNr2dEn;// Nr2d控制使能 | [0x0~0x1]
    FH_BOOL bNr2dMode;// NR2D 控制模式 0: 手动模式 1: gain mapping | [0x0~0x1]
    FH_UINT8 u08Nr2dSft;// 2d降噪移位,U4 | [0x0~0xf]
    FH_BOOL bLumaLutSet;// 内置的LumaLut组别选择： 0：第0组； 1：第1组； | [0x0~0x1]
    FH_BOOL bSigLutSet;// 内置的SigLut组别选择： 0：第0组； 1：第1组； | [0x0~0x1]
    FH_BOOL bSigLutMode;// siglut的曲线模式： 0：采用内置的SigLut, 根据SigLutIdx确定； 1：自定义模式，用户通过NR2D_SIGLUT_CONFIG0/1/2设置sigLut曲线； Range: 0x0-0x1 | [0x0~0x1]
    FH_UINT8 u08SiglutIdx;// siglut的曲线选择，越大去噪越强 | [0x0~0xf]
    FH_BOOL bInttMappingEn;// Gain mapping控制模式： 0: intt mapping关闭 1: intt mapping 开启 | [0x0~0x1]
    FH_BOOL bNr2dMenEn;// Nr2d联动使能开关 | [0x0~0x1]
    FH_UINT16 u16K1;// Long frame: the slope of “slope K”,U8.8 | [0x0~0xffff]
    FH_UINT8 u08K2;// Long frame: the offset of “slope K”,U8 | [0x0~0xff]
    FH_SINT32 s32O1;// Long frame: the slope of “offset O”,S20.8 | [-134217728~134217727]
    FH_SINT32 s32O2;// Long frame: the offset of “offset O”,S25 | [-16777216~16777215]
    FH_UINT8 u08SlopeStr;// 噪声斜率估计强度，U4.4 | [0x0~0xff]
    FH_UINT8 u08OffsetStr;// 噪声偏移估计强度，U4.4 | [0x0~0xff]
    FH_UINT8 u08SigLutX[6];// 输入coef x0（0-255） x越小，代表越接近静止区 x越大，代表越偏向运动区 x0固定为0 | [0x0~0xff]
    FH_UINT8 u08SigLutY[6];// 输出coef y0,越大去噪越强 | [0x0~0xff]
    FH_UINT8 u08SlopeInttStrMap[4];// 1/16 inttMax时，噪声斜率估计强度，U4.4 | [0x0~0xff]
    FH_UINT8 u08SlopeStrMap[12];// 0db时，噪声斜率估计强度，U4.4 | [0x0~0xff]
    FH_UINT8 u08OffsetStrMap[12];// 0db时，噪声偏移估计强度，U4.4 | [0x0~0xff]
    FH_UINT8 u08AddLutX[4];// 输入coef x0（0-255） 固定为0 x越小，代表越接近静止区 x越大，代表越偏向运动区 | [0x0~0xff]
    FH_UINT8 u08AddLutY[4];// 输出coef y0,越大去噪越强 | [0x0~0xff]
    FH_UINT16 u16LumaLutX[4];// LUT表横坐标x0（0-4095） 固定为0 代表输入亮度值 | [0x0~0xfff]
    FH_UINT8 u08LumaLutY[4];// LUT表纵坐标y0,越小去噪强度越强 | [0x0~0xff]
    FH_UINT8 u08AddLutIdxMap[12];// 0db, 回加曲线选择，值越大去噪越强 | [0x0~0xf]
    FH_UINT8 u08LumaLutIdxMap[12];// 0db, 根据亮度去噪曲线选择，值越大去噪越强 | [0x0~0xf]
    FH_UINT8 u08AddLutIdX1OffsetMap[12];// 0db, 根据Coef去噪曲线X1偏移值，值越大越偏向运动 | [0x0~0xf]
    FH_UINT8 u08AddLutIdX2OffsetMap[12];// 0db, 根据Coef去噪曲线X2偏移值，值越大越偏向运动 | [0x0~0xf]
    FH_UINT8 u08SigLutIdxMap[16];// 1/16 inttMax, SigLut曲线选择，值越大去噪越强 | [0x0~0xf]
} ISP_NR2D_CFG;
typedef struct _ISP_NR2D_DPC_CFG_S_
{
    FH_BOOL bMode;// 0: 手动模式 1: gain mapping | [0x0~0x1]
    FH_UINT8 u08Enable;// 0：关闭去坏点 1：打开去白点功能 2：打开去黑点功能 3：同时打开去白点黑点功能 | [0x0~0x3]
    FH_UINT8 u08Str;// 手动DPC强度, 0~7 | [0x0~0x7]
    FH_UINT8 u08WDc;// 手动白点门限值 | [0x0~0xff]
    FH_UINT8 u08BDc;// 手动黑点门限值 | [0x0~0xff]
    FH_UINT8 u08StrMap[12];// DPC强度（增益0dB时） | [0x0~0x7]
    FH_UINT8 u08WDcMap[12];// w_dc值（增益0dB时） | [0x0~0xff]
    FH_UINT8 u08BDcMap[12];// b_dc值（增益0dB时） | [0x0~0xff]
} ISP_NR2D_DPC_CFG;

/**|DRC|**/
typedef struct _DRC_STAT_CFG_S_
{
    FH_UINT8 u08WinNumH;// 直方图统计垂直方向统计窗个数 | [0x0~0x1f]
    FH_UINT8 u08WinNumW;// 直方图统计水平方向统计窗个数 | [0x0~0x1f]
    FH_UINT8 u08WinNumD;// 直方图统计值域方向统计窗个数 | [0x0~0xf]
} DRC_STAT_CFG;
typedef struct _ISP_DRC_CFG_S_
{
    FH_BOOL bDrcEn;// Drc控制使能 | [0x0~0x1]
    FH_BOOL bDrcMode;// DRC控制模式 0：手动模式 1：gain mapping | [0x0~0x1]
    FH_BOOL bSatEn;// 饱和度使能开关 | [0x0~0x1]
    FH_BOOL bDrcCurveMode;// 0：采用内置曲线 1：采用用户自定义曲线 | [0x0~0x1]
    DRC_STAT_CFG stDrcStatCfg;// Drc统计窗口配置 | [DRC_STAT_CFG]
    FH_UINT8 u08LightStr;// 明度强度控制参数。值越大，高亮/高饱和度区域越亮 | [0x0~0x7f]
    FH_UINT16 u16GtmTrF;// 全局亮度控制：值越大，图像越亮 | [0x0~0xfff]
    FH_UINT16 u16MaxGain;// DRC最大增益值。该值越大，DRC可被允许拉亮的倍率越大，最高不会超过16倍。 取值范围：[0,4095]。 | [0x0~0xfff]
    FH_UINT16 u16MinGain;// DRC最小增益值。该值越小，DRC可被允许压暗的比例越大，默认不会超过1/16。 取值范围：[0,4095]。 | [0x0~0xfff]
    FH_UINT16 u16GlobalStr;// DRC强度，值越大，DRC的效果越明显 | [0x0~0x7ff]
    FH_UINT8 u08GrayProtTh;// 灰色保护阈值。取值范围[0,255]。 | [0x0~0xff]
    FH_UINT8 u08GrayProtShift;// 灰色保护移位值。取值范围[0,15]。 | [0x0~0xf]
    FH_UINT8 u08GrayProtStr;// 灰色保护强度。取值范围[0,255]。默认精度为U4.4。 | [0x0~0xff]
    FH_UINT8 u08LutRangeY[20];// 值域自适应LUT0，128代表1.0  [0-128] | [0x0~0xff]
    FH_UINT8 u08LutSatY[36];// 饱和度自适应LUT0，128代表1.0  [0-128] | [0x0~0xff]
    FH_UINT16 u16DataX0[97];// 系数0 | [0x0~0xffff]
    FH_UINT16 u16DataX1[97];// 系数1 | [0x0~0xffff]
    FH_UINT8 u08LightStrMap[12];// 0db明度强度控制参数 | [0x0~0x7f]
    FH_UINT16 u16GtmTrFMap[12];// 0db全局亮度 | [0x0~0xfff]
    FH_UINT16 u16GlobalStrMap[12];// 0db DRC强度 | [0x0~0x7ff]
    FH_UINT8 u08GrayProtStrMap[12];// 0db灰色保护强度。取值范围[0,255]。默认精度为U4.4。 | [0x0~0xff]
    FH_UINT16 u16GlobalStrInttMap[8];// 1/256 inttMax DRC强度 | [0x0~0x7ff]
} ISP_DRC_CFG;
/**|HLR|**/
typedef struct _ISP_HLR_CFG_S_
{
    FH_BOOL bHlrHwEn;// HLR硬件使能开关 | [0x0~0x1]
    FH_UINT16 u16MaxValue;// HLR处理高光最大值，大于该值进行处理 | [0x0~0x3ff]
} ISP_HLR_CFG;
/**|LCE|**/
typedef struct _ISP_LCE_CFG_S_
{
    FH_BOOL bLceEn;// Lce控制使能 | [0x0~0x1]
    FH_BOOL bLceHwEn;// 硬件开关 0:关闭 1:打开 | [0x0~0x1]
    FH_BOOL bLceMode;// LCE控制模式 0：手动模式 1：gain mapping | [0x0~0x1]
    FH_BOOL bGtmEn;// 全局亮度控制使能 0:关闭 1:打开 | [0x0~0x1]
    FH_BOOL bLceCurveMode;// 0：采用内置曲线 1：采用用户自定义曲线 | [0x0~0x1]
    FH_UINT8 u08GlobalStr;// 直方图均衡程度，该值越大，对比度越强 | [0x0~0xff]
    FH_UINT8 u08SmoothStr;// 直方图均衡平滑程度，该值越大，越平滑，即对比越弱 | [0x0~0xff]
    FH_UINT8 u08WinNumH;// 直方图统计垂直方向统计窗个数 | [0x0~0x1f]
    FH_UINT8 u08WinNumW;// 直方图统计水平方向统计窗个数 | [0x0~0x1f]
    FH_UINT8 u08SpatialFilterCoef;// 空域滤波程度，该值越小，空域滤波效果越局部 | [0x0~0x7]
    FH_UINT16 u16TprFilterCoef;// 时域滤波程度，该值越大，当前帧权重越大，即时域滤波越弱 | [0x0~0x1ff]
    FH_UINT8 u08KernelSel;// Kernel选择 0：杂 1：5x5 2：7x7 3：9x9 | [0x0~0x3]
    FH_UINT16 u16ColorOffset;// 色度补偿偏置值 | [0x0~0x3ff]
    FH_UINT8 u08DataX[50];// 系数0 | [0x0~0xff]
    FH_UINT8 u08GlobalStrMap[12];// 0db对比度整体强度 | [0x0~0xff]
    FH_UINT8 u08SmoothStrMap[12];// 0db平滑强度 | [0x0~0xff]
} ISP_LCE_CFG;
/**|IE|**/
typedef struct _ISP_CONTRAST_CFG_S_
{
    FH_BOOL  bYcEn;   // 亮度控制使能 | [0~1]
    FH_BOOL  bGainMappingEn;// 对比度控制模式 0: 手动模式 1: gain mapping | [0x0~0x1]
    FH_UINT8 u08Crt;// 手动模式下contrast U2.6 | [0x0~0xff]
    FH_UINT8 u08CrtInttMap[INTT_NODES];// 1/16 inttMax对比度U2.6 | [0x0~0xff]
    FH_UINT8 u08CrtMap[12];// 0dB时，对比度U2.6 | [0x0~0xff]
} ISP_CONTRAST_CFG;
typedef struct _ISP_BRIGHTNESS_CFG_S_
{
    FH_BOOL  bYcEn;   // 亮度控制使能 | [0~1]
    FH_BOOL  bGainMappingEn; // 0：手动	1：增益映射 | [0~1]
    FH_SINT8 s08Brt;         // 手动模式下亮度S8(-128到127) | [-128~127]
    FH_SINT8 s08BrtMap[12];// 0dB时，亮度S8 | [-128~127]
} ISP_BRIGHTNESS_CFG;
/**|CE|**/
typedef struct _ISP_SAT_CFG_S_
{
    FH_BOOL bSatEn;// Sat控制使能 | [0x0~0x1]
    FH_BOOL bGainMappingEn;// 饱和度控制模式 0: 手动模式 1: gain mapping | [0x0~0x1]
    FH_SINT8 s08RollAngle;// 色度旋转因子，范围为~64°-63°，最高比特为符号位，S7 | [-64~63]
    FH_UINT8 u08Sat;// 饱和度增益 U3.5 | [0x0~0xff]
    FH_UINT8 u08BlueSup;// 蓝色抑制增益 U2.6 | [0x0~0xff]
    FH_UINT8 u08RedSup;// 红色抑制增益 U2.6 | [0x0~0xff]
    FH_BOOL bGlutEn;// 亮度增益抑制使能： 0: 关闭 1: 开启 | [0x0~0x1]
    FH_BOOL bPresetLut;// 预设亮度增益抑制LUT 0:采用内置lut映射表； 1：采用自定义lut映射表 | [0x0~0x1]
    FH_BOOL bThreshEn;// 阈值抑制使能： 0: 关闭 1: 开启 | [0x0~0x1]
    FH_BOOL bThhMode;// 抑制阈值控制模式: 0:手动模式;  1:gain mapping | [0x0~0x1]
    FH_UINT8 u08ThhSup;// 抑制阈值:值越小抑制作用越强 | [0x0~0xff]
    FH_UINT8 u08CeGainWeight;// 亮度增益抑制LUT融合权重，值越大抑制作用越强 | [0x0~0xff]
    FH_UINT16 u16SatParaR1;// 白色门限值，饱和度小于该值时会被设置为0，即成为白色 | [0x0~0x1ff]
    FH_UINT8 u08SatMap[12];// 0dB时，饱和度U2.5 | [0x0~0xff]
    FH_UINT8 u08ThhMap[12];// 0dB时，抑制阈值U2.6 | [0x0~0xff]
    FH_UINT8 u08WeightMap[12];// 0dB时，融合权重 | [0x0~0xff]
    FH_UINT8 u08Gain[128];// 亮度增益抑制映射表：128项 | [0x0~0xff]
} ISP_SAT_CFG;
/**|APC|**/
typedef struct _ISP_APC_CFG_S_
{
    FH_BOOL bApcEn;// Apc控制使能 | [0x0~0x1]
    FH_BOOL bGainMappingEn;// apc control mode 0: manual 1: gain mapping | [0x0~0x1]
    FH_BOOL bLutMappingEn;// lut control mode 0: manual 1: gain mapping | [0x0~0x1]
    FH_BOOL bFilterMappingEn;// filter control mode 0: manual 1: gain mapping | [0x0~0x1]
    FH_BOOL bMergeSel;// Detail Enhancer和Edge Sharpener合并模式选择 0：相加 1：取绝对最大值 | [0x0~0x1]
    FH_BOOL bLutMode;// LUT 系数选择模式： 0：系数表选择模式 1：手动配置系数 | [0x0~0x1]
    FH_BOOL bFilterMode;// 滤波器选择模式: 0：系数表选择模式 1：手动配置系数 | [0x0~0x1]
    FH_UINT8 u08DetailLutNum;// 细节增强LUT系数选择 | [0x0~0x7]
    FH_UINT8 u08EdgeLutNum;// 边界锐化LUT系数选择 | [0x0~0x7]
    FH_UINT8 u08DetailFNum;// 细节滤波器选择 | [0x0~0x3]
    FH_UINT8 u08EdgeFNum;// 边界滤波器选择 | [0x0~0x7]
    FH_UINT8 u08DetailLutGroup;// 细节增强LUT组别选择（两组） | [0x0~0xf]
    FH_UINT8 u08EdgeLutGroup;// 边界增强LUT组别选择（两组） | [0x0~0xf]
    FH_UINT8 u08Pgain;// 总体APC 正向增益 | [0x0~0xff]
    FH_UINT8 u08Ngain;// 总体APC 负向增益 | [0x0~0xff]
    FH_UINT8 u08DetailGain;// 细节锐化强度 | [0x0~0xff]
    FH_UINT8 u08EdgeGain;// 边界锐化强度 | [0x0~0xff]
    FH_UINT16 u16DetailThl;// 细节增强下门限值 | [0x0~0x3ff]
    FH_UINT16 u16DetailThh;// 细节增强上门限值 | [0x0~0x3ff]
    FH_UINT16 u16EdgeThl;// 边界增强下门限值 | [0x0~0x3ff]
    FH_UINT16 u16EdgeThh;// 边界增强上门限值 | [0x0~0x3ff]
    FH_BOOL bShootEnable;// 过冲抑制使能 | [0x0~0x1]
    FH_UINT8 u08OverShootStr;// 正向过冲抑制强度 | [0x0~0x1f]
    FH_UINT8 u08UnderShootStr;// 负向过冲抑制强度 | [0x0~0x1f]
    FH_UINT8 u08EdgeGainMap[12];// 0dB时，边界锐化强度 | [0x0~0xff]
    FH_UINT8 u08DetailGainMap[12];// 0dB时，细节锐化强度 | [0x0~0xff]
    FH_UINT8 u08PgainMap[12];// 0dB时，白边界强度 | [0x0~0xff]
    FH_UINT8 u08NgainMap[12];// 0dB时，黑边界强度 | [0x0~0xff]
    FH_UINT8 u08DetailLutNumMap[12];// LUT系数选择（0db） | [0x0~0xf]
    FH_UINT8 u08EdgeLutNumMap[12];// LUT系数选择（0db） | [0x0~0xf]
    FH_UINT8 u08DetailFNumMap[12];// filter系数选择（增益0dB时） | [0x0~0xf]
    FH_UINT8 u08EdgeFNumMap[12];// filter系数选择（增益0dB时） | [0x0~0xf]
} ISP_APC_CFG;

typedef struct _ISP_APC_MT_CFG_S_ {
    FH_BOOL bApcMtEn;// apc联动使能： 0: Off 1: On | [0x0~0x1]
    FH_BOOL bGainMappingEn;// apc联动模式： 0: manual 1: gain mapping | [0x0~0x1]
    FH_UINT8 u08MtStr;// 联动强度，越大运动区域越模糊 | [0x0~0xff]
    FH_UINT16 u16MtOffset;// 联动偏移，越大运动区域越模糊 | [0~511]
    FH_UINT16 u16ApcCoeffMin;// 联动系数最小值。范围0-256 | [0x0~0x1ff]
    FH_UINT16 u16ApcCoeffMax;// 联动系数最大值。范围0-256 | [0x0~0x1ff]
    FH_UINT8 u08MtStrMap[12];// 0dB时，联动强度 | [0x0~0xff]
} ISP_APC_MT_CFG;

typedef struct _SHARPEN_STAT_CFG_S{
    FH_UINT16 w;  // 配置的统计窗口宽 | [1, PicW]
    FH_UINT16 h;  // 配置的统计窗口的高 | [1, PicH]
    FH_UINT16 x_offset;  // 统计窗口的水平偏移 | [0, PicW - w]
    FH_UINT16 y_offset;  // 统计窗口的垂直偏移 | [0, PicH - h]
} SHARPEN_STAT_CFG;

typedef struct _SHARPEN_STAT_S{
    FH_UINT32 sum;  // 获取到的锐度统计信息 | [0~0xffffffff]
} SHARPEN_STAT;

/**|YCNR|**/
typedef struct _ISP_YNR_CFG_S_ {
    FH_BOOL bYnrEn;// Ynr控制使能 | [0x0~0x1]
    FH_BOOL bYnrMode;// 运动系数映射亮度去噪强度曲线控制模式 0：手动模式 1：gain mapping | [0x0~0x1]
    FH_BOOL bYnrSigmaTabMode;// 去噪强度映射表控制模式 0：采用内置映射表 1：采用自定义映射表 | [0x0~0x1]
    FH_UINT8 u08StrengthCtrlMode;// 0:弱去噪模式 1:中去噪模式 2:强去噪模式 3:超强去噪模式 | [0x0~0x3]
    FH_BOOL bCoefEn;// ynr 联动使能 0: Off 1: On | [0x0~0x1]
    FH_BOOL bSigmaLutMode;//去噪强度LUT配置模式 0：选择内置LUT用idx控制 1：手动配置映射线纵坐标 | [0x0~0x1]
    FH_BOOL bAddLutMode;//残差回加强度LUT配置模式 0：选择内置LUT用idx控制 1：手动配置映射线纵坐标 | [0x0~0x1]
    FH_UINT8 u08SigmaLutNum;// 去噪强度LUT系数选择 | [0x0~0x2a]
    FH_UINT8 u08AddLutNum;// 残差回归LUT系数选择 | [0x0~0xe]
    FH_UINT8 u08SigmaShift;// 去噪强度斜率精度 | [0x0~0xf]
    FH_UINT8 u08AddShift;// 残差回加斜率精度 | [0x0~0xf]
    FH_UINT8 u08CoeffThl;// 小运动分界点0~255 | [0x0~0xff]
    FH_UINT8 u08CoeffThh;// 大运动分界点coeffThl~255 | [0x0~0xff]
    FH_UINT8 u08SigmaY[4];//Sigma映射纵坐标 | [0x0~0xff]
    FH_UINT8 u08AddY[4];//Add残差回加纵坐标 | [0x0~0xff]
    FH_UINT8 u08SigmaLutNumMap[16];// LUT系数选择（1/16 inttMax） | [0x0~0x2a]
    FH_UINT8 u08AddLutNumMap[16];// LUT系数选择（1/16 inttMax） | [0x0~0xe]
    FH_UINT8 u08StrengthCtrlModeMap[12];// 去噪模式选择,越大越（0db） | [0x0~0x3]
    FH_UINT8 u08CoeffThlMap[12];// 0db小运动分界点 | [0x0~0xff]
    FH_UINT8 u08CoeffThhMap[12];// 0db大运动分界点 | [0x0~0xff]
    FH_UINT16 u16Level[128];// 去噪强度下限映射表：128项 | [0x0~0xffff]
} ISP_YNR_CFG;

typedef struct _ISP_CNR_CFG_S_
{
    FH_BOOL bCnrEn;// Cnr控制使能 | [0x0~0x1]
    FH_BOOL bSatLimitMode;// 高饱和度抑制曲线控制模式 0: 手动模式 1: gain mapping | [0x0~0x1]
    FH_BOOL bSigmaCoeffMode;// 运动系数映射色度去噪强度曲线控制模式 0: 手动模式 1: gain mapping | [0x0~0x1]
    FH_BOOL bSigmaTabMode;// 去噪强度映射表控制模式 0：采用内置映射表 1：采用自定义映射表 | [0x0~0x1]
    FH_BOOL bCoefEn;// cnr 联动使能 0: Off 1: On | [0x0~0x1]
    FH_BOOL bSatCoefEn;// 饱和度抑制联动使能 0: Off 1: On | [0x0~0x1]
    FH_UINT8 u08SatCoefmapSlope;// 根据运动系数计算去噪保护权重的阈值 | [0x0~0xff]
    FH_UINT8 u08SatCoefmapOffest;// 根据运动系数计算去噪保护权重的偏置 | [0x0~0xff]
    FH_UINT8 u08SatLimitLutGroup;// 高饱和度抑制LUT组别选择（两组） 0: 高低饱和度去噪强度不同 1：高低饱和度区域去噪相同 | [0x0~0x3]
    FH_UINT8 u08SatLimitCoefLutGroup;// 高饱和度抑制联动LUT组别选择（两组） 0：不同运动强度去噪不同 1：不同运动强度去噪相同 | [0x0~0x3]
    FH_UINT8 u08SatLimitLutNum;// 高饱和度抑制LUT系数选择0~15 | [0x0~0xf]
    FH_UINT8 u08SatLimitShift;// 高饱和度抑制曲线斜率精度 | [0x0~0xf]
    FH_UINT8 u08SigmaCoeffLutNum;// 高饱和度抑制联动LUT系数选择 0~15 | [0x0~0xf]
    FH_UINT8 u08SigmaCoeffShift;// 色度去噪强度曲线斜率精度 | [0x0~0xf]
    FH_UINT8 u08SatLimitCoefLutNum;// 高饱和度抑制联动LUT系数选择0~15 | [0x0~0xf]
    FH_UINT8 u08SatLimitCoefShift;// 高饱和度抑制联动曲线斜率精度 | [0x0~0xf]
    FH_UINT8 u08SigmaCoefVRatio;// 色度去噪强度垂直方向强度偏移U.5 | [0x0~0xff]
    FH_UINT16 u16MaxSat;// 饱和度计算上限值 | [0x0~0x1ff]
    FH_UINT16 u16SatTh;// 根据饱和度计算去噪保护权重的阈值 | [0x0~0x1ff]
    FH_UINT8 u08SatSlope;// 根据饱和度计算去噪权重的斜率，值越大，去噪强度越弱 | [0x0~0xff]
    FH_UINT8 u08SatMapShift;// 根据饱和度计算去噪保护权重的斜率精度 | [0x0~0xf]
    FH_UINT8 u08SatLimitNumMap[12];// LUT系数选择（0db） | [0x0~0xf]
    FH_UINT8 u08SatLimitCoefNumMap[12];// LUT系数选择（0db） | [0x0~0xf]
    FH_UINT8 u08SigmaCoeffNumMap[12];// LUT系数选择（0db） | [0x0~0xf]
    FH_UINT16 u16Level[64];// 去噪强度下限映射表：128项 | [0x0~0xffff]
} ISP_CNR_CFG;

/**|PURPLE|**/
typedef struct _ISP_PURPLEFRI_CFG_S_
{
    FH_BOOL bPurpleEn;// Purple控制使能 | [0x0~0x1]
    FH_BOOL bMode;// 去紫边控制模式 0: 手动模式; 1: gain mapping | [0x0~0x1]
    FH_BOOL bHwEnable;// 0: 关闭去紫边 1: 打开去紫边 | [0x0~0x1]
    FH_UINT8 u08StrMax;// 去紫边最大强度 | [0x0~0xff]
    FH_UINT16 u16HueThL;// 紫边判断区域下阈值（0~359），单位为度，（逆时针计算） | [0x0~0x167]
    FH_UINT16 u16HueThH;// 紫边判断区域上阈值（0~359），单位为度，（逆时针计算） HueThH > HueThL | [0x0~0x167]
    FH_UINT16 u16HueOffset;// 保护区域色调计算偏置（影响起始点的位置，（顺时针计算）（0~359） | [0x0~0x167]
    FH_UINT16 u16LumaThl;// 紫边判断亮度下阈值，l~h紫边权重线性递增 | [0x0~0x3ff]
    FH_UINT16 u16LumaThh;// 紫边判断亮度上阈值，大于该值则紫边权重最大 lumaThh > lumaThl | [0x0~0x3ff]
    FH_UINT16 u16LumaThlMap[12];// 0db时，紫边判断亮度下阈值  | [0x0~0x3ff]
    FH_UINT16 u16EdgeThlMap[12];// 0db时，紫边判断边界下阈值  | [0x0~0x3ff]
} ISP_PURPLEFRI_CFG;

/**|GME|**/
typedef struct _ISP_GME_PARAM_S_
{
    FH_SINT32 s32x; // x vector | [0~0xffffffff]
    FH_SINT32 s32y; // y vector | [0~0xffffffff]
} ISP_GME_PARAM;

/**|POST_GAIN|**/
typedef struct _ISP_POST_GAIN_CFG_S
{
    FH_UINT16 rGain;        //R分量增益 | [0-0xfff]
    FH_UINT16 gGain;        //G分量增益 | [0-0xfff]
    FH_UINT16 bGain;        //B分量增益 | [0-0xfff]
    FH_UINT16 rOffset;      //R分量黑度补偿 | [0-0xfff]
    FH_UINT16 gOffset;      //G分量黑度补偿 | [0-0xfff]
    FH_UINT16 bOffset;      //B分量黑度补偿 | [0-0xfff]
} ISP_POST_GAIN_CFG;

/**|RGBA|**/
typedef struct _RGBA_ABAL_CFG_S_
{
    FH_UINT8 abalSpeed;          // Abal的调整速度 | [0~0xf]
    FH_UINT8 abalThl;            // Abal不调整的阈值 | [0~0xf]
    FH_UINT8 abalThh;            // Abal开始调整的阈值 | [0~0xf]
    FH_UINT8 abalStabTime;       // Abal的稳定时间 | [0~0xff]
    FH_UINT8 abalStep;           // ABal调整的步长 | [0~0xf]
    FH_UINT8 abalTargetStabTime; // AbalTarget的稳定时间 | [0~0xff]
    FH_UINT8 abalTargetThr;      // AbalTarget的调整阈值 | [0~0xf]
} RGBA_ABAL_CFG;

typedef struct _RGBA_WEIGHT_CFG_S_
{
    FH_UINT8  W2Speed;    // W2的调整速度 | [0~0xf]
    FH_UINT8  W2Thl;      // W2的不调整阈值 | [0~0xf]
    FH_UINT8  W2Thh;      // W2开始调整的阈值 | [0~0xf]
    FH_UINT8  W2StabTime; // W2的稳定时间 | [0~0xf]
    FH_UINT8  W2Step;     // W2的调整步长 | [0~0xf]
    FH_UINT8  W2AThl;     // W2计算时增益的上阈值 | [0~0x7f]
    FH_UINT8  W2AThh;     // W2计算时增益的下阈值 | [0~0x7f]
    FH_UINT16 W2GThl;     // W2计算时红外的下阈值 | [0~0x7f]
    FH_UINT16 W2GThh;     // W2计算时红外的上阈值 | [0~0x7f]
} RGBA_WEIGHT_CFG;

typedef struct _RGBA_CCM_TABLE_S_
{
    FH_UINT16 RgbaCcmTable[2][16]; // rgba ccm矩阵系数 | [-4096~4095]
} RGBA_CCM_TABLE;

typedef struct _RGBA_CONVERT_TH_S_
{
    FH_UINT16 convThl; // 格式转换模块LUT上阈值 | [0~0xffff]
    FH_UINT16 convThh; // 格式转换模块LUT下阈值 | [0~0xffff]
} RGBA_CONVERT_TH;

typedef struct _RGBA_ENHANCE_TH_S_
{
    FH_UINT16 enhThl; // 亮度/锐度增强模块亮度下阈值 | [0~0xffff]
    FH_UINT16 enhThh; // 亮度/锐度增强模块亮度上阈值 | [0~0xffff]
} RGBA_ENHANCE_TH;

typedef struct _RGBA_ETH_S_
{
    FH_UINT16 eThl; // 边界强度下限 | [0~0xffff]
    FH_UINT16 eThh; // 边界强度上限 | [0~0xffff]
} RGBA_ETH;

typedef struct _RGBA_EGAIN_CFG_S_
{
    FH_UINT8 egainR; // 亮度/锐度增强模块R通道增强增益 | [0~0xff]
    FH_UINT8 egainG; // 亮度/锐度增强模块G通道增强增益 | [0~0xff]
    FH_UINT8 egainB; // 亮度/锐度增强模块B通道增强增益 | [0~0xff]
} RGBA_EGAIN_CFG;

typedef struct _RGBA_DEFAULT_CFG_S_
{
    FH_BOOL         bRgbaEn;     // RGBA控制使能 | [0~1]
    FH_BOOL         bIrmode;     // Abal是否使能 | [0~1]
    FH_BOOL         bFirFrm;     // 是否是第一帧标志 | [0~1]
    RGBA_ABAL_CFG   abalParam;   // Abal参数结构体 | [RGBA_ABAL_CFG]
    RGBA_WEIGHT_CFG weightParam; // W2参数结构体 | [RGBA_WEIGHT_CFG]
    RGBA_CCM_TABLE  ccmTab;      // ccm矩阵表 | [RGBA_CCM_TABLE]
    RGBA_CONVERT_TH convTh;      // 格式转换LUT阈值参数结构体 | [RGBA_CONVERT_TH]
    RGBA_ENHANCE_TH enhTh;       // 亮度/锐度阈值参数结构体 | [RGBA_ENHANCE_TH]
    RGBA_ETH        eTh;         // 边界强度参数结构体 | [RGBA_ETH]
    RGBA_EGAIN_CFG  egain;       // 分通道亮度/锐度增强模块增益参数结构体 | [RGBA_EGAIN_CFG]
} RGBA_DEFAULT_CFG;

typedef struct _RGBA_CURR_INFO_S_
{
    FH_UINT16 u08RBalGainPre;   // 上一帧RBalGain | [0~0xffff]
    FH_UINT8  u08RBalCnt;       // 当前RBal稳定时间计数 | [0~0xff]
    FH_UINT8  u08RBalTargetCnt; // 当前RBal目标值的稳定时间计数 | [0~0xff]
    FH_UINT16 u16W2Pre;         // 上一帧CCM2权重 | [0~0x7fff]
    FH_UINT16 u16W2Cnt;         // W2的稳定时间计数 | [0~0xff]
} RGBA_CURR_INFO;

typedef enum _RGBA_CMD_KEY_S_ {
    SET_RGBA_DEFAULT   = 0x0, // 配置RGBA　DRV寄存器
    SET_RGBA_EN        = 0x1, // RGBA策略使能开关
    SET_RGBA_FIRFRM    = 0x2, // 配置RGBA处理第一帧标志
    SET_RGBA_IRMODE    = 0x3, // Abal使能
    SET_ABAL_PARA      = 0x4, // 配置Abal相关DRV参数
    SET_WEIGHT_PARA    = 0x5, // 配置W2相关DRV参数
    SET_RGBA_CCM_TABLE = 0x6, // 配置RGBA的CCM矩阵
    SET_CONVERT_TH     = 0x7, // 格式转换LUT上下阈值配置
    SET_ENHANCE_TH     = 0x8, //  亮度/锐度上下阈值设置
    SET_ETH            = 0x9, // 边界强度上下阈值设置
    SET_EGAIN          = 0xa, // RGB通道亮度/锐度增强模块增益设置
    RGBA_CMD_KEY_DUMMY =0xffffffff,
} RGBA_CMD_KEY;

typedef struct _RGBA_STAT_INFO_S_
{
    struct rgba_blk_info
    {
        FH_UINT32 R_sum;  // RGBA统计值R sum | [0~0xffffffff]
        FH_UINT32 G_sum;  // RGBA统计值G sum | [0~0xffffffff]
        FH_UINT32 B_sum;  // RGBA统计值B sum | [0~0xffffffff]
        FH_UINT32 A_sum;  // RGBA统计值A sum | [0~0xffffffff]
        FH_UINT32 IR_sum; // RGBA统计值IR sum | [0~0xffffffff]
        FH_UINT32 cnt;    // RGBA统计值CNT | [0~0xffffffff]
    } blk_rgba[32];
} RGBA_STAT_INFO;

typedef struct _RGBA_DPC_CFG_S
{
    FH_BOOL   bRGBADpcEn;              // RGBA模式下的dpc控制使能 | [0~1]
    FH_BOOL   bGainMappingEn;          // 0：手动	1：增益映射 | [0~1]
    FH_UINT8  u08DpcMode;              // 0:all close;1:white pixel correct open;2.black pixel correct open;3.all open.
    FH_UINT8  u08ModeIdx;              // 去坏点强度 | [0~2]
    FH_UINT8  u08Str;                  // nr2d去噪强度,U1.3 | [0-0xf]
    FH_UINT8  u08wdc;                  //手动白点门限值 | [0-0xff]
    FH_UINT8  u08bdc;                  //手动黑点门限值 | [0-0xff]
    FH_UINT8  u08ThhIR;                // IR通道比值上阈值 | [0~0x3f]
    FH_UINT8  u08ThlIR;                // IR通道比值下阈值 | [0~0xf]
    FH_UINT8  u08ThhG;                 // G通道比值上阈值 | [0~0x3f]
    FH_UINT8  u08ThlG;                 // G通道比值下阈值 | [0~0xf]
    FH_UINT16 u16DiffMin;              // 变化程度的最小值 | [0~0x3ff]
    FH_UINT8  u08DiffShift;            // 变化程度的移位值 | [0~0xf]
    FH_UINT8  u08Strenght[GAIN_NODES]; // 包括:mode,w_s,b_s
    FH_UINT8  u08WhiteThr[GAIN_NODES]; //白点门限DC值
    FH_UINT8  u08BlackThr[GAIN_NODES]; //黑点门限值
} RGBA_DPC_CFG;

/**|SMART IR|**/
typedef struct _SMART_IR_CFG_S
{
    FH_BOOL bFgcMode;// 夜状态时副载波控制模式 | [0x0~0x1]
    FH_UINT8 u08IrImpulseWidth;// ICR控制方波宽度（帧数） | [0x0~0xff]
    FH_UINT16 u16IrDelay;// ICR的延时帧数 | [0x0~0xffff]
    FH_UINT16 u16GainDayToNight;// 白天切黑夜增益门限 U10.6 | [0x0~0xffff]
    FH_UINT16 u16GainNightToDay;// 黑夜切白天增益门限 U10.6 | [0x0~0xffff]
} SMART_IR_CFG;

/**|FC|**/

typedef struct _FC_CFG_S
{
    FH_UINT8 u08Str;// 去假色强度 | [0x0~0xff]
    FH_UINT16 u16OffsetT;// 去假色纹理信息偏移量 | [0x0~0x3ff]
    FH_UINT16 u16OffsetY;// 去假色亮度信息偏移量 | [0x0~0x3ff]
} FC_CFG;

/**|HSV|**/
typedef struct _ISP_HSV_CFG_S_
{
    FH_BOOL bHsvEn;// Hsv控制使能 | [0x0~0x1]
    FH_BOOL bColorCtrlEn;// 颜色校正功能开关 0：关闭； 1：打开。 | [0x0~0x1]
    FH_BOOL bSatCtrlEn;// 饱和度调整功能开关 0：关闭； 1：打开。 | [0x0~0x1]
    FH_BOOL bVibranceEn;// 自然饱和度调整功能开关， 0:普通饱和度;  1:自然饱和度； 当satCtrlEn=1时有效。 | [0x0~0x1]
    FH_UINT8 u08SatRefMode;// 饱和度变化模式选择，选择RGB通道的不同类别信息来代表当前像素基准值。 0：以灰度为标准进行饱和度校正（默认）； 1：以最大值为标准进行饱和度校正； 2：以最小值为标准进行饱和度校正； 3：以最大值最小值的均值为标准进行饱和度校正。 | [0x0~0x3]
    FH_SINT8 s08Increment;// 自然饱和度调整参数，值越大饱和度越大，值小于0时饱和度减小。[-100~100] | [-128~127]
    FH_SINT8 s08HueByHueStr[36];// 纵坐标色相偏向0 | [-128~127]
    FH_UINT8 u08SatByHueStr[36];// 纵坐标色饱和度强度0，U2.6精度 | [0x0~0xff]
    FH_UINT8 u08SatBySatStr[12];// 纵坐标色饱和度强度0，U2.6精度 | [0x0~0xff]
    FH_UINT8 u08SatByYStr[12];// 纵坐标色饱和度强度0，U2.6精度 | [0x0~0xff]
    FH_UINT8 u08SatBySYStr[12];// 纵坐标色饱和度强度0，U2.6精度 | [0x0~0xff]
    FH_UINT8 u08GrayProtStr;// 灰色保护强度，值越小时保护力度越大，取值范围0-64，64：不保护； 0：最强保护。 | [0x0~0x7f]
    FH_UINT8 u08GrayProtTh;// 灰色保护阈值，值越小越接近灰色，阈值之内完全使用grayProtStr。 | [0x0~0xff]
    FH_UINT8 u08GrayProtXEnd;// 灰色保护结束值 | [0x0~0xff]
    FH_UINT8 u08GrayProtYEnd;// 灰色保护强度结束值 | [0x0~0xff]
} ISP_HSV_CFG;

/**|LDC|**/
typedef struct _ISP_LDC_CFG_S_
{
    FH_BOOL bLdcEn;// Ldc控制使能 | [0x0~0x1]
    FH_BOOL bLdcHwEnable;// LDC硬件开关使能 | [0x0~0x1]
    FH_UINT16 u16DistortionRatio;// LDC矫正比例，单位千分之几 | [0x0~0x3e8]
    FH_UINT16 u16CenterX;// 水平光学中心横坐标 | [0x0~0xfff]
    FH_UINT16 u16CenterY;// 水平光学中心纵坐标 | [0x0~0xfff]
} ISP_LDC_CFG;

/**|YCGAIN|**/
typedef struct _YC_COEFF_STRUCT_S_
{
    FH_SINT16 s12Rcoeff;    //s2.10 rgb2yuv时r通道系数|[0~0xfff]
    FH_SINT16 s12Gcoeff;    //s2.10 rgb2yuv时g通道系数|[0~0xfff]
    FH_SINT16 s12Bcoeff;    //s2.10 rgb2yuv时b通道系数|[0~0xfff]
    FH_SINT16 s12Offset;    //s12 rgb2yuv时偏移|[0~0xfff]
} YC_COEFF_STRUCT;

typedef struct _ISP_YC_GAIN_STRUCT_S_
{
    YC_COEFF_STRUCT Y;    // Y通道系数 | [YC_COEFF_STRUCT]
    YC_COEFF_STRUCT Cb;   // Cb通道系数 | [YC_COEFF_STRUCT]
    YC_COEFF_STRUCT Cr;   // Cr通道系数 | [YC_COEFF_STRUCT]
} ISP_YC_GAIN_STRUCT;

typedef struct _ISP_YC_GAIN_CFG_S_
{
    FH_UINT8 u08YcGainMode;          // rgb2yuv的转换方式，0：BT601（默认）;1：BT709;2：用户自定义 | [0~2]
    ISP_YC_GAIN_STRUCT strYcGain;   // 用户自定义rgb2yuv的转换矩阵系数 | [YC_COEFF_STRUCT]
} ISP_YC_GAIN_CFG;

/**|STATISTICS|**/
typedef enum _HIST_MODE
{
    Y_MODE = 0,//1024段统计
    RGB_MODE = 1,//4通道256段统计
    HIST_MODE_DUMMY = 0xffffffff,
} HIST_MODE;

typedef enum _STAT_POS_SEL_AWB_ADV
{
    POS_SEL_AWB_ADV_AFTER_DG0 = 0,//ISP_DG0后进行AWB统计
    POS_SEL_AWB_ADV_AFTER_DG3 = 1,//ISP_DG3后进行AWB统计
    POS_SEL_AWB_ADV_AFTER_LUT = 2,//ISP_LUT后进行AWB统计
    POS_SEL_AWB_ADV_AFTER_LSC = 3,//ISP_LSC后进行AWB统计
    POS_SEL_AWB_DUMMY = 0xffffffff,
}STAT_POS_SEL_AWB_ADV;

typedef enum _STAT_POS_SEL_GL
{
    POS_SEL_GL_AFTER_DG0 = 0,//ISP_DG0(LSC后的Gain)后进行AE统计
    POS_SEL_GL_AFTER_DG1 = 1,//ISP_DG1(NR2D后的Gain)后进行AE统计
    POS_SEL_AE_DUMMY = 0xffffffff,
}STAT_POS_SEL_GL;

typedef enum _STAT_POS_SEL_AF
{
    POS_SEL_AF_AFTER_LUT  = 0,//ISP_LUT后进行AF统计
    POS_SEL_AF_AFTER_DG2  = 1,//ISP_DG2后进行AF统计
    POS_SEL_AF_AFTER_NR3D = 2,//ISP_DG2后进行AF统计，FH885XV300不支持
    POS_SEL_AF_AFTER_DRC  = 3,//ISP_DG2后进行AF统计，FH885XV300不支持
    POS_SEL_AF_DUMMY = 0xffffffff,
}STAT_POS_SEL_AF;

typedef enum _STAT_POS_SEL_HIST
{
    POS_SEL_HIST_AFTER_DG2 = 0,//ISP_DG2后进行HIST统计
    POS_SEL_HIST_AFTER_DG3 = 1,//ISP_DG3后进行HIST统计
    POS_SEL_HIST_AFTER_WB3 = 2,//ISP_WB3后进行HIST统计
    POS_SEL_HIST_DUMMY = 0xffffffff,
}STAT_POS_SEL_HIST;

typedef enum _STAT_POS_SEL_MD
{
    POS_SEL_MD_AFTER_DG2 = 0,//ISP_DG2后进行MD统计
    POS_SEL_MD_AFTER_DG3 = 1,//ISP_DG3后进行MD统计
    POS_SEL_MD_AFTER_WB3 = 2,//ISP_WB3后进行MD统计
    POS_SEL_MD_AFTER_DUMMY = 0xffffffff,
}STAT_POS_SEL_MD;

typedef enum _STAT_POS_SEL_DRC
{
    POS_SEL_DRC_MATCH_WB3 = 0,//ISP_MATCH_WB3后进行DRC统计
    POS_SEL_DRC_WB3 = 1,//ISP_WB3后进行DRC统计
    POS_SEL_DRC_DUMMY = 0xffffffff,
}STAT_POS_SEL_DRC;

typedef struct _ISP_STAT_WIN_S_
{
    FH_UINT16 winHOffset;  // 统计窗口水平偏移 | [0~PicW]
    FH_UINT16 winVOffset;  // 统计窗口垂直偏移 | [0~PicH]
    FH_UINT16 winHSize;    // 统计窗口宽度 | [0~(PicW-winHOffset)]
    FH_UINT16 winVSize;    // 统计窗口高度 | [0~(PicH-winVOffset)]
} ISP_STAT_WIN;

typedef struct _ISP_STAT_WIN1_S_
{
    FH_UINT16 winHOffset;  // 统计窗水平方向偏移 | [0~0xfff]
    FH_UINT16 winVOffset;  // 统计窗垂直方向偏移 | [0~0xfff]
    FH_UINT16 winHSize;    // 统计窗总宽度 winHOffset+winHSize<=picW | [0~0xfff]
    FH_UINT16 winVSize;    // 统计窗总高度 winHOffset+winHSize<=picH| [0~0xfff]
    FH_UINT8 winHCnt;      // 全局统计水平方向窗口个数 | [1~32]
    FH_UINT8 winVCnt;      // 全局统计垂直方向窗口个数 | [1~32]
} ISP_STAT_WIN1;

typedef struct _AWB_BLOCK_STAT_S_
{
    FH_UINT32 u32AwbBlockR;    // AWB R分量sum统计值 | [0~0xffffffff]
    FH_UINT32 u32AwbBlockG;    // AWB G分量sum统计值 | [0~0xffffffff]
    FH_UINT32 u32AwbBlockB;    // AWB B分量sum统计值 | [0~0xffffffff]
    FH_UINT32 u32AwbBlockCnt;  // AWB 统计值cnt值 | [0~0xffffffff]
} AWB_BLOCK_STAT;


typedef struct _ISP_AWB_STAT2_S_
{
    AWB_BLOCK_STAT stBlkStat;  // AWB统计,最大支持32*32,位置可选择在长帧上或合成后,具体可看框图 | [AWB_BLOCK_STAT]
} ISP_AWB_ADV_STAT;



typedef struct _ISP_AWB_ADV_STAT_CFG_S_
{
    ISP_STAT_WIN1 stStatWin;      // 统计窗口配置 winHCnt/winVCnt取值范围[1~8] | [ISP_STAT_WIN1]
    FH_UINT16 u16YHighThreshold;  // awb统计上門限值 | [0~0xfff]
    FH_UINT16 u16YLowThreshold;   // awb统计低門限值 | [0~0xfff]
    STAT_WHITE_POINT stPoint[6];  // awb白框点，A，B，C，D，E，F六个点，Ax=Bx By=Cy Dy=Ey Ex=Fx | [STAT_WHITE_POINT]
} ISP_AWB_ADV_STAT_CFG;

typedef union _ISP_HIST_STAT_S_
{
    struct _hist_bin {
        FH_UINT32 cnt; // hist统计cnt值 | [0~0xffffffff]
        FH_UINT32 sum; // hist统计sum值 | [0~0xffffffff]
    }bin[33];
} ISP_HIST_STAT;

typedef struct _ISP_HIST_STAT_CFG_S_
{
    ISP_STAT_WIN stStatWin;        // 统计窗口配置 | [ISP_STAT_WIN]
} ISP_HIST_STAT_CFG;

typedef struct _GLOBE_STAT_S
{
    struct _Block_gstat
    {
        FH_UINT32 sum;  // 全局统计sum值 | [0~0xffffffff]
        FH_UINT32 cnt;  // 全局统计cnt值 | [0~0xffffffff]
        FH_UINT32 max;  // 统计块内出现的亮度最大值 | [0~0xffffffff]
        FH_UINT32 min;  // 统计块内出现的亮度最小值 | [0~0xffffffff]
    } r, gr, gb, b;
} GLOBE_STAT;

typedef struct _GLOBE_STAT_CFG_S
{
    ISP_STAT_WIN1 stStatWin;  // 统计窗口配置,winHCnt/winVCnt取值范围[1~16] winHSize和winVSize需小于512,窗口个数较小时需注意 | [ISP_STAT_WIN1]
    FH_UINT16 u16ChnThH;      // 统计像素上阈值,默认0xfff | [0x0~0x3ff]
    FH_UINT16 u16ChnThL;      // 统计像素下阈值,默认0 | [0x0~0x3ff]
    STAT_POS_SEL_GL StatPosSel;  // 统计位置 0:LSC后的Gain0，1:NR2D后的Gain1 | [STAT_POS_SEL_GL]
} GLOBE_STAT_CFG;

typedef struct _BLC_STAT_S
{
    FH_UINT16 u16BlcR; //R通道BLC值  | [0~0xffff]
    FH_UINT16 u16BlcG; //G通道BLC值  | [0~0xffff]
    FH_UINT16 u16BlcB; //B通道BLC值  | [0~0xffff]
} ISP_BLC_STAT;

/**|FAST_BOOT|**/
typedef struct _STRATEGY_INIT_CFG_S
{
    FH_BOOL   bFisrtRunFlag;     // 初始化第一次运行标志位，第一次运行置1, 非首次置0 | [0x0~0x1]
    FH_UINT32 u32CurrTotalGain;  // 初始化当前曝光总增益U.6  | [0x40~0xffffffff]
    FH_UINT16 u16CurrInttLine;   // 初始化当前曝光时间，单位为行  | [1~0xffff]
    FH_UINT16 u16MaxInttLine;    // 初始化最大曝光时间，单位为行  | [1~0xffff]
    FH_UINT16 u16SensorAgainMin; // 初始化Sensor Again最小值U.6  | [0x40~0xffff]
    FH_UINT16 u16DgainMin;       // 初始化ISP Dgain最小值U.6  | [0x40~0xffff]
} STRATEGY_INIT_CFG;

typedef struct _BLC_INIT_CFG_S
{
    FH_UINT16 blc_level; //黑电平值  | [0~0xffff]
} BLC_INIT_CFG;

typedef struct _WB_INIT_CFG_S
{
    FH_UINT16 wbGain[3];  // 白平衡三通道增益值rgain, ggain, bgain; | [0~0x1fff]
} WB_INIT_CFG;

typedef struct _CCM_INIT_CFG_S
{
    FH_UINT8  ccmIdx;     // 选择使用ccm的序号 | [0~1]，此平台只支持一个ccm 0
    FH_SINT16 ccmCfg[9];  // 颜色矫正矩阵 | [-4096~4095]
} CCM_INIT_CFG;

typedef struct _DPC_INIT_CFG_S
{
    FH_UINT8 ctrlMode;  // 去坏点模式 | [0~3]
    FH_UINT8 str;  // 去坏点比例門限值 | [0~7]
} DPC_INIT_CFG;

typedef struct _APC_INIT_CFG_S
{
    FH_UINT8 mergeSel;     // 细节增强和边界锐化合并模式 U1 | [0x0~0x1]
    FH_UINT8 positiveStr;  // 总体正向增益, U8 | [0~0xff]
    FH_UINT8 negativeStr;  // 总体负向增益, U8 | [0~0xff]
} APC_INIT_CFG;

typedef struct _YNR_INIT_CFG_S
{
    FH_UINT8 th_str[3];  // th_str[0] 高频亮度阈值, th_str[1] 中频亮度阈值, th_str[2] 低频亮度阈值，U4.4 | [0~0xff]
} YNR_INIT_CFG;

typedef struct _CTR_INIT_CFG_S
{
    FH_UINT16 ctr;  // 对比度增强因子,U2.6 | [0~255]
} CTR_INIT_CFG;

typedef struct _SAT_INIT_CFG_S
{
    FH_UINT16 sat;  // 饱和度调整因子,U3.5 | [0~0xff]
    FH_UINT16 blue_sup;  // 饱和度蓝色分量,U2.6 | [0~200]
    FH_UINT16 red_sup;  // 饱和度红色分量,U2.6 | [0~200]
} SAT_INIT_CFG;

typedef struct _GAMMA_MODE_INIT_CFG_S
{
    FH_UINT16 gamma_mode;  // 当前gamma运行模式 0：gamma关 1：YC 2：RGB 3：RGBY | [0~3]
} GAMMA_MODE_INIT_CFG;

typedef struct _GAMMA_INIT_CFG_S
{
    FH_UINT32 gamma[512];  // gamma表对应的值 | [0~0xffffffff]
} GAMMA_INIT_CFG;

typedef struct _ISP_GAIN_INIT_CFG_S
{
    FH_UINT32 u32IspGain; // isp增益值 | [0~0xffffffff]
} ISP_GAIN_INIT_CFG;

typedef struct _MAP_INTT_GAIN_CFG_S
{
    FH_SINT32 currFormat;  //当前的sensor format | [0~0xffffffff]
    FH_SINT32 desFormat;   //目标的sensor format | [0~0xffffffff]
    FH_UINT32 currIntt;    //当前的曝光时间，单位为行 | [0~0xffffffff]
    FH_UINT32 currTotalGain; //当前的曝光增益，u.6精度 | [0~0xffffffff]
    FH_UINT32 desIntt;     //计算返回的曝光时间，单位为行 | [0~0xffffffff]
    FH_UINT32 desTotalGain; //计算返回的曝光增益，u.6精度 | [0~0xffffffff]
} MAP_INTT_GAIN_CFG;

typedef struct _GPIO_PARAM_CFG_S
{
    FH_BOOL gpio_enable;  //第二阶段是否需要拉高拉低GPIO | [0-0x1]
    FH_UINT32 gpio_time;  //第二阶段拉高拉低需要配置的延时　| [0-0xffffffff]
} GPIO_PARAM_CFG;

typedef struct _BLC_PARAM_CFG_S
{
    FH_UINT32 blc_value;  //配置的第一阶段blc值 | [0~0xffff]
} BLC_PARAM_CFG;

typedef struct _NEW_SNS_ADDR_S_
{
    FH_UINT32 u32newI2cAddr; //新配置的i2c地址 | [0~0xffffffff]
    FH_UINT32 u32SnsId; // 设备号 | [0xffffffff]
} NEW_SNS_ADDR_S;

typedef union _ISP_SENSOR_COMMON_CMD_DATA0_U_
{
    BLC_PARAM_CFG blc_param_cfg;        // BLC参数 | [BLC_PARAM_CFG]
    GPIO_PARAM_CFG gpio_param_cfg;      // GPIO相关的参数配置 | [GPIO_PARAM_CFG]
    MAP_INTT_GAIN_CFG mapInttGainCfg;   // 曝光相关的一些参数 | [MAP_INTT_GAIN_CFG]
    NEW_SNS_ADDR_S new_sns_addr;        // i2c地址参数配置 | [NEW_SNS_ADDR_S]
    unsigned int dw[128];               // 其他的一些保留参数 | [0~0xffffffff]
} ISP_SENSOR_COMMON_CMD_DATA0;

typedef union _ISP_SENSOR_COMMON_CMD_DATA1_
{
    unsigned int dw[128];               // 其他的一些保留参数 | [0~0xffffffff]
} ISP_SENSOR_COMMON_CMD_DATA1;
/*******************************************************************/


/**|DBG|**/
enum _DBG_OUT_DATA_TYPE_
{
    DATA_TYPE_RAW = 0,  // DBG模块的导出的数据格式为RAW格式
    DATA_TYPE_RGB = 1,  // DBG模块的导出的数据格式为RGB格式
    DATA_TYPE_YUV = 2,  // DBG模块的导出的数据格式为YUV422格式
    DATA_TYPE_YUV444 = 3,  // DBG模块的导出的数据格式为YUV444格式
};

/**|RUBBISH|**/
typedef enum _SNS_CLK_S_ {
    SNS_CLK_24_POINT_0   = 0x1,
    SNS_CLK_27_POINT_0   = 0x2,
    SNS_CLK_37_POINT_125 = 0x3,
    SNS_CLK_DUMMY =0xffffffff,
} SNS_CLK;

typedef enum _SNS_DATA_BIT_S_ {
    LINER_DATA_8_BITS  = 0x1,
    LINER_DATA_12_BITS = 0x2,
    LINER_DATA_14_BITS = 0x3,
    WDR_DATA_16_BITS   = 0x4,
    SNS_DATA_BITS_DUMMY =0xffffffff,
} SNS_DATA_BITS;

typedef enum _SIGNAL_POLARITY_S_ {
    ACTIVE_HIGH = 0x0,
    ACTIVE_LOW = 0x1,
    SIGNAL_POLARITY_DUMMY =0xffffffff,
} SIGNAL_POLARITY;

typedef struct _ISP_VI_HW_ATTR_S_
{
    SNS_CLK         eSnsClock;         // 配置的cis时钟 | [SNS_CLK]
    SNS_DATA_BITS   eSnsDataBits;      // 配置的sensor数据位宽 | [SNS_DATA_BITS]
    SIGNAL_POLARITY eHorizontalSignal; // 时钟水平极性 | [0~1]
    SIGNAL_POLARITY eVerticalSignal;   // 时钟垂直极性 | [0~1]
    FH_BOOL         u08Mode16;         //  | [ ]
    FH_UINT32       u32DataMaskHigh;   //  | [ ]
    FH_UINT32       u32DataMaskLow;    //  | [ ]
} ISP_VI_HW_ATTR;

typedef struct _ISP_ALGORITHM_S_
{
    FH_UINT8 u08Name[16];    //  | [ ]
    FH_UINT8 u08AlgorithmId; //  | [ ]
    FH_VOID (*run)(FH_VOID); //  | [ ]
} ISP_ALGORITHM;

typedef struct _ISP_DEBUG_INFO_S_
{
    FH_UINT32 envLuma;
    FH_UINT32 sqrtenvLuma;
    FH_UINT32 sensor_gain;
    FH_UINT32 isp_gain;
} ISP_DEBUG_INFO;

typedef struct _ISP_DEFAULT_PARAM_
{
    ISP_BLC_CFG        stBlcCfg;     //　 | [ ]
    ISP_DRC_CFG        stDRCCfg;     //  | [ ]
    ISP_GAMMA_CFG      stGamma;      //  | [ ]
    ISP_SAT_CFG        stSaturation; //  | [ ]
    ISP_APC_CFG        stApc;        //  | [ ]
    ISP_CONTRAST_CFG   stContrast;   //  | [ ]
    ISP_BRIGHTNESS_CFG stBrt;        //  | [ ]
    ISP_NR3D_CFG       stNr3d;       //  | [ ]
    ISP_NR2D_CFG       stNr2d;       //  | [ ]
    ISP_YNR_CFG        stYnr;        //  | [ ]
    ISP_CNR_CFG        stCnr;        //  | [ ]
    ISP_DPC_CFG        stDpc;        //  | [ ]
    ISP_LSC_CFG        stLscCfg;     //  | [ ]
} ISP_DEFAULT_PARAM;

#pragma pack()
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*_ISP_COMMON_H_*/
