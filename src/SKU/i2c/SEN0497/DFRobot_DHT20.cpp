/*!
 *@file DFRobot_DHT20.cpp
 *@brief Define the basic structure of class DFRobot_DHT20, the implementation of the basic methods
 *@copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 *@licence     The MIT License (MIT)
 *@author [fengli](li.feng@dfrobot.com)
 *@version  V1.0
 *@date  2021-6-24
 *@get from https://www.dfrobot.com
 *@https://github.com/DFRobot/DFRobot_DHT20
*/

#include "DFRobot_DHT20.h"

uint8_t dht20Com = 0;
DFRobot_DHT20::DFRobot_DHT20(SoftwareTwoWire * pWire,uint8_t address)
  : _pWire(pWire) {
  _address = address;
}
DFRobot_DHT20::DFRobot_DHT20(TwoWire * pWire,uint8_t address)
  : _pWire1(pWire) {
  _address = address;
}

DFRobot_DHT20::DFRobot_DHT20()
  : _pWire(NULL), _address(0x38){
}

int DFRobot_DHT20::begin() {
  uint8_t readCMD[3]={0x71};
  uint8_t data;
  if(dht20Com == 1){
    if(_pWire == NULL) return 1;
  }else{
    if(_pWire1 == NULL) return 1;
  }

  delay(100);

  //check if the IIC communication works 
  writeCommand(readCMD,1);
  
  readData(&data, 1);
  //Serial.println(data);
  if((data | 0x8) == 0){
     return 1;
  }
  if(data == 255) return 1;
  return 0;
}

int DFRobot_DHT20::begin(uint8_t address) {
  _address = address;
  return begin();
}

bool DFRobot_DHT20::isOnline(){
  if(dht20Com == 1){
    _pWire->beginTransmission(_address);
    if(_pWire->endTransmission() == 0) return true;
  }else{
    _pWire1->beginTransmission(_address);
    if(_pWire1->endTransmission() == 0) return true;
  }

  return false;
}

float DFRobot_DHT20::getTemperature() {
  uint8_t readCMD[3]={0xac,0x33,0x00};
  uint8_t data[6] = {0};
  int retries = 10;
  float temperature;
  // when the returned data is wrong, request to get data again until the data is correct. 
  writeCommand(readCMD, 3);
  while(retries--) {
    delay(10);
    readData(data,6);
    if((data[0] >> 7) == 0){
       DBG("bus not busy");
       break;
    }
  }
  uint32_t temp = data[3] & 0xff;
  uint32_t temp1 = data[4] & 0xff;
  uint32_t rawData = 0;
  rawData = ((temp&0xf)<<16)+(temp1<<8)+(data[5]);
  DBG(rawData);
  DBG((temp&0xf)<<16);
  temperature = (float)rawData/5242 -50;
  DBG(temperature)
  return temperature;
}

float DFRobot_DHT20::getHumidity() {
  uint8_t readCMD[3]={0xac,0x33,0x00};
  uint8_t data[6] = {0};
  int retries = 10;
  float humidity;
  // when the returned data is wrong, request to get data again until the data is correct. 
  writeCommand(readCMD, 3);
  while(retries--) {
    delay(10);
    readData(data,6);
    if((data[0] >> 7) == 0){
       DBG("bus not busy");
       break;
    }
  }
  uint32_t temp = data[1] & 0xff;
  uint32_t temp1 = data[2] & 0xff;
  uint32_t rawData = 0;
  rawData = (temp<<12)+(temp1<<4)+((data[3]&0xf0)>>4);
  DBG(rawData);
  DBG(temp<<12);
  humidity = (float)rawData/0x100000;
  DBG(humidity)
  return humidity;
}
void DFRobot_DHT20::writeCommand(const void *pBuf, size_t size) {
  if (pBuf == NULL) {
    DBG("pBuf ERROR!! : null pointer");
  }

  uint8_t * _pBuf = (uint8_t *)pBuf;
  if(dht20Com == 1){
    _pWire->beginTransmission(_address);
    for (uint8_t i = 0; i < size; i++) {
      
      _pWire->write(_pBuf[i]);
      
    }
    _pWire->endTransmission();
  }else{
    _pWire1->beginTransmission(_address);
    for (uint8_t i = 0; i < size; i++) {
      
      _pWire1->write(_pBuf[i]);
      
    }
    _pWire1->endTransmission();
  }

}

uint8_t DFRobot_DHT20::readData(void *pBuf, size_t size) {
  delay(10);
  if (pBuf == NULL) {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  //read the data returned by the chip
  if(dht20Com == 1){
    _pWire->requestFrom(_address, size);
    uint8_t i = 0;
    for (uint8_t i = 0 ; i < size; i++) {
      _pBuf[i] = _pWire->read();
      DBG(_pBuf[i]);
    }
  }else{
    _pWire1->requestFrom(_address, size);
    uint8_t i = 0;
    for (uint8_t i = 0 ; i < size; i++) {
      _pBuf[i] = _pWire1->read();
      DBG(_pBuf[i]);
    }
  }

  return 1;
  
}

DFRobot_DHT20 DHT20_1(&SOF_WIRE1, 0x38);
DFRobot_DHT20 DHT20_2(&Wire1, 0x38);