/******************************************************************************
Copyright Shanghai YG Electronic Technology Co., Ltd.
FileName: ut_str.h
Description:  String utility APIs
******************************************************************************/
#ifndef _UT_STR_H_
#define _UT_STR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "base.h"

/**
 * @brief       基本功能同strcpy，增加了NULL的判断。
 *
 * @param[out]  p_dst:          输出字串，当它为NULL时，直接返回NULL
 * @param[in]   p_src:          输入字串，当它为NULL时，p_dst为""
 *
 * @return      返回p_dst
 *
 */
extern char* ut_strcpy(char* p_dst, const char* p_src);

/**
 * @brief       基本功能同strncpy，增加了NULL的判断，0结尾
 *
 * @param[out]  p_dst:          输出字串，当它为NULL时，直接返回NULL
 * @param[in]   p_src:          输入字串，当它为NULL时，p_dst为""
 * @param[in]   n:              字符最大长度
 *
 * @return      返回p_dst
 * @note        当p_src长度超过n时，在p_dst第n+1位置填0，所以要保证p_dst有n+1个字符可写
 */
extern char* ut_strncpy(char* p_dst, const char* p_src, int n);

/**
 * @brief       基本功能同strlen，增加了NULL的判断
 *
 * @param[out]  p_str:          输入字串，当它为NULL时，返回0
 *
 * @return      返回字串长度
 */
extern int ut_strlen(const char* p_str);

/**
 * @brief       基本功能同strdup，增加了NULL的判断
 *
 * @param[out]  p_str:          输入字串，当它为NULL时，返回NULL
 *
 * @return      返回复制字串，使用MALLOC分配，需要FREE释放
 */
extern char* ut_strdup(const char* p_str);

/**
 * @brief       基本功能同strcmp，增加了NULL的判断
 *
 * @param[in]   p_str1:         比较字串1
 * @param[in]   p_str2:         比较字串2
 *
 * @return      p_str1==NULL, p_str2==NULL, 返回0
 *              p_str1==NULL, p_str2!=NULL, 返回-1
 *              p_str1!=NULL, p_str2==NULL, 返回1
 *              p_str1!=NULL, p_str2!=NULL, strcmp(p_str1, p_str2)
 */
extern int ut_strcmp(const char* p_str1, const char* p_str2);

/**
 * @brief       基本功能同strncmp，增加了NULL的判断
 *
 * @param[in]   p_str1:         比较字串1
 * @param[in]   p_str2:         比较字串2
 * @param[in]   n:              比较字串长度
 *
 * @return      p_str1==NULL, p_str2==NULL, 返回0
 *              p_str1==NULL, p_str2!=NULL, 返回-1
 *              p_str1!=NULL, p_str2==NULL, 返回1
 *              p_str1!=NULL, p_str2!=NULL, strncmp(p_str1, p_str2, n)
 */
extern int ut_strncmp(const char* p_str1, const char* p_str2, int n);

/**
 * @brief       基本功能同strcasecmp或stricmp，增加了NULL的判断
 *
 * @param[in]   p_str1:         比较字串1
 * @param[in]   p_str2:         比较字串2
 *
 * @return      p_str1==NULL, p_str2==NULL, 返回0
 *              p_str1==NULL, p_str2!=NULL, 返回-1
 *              p_str1!=NULL, p_str2==NULL, 返回1
 *              p_str1!=NULL, p_str2!=NULL, strcasecmp(p_str1, p_str2)
 */
extern int ut_strcasecmp(const char* p_str1, const char* p_str2);

/**
 * @brief       基本功能同strncasecmp或stricmp，增加了NULL的判断
 *
 * @param[in]   p_str1:         比较字串1
 * @param[in]   p_str2:         比较字串2
 * @param[in]   n:              比较字串长度
 *
 * @return      p_str1==NULL, p_str2==NULL, 返回0
 *              p_str1==NULL, p_str2!=NULL, 返回-1
 *              p_str1!=NULL, p_str2==NULL, 返回1
 *              p_str1!=NULL, p_str2!=NULL, strcasecmp(p_str1, p_str2)
 */
extern int ut_strncasecmp(const char* p_str1, const char* p_str2, int n);

