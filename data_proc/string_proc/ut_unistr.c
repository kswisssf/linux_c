/******************************************************************************
Copyright Shanghai YG Electronic Technology Co., Ltd.
FileName: ut_unistr.c
Description:  Unicode string utility
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <malloc.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>
#include <sys/time.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <fcntl.h>
#include <math.h>

#include "base.h"

#define _MAX_CHAR_NUM		0xFFFF
#define _MAX_CHAR_BUF		0xFFFF
static BOOL str_is_arabic(const unsigned char* p_utf8);

int ut_uni_strlen(const unsigned short* string)
{
	int i=0;

	if(NULL == string)
		return 0;
	while (string[i])
		i++;
	return i;
}

unsigned short* ut_uni_strcpy(unsigned short* dst, const unsigned short* src)
{
	unsigned short* str = dst;
	if((NULL == dst)|| (NULL == src))
	{
		return NULL;
	}
	while(*src)
    {
        *dst++ = *src++;
    }
    *dst = 0;

	return str;
}

unsigned short* ut_uni_strncpy(unsigned short* dst, const unsigned short* src, int len)
{
	unsigned short* str = dst;
	if((NULL == dst)|| (NULL == src))
	{
		return NULL;
	}
	while(*src && len > 0)
    {
        *dst++ = *src++;
        len--;
    }
    *dst = 0;

	return str;
}

unsigned short* ut_uni_strcat(unsigned short* dst, const unsigned short* src)
{
	unsigned short* str = dst;
	if((NULL == dst)|| (NULL == src))
	{
		return NULL;
	}
    while(*dst!=0)
    {
        dst++;
    }

    while((*dst++ = *src++)!= 0);
    return str;
}

unsigned short* ut_uni_strinsert(unsigned short* dst, const unsigned short* src, int pos)
{
#define UT_UNI_INSERT_TEMP_BUF_SIZE     64
    unsigned short temp_str[UT_UNI_INSERT_TEMP_BUF_SIZE];
	unsigned short* str;

	int dlen = ut_uni_strlen(dst);

	if((NULL == dst)|| (NULL == src)||(dlen < pos))
	{
		return NULL;
	}

	if(dlen < UT_UNI_INSERT_TEMP_BUF_SIZE)
        str = temp_str;
    else
        str = (unsigned short*)malloc(dlen * sizeof(unsigned short));
    ut_uni_strcpy(str, dst+pos);
    ut_uni_strcpy(dst+pos, src);
    ut_uni_strcat(dst, str);
    if(str != temp_str)
        FREE(str);
    return dst;
}

unsigned short ut_char_asc_to_uni(char asc_char)
{
#ifdef __SYS_ENDIAN_LE__
       return (short)(asc_char<<8);
#else
       return (unsigned short)asc_char;
#endif

}

void ut_str_asc_to_uni(unsigned short* ustr, const char* astr)
{
    int i;
    int str_len = strlen(astr);
    for(i = 0; i < str_len; i++)
    {
        ustr[i] = ut_char_asc_to_uni(astr[i]);
    }
    ustr[i] = 0;
}

int char_unicode_to_utf8(UINT16 unicode, UINT8* p_utf8, int* p_utf8_len)
{
	if(!p_utf8)
	{
		if(p_utf8_len)
			*p_utf8_len = 0;
		return -1;
	}

	if(unicode < 0x80)
	{
		*p_utf8 = (UINT8)unicode;
		if(p_utf8_len)
			*p_utf8_len = 1;
		return 0;
	}

	if(unicode < 0x800)
	{
		*p_utf8++ = (UINT8)((unicode >> 6) | 0xC0);
		*p_utf8 = (UINT8)((unicode & 0x3F) | 0x80);
		if(p_utf8_len)
			*p_utf8_len = 2;
		return 0;
	}

	*p_utf8++ = (UINT8)((unicode >> 12) | 0xE0);
	*p_utf8++ = (UINT8)(((unicode >> 6) & 0x3F)| 0x80);
	*p_utf8 = (UINT8)((unicode & 0x3F) | 0x80);

	if(p_utf8_len)
		*p_utf8_len = 3;
	return 0;
}

int ut_str_unicode_to_utf8(const UINT16* p_unicode, UINT8* p_utf8, int* p_char_num)
{
	int ret = 0;
	int utf8_len;
	int char_num = 0;
	UINT16* p_unicode_cur;

	if(!p_utf8)
		return 0;

	int max_char_num = 0;
	if(p_char_num)
		max_char_num = *p_char_num;
	if(max_char_num == 0)
		max_char_num = 32;

	p_unicode_cur = (UINT16*)p_unicode;
	while(p_unicode && char_num < max_char_num && *p_unicode_cur)
	{
		if(char_unicode_to_utf8(*p_unicode_cur, p_utf8, &utf8_len) != 0)
		{
			ret = -1;
			break;
		}
		p_utf8 += utf8_len;
		char_num++;
		p_unicode_cur++;
	}
	*p_utf8 = 0;
	if(p_char_num)
		*p_char_num = char_num;

	return ret;
}

//UNICODE(hex) UTF-8(bin)
//0000 - 007F  0xxxxxxx
//0080 - 07FF  110xxxxx 10xxxxxx
//0800 - FFFF  1110xxxx 10xxxxxx 10xxxxxx
int ut_char_utf8_to_unicode(const unsigned char* p_utf8, UINT16* p_unicode, int* p_utf8_len)
{
	if(!p_utf8)
	{
		if(p_unicode)
			*p_unicode = 0;
		return RET_FAIL;
	}

	if(*p_utf8 < 0x80)
	{
		if(p_unicode)
			*p_unicode = *p_utf8;

		if(p_utf8_len)
			*p_utf8_len = 1;
		return RET_SUCC;
	}

	if((*p_utf8 & 0xE0) == 0xC0 && (*(p_utf8 + 1) & 0xC0) == 0x80)
	{
		if(p_unicode)
		{
			*p_unicode = *p_utf8 & 0x1F;
			(*p_unicode) <<= 6;
			(*p_unicode) |= *(p_utf8 + 1) & 0x3F;
		}
		if(p_utf8_len)
			*p_utf8_len = 2;
		return RET_SUCC;
	}

	if((*p_utf8 & 0xF0) == 0xE0 && (*(p_utf8 + 1) & 0xC0) == 0x80 && (*(p_utf8 + 2) & 0xC0) == 0x80)
	{
		if(p_unicode)
		{
			*p_unicode = *p_utf8 & 0x0F;
			(*p_unicode) <<= 6;
			(*p_unicode) |= *(p_utf8 + 1) & 0x3F;
			(*p_unicode) <<= 6;
			(*p_unicode) |= *(p_utf8 + 2) & 0x3F;
		}
		if(p_utf8_len)
			*p_utf8_len = 3;
		return RET_SUCC;
	}

	if(p_unicode)
		*p_unicode = *p_utf8;
	if(p_utf8_len)
		*p_utf8_len = 1;

	return RET_FAIL;
}

int ut_str_utf8_to_unicode(const unsigned char* p_utf8, UINT16* p_unicode, int* p_char_num)
{
	int ret = RET_SUCC;
	int utf8_len;
	int char_num = 0;
	unsigned char* p_utf8_cur;

	int max_char_num = 0;
	if(p_char_num)
		max_char_num = *p_char_num;
	if(max_char_num == 0)
		max_char_num = _MAX_CHAR_NUM;

	p_utf8_cur = (unsigned char*)p_utf8;
	while(p_utf8 && char_num < max_char_num && *p_utf8_cur)
	{
		if(ut_char_utf8_to_unicode(p_utf8_cur, p_unicode, &utf8_len) != 0)
		{
			ret = RET_FAIL;
			break;
		}
		p_utf8_cur += utf8_len;
		char_num++;
		if(p_unicode)
			p_unicode++;
	}

	if(p_unicode)
		*p_unicode = 0;
	if(p_char_num)
		*p_char_num = char_num;

	return ret;
}

int ut_utf8_buflen(const unsigned char* p_utf8)
{
	int str_len = 0;
	int utf8_len;
	unsigned char* p_utf8_cur;

	if(!p_utf8)
        return 0;

	p_utf8_cur = (unsigned char*)p_utf8;
	while(*p_utf8_cur)
	{
		if(ut_char_utf8_to_unicode(p_utf8_cur, NULL, &utf8_len) != RET_SUCC)
		{
			break;
		}
		p_utf8_cur += utf8_len;
		str_len += utf8_len;
	}

	return str_len;
}

static BOOL unicode_is_arabic(UINT16 unicode)
{
/*
	UNICODE_TYPE_t type = unicode_get_type(unicode);
	if(type == UNICODE_TYPE_ARABIC || type == UNICODE_TYPE_ARABIC_PRESENTATION_A || type == UNICODE_TYPE_ARABIC_PRESENTATION_B)
		return TRUE;
*/
    if(unicode >= 0x0600 && unicode <= 0x06FF)
		return TRUE;
    if(unicode >= 0xFB50 && unicode <= 0xFDFF)
		return TRUE;
    if(unicode >= 0xFE70 && unicode <= 0xFEFF)
		return TRUE;

	return FALSE;
}

