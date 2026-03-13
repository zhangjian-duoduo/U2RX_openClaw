#include "libsample_common/include/libini_parse.h"

static char *model_name = MODEL_TAG_INI_PARSE;

static INI_MEDIA_CFG g_ini_media_cfg;

// 创建一个新的节
static Section *create_section(const char *name)
{
    Section *section = (Section *)malloc(sizeof(Section));
    if (section)
    {
        strncpy(section->name, name, MAX_LEN);
        section->type = NONE;
        section->grp_id = -1;
        section->chn_id = -1;
        section->keyValueList = NULL;
        section->next = NULL;
    }
    return section;
}

// 创建一个新的键值对
static KeyValue *create_key_value(const char *key, const char *value)
{
    KeyValue *keyValue = (KeyValue *)malloc(sizeof(KeyValue));
    if (keyValue)
    {
        strncpy(keyValue->key, key, MAX_LEN);
        strncpy(keyValue->value, value, MAX_LEN);
        keyValue->next = NULL;
    }
    return keyValue;
}

static void trim_whitespace(char *str)
{
    char *start = str;
    char *end = NULL;

    // 跳过前导空格
    while (*start == ' ' || *start == '\t')
    {
        start++;
    }

    // 如果字符串中只包含空白字符
    if (*start == 0)
    {
        *str = '\0'; // 清空字符串
        return;
    }

    // 查找尾部空白字符
    end = start + strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n'))
    {
        end--;
    }

    // 保证目标区域不会越界
    if (start != str)
    {
        memmove(str, start, end - start + 1);
    }

    // 确保字符串正确终止
    str[end - start + 1] = '\0';
}

static void print_error(const char *filename, int line_number, const char *message)
{
    fprintf(stderr, "Error in file '%s', line %d: %s\n", filename, line_number, message);
}

// 释放内存
static void free_ini_file(IniFile *iniFile)
{
    if (!iniFile)
        return;

    Section *section = iniFile->sections;
    while (section)
    {
        KeyValue *keyValue = section->keyValueList;
        while (keyValue)
        {
            KeyValue *temp = keyValue;
            keyValue = keyValue->next;
            free(temp);
        }
        Section *temp = section;
        section = section->next;
        free(temp);
    }
    free(iniFile);
}

// 解析INI文件
static IniFile *parse_ini_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        SDK_PRT(model_name, "Ini FIle[%s] not exist\n", filename);
        return NULL;
    }

    IniFile *iniFile = (IniFile *)malloc(sizeof(IniFile));
    if (!iniFile)
    {
        fclose(file);
        return NULL;
    }

    iniFile->sections = NULL;
    Section *currentSection = NULL;
    int line_number = 0;

    char line[512];
    while (fgets(line, sizeof(line), file))
    {
        line_number++;
        // 清除行尾的换行符
        line[strcspn(line, "\r\n")] = '\0';
        trim_whitespace(line);

        if (line[0] == '\0' || line[0] == ';' || line[0] == '#')
        {
            // 跳过空行和注释
            continue;
        }

        if (line[0] == '[')
        {
            // 处理节（section）
            char *sectionName = line + 1;
            char *end = strchr(sectionName, ']');
            if (end)
            {
                *end = '\0';
                if (strlen(sectionName) == 0 || strlen(sectionName) > MAX_LEN)
                {
                    print_error(filename, line_number, "Section name length error");
                    continue; // 节名为空时报错并跳过该行
                }
                currentSection = create_section(sectionName);
                if (!currentSection)
                {
                    print_error(filename, line_number, "create_section failed!");
                    free_ini_file(iniFile);
                    fclose(file);
                    return NULL;
                }
                currentSection->next = iniFile->sections;
                iniFile->sections = currentSection;
            }
            else
            {
                print_error(filename, line_number, "Missing closing ']' in section");
                continue; // 如果没有找到 ']'，跳过该行
            }
        }
        else if (currentSection)
        {
            // 处理键值对（key=value）
            char *delimiter = strchr(line, '=');
            if (delimiter)
            {
                // 检查是否有多个 '=' 号
                if (strchr(delimiter + 1, '='))
                {
                    print_error(filename, line_number, "Multiple '=' signs in key-value pair");
                    continue; // 如果有多个 '=' 号，报错并跳过该行
                }

                *delimiter = '\0';
                char *key = line;
                char *value = delimiter + 1;

                trim_whitespace(key);
                trim_whitespace(value);

                // 如果键或值为空，输出警告
                if (strlen(key) == 0 || strlen(value) == 0 || strlen(value) > MAX_LEN)
                {
                    print_error(filename, line_number, "Key or value length error");
                    continue; // 跳过格式错误的键值对
                }

                KeyValue *keyValue = create_key_value(key, value);
                keyValue->next = currentSection->keyValueList;
                currentSection->keyValueList = keyValue;
            }
            else
            {
                print_error(filename, line_number, "Missing '=' in key-value pair");
            }
        }
    }

    fclose(file);
    return iniFile;
}