/**
 * @brief       判断一个字串是否开始于另一个字串
 *
 * @param[in]   p_str1:         字串1
 * @param[in]   p_str2:         字串2
 *
 * @return      p_str1==NULL, p_str2==NULL, 返回TRUE
 *              p_str1==NULL, p_str2!=NULL, 返回FALSE
 *              p_str1!=NULL, p_str2==NULL, 返回FALSE
 *              p_str1!=NULL, p_str2!=NULL, p_str1是否开始于p_str2
 */
extern BOOL ut_strstart(const char* p_str1, const char* p_str2);

/**
 * @brief       判断一个字串是否开始于另一个字串(不区分大小写)
 *
 * @param[in]   p_str1:         字串1
 * @param[in]   p_str2:         字串2
 *
 * @return      p_str1==NULL, p_str2==NULL, 返回TRUE
 *              p_str1==NULL, p_str2!=NULL, 返回FALSE
 *              p_str1!=NULL, p_str2==NULL, 返回FALSE
 *              p_str1!=NULL, p_str2!=NULL, p_str1是否开始于p_str2
 */
extern BOOL ut_strcasestart(const char* p_str1, const char* p_str2);

/**
 * @brief       判断一个字串是否结束于另一个字串
 *
 * @param[in]   p_str1:         字串1
 * @param[in]   p_str2:         字串2
 *
 * @return      p_str1==NULL, p_str2==NULL, 返回TRUE
 *              p_str1==NULL, p_str2!=NULL, 返回FALSE
 *              p_str1!=NULL, p_str2==NULL, 返回FALSE
 *              p_str1!=NULL, p_str2!=NULL, p_str1是否结束于p_str2
 */
extern BOOL ut_strend(const char* p_str1, const char* p_str2);

/**
 * @brief       判断一个字串是否结束于另一个字串(不区分大小写)
 *
 * @param[in]   p_str1:         字串1
 * @param[in]   p_str2:         字串2
 *
 * @return      p_str1==NULL, p_str2==NULL, 返回TRUE
 *              p_str1==NULL, p_str2!=NULL, 返回FALSE
 *              p_str1!=NULL, p_str2==NULL, 返回FALSE
 *              p_str1!=NULL, p_str2!=NULL, p_str1是否结束于p_str2
 */
extern BOOL ut_strcaseend(const char* p_str1, const char* p_str2);

/**
 * @brief       基本功能同strcat，增加了NULL的判断
 *
 * @param[in][out]  p_str1:     需要附加p_str2字串，当p_str1==NULL，返回NULL
 * @param[in]   p_str2:         字串2，当p_str2==NULL，直接返回p_str1
 *
 * @return      p_str1
 */
extern char* ut_strcat(char* p_str1, const char* p_str2);

/**
 * @brief       去掉字串尾部空格/换行，包含0x20, 0x09, 0x0d, 0x0a, 0x1a
 *
 * @param[in][out]  p_str:      需要去掉空格字串
 *
 * @return      p_str
 */
extern char* ut_strtrimright(char* p_str);

/**
 * @brief       去掉字串尾部换行，包含0x0d, 0x0a, 0x1a
 *
 * @param[in][out]  p_str:      需要去掉换行字串
 *
 * @return      p_str
 */
extern char* ut_strtrimend(char* p_str);

/**
 * @brief       判断字符串是否空字符串（NULL或者“”）
 *
 * @param[in]   p_str:      输入字符串
 *
 * @return      是否空字符串
 */
extern BOOL ut_str_is_empty(const char* p_str);

/**
 * @brief       分割字符串
 *
 * @param[in]   p_str:          需要分割字符串
 * @param[in]   split_ch:       分割字符
 * @param[in]   idx:            第几个分割字串
 * @param[out]  p_split_str:    输出字串
 * @param[in]   len:            输出字串最大长度（包含结尾0）
 *
 * @return      p_split_str，NULL表示失败
 */
extern char* ut_strsplit(const char* p_str, char split_ch, int idx, char* p_split_str, int len);

/**
 * @brief       把字符串转换成大写
 *
 * @param[in]   p_str_src:      输入字符串
 * @param[in]   p_str_dst:      输出字符串，如果为NULL，表示直接转换p_str_src
 *
 * @return      输出字符串
 */
