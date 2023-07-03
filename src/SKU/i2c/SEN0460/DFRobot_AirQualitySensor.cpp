/*!
 * @file DFRobot_AirQualitySensor.cpp
 * @brief The sensor can obtain concentration of some PM in the air.
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author      PengKaixing(kaixing.peng@dfrobot.com)
 * @version  V1.0.0
 * @date  2020-11-23
 * @url https://github.com/dfrobot/DFRobot_AirQualitySensor
 */
#include "DFRobot_AirQualitySensor.h"

uint8_t airqCom = 1;
DFRobot_AirQualitySensor::DFRobot_AirQualitySensor(SoftwareTwoWire *pWire, uint8_t addr)
{
  _pWire = pWire;
  this->_I2C_addr = addr;
}

DFRobot_AirQualitySensor::DFRobot_AirQualitySensor(TwoWire *pWire, uint8_t addr)
{
  _pWire1 = pWire;
  this->_I2C_addr = addr;
}

bool DFRobot_AirQualitySensor::begin(uint8_t addr)
{
  this->_I2C_addr = addr;
  if(airqCom == 1){
    _pWire->beginTransmission(_I2C_addr);
    if (_pWire->endTransmission() == 0)
      return true;
  }else{
    _pWire1->beginTransmission(_I2C_addr);
    if (_pWire1->endTransmission() == 0)
      return true;
  }

  return false;
}

uint16_t DFRobot_AirQualitySensor::gainParticleConcentration_ugm3(uint8_t type)
{
  uint8_t buf[2];
  readReg(type, buf, 2);
  uint16_t concentration = ((uint16_t)buf[0] << 8) + (uint16_t)buf[1];
  return concentration;
}

uint16_t DFRobot_AirQualitySensor::gainParticleNum_Every0_1L(uint8_t type)
{
  uint8_t buf[2];
  readReg(type, buf, 2);
  uint16_t particlenum = ((uint16_t)buf[0] << 8) + (uint16_t)buf[1];
  return particlenum;
}

uint8_t DFRobot_AirQualitySensor::gainVersion(void)
{
  uint8_t version = 0 ;
  readReg(PARTICLENUM_GAIN_VERSION, &version, 1);
  return version;
}

void DFRobot_AirQualitySensor::setLowpower(void)
{
  uint8_t mode = 1;
  writeReg(0x01, &mode, 1);
}

void DFRobot_AirQualitySensor::awake(void)
{
  uint8_t mode = 2;
  writeReg(0x01, &mode, 1);
}

void DFRobot_AirQualitySensor::writeReg(uint8_t Reg, void *pData, uint8_t len)
{
  uint8_t *Data = (uint8_t *)pData;
  if(airqCom == 1){
    _pWire->beginTransmission(this->_I2C_addr);
    _pWire->write(Reg);
    for (uint8_t i = 0; i < len; i++)
    {
      _pWire->write(Data[i]);
    }
    _pWire->endTransmission();
  }else{
    _pWire1->beginTransmission(this->_I2C_addr);
    _pWire1->write(Reg);
    for (uint8_t i = 0; i < len; i++)
    {
      _pWire1->write(Data[i]);
    }
    _pWire1->endTransmission();
  }

}

int16_t DFRobot_AirQualitySensor::readReg(uint8_t Reg, uint8_t *Data, uint8_t len)
{
  int i = 0;
  if(airqCom == 1){
    _pWire->beginTransmission(this->_I2C_addr);
    _pWire->write(Reg);
    if (_pWire->endTransmission() != 0)
    {
      return -1;
    }
    _pWire->requestFrom((uint8_t)this->_I2C_addr, (uint8_t)len);
    while (_pWire->available())
    {
      Data[i++] = _pWire->read();
    }
  }else{
    _pWire1->beginTransmission(this->_I2C_addr);
    _pWire1->write(Reg);
    if (_pWire1->endTransmission() != 0)
    {
      return -1;
    }
    _pWire1->requestFrom((uint8_t)this->_I2C_addr, (uint8_t)len);
    while (_pWire1->available())
    {
      Data[i++] = _pWire1->read();
    }
  }

  return len;
}
DFRobot_AirQualitySensor AIRQ_1(&SOF_WIRE1, 0x19);
DFRobot_AirQualitySensor AIRQ_2(&Wire1, 0x19);