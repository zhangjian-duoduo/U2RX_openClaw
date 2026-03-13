#ifndef __FH_MCDB_API_H__
#define __FH_MCDB_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#define MCDB_API_ERRNO(n)    (0xadb00000|n)

#define MCDB_API_ERROR_OPENDEV      MCDB_API_ERRNO(1)
#define MCDB_API_ERROR_MAGIC        MCDB_API_ERRNO(2)
#define MCDB_API_ERROR_CRC          MCDB_API_ERRNO(3)
#define MCDB_API_ERROR_DBHEAD       MCDB_API_ERRNO(4)
#define MCDB_API_ERROR_MALLOC       MCDB_API_ERRNO(5)
#define MCDB_API_ERROR_IOCTL        MCDB_API_ERRNO(6)
#define MCDB_API_ERROR_NOTSUPPORT   MCDB_API_ERRNO(7)

/*
 * FH_MCDB_Database_Init
 * db_buf: database buf
 * size: size of database
 * return: 0 on success, errcode
 */
int FH_MCDB_Database_Init(void *db_buf, int size);

/*
 * FH_MCDB_Load_DBFile
 * db_file: database file name
 */
int FH_MCDB_Load_DBFile(char *db_file);

#ifdef __cplusplus
}
#endif

#endif // __FH_MCDB_API_H__
