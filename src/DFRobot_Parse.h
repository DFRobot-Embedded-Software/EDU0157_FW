/**
 * @file DFRobot_Parse.h
 * @brief I2C从机命令解析
 *
 * @copyright   Copyright (c) 2022 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-08-02
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 * 
 */

#ifndef __DFROBOT_PARSE_H
#define __DFROBOT_PARSE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Arduino.h>
#include "global.h"

#define CMD_START            0x00
// #define CMD_SET_IF0          0x00  ///< 设置接口0命令，可以用此命令配置A&D接口的功能和SKU
// #define CMD_SET_IF1          0x01  ///< 设置接口1命令，可以用此命令配置I2C1&UART1接口的功能和SKU
// #define CMD_SET_IF2          0x02  ///< 设置接口2命令，可以用此命令配置I2C2&UART2接口的功能和SKU
// #define CMD_READ_IF0         0x00  ///< 读取接口0的功能和SKU
// #define CMD_READ_IF1         0x01  ///< 读取接口1的功能和SKU
// #define CMD_READ_IF2         0x02  ///< 读取接口2的功能和SKU

// #define CMD_SET_ADDR         0x03  ///< 设置I2C地址命令（此命令，设置成功后，立即生效）
// #define CMD_READ_ADDR        0x03  ///< 读取I2C地址命令（此命令，设置成功后，立即生效）
// #define CMD_SET_TIME         0x04  ///< 设置时间的年，月，日，时，分，秒
// #define CMD_GET_TIME         0x04  ///< 获取时间的年，月，日，时，分，秒
// #define CMD_RECORD_ON        0x05  ///< 启动csv记录
// #define CMD_RECORD_OFF       0x06  ///< 停止CSV记录

// #define CMD_SCREEN_ON        0x07  ///< 开启oled显示
// #define CMD_SCREEN_OFF       0x08  ///< 关闭oled显示
// #define CMD_GET_NAME         0x09  ///< 获取传感器数据名
// #define CMD_GET_VALUE        0x0A  ///< 获取传感器数据值
// #define CMD_GET_UNIT         0x0B  ///< 获取传感器数据单位
// #define CMD_GET_SKU          0x0C  ///< 获取传感器的数据名，值和单位名，值和单位名之间空一格，其他用逗号(,)分开
// #define CMD_GET_INFO         0x0D  ///< 获取传感器的数据名，值和单位名，值和单位名之间空一格，其他用逗号(,)分开

// #define CMD_GET_KEY_VALUE0    0x0E  ///< 根据数据名获取对应的数据的值
// #define CMD_GET_KEY_VALUE1    0x0F  ///< 根据数据名获取选中的接口对应的数据的值
// #define CMD_GET_KEY_VALUE2    0x10  ///< 根据数据名获取选中的接口上指定SKU对应的数据的值
// #define CMD_GET_KEY_UINT0     0x11  ///< 根据数据名获取对应的数据的单位
// #define CMD_GET_KEY_UINT1     0x12  ///< 根据数据名获取选中的接口对应的数据的单位
// #define CMD_GET_KEY_UINT2     0x13  ///< 根据数据名获取选中的接口上指定SKU对应的数据的单位
// #define CMD_RESET             0x14  ///< 复制I2C从机发送缓存命令
// #define CMD_SKU_A             0x15  ///< 获取传感器转接板支持的Analog传感器SKU命令
// #define CMD_SKU_D             0x16  ///< 获取传感器转接板支持的Digital传感器SKU命令
// #define CMD_SKU_IIC           0x17  ///< 获取传感器转接板支持的I2C传感器SKU命令
// #define CMD_SKU_UART          0x18  ///< 获取传感器转接板支持的UART传感器SKU命令
// #define CMD_GET_TIMESTAMP     0x19  ///< 获取时间戳
// #define CMD_SET_REFRESH_TIME  0x20  ///< 设置刷新率
// #define CMD_GET_REFRESH_TIME  0x20  ///< 获取刷新率
// #define CMD_GET_VERSION       0x21  ///< 获取版本号

#define CMD_END             CMD_GET_VERSION

