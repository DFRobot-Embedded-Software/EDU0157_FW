/**
 * @file general.h
 * @author your name (you@domain.com)
 * @brief 通用的功能
 * @version 0.1
 * @date 2023-03-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef GENERAL_H
#define GENERAL_H
#include "udisk.h"
#include "configFile.h"
#include "RGBLight.h"
#include "Slave.h"
#include "power.h"
#include "./SKU/sensor_i2c.h"
#include "DFRobot_PCF8563T.h"
#include "hardware/gpio.h"
#include "hardware/xosc.h"
#include "hardware/pll.h"
#include "hardware/regs/clocks.h"
#include "hardware/clocks.h"
#include "DFRobot_Obloq_I2C.h"
#include "hardware/resets.h"
#include <pico/stdlib.h>
#include "windCup.h"


#include "Wire.h"
//所有打印开关
//#define UDISK_DBG_ENABLE  
//#define SENSOR_I2C_DBG_ENABLE
//#define CONFIG_DBG_ENABLE
//#define ENABLE_I2C_SLAVE_DBG
//#define ENABLE_PARSE_DBG
//#define POWER_DBG_ENABLE

//通信模式
#define I2C  0
#define UART 1

//开关
#define ON    1
#define OFF   0 

//采样频率单位
#define SECOND    0
#define MINUTE    1

//I2C接口
#define SOFI2C  1
#define HARI2C  2

//DTU
#define WIFI  1

#define EASYIOT 1
#define SIOT_V1 2
#define SIOT_V2 3



struct sDTUData{
  char* key;//需要传输的数据名称
  char* topic;//接口
  struct sDTUData* next;
};




typedef struct{
  uint8_t sFlashWrite;//写flash状态
  uint8_t sUsbState;//USB连接状态
  uint8_t sCommunicationMode;//通信模式
  eColor_t sRGBColor;//RGB显示的颜色
  uint8_t sStandardConfigFileState;//标准配置文件配置状态
  float sRadial;//风杯半径
  uint8_t sRecord;//存储状态
  uint8_t sLightSwitch;//RGB灯开关
  uint8_t sLowPower;//低功耗控制
  uint32_t sDelayRecord;//延时存储
  uint8_t sSampleRate;//采样频率
  uint8_t sUnit;//采样频率单位
  uint8_t configState;//config文件解析状态
  uint8_t DTUState;//DTU文件解析状态
  uint32_t interrcount;//风速中断计算
  uint8_t timeConfig;//通过config配置时间
  uint8_t saveStateTime;//延时存储
  uint8_t sReadData;//读取数据
  //DTU
  uint8_t sDTUSwitch;//DTU模块开关
  uint8_t sDTUMethod;
  uint8_t sDTULora_ID;
  char* sDTUWIFISSID;
  char* sDTUWIFIPASSWORD;
  uint8_t sDTUPlatform;//模式选择
  char* sDTUIP;//
  uint16_t sDTUPort;
  char* sDTUID;
  char* sDTUPWD;
  char* sDTUSave;
  uint8_t sDtuState;
  uint8_t sDTUConnectState;//WiFi连接成功中途断网状态标志位

  float sStandWindSpeed1;//标准风速1
  float sAtPresentSpeed1;//当前风速1
  float sStandWindSpeed2;//标准风速2
  float sAtPresentSpeed2;//当前风速2
  float sOffset;//偏移量;
  float sLinearFactor;//线性因子
  uint8_t sWindFlage;
}sGeneral_t;

/**
 * @brief 初始化主板
 * 
 */
void initBoard(void);

/**
 * @brief 初始化通用结构体中的内容
 * 
 */
void initStruct(void);

/**
 * @brief 检测USB连接状体
 * 
 */
extern void readUSBState(void);

/**
 * @brief 根据解析的配置文件配置配置主板
 * 
 */
void configBoard(void);

/**
 * @brief 设置RGB灯颜色
 * 
 * @param color RGB灯颜色
 */
extern void setRGBColor(eColor_t color);

/**
 * @brief 获取文件名称
 * 
 * @return char* 
 */
char* getFileName(void);

/**
 * @brief 向服务发送数据
 * 
 */
void publishData(void);

/**
 * @brief 进行WiFi连接和其他DTU功能
 * 
 */
void wifi(void);

/**
 * @brief 设别从低功耗退出后进行的一些操作
 * 
 */
void initPeripheral(void);

/**
 * @brief RGB灯状态
 * 
 * @param location  0 CSV位置；1 DTU位置
 */
void RGBStart(uint8_t location);
/**
 * @brief RGB灯状态
 * 
 * @param location  0 CSV位置；1 DTU位置
 */
void RGBEnd(uint8_t location);
/**
 * @brief RGB标准状态
 */
void RGBFundamental(void);

#endif