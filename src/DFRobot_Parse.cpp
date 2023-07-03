
// /**
//  * @file DFRobot_Parse.cpp
//  * @brief I2C从机命令解析
//  *
//  * @copyright   Copyright (c) 2022 DFRobot Co.Ltd (http://www.dfrobot.com)
//  * @license     The MIT License (MIT)
//  * @author [Arya](xue.peng@dfrobot.com)
//  * @version  V1.0
//  * @date  2022-08-02
//  * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
//  * 
//  */
#include "DFRobot_Parse.h"
#include <Arduino.h>
#include "DFRobot_PCF8563T.h"
extern sGeneral_t *pGeneral;

uint8_t getVersion(pCmdPacktet_t pkt){
  if(pkt->cmd != CMD_GET_VERSION){
    ENABLE_PARSE_DBG("invaild command!");
    return ERR_CODE_CMD_PKT;
  }
  uint8_t sendData[RESPONSE_ERR_PKT_LEN - 1 + 2];
  memset(sendData, 0, sizeof(sendData));
  sendData[RESPONSE_STATUS] =  STATUS_SUCCESS;
  sendData[RESPONSE_CMD]    =  CMD_GET_VERSION;
  sendData[RESPONSE_LEN_L]  =  2;
  sendData[RESPONSE_LEN_H]  =  0;
  sendData[RESPONSE_ERR_CODE] = 1;
  sendData[RESPONSE_ERR_CODE + 1] = 0;
  DFRobot_Response(sendData, sizeof(sendData));
  return ERR_CODE_NONE;
}

uint8_t getData(pCmdPacktet_t pkt)
{
  if(pkt->cmd != CMD_GET_DATA){
    ENABLE_PARSE_DBG("invaild command!");
    return ERR_CODE_CMD_PKT;
  }
  char key[pkt->len + 1];
  memcpy(key, pkt->data, pkt->len);
  key[pkt->len] = '\0';
  String sensor = getAllValue(key);
  
  uint8_t sendData[RESPONSE_ERR_PKT_LEN - 1 + sensor.length()];
  memset(sendData, 0, sizeof(sendData));
  sendData[RESPONSE_STATUS] =  STATUS_SUCCESS;
  sendData[RESPONSE_CMD]    =  CMD_GET_DATA;
  sendData[RESPONSE_LEN_L]  =  sensor.length() & 0xFF;
  sendData[RESPONSE_LEN_H]  =  (sensor.length() >> 8) & 0xFF;
  memcpy(sendData + 4, sensor.c_str(), sensor.length());
  DFRobot_Response(sendData, sizeof(sendData));
  return ERR_CODE_NONE;
}

extern struct I2CSensorInfo *I2C_SENSOR_INFO_HEAD;
extern float wSpeed;
extern String timeData;
extern String widDir;
// extern float voltage;
// extern float percentage;
String getAllValue(const char *key){
  String sensor = "";
  struct I2CSensorInfo *p = I2C_SENSOR_INFO_HEAD;
  if(String(key) == "Speed"){
    sensor = String(wSpeed,1);
    return sensor;
  }
  else if(String(key) == "Dir"){
    sensor = widDir;
    return sensor;
  }else if(String(key) == "Time"){
    sensor = timeData;
    return sensor;
  }           
  while(p){
    if(p->attr && p->attr->attr){
      struct keyValue *pkey = p->attr->attr;
      while(pkey){
        if(pkey->key.equals(String(key))){
          if(sensor.length() > 0) sensor += ",";
          sensor += pkey->value;
        }
        pkey = pkey->next;
      }
    }
    p = p->next;
  }
  return sensor;
}

uint8_t getUint(pCmdPacktet_t pkt)
{
  if(pkt->cmd != CMD_GET_UNIT){
    ENABLE_PARSE_DBG("invaild command!");
    return ERR_CODE_CMD_PKT;
  }
  char key[pkt->len + 1];
  memcpy(key, pkt->data, pkt->len);
  key[pkt->len] = '\0';
  String sensor = getAllUint(key);
  uint8_t sendData[RESPONSE_ERR_PKT_LEN - 1 + sensor.length()];
  memset(sendData, 0, sizeof(sendData));
  sendData[RESPONSE_STATUS] =  STATUS_SUCCESS;
  sendData[RESPONSE_CMD]    =  CMD_GET_UNIT;
  sendData[RESPONSE_LEN_L]  =  sensor.length() & 0xFF;
  sendData[RESPONSE_LEN_H]  =  (sensor.length() >> 8) & 0xFF;
  memcpy(sendData + 4, sensor.c_str(), sensor.length());
  DFRobot_Response(sendData, sizeof(sendData));
  return ERR_CODE_NONE;
}