// 打印 INI 文件内容
void print_ini_file(const IniFile *iniFile)
{
    Section *section = iniFile->sections;
    while (section)
    {
        printf("[%s] type[%d] grp_id[%d],chn_id[%d]\n", section->name, section->type, section->grp_id, section->chn_id);
        KeyValue *keyValue = section->keyValueList;
        while (keyValue)
        {
            printf("  %s=%s\n", keyValue->key, keyValue->value);
            keyValue = keyValue->next;
        }
        section = section->next;
    }
}

static int set_isp_config(int grp_id, const Section *section)
{
    int effect = -1;
    int enable = -1;
    int result = -1;
    char *end = NULL;

    SDK_CHECK_MAX_VALID(model_name, grp_id, MAX_GRP_NUM - 1, "grp_id[%d] > MAX_GRP_NUM[%d]\n", grp_id, MAX_GRP_NUM - 1);

    KeyValue *keyValue = section->keyValueList;
    while (keyValue)
    {
        if (!strcmp("effect", keyValue->key) && (effect == -1))
        {
            effect = strtol(keyValue->value, NULL, 0);
            if (effect == 1)
            {
                effect = 1;
                g_ini_media_cfg.ini_isp_cfg[grp_id].effect = effect;
                keyValue = section->keyValueList;
            }
            else
            {
                effect = 0;
                g_ini_media_cfg.ini_isp_cfg[grp_id].effect = effect;
                return FH_SDK_SUCCESS;
            }
        }
        else if (!strcmp("enable", keyValue->key) && (effect == 1) && (enable == -1))
        {
            enable = strtol(keyValue->value, NULL, 0);
            if (enable == 1)
            {
                enable = 1;
                g_ini_media_cfg.ini_isp_cfg[grp_id].enable = enable;
                keyValue = section->keyValueList;
            }
            else
            {
                enable = 0;
                g_ini_media_cfg.ini_isp_cfg[grp_id].enable = enable;
                return FH_SDK_SUCCESS;
            }
        }
        else if (effect == 1 && enable == 1)
        {
            if (!strcmp("sensor_i2c", keyValue->key))
            {
                result = strtol(keyValue->value, &end, 0);
                if (keyValue->value == end)
                {
                    SDK_ERR_PRT(model_name, "Parse ISP[%d] config[%s] Error Value[%d]\n", grp_id, keyValue->key, result);
                }
                else
                {
                    g_ini_media_cfg.ini_isp_cfg[grp_id].sensor_i2c = result;
                    SDK_PRT(model_name, "Parse ISP[%d] Config[%s] Value[%d]\n", grp_id, keyValue->key, result);
                }
            }
            else if (!strcmp("sensor_reset_gpio", keyValue->key))
            {
                result = strtol(keyValue->value, &end, 0);
                if (keyValue->value == end)
                {
                    SDK_ERR_PRT(model_name, "Parse ISP[%d] config[%s] Error Value[%d]\n", grp_id, keyValue->key, result);
                }
                else
                {
                    g_ini_media_cfg.ini_isp_cfg[grp_id].sensor_reset_gpio = result;
                    SDK_PRT(model_name, "Parse ISP[%d] Config[%s] Value[%d]\n", grp_id, keyValue->key, result);
                }
            }
            else if (!strcmp("sensor_format", keyValue->key))
            {
                result = strtol(keyValue->value, &end, 16);
                if (keyValue->value == end)
                {
                    SDK_ERR_PRT(model_name, "Parse ISP[%d] config[%s] Error Value[%d]\n", grp_id, keyValue->key, result);
                }
                else
                {
                    g_ini_media_cfg.ini_isp_cfg[grp_id].sensor_format = result;
                    SDK_PRT(model_name, "Parse ISP[%d] Config[%s] Value[0x%x]\n", grp_id, keyValue->key, result);
                }
            }
            else if (!strcmp("max_width", keyValue->key))
            {
                result = strtol(keyValue->value, &end, 0);
                if (keyValue->value == end)
                {
                    SDK_ERR_PRT(model_name, "Parse ISP[%d] config[%s] Error Value[%d]\n", grp_id, keyValue->key, result);
                }
                else
                {
                    g_ini_media_cfg.ini_isp_cfg[grp_id].max_width = result;
                    SDK_PRT(model_name, "Parse ISP[%d] Config[%s] Value[%d]\n", grp_id, keyValue->key, result);
                }
            }
            else if (!strcmp("max_height", keyValue->key))
            {
                result = strtol(keyValue->value, &end, 0);
                if (keyValue->value == end)
                {
                    SDK_ERR_PRT(model_name, "Parse ISP[%d] config[%s] Error Value[%d]\n", grp_id, keyValue->key, result);
                }
                else
                {
                    g_ini_media_cfg.ini_isp_cfg[grp_id].max_height = result;
                    SDK_PRT(model_name, "Parse ISP[%d] Config[%s] Value[%d]\n", grp_id, keyValue->key, result);
                }
            }
            else if (!strcmp("width", keyValue->key))
            {
                result = strtol(keyValue->value, &end, 0);
                if (keyValue->value == end)
                {
                    SDK_ERR_PRT(model_name, "Parse ISP[%d] config[%s] Error Value[%d]\n", grp_id, keyValue->key, result);
                }
                else
                {
                    g_ini_media_cfg.ini_isp_cfg[grp_id].width = result;
                    SDK_PRT(model_name, "Parse ISP[%d] Config[%s] Value[%d]\n", grp_id, keyValue->key, result);
                }
            }
            else if (!strcmp("height", keyValue->key))
            {
                result = strtol(keyValue->value, &end, 0);
                if (keyValue->value == end)
                {
                    SDK_ERR_PRT(model_name, "Parse ISP[%d] config[%s] Error Value[%d]\n", grp_id, keyValue->key, result);
                }
                else
                {
                    g_ini_media_cfg.ini_isp_cfg[grp_id].height = result;
                    SDK_PRT(model_name, "Parse ISP[%d] Config[%s] Value[%d]\n", grp_id, keyValue->key, result);
                }
            }
            else if (!strcmp("sensor_name", keyValue->key))
            {
                memcpy(g_ini_media_cfg.ini_isp_cfg[grp_id].sensor_name, keyValue->value, sizeof(g_ini_media_cfg.ini_isp_cfg[grp_id].sensor_name));
                SDK_PRT(model_name, "Parse ISP[%d] Config[%s] Value[%s]\n", grp_id, keyValue->key, g_ini_media_cfg.ini_isp_cfg[grp_id].sensor_name);
            }
            keyValue = keyValue->next;
        }
        else
        {
            keyValue = keyValue->next;
        }
    }

    return FH_SDK_SUCCESS;
}

