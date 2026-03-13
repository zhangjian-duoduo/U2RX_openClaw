/**
 * @file feature_config.h
 * @brief Feature configuration header file
 * @note Each new feature needs a macro control, just set to 0 to disable
 */
#ifndef __FEATURE_CONFIG_H__
#define __FEATURE_CONFIG_H__

// ==================== Feature Switches ==================== //

// OSD Cross feature (Red cross in center) - works for NV12 YUV format
#define ENABLE_OSD_CROSS      1

// OSD Cross for H264/H265/MJPEG encoded streams (not implemented yet)
#define ENABLE_OSD_CROSS_ENC   0

// MP4 Record feature
#define ENABLE_MP4_RECORD     0

// SD Card auto mount feature
#define ENABLE_SDCARD_MOUNT   0

// ==================== Feature Config ==================== //

// OSD Cross Config
#if ENABLE_OSD_CROSS
    #ifndef OSD_CROSS_LINE_WIDTH
    #define OSD_CROSS_LINE_WIDTH    2
    #endif
    
    #ifndef OSD_CROSS_LENGTH
    #define OSD_CROSS_LENGTH        50
    #endif
    
    #ifndef OSD_CROSS_RED
    #define OSD_CROSS_RED        255
    #define OSD_CROSS_GREEN      0
    #define OSD_CROSS_BLUE       0
    #define OSD_CROSS_ALPHA      255
    #endif
#endif

#endif // __FEATURE_CONFIG_H__