#define STATUS_SUCCESS      0x53  ///< 响应成功状态   
#define STATUS_FAILED       0x63  ///< 响应成功状态 

#define ERR_CODE_NONE               0x00 ///< 通信正常
#define ERR_CODE_CMD_INVAILED       0x01 ///< 无效命令
#define ERR_CODE_RES_PKT            0x02 ///< 响应包错误
#define ERR_CODE_M_NO_SPACE         0x03 ///< I2C主机内存不够
#define ERR_CODE_RES_TIMEOUT        0x04 ///< 响应包接收超时
#define ERR_CODE_CMD_PKT            0x05 ///< 无效的命令包或者命令不匹配
#define ERR_CODE_SLAVE_BREAK        0x06 ///< 从机故障
#define ERR_CODE_ARGS               0x07 ///< 设置的参数错误
#define ERR_CODE_SKU                0x08 ///< 该SKU为无效SKU，或者传感器通用适配器板(Sensor Universal Adapter Board)不支持
#define ERR_CODE_S_NO_SPACE         0x09 ///< I2C从机内存不够
#define ERR_CODE_I2C_ADRESS         0x0A ///< I2C地址无效

#define  RESPONSE_STATUS              0x00  ///< 响应包状态位
#define  RESPONSE_CMD                 0x01  ///< 响应包命令位
#define  RESPONSE_LEN_L               0x02  ///< 响应包数据长度低字节位
#define  RESPONSE_LEN_H               0x03  ///< 响应包数据长度高字节位
#define  RESPONSE_ERR_CODE            0x04  ///< 响应包错误代码位或数据初始位置
#define  RESPONSE_ERR_PKT_LEN         5     ///< 错误包响应长度

#define CMD_GET_DATA                0x00 //根据传过来的名称返回名字
#define CMD_GET_ALL_DATA            0x01 //获取板载全部传感器数据
#define CMD_SET_TIME                0x02 //设置板载RTC时间
#define CME_GET_TIME                0x03
#define CMD_GET_UNIT                0x04 //获取传感器单位
#define CMD_GET_VERSION             0x05 //获取版本号
#define CMD_RESET_DATA              0x06

/**
 * @fn i2cSend
 * @brief I2C发送包打包
 * 
 * @param data 数据缓存
 * @param len  数据长度
 */
extern void i2cSend(uint8_t *data, uint16_t len);

/**
 * @fn uartSend
 * @brief I2C发送包打包
 * 
 * @param data 数据缓存
 * @param len  数据长度
 */
extern void uartSend(uint8_t *data, uint16_t len);
/**
 * @fn DFRobot_Response
 * @brief 响应包打包
 * 
 * @param data 数据缓存
 * @param len  数据长度
 */
extern void DFRobot_Response(uint8_t *data, uint8_t len);



// /**
//  * @fn setOrGetInterface0
//  * @brief 设置或获取接口0(A&D)的配置
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t setOrGetInterface0(pCmdPacktet_t pkt);
// /**
//  * @fn setOrGetInterface1
//  * @brief 设置或获取接口1(I2C&UART1)的配置
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t setOrGetInterface1(pCmdPacktet_t pkt);
// /**
//  * @fn setOrGetInterface2
//  * @brief 设置或获取接口2(I2C&UART1)的配置
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t setOrGetInterface2(pCmdPacktet_t pkt);
// /**
//  * @fn setOrGetI2CAddress
//  * @brief 设置或获取从机I2C地址
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t setOrGetI2CAddress(pCmdPacktet_t pkt);
// /**
//  * @fn setOrGetTime
//  * @brief 设置或获取时间
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t setOrGetTime(pCmdPacktet_t pkt);
// /**
//  * @fn enableRecord
//  * @brief 开启记录
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t enableRecord(pCmdPacktet_t pkt);
// /**
//  * @fn disableRecord
//  * @brief 关闭记录
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t disableRecord(pCmdPacktet_t pkt);
// /**
//  * @fn oledScreenOn
//  * @brief oled屏幕开
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t oledScreenOn(pCmdPacktet_t pkt);
// /**
//  * @fn oledScreenOff
//  * @brief oled屏幕关
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t oledScreenOff(pCmdPacktet_t pkt);