static BOOL str_is_arabic(const unsigned char* p_utf8)
{
	UINT16 unicode;
	int utf8_len;
	int right=0, wrong=0;
	unsigned char* p_utf8_cur;

	if(!p_utf8)
		return FALSE;
	p_utf8_cur = (unsigned char*)p_utf8;
	while(*p_utf8_cur)
	{
		if(ut_char_utf8_to_unicode(p_utf8_cur, &unicode, &utf8_len) != RET_SUCC)
		{
			return FALSE;
		}
		if(unicode_is_arabic(unicode))
			right++;
                else
                        wrong++;
		p_utf8_cur += utf8_len;
	}

	return right*9>=wrong? TRUE: FALSE;
}



#if 0
int str_convert_arabic(const PBYTE_t p_src_utf8, PBYTE_t p_dst_utf8, STR_LANG_MODE_t str_mode)
{
	int str_len = utf8_strlen(p_src_utf8);
	PUNICODE_t uni_dst;
	PUNICODE_t uni_convert;
	UNICODE_t uni_prev, uni, uni_next;
	int offset = 0;
	int char_len = 0;
    int i;

	if(!p_src_utf8)
	{
	    *p_dst_utf8 = 0;
		return 0;
	}

	uni_dst = (PUNICODE_t)ut_malloc((str_len + 1) * sizeof(UNICODE_t));
	uni_convert = (PUNICODE_t)ut_malloc((str_len + 1) * sizeof(UNICODE_t));

	uni_prev = 0;
	ut_char_utf8_to_unicode(p_src_utf8, &uni, &char_len);

	for(i = 0; offset < str_len; i++)
	{
		BOOL prev_tab, next_tab;

		offset += char_len;
		ut_char_utf8_to_unicode(p_src_utf8 + offset, &uni_next, &char_len);
		if((uni >= 0x0621) && (uni <= 0x064A))//it is an arabic char
		{
			prev_tab = unichar_is_in_table(uni_prev, theSet1, ARABIC_SET1_COUNT);
			next_tab = unichar_is_in_table(uni_next, theSet2, ARABIC_SET2_COUNT);
			if((0x0644 == uni) && ((0x0622 == uni_next) || (0x0623 == uni_next) || (0x0625 == uni_next) || (0x0627 == uni_next)))
			{
				if(prev_tab)
				{
					uni_dst[i] = arabic_specs[(uni_next - 0x0621) / 2][1];
				}
				else
				{
					uni_dst[i] = arabic_specs[(uni_next - 0x0621) / 2][0];
				}
                offset += char_len;
				uni_prev = uni;
				uni = uni_next;
				ut_char_utf8_to_unicode(p_src_utf8 + offset, &uni_next, &char_len);
			}
			else if(prev_tab && next_tab)
			{
				uni_dst[i] = Arbic_Position[uni - 0x0621][ARABIC_MIDDLE];
			}
			else if(prev_tab)
			{
				uni_dst[i] = Arbic_Position[uni - 0x0621][ARABIC_FIRST];
			}
			else if(next_tab)
			{
				uni_dst[i] = Arbic_Position[uni - 0x0621][ARABIC_LAST];
			}
			else
			{
				uni_dst[i] = Arbic_Position[uni - 0x0621][ARABIC_ALONE];
			}
		}
		else
		{
			uni_dst[i] = uni;
		}
		uni_prev = uni;
		uni = uni_next;
	}
	uni_dst[i] = 0;

	PUNICODE_t p_uni_1 = uni_dst;
	PUNICODE_t p_uni_2 = uni_convert;
	STR_LANG_MODE_t uni_str_mode;
	int len;

    if(str_mode == STR_LANG_MODE_ARABIC)
    {
        p_uni_2 = uni_convert + str_unicode_length(uni_dst);
        *p_uni_2 = 0;
        while(*p_uni_1)
        {
            len = str_get_next_whole(p_uni_1, &uni_str_mode);
            p_uni_2 -= len;
            if(uni_str_mode == STR_LANG_MODE_ARABIC)
            {
                str_copy_revert(p_uni_1, p_uni_2, len);
            }
            else
            {
                MEMCPY(p_uni_2, p_uni_1, len * sizeof(UNICODE_t));
            }
            p_uni_1 += len;
        }
    }
    else
    {
        p_uni_2 = uni_convert;
        while(*p_uni_1)
        {
            len = str_get_next_whole(p_uni_1, &uni_str_mode);
            if(uni_str_mode == STR_LANG_MODE_ARABIC)
            {
                str_copy_revert(p_uni_1, p_uni_2, len);
            }
            else
            {
                MEMCPY(p_uni_2, p_uni_1, len * sizeof(UNICODE_t));
            }
            p_uni_1 += len;
            p_uni_2 += len;
        }
        *p_uni_2 = 0;
    }

	ut_str_unicode_to_utf8(uni_convert, p_dst_utf8, NULL);

	FREE(uni_dst);
	FREE(uni_convert);

    return 0;
}
#endif

int ut_str_utf8_char_num(const unsigned char* p_utf8)
{
	int char_num = 0;
	if(str_is_arabic(p_utf8))
	{
		// to be added
		//char*	str_arabic;
		//int	str_len = ut_utf8_buflen(p_utf8);

		//str_arabic = (PBYTE_t)ut_malloc(str_len * 4 +1);
		//str_convert_arabic(p_utf8,str_arabic, STR_LNAG_MODE_DEFAULT);
		//ut_str_utf8_to_unicode(str_arabic, (PUNICODE_t)NULL, &char_num);
		//FREE(str_arabic);
	}
	else
	{
		ut_str_utf8_to_unicode(p_utf8, (UINT16*)NULL, &char_num);
	}
	return char_num;
}


// E.O.F
