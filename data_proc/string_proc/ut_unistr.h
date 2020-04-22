/******************************************************************************
Copyright Shanghai YG Electronic Technology Co., Ltd.
FileName: ut_unistr.h
Description:  Unicode string utility APIs
******************************************************************************/
#ifndef _UT_UNISTR_H_
#define _UT_UNISTR_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief       得到双字节Unicode(UTF-16)字串长度
 *
 * @param[in]   p_string:       双字节Unicode(UTF-16)字串
 *
 * @return      字串长度
 *
 */
extern int ut_uni_strlen(const unsigned short* p_string);

/**
 * @brief       ASCII字符转换成unicode
 *
 * @param[in]   asc_char:       ASCII字符
 *
 * @return      unicode
 *
 */
extern unsigned short ut_char_asc_to_uni(char asc_char);

/**
 * @brief       ASCII字串转换成UTF-16字串
 *
 * @param[out]  ustr:       输出UTF-16字串
 * @param[in]   astr:       ASCII字串
 *
 * @return      none
 *
 */
extern void ut_str_asc_to_uni(unsigned short* ustr, const char* astr);

/**
 * @brief       UTF-16转换成UTF-8
 *
 * @param[in]   unicode:    UTF-16
 * @param[out]  p_utf8:     UTF-8字串
 * @param[out]  p_utf8_len: UTF-8字串长度
 *
 * @return      none
 *
 */
extern int ut_char_unicode_to_utf8(unsigned short unicode, unsigned char* p_utf8, int* p_utf8_len);

/**
 * @brief       UTF-16字串字串转换成UTF-8字串
 *
 * @param[in]   p_unicode:      输入UTF-16字串
 * @param[out]  p_utf8:         输出UTF-8字串
 * @param[out]  p_char_num:     输出字符个数，NULL表示不输出
 *
 * @return      0成功，非0失败
 *
 */
extern int ut_str_unicode_to_utf8(const UINT16* p_unicode, UINT8* p_utf8, int* p_char_num);

/**
 * @brief       UTF-16字串复制
 *
 * @param[out]  dst:        输出字串
 * @param[in]   src:        输入字串
 *
 * @return      输出字串(dst)
 *
 */
extern unsigned short* ut_uni_strcpy(unsigned short* dst, const unsigned short* src);

/**
 * @brief       UTF-16字串复制
 *
 * @param[out]  dst:        输出字串
 * @param[in]   src:        输入字串
 * @param[in]   len:        字串最大長度
 *
 * @return      输出字串(dst)
 *
 */
extern unsigned short* ut_uni_strncpy(unsigned short* dst, const unsigned short* src, int len);

/**
 * @brief       UTF-16字串连接
 *
 * @param[out]  dst:        输出字串
 * @param[in]   src:        输入字串
 *
 * @return      输出字串(dst)
 *
 */
extern unsigned short* ut_uni_strcat(unsigned short* dst, const unsigned short* src);

/**
 * @brief       UTF-16字串插入
 *
 * @param[out]  dst:        输出字串
 * @param[in]   src:        输入字串
 * @param[in]   pos:        插入位置
 *
 * @return      输出字串(dst)
 *
 */
extern unsigned short* ut_uni_strinsert(unsigned short* dst, const unsigned short* src, int pos);

/**
 * @brief       UTF-8字符转换成UTF-16字符
 *
 * @param[in]   p_utf8:         UTF-8字符指针
 * @param[out]  p_unicode:      输出UTF-16字符，NULL表示不输出
 * @param[out]  p_utf8_len:     UTF-8字符字节数，NULL表示不输出
 *
 * @return      0成功，非0失败
 *
 */
extern int ut_char_utf8_to_unicode(const unsigned char* p_utf8, UINT16* p_unicode, int* p_utf8_len);

/**
 * @brief       UTF-8字串转换成UTF-16字串
 *
 * @param[in]   p_utf8:         UTF-8字串
 * @param[out]  p_unicode:      输出UTF-16字串，NULL表示不输出
 * @param[out]  p_char_num:     成功转换UTF-8字符数，NULL表示不输出
 *
 * @return      0全部转换成功，非0表示p_utf8中有无法转换的字符
 *
 */
extern int ut_str_utf8_to_unicode(const unsigned char* p_utf8, UINT16* p_unicode, int* p_char_num);

/**
 * @brief       UTF-8字串的字节数
 *
 * @param[in]   p_utf8:         UTF-8字串
 *
 * @return      UTF-8字串的字节数
 *
 */
extern int ut_utf8_buflen(const unsigned char* p_utf8);

/**
 * @brief       UTF-8字串的字符数
 *
 * @param[in]   p_utf8:         UTF-8字串
 *
 * @return      UTF-8字串的字符数
 *
 */
extern int ut_str_utf8_char_num(const unsigned char* p_utf8);

extern unsigned short ut_char_uni_gb2312(UINT16 unicode);
extern int ut_str_utf8_gb2312(const unsigned char* p_utf8, unsigned char* p_gb2312);
extern unsigned short ut_char_gb2312_uni(unsigned short gb);
extern int ut_str_gb2312_utf8(const unsigned char* p_gb2312, unsigned char* p_utf8);

#ifdef __cplusplus
}
#endif

#endif // _UT_UNISTR_H_
// E.O.F
