#include "Slave.h"

#define RECV_BUFFER_SIZE  4096
#define SEND_BUFFER_SIZE  4096

#define TWIRE  Wire

uint16_t recvIndex = 0;
pCmdPacktet_t  recvBufferPtr = NULL;
pSendPacktet_t sendBufferPtr = NULL;
extern sGeneral_t *pGeneral;
 
uint64_t firstTime = 0;

void i2cSlaveInit(void){
  recvBufferPtr = i2cCuappEnqueue(RECV_BUFFER_SIZE);
  sendBufferPtr = i2cSendCuappEnqueue(SEND_BUFFER_SIZE);
  TWIRE.setSDA(0);
  TWIRE.setSCL(1);
  TWIRE.begin(0x42);
  
  TWIRE.setClock(100000);
  //注册一个I2C接收事件，如果I2C从机接收到主机发来的数据，则调用receiveEvent接收数据
  TWIRE.onReceive(receiveEvent);
  //注册一个I2C发送事件，如果I2C从机接收到主机发来数据请求，则调用sendEvent发送数据
  TWIRE.onRequest(sendEvent);
  
}

void initSerialSlave(void)
{
  recvBufferPtr = i2cCuappEnqueue(RECV_BUFFER_SIZE);
  sendBufferPtr = i2cSendCuappEnqueue(SEND_BUFFER_SIZE);
  Serial1.setRX(1);
  Serial1.setTX(0);
  Serial1.begin(115200);
}

void i2c_slave_end(){
  TWIRE.end();
}

uint32_t recvT = 0; //记录包的接收时间
uint32_t pktT  = 0;

void receiveEvent(int len){
  clearRecvBuf();
  while(len){
    switch(recvIndex){
      case 0:{
        recvT = millis();
        recvBufferPtr->flag  = false;
        recvBufferPtr->len   = 0;
        recvBufferPtr->index = 0;
        recvBufferPtr->cmd   = TWIRE.read();
        recvIndex += 1;
        len -= 1;}       
      break;
      case 1:{
        recvBufferPtr->len = TWIRE.read();
        recvIndex += 1;
        len -= 1;}
      break;
      case 2:
        recvBufferPtr->len |= (TWIRE.read() << 8);
        recvIndex += 1;
        len -= 1;
        if(recvBufferPtr->len > 4096){
          recvIndex = 0;
          recvBufferPtr->len = 0;}
        if(recvBufferPtr->index == recvBufferPtr->len){
          recvBufferPtr->flag = true;
          recvIndex = 0;}
      break;
      default:
        if(recvBufferPtr->index < recvBufferPtr->len){
          recvBufferPtr->data[recvBufferPtr->index] = TWIRE.read();
          recvBufferPtr->index++;
        }else{
            TWIRE.read();}
        recvIndex += 1;
        len -= 1;
        if(recvBufferPtr->index == recvBufferPtr->len){
          recvBufferPtr->flag = true;
          recvIndex = 0;}
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
      }
      return;
    } 
  }
  noInterrupts();
  TWIRE.write(0xFF);
  interrupts();
 
}

void i2cloop(){
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
    I2C_SLAVE_DBG("pkt is null or len is zero");
    return;
  }
  I2C_SLAVE_DBG(pkt->cmd);
  I2C_SLAVE_DBG(pkt->len);
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
    case CMD_RADIUS_DATA:
      errCode = setRadius(pkt);
    break;
    case CMD_SPEED1_DATA:
      errCode =setStandaraWindSpeed1(pkt);
    break;
    case CMD_SPEED2_DATA:
      errCode =setStandaraWindSpeed2(pkt);
    break;
    case CMD_CALIBRATOR:
      errCode =calibrator(pkt);
    break;
    case CMD_PROJECT:
      errCode = project(pkt);
    break;
    default:
      I2C_SLAVE_DBG("Invaild command: ");
      I2C_SLAVE_DBG(pkt->cmd, HEX);
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

void uartloop(void)
{
  if(Serial1.available()>0){
    firstTime = millis();
    while(1){
      switch(recvIndex){
        case 0:
        {
          //memset(sendBufferPtr->data, 0, SEND_BUFFER_SIZE);
          recvBufferPtr->flag  = false;
          recvBufferPtr->len   = 0;
          recvBufferPtr->index = 0;
          sendBufferPtr->flag  = false;
          //sendBufferPtr->total = 0;
          //sendBufferPtr->index = 0;
          if(Serial1.available()>0){
            firstTime = millis();
            recvBufferPtr->cmd = Serial1.read();
            recvIndex += 1;
          }
        }       
        break;
        case 1:
        {
          if(Serial1.available()>0){
            firstTime = millis();
            recvBufferPtr->len = Serial1.read();
            recvIndex += 1;
          }
        }
        break;
        case 2:
          if(Serial1.available()>0){
            firstTime = millis();
            recvBufferPtr->len |= Serial1.read();
            recvIndex += 1;
          }
          if(recvBufferPtr->len > 4096){
            recvIndex = 0;
            recvBufferPtr->len = 0;
          }
          if(recvBufferPtr->index == recvBufferPtr->len){
            parseCmdPkt(recvBufferPtr);
            recvIndex = 0;
          }
        break;
        default:
          if(recvBufferPtr->index < recvBufferPtr->len){
            if(Serial1.available()>0){
              firstTime = millis();
              recvBufferPtr->data[recvBufferPtr->index] = Serial1.read();
              recvBufferPtr->index++;
            }
          }else{
            if(Serial1.available()>0){
              firstTime = millis();
              Serial1.read();
            }
          }
          recvIndex += 1;
          if(recvBufferPtr->index == recvBufferPtr->len){
            parseCmdPkt(recvBufferPtr);
            recvIndex = 0;
          }
        break;
      }
      if((millis() - firstTime)>1000){
        recvIndex = 0;
        return;
      }
      delay(1);
    }
  }
}