static int set_vpuchn_config(int grp_id, int chn_id, const Section *section)
{
    int effect = -1;
    int enable = -1;
    int result = -1;
    char *end = NULL;

    KeyValue *keyValue = section->keyValueList;
    while (keyValue)
    {
        if (!strcmp("effect", keyValue->key) && (effect == -1))
        {
            effect = strtol(keyValue->value, NULL, 0);
            if (effect == 1)
            {
                effect = 1;
                g_ini_media_cfg.ini_vpuchn_cfg[grp_id][chn_id].effect = effect;
                keyValue = section->keyValueList;
            }
            else
            {
                effect = 0;
                g_ini_media_cfg.ini_vpuchn_cfg[grp_id][chn_id].effect = effect;
                return FH_SDK_SUCCESS;
            }
        }
        else if (!strcmp("enable", keyValue->key) && (effect == 1) && (enable == -1))
        {
            enable = strtol(keyValue->value, NULL, 0);
            if (enable == 1)
            {
                enable = 1;
                g_ini_media_cfg.ini_vpuchn_cfg[grp_id][chn_id].enable = enable;
                keyValue = section->keyValueList;
            }
            else
            {
                enable = 0;
                g_ini_media_cfg.ini_vpuchn_cfg[grp_id][chn_id].enable = enable;
                return FH_SDK_SUCCESS;
            }
        }
        else if (effect == 1 && enable == 1)
        {
            if (!strcmp("lpbuf_enable", keyValue->key))
            {
                result = strtol(keyValue->value, NULL, 0);
                if (result == 1)
                {
                    result = 1;
                    g_ini_media_cfg.ini_vpuchn_cfg[grp_id][chn_id].lpbuf_enable = result;
                    SDK_PRT(model_name, "Parse VPU[%d] CHN[%d] Config[%s] Value[%d]\n", grp_id, chn_id, keyValue->key, result);
                }
                else
                {
                    result = 0;
                    g_ini_media_cfg.ini_vpuchn_cfg[grp_id][chn_id].lpbuf_enable = result;
                    SDK_PRT(model_name, "Parse VPU[%d] CHN[%d] Config[%s] Value[%d]\n", grp_id, chn_id, keyValue->key, result);
                }
            }
            else if (!strcmp("max_width", keyValue->key))
            {
                result = strtol(keyValue->value, &end, 0);
                if (keyValue->value == end)
                {
                    SDK_ERR_PRT(model_name, "Parse VPU[%d] CHN[%d] config[%s] Error Value[%d]\n", grp_id, chn_id, keyValue->key, result);
                }
                else
                {
                    g_ini_media_cfg.ini_vpuchn_cfg[grp_id][chn_id].max_width = result;
                    SDK_PRT(model_name, "Parse VPU[%d] CHN[%d] Config[%s] Value[%d]\n", grp_id, chn_id, keyValue->key, result);
                }
            }
            else if (!strcmp("max_height", keyValue->key))
            {
                result = strtol(keyValue->value, &end, 0);
                if (keyValue->value == end)
                {
                    SDK_ERR_PRT(model_name, "Parse VPU[%d] CHN[%d] config[%s] Error Value[%d]\n", grp_id, chn_id, keyValue->key, result);
                }
                else
                {
                    g_ini_media_cfg.ini_vpuchn_cfg[grp_id][chn_id].max_height = result;
                    SDK_PRT(model_name, "Parse VPU[%d] CHN[%d] Config[%s] Value[%d]\n", grp_id, chn_id, keyValue->key, result);
                }
            }
            else if (!strcmp("width", keyValue->key))
            {
                result = strtol(keyValue->value, &end, 0);
                if (keyValue->value == end)
                {
                    SDK_ERR_PRT(model_name, "Parse VPU[%d] CHN[%d] config[%s] Error Value[%d]\n", grp_id, chn_id, keyValue->key, result);
                }
                else
                {
                    g_ini_media_cfg.ini_vpuchn_cfg[grp_id][chn_id].width = result;
                    SDK_PRT(model_name, "Parse VPU[%d] CHN[%d] Config[%s] Value[%d]\n", grp_id, chn_id, keyValue->key, result);
                }
            }
            else if (!strcmp("height", keyValue->key))
            {
                result = strtol(keyValue->value, &end, 0);
                if (keyValue->value == end)
                {
                    SDK_ERR_PRT(model_name, "Parse VPU[%d] CHN[%d] config[%s] Error Value[%d]\n", grp_id, chn_id, keyValue->key, result);
                }
                else
                {
                    g_ini_media_cfg.ini_vpuchn_cfg[grp_id][chn_id].height = result;
                    SDK_PRT(model_name, "Parse VPU[%d] CHN[%d] Config[%s] Value[%d]\n", grp_id, chn_id, keyValue->key, result);
                }
            }
            else if (!strcmp("vo_mode", keyValue->key))
            {
                result = strtol(keyValue->value, &end, 0);
                if (keyValue->value == end)
                {
                    SDK_ERR_PRT(model_name, "Parse VPU[%d] CHN[%d] config[%s] Error Value[%d]\n", grp_id, chn_id, keyValue->key, result);
                }
                else
                {
                    g_ini_media_cfg.ini_vpuchn_cfg[grp_id][chn_id].vo_mode = result;
                    SDK_PRT(model_name, "Parse VPU[%d] CHN[%d] Config[%s] Value[%d]\n", grp_id, chn_id, keyValue->key, result);
                }
            }
            else if (!strcmp("buf_num", keyValue->key))
            {
                result = strtol(keyValue->value, &end, 0);
                if (keyValue->value == end)
                {
                    SDK_ERR_PRT(model_name, "Parse VPU[%d] CHN[%d] config[%s] Error Value[%d]\n", grp_id, chn_id, keyValue->key, result);
                }
                else
                {
                    g_ini_media_cfg.ini_vpuchn_cfg[grp_id][chn_id].buf_num = result;
                    SDK_PRT(model_name, "Parse VPU[%d] CHN[%d] Config[%s] Value[%d]\n", grp_id, chn_id, keyValue->key, result);
                }
            }
            keyValue = keyValue->next;
        }
        else
        {
            keyValue = keyValue->next;
        }
    }

    return FH_SDK_SUCCESS;
}

