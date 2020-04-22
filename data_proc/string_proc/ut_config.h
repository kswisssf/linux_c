/******************************************************************************
Copyright Shanghai YG Electronic Technology Co., Ltd.
FileName: ut_config.h
Description:  Config文件管理模块

Config文件格式:
[key1]=[value1]
[key2]=[value2]
...

注释符号'#‘
******************************************************************************/
#ifndef _UT_CONFIG_H_
#define _UT_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "base.h"

typedef struct
{
    char*               m_key;
    char*               m_value;
} UT_CONFIG_ITEM_t;

typedef long		H_CONFIG_t;     ///< config file handle type

/**
 * @brief       载入config文件数据
 *
 * @param[in]   p_config_file:  config文件名
 * @param[in]   max_num:        载入最大数据项，若为0，则默认256
 *
 * @return      config文件handle，0表示失败
 *
 */
extern H_CONFIG_t ut_config_load(const char* p_config_file, int max_num);
extern H_CONFIG_t ut_config_load_ex(const char* p_config_file, int max_num, int sort_flag);

/**
 * @brief       生成新的config文件
 *
 * @param[in]   p_config_file:  config文件名
 * @param[in]   max_num:        载入最大数据项，若为0，则默认256
 *
 * @return      config文件handle，0表示失败
 *
 */
extern H_CONFIG_t ut_config_create(const char* p_config_file, int max_num);
extern H_CONFIG_t ut_config_create_ex(const char* p_config_file, int max_num, int sort_flag);

/**
 * @brief       保存config文件数据
 *
 * @param[in]   h_config:       config handle
 * @param[in]   p_config_file:  保存文件名，如果为NULL，则表示保存在ut_config_load指定的文件
 *
 * @return      0成功，非0失败
 *
 */
extern int ut_config_save(H_CONFIG_t h_config, const char* p_config_file);

/**
 * @brief       释放config数据，调用ut_config_load载入的数据使用完毕，必须调用此函数释放
 *
 * @param[in]   h_config:       config handle
 *
 * @return      0成功，非0失败
 *
 */
extern int ut_config_free(H_CONFIG_t h_config);

/**
 * @brief       返回config item数
 *
 * @param[in]   h_config:       config handle
 *
 * @return      config item数
 *
 */
extern int ut_config_get_item_num(H_CONFIG_t h_config);

/**
 * @brief       返回config item
 *
 * @param[in]   h_config:       config handle
 * @param[in]   idx:            config item索引
 *
 * @return      config time，NULL失败
 *
 */
extern UT_CONFIG_ITEM_t* ut_config_get_item(H_CONFIG_t h_config, int idx);

/**
 * @brief       得到config文本值
 *
 * @param[in]   h_config:       config handle
 * @param[in]   p_key:          key
 *
 * @return      key对应的text value，NULL表示没有此key的值
 *
 */
extern const char* ut_config_get_text(H_CONFIG_t h_config, const char* p_key);
extern const char* ut_config_get_text_ex(H_CONFIG_t h_config, const char* p_key, const char* p_sub_key);

/**
 * @brief       设定config文本值
 *
 * @param[in]   h_config:       config handle
 * @param[in]   p_key:          key
 * @param[in]   p_val:          key对应的值
 *
 * @return      0成功，非0失败
 *
 */
extern int ut_config_set_text(H_CONFIG_t h_config, const char* p_key, const char* p_val);

/**
 * @brief       得到config整数值
 *
 * @param[in]   h_config:       config handle
 * @param[in]   p_key:          key
 * @param[in]   def_val:        默认值，如果获取指失败，取此默认值
 *
 * @return      key对应的整数值
 *
 */
extern int ut_config_get_int(H_CONFIG_t h_config, const char* p_key, int def_val);
extern int ut_config_get_int_ex(H_CONFIG_t h_config, const char* p_key, const char* p_sub_key, int def_val);

/**
 * @brief       设定config整数值
 *
 * @param[in]   h_config:       config handle
 * @param[in]   p_key:          key
 * @param[in]   val:            key对应的整数值
 *
 * @return      0成功，非0失败
 *
 */
extern int ut_config_set_int(H_CONFIG_t h_config, const char* p_key, int val);

/**
 * @brief       得到config BOOL值
 *
 * @param[in]   h_config:       config handle
 * @param[in]   p_key:          key
 * @param[in]   def_val:        默认值，如果获取指失败，取此默认值
 *
 * @return      key对应的BOOL值
 * @note        当config文件中value为"1","true","yes","on"时（不区分大小写），返回TRUE，否则为FALSE
 */
extern BOOL ut_config_get_bool(H_CONFIG_t h_config, const char* p_key, BOOL def_val);
extern BOOL ut_config_get_bool_ex(H_CONFIG_t h_config, const char* p_key, const char* p_sub_key, BOOL def_val);

/**
 * @brief       设定config BOOL值
 *
 * @param[in]   h_config:       config handle
 * @param[in]   p_key:          key
 * @param[in]   val:            key对应的BOOL值
 *
 * @return      0成功，非0失败
 *
 */
extern int ut_config_set_bool(H_CONFIG_t h_config, const char* p_key, BOOL val);

/**
 * @brief       删除某个key
 *
 * @param[in]   h_config:       config handle
 * @param[in]   p_key:          待删除key
 *
 * @return      0成功，非0失败
 *
 */
extern int ut_config_delete(H_CONFIG_t h_config, const char* p_key);

#ifdef EMAIL_TEMPLET
extern int ut_get_mail_templet(const char* p_mail_file, const char* key_name, char* buf, int buf_len);
#endif // EMAIL_TEMPLET

#ifdef __cplusplus
}
#endif

#endif // _UT_CONFIG_H_
// E.O.F
