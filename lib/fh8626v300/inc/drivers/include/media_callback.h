#ifndef __MEDIA_CALLBACK_H__
#define __MEDIA_CALLBACK_H__

/** 媒体驱动注册回调函数(对外接口)
 * 回调接口为应用提供一个更加及时的事件感知节点，以供应用实现一些对实时性要求极高或个性化的定制功能
 * 因为实时性的考虑，用户的回调函数需要和驱动运行在相同的环境下(linux为内核态，RPC模式时运行在ARC固件侧 !!!)
 * 同时相关回调函数可能在中断或核心线程中调用，用户不能在回调函数中执行耗时过长的逻辑，否则会导致媒体流水线异常．
 * 中断回调中不能使用malloc，mutex，sleep等不允许在中断中使用的函数．
 *
 * 在注册针对某个模块的回调函数时，需要保证相关模块已经被加载(等待模块创建了其支持的回调事件后，才能被用户注册)
 * 每回调事件只能被注册一次，如需要由多个应用逻辑相应，可以自己做分发．
 *
 * 因回调接口定制化程度比较高，相关接口在不同芯片平台的兼容性会有差异，使用此回调接口时建议和原厂开发人员充分沟通确认．
 * 一般情况下建议用户基于SDK API接口实现产品功能，非特别必要不要使用此接口．
*/

/*通用接口结构体定义*/
typedef enum
{
	MUCB_OBJ_VPU    = 0,
	MUCB_OBJ_ENC    = 1,
	MUCB_OBJ_JPEG   = 2,
	MUCB_OBJ_NN     = 3,
	MUCB_OBJ_ISP    = 4,
	MUCB_OBJ_VISE   = 5,
	MUCB_OBJ_MAX,
}MEDIA_USR_CB_OBJ;

typedef struct
{
	FH_SINT32(*usr_cb)(FH_UINT32 mucb_obj,FH_UINT32 mucb_event, FH_VOID *cb_param);
}MUCB_INFO;

/*媒体回调函数注册,注销接口*/
FH_SINT32 mucb_register(FH_UINT32 mucb_obj,FH_UINT32 mucb_event,MUCB_INFO *obj);
FH_SINT32 mucb_unregister(FH_UINT32 mucb_obj,FH_UINT32 mucb_event);

#ifndef MEDIA_PROCESS_H_
typedef struct
{
	FH_PHYADDR base;
	FH_VOID *  vbase;
	FH_UINT32  size;
}MEM_INFO;

typedef struct
{
	MEM_INFO  mem;
	FH_UINT8 *remap_base;
}RW_MEM_INFO;

typedef struct
{
	FH_UINT32 width;
	FH_UINT32 height;
}PIC_SIZE;
#endif

/*各模块回调相关接口结构体定义*/
/*VPU*/

/**　注意:
 * VPU的开始和结束为每次vpu硬件的工作过程，在一些特定的工作模式下(比如分块或融合拼接)一帧图像会需要多次VPU工作才能全部完成．
 * 所以应用在这种情况下会捕获到多次处理同一帧的情况，另外在此类情况时同一帧的多块处理之间也可能会穿插其他帧的处理的情况．
 */
typedef enum
{
	MUCB_VPU_START  = 0, // VPU开始工作的回调事件
	MUCB_VPU_END    = 1, // VPU结束工作的回调事件
	MUCB_VPU_LPBLK  = 2, // VPU LP工作时的指针行完成的回调事件
	MUCB_VPU_CHNEND = 3, // VPU通道输出完成的回调事件
	MUCB_VPU_RST    = 4, // VPU复位的回调事件
	MUCB_VPU_MAX,
}MUCB_VPU_EVENT;

typedef struct
{
	FH_UINT32 grpidx;
	FH_UINT64 frame_id;
	FH_UINT64 time_stamp;
}MUCB_PARAM_VPU_START;

typedef struct
{
	FH_UINT32 grpidx;
	FH_UINT64 frame_id;
	FH_UINT64 time_stamp;
}MUCB_PARAM_VPU_END;

typedef struct
{
	FH_UINT32   grpidx;
	FH_UINT32   chan;
	PIC_SIZE    size;
	FH_UINT32   fmt;
	RW_MEM_INFO data;
	FH_UINT32   stride;        // 每个指针的stride
	FH_UINT32   lpbuf_h;
	FH_UINT32   ptr_h;         // 一个指针内的像素行数
	FH_UINT32   frm_start_pos; // 当前帧buf内正在写的指针位置，单位指针
	FH_UINT32   frm_line_wr;   // 当前帧已写出数据位置，单位指针
}MUCB_PARAM_VPU_LPBLK;

typedef struct
{
	FH_UINT32 grpidx;
	FH_UINT32 chan;
	FH_UINT32 cur_depth;
	FH_UINT64 frame_id;
	FH_UINT64 time_stamp;
}MUCB_PARAM_VPU_CHNEND;