static int set_encchn_config(int grp_id, int chn_id, const Section *section)
{
    int effect = -1;
    int enable = -1;
    int result = -1;
    char *end = NULL;

    KeyValue *keyValue = section->keyValueList;
    while (keyValue)
    {
        if (!strcmp("effect", keyValue->key) && (effect == -1))
        {
            effect = strtol(keyValue->value, NULL, 0);
            if (effect == 1)
            {
                effect = 1;
                g_ini_media_cfg.ini_encchn_cfg[grp_id][chn_id].effect = effect;
                keyValue = section->keyValueList;
            }
            else
            {
                effect = 0;
                g_ini_media_cfg.ini_encchn_cfg[grp_id][chn_id].effect = effect;
                return FH_SDK_SUCCESS;
            }
        }
        else if (!strcmp("enable", keyValue->key) && (effect == 1) && (enable == -1))
        {
            enable = strtol(keyValue->value, NULL, 0);
            if (enable == 1)
            {
                enable = 1;
                g_ini_media_cfg.ini_encchn_cfg[grp_id][chn_id].enable = enable;
                keyValue = section->keyValueList;
            }
            else
            {
                enable = 0;
                g_ini_media_cfg.ini_encchn_cfg[grp_id][chn_id].enable = enable;
                return FH_SDK_SUCCESS;
            }
        }
        else if (effect == 1 && enable == 1)
        {
            if (!strcmp("enc_type", keyValue->key))
            {
                result = strtol(keyValue->value, &end, 0);
                if (keyValue->value == end)
                {
                    SDK_ERR_PRT(model_name, "Parse ENC[%d] CHN[%d] config[%s] Error Value[%d]\n", grp_id, chn_id, keyValue->key, result);
                }
                else
                {
                    g_ini_media_cfg.ini_encchn_cfg[grp_id][chn_id].enc_type = result;
                    SDK_PRT(model_name, "Parse ENC[%d] CHN[%d] Config[%s] Value[%d]\n", grp_id, chn_id, keyValue->key, result);
                }
            }
            else if (!strcmp("rc_type", keyValue->key))
            {
                result = strtol(keyValue->value, &end, 0);
                if (keyValue->value == end)
                {
                    SDK_ERR_PRT(model_name, "Parse ENC[%d] CHN[%d] config[%s] Error Value[%d]\n", grp_id, chn_id, keyValue->key, result);
                }
                else
                {
                    g_ini_media_cfg.ini_encchn_cfg[grp_id][chn_id].rc_type = result;
                    SDK_PRT(model_name, "Parse ENC[%d] CHN[%d] Config[%s] Value[%d]\n", grp_id, chn_id, keyValue->key, result);
                }
            }
            keyValue = keyValue->next;
        }
        else
        {
            keyValue = keyValue->next;
        }
    }

    return FH_SDK_SUCCESS;
}

