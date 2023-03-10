/**
 * @file i2c_slave.cpp
 * @brief i2c从机通信
 *
 * @copyright   Copyright (c) 2022 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-08-02
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 * 
 */
#include "Arduino.h"
#include "Wire.h"
#include "global.h"
#include "DFRobot_Parse.h"
extern struct InputRegList sInputReg;
extern struct HoldingRegList sHoldingReg;
extern DFRobot_PCF8563T rtc;
extern sGeneral_t *pGeneral_t;

// #ifdef ENABLE_I2C_SLAVE_DBG
// #define I2C_SLAVE_DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
// #else
// #define I2C_SLAVE_DBG(...)
// #endif

#define TWIRE  Wire

//uint8_t sendBufferAche[256];
uint8_t recvBuffer[50];
uint8_t sendIndex = 0;
uint8_t sendTotal = 0;

#define RECV_BUFFER_SIZE  4096
#define SEND_BUFFER_SIZE  4096

uint16_t recvIndex = 0;
uint16_t dataLength = 0;
extern uint8_t readState;
uint16_t checkSumData = 0;
uint8_t retState = 0;

uint8_t returnBuffer[120];
void receiveEvent(int len);
void sendEvent(void);
void parseCmdPkt(void);
void returnInput(void);
void returnHolding(void);

#define RECV_BUFFER_SIZE  4096
#define SEND_BUFFER_SIZE  4096
pCmdPacktet_t  recvBufferPtr = NULL;
pSendPacktet_t sendBufferPtr = NULL;

void i2cSlaveInit(void){
  recvBufferPtr = i2cCuappEnqueue(RECV_BUFFER_SIZE);
  sendBufferPtr = i2cSendCuappEnqueue(SEND_BUFFER_SIZE);
  TWIRE.setSDA(0);
  TWIRE.setSCL(1);
  TWIRE.begin(0x42);
  //TWIRE.setClock(400000);
  //注册一个I2C接收事件，如果I2C从机接收到主机发来的数据，则调用receiveEvent接收数据
  TWIRE.onReceive(receiveEvent);
  //注册一个I2C发送事件，如果I2C从机接收到主机发来数据请求，则调用sendEvent发送数据
  TWIRE.onRequest(sendEvent);
  
}

void i2c_slave_end(){
  TWIRE.end();
}



uint32_t recvT = 0; //记录包的接收时间
uint32_t pktT  = 0;

void receiveEvent(int len){
  // uint8_t *buf = (uint8_t*)malloc(len*sizeof(uint8_t));
  // for(uint8_t i = 0;i < len;i ++){
  //   buf[i] = TWIRE.read();
  // }
  // memcpy((uint8_t*)&recvBufferPtr+3,buf,len);
  // free(buf);
  //  recvBufferPtr->flag = true;
  //GLOBAL_DBG("recvIndex=");GLOBAL_DBG(recvIndex); GLOBAL_DBG(",howMany=");GLOBAL_DBG(len);
  clearRecvBuf();
    while(len){
        switch(recvIndex){
            case 0:
            {
                 recvT = millis();
                 //memset(sendBufferPtr->data, 0, SEND_BUFFER_SIZE);
                 recvBufferPtr->flag  = false;
                 recvBufferPtr->len   = 0;
                 recvBufferPtr->index = 0;
                 //sendBufferPtr->flag  = false;
                 //sendBufferPtr->total = 0;
                 //sendBufferPtr->index = 0;
                 recvBufferPtr->cmd   = TWIRE.read();
                 recvIndex += 1;
                 len -= 1;
            }       
                   break;
            case 1:
            {
                 recvBufferPtr->len = TWIRE.read();
                 recvIndex += 1;
                 len -= 1;
            }
                   break;
            case 2:
                   recvBufferPtr->len |= (TWIRE.read() << 8);
                   recvIndex += 1;
                   len -= 1;
                   if(recvBufferPtr->len > 4096){
                        recvIndex = 0;
                        recvBufferPtr->len = 0;
                   }
                   if(recvBufferPtr->index == recvBufferPtr->len){
                       recvBufferPtr->flag = true;
                       recvIndex = 0;
                   }
                   break;
            default:
                   if(recvBufferPtr->index < recvBufferPtr->len){
                      recvBufferPtr->data[recvBufferPtr->index] = TWIRE.read();
                      recvBufferPtr->index++;
                   }else{
                      TWIRE.read();
                   }
                   recvIndex += 1;
                   len -= 1;
                   if(recvBufferPtr->index == recvBufferPtr->len){
                      recvBufferPtr->flag = true;
                      recvIndex = 0;
                   }
                   break;
        }
    }

}

