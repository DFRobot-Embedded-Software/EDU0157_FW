/**
 * @file DFRobot_PCF8563T.h
 * @brief 实时时钟方法
 *
 * @copyright   Copyright (c) 2022 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-08-02
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 * 
 */
#ifndef DFROBOT_PCF8563T_H
#define DFROBOT_PCF8563T_H

#include <Wire.h>
#include <Arduino.h>
#include "general.h"
#include "./SKU/software_iic.h"


/**
 * @struct sPCF8563TTime_t
 * @brief 时钟结构体 
 */
typedef struct{
    uint8_t second; ///< 秒，  范围0~59
    uint8_t minute; ///< 分钟，范围0~59
    uint8_t hour;   ///< 小时，范围0~23
    uint8_t day;    ///< 天，  范围1~31
    uint8_t week;   ///< 周，  范围0~6
    uint8_t month;  ///< 月，  范围1~12
    uint16_t year;  ///< 年，  大于2000
}sPCF8563TTime_t;


class DFRobot_PCF8563T{
public:
  /**
   * @fn DFRobot_PCF8563T
   * @brief Construct a new dfrobot pcf8563t object
   * 
   * @param pWire 指向TwoWire类指针
   * @param addr  时钟模块的I2C地址，为0x51
   */
  //DFRobot_PCF8563T(SoftwareTwoWire *pWire = &SOF_WIRE1, uint8_t addr = 0x51);
  DFRobot_PCF8563T(SoftwareTwoWire *pWire , uint8_t addr );
  //DFRobot_PCF8563T(TwoWire *pWire , uint8_t addr);
  /**
   * @fn begin
   * @brief 设置电脑的时间到时钟模块
   * 
   * @param date 日期
   * @param time 时间
   */
  void begin(const __FlashStringHelper* date, const __FlashStringHelper* time);
  /**
   * @fn begin
   * @brief 设置时钟模块的初始时间
   * 
   * @param year   年，  大于2000
   * @param month  月，  范围1~12
   * @param day    天，  范围1~31
   * @param week   周，  范围0~6
   * @param hour   小时，范围0~23
   * @param minute 分钟，范围0~59
   * @param second 秒，  范围0~59
   */
  void begin(uint16_t year,uint8_t month,uint8_t day,uint8_t week,uint8_t hour,uint8_t minute,uint8_t second);
  /**
   * @fn setTime
   * @brief 设置时钟模块的初始时间
   * 
   * @param t 指向时钟结构体sPCF8563TTime_t对象
   */
  void setTime(sPCF8563TTime_t t);
  /**
   * @fn getTime
   * @brief 获取时间
   * 
   * @return sPCF8563TTime_t 返回时间结构体对象
   */
  sPCF8563TTime_t getTime();
  /**
   * @fn dayOfTheWeek
   * @brief 根据年/月/日计算星期数
   * 
   * @param year   年
   * @param month  月
   * @param day    日
   * @return uint8_t 0~6星期数
   * @n 0   星期天
   * @n 1   星期一
   * @n 2   星期二
   * @n 3   星期三
   * @n 4   星期四
   * @n 5   星期五
   * @n 6   星期六
   */
  uint8_t dayOfTheWeek(uint16_t year, uint8_t month, uint8_t day);

  void rtc_setup(void);
  sPCF8563TTime_t get_rtc(void);
  String getHourMinuteSecond(void);
  String convertHourMinuteSecond(sPCF8563TTime_t t);
  char* convertDataFileName(sPCF8563TTime_t t);
  void setCountDown(void);
  void cleanState(void);
protected:
  /**
   * @fn writeReg
   * @brief 定位寄存器
   * 
   * @param reg 寄存器地址
   */
  void writeReg(uint8_t reg);
  /**
   * @fn readReg
   * @brief 写入数据
   * 
   * @param pdata 指向要写入的数据的缓存
   * @param len 要写的字节的长度
   */
  void writeData(void *pdata, int16_t len);

  void writeData(uint8_t reg, void *pdata, int16_t len);
  /**
   * @fn readReg
   * @brief 读取数据
   * 
   * @param pdata 指向存储数据的缓存
   * @param len 要读取的字节的长度
   * @return int16_t 实际读取的字节的长度
   */
  int16_t readReg(void *pdata, int16_t len);
  /**
   * @fn conv2d
   * @brief 将字符转换为2位10进制数据
   * 
   * @param p 指向数组的指针
   * @return uint8_t 返回2位十进制数据
   */
  uint8_t conv2d(const char* p);
  
  /**
   * @fn date2days
   * @brief 计算当前年月日距2000年1月1日相差的天数
   * 
   * @param year   年
   * @param month  月
   * @param day    日
   * @return uint16_t 天数，当前年月日距离2000年1月1日的天数
   * @note 只能保证2000年到2099年的计算是准确的
   */
  uint16_t date2days(uint16_t year, uint8_t month, uint8_t day);
  /**
   * @fn decToBCD
   * @brief 将十进制数据转为BCD码表示
   * 
   * @param num   十进制数据
   * @return uint8_t 返回十进制数据的BCD码表示
   */
  uint8_t decToBCD(uint8_t num);

private:
  SoftwareTwoWire *_pWire;
  TwoWire *_pWire1;
  uint8_t _addr;
  sPCF8563TTime_t _t;
};
extern DFRobot_PCF8563T PCF8563T_1;
//extern DFRobot_PCF8563T PCF8563T_2;

#endif