static int set_mjpeg_config(int grp_id, int chn_id, const Section *section)
{
    int effect = -1;
    int enable = -1;
    int result = -1;
    char *end = NULL;

    KeyValue *keyValue = section->keyValueList;
    while (keyValue)
    {
        if (!strcmp("effect", keyValue->key) && (effect == -1))
        {
            effect = strtol(keyValue->value, NULL, 0);
            if (effect == 1)
            {
                effect = 1;
                g_ini_media_cfg.ini_mjpeg_cfg[grp_id][chn_id].effect = effect;
                keyValue = section->keyValueList;
            }
            else
            {
                effect = 0;
                g_ini_media_cfg.ini_mjpeg_cfg[grp_id][chn_id].effect = effect;
                return FH_SDK_SUCCESS;
            }
        }
        else if (!strcmp("enable", keyValue->key) && (effect == 1) && (enable == -1))
        {
            enable = strtol(keyValue->value, NULL, 0);
            if (enable == 1)
            {
                enable = 1;
                g_ini_media_cfg.ini_mjpeg_cfg[grp_id][chn_id].enable = enable;
                keyValue = section->keyValueList;
            }
            else
            {
                enable = 0;
                g_ini_media_cfg.ini_mjpeg_cfg[grp_id][chn_id].enable = enable;
                return FH_SDK_SUCCESS;
            }
        }
        else if (effect == 1 && enable == 1)
        {
            if (!strcmp("rc_type", keyValue->key))
            {
                result = strtol(keyValue->value, &end, 0);
                if (keyValue->value == end)
                {
                    SDK_ERR_PRT(model_name, "Parse MJPEG[%d] CHN[%d] config[%s] Error Value[%d]\n", grp_id, chn_id, keyValue->key, result);
                }
                else
                {
                    g_ini_media_cfg.ini_mjpeg_cfg[grp_id][chn_id].rc_type = result;
                    SDK_PRT(model_name, "Parse MJPEG[%d] CHN[%d] Config[%s] Value[%d]\n", grp_id, chn_id, keyValue->key, result);
                }
            }
            keyValue = keyValue->next;
        }
        else
        {
            keyValue = keyValue->next;
        }
    }

    return FH_SDK_SUCCESS;
}

static int set_jpeg_config(const Section *section)
{
    int effect = -1;
    int enable = -1;
    int result = -1;
    char *end = NULL;

    KeyValue *keyValue = section->keyValueList;
    while (keyValue)
    {
        if (!strcmp("effect", keyValue->key) && (effect == -1))
        {
            effect = strtol(keyValue->value, NULL, 0);
            if (effect == 1)
            {
                effect = 1;
                g_ini_media_cfg.ini_jpeg_cfg.effect = effect;
                keyValue = section->keyValueList;
            }
            else
            {
                effect = 0;
                g_ini_media_cfg.ini_jpeg_cfg.effect = effect;
                return FH_SDK_SUCCESS;
            }
        }
        else if (!strcmp("enable", keyValue->key) && (effect == 1) && (enable == -1))
        {
            enable = strtol(keyValue->value, NULL, 0);
            if (enable == 1)
            {
                enable = 1;
                g_ini_media_cfg.ini_jpeg_cfg.enable = enable;
                keyValue = section->keyValueList;
            }
            else
            {
                enable = 0;
                g_ini_media_cfg.ini_jpeg_cfg.enable = enable;
                return FH_SDK_SUCCESS;
            }
        }
        else if (effect == 1 && enable == 1)
        {
            if (!strcmp("fps", keyValue->key))
            {
                result = strtol(keyValue->value, &end, 0);
                if (keyValue->value == end)
                {
                    SDK_ERR_PRT(model_name, "Parse JPEG config[%s] Error Value[%d]\n", keyValue->key, result);
                }
                else
                {
                    g_ini_media_cfg.ini_jpeg_cfg.fps = result;
                    SDK_PRT(model_name, "Parse JPEG Config[%s] Value[%d]\n", keyValue->key, result);
                }
            }
            keyValue = keyValue->next;
        }
        else
        {
            keyValue = keyValue->next;
        }
    }

    return FH_SDK_SUCCESS;
}

