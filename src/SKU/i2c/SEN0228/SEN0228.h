/*!
 * @file SEN0228.h
 * @brief SEN0228 驱动声明
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-08-02
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 */
#ifndef SEN0228_H
#define SEN0228_H
#include "Arduino.h"
#include "../../../global.h"
#include "DFRobot_VEML7700.h"
/**
 * @fn SEN0228_SETUP
 * @brief SEN0228 传感器初始化
 * 
 * @param ifn 接口号
 * @n     1   I2C&UART1
 * @n     2   I2C&UART2
 * @param address I2C地址
 * @return struct sensorAttr* 
 */
extern struct sensorAttr* SEN0228_SETUP(uint8_t ifn, uint8_t address);
/**
 * @fn SEN0228_FUN
 * @brief SEN0228 传感器采集数据
 * 
 * @param ifn 接口号
 * @n     1   I2C&UART1
 * @n     2   I2C&UART2
 * @param head 存储采集数据的指针
 */
extern void SEN0228_FUN(uint8_t ifn, struct keyValue* head);

#endif

