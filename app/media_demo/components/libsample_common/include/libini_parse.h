#ifndef __INI_PARSE_H__
#define __INI_PARSE_H__
#include "sample_common_media.h"

#define MAX_LEN 64

typedef enum MediaConfigType
{
    NONE = 0,
    MEDIA_ISP_CONFIG,
    MEDIA_VPUCHN_CONFIG,
    MEDIA_ENCCHN_CONFIG,
    MEDIA_MJPEG_CONFIG,
    MEDIA_ENC_CONFIG,
    MEDIA_APP_CONFIG,
    MEDIA_JPEG_CONFIG,
} MediaConfigType;

// 定义键值对结构
typedef struct KeyValue
{
    char key[MAX_LEN];
    char value[MAX_LEN];
    struct KeyValue *next;
} KeyValue;

// 定义节结构
typedef struct Section
{
    char name[MAX_LEN];
    MediaConfigType type;
    int grp_id;
    int chn_id;
    KeyValue *keyValueList;
    struct Section *next;
} Section;

// 解析后的 INI 文件结构
typedef struct IniFile
{
    Section *sections;
} IniFile;

typedef struct ini_isp_config
{
    int effect;
    int enable;
    int sensor_i2c;
    int sensor_reset_gpio;
    int sensor_format;
    int max_width;
    int max_height;
    int width;
    int height;
    char sensor_name[32];
} INI_ISP_CFG;

typedef struct ini_vpuchn_config
{
    int effect;
    int enable;
    int lpbuf_enable;
    int max_width;
    int max_height;
    int width;
    int height;
    int vo_mode;
    int buf_num;
} INI_VPUCHN_CFG;

typedef struct ini_encchn_config
{
    int effect;
    int enable;
    int enc_type;
    int rc_type;
} INI_ENCCHN_CFG;

typedef struct ini_jpeg_config
{
    int effect;
    int enable;
    int fps;
} INI_JPEG_CFG;

typedef struct ini_enc_config
{
    int effect;
    unsigned int common_buffer_size_h265_h264;
    unsigned int common_buffer_size_jpeg;
    unsigned int common_buffer_size_mjpeg;
} INI_ENC_CFG;

typedef struct ini_app_config
{
    int effect;
    int nn_enable;
    int nn_fps;
    int osd_gbox_enable;
    int osd_gbox_twobuf;
    int osd_gbox_bit;
    int osd_tosd_enable;
    int osd_tosd_twobuf;
} INI_APP_CFG;

typedef struct ini_media_config
{
    INI_ISP_CFG ini_isp_cfg[MAX_GRP_NUM];
    INI_VPUCHN_CFG ini_vpuchn_cfg[MAX_GRP_NUM][MAX_VPU_CHN_NUM];
    INI_ENCCHN_CFG ini_encchn_cfg[MAX_GRP_NUM][MAX_VPU_CHN_NUM];
    INI_ENCCHN_CFG ini_mjpeg_cfg[MAX_GRP_NUM][MAX_VPU_CHN_NUM];
    INI_JPEG_CFG ini_jpeg_cfg;
    INI_ENC_CFG ini_enc_cfg;
    INI_APP_CFG ini_app_cfg;
} INI_MEDIA_CFG;

INI_ISP_CFG *get_ini_isp_cfg(int grp_id);

INI_VPUCHN_CFG *get_ini_vpuchn_cfg(int grp_id, int chn_id);

INI_ENCCHN_CFG *get_ini_encchn_cfg(int grp_id, int chn_id);

INI_ENCCHN_CFG *get_ini_mjpeg_cfg(int grp_id, int chn_id);

INI_JPEG_CFG *get_ini_jpeg_cfg(void);

INI_ENC_CFG *get_ini_enc_cfg(void);

INI_APP_CFG *get_ini_app_cfg(void);

int load_ini_file(const char *filename);

#endif //__INI_PARSE_H__
