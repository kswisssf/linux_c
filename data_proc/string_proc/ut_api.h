/******************************************************************************
Copyright Shanghai YG Electronic Technology Co., Ltd.
FileName: ut_api.h
Description:  Utility APIs
******************************************************************************/
#ifndef _UT_API_H_
#define _UT_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "std_inc.h"

#include "ut_time.h"
#include "ut_memp.h"
#include "ut_str.h"
#include "ut_unistr.h"
#include "ut_net.h"
#include "ut_netcfg.h"
#include "ut_socket.h"
#include "ut_watchdog.h"
#include "ut_config.h"
#include "ut_file.h"
#include "ut_misc.h"
#include "ut_wifi.h"
#include "ut_packbits.h"
#include "ut_storage.h"
#include "ut_base64.h"
#include "ut_crc32.h"
#include "ut_md5.h"
#include "ut_sha1.h"
#include "ut_blowfish.h"
#include "ut_atomic.h"
#include "ut_net_server.h"
#include "ut_sslmail.h"
#include "ut_cmd.h"
#include "ut_mtd.h"
#include "ut_uboot_env.h"
#include "ut_fwimg_info.h"
#include "ut_fwimg_apenv.h"
#include "ut_fwimg_upgrade.h"
#include "ut_json.h"
#include "ut_websocket.h"

#ifndef OS_RTT

#include "ut_dev.h"
#include "ut_ftpc.h"
#include "ut_iwtool.h"

#define UT_MEMCPY       memcpy
#define UT_MEMSET       memset

#else

#include "ut_rtt.h"

#include "ut_dns.h"

#if 0
#define UT_MEMCPY       rt_memcpy
#define UT_MEMSET       rt_memset
#else
#define UT_MEMCPY       memcpy
#define UT_MEMSET       memset
#endif

#endif // OS_RTT

#include "ut_vthread.h"

#define UT_ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))
#define UT_VAL_ALIGN(size, align)       (((size) + (align) - 1) & ~((align) - 1))
#define UT_MEM_ALIGN(size, align)       UT_VAL_ALIGN(size, align)

#define UT_ARRAY_SIZE(ary)              ((int)(sizeof(ary) / sizeof(ary[0])))

#if defined(OS_RTT) && (FH_RTT_SDK_VER < 131)
    #define USING_FLOAT2STRING
#endif

#ifdef USING_FLOAT2STRING
    #define FLOAT_BUF_DEF(a,b)      char a[b]
    #define FLOAT_TYPE(a)           "%s"
    #define FLOAT_VAL(a,b,c,d)      ut_float2string(a,b,c,d)
#else
    #define FLOAT_BUF_DEF(a,b)
    #define FLOAT_TYPE(a)           a
    #define FLOAT_VAL(a,b,c,d)      a
#endif

#ifdef __cplusplus
}
#endif

#endif // _UT_API_H_
// E.O.F
