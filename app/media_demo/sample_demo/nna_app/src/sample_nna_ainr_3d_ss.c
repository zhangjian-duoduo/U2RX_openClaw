
#include "nna_app/include/sample_common_nna.h"
#include "aiispapi.h"

static FH_UINT32 g_nna_grpid[MAX_GRP_NUM] = {0};
static FH_SINT32 g_nna_running[MAX_GRP_NUM] = {0};
static FH_SINT32 g_nna_stop[MAX_GRP_NUM] = {0};
static FH_SINT32 g_nna_pause[MAX_GRP_NUM] = {0};

// static FH_SINT32 g_switch_ainr_running = 0;
// static FH_SINT32 g_switch_ainr_stop = 0;

int pic_raw = 0;   // 是否记录当前输入帧
int coef_flag = 1; // 是否使用coef, 0 的话表示进模型和进入后续链路的coef都是0
// plan b
FH_CHAR model_path_2d[256] = "./models/net_combine_1222_plB_45ms.bin";
FH_CHAR model_path_3d[256] = "./models/net_combine_3d_1221_4w.bin";
FH_CHAR model_folder2d[64] = "./models/2d";
FH_CHAR model_folder3d[64] = "./models/3d";

int isp_ready_init = 1;
long model_handle = 0;
int is_time_print = 1;
int k = 250;
int b = 128;
int coef_max_3d = 240;
int coef_min_3d = 15;
int coef_max_2d = 240;
int coef_min_2d = 15;


typedef struct
{
    char *path;
    time_t mtime;
} file_info;

// 比较函数，用于排序
int compare_mtime(const void *a, const void *b)
{
    file_info *info_a = (file_info *)a;
    file_info *info_b = (file_info *)b;
    return (info_b->mtime - info_a->mtime);
}

bool list_files(const char *path, char ***file_paths, int *count)
{
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;

    // 打开目录
    dir = opendir(path);
    if (dir == NULL)
    {
        printf("无法打开目录\n");
        return false;
    }
    // 第一次遍历计算文件数量
    *count = 0;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        (*count)++;
    }
    // 分配内存来存储所有文件信息
    file_info *files = malloc(*count * sizeof(file_info));
    if (files == NULL)
    {
        printf("内存分配失败\n");
        closedir(dir);
        return false;
    }
    rewinddir(dir);
    int index = 0;
    // 第二次遍历存储文件路径和修改时间
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        size_t path_len = strlen(path) + strlen(entry->d_name) + 2; // '/' 和 '\0'
        char *full_path = malloc(path_len);
        if (full_path == NULL)
        {
            printf("内存分配失败\n");
            for (int i = 0; i < index; i++)
            {
                free(files[i].path);
            }
            free(files);
            closedir(dir);
            return false;
        }
        snprintf(full_path, path_len, "%s/%s", path, entry->d_name);
        stat(full_path, &file_stat);
        files[index].path = full_path;
        files[index].mtime = file_stat.st_mtime;
        index++;
    }
    closedir(dir);

    // 对文件信息按修改时间排序
    qsort(files, *count, sizeof(file_info), compare_mtime);

    // 分配内存来存储排序后的文件路径
    *file_paths = (char **)malloc(*count * sizeof(char *));
    if (*file_paths == NULL)
    {
        printf("内存分配失败\n");
        for (int i = 0; i < *count; i++)
        {
            free(files[i].path);
        }
        free(files);
        return false;
    }

    // 将排序后的路径复制到输出数组
    for (int i = 0; i < *count; i++)
    {
        (*file_paths)[i] = files[i].path;
    }
    free(files); // 释放临时数组，但不释放路径字符串
    return true;
}
void free_file_paths(char **file_paths, int count)
{
    for (int i = 0; i < count; i++)
    {
        free(file_paths[i]);
    }
    free(file_paths);
}
int save_data_to_file(uint8_t *data, size_t size, const char *filename)
{
    if (data == NULL || filename == NULL)
    {
        return -1; // 参数错误
    }

    FILE *file = fopen(filename, "wb");
    if (file == NULL)
    {
        return -1; // 文件打开失败
    }

    size_t written = fwrite(data, 1, size, file);
    fclose(file);

    if (written != size)
    {
        return -1; // 写入数据不完整
    }

    return 0; // 成功写入
}
unsigned long long getus(void)
{
    unsigned long long usec;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    usec = tv.tv_sec * 1000 * 1000 + tv.tv_usec;
    return usec;
}