String getAllUint(const char *key){
  String sensor = "";
  struct I2CSensorInfo *p = I2C_SENSOR_INFO_HEAD;
  if(String(key) == "Speed"){
    sensor = "m/s";
    return sensor;
  }
  while(p){
    if(p->attr && p->attr->attr){
      struct keyValue *pkey = p->attr->attr;
      while(pkey){
        if(pkey->key.equals(String(key))){
        if(sensor.length() > 0) sensor += ",";
          sensor += pkey->unit;
        }
          pkey = pkey->next;
      }
    }
    p = p->next;
  }
  return sensor;
}

uint8_t getALLData(pCmdPacktet_t pkt)
{
  if(pkt->cmd != CMD_GET_ALL_DATA){
    ENABLE_PARSE_DBG("invaild command!");
    return ERR_CODE_CMD_PKT;
  }
  String sensor = combinationAllValue(pkt);
  uint8_t sendData[RESPONSE_ERR_PKT_LEN - 1 + sensor.length()];
  memset(sendData, 0, sizeof(sendData));
  sendData[RESPONSE_STATUS] =  STATUS_SUCCESS;
  sendData[RESPONSE_CMD]    =  CMD_GET_ALL_DATA;
  sendData[RESPONSE_LEN_L]  =  sensor.length() & 0xFF;
  sendData[RESPONSE_LEN_H]  =  (sensor.length() >> 8) & 0xFF;
  memcpy(sendData + 4, sensor.c_str(), sensor.length());
  DFRobot_Response(sendData, sizeof(sendData));
  return ERR_CODE_NONE;
}
extern float wSpeed;
// extern float voltage;
// extern float percentage;
String combinationAllValue(pCmdPacktet_t key){
  String sensor = "";
  struct I2CSensorInfo *p = I2C_SENSOR_INFO_HEAD;
  if(key->data[0] == 1){
    sensor += timeData;
    sensor += ", ";
  }
  sensor += "WindSpeed:";
  sensor += String(wSpeed,1);
  sensor += " m/s, ";
  sensor += "WindDirection:";
  sensor += widDir;

  while(p){
    if(p->attr && p->attr->attr){
      struct keyValue *pkey = p->attr->attr;
      while(pkey){
        if(sensor.length() > 0) sensor += ", ";
          sensor += pkey->key;
          sensor += ":";
          sensor += pkey->value;
          sensor += " ";
          sensor += pkey->unit;
          pkey = pkey->next;
      }
    }
    p = p->next;
  }
  return sensor;
}
extern sPCF8563TTime_t rtt;
uint8_t set_Time(pCmdPacktet_t pkt)
{
  if(pkt->cmd != CMD_SET_TIME){
    ENABLE_PARSE_DBG("invaild command!");
    return ERR_CODE_CMD_PKT;
  }
  rtt.year = pkt->data[0] + 2000;
  rtt.month = pkt->data[1];
  rtt.day = pkt->data[2];
  rtt.week = pkt->data[3];
  rtt.hour = pkt->data[4];
  rtt.minute = pkt->data[5];
  rtt.second = pkt->data[6];
  pGeneral->timeConfig = ON;
  ENABLE_PARSE_DBG(rtt.year);
  ENABLE_PARSE_DBG(rtt.month);
  ENABLE_PARSE_DBG(rtt.day);
  ENABLE_PARSE_DBG(rtt.week);
  ENABLE_PARSE_DBG(rtt.hour);
  ENABLE_PARSE_DBG(rtt.minute);
  ENABLE_PARSE_DBG(rtt.second);
  PCF8563T_1.setTime(rtt);
  //PCF8563T_1.begin(year,month,day,week,hour,minute,second);
  uint8_t sendData[RESPONSE_ERR_PKT_LEN - 1 ];
  sendData[RESPONSE_STATUS] =  STATUS_SUCCESS;
  sendData[RESPONSE_CMD]    =  CMD_SET_TIME;
  sendData[RESPONSE_LEN_L]  =  0;
  sendData[RESPONSE_LEN_H]  =  0;
  DFRobot_Response(sendData, sizeof(sendData));
  return ERR_CODE_NONE;
}

