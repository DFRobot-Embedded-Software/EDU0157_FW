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

/**
 * @struct sQueueData
 * @brief 用来组织和索引显示页面
 */
struct sQueueData{
    struct sQueueData *before; ///< 指向前一个页面
    struct sQueueData *next;   ///< 指向下一个页面
    uint8_t ok;                ///< 当前页面索引，大于或等于0，0表示初始页
    uint8_t down;              ///< 当前页面内容索引，大于或等于0，每创造一个初始页面，其值默认为0
    uint8_t back;              ///< 返回前一个页面，在初始页面时，该值还可以用来控制csv文件的记录
    uint8_t downNum;           ///< 当前页面内容索引的最大值，用来控制变量 down 的最大值
    uint8_t isLastPage;        ///< 判定是否为最后一页，大于0：最后一页, 等于0: 当前页不是最后一页，用来控制变量 ok 的最大值
};

//记录页面的个数
extern uint32_t queuedata;
/**
 * @fn cuappEnqueue
 * @brief 增加一个页面
 * 
 * @param ok          新增加页面的页面索引，大于或等于0，初始页面为0
 * @param down        新增加页面内容的初始索引值，默认为0
 * @param back        当前页面的索引值
 * @param down_num    新增加页面内容索引个数
 * @param last_page   新增加页面是否是最后一页
 */
extern void cuappEnqueue(uint8_t ok, uint8_t down, uint8_t back, uint8_t down_num, uint8_t last_page);

/**
 * @fn cuappQueueTail
 * @brief 获取链表中最后一页，并脱离链表
 * 
 * @return struct sQueueData* 返回最后一页的指针对象
 */
extern struct sQueueData *cuappQueueTail( void );
/**
 * @fn getQueueTail
 * @brief 获取链表中最后一页
 * 
 * @return struct sQueueData* 返回最后一页的指针对象
 */
extern struct sQueueData *getQueueTail(void);
/**
 * @fn getQueueHead
 * @brief 获取初始页
 * 
 * @return struct sQueueData* 返回初始页的指针对象
 */
extern struct sQueueData *getQueueHead(void);

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
extern pCmdPacktet_t i2cCuappEnqueue(uint16_t len);
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
extern pSendPacktet_t i2cSendCuappEnqueue(uint16_t len);
//extern pSendPacktet_t i2cSendCuappDequeue( void );
//extern pSendPacktet_t i2cSendCuappQueueTail( void );
extern pSendPacktet_t i2cSendGetQueueHead(void);


#endif
