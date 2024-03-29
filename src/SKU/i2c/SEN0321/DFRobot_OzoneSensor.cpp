/*!
 * @file DFRobot_OzoneSensor.cpp
 * @brief Define the basic structure of the DFRobot_OzoneSensor class, the implementation of the basic methods
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [ZhixinLiu](zhixin.liu@dfrobot.com)
 * @version V1.0
 * @date 2019-07-13
 * @url https://github.com/DFRobot/DFRobot_OzoneSensor
 */
#include "DFRobot_OzoneSensor.h"
uint8_t OzoneCom = 0;
DFRobot_OzoneSensor::DFRobot_OzoneSensor(SoftwareTwoWire * pWire){
  _pWire = pWire;
}
DFRobot_OzoneSensor::DFRobot_OzoneSensor(TwoWire * pWire){
  _pWire1 = pWire;
}
DFRobot_OzoneSensor::~DFRobot_OzoneSensor(){}

bool DFRobot_OzoneSensor::begin(uint8_t addr)
{
  this->_addr = addr;
  if(OzoneCom == 1){
    _pWire->beginTransmission(_addr);
    if(_pWire->endTransmission() == 0){
      return true;
    }
  }else{
    _pWire1->beginTransmission(_addr);
    if(_pWire1->endTransmission() == 0){
      return true;
    }
  }
  
  return false;
}

void DFRobot_OzoneSensor::i2cWrite(uint8_t reg , uint8_t pData)
{
  if(OzoneCom == 1){
    _pWire->beginTransmission(_addr);
    _pWire->write(reg);
    _pWire->write(pData);
    _pWire->endTransmission();
  }else{
    _pWire1->beginTransmission(_addr);
    _pWire1->write(reg);
    _pWire1->write(pData);
    _pWire1->endTransmission();
  }
  
}

int16_t DFRobot_OzoneSensor::i2cReadOzoneData(uint8_t reg)
{
  uint8_t i = 0;
  uint8_t rxbuf[10]={0};
  if(OzoneCom == 1){
    _pWire->beginTransmission(_addr);
    _pWire->write(reg);
    _pWire->endTransmission();
    _pWire->requestFrom(_addr, (uint8_t)2);
    while (_pWire->available())
      rxbuf[i++] = _pWire->read();
  }else{
    _pWire1->beginTransmission(_addr);
    _pWire1->write(reg);
    _pWire1->endTransmission();
    _pWire1->requestFrom(_addr, (uint8_t)2);
    while (_pWire1->available())
      rxbuf[i++] = _pWire1->read();
  }
 
  return ((int16_t)rxbuf[0] << 8) + rxbuf[1];
}

void DFRobot_OzoneSensor::setModes(uint8_t mode)
{
  if(mode == MEASURE_MODE_AUTOMATIC1){
    i2cWrite(MODE_REGISTER , MEASURE_MODE_AUTOMATIC1);
    _M_Flag = 0;
  }else if(mode == MEASURE_MODE_PASSIVE){
    i2cWrite(MODE_REGISTER , MEASURE_MODE_PASSIVE);
    _M_Flag = 1;
  }else {
    return;
  }
}

int16_t DFRobot_OzoneSensor::readOzoneData(uint8_t collectNum)
{
  static uint8_t i = 0, j = 0;
  if (collectNum > 0) {
    for(j = collectNum - 1;  j > 0; j--){
      ozoneData[j] = ozoneData[j-1];
    }
    if(_M_Flag == 0){        
      i2cWrite(SET_PASSIVE_REGISTER , AUTO_READ_DATA);
      ozoneData[0] = i2cReadOzoneData(AUTO_DATA_HIGE_REGISTER);
    }else if(_M_Flag == 1){
      i2cWrite(SET_PASSIVE_REGISTER , PASSIVE_READ_DATA);      // read passive data in passive mode, first request once, then read the data
      ozoneData[0] = i2cReadOzoneData(PASS_DATA_HIGE_REGISTER);
    }
    if(i < collectNum) i++;
    return getAverageNum(ozoneData, i);
  }else if(collectNum <= 0 || collectNum > 100){
   return -1;
  }
  return 0;
}

int DFRobot_OzoneSensor::getAverageNum(int bArray[], int iFilterLen) 
{
  unsigned long bTemp = 0;
  for(uint16_t i = 0; i < iFilterLen; i++) {
    bTemp += bArray[i];
  }
  return bTemp / iFilterLen;
}

DFRobot_OzoneSensor SEN0321_1(&SOF_WIRE1);
DFRobot_OzoneSensor SEN0321_2(&Wire1);