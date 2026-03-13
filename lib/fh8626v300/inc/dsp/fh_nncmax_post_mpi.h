#ifndef __FH_CMAX_POST_MPI_H__
#define __FH_CMAX_POST_MPI_H__

#include "fh_nnpost_common_mpi.h"



/**
 * 后处理宏定义 should add _ALGDEF
 */

/*
 * 本后处理库可以支持的模型ID, should add _ALGST
*/
enum {
	FN_TYPE_CMAX = 20,    //CMAX应用 | [1234]
};
/**
 * 后处理相关结构体定义, should add _ALGST
 */
typedef struct
{   FH_UINT64 frame_id;                     //矩形信息对应的帧序 | [ ]
	FH_UINT64 time_stamp;                   //矩形信息对应帧的时间戳 | [ ]
	FH_FLOAT max_value;                     //cmax最高分数，[0,1]
}FH_EXT_CMAX_OUT_ALGST;


/*
*   Name: FH_NNPost_CMAX_Create
*            CMAX 后处理初始化函数
*
*   Parameters:
*
*       [in] FH_VOID *in
*            添加说明
*
*       [out] FH_VOID *out
*            添加说明
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          添加说明
*
*/

FH_SINT32 FH_NNPost_CMAX_Create(FH_VOID** postHandle, NnPostInitParam* post_init_param);
/*
*   Name: FH_NNPost_CMAX_Destroy
*            CMAX 后处理销毁函数
*
*   Parameters:
*
*       [in] FH_VOID *in
*            添加说明
*
*       [out] FH_VOID *out
*            添加说明
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          添加说明
*
*/
FH_SINT32 FH_NNPost_CMAX_Destroy(FH_VOID* postHandle);

/*
*   Name: FH_NNPost_CMAX_Process
*            CMAX 后处理处理函数
*
*   Parameters:
*
*       [in] FH_VOID *in
*            添加说明
*
*       [out] FH_VOID *out
*            添加说明
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          添加说明
*
*/
FH_SINT32 FH_NNPost_CMAX_Process(FH_VOID* postHandle, FH_NN_POST_INPUT_INFO* input_info, FH_EXT_CMAX_OUT_ALGST* out);


/*
*   Name: FH_NNPost_CMAX_Process
*            获取后处理库版本信息
*
*   Parameters:
*
*       [in] FH_UINT32 print_enable
*            是否打印版本信息
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*
*/
FH_CHAR *FH_NNPost_CMAX_Version(FH_UINT32 print_enable);
#endif