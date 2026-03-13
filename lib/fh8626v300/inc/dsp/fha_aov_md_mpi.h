#ifndef __FHA_MD_H__
#define __FHA_MD_H__

#include "fha_defs.h"
#include "fha_type.h"

#define MAX_RECTS_NUM 6 // 允许的最大目标框个数
typedef void *FHIMGPROC_AOV_MD_HANDLE;

typedef struct
{
	unsigned short frameW; // 输入原图幅面宽
	unsigned short frameH; // 输入原图幅面高
	int areaTh; //目标框在MD上的面积阈值,默认值93
} FHIMGPROC_AOV_MD_CONFIG; // [2M，3M]

typedef struct
{
	//FHA_IMAGE_s yuv;  // 输入原图数据，例子为yuv420
	FHIMGPROC_AOV_MD_CONFIG yuv_in;
	FHA_IMAGE_s y_md;   // 输入8倍下采样的MD数据
} FHIMGPROC_AOV_MD_IN;

typedef struct
{
	FH_UINT8 HKeep0;
	FH_UINT8 HDrop0;
	FH_UINT8 HKeep1;
	FH_UINT8 HDrop1;
	FH_UINT8 HKeep2;
	FH_UINT8 HDrop2;
	FH_UINT8 VKeep0;
	FH_UINT8 VDrop0;
	FH_UINT8 VKeep1;
	FH_UINT8 VDrop1;
	FH_UINT8 VKeep2;
	FH_UINT8 VDrop2;
} FHIMGPROC_AOV_MD_FM_CFG; //nna.cfg

typedef struct
{
	FH_UINT8 ExtMode;			// 通道拓展模式，1为开启，0为关闭，默认为1
	FH_UINT8 C0ExtMode;			// 如果拓展模式打开，第一个通道内容是否为复制单y，默认为1
	FH_UINT8 C1ExtMode;			// 第二个通道同上，默认为1
	FH_UINT8 C2ExtMode;         // 第三个通道同上，默认为1
	FH_UINT8 C3ExtMode;         // 第四个通道同上，默认为1
	FH_UINT8 HMode;         // 水平方向模式，0为跳点即下采样，1为复制即上采样
	FH_UINT8 HRate;         // 水平方向上采样倍率，跳点模式时置1，即不做上采样；复制上采样模式时，置1/ratio_h 
	FH_UINT8 VMode;         // 垂直方向模式，0为跳点即下采样，1为复制即上采样
	FH_UINT8 VRate;         // 垂直方向上采样倍率，跳点模式时置1，即不做上采样；复制上采样模式时，置1/ratio_v
} FHIMGPROC_AOV_NNA_FM0_INFO;

typedef struct
{
	FH_UINT16 CutStartX;			  // cut模式，详见寄存器文档，默认0
	FH_UINT16 CutStartY;			  // cut模式，详见寄存器文档，默认0
	FH_UINT16 CutWidth;			  // 默认nn输入宽,遇到边界需要保护
	FH_UINT16 CutHeigh;            // 默认nn输入高,遇到边界需要保护
	FH_UINT16 OrignalWidth;        // 下采样前宽度-1
	FH_UINT16 OrignalHeigh;        // 下采样前高度-1
} FHIMGPROC_AOV_NNA_FM345_INFO;

typedef struct
{
	FH_UINT16 u32X;			 // 目标框左上角x
	FH_UINT16 u32Y;			 // 目标框左上角y
	float ratio;				 // 缩放倍率
	FHIMGPROC_AOV_MD_FM_CFG cfg; // 跳点寄存器配置,默认横纵缩放比一致
	FHIMGPROC_AOV_NNA_FM0_INFO cfg_fm0;
	FHIMGPROC_AOV_NNA_FM345_INFO cfg_fm345;
} FHIMGPROC_AOV_MD_CROP_INFO;

typedef struct
{
	FH_UINT8 rect_num;							// 目标框个数
	FHIMGPROC_AOV_MD_CROP_INFO rect[MAX_RECTS_NUM]; // 目标框在MD图上的坐标等信息
} FHIMGPROC_AOV_MD_RESULT;

typedef struct
{
	unsigned int u32X;//目标框左上角x
	unsigned int u32Y;//目标框左上角y
	unsigned int u32Width;//目标框宽
	unsigned int u32Height;//目标框高
} MOtion_ROI; // 目标框在MD图上的坐标信息

typedef struct
{
	unsigned int base_w;//MD图宽
	unsigned int base_h;//MD图高
	int areaTh;//目标框在MD上的面积阈值
	unsigned int rect_num;//目标框的个数
	MOtion_ROI rect[MAX_RECTS_NUM];
} MOtion_BGM_RUNTB_RECT;//MD图上目标框的信息


/*
*功能： 初始化aov_md
*参数：
FHIMGPROC_AOV_MD_HANDLE *handle[IN] 句柄
FHIMGPROC_AOV_MD_CONFIG *param[IN] 参数
*返回值： 0 成功   非0 失败
*/
int fhimgproc_aov_md_init(FHIMGPROC_AOV_MD_HANDLE *handle, FHIMGPROC_AOV_MD_CONFIG *param);
int fhimgproc_aov_md_get_det_process(FHIMGPROC_AOV_MD_HANDLE *handle, FHIMGPROC_AOV_MD_IN in, MOtion_BGM_RUNTB_RECT *det);

/*
*功能： 运行aov_md模块
*参数：
FHIMGPROC_AOV_MD_HANDLE *handle[IN] 句柄
FHIMGPROC_AOV_MD_IN in[IN] 输入，两个码流
FHIMGPROC_AOV_MD_RESULT *out[OUT] 输出
*返回值： 0 成功   非0 失败
*/
//int fhimgproc_aov_md_process(FHIMGPROC_AOV_MD_HANDLE *handle, FHIMGPROC_AOV_MD_IN in, FHIMGPROC_AOV_MD_RESULT *out);
int fhimgproc_aov_md_get_cropinfo_process(FHIMGPROC_AOV_MD_HANDLE *handle, FHIMGPROC_AOV_MD_IN in, MOtion_BGM_RUNTB_RECT run, FHIMGPROC_AOV_MD_RESULT *out);

/*
*功能： 退出aov_md模块释放空间
*参数：
FHIMGPROC_AOV_MD_HANDLE *handle[IN] 句柄
*返回值： 0 成功   非0 失败
*/
int fhimgproc_aov_md_destory(FHIMGPROC_AOV_MD_HANDLE *handle);

#endif