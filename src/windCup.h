#ifndef _WINDCUP_H_
#define _WINDCUP_H_
#include <EEPROM.h>
#include "general.h"
#define WIND_HEAD_ADDR  0x02
#define WIND_CUP_H      0x03
#define WIND_CUP_L      0x04
#define OFFSET_H        0x05
#define OFFSET_L        0x06
#define LINEARFACTOR_H    0x07
#define LINEARFACTOR_L    0x08
#define STAND1_H          0x09
#define STAND1_L          0x0A
#define ATPRESENT_H       0X0B
#define ATPRESENT_L       0x0C
/**
 * @brief  初始化eeprom
 * 
 */
void initEeprom(void);
/**
 * @brief 将风杯半径存储到eeprom中
 * 
 * @param buf 需要存储的风杯数据
 */
void writeRadius(uint8_t *buf);
/**
 * @brief 将补偿和系数存储在eeprom中
 * 
 * @param offset 补偿数据
 * @param linearfactor 线性因子
 */
void writeData(float offset,float linearfactor);
/**
 * @brief 将标准风速1存储在eeprom中
 * 
 * @param buf 存储的数据
 */
void writeStand1(uint8_t *buf);
/**
 * @brief 将采集的风速1存在eeprom中
 * 
 * @param data 需要存储的数据
 */
void writeAtpresent(float data);
#endif