FH_VOID *thread_scan(FH_VOID *args)
{
    printf("TY_NPU_SysInit start\n");
    TY_NPU_SysInit();
    TY_SDK_SetLogLevel(3);
    printf("TY_NPU_SysInit success\n");
    FH_SINT32 grp_id = *((FH_SINT32 *)args);
    HW_MODULE_CFG nr3d_cfg;
    nr3d_cfg.moduleCfg = HW_MODUL_NR3D;
    nr3d_cfg.enable = 1;
    API_ISP_Set_Determined_HWmodule(grp_id, &nr3d_cfg);

    HW_MODULE_CFG yuv3d_cfg;
    yuv3d_cfg.moduleCfg = HW_MODUL_YUV3D;
    yuv3d_cfg.enable = 1;
    API_ISP_Set_Determined_HWmodule(grp_id, &yuv3d_cfg);
    printf("enable yuv3d nr3d\n");
    g_nna_running[grp_id] = 1;
    FH_CHAR cmd_s[256];
    char cmd;
    while (1)
    {
        cmd = getchar();
        if (cmd == 'c')
        {
            int temp_time_print = is_time_print;
            is_time_print = 0;
            printf("-> enter k value now %d\n", k);
            scanf("\t%d", &k);
            printf("<- set k value %d\n", k);

            printf("  enter b value now %d\n", b);
            scanf("\t%d", &b);
            printf("<-set b value %ddn", b);

            printf("->enter coef_max value now %d\n", coef_max_3d);
            scanf("\t%d", &coef_max_3d);
            printf("<-set coef_max value %d\n", coef_max_3d);
            printf("->enter coef_min value now %d\n", coef_min_3d);
            scanf("\t%d", &coef_min_3d);
            printf("<-set coef_min value %d\n", coef_min_3d);
            is_time_print = temp_time_print;
        }
        else if (cmd == 'q') // 推出当前循环
        {
            memset(cmd_s, 0, sizeof(cmd_s));
            scanf("%s", cmd_s);
            if (strcmp(cmd_s, "exit") == 0)
            {
                break;
            }
        }
        else if (cmd == 'g') // 开始录制输入内容
        {
            if (pic_raw)
            {
                pic_raw = 0;
                printf("stop save data\n");
            }
            else
            {
                printf("please enter raw/2d/3d/vicap\n");
                memset(cmd_s, 0, sizeof(cmd_s));
                scanf("%s", cmd_s);
                if (strcmp(cmd_s, "raw") == 0)
                    pic_raw = 1;
                else if (strcmp(cmd_s, "2d") == 0)
                    pic_raw = 2;
                else if (strcmp(cmd_s, "3d") == 0)
                    pic_raw = 3;
                else if (strcmp(cmd_s, "vicap") == 0)
                    pic_raw = 4;
                else if (strcmp(cmd_s, "coef") == 0)
                    pic_raw = 5;
            }
        }
        else if (cmd == 'p')
        {
            printf("pic_raw %d\n", pic_raw);
            printf("2d model path %s\n", model_path_2d);
            printf("3d model path %s\n", model_path_3d);
            printf("is nna init: %s\n", isp_ready_init ? "true" : "false");
            printf("model handle init: %s\n", model_handle ? "true" : "false");
            printf("k value %d\n", k);
            printf("b value %d\n", b);
            printf("coef_max_3d value %d\n", coef_max_3d);
            printf("coef_min_3d value %d\n", coef_min_3d);
            is_time_print = 1 - is_time_print;
        }
        else if (cmd == 'm')
        {
            if (model_handle != 0)
            {
                releaseModel(model_handle);
                model_handle = 0;
            }
            else
            {
                char **file_paths_2d = NULL;
                int count_2d = 0;
                if (!list_files(model_folder2d, &file_paths_2d, &count_2d))
                {
                    printf("list 2d model failed\n");
                    free_file_paths(file_paths_2d, count_2d);
                    continue;
                }
                char **file_paths_3d = NULL;
                int count_3d = 0;
                if (!list_files(model_folder3d, &file_paths_3d, &count_3d))
                {
                    printf("list 3d model failed\n");
                    free_file_paths(file_paths_2d, count_2d);
                    continue;
                }
                if (count_2d >= 20 || count_3d >= 20)
                {
                    printf("please delete useless model, too much (2d: %d, 3d: %d)\n", count_2d, count_3d);
                    free_file_paths(file_paths_2d, count_2d);
                    free_file_paths(file_paths_3d, count_3d);
                    continue;
                }
                isp_ready_init = 1;
                if (isp_ready_init)
                {
                    int temp_time_print = is_time_print;
                    is_time_print = 0;
                    int model_index = 0;
                    printf("aiisp init 2d\n");
                    printf("select 2d model path: \n");
                    for (int i = 0; i < count_2d; i++)
                    {
                        printf("\t%d: %s\n", i, file_paths_2d[i]);
                    }
                    printf("->enter 2d model path: ");
                    scanf("%d", &model_index);
                    strcpy(model_path_2d, file_paths_2d[model_index]);
                    printf("\n<- set 2d model path: %s\n", model_path_2d);

                    printf("aiisp init 3d\n");
                    printf("3d model path: \n");
                    for (int i = 0; i < count_3d; i++)
                    {
                        printf("\t%d: %s\n", i, file_paths_3d[i]);
                    }
                    printf("-> enter 3d model path: ");
                    scanf("%d", &model_index);
                    strcpy(model_path_3d, file_paths_3d[model_index]);
                    printf("\n<- set 3d model path: %s\n", model_path_3d);
                    struct AiIspInitParm parm;
                    parm.model_path_2d = model_path_2d;
                    parm.model_path_3d = model_path_3d;
                    parm.h = 1440;
                    parm.w = 2560;
                    parm.num_sensor = 1;
                    model_handle = createModel(parm);
                    if (model_handle == 0)
                    {
                        printf("createModel failed\n");
                        isp_ready_init = 0;
                    }
                    else
                    {
                        printf("createModel success\n");
                    }
                    is_time_print = temp_time_print;
                }
                free_file_paths(file_paths_2d, count_2d);
                free_file_paths(file_paths_3d, count_3d);
            }
        }
    }
    return NULL;
}
FH_VOID *ainr(FH_VOID *args)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;
    unsigned long long time_main_s = getus();
    FH_UINT64 frame_id = 0;
    FH_SINT32 grp_id = *((FH_SINT32 *)args);
    FH_CHAR thread_name[20];
    memset(thread_name, 0, sizeof(thread_name));
    sprintf(thread_name, "nna_ainr_%d", grp_id);
    prctl(PR_SET_NAME, thread_name);
    ISP_INFO *isp_info;
    isp_info = get_isp_config(grp_id);
    if (!isp_info->enable)
    {
        goto Exit;
    }
    ISP_POST_FRAME_INFO_S stFramePost;
    FH_VICAP_STREAM_S vicapStream;
    ISP_SRC_INFO_S_S stPreInfo;
    ISP_MOD_ADDR_S st3dCfg;
    ISP_MOD_ADDR_S stYCfg;
    ISP_MOD_ADDR_S stCCfg;

    uint32_t size_h8l8 = 1440 * 2560;

    st3dCfg.enMod = ISP_MOD_NR3D_COEF;
    API_ISP_GetModAddr(grp_id, &st3dCfg);
    T_TY_Mem output_3dcoef;
    output_3dcoef.size = 1440 * 2560 * 10 / 8 / 16;
    FH_SYS_VmmAlloc_Cached64(&output_3dcoef.phyAddr, (void **)&output_3dcoef.virAddr, "AIISP_3DCOEF", "anonymous", output_3dcoef.size);
    memset((void *)(uintptr_t)output_3dcoef.virAddr, 0, output_3dcoef.size);
    FH_SYS_VmmFlushCache64(output_3dcoef.phyAddr, (void *)(uintptr_t)output_3dcoef.virAddr, output_3dcoef.size);
    st3dCfg.u32Addr = output_3dcoef.phyAddr;
    API_ISP_SetModAddr(grp_id, st3dCfg);

    stYCfg.enMod = ISP_MOD_YC3D_YCOEF;
    API_ISP_GetModAddr(grp_id, &stYCfg);
    T_TY_Mem output_ycoef;
    output_ycoef.size = 2560 * 1440 / 16;
    FH_SYS_VmmAlloc_Cached64(&output_ycoef.phyAddr, (void **)&output_ycoef.virAddr, "AIISP_YCOEF", "anonymous", output_ycoef.size);
    memset((void *)(uintptr_t)output_ycoef.virAddr, 0, output_ycoef.size);
    FH_SYS_VmmFlushCache64(output_ycoef.phyAddr, (void *)(uintptr_t)output_ycoef.virAddr, output_ycoef.size);
    stYCfg.u32Addr = output_ycoef.phyAddr;
    API_ISP_SetModAddr(grp_id, stYCfg);

    stCCfg.enMod = ISP_MOD_YC3D_CCOEF;
    API_ISP_GetModAddr(grp_id, &stCCfg);
    T_TY_Mem output_ccoef;
    output_ccoef.size = 2560 * 1440 / 32;
    FH_SYS_VmmAlloc_Cached64(&output_ccoef.phyAddr, (void **)&output_ccoef.virAddr, "AIISP_CCOEF", "anonymous", output_ccoef.size);
    memset((void *)(uintptr_t)output_ccoef.virAddr, 0, output_ccoef.size);
    FH_SYS_VmmFlushCache64(output_ccoef.phyAddr, (void *)(uintptr_t)output_ccoef.virAddr, output_ccoef.size);
    stCCfg.u32Addr = output_ccoef.phyAddr;
    API_ISP_SetModAddr(grp_id, stCCfg);

    // FHAdv_Isp_SetMirrorAndflip(grp_id, grp_id, 1, 1);
    printf("---------------------------------- start loop ----------------------------------\n");
    struct AiIspInput3D input_3d;
    input_3d.coef3d_output_mem = &output_3dcoef;
    struct AiIspInput2D input_2d;
    while (!g_nna_stop[grp_id])
    {
        if (!g_nna_pause[grp_id])
        {
            vicapStream.u8DevId = 0;
            vicapStream.bLock = FH_TRUE;
            vicapStream.u32TimeOut = 2000;
            // MARK vicap get stream

            ret = FH_VICAP_GetStream(&vicapStream);

            struct Config config = {k, b, coef_max_3d, coef_min_3d, pic_raw, is_time_print};
            if (ret == 0)
            {
                // map vicap data
                void *vicap_data = FH_SYS_MmapCached(vicapStream.stSf.addrPhy, vicapStream.stSf.u32BufSize);
                FH_SYS_VmmFlushCache64(0, vicap_data, vicapStream.stSf.u32BufSize);
                if (model_handle != 0)
                {
                    // attrsense aiisp
                    T_TY_Mem vicap_mem = {vicapStream.stSf.addrPhy, (uint64_t)(uintptr_t)vicap_data, vicapStream.stSf.u32BufSize};
                    input_3d.raw_input_mem = &vicap_mem;
                    input_3d.raw_input_stride = vicapStream.stSf.u32LineStride;
                    input_3d.coef3d_output_mem = &output_3dcoef;
                    // input_3d.ref_3d_mem = &ref_3d;
                    struct AiIspInput input = {
                        .input_type = INPUT_TYPE_3D,
                        .frame_id = frame_id,
                        .sensor_id = 0,
                        .input_3d = input_3d};

                    aiIspTask(model_handle, input, config);

                    FH_SYS_VmmFlushCache64(output_3dcoef.phyAddr, (void *)(uintptr_t)output_3dcoef.virAddr, output_3dcoef.size);
                    if (pic_raw == 1)
                    {
                        char file_name[32];
                        sprintf(file_name, "./data/vicap_raw%lld.bin", frame_id);
                        save_data_to_file((uint8_t *)vicap_data, vicapStream.stSf.u32BufSize, file_name);
                    }
                }
                stPreInfo.u32VicapDevId = vicapStream.u8DevId;
                stPreInfo.u64TimeStamp = vicapStream.stSf.u64TimeStamp;
                stPreInfo.u64FrameId = vicapStream.u64FrmCnt; // 帧号 | [0~0xffffffff]
                stPreInfo.stPic.u16Height = vicapStream.stPic.u16Height;
                stPreInfo.stPic.u16Width = vicapStream.stPic.u16Width;
                stPreInfo.stSfInfo.u32DataBufAddr = vicapStream.stSf.addrPhy;
                stPreInfo.u32PoolId = vicapStream.u32PoolId;
                stPreInfo.stSfInfo.bReady = 1;

                ret = API_ISP_SendIspPreData(grp_id, &stPreInfo);
                if (ret)
                {
                    printf("Error: API_ISP_SendIspPreData failed with %d 0x%x\n", ret, ret);
                }
                ret = FH_VICAP_ReleaseStream(&vicapStream);
                if (ret)
                {
                    printf("Error: FH_VICAP_ReleaseStream failed with %d 0x%x\n", ret, ret);
                }
                FH_SYS_Munmap(vicap_data, vicapStream.stSf.u32BufSize);
                // MARK isp get data
                ret = API_ISP_GetIspPostStream(grp_id, 1 * 1000, &stFramePost);
                if (ret == 0)
                {

                    void *input_data = FH_SYS_MmapCached(stFramePost.stNNPreRaw.u32Base, size_h8l8);
                    FH_SYS_VmmFlushCache64(0, input_data, size_h8l8);
                    void *input_coef_ptr = FH_SYS_MmapCached(stFramePost.stNNPreStd.u32Base, size_h8l8 / 16);
                    FH_SYS_VmmFlushCache64(0, input_coef_ptr, size_h8l8 / 16);
                    if (model_handle != 0)
                    {
                        if (pic_raw == 1)
                        {
                            char file_name[32];
                            sprintf(file_name, "./data/raw_high%lld.bin", frame_id);
                            save_data_to_file((uint8_t *)input_data, size_h8l8, file_name);
                        }
                        T_TY_Mem input_high_mem = {stFramePost.stNNPreRaw.u32Base, (uint64_t)(uintptr_t)input_data, size_h8l8};
                        input_2d.high_input_mem = &input_high_mem;
                        T_TY_Mem input_coef_mem = {stFramePost.stNNPreStd.u32Base, (uint64_t)(uintptr_t)input_coef_ptr, size_h8l8 / 16};
                        input_2d.input_coef_mem = &input_coef_mem;
                        input_2d.high_output_mem = &input_high_mem;
                        input_2d.ouptut_ycoef_mem = &output_ycoef;
                        input_2d.ouptut_ccoef_mem = &output_ccoef;
                        struct AiIspInput input = {
                            .input_type = INPUT_TYPE_2D,
                            .frame_id = frame_id,
                            .sensor_id = 0,
                            .input_2d = input_2d};

                        aiIspTask(model_handle, input, config);

                        FH_SYS_VmmFlushCache64(output_ycoef.phyAddr, (void *)(uintptr_t)output_ycoef.virAddr, output_ycoef.size);
                        FH_SYS_VmmFlushCache64(output_ccoef.phyAddr, (void *)(uintptr_t)output_ccoef.virAddr, output_ccoef.size);
                    }

                    ret = API_ISP_SendIspPostData(grp_id, &stFramePost);
                    SDK_FUNC_ERROR_CONTINUE(model_path_2d, ret);
                    ret = API_ISP_ReleaseIspPostStream(grp_id, &stFramePost);
                    SDK_FUNC_ERROR_CONTINUE(model_path_2d, ret);
                    FH_SYS_Munmap(input_data, size_h8l8);
                    FH_SYS_Munmap(input_coef_ptr, size_h8l8 / 16);
                }
                else
                {
                    printf("API_ISP_GetIspPostStream = %x\n", ret);
                    usleep(1 * 1000);
                }
            }
            else
            {
                usleep(1 * 1000);
            }
            frame_id++;
            if (frame_id % 100 == 0)
            {
                unsigned long long time_main_e = getus();
                if (is_time_print)
                    printf("[ainr%d]FRAME 100 use %lldms,FPS:%f\n", 0, (time_main_e - time_main_s) / 1000, (double)(1000) * 1000 * 100 / (time_main_e - time_main_s));
                time_main_s = time_main_e;
            }
        }
        else
        {
            usleep(10 * 1000);
        }
    }

    SDK_FUNC_ERROR_GOTO(model_path_2d, ret);
    SDK_PRT(model_path_2d, "ainr[%d] End!\n", grp_id);