uint8_t getTime(pCmdPacktet_t pkt){
  if(pkt->cmd != CME_GET_TIME){
    ENABLE_PARSE_DBG("invaild command!");
    return ERR_CODE_CMD_PKT;
  }
  String sensor = timeData;

  uint8_t sendData[RESPONSE_ERR_PKT_LEN - 1 + sensor.length()];
  memset(sendData, 0, sizeof(sendData));
  sendData[RESPONSE_STATUS] =  STATUS_SUCCESS;
  sendData[RESPONSE_CMD]    =  CME_GET_TIME;
  sendData[RESPONSE_LEN_L]  =  sensor.length() & 0xFF;
  sendData[RESPONSE_LEN_H]  =  (sensor.length() >> 8) & 0xFF;
  memcpy(sendData + 4, sensor.c_str(), sensor.length());
  DFRobot_Response(sendData, sizeof(sendData));
  return ERR_CODE_NONE;
}

extern pSendPacktet_t sendBufferPtr;
uint8_t resetData(pCmdPacktet_t pkt){
  if(pkt->cmd != CMD_RESET_DATA){
    ENABLE_PARSE_DBG("invaild command!");
    return ERR_CODE_CMD_PKT;
  }
  //Serial.println("resetData");
  sendBufferPtr->index = 0;
  return ERR_CODE_NONE;
}

void DFRobot_Response(uint8_t *data, uint8_t len){
  (pGeneral->sCommunicationMode == I2C)? i2cSend(data, len):uartSend(data,len);
}

void i2cSend(uint8_t *data, uint16_t len){
  if(sendBufferPtr->flag){
    sendBufferPtr->flag = false;
  }

  if(len <= 4096){
    memset(sendBufferPtr->data, 0, 4069);
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
    delay(10);
  }
  ENABLE_PARSE_DBG(f);
}

uint8_t setRadius(pCmdPacktet_t pkt){
  uint8_t buf[2];
  uint16_t data = 0;
  
  if(pkt->cmd != CMD_RADIUS_DATA){
    ENABLE_PARSE_DBG("invaild command!");
    return ERR_CODE_CMD_PKT;
  }
  memcpy(buf,pkt->data,2);
  writeRadius(buf);
  data = (buf[0] << 8 | buf[1]);
  pGeneral->sRadial = (float)data/100.0;
  ENABLE_PARSE_DBG(pGeneral->sRadial);
  uint8_t sendData[RESPONSE_ERR_PKT_LEN - 1 ];
  sendData[RESPONSE_STATUS] =  STATUS_SUCCESS;
  sendData[RESPONSE_CMD]    =  CMD_RADIUS_DATA;
  sendData[RESPONSE_LEN_L]  =  0;
  sendData[RESPONSE_LEN_H]  =  0; 
  DFRobot_Response(sendData, sizeof(sendData));
  return ERR_CODE_NONE;
}

uint8_t setStandaraWindSpeed1(pCmdPacktet_t pkt){
  uint16_t data= 0;
  uint8_t buf[2];
  if(pkt->cmd != CMD_SPEED1_DATA){
    ENABLE_PARSE_DBG("invaild command!");
    return ERR_CODE_CMD_PKT;
  }
  memcpy(buf,pkt->data,2);
  writeStand1(buf);
  data = buf[0] << 8 | buf[1];
  pGeneral->sStandWindSpeed1 = (float)data / 100.0;
  ENABLE_PARSE_DBG(pGeneral->sStandWindSpeed1);
  pGeneral->sWindFlage = 1;
  uint8_t sendData[RESPONSE_ERR_PKT_LEN - 1 ];
  sendData[RESPONSE_STATUS] =  STATUS_SUCCESS;
  sendData[RESPONSE_CMD]    =  CMD_SPEED1_DATA;
  sendData[RESPONSE_LEN_L]  =  0;
  sendData[RESPONSE_LEN_H]  =  0; 
  DFRobot_Response(sendData, sizeof(sendData));
  return ERR_CODE_NONE;
}