static int set_enc_config(const Section *section)
{
    int effect = -1;
    int result = -1;
    char *end = NULL;

    KeyValue *keyValue = section->keyValueList;
    while (keyValue)
    {
        if (!strcmp("effect", keyValue->key) && (effect == -1))
        {
            effect = strtol(keyValue->value, NULL, 0);
            if (effect == 1)
            {
                effect = 1;
                g_ini_media_cfg.ini_enc_cfg.effect = effect;
                keyValue = section->keyValueList;
            }
            else
            {
                effect = 0;
                g_ini_media_cfg.ini_enc_cfg.effect = effect;
                return FH_SDK_SUCCESS;
            }
        }
        else if (effect == 1)
        {
            if (!strcmp("common_buffer_size_h265_h264", keyValue->key))
            {
                result = strtol(keyValue->value, &end, 0);
                if (keyValue->value == end)
                {
                    SDK_ERR_PRT(model_name, "Parse ENC config[%s] Error Value[%d]\n", keyValue->key, result);
                }
                else
                {
                    g_ini_media_cfg.ini_enc_cfg.common_buffer_size_h265_h264 = result;
                    SDK_PRT(model_name, "Parse ENC Config[%s] Value[%d]KB\n", keyValue->key, result / 1024);
                }
            }
            else if (!strcmp("common_buffer_size_jpeg", keyValue->key))
            {
                result = strtol(keyValue->value, &end, 0);
                if (keyValue->value == end)
                {
                    SDK_ERR_PRT(model_name, "Parse ENC config[%s] Error Value[%d]\n", keyValue->key, result);
                }
                else
                {
                    g_ini_media_cfg.ini_enc_cfg.common_buffer_size_jpeg = result;
                    SDK_PRT(model_name, "Parse ENC Config[%s] Value[%d]KB\n", keyValue->key, result / 1024);
                }
            }
            else if (!strcmp("common_buffer_size_mjpeg", keyValue->key))
            {
                result = strtol(keyValue->value, &end, 0);
                if (keyValue->value == end)
                {
                    SDK_ERR_PRT(model_name, "Parse ENC config[%s] Error Value[%d]\n", keyValue->key, result);
                }
                else
                {
                    g_ini_media_cfg.ini_enc_cfg.common_buffer_size_mjpeg = result;
                    SDK_PRT(model_name, "Parse ENC Config[%s] Value[%d]KB\n", keyValue->key, result / 1024);
                }
            }
            keyValue = keyValue->next;
        }
        else
        {
            keyValue = keyValue->next;
        }
    }

    return FH_SDK_SUCCESS;
}