Exit:
    releaseModel(model_handle);
    g_nna_running[grp_id] = 0;
    TY_NPU_SysExit();
    return NULL;
}

FH_SINT32 sample_fh_ainr_start(FH_SINT32 grp_id)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;
    pthread_t thread;
    pthread_attr_t attr;

    if (g_nna_running[grp_id])
    {
        SDK_PRT(model_path_2d, "ainr[%d] already running!\n", grp_id);
        return FH_SDK_SUCCESS;
    }

    g_nna_grpid[grp_id] = grp_id;
    g_nna_stop[grp_id] = 0;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
#ifdef __RTTHREAD_OS__
    pthread_attr_setstacksize(&attr, 10 * 1024);
#endif
    pthread_t scan_thread_t;
    pthread_attr_t scan_attr;
    pthread_attr_init(&scan_attr);
    pthread_attr_setdetachstate(&scan_attr, PTHREAD_CREATE_DETACHED);
    if (pthread_create(&scan_thread_t, &scan_attr, thread_scan, &g_nna_grpid[grp_id]))
    {
        printf("scan thread pthread_create failed\n");
        goto Exit;
    }
    if (pthread_create(&thread, &attr, ainr, &g_nna_grpid[grp_id]))
    {
        printf("pthread_create failed\n");
        goto Exit;
    }
    return ret;

Exit:
    return -1;
}

FH_SINT32 sample_fh_ainr_stop(FH_SINT32 grp_id)
{
    FH_SINT32 ret = FH_SDK_SUCCESS;

    if (g_nna_running[grp_id])
    {
        g_nna_stop[grp_id] = 1;

        while (g_nna_running[grp_id])
        {
            usleep(10 * 1000);
        }
    }
    else
    {
        SDK_PRT(model_path_2d, "ainr[%d] not running!\n", grp_id);
    }

    return ret;
}
