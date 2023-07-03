/**
 * @file power.h
 * @author your name (you@domain.com)
 * @brief 配置板载传感器电源和中断配置
 * @version 0.1
 * @date 2023-04-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef POWER_H
#define POWER_H
#include "Arduino.h"
#include "general.h"

#ifdef POWER_DBG_ENABLE
#define POWER_DBG(...) {Serial.print("power ");Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define POWER_DBG(...)
#endif
/**
 * @brief 打开板载电源
 * 
 */
void powerON(void);
/**
 * @brief 关闭板载传感器电源
 * 
 */
void powerOFF(void);
/**
 * @brief 初始化引脚和中断
 * 
 */
void initPin(void);
/**
 * @brief 风速的中断处理函数
 * 
 */
void windSpeed(void);
/**
 * @brief 倒计时中断处理
 * 
 */
void readData(void);
/**
 * @brief 中断配置
 * 
 */
void interruptConfig(void);
/**
 * @brief 获取向
 * 
 * @return String 
 */
void getCompassData(void);
void getCompassData1(void);
/**
 * @brief 风向中断处理函数
 * 
 */
void A(void);
void B(void);
void C(void);
void D(void);

#endif

