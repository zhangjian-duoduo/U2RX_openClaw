#ifndef __FH_DEBUG_H__
#define __FH_DEBUG_H__

void FHA_LogInfo(const char* str, ...);
void FHA_LogError(const char* str, ...);

#define FHA_CHECK_PTR(ptr, label)      do {\
    if( NULL == (ptr) ) {\
        FHA_LogError("Error: %s: %s at %d\n", __FILE__, __FUNCTION__, __LINE__);\
        goto label;\
    }\
} while(0)

#define FHA_CHECK_STATUS(status, label)  do {\
    if( status ) {\
        FHA_LogError("Error: %s: %s line %d, errcode[%d]\n", __FILE__, __FUNCTION__, __LINE__, status);\
        goto label;\
    }\
} while(0)

#define FHA_RETURN_STATUS(status)  do {\
    if( status ) {\
        FHA_LogError("Error: %s: %s line %d, errcode[%d]\n", __FILE__, __FUNCTION__, __LINE__, status);\
    }\
} while(0)


#endif /* __FH_DEBUG_H__ */