typedef struct
{
	union
	{
		MUCB_PARAM_VPU_START param_start;
		MUCB_PARAM_VPU_END   param_end;
		MUCB_PARAM_VPU_LPBLK param_lpblk;
		MUCB_PARAM_VPU_CHNEND param_chnend;
	};
}MUCB_VPU_CB_PARAM;

/*ENC*/
typedef enum
{
	MUCB_ENC_START  = 0,
	MUCB_ENC_END    = 1,
	MUCB_ENC_SSKEY  = 2,
	MUCB_ENC_MAX,
}MUCB_ENC_EVENT;

typedef struct
{
	FH_UINT32 chan;
	FH_UINT64 frame_id;
	FH_UINT64 time_stamp;
}MUCB_PARAM_ENC_START;

typedef struct
{
	FH_UINT32 chan;
	/*独立缓存模式时此为通道的码流占有情况，
		公共缓存模式时此为公共缓冲的占有情况(会包含所有使用公共缓存通道的码流)*/
	FH_UINT32 stm_buf_use;   // 当前码流buf占有大小
	FH_UINT32 stm_buf_total; // 当前码流buf总大小
	FH_UINT32 stm_num_use;   // 当前码流个数
	FH_UINT32 stm_num_total; // 当前码流总数
	FH_UINT64 frame_id;
	FH_UINT64 time_stamp;
}MUCB_PARAM_ENC_END;

typedef struct
{
	// in
	FH_UINT32     chan;
	PIC_SIZE      size;
	FH_UINT32     stm_type; // 0: avc 1:hevc
	FH_UINT32     frame_type;
	RW_MEM_INFO   brc_info;
	FH_UINT64     frame_id;
	FH_UINT64     time_stamp;
	RW_MEM_INFO   stm_info;

	// out
	FH_UINT32     nalu_size;
}MUCB_PARAM_ENC_SSKEY;

typedef struct
{
	union
	{
		MUCB_PARAM_ENC_START param_start;
		MUCB_PARAM_ENC_END   param_end;
		MUCB_PARAM_ENC_SSKEY param_sskey;
	};
}MUCB_ENC_CB_PARAM;

/*JPEG*/
typedef enum
{
	MUCB_JPEG_START  = 0,
	MUCB_JPEG_END    = 1,
	MUCB_JPEG_MAX,
}MUCB_JPEG_EVENT;

typedef struct
{
	FH_UINT32 chan;
	FH_UINT64 frame_id;
	FH_UINT64 time_stamp;
}MUCB_PARAM_JPEG_START;

typedef struct
{
	FH_UINT32 chan;
	FH_UINT64 frame_id;
	FH_UINT64 time_stamp;
}MUCB_PARAM_JPEG_END;

typedef struct
{
	union
	{
		MUCB_PARAM_JPEG_START param_start;
		MUCB_PARAM_JPEG_END   param_end;
	};
}MUCB_JPEG_CB_PARAM;

/*NN*/
typedef enum
{
	MUCB_NN_START  = 0,
	MUCB_NN_END    = 1,
	MUCB_NN_MAX,
}MUCB_NN_EVENT;

typedef struct
{
	FH_UINT32 chan;
	FH_UINT64 frame_id;
	FH_UINT64 time_stamp;
}MUCB_PARAM_NN_START;

typedef struct
{
	FH_UINT32 chan;
	FH_UINT64 frame_id;
	FH_UINT64 time_stamp;
}MUCB_PARAM_NN_END;

typedef struct
{
	union
	{
		MUCB_PARAM_NN_START param_start;
		MUCB_PARAM_NN_END   param_end;
	};
}MUCB_NN_CB_PARAM;

/**
 * Note: 此文件未定义相关事件的模块为未支持回调事件
*/

/*以下接口为内部模块使用的内部接口，用户不需要调用*/
FH_SINT32 mucb_init(FH_VOID);
FH_SINT32 mucb_uninit(FH_VOID);
/*由各模块注册给mucb模块，声明本模块最大支持的回调事件数量,应该在模块被加载时创建，卸载时销毁*/
FH_SINT32 mucb_create(FH_UINT32 mucb_obj,FH_UINT32 max_event);
FH_SINT32 mucb_destroy(FH_UINT32 mucb_obj);
/*检查当前事件是否被外部注册,用于回调传输参数收集有一定执行代价时做优化，无人注册回调时可以不准备参数*/
FH_BOOL mucb_is_registered(FH_UINT32 mucb_obj,FH_UINT32 mucb_event);
/*mucb回调函数，传递前面结构体约定的参数*/
FH_SINT32 mucb_callback(FH_UINT32 mucb_obj,FH_UINT32 mucb_event,FH_VOID *cb_param);
#endif