static int set_app_config(const Section *section)
{
    int effect = -1;
    int result = -1;
    char *end = NULL;

    KeyValue *keyValue = section->keyValueList;
    while (keyValue)
    {
        if (!strcmp("effect", keyValue->key) && (effect == -1))
        {
            effect = strtol(keyValue->value, NULL, 0);
            if (effect == 1)
            {
                effect = 1;
                g_ini_media_cfg.ini_app_cfg.effect = effect;
                keyValue = section->keyValueList;
            }
            else
            {
                effect = 0;
                g_ini_media_cfg.ini_app_cfg.effect = effect;
                return FH_SDK_SUCCESS;
            }
        }
        else if (effect == 1)
        {
            if (!strcmp("nn_enable", keyValue->key))
            {
                result = strtol(keyValue->value, NULL, 0);
                if (result == 1)
                {
                    result = 1;
                    g_ini_media_cfg.ini_app_cfg.nn_enable = result;
                    SDK_PRT(model_name, "Parse APP Config[%s] Value[%d]\n", keyValue->key, result);
                }
                else
                {
                    result = 0;
                    g_ini_media_cfg.ini_app_cfg.nn_enable = result;
                    SDK_PRT(model_name, "Parse APP Config[%s] Value[%d]\n", keyValue->key, result);
                }
            }
            else if (!strcmp("nn_fps", keyValue->key))
            {
                result = strtol(keyValue->value, &end, 0);
                if (keyValue->value == end)
                {
                    SDK_ERR_PRT(model_name, "Parse APP config[%s] Error Value[%d]\n", keyValue->key, result);
                }
                else
                {
                    g_ini_media_cfg.ini_app_cfg.nn_fps = result;
                    SDK_PRT(model_name, "Parse APP Config[%s] Value[%d]KB\n", keyValue->key, result / 1024);
                }
            }
            else if (!strcmp("osd_gbox_enable", keyValue->key))
            {
                result = strtol(keyValue->value, NULL, 0);
                if (result == 1)
                {
                    result = 1;
                    g_ini_media_cfg.ini_app_cfg.osd_gbox_enable = result;
                    SDK_PRT(model_name, "Parse APP Config[%s] Value[%d]\n", keyValue->key, result);
                }
                else
                {
                    result = 0;
                    g_ini_media_cfg.ini_app_cfg.osd_gbox_enable = result;
                    SDK_PRT(model_name, "Parse APP Config[%s] Value[%d]\n", keyValue->key, result);
                }
            }
            else if (!strcmp("osd_gbox_two_buf", keyValue->key))
            {
                result = strtol(keyValue->value, NULL, 0);
                if (result == 1)
                {
                    result = 1;
                    g_ini_media_cfg.ini_app_cfg.osd_gbox_twobuf = result;
                    SDK_PRT(model_name, "Parse APP Config[%s] Value[%d]\n", keyValue->key, result);
                }
                else
                {
                    result = 0;
                    g_ini_media_cfg.ini_app_cfg.osd_gbox_twobuf = result;
                    SDK_PRT(model_name, "Parse APP Config[%s] Value[%d]\n", keyValue->key, result);
                }
            }
            else if (!strcmp("osd_gbox_bit", keyValue->key))
            {
                result = strtol(keyValue->value, &end, 0);
                if (keyValue->value == end)
                {
                    SDK_ERR_PRT(model_name, "Parse APP config[%s] Error Value[%d]\n", keyValue->key, result);
                }
                else
                {
                    g_ini_media_cfg.ini_app_cfg.osd_gbox_bit = result;
                    SDK_PRT(model_name, "Parse APP Config[%s] Value[%d]\n", keyValue->key, result / 1024);
                }
            }
            else if (!strcmp("osd_tosd_enable", keyValue->key))
            {
                result = strtol(keyValue->value, NULL, 0);
                if (result == 1)
                {
                    result = 1;
                    g_ini_media_cfg.ini_app_cfg.osd_tosd_enable = result;
                    SDK_PRT(model_name, "Parse APP Config[%s] Value[%d]\n", keyValue->key, result);
                }
                else
                {
                    result = 0;
                    g_ini_media_cfg.ini_app_cfg.osd_tosd_enable = result;
                    SDK_PRT(model_name, "Parse APP Config[%s] Value[%d]\n", keyValue->key, result);
                }
            }
            else if (!strcmp("osd_tosd_two_buf", keyValue->key))
            {
                result = strtol(keyValue->value, NULL, 0);
                if (result == 1)
                {
                    result = 1;
                    g_ini_media_cfg.ini_app_cfg.osd_tosd_twobuf = result;
                    SDK_PRT(model_name, "Parse APP Config[%s] Value[%d]\n", keyValue->key, result);
                }
                else
                {
                    result = 0;
                    g_ini_media_cfg.ini_app_cfg.osd_tosd_twobuf = result;
                    SDK_PRT(model_name, "Parse APP Config[%s] Value[%d]\n", keyValue->key, result);
                }
            }
            keyValue = keyValue->next;
        }
        else
        {
            keyValue = keyValue->next;
        }
    }

    return FH_SDK_SUCCESS;
}

static int parseKeyValue(const Section *section)
{
    int ret = FH_SDK_SUCCESS;
    switch (section->type)
    {
    case MEDIA_ISP_CONFIG:
        ret = set_isp_config(section->grp_id, section);
        break;
    case MEDIA_VPUCHN_CONFIG:
        ret = set_vpuchn_config(section->grp_id, section->chn_id, section);
        break;
    case MEDIA_ENCCHN_CONFIG:
        ret = set_encchn_config(section->grp_id, section->chn_id, section);
        break;
    case MEDIA_MJPEG_CONFIG:
        ret = set_mjpeg_config(section->grp_id, section->chn_id, section);
        break;
    case MEDIA_ENC_CONFIG:
        ret = set_enc_config(section);
        break;
    case MEDIA_APP_CONFIG:
        ret = set_app_config(section);
        break;
    case MEDIA_JPEG_CONFIG:
        ret = set_jpeg_config(section);
        break;
    default:
        break;
    }

    return ret;
}

