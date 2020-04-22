/******************************************************************************
Copyright Shanghai YG Electronic Technology Co., Ltd.
FileName: base.h
Description:  base include file
******************************************************************************/
#ifndef __BASE_H__
#define __BASE_H__

#ifndef BOOL_DEF
typedef int                 BOOL;
#define BOOL_DEF
#endif

#ifndef TRUE
#define TRUE                1
#define	FALSE               0
#endif

#include <stdint.h>
typedef signed char         INT8, *PINT8;
typedef signed short        INT16, *PINT16;
typedef signed int          INT32, *PINT32;
typedef unsigned char       UINT8, *PUINT8;
typedef unsigned short      UINT16, *PUINT16;
typedef unsigned int        UINT32, *PUINT32;
typedef unsigned long long  UINT64, *PUINT64;

#define RET_SUCC            0
#define RET_FAIL            -1
#define RET_TIMEOUT         -2
#define RET_NOMEM           -3

#ifndef OS_RTT

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define PRINTF(format, arg...)      printf(format, ##arg)

//#define _MALLOC_DEBUG_

#ifdef _MALLOC_DEBUG_

static inline void *malloc_dbg(unsigned long nbytes, const char* file_name, const char* func_name, int line)
{
    void* ptr = malloc(nbytes);
    PRINTF("[%s, %s, %d]malloc size=%ld, ret=%p\n", file_name, func_name, line, nbytes, ptr);
    return ptr;
}

static inline void *realloc_dbg(void *mem_address, unsigned int newsize, const char* file_name, const char* func_name, int line)
{
    void* ptr = realloc(mem_address, newsize);
    PRINTF("[%s, %s, %d]realloc %p newsize=%d, ret=%p\n", file_name, func_name, line, mem_address, newsize, ptr);
    return ptr;
}

static inline void *calloc_dbg(size_t n, size_t size, const char* file_name, const char* func_name, int line)
{
    void* ptr = calloc(n, size);
    PRINTF("[%s, %s, %d]calloc size=%d*%d, ret=%p\n", file_name, func_name, line, (int)n, (int)size, ptr);
    return ptr;
}

static inline void free_dbg(void *ptr, const char* file_name, const char* func_name, int line)
{
    PRINTF("[%s, %s, %d]free %p\n", file_name, func_name, line, ptr);
    free(ptr);
}

static inline char *strdup_dbg(const char *s, const char* file_name, const char* func_name, int line)
{
    PRINTF("[%s, %s, %d]strdup %s\n", file_name, func_name, line, s);
    return strdup(s);
}
#define MALLOC(n)                   malloc_dbg((n), __FILE__, __FUNCTION__, __LINE__)
#define REALLOC(p, n)               realloc_dbg((p), (n), __FILE__, __FUNCTION__, __LINE__)
#define CALLOC(n, m)                calloc_dbg((n), (m), __FILE__, __FUNCTION__, __LINE__)
#define FREE(p)                     free_dbg((p), __FILE__, __FUNCTION__, __LINE__)
#define STRDUP(s)                   strdup_dbg((s), __FILE__, __FUNCTION__, __LINE__)
#else
#if 1
#define MALLOC(n)                   malloc(n)
#define REALLOC(p, n)               realloc(p, n)
#define CALLOC(n, m)                calloc(n, m)
#define FREE(p)                     free(p)
#define STRDUP(s)                   strdup(s)
#else //use checkmem here
#include "utility/ut_checkmem.h"
#define MALLOC(n)                   ut_cmalloc(n, __FILE__, __LINE__)
#define REALLOC(p, n)               ut_crealloc(p, n, __FILE__, __LINE__)
#define CALLOC(n, m)                ut_ccalloc(n, m, __FILE__, __LINE__)
#define FREE(p)                     ut_cfree(p)
#define STRDUP(s)                   ut_cstrdup(s, __FILE__, __LINE__)
#endif
#endif

#define ACCEPT(a,b,c)               accept(a,b,c)
#define BIND(a,b,c)                 bind(a,b,c)
#define SHUTDOWN(a,b)               shutdown(a,b)
#define CLOSESOCKET(s)              close(s)
#define CONNECT(a,b,c)              connect(a,b,c)
#define GETSOCKNAME(a,b,c)          getsockname(a,b,c)
#define GETPEERNAME(a,b,c)          getpeername(a,b,c)
#define SETSOCKOPT(a,b,c,d,e)       setsockopt(a,b,c,d,e)
#define GETSOCKOPT(a,b,c,d,e)       getsockopt(a,b,c,d,e)
#define LISTEN(a,b)                 listen(a,b)
#define RECV(a,b,c,d)               recv(a,b,c,d)
#define RECVFROM(a,b,c,d,e,f)       recvfrom(a,b,c,d,e,f)
#define SEND(a,b,c,d)               send(a,b,c,d)
#define SENDTO(a,b,c,d,e,f)         sendto(a,b,c,d,e,f)
#define OPENSOCKET(a,b,c)           socket(a,b,c)
#define SELECT(a,b,c,d,e)           select(a,b,c,d,e)
#define IOCTLSOCKET(a,b,c)          ioctl(a,b,c)
#define READSOCKET(a,b,c)           read(a,b,c)
#define WRITESOCKET(a,b,c)          write(a,b,c)
#define FCNTL(a,b,c)                fcntl(a,b,c)
#define GETHOSTBYNAME(a)            gethostbyname(a)
#define GETHOSTBYNAME_R(a,b,c,d,e,f)    gethostbyname_r(a,b,c,d,e,f)
#define GETADDRINFO(a,b,c,d)        getaddrinfo(a,b,c,d)
#define FREEADDRINFO(a)             freeaddrinfo(a)

#define SYNC()                      sync()

#else

#include "rtthread.h"

#define PRINTF(format, arg...)      rt_kprintf(format, ##arg)

#ifdef _MALLOC_DEBUG_

static inline void *rt_malloc_dbg(unsigned long nbytes, const char* file_name, const char* func_name, int line)
{
    void* ptr = rt_malloc(nbytes);
    PRINTF("[%s, %s, %d]rt_malloc size=%ld, ret=%p\n", file_name, func_name, line, nbytes, ptr);
    return ptr;
}

static inline void *rt_realloc_dbg(void *mem_address, rt_size_t nbytes, const char* file_name, const char* func_name, int line)
{
    void* ptr = rt_realloc(mem_address, nbytes);
    PRINTF("[%s, %s, %d]rt_realloc %p newsize=%d, ret=%p\n", file_name, func_name, line, mem_address, nbytes, ptr);
    return ptr;
}

static inline void *rt_calloc_dbg(rt_size_t count, rt_size_t size, const char* file_name, const char* func_name, int line)
{
    void* ptr = rt_calloc(count, size);
    PRINTF("[%s, %s, %d]rt_calloc size=%d*%d, ret=%p\n", file_name, func_name, line, count, size, ptr);
    return ptr;
}

static inline void rt_free_dbg(void *ptr, const char* file_name, const char* func_name, int line)
{
    PRINTF("[%s, %s, %d]rt_free %p\n", file_name, func_name, line, ptr);
    rt_free(ptr);
}

static inline char *rt_strdup_dbg(const char *s, const char* file_name, const char* func_name, int line)
{
    PRINTF("[%s, %s, %d]rt_strdup %s\n", file_name, func_name, line, s);
    return rt_strdup(s);
}

#define MALLOC(n)                   rt_malloc_dbg((n), __FILE__, __FUNCTION__, __LINE__)
#define REALLOC(p, n)               rt_realloc_dbg((p), (n), __FILE__, __FUNCTION__, __LINE__)
#define CALLOC(n, m)                rt_calloc_dbg((n), (m), __FILE__, __FUNCTION__, __LINE__)
#define FREE(p)                     rt_free_dbg((p), __FILE__, __FUNCTION__, __LINE__)
#define STRDUP(s)                   rt_strdup_dbg((s), __FILE__, __FUNCTION__, __LINE__)

#else

#define MALLOC(n)                   rt_malloc(n)
#define REALLOC(p, n)               rt_realloc(p, n)
#define CALLOC(n, m)                rt_calloc(n, m)
#define FREE(p)                     rt_free(p)
#define STRDUP(s)                   rt_strdup(s)

#endif

#define ACCEPT(a,b,c)               lwip_accept(a,b,c)
#define BIND(a,b,c)                 lwip_bind(a,b,c)
#define SHUTDOWN(a,b)               lwip_shutdown(a,b)
#define CLOSESOCKET(s)              lwip_close(s)
#define CONNECT(a,b,c)              lwip_connect(a,b,c)
#define GETSOCKNAME(a,b,c)          lwip_getsockname(a,b,c)
#define GETPEERNAME(a,b,c)          lwip_getpeername(a,b,c)
#define SETSOCKOPT(a,b,c,d,e)       lwip_setsockopt(a,b,c,d,e)
#define GETSOCKOPT(a,b,c,d,e)       lwip_getsockopt(a,b,c,d,e)
#define LISTEN(a,b)                 lwip_listen(a,b)
#define RECV(a,b,c,d)               lwip_recv(a,b,c,d)
#define RECVFROM(a,b,c,d,e,f)       lwip_recvfrom(a,b,c,d,e,f)
#define SEND(a,b,c,d)               lwip_send(a,b,c,d)
#define SENDTO(a,b,c,d,e,f)         lwip_sendto(a,b,c,d,e,f)
#define OPENSOCKET(a,b,c)           lwip_socket(a,b,c)
#define SELECT(a,b,c,d,e)           lwip_select(a,b,c,d,e)
#define IOCTLSOCKET(a,b,c)          lwip_ioctl(a,b,c)
#define READSOCKET(a,b,c)           lwip_read(a,b,c)
#define WRITESOCKET(a,b,c)          lwip_write(a,b,c)
#define FCNTL(a,b,c)                lwip_fcntl(a,b,c)
#define GETHOSTBYNAME(a)            lwip_gethostbyname(a)
#define GETHOSTBYNAME_R(a,b,c,d,e,f)    lwip_gethostbyname_r(a,b,c,d,e,f)
#define GETADDRINFO(a,b,c,d)        lwip_getaddrinfo(a,b,c,d)
#define FREEADDRINFO(a)             lwip_freeaddrinfo(a)

#if (FH_RTT_SDK_VER >= 120)
    #define SYNC()                  sync()
#else
    #define SYNC()
#endif

#endif // OS_RTT

#define DISABLE_UNUSED_WARNING(x)       ((void)(x))

#ifndef UNUSED
#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(_p)                   do { if((_p) != NULL) { FREE(_p); (_p) = NULL; } } while(0)
#endif

#ifndef SAFE_MALLOC
#define SAFE_MALLOC(_p, _size)          do { if((_p) != NULL) FREE(_p); (_p) = MALLOC(_size); } while(0)
#endif

#ifndef SAFE_STRDUP
#define SAFE_STRDUP(_p, _str)           do { if((_p) != NULL) FREE(_p); if(_str) { (_p) = STRDUP(_str); } } while(0)
#endif

#endif // __BASE_H__
