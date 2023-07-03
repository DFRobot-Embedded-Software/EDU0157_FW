#ifndef DFROBOT_QUEUE_H_
#define DFROBOT_QUEUE_H_
/*!
 * @file DFRobot_queue.h
 * @brief 页面索引队列及I2C从机数据出队入队定义
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-08-02
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 */

#include "stdint.h"
#include "string.h"
#include "stdlib.h"


// 以下函数是否并未被使用
struct sCmdPacktet{
    //struct sCmdPacktet *next;  /**< 指向下一个结构体*/
    //struct sCmdPacktet *before;/**< 指向上一个结构体*/
    uint8_t flag;              /**< 判定一个包是否接收完成 true:接收完成*/
    uint16_t index;            /**< 命令包长度索引*/
    uint8_t cmd;               /**< 命令*/
    uint16_t len;              /**< 命令后数据的长度*/
    uint8_t data[0];           /**< 0长度数组，存储数据*/
};
typedef struct sCmdPacktet sCmdPacktet_t;
typedef struct sCmdPacktet *pCmdPacktet_t;

//extern uint32_t i2cqueuedata;
/**
 * @fn i2cCuappEnqueue
 * @brief I2C接收buf malloc
 * 
 * @param len 数据长度
 * @return pCmdPacktet_t 返回
 */
pCmdPacktet_t i2cCuappEnqueue(uint16_t len);
//extern pCmdPacktet_t i2cCuappDequeue( void );
//extern pCmdPacktet_t i2cCuappQueueTail( void );
//extern pCmdPacktet_t i2cGetQueueHead(void);

struct sSendPacktet{
    //struct sSendPacktet *next;
    uint8_t flag;   /**< 判定一个反应包是否发送完成 true:可以发送*/
    uint16_t total; /**<总长度*/
    uint16_t index;///< 命令包长度索引
    uint8_t data[0];/**< 0长度数组，存储数据*/
};

typedef struct sSendPacktet sSendPacktet_t;
typedef struct sSendPacktet *pSendPacktet_t;
//extern uint32_t i2csendqueue;
/**
 * @fn i2cSendCuappEnqueue
 * @brief I2C发送buf malloc
 * 
 * @param len 数据长度
 * @return pSendPacktet_t 返回
 */
pSendPacktet_t i2cSendCuappEnqueue(uint16_t len);
//extern pSendPacktet_t i2cSendCuappDequeue( void );
//extern pSendPacktet_t i2cSendCuappQueueTail( void );
pSendPacktet_t i2cSendGetQueueHead(void);


#endif
