#ifndef __AOV_XBUS_COMMAND_H__
#define __AOV_XBUS_COMMAND_H__



// #define AOV_XBUS_COMMAND   0x78414F56 //xAOV
#define RPC_AOV_CMD_3  (3) //aov cmd


typedef enum  {
    AOV_TASK_STOP = 0,
    AOV_TASK_RUN,
}AOV_TASK_STATE;


typedef enum {
    AOV_CMD_QUERY_STATE = 0x41305600,
    AOV_CMD_EXIT_AOV,
    AOV_CMD_START_AOV,
    AOV_CMD_SLEEP,
    AOV_CMD_WAKE_REASON,
} AOV_RTT_CMD_ID;

typedef enum
{
    AOV_WAKEUP_NONE = (1 << 0),
    AOV_WAKEUP_PIR = (1 << 1),
    AOV_WAKEUP_USER = (1 << 2),
    AOV_WAKEUP_NN = (1 << 3),
    AOV_WAKEUP_BUFFFULL = (1 << 4),
    AOV_WAKEUP_AE = (1 << 5),
} AOV_WAKEUP_REASON;

typedef enum
{
    AOV_STREAM_NONE = AOV_WAKEUP_NONE,
    AOV_STREAM_PIR = AOV_WAKEUP_PIR,
    AOV_STREAM_USER = AOV_WAKEUP_USER,
    AOV_STREAM_NN = AOV_WAKEUP_NN,
    AOV_STREAM_AOV = (1 << 4),
    AOV_STREAM_AE = AOV_WAKEUP_AE,
} AOV_STREAM_TYPE;

struct aov_ctrl {
    int aov_enable;
    int frame_num;      //nn或pir触发时连续抓取图片数量
    int aov_interval;
    int aovnn_enable;
    int md_enable;
    int venc_mempool_pct;
    int aovmd_threshold;
    float aovnn_threshold;
};

struct stream_buf {
    void* stream_addr;
    int   stream_size;
};

struct wake_source {
    int grpid;
    AOV_WAKEUP_REASON   wakeup_reason;      //rtt唤醒linux原因
    AOV_WAKEUP_REASON   event_status;      //rtt唤醒linux原因
};

typedef struct {
    int cmd;
    int grpid;
    //wake reason

    union {
        //rtt stream
        struct stream_buf buf;
        //stream buf
        struct aov_ctrl aov;


        struct wake_source wake_source;
    } data;

} AOV_XBUS_CMD_S;


#endif // __AOV_XBUS_COMMAND_H__