static int parseSection(Section *section)
{
    int grp_id = -1;
    int chn_id = -1;
    char *token = NULL;
    char *endptr = NULL;
    char section_name[64] = {0};

    memset(section_name, 0, sizeof(section_name));
    strncpy(section_name, section->name, sizeof(section_name));
    token = strtok(section_name, "_");
    while (token != NULL)
    {
        if (!strcmp("GRP", token))
        {
            token = strtok(NULL, "_");
            grp_id = (int)strtol(token, &endptr, 10);
            if (*endptr != '\0')
            {
                return -1;
            }
            else
            {
                section->grp_id = grp_id;
                token = strtok(NULL, "_");
                if (!strcmp("ISP", token))
                {
                    section->type = MEDIA_ISP_CONFIG;
                    break;
                }
                else if (!strcmp("VPUCHN", token))
                {
                    section->type = MEDIA_VPUCHN_CONFIG;
                    token = strtok(NULL, "_");
                    chn_id = (int)strtol(token, &endptr, 10);
                    if (*endptr != '\0')
                    {
                        return -1;
                    }
                    else
                    {
                        section->chn_id = chn_id;
                        break;
                    }
                }
                else if (!strcmp("ENC", token))
                {
                    section->type = MEDIA_ENCCHN_CONFIG;
                    token = strtok(NULL, "_");
                    if (!strcmp("VPUCHN", token))
                    {
                        token = strtok(NULL, "_");
                        chn_id = (int)strtol(token, &endptr, 10);
                        if (*endptr != '\0')
                        {
                            return -1;
                        }
                        else
                        {
                            section->chn_id = chn_id;
                            break;
                        }
                    }
                }
                else if (!strcmp("MJPEG", token))
                {
                    section->type = MEDIA_MJPEG_CONFIG;
                    token = strtok(NULL, "_");
                    if (!strcmp("VPUCHN", token))
                    {
                        token = strtok(NULL, "_");
                        chn_id = (int)strtol(token, &endptr, 10);
                        if (*endptr != '\0')
                        {
                            return -1;
                        }
                        else
                        {
                            section->chn_id = chn_id;
                            break;
                        }
                    }
                }
                else
                {
                    return -1;
                }
            }
        }
        else if (!strcmp("ENC", token))
        {
            section->type = MEDIA_ENC_CONFIG;
            break;
        }
        else if (!strcmp("APP", token))
        {
            section->type = MEDIA_APP_CONFIG;
            break;
        }
        else if (!strcmp("JPEG", token))
        {
            section->type = MEDIA_JPEG_CONFIG;
            break;
        }
        else
        {
            section->type = NONE;
            break;
        }
    }

    parseKeyValue(section);

    return 0;
}

static int parse_ini_config(const Section *section)
{
    KeyValue *keyValue = section->keyValueList;
    while (keyValue)
    {
        if (!strcmp("effect", keyValue->key))
        {
            int effect = strtol(keyValue->value, NULL, 0);
            if (effect == 1)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        keyValue = keyValue->next;
    }

    return 0;
}

// 处理 INI 文件内容
static void deal_ini_file(const IniFile *iniFile)
{
    int ret = -1;
    int ini_config_effect = -1;
    Section *section = iniFile->sections;
    while (section)
    {
        if (!strcmp("INI_CONFIG", section->name) && (ini_config_effect == -1))
        {
            ini_config_effect = parse_ini_config(section);
            if (!ini_config_effect)
            {
                SDK_PRT(model_name, "Ini File Not Effect!\n");
                break;
            }
            else
            {
                section = iniFile->sections;
            }
        }
        else if (ini_config_effect == 1)
        {
            ret = parseSection(section);
            if (ret)
            {
                printf("section[%s] error! next!\n", section->name);
            }
            section = section->next;
        }
        else
        {
            section = section->next;
        }
    }
}

INI_ISP_CFG *get_ini_isp_cfg(int grp_id)
{
    if (g_ini_media_cfg.ini_isp_cfg[grp_id].effect)
    {
        return &g_ini_media_cfg.ini_isp_cfg[grp_id];
    }
    else
    {
        return NULL;
    }
}

INI_VPUCHN_CFG *get_ini_vpuchn_cfg(int grp_id, int chn_id)
{
    if (g_ini_media_cfg.ini_vpuchn_cfg[grp_id][chn_id].effect)
    {
        return &g_ini_media_cfg.ini_vpuchn_cfg[grp_id][chn_id];
    }
    else
    {
        return NULL;
    }
}

INI_ENCCHN_CFG *get_ini_encchn_cfg(int grp_id, int chn_id)
{
    if (g_ini_media_cfg.ini_encchn_cfg[grp_id][chn_id].effect)
    {
        return &g_ini_media_cfg.ini_encchn_cfg[grp_id][chn_id];
    }
    else
    {
        return NULL;
    }
}

INI_ENCCHN_CFG *get_ini_mjpeg_cfg(int grp_id, int chn_id)
{
    if (g_ini_media_cfg.ini_mjpeg_cfg[grp_id][chn_id].effect)
    {
        return &g_ini_media_cfg.ini_mjpeg_cfg[grp_id][chn_id];
    }
    else
    {
        return NULL;
    }
}

INI_JPEG_CFG *get_ini_jpeg_cfg(void)
{
    if (g_ini_media_cfg.ini_jpeg_cfg.effect)
    {
        return &g_ini_media_cfg.ini_jpeg_cfg;
    }
    else
    {
        return NULL;
    }
}

INI_ENC_CFG *get_ini_enc_cfg(void)
{
    if (g_ini_media_cfg.ini_enc_cfg.effect)
    {
        return &g_ini_media_cfg.ini_enc_cfg;
    }
    else
    {
        return NULL;
    }
}

INI_APP_CFG *get_ini_app_cfg(void)
{
    if (g_ini_media_cfg.ini_app_cfg.effect)
    {
        return &g_ini_media_cfg.ini_app_cfg;
    }
    else
    {
        return NULL;
    }
}

int load_ini_file(const char *filename)
{
    IniFile *iniFile = parse_ini_file(filename);
    if (iniFile)
    {
        deal_ini_file(iniFile);
        // print_ini_file(iniFile);
        free_ini_file(iniFile);
    }

    return 0;
}
