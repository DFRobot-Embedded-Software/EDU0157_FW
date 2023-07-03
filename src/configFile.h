/**
 * @file file.h
 * @author your name (you@domain.com)
 * @brief U盘中文件的处理
 * @version 0.1
 * @date 2023-03-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef CONFIGFILE_H
#define CONFIGFILE_H
#include "udisk.h"
#include <stdlib.h>
#include <stdio.h>
#include "string.h"

#ifdef CONFIG_DBG_ENABLE
#define CONFIG_DBG(...) {Serial.print("config ");Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define CONFIG_DBG(...)
#endif
/**
 * @brief 创建标准文件和DTU文件
 * 
 */
void creatorStandardConfigFileAndDTUFile(void);

/**
 * @brief 获取标准配置文件
 * 
 */
void readstandardConfigFile(void);

/**
 * @brief 解析标准文件中每一行数据
 * 
 * @param str 需要解析的数据
 */
void standardConfigParsing(char *str);

/**
 * @brief 比较字符串
 * 
 * @param str1 原始数据
 * @param str2 比较数据
 */
uint8_t strCompare(char* str1,const char* str2);
/**
 * @brief 解析采样频率数据
 * 
 * @param str1 传入数据
 * @return uint8_t 返回单位
 */
uint8_t parsingSampleRate(char* str1);

/**
 * @brief 创建LOG文件
 * 
 */
void creatorLogFile(void);

/**
 * @brief 向log文件中记录数据
 * 
 * @param location 报错位置
 * @param str 报错问题
 */
void  writeLog(String location, String str);
/**
 * @brief 根据配置文件中的时间更新RTC
 * 
 * @param str 配置文件中时间
 * @return uint8_t 返回解析状态
 */
uint8_t timeRead(char* str);
/**
 * @brief 跟新config文件
 * 
 * @param str config 文件中数据
 */
void updataConfig(char* str);
/**
 * @brief 解析DTU文件
 * 
 */
void DTUParsing(void);
/**
 * @brief 解析DTU文件
 * 
 * @param str 需要解析的字符串
 */
void standardDTUParsing(char *str);
/**
 * @brief 存储CSV文件
 * 
 */
void csvRecord(void);
/**
 * @brief 创建CSV标题
 * 
 * @return 返回创建好的标题
 */
String dataName(void);
/**
 * @brief 获取数据
 * 
 * @return String 返回获取的csv数据
 */
String dataSplitting(void);
/**
 * @brief 添加topic
 * 
 * @param str topic 上传的数据
 * @param str1 topic 密钥
 */
void addTopic(char* str,char* str1);
/**
 * @brief 接卸topic的密钥
 * 
 * @param str 需要解析的数据
 * @return char* 返回解析好的数据
 */
char* getKey(char* str);

#endif
