#ifndef SEN0414_H
#define SEN0414_H


#include "Arduino.h"
#include "../../sensor_i2c.h"
#include "DFRobot_SHTC3.h"
/**
 * @fn SEN0529_SETUP
 * @brief SEN0529 传感器初始化
 * 
 * @param ifn 接口号
 * @n     1   I2C&UART1
 * @param address I2C地址
 * @return struct sensorAttr* 
 */
extern struct sensorAttr* SEN0414_SETUP(uint8_t ifn, uint8_t address);
/**
 * @fn SEN0529_FUN
 * @brief SEN0529 传感器采集数据
 * 
 * @param ifn 接口号
 * @n     1   I2C&UART1
 * @n     2   I2C&UART2
 * @param head 存储采集数据的指针
 */
extern void SEN0414_FUN(uint8_t ifn, struct keyValue* head);



#endif