extern char* ut_strupper(char* p_str_src, char* p_str_dst);

/**
 * @brief       把字符串转换成小写
 *
 * @param[in]   p_str_src:      输入字符串
 * @param[in]   p_str_dst:      输出字符串，如果为NULL，表示直接转换p_str_src
 *
 * @return      输出字符串
 */
extern char* ut_strlower(char* p_str_src, char* p_str_dst);

/**
 * @brief       HEX字符转换成整数值，比如5转成5，b转成11
 *
 * @param[in]   hex:        HEX字符
 *
 * @return      转换的整数值，失败返回-1
 *
 */
extern int ut_hex_to_int(char hex);

/**
 * @brief       16进制字符串转换成整数值
 *
 * @param[in]   p_hex:      16进制字符串
 *
 * @return      转换的整数值，失败返回-1
 *
 */
extern int ut_str_hex_to_int(const char* p_hex);

/**
 * @brief       HEX字符串转换成字符，比如“61”->'a'
 *
 * @param[in]   p_hex:      16进制字符串
 *
 * @return      转换的整字符，失败返回-1
 *
 */
extern int ut_hex_to_byte(const char *p_hex);

/**
 * @brief       HEX字符串转换成字符串，比如“6162636465”->"abcde"
 *
 * @param[in]   hex:        16进制字符串
 * @param[out]  p_buffer:   输出字符串
 * @param[in]   max_len:    输出字符串最大长度
 *
 * @return      转换成功的字符串长度
 *
 */
extern int ut_hex_str_to_buffer(const char *hex, unsigned char* p_buffer, int max_len);

/**
 * @brief       字符串转换成HEX字符串，比如"abcde"->“6162636465”
 *
 * @param[in]   p_buffer:   输入字符串
 * @param[in]   buf_size:   转换长度
 * @param[out]  p_hex:      输出HEX字符串
 *
 * @return      0
 *
 */
extern int ut_hex_buffer_to_str(const char* p_buffer, int buf_size, char *p_hex);

/**
 * @brief       字符串转换成网络URL字符串，比如"a b"->“a%20b”
 *
 * @param[in]   p_src:      输入字符串
 * @param[out]  p_dst:      输出字符串
 * @param[in]   dst_max:    输出字符串最大长度
 *
 * @return      输出字符串
 *
 */
extern char* ut_str_convert_to_net(const unsigned char* p_src, char* p_dst, int dst_max);

/**
 * @brief       网络URL字符串转换成字符串，比如“a%20b”->"a b"
 *
 * @param[in]   p_src:      输入字符串
 * @param[out]  p_dst:      输出字符串
 * @param[in]   dst_max:    输出字符串最大长度
 *
 * @return      输出字符串
 *
 */
extern unsigned char* ut_str_convert_from_net(const char* p_src, unsigned char* p_dst, int dst_max);

/**
 * @brief       基本功能同memcpy，增加了NULL的判断。
 *
 * @param[out]  p_dst:          输出buffer，当它为NULL时，直接返回NULL
 * @param[in]   p_src:          输入buffer，当它为NULL时，p_dst为填0
 * @param[in]   n:              buffer大小
 *
 * @return      返回p_dst
 *
 */
extern void* ut_memcpy(void *p_dst, const void *p_src, size_t n);

/**
 * @brief       fast memcpy, 只针对RTT，LINUX等同于memcpy
 *
 * @param[out]  p_dst:          输出buffer，当它为NULL时，直接返回NULL
 * @param[in]   p_src:          输入buffer，当它为NULL时，p_dst为填0
 * @param[in]   len:            buffer大小
 *
 * @return      返回p_dst
 *
 */
void* ut_fast_memcpy(void* p_dst, const void* p_src, size_t len);

#define utm_strncpy(_dst, _src)     ut_strncpy((_dst), (_src), sizeof(_dst) - 1)
#define utm_memcpy(_dst, _src)      ut_memcpy((_dst), (_src), sizeof(_dst))
#define utm_memset(_v, _c)          memset(&(_v), (_c), sizeof(_v))
#define utm_memzero(_v)             utm_memset((_v), 0)

#ifdef __cplusplus
}
#endif

#endif // _UT_STR_H_
// E.O.F