uint8_t setStandaraWindSpeed2(pCmdPacktet_t pkt){
  uint16_t data= 0;
  if(pkt->cmd != CMD_SPEED2_DATA){
    ENABLE_PARSE_DBG("invaild command!");
    return ERR_CODE_CMD_PKT;
  }
  data = pkt->data[0] << 8 | pkt->data[1];
  pGeneral->sStandWindSpeed2 = (float)data / 100.0;
  ENABLE_PARSE_DBG(pGeneral->sStandWindSpeed2);
  pGeneral->sWindFlage = 2;
  uint8_t sendData[RESPONSE_ERR_PKT_LEN - 1 ];
  sendData[RESPONSE_STATUS] =  STATUS_SUCCESS;
  sendData[RESPONSE_CMD]    =  CMD_SPEED2_DATA;
  sendData[RESPONSE_LEN_L]  =  0;
  sendData[RESPONSE_LEN_H]  =  0; 
  DFRobot_Response(sendData, sizeof(sendData));
  return ERR_CODE_NONE;
}

uint8_t calibrator(pCmdPacktet_t pkt){
  String str="";
  if(pkt->cmd != CMD_CALIBRATOR){
    ENABLE_PARSE_DBG("invaild command!");
    return ERR_CODE_CMD_PKT;
  }
  ENABLE_PARSE_DBG(pGeneral->sAtPresentSpeed2);
  ENABLE_PARSE_DBG(pGeneral->sAtPresentSpeed1);
  if((pGeneral->sAtPresentSpeed2 == 0) || (pGeneral->sAtPresentSpeed1 == 0)){
    str += "error";
    pGeneral->sLinearFactor = 1;
    pGeneral->sOffset = 0;
    writeData(pGeneral->sOffset,pGeneral->sLinearFactor);
  }else{
    pGeneral->sLinearFactor = (pGeneral->sStandWindSpeed2 - pGeneral->sStandWindSpeed1) / (pGeneral->sAtPresentSpeed2 - pGeneral->sAtPresentSpeed1);
    pGeneral->sOffset = (pGeneral->sStandWindSpeed2 - (pGeneral->sAtPresentSpeed2 * pGeneral->sLinearFactor))/pGeneral->sLinearFactor;
    writeData(pGeneral->sOffset,pGeneral->sLinearFactor);
    ENABLE_PARSE_DBG(pGeneral->sLinearFactor);
    ENABLE_PARSE_DBG(pGeneral->sOffset);
    str += "K: ";
    str += String(pGeneral->sLinearFactor);
    str += " ";
    str += "B: ";
    str += String(pGeneral->sOffset);
    str += " ";
    str += "speed1: ";
    str += String(pGeneral->sAtPresentSpeed1);
    str += " ";
    str += "speed2: ";
    str += String(pGeneral->sAtPresentSpeed2);

  }
  
  uint8_t sendData[RESPONSE_ERR_PKT_LEN - 1 + str.length()];
  sendData[RESPONSE_STATUS] =  STATUS_SUCCESS;
  sendData[RESPONSE_CMD]    =  CMD_CALIBRATOR;
  sendData[RESPONSE_LEN_L]  =  str.length() & 0xFF;
  sendData[RESPONSE_LEN_H]  =  (str.length() >> 8) & 0xFF;
  memcpy(sendData + 4, str.c_str(), str.length());
  DFRobot_Response(sendData, sizeof(sendData));
  return ERR_CODE_NONE;
}

uint8_t project(pCmdPacktet_t pkt){
  pGeneral->sSampleRate = 1;
  pGeneral->sUnit = SECOND;
  PCF8563T_1.setCountDown();
  uint8_t sendData[RESPONSE_ERR_PKT_LEN - 1 ];
  sendData[RESPONSE_STATUS] =  STATUS_SUCCESS;
  sendData[RESPONSE_CMD]    =  CMD_PROJECT;
  sendData[RESPONSE_LEN_L]  =  0;
  sendData[RESPONSE_LEN_H]  =  0; 
  DFRobot_Response(sendData, sizeof(sendData));
  return ERR_CODE_NONE;
}