void sendEvent(void){
  if(sendBufferPtr->flag){
    if(sendBufferPtr->index < sendBufferPtr->total){
      noInterrupts();
      TWIRE.write(sendBufferPtr->data[sendBufferPtr->index]);
      interrupts();
      sendBufferPtr->index += 1;
      if(sendBufferPtr->index == sendBufferPtr->total){
        sendBufferPtr->flag = false;
        GLOBAL_DBG(sendBufferPtr->total,HEX);
      }
      return;
    } 
  }
  noInterrupts();
  TWIRE.write(0xFF);
  interrupts();
}


void i2cloop(){
  Serial.println(millis());
  if(recvBufferPtr && recvBufferPtr->flag){
    recvBufferPtr->flag = false;
    parseCmdPkt(recvBufferPtr);
  }
}


void clearRecvBuf(){
  if(millis() - recvT > 10){
    recvIndex = 0;
  }
  recvT = millis();
}

void parseCmdPkt(pCmdPacktet_t pkt){
  uint8_t errCode = ERR_CODE_NONE;
  if(pkt == NULL){
    GLOBAL_DBG("pkt is null or len is zero");
    return;
  }
  GLOBAL_DBG(pkt->cmd);
  GLOBAL_DBG(pkt->len);
  switch(pkt->cmd){
    case CMD_GET_DATA:
      errCode = getData(pkt);
      break;
    case CMD_GET_ALL_DATA:
      errCode = getALLData(pkt);
      break;
    case CMD_SET_TIME:
      errCode = set_Time(pkt);
      break;
    case CMD_GET_VERSION:
      errCode = getVersion(pkt);
      break;
    case CMD_GET_UNIT:
      errCode = getUint(pkt);
      break;
    case CME_GET_TIME:
      errCode = getTime(pkt);
      break;
    case CMD_RESET_DATA:
      errCode = resetData(pkt);
      break;
    default:
      GLOBAL_DBG("Invaild command: ");
      GLOBAL_DBG(pkt->cmd, HEX);
      errCode = ERR_CODE_CMD_INVAILED;
      break;
    }
    if(errCode != ERR_CODE_NONE){
      uint8_t sendData[RESPONSE_ERR_PKT_LEN];
      memset(sendData, 0, sizeof(sendData));
      sendData[RESPONSE_STATUS]   = STATUS_FAILED;
      sendData[RESPONSE_CMD]      = pkt->cmd;
      sendData[RESPONSE_LEN_L]    = 1;
      sendData[RESPONSE_LEN_H]    = 0;
      sendData[RESPONSE_ERR_CODE] = errCode;
      DFRobot_Response(sendData, sizeof(sendData));
    }
}

void DFRobot_Response(uint8_t *data, uint8_t len){
  (pGeneral_t->communicationMode == I2CMODE)? i2cSend(data, len):uartSend(data,len);
}

void i2cSend(uint8_t *data, uint16_t len){
  if(sendBufferPtr->flag){
    sendBufferPtr->flag = false;
  }

  if(len <= 4096){
    memset(sendBufferPtr->data, 0, SEND_BUFFER_SIZE);
    memcpy(sendBufferPtr->data, data, len);
    sendBufferPtr->index = 0;
    sendBufferPtr->total = len;
    sendBufferPtr->flag = true;
  }
}

void uartSend(uint8_t *data, uint16_t len){
  uint8_t* pBuf = data;
  int f =0;
  for(int i = 0; i < len; i++){
    Serial1.write(pBuf[i]);
    f++;
  }
    GLOBAL_DBG(f);
}