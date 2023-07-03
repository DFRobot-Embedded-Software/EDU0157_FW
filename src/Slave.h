#ifndef SLAVE_H
#define SLAVE_H

#include "Arduino.h"
#include "Wire.h"
#include "DFRobot_Parse.h"
#include "general.h"

// #ifdef __cplusplus
// extern "C"{
// #endif
#include "DFRobot_queue.h"

// #ifdef __cplusplus
// } // extern "C"
// #endif

#ifdef ENABLE_I2C_SLAVE_DBG
#define I2C_SLAVE_DBG(...) {Serial.print("i2c_slave ");Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define I2C_SLAVE_DBG(...)
#endif



 /**
   * @brief 初始化I2C
   */
  void i2cSlaveInit(void);

  /**
 * @brief 初始化串口从机
 * 
 */
  void initSerialSlave(void);
  
  void receiveEvent(int len);
  void sendEvent(void);
/**
 * @fn parseCmdPkt
 * @brief 解析命令包
 * 
 * @param pkt 指向pCmdPacktet_t命令包
 * @return uint8_t 错误代码
 */
  void parseCmdPkt(pCmdPacktet_t pkt);
  
  void clearRecvBuf();

 /**
   * @brief I2C 从机接收数据
   */
  void i2cloop();

  /**
   * @brief UART 从机接收数据
   */
  void uartloop(void);



#endif