// /**
//  * @fn getSensorDataName
//  * @brief 获取传感器数据名
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t getSensorDataName(pCmdPacktet_t pkt);
// /**
//  * @fn getSensorDataValue
//  * @brief 获取传感器数据值
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t getSensorDataValue(pCmdPacktet_t pkt);
// /**
//  * @fn getSensorDataUnit
//  * @brief 获取传感器数据单位
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t getSensorDataUnit(pCmdPacktet_t pkt);
// /**
//  * @fn getInformation
//  * @brief 获取信息项
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t getInformation(pCmdPacktet_t pkt);
// /**
//  * @fn getSKU
//  * @brief 获取SKU列表
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t getSKU(pCmdPacktet_t pkt);
// /**
//  * @fn getValue0
//  * @brief 根据数据名获取对应的数据的值
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t getValue0(pCmdPacktet_t pkt);
// /**
//  * @fn getValue1
//  * @brief 根据数据名获取选中的接口对应的数据的值
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t getValue1(pCmdPacktet_t pkt);
// /**
//  * @fn getValue2
//  * @brief 根据数据名获取选中的接口上指定SKU对应的数据的值
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t getValue2(pCmdPacktet_t pkt);
// /**
//  * @fn getUnit0
//  * @brief 根据数据名获取对应的数据的单位
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t getUnit0(pCmdPacktet_t pkt);
// /**
//  * @fn getUnit1
//  * @brief 根据数据名获取选中的接口对应的数据的单位
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t getUnit1(pCmdPacktet_t pkt);
// /**
//  * @fn getUnit2
//  * @brief 根据数据名获取选中的接口上指定SKU对应的数据的单位
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t getUnit2(pCmdPacktet_t pkt);
// /**
//  * @fn reset
//  * @brief 复位
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern void reset(pCmdPacktet_t pkt);
// /**
//  * @fn getAnalogSKU
//  * @brief 获取Analog类型传感器的支持列表
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t getAnalogSKU(pCmdPacktet_t pkt);
// /**
//  * @fn getDigitalSKU
//  * @brief 获取Digital类型传感器的支持列表
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t getDigitalSKU(pCmdPacktet_t pkt);
// /**
//  * @fn getI2CSKU
//  * @brief 获取I2C类型传感器的支持列表
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t getI2CSKU(pCmdPacktet_t pkt);
// /**
//  * @fn getUARTSKU
//  * @brief 获取UART类型传感器的支持列表
//  * 
//  * @param pkt 指向pCmdPacktet_t命令包
//  * @return uint8_t 错误代码
//  */
// extern uint8_t getUARTSKU(pCmdPacktet_t pkt);
/**
 * @fn parseCmdPkt
 * @brief 解析命令包
 * 
 * @param pkt 指向pCmdPacktet_t命令包
 * @return uint8_t 错误代码
 */
extern void parseCmdPkt(pCmdPacktet_t pkt);

extern uint8_t  getTimestamp(pCmdPacktet_t pkt);
extern uint8_t  setOrGetRefreshRate(pCmdPacktet_t pkt);
extern uint8_t  getVersion(pCmdPacktet_t pkt);

extern uint8_t getData(pCmdPacktet_t pkt);
extern uint8_t getUint(pCmdPacktet_t pkt);
extern uint8_t getALLData(pCmdPacktet_t pkt);
extern uint8_t set_Time(pCmdPacktet_t pkt);
extern uint8_t getTime(pCmdPacktet_t pkt);
extern uint8_t resetData(pCmdPacktet_t pkt);

/**
 * @fn getAllDataName
 * @brief 获取指定一个或多个接口上连接的货号为sku传感器，属性名为key的传感器的数值
 * 
 * @param if_flag bit0: 1->表示选中A&D接口 bit1: 1->表示选中I2C&UART1接口 bit2: 1->表示选中I2C&UART2接口
 * @return String 返回指定一个或多个接口上连接的货号为sku传感器，属性名为key的传感器的数值
 */
extern String getAllValue(const char *key);
extern String getAllUint(const char *key);
extern String combinationAllValue(pCmdPacktet_t key);


#endif
