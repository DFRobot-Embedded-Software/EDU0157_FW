/*!
 * @file SEN0334.cpp
 * @brief SEN0334 驱动定义
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-08-02
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 */
#ifndef SEN0334_H
#define SEN0334_H
#include "Arduino.h"
#include "../../sensor_i2c.h"
#include "DFRobot_SHT3x.h"
/**
 * @fn SEN0334_SETUP
 * @brief SEN0334 传感器初始化
 * 
 * @param ifn 接口号
 * @n     1   I2C&UART1
 * @n     2   I2C&UART2
 * @param address I2C地址
 * @return struct sensorAttr* 
 */
extern struct sensorAttr* SEN0334_SETUP(uint8_t ifn, uint8_t address);
/**
 * @fn SEN0334_FUN
 * @brief SEN0334 传感器采集数据
 * 
 * @param ifn 接口号
 * @n     1   I2C&UART1
 * @n     2   I2C&UART2
 * @param head 存储采集数据的指针
 */
extern void SEN0334_FUN(uint8_t ifn, struct keyValue* head);

#endif

