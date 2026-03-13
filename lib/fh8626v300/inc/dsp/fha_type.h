#ifndef __FH_TYPE_H__
#define __FH_TYPE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*----------------------------------------------*
 * The common data type definition              *
 *----------------------------------------------*/

#ifndef __TYPE_DEF_H__
typedef unsigned char           FH_UINT8;
typedef unsigned short          FH_UINT16;
typedef unsigned int            FH_UINT32;
typedef unsigned long long      FH_UINT64;

typedef signed char             FH_SINT8;
typedef short                   FH_SINT16;
typedef int                     FH_SINT32;
typedef long long               FH_SINT64;


typedef float                   FH_FLOAT;
typedef double                  FH_DOUBLE;


typedef char                    FH_CHAR;
#define FH_VOID                 void

typedef unsigned int            FH_HANDLE;
typedef FH_UINT8 *              FH_VADDR;
typedef unsigned int            FH_PHYADDR;

/*----------------------------------------------*
 * const definition                             *
 *----------------------------------------------*/
typedef enum {
    FH_FALSE = 0,
    FH_TRUE  = 1,
    DUMMY = 0xffffffff,
} FH_BOOL;


#ifndef NULL
    #define NULL        0L
#endif

#define FH_NULL         0L
#define FH_SUCCESS      0
#define FH_FAILURE      (-1)
#endif


/*fixed for enum type*/
#define INT_MINI     (-2147483647 - 1) /* minimum (signed) int value */
#define INT_MAXI       2147483647    /* maximum (signed) int value */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __FH_TYPE_H__ */

