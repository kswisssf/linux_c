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

#include "ut_config.h"
#include "base.h"

BOOL ut_file_exist(const char* p_file)
{
    return TRUE;
}

int ut_fread_config(const char* p_file, const char* p_key, char *p_val, int len)
{
    if(!ut_file_exist(p_file))
    {
        printf("file unexist\n");
        return -1;
    }

    H_CONFIG_t hconfig = ut_config_load(p_file, 0);
    if(hconfig == 0)
        return -1;

    int ret = -1;
    const char* p_read_val = ut_config_get_text(hconfig, p_key);
    //ut_strncpy(p_val, p_read_val, len - 1);
    strncpy(p_val, p_read_val, len - 1);
    if(p_read_val)
        ret = 0;
    ut_config_free(hconfig);

    return ret;
}

/*
 * @brief       从配置文件读数据
 *
 * @param[in]   p_file:         文件名
 * @param[in]   p_key:          key
 * @param[out]  p_val:          输出字串
 * @param[in]   len:            输出字串最大长度
 *
 * @return      0成功，非0失败
 *

extern int ut_fread_config(const char* p_file, const char* p_key, char *p_val, int len);
extern int ut_fread_config_int(const char* p_file, const char* p_key, int def_val);
extern BOOL ut_fread_config_bool(const char* p_file, const char* p_key, BOOL def_val);

extern int ut_fread_config_ex(const char* p_file, const char* p_key, const char* p_sub_key, char *p_val, int len);
extern int ut_fread_config_int_ex(const char* p_file, const char* p_key, const char* p_sub_key, int def_val);
extern BOOL ut_fread_config_bool_ex(const char* p_file, const char* p_key, const char* p_sub_key, BOOL def_val);
*/
int apl_dev_get_sensor(void)
{
    int ret = -1;
    char temp_buf[32];
    printf("%s\n", __func__);
    if(ut_fread_config("config/sysinfo/hw_info", "sensor", temp_buf, sizeof(temp_buf)) != 0)
    { 
        printf("get sensor name failed\n");
        return ret;
    }
    printf("sensor name: %s\n",temp_buf);
    
    return 0;
}    

int main(int argc, char **argv)
{

    apl_dev_get_sensor();

    return 0;
}


