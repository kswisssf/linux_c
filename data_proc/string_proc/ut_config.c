/******************************************************************************
Copyright Shanghai YG Electronic Technology Co., Ltd.
FileName: ut_config.c
Description:  Config file utility
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
#include "ut_str.h"
#include "ut_config.h"

#define UT_MAX_CONFIG_KEY_NUM   256
#define ut_malloc               malloc
#define dbg_log_e               printf
#define dbg_log_w               printf


typedef struct
{
    int                 m_max_num;
    int                 m_num;
    int                 m_sort;
    BOOL                m_saved;
    char                m_filename[256];
    UT_CONFIG_ITEM_t*   m_itmes;
} UT_CONFIG_LIST_t;

//static int _ut_config_get_num(FILE* fp);
static int _ut_config_read(FILE* fp, UT_CONFIG_LIST_t* p_config_list, int num);
static int _ut_config_find(H_CONFIG_t h_config, const char* p_key);
static int _ut_config_insert(UT_CONFIG_LIST_t* p_config_list, const char* p_key, const char* p_value);

int ut_read_key_value_ex(char* buf, char** pp_key, char** pp_value, int org_flag)
{
    char* ptr;
    char* p_key;
    char* p_value;

    if(!buf)
        return RET_FAIL;

    if (!org_flag)
    {
        ptr = strchr(buf, '#');
        if(ptr)
            *ptr = 0;
    }

    ptr = strchr(buf, '\r');
    if(ptr)
        *ptr = 0;
    ptr = strchr(buf, '\n');
    if(ptr)
        *ptr = 0;

    ptr = strchr(buf, '=');
    if(ptr == NULL)
        return RET_FAIL;
    *ptr = 0;

    p_key = buf;
    p_value = ptr + 1;

    if (!org_flag)
    {
        // trim key left
        for(ptr = p_key; *ptr == ' ' || *ptr == '\t'; ptr++);
        p_key = ptr;
        // trim key right
        ptr = strchr(p_key, ' ');
        if(ptr)
            *ptr = 0;
        ptr = strchr(p_key, '\t');
        if(ptr)
            *ptr = 0;

        if(*p_key == 0)
            return RET_FAIL;        // no key

        // trim value left
        for(ptr = p_value; *ptr == ' ' || *ptr == '\t'; ptr++);
        p_value = ptr;
        // trim value right
        for(ptr = p_value + strlen(p_value) - 1; ptr > p_value && (*ptr == ' ' || *ptr == '\t'); ptr--);
        ptr++;
        *ptr = 0;
    }

    if(pp_key)
        *pp_key = p_key;
    if(pp_value)
        *pp_value = p_value;

    return RET_SUCC;
}

int ut_read_key_value(char* buf, char** pp_key, char** pp_value)
{
    return ut_read_key_value_ex(buf,pp_key,pp_value, 0);
}

H_CONFIG_t ut_config_load_ex(const char* p_config_file, int max_num, int sort_flag)
{
    FILE* fp = NULL;
    UT_CONFIG_LIST_t* p_config_list;
    int num;

    if(p_config_file)
    {
        fp = fopen(p_config_file, "r");
        if(!fp)
        {
            printf("Open file fail: %s\n", p_config_file);
            return 0;
        }
    }

#if 0
    num = _ut_config_get_num(fp);
    if(num <= 0)
    {
        dbg_log_w("Can't get config value\n");
        fclose(fp);
        return 0;
    }
#else
    if(max_num <= 0)
        num = UT_MAX_CONFIG_KEY_NUM;
    else
        num = max_num;
#endif

    p_config_list = (UT_CONFIG_LIST_t *)ut_malloc(sizeof(UT_CONFIG_LIST_t));
    if(fp)
    {
        strncpy(p_config_list->m_filename, p_config_file, 255);
    }

    p_config_list->m_sort = sort_flag;
    _ut_config_read(fp, p_config_list, num);

    fclose(fp);

    return (H_CONFIG_t)p_config_list;
}

H_CONFIG_t ut_config_load(const char* p_config_file, int max_num)
{
    return ut_config_load_ex(p_config_file, max_num, 1);
}

BOOL ut_string_to_bool(const char* p_str, BOOL def_val)
{
    if(ut_strcasecmp(p_str, "1") == 0)
        return TRUE;
    if(ut_strcasecmp(p_str, "0") == 0)
        return FALSE;
    if(ut_strcasecmp(p_str, "true") == 0)
        return TRUE;
    if(ut_strcasecmp(p_str, "false") == 0)
        return FALSE;
    if(ut_strcasecmp(p_str, "yes") == 0)
        return TRUE;
    if(ut_strcasecmp(p_str, "no") == 0)
        return FALSE;
    if(ut_strcasecmp(p_str, "on") == 0)
        return TRUE;
    if(ut_strcasecmp(p_str, "off") == 0)
        return FALSE;

    return def_val;
}


H_CONFIG_t ut_config_create_ex(const char* p_config_file, int max_num, int sort_flag)
{
    FILE* fp = NULL;
    UT_CONFIG_LIST_t* p_config_list;

    if(max_num <= 0)
        max_num = UT_MAX_CONFIG_KEY_NUM;

    if(p_config_file)
    {
        fp = fopen(p_config_file, "w");
        if(!fp)
        {
            printf("Open file fail: %s\n", p_config_file);
            return 0;
        }
    }

    p_config_list = (UT_CONFIG_LIST_t *)ut_malloc(sizeof(UT_CONFIG_LIST_t));
    strncpy(p_config_list->m_filename, p_config_file, 255);
    p_config_list->m_itmes = (UT_CONFIG_ITEM_t*)ut_malloc(max_num * sizeof(UT_CONFIG_ITEM_t));
    p_config_list->m_max_num = max_num;
    p_config_list->m_sort = sort_flag;

    fclose(fp);

    return (H_CONFIG_t)p_config_list;
}

H_CONFIG_t ut_config_create(const char* p_config_file, int max_num)
{
    return ut_config_create_ex(p_config_file, max_num, 1);
}

int ut_config_save(H_CONFIG_t h_config, const char* p_config_file)
{
    UT_CONFIG_LIST_t* p_config_list = (UT_CONFIG_LIST_t*)h_config;
    FILE* fp;
    int i;

    if(!p_config_list)
    {
        return -1;
    }

    if(p_config_list->m_saved && !p_config_file)
    {
        // already saved
        return 0;
    }

    if(!p_config_file)
        p_config_file = p_config_list->m_filename;

    fp = fopen(p_config_file, "w");
    if(!fp)
    {
        dbg_log_e("Open file fail: %s\n", p_config_file);
        return -1;
    }

    for(i = 0; i < p_config_list->m_num; i++)
    {
        if(ut_strlen(p_config_list->m_itmes[i].m_key) <= 0)
            continue;
        fprintf(fp, "%s=%s\n", p_config_list->m_itmes[i].m_key, p_config_list->m_itmes[i].m_value ? p_config_list->m_itmes[i].m_value : "");
    }

    fclose(fp);
    SYNC();

    if(!p_config_file)
        p_config_list->m_saved = TRUE;

    return 0;
}

int ut_config_free(H_CONFIG_t h_config)
{
    UT_CONFIG_LIST_t* p_config_list = (UT_CONFIG_LIST_t*)h_config;
    int i;

    if(!p_config_list)
        return 0;

    for(i = 0; i < p_config_list->m_max_num; i++)
    {
        SAFE_FREE(p_config_list->m_itmes[i].m_key);
        SAFE_FREE(p_config_list->m_itmes[i].m_value);
    }
    SAFE_FREE(p_config_list->m_itmes);
    SAFE_FREE(p_config_list);

    return 0;
}

int ut_config_get_item_num(H_CONFIG_t h_config)
{
    UT_CONFIG_LIST_t* p_config_list = (UT_CONFIG_LIST_t*)h_config;

    if(!p_config_list)
        return 0;

    return p_config_list->m_num;
}

UT_CONFIG_ITEM_t* ut_config_get_item(H_CONFIG_t h_config, int idx)
{
    UT_CONFIG_LIST_t* p_config_list = (UT_CONFIG_LIST_t*)h_config;

    if(!p_config_list)
        return NULL;

    if(idx < 0 || idx >= p_config_list->m_max_num)
        return NULL;

    return &p_config_list->m_itmes[idx];
}

const char* ut_config_get_text(H_CONFIG_t h_config, const char* p_key)
{
    UT_CONFIG_LIST_t* p_config_list = (UT_CONFIG_LIST_t*)h_config;
    int idx = _ut_config_find(h_config, p_key);

    if(idx < 0)
        return NULL;
    return p_config_list->m_itmes[idx].m_value;
}

const char* ut_config_get_text_ex(H_CONFIG_t h_config, const char* p_key, const char* p_sub_key)
{
    char* p_temp_key;
    const char* p_ret = NULL;

    p_temp_key = ut_malloc(1024);
    if(ut_strlen(p_sub_key) > 0)
    {
        snprintf(p_temp_key, 1024, "%s:%s", p_key, p_sub_key);
        p_ret = ut_config_get_text(h_config, p_temp_key);
    }
    if(!p_ret)
    {
        p_ret = ut_config_get_text(h_config, p_key);
    }
    FREE(p_temp_key);
    return p_ret;
}

int ut_config_set_text(H_CONFIG_t h_config, const char* p_key, const char* p_val)
{
    UT_CONFIG_LIST_t* p_config_list = (UT_CONFIG_LIST_t*)h_config;
    int idx = _ut_config_find(h_config, p_key);

    if(idx >= 0)
    {
        SAFE_FREE(p_config_list->m_itmes[idx].m_value);
        p_config_list->m_itmes[idx].m_value = ut_strdup(p_val);
        p_config_list->m_saved = FALSE;
        return 0;
    }

    idx = _ut_config_find(h_config, NULL);
    if(idx < 0)
    {
        dbg_log_e("No enough empty key space\n");
        return -1;
    }

    _ut_config_insert(p_config_list, p_key, p_val);
    p_config_list->m_saved = FALSE;

    return 0;
}

int ut_config_get_int(H_CONFIG_t h_config, const char* p_key, int def_val)
{
    const char* p_val = ut_config_get_text(h_config, p_key);
    if(!p_val)
        return def_val;

    return atoi(p_val);
}

int ut_config_get_int_ex(H_CONFIG_t h_config, const char* p_key, const char* p_sub_key, int def_val)
{
    const char* p_val = ut_config_get_text_ex(h_config, p_key, p_sub_key);
    if(!p_val)
        return def_val;

    return atoi(p_val);
}

int ut_config_set_int(H_CONFIG_t h_config, const char* p_key, int val)
{
    char temp[32];

    sprintf(temp, "%d", val);
    return ut_config_set_text(h_config, p_key, temp);
}

BOOL ut_config_get_bool(H_CONFIG_t h_config, const char* p_key, BOOL def_val)
{
    const char* p_val = ut_config_get_text(h_config, p_key);
    if(!p_val)
        return def_val;

    return ut_string_to_bool(p_val, def_val);
}

BOOL ut_config_get_bool_ex(H_CONFIG_t h_config, const char* p_key, const char* p_sub_key, BOOL def_val)
{
    const char* p_val = ut_config_get_text_ex(h_config, p_key, p_sub_key);
    if(!p_val)
        return def_val;

    return ut_string_to_bool(p_val, def_val);
}

int ut_config_set_bool(H_CONFIG_t h_config, const char* p_key, BOOL val)
{
    return ut_config_set_text(h_config, p_key, val ? "yes" : "no");
}

int ut_config_delete(H_CONFIG_t h_config, const char* p_key)
{
    UT_CONFIG_LIST_t* p_config_list = (UT_CONFIG_LIST_t*)h_config;
    int idx = _ut_config_find(h_config, p_key);
    if(idx < 0)
    {
        // not found the key
        return -1;
    }

    SAFE_FREE(p_config_list->m_itmes[idx].m_key);
    SAFE_FREE(p_config_list->m_itmes[idx].m_value);

    int i;
    for(i = idx; i < p_config_list->m_num - 1; i++)
    {
        memcpy(&p_config_list->m_itmes[i], &p_config_list->m_itmes[i + 1], sizeof(UT_CONFIG_ITEM_t));
    }
    (p_config_list->m_num)--;
    memset(&p_config_list->m_itmes[p_config_list->m_num], 0, sizeof(UT_CONFIG_ITEM_t));

    p_config_list->m_saved = FALSE;

    return 0;
}

#if 0
static int _ut_config_get_num(FILE* fp)
{
    char line_buf[512];
    int num = 0;

    fseek(fp, 0, SEEK_SET);
    while(fgets(line_buf, sizeof(line_buf) - 1, fp))
    {
        if(ut_read_key_value(line_buf, NULL, NULL) == RET_SUCC)
            num++;
    }

    return num;
}
#endif

#define UT_CONFIG_LINE_BUF_LEN      4096
static int _ut_config_read(FILE* fp, UT_CONFIG_LIST_t* p_config_list, int num)
{
    char* p_key;
    char* p_value;
    char* p_line_buf;
    //int n = 0;

    p_line_buf = ut_malloc(UT_CONFIG_LINE_BUF_LEN);
    if(!p_line_buf)
        return -1;

    p_config_list->m_itmes = (UT_CONFIG_ITEM_t *)ut_malloc(num * sizeof(UT_CONFIG_ITEM_t));
    p_config_list->m_max_num = num;
    p_config_list->m_num = 0;
    p_config_list->m_saved = FALSE;

    if(!fp)
        goto DONE;

    fseek(fp, 0, SEEK_SET);
    while(fgets(p_line_buf, UT_CONFIG_LINE_BUF_LEN - 1, fp))
    {
        if(p_config_list->m_num >= num)
            break;

        if(ut_read_key_value(p_line_buf, &p_key, &p_value) != RET_SUCC)
            continue;

        _ut_config_insert(p_config_list, p_key, p_value);
    }
    p_config_list->m_saved = TRUE;

DONE:
    SAFE_FREE(p_line_buf);

    return 0;
}

static int _ut_config_find(H_CONFIG_t h_config, const char* p_key)
{
    UT_CONFIG_LIST_t* p_config_list = (UT_CONFIG_LIST_t*)h_config;

    if(!p_config_list)
        return -1;

    if(!p_key)
    {
        if(p_config_list->m_num < p_config_list->m_max_num)
            return p_config_list->m_num;
        return -1;
    }

    if(p_config_list->m_sort)
    {
        int min = 0;
        int max = p_config_list->m_num - 1;
        int mid;

        while(min <= max)
        {
            mid = (min + max) >> 1;
            int result = ut_strcmp(p_config_list->m_itmes[mid].m_key, p_key);
            if(result == 0)
            {
                return mid;
            }
            if(result < 0)
            {
                min = mid + 1;
            }
            else
            {
                max = mid - 1;
            }
        }
    }
    else
    {
        int i;
        for(i = 0; i < p_config_list->m_num; i++)
        {
            if(ut_strcmp(p_config_list->m_itmes[i].m_key, p_key) == 0)
                return i;
        }
    }

    return -1;
}

static int _ut_config_insert(UT_CONFIG_LIST_t* p_config_list, const char* p_key, const char* p_value)
{
    if(p_config_list->m_num >= p_config_list->m_max_num)
        return -1;

    int i;
    if(p_config_list->m_sort)
    {
        for(i = p_config_list->m_num - 1; i >= 0; i--)
        {
            if(ut_strcmp(p_key, p_config_list->m_itmes[i].m_key) < 0)
            {
                memcpy(&p_config_list->m_itmes[i + 1], &p_config_list->m_itmes[i], sizeof(p_config_list->m_itmes[i]));
            }
            else
            {
                break;
            }
        }
        i++;
    }
    else
    {
        i = p_config_list->m_num;
    }
    p_config_list->m_itmes[i].m_key = ut_strdup(p_key);
    p_config_list->m_itmes[i].m_value = ut_strdup(p_value);
    (p_config_list->m_num)++;

    return 0;
}

#ifdef EMAIL_TEMPLET
static int _ut_strstr(const char* p_str1, const char* p_str2)
{
    int i,len;
    char* p_src;
    if (!p_str1 || !p_str2)
        return -1;

    if (ut_strlen(p_str2)<=0)
        return -1;

    len = ut_strlen(p_str1) - ut_strlen(p_str2)+1;;
    p_src = (char*)p_str1;

    for(i = 0; i < len; i++){
        if(ut_strstart(p_src, p_str2)){
            return i;
        }else{
            p_src++;
        }
    }
    return -1;
}

int ut_get_mail_templet(const char* p_mail_file, const char* key_name, char* buf, int buf_len)
{
    FILE*   fp = NULL;
    int     flen;
    char*   tmp_buf;
    char*   cpy_point;
    char    start[32];
    char    end[32];
    BOOL    find = FALSE;
    char*   p_target;
    int     cpy_len = 0;
    int     line_len = 0;

    if(p_mail_file)
    {
        fp = fopen(p_mail_file, "r");
        if(!fp)
        {
            dbg_log_e("Open file fail: %s\n", p_mail_file);
            return -1;
        }
    }

    fseek(fp,0L,SEEK_END); /* 定位到文件末尾 */
    flen = ftell(fp); /* 得到文件大小 */
    if(flen > 0){
        sprintf(start, "<%s>", key_name);
        sprintf(end, "</%s>", key_name);
        tmp_buf = (char*)ut_malloc(flen);
        p_target = buf;
        fseek(fp, 0, SEEK_SET);
        while(fgets(tmp_buf, flen - 1, fp)){
            cpy_point = tmp_buf;
            if(!find){
                if(ut_strstart(cpy_point, start)){
                    find = TRUE;
                    cpy_point += ut_strlen(start);
                }
            }

            if(find){
                line_len =_ut_strstr(cpy_point,end);

                if(line_len >= 0){
                    if((cpy_len < buf_len) && (cpy_len + line_len > buf_len)){
                        line_len = buf_len - cpy_len;
                    }
                    if(line_len > 0){
                        ut_strncpy(p_target, cpy_point, line_len);
                        cpy_len += line_len;
                    }
                    break;
                }else{
                    line_len = ut_strlen(cpy_point);
                    if((cpy_len < buf_len) && (cpy_len + line_len > buf_len)){
                        line_len = buf_len - cpy_len;
                    }
                    ut_strncpy(p_target, cpy_point, line_len);
                    p_target += ut_strlen(cpy_point);
                    cpy_len += line_len;
                    if(cpy_len >= buf_len){
                        break;
                    }
                }
            }
        }
        fclose(fp);
        FREE(tmp_buf);
    }
    if(find){
        return 0;
    }
    return -1;
}
#endif
// E.O.F
