/**
 * @file udisk.h
 * @author your name (you@domain.com)
 * @brief 创建U盘
 * @version 0.1
 * @date 2023-03-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef UDISK_H
#define UDISK_H
#include "Arduino.h"
#include "string.h"
#include "Adafruit_TinyUSB.h"
#include "SPI.h"
#include "SdFat.h"
#include "Adafruit_SPIFlash.h"
#include "general.h"
#include "configFile.h"

#ifdef UDISK_DBG_ENABLE
#define UDISK_DBG(...) {Serial1.print("udisk ");Serial1.print("[");Serial1.print(__FUNCTION__); Serial1.print("(): "); Serial1.print(__LINE__); Serial1.print(" ] "); Serial1.println(__VA_ARGS__);}
#else
#define UDISK_DBG(...)
#endif

/**
 * @brief 初始化U盘
 * 
 */
  void initUdisk(void);
  /**
   * @brief 设置U盘时间同步
   * 
   * @param date 
   * @param time 
   */
  void dateTime(uint16_t* date, uint16_t* time);

/**
 * @fn  msc_read_cb
 * @brief 读flash
 * 
 * @param lba       地址
 * @param buffer    存储读取数据的缓存
 * @param bufsize   读取字节的大小
 * @return int32_t 返回实际读取的字节
 */
int32_t msc_read_cb (uint32_t lba, void* buffer, uint32_t bufsize);
/**
 * @fn  msc_write_cb
 * @brief 写flash
 * 
 * @param lba       地址
 * @param buffer    指向写数据缓存
 * @param bufsize   写字节的大小
 * @return int32_t 返回实际写入的字节
 */
int32_t msc_write_cb (uint32_t lba, uint8_t* buffer, uint32_t bufsize);
/**
 * @fn msc_flush_cb
 * @brief 同步
 * 
 */
void msc_flush_cb (void);

/**
 * @brief 检测USB连接状体
 * 
 */
void readUSBState(void);

#endif 