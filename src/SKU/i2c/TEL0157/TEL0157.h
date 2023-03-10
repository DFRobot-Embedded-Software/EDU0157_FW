/*!
 * @file TEL0157.cpp
 * @brief TEL0157 驱动定义
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-08-02
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 */
#ifndef TEL0157_H
#define TEL0157_H
#include "Arduino.h"
#include "../../../global.h"
#include "DFRobot_GNSS.h"

/**
 * @fn TEL0157_SETUP
 * @brief TEL0157 传感器初始化
 * 
 * @param ifn 接口号
 * @n     1   I2C&UART1
 * @n     2   I2C&UART2
 * @param address I2C地址
 * @return struct sensorAttr* 
 */
extern struct sensorAttr* TEL0157_SETUP(uint8_t ifn, uint8_t address);
/**
 * @fn TEL0157_FUN
 * @brief TEL0157 传感器采集数据
 * 
 * @param ifn 接口号
 * @n     1   I2C&UART1
 * @n     2   I2C&UART2
 * @param head 存储采集数据的指针
 */
extern void TEL0157_FUN(uint8_t ifn, struct keyValue* head);

#endif

