/*******************************************************************************
* Copyright (c) 2008 - 2019 Shanghai Fullhan Microelectronics Co., Ltd.
* TODO:
******************************************************************************/
#ifndef _FHA_DEFS_H_
#define _FHA_DEFS_H_

#include "fha_type.h"
#include "fha_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FHA_INVALID_PARAM 2
#define FHA_MAX_LANDMARK_PTS_NUM 5
#define FHA_MAX_BBOX_NUM 150
#define FHA_MAX_EMBEDDING_LEN 128
#define FHA_DBG_LEVEL 6

#ifndef MAX
#define FHA_MAX(a, b)                    ((a) > (b) ? (a) : (b))
// Macro returning max value
#endif

#ifndef MIN
#define FHA_MIN(a, b)                    ((a) < (b) ? (a) : (b))
// Macro returning min value
#endif

//#define FHA_MALLOC(size) malloc(size);


#ifndef CLIP
#define FHA_CLIP(val, min, max)	         (((val)<(min)) ? (min):(((val)>(max))? (max):(val)))
#endif
#define FHA_IABS(a)                      ((a)>=0?(a):(-(a)))

typedef enum _FHA_IMAGE_TYPE {
    FH_IMAGE_FORMAT_RGBA = (1 << 1),
    FH_IMAGE_FORMAT_I420 = (1 << 2),
    FH_IMAGE_FORMAT_RGB_PLANE = (1 << 3),
    FH_IMAGE_FORMAT_Y = (1 << 4)
}FHA_IMAGE_TYPE_e;

typedef struct _FHA_IMAGE {
    FH_UINT8         *data;
    FH_UINT32        width;
    FH_UINT32        height;
    FH_UINT32        stride;
    FH_UINT64        timestamp;
    FHA_IMAGE_TYPE_e imageType;
}FHA_IMAGE_s;

typedef struct _FHA_POINT {
    float x;
    float y;
}FHA_POINTF_s;

typedef struct _FHA_BOX {
    float x, y, w, h;
}FHA_BOX_s;



typedef struct _FHA_BBOX {
    FH_UINT16    class_id; //class index
    FHA_BOX_s    bbox;     //bounding box[0, 1] for network inputs
    float        conf;     //confidence
} FHA_BBOX_s;

typedef struct _FHA_DET_BBOX {
    FH_UINT32    bbox_num;  //number
    FHA_BBOX_s   bbox[FHA_MAX_BBOX_NUM];
} FHA_DET_BBOX_s;

typedef struct _FHA_BBOX_EXT {
    FH_UINT16    class_id; //class index
    FHA_BOX_s    bbox;     //bounding box[0, 1] for network inputs
    float        conf;     //confidence
    FHA_POINTF_s landmark[FHA_MAX_LANDMARK_PTS_NUM];
    float        occlude[FHA_MAX_LANDMARK_PTS_NUM];
    float        quality;
}FHA_BBOX_EXT_s;

typedef struct _FHA_DET_BBOX_EXT {
    FH_UINT32       bbox_num;  //number
    FHA_BBOX_EXT_s  bbox[FHA_MAX_BBOX_NUM];
}FHA_DET_BBOX_EXT_s;

typedef struct _FHA_EMBEDDING {
    float       scale;
    FH_SINT32   zero_point;
    FH_UINT8    val[FHA_MAX_EMBEDDING_LEN];
    FH_UINT32   length;
} FHA_EMBEDDING_s;

typedef struct _FHA_FACEPOSE_OUT {
    float yaw;
    float roll;
    float pitch;
} FHA_FACEPOSE_OUT_s;

typedef struct _FHA_TRK_OBJ_EXT_FACEQUALITY {
    FH_UINT16   clsType;  //class index
    FHA_BOX_s	bbox;     //bounding box
    float		conf;     //confidence
    FH_SINT64   id;       //id
    FHA_FACEPOSE_OUT_s pose;
    float		quality;
    int         is_best_face;
    int         face_hold_time;
    FHA_POINTF_s landmark[FHA_MAX_LANDMARK_PTS_NUM];
    float        occlude[FHA_MAX_LANDMARK_PTS_NUM];
} FHA_TRK_OBJ_EXT_FACEQUALITY_s;

typedef struct _FHA_FACEQUALITY_OUT {
    FHA_TRK_OBJ_EXT_FACEQUALITY_s	objs[FHA_MAX_BBOX_NUM];
    FH_UINT32			objNum;  //number
} FHA_FACEQUALITY_OUT_s;



#ifdef __cplusplus
}
#endif

#endif  //_FHA_DEFS_H_

