
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
#include "global.h"

uint8_t getVersion(pCmdPacktet_t pkt){
  if(pkt->cmd != CMD_GET_VERSION){
    GLOBAL_DBG("invaild command!");
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
    GLOBAL_DBG("invaild command!");
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
extern float voltage;
extern float percentage;
String getAllValue(const char *key){
  String sensor = "";
  struct I2CSensorInfo *p = I2C_SENSOR_INFO_HEAD;
  if(String(key) == "speed"){
    sensor = String(wSpeed/100.0);
    return sensor;
  }else if(String(key) == "dir"){
    sensor = getCompassData();
    return sensor;
  }else if(String(key) == "time"){
    sensor = timeData;
    return sensor;
  }else if(String(key) == "Voltage"){
    sensor = String(voltage);
    return sensor;
  }else if(String(key) == "Percentage"){
    sensor = String(percentage);
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
    GLOBAL_DBG("invaild command!");
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
  if(String(key) == "speed"){
    sensor = "m/s";
    return sensor;
  }else if(String(key) == "Voltage"){
    sensor = "mV";
    return sensor;
  }else if(String(key) == "Percentage"){
    sensor = "%";
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
    GLOBAL_DBG("invaild command!");
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
extern float voltage;
extern float percentage;
String combinationAllValue(pCmdPacktet_t key){
  String sensor = "";
  struct I2CSensorInfo *p = I2C_SENSOR_INFO_HEAD;
  if(key->data[0] == 1){
    sensor += timeData;
    sensor += ", ";
  }
  sensor += "WindSpeed:";
  sensor += String(wSpeed/100.0);
  sensor += " m/s, ";
  sensor += "WindDirection:";
  sensor += getCompassData();
  sensor += ", Voltage:";
  sensor += String(voltage);
  sensor += " mV, ";
  sensor += "Percentage:";
  sensor += String(percentage);
  sensor += " %";

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
extern DFRobot_PCF8563T rtc;
uint8_t set_Time(pCmdPacktet_t pkt)
{
  if(pkt->cmd != CMD_SET_TIME){
    GLOBAL_DBG("invaild command!");
    return ERR_CODE_CMD_PKT;
  }
  uint16_t year = pkt->data[0] + 2000;
  uint8_t month = pkt->data[1];
  uint8_t day = pkt->data[2];
  uint8_t week = pkt->data[3];
  uint8_t hour = pkt->data[4];
  uint8_t minute = pkt->data[5];
  uint8_t second = pkt->data[6];
  GLOBAL_DBG(year);
  GLOBAL_DBG(month);
  GLOBAL_DBG(day);
  GLOBAL_DBG(week);
  GLOBAL_DBG(hour);
  GLOBAL_DBG(minute);
  GLOBAL_DBG(second);
  rtc.begin(year,month,day,week,hour,minute,second);
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
    GLOBAL_DBG("invaild command!");
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
    GLOBAL_DBG("invaild command!");
    return ERR_CODE_CMD_PKT;
  }
  Serial.println("resetData");
  sendBufferPtr->index = 0;
  return ERR_CODE_NONE;
}