/*!
 * @file SEN0514.h
 * @brief SEN0514 驱动声明
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-08-02
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 */
#ifndef SEN0514_H
#define SEN0514_H
#include "Arduino.h"
#include "../../../global.h"
#include "DFRobot_ENS160.h"

/**
 * @fn SEN0514_SETUP
 * @brief SEN0514 传感器初始化
 * 
 * @param ifn 接口号
 * @n     1   I2C&UART1
 * @n     2   I2C&UART2
 * @param address I2C地址
 * @return struct sensorAttr* 
 */
extern struct sensorAttr* SEN0514_SETUP(uint8_t ifn, uint8_t address);
/**
 * @fn SEN0514_FUN
 * @brief SEN0514 传感器采集数据
 * 
 * @param ifn 接口号
 * @n     1   I2C&UART1
 * @n     2   I2C&UART2
 * @param head 存储采集数据的指针
 */
extern void SEN0514_FUN(uint8_t ifn, struct keyValue* head);

#endif

