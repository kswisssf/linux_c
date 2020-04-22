/******************************************************************************
Copyright Shanghai YG Electronic Technology Co., Ltd.
FileName: ut_str.c
Description:  String utility
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

#include "ut_str.h"
#include "base.h"

char* ut_strcpy(char* p_dst, const char* p_src)
{
    if(!p_dst)
    {
        //dbg_log_w("p_dst==NULL\n");
        return NULL;
    }
    if(!p_src)
    {
        *p_dst = 0;
        return p_dst;
    }

    return strcpy(p_dst, p_src);
}

char* ut_strncpy(char* p_dst, const char* p_src, int n)
{
    if(!p_dst)
    {
        //dbg_log_w("p_dst==NULL\n");
        return NULL;
    }
    if(!p_src)
    {
        *p_dst = 0;
        return p_dst;
    }

    char* p = p_dst;
    int i;
    for(i = 0; i < n; i++)
    {
        if(*p_src == 0)
            break;
        *p++ = *p_src++;
    }
    *p = 0;

    return p_dst;
}

int ut_strlen(const char* p_str)
{
    if(!p_str)
        return 0;
    return strlen(p_str);
}

char* ut_strdup(const char* p_str)
{
    if (!p_str)
        return NULL;
    return STRDUP(p_str);
}

int ut_strcmp(const char* p_str1, const char* p_str2)
{
    if (!p_str1 && !p_str2)
        return 0;
    if (!p_str1 && p_str2)
        return -1;
    if (p_str1 && !p_str2)
        return 1;
    return strcmp(p_str1, p_str2);
}

int ut_strncmp(const char* p_str1, const char* p_str2, int n)
{
    if (!p_str1 && !p_str2)
        return 0;
    if (!p_str1 && p_str2)
        return -1;
    if (p_str1 && !p_str2)
        return 1;
    return strncmp(p_str1, p_str2, n);
}       

int ut_strcasecmp(const char* p_str1, const char* p_str2)
{
    if (!p_str1 && !p_str2)
        return 0;
    if (!p_str1 && p_str2)
        return -1;
    if (p_str1 && !p_str2)
        return 1;
    return strcasecmp(p_str1, p_str2);
}

int ut_strncasecmp(const char* p_str1, const char* p_str2, int n)
{
    if (!p_str1 && !p_str2)
        return 0;
    if (!p_str1 && p_str2)
        return -1;
    if (p_str1 && !p_str2)
        return 1;
    return strncasecmp(p_str1, p_str2, n);
}

BOOL ut_strstart(const char* p_str1, const char* p_str2)
{
    return ut_strncmp(p_str1, p_str2, ut_strlen(p_str2)) == 0 ? TRUE : FALSE;
}

BOOL ut_strcasestart(const char* p_str1, const char* p_str2)
{
    return ut_strncasecmp(p_str1, p_str2, ut_strlen(p_str2)) == 0 ? TRUE : FALSE;
}

char* ut_strcat(char* p_str1, const char* p_str2)
{
    char* ptr1;
    char* ptr2;

    if(!p_str1)
        return NULL;
    if(!p_str2)
        return p_str1;

    ptr1 = p_str1 + strlen(p_str1);
    for(ptr2 = (char*)p_str2; *ptr2; ptr2++)
    {
        if(*ptr2 == 0x20)
        {
            *ptr1++ = '%';
            *ptr1++ = '2';
            *ptr1++ = '0';
        }
        else
        {
            *ptr1++ = *ptr2;
        }
    }
    *ptr1 = 0;

    return p_str1;
}

char* ut_strtrimright(char* p_str)
{
    int len;
    char* ptr;

    len = ut_strlen(p_str);
    if(len <= 0)
        return p_str;

    for(ptr = p_str + len - 1; ptr >= p_str && (*ptr == 0x20 || *ptr == 0x09 || *ptr == 0x0d || *ptr == 0x0a || *ptr == 0x1a); ptr--);
    ptr++;
    *ptr = 0;

    return p_str;
}

char* ut_strsplit(const char* p_str, char split_ch, int idx, char* p_split_str, int len)
{
    char* ptr = (char*)p_str;
    int i = 0;

    *p_split_str = 0;

    while(ptr)
    {
        for( ; *ptr != 0 && *ptr == split_ch; ptr++);
        if(*ptr == 0 && i < idx)
            break;

        if(i == idx)
        {
            char* p_in = ptr;
            char* p_out = p_split_str;
            int l;

            for(l = 0; l < len && *p_in != 0 && *p_in != split_ch; p_in++, p_out++, l++)
            {
                *p_out = *p_in;
            }
            *p_out = 0;
            return p_split_str;
        }

        ptr = strchr(ptr, split_ch);
        i++;
    }

    return NULL;
}

int ut_hex_to_int(char hex)
{
    int temp = 0;

    if((hex>='a')&&(hex<='f'))
    {
        temp = hex-'a'+10;
    }
    else if((hex>='A')&&(hex<='F'))
    {
        temp = hex-'A'+10;
    }
    else if((hex>='0')&&(hex<='9'))
    {
        temp = hex-'0';
    }
    return temp;
}

int ut_str_hex_to_int(const char* hex)
{
    int sum = 0;
    int i;
    int temp;

    for (i = 0; hex[i] != 0; i++)
    {
        if((hex[i]>='a')&&(hex[i]<='f'))
        {
            temp = hex[i]-'a'+10;
        }
        else if((hex[i]>='A')&&(hex[i]<='F'))
        {
            temp = hex[i]-'A'+10;
        }
        else if((hex[i]>='0')&&(hex[i]<='9'))
        {
            temp = hex[i]-'0';
        }
        else
        {
            break;
        }
        sum = sum * 16 + temp;
    }
    return sum;
}

void* ut_memcpy(void *p_dst, const void *p_src, size_t n)
{
    if(!p_dst)
    {
        return NULL;
    }
    if(n == 0)
    {
        *((char*)p_dst) = 0;
        return p_dst;
    }
    if(!p_src)
    {
        memset(p_dst, 0, n);
        return p_dst;
    }

    return memcpy(p_dst, p_src, n);
}

// E.O.F
