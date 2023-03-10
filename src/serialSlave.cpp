#include "global.h"
#include "string.h"
#include "DFRobot_Parse.h"
uint32_t t = 0;
uint8_t buffer[20];
uint8_t dataLen = 0;
uint8_t dataff = 0;
#define RECV_BUFFER_SIZE  4096
#define SEND_BUFFER_SIZE  4096
extern pCmdPacktet_t  recvBufferPtr;
extern pSendPacktet_t sendBufferPtr;
extern uint16_t recvIndex;
uint32_t firstTime = 0;
void initSerialSlave(void)
{
  recvBufferPtr = i2cCuappEnqueue(RECV_BUFFER_SIZE);
  sendBufferPtr = i2cSendCuappEnqueue(SEND_BUFFER_SIZE);
  Serial1.setRX(1);
  Serial1.setTX(0);
  Serial1.begin(115200);
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
