/*!
 * @file DFRobot_BloodOxygen_S.cpp
 * @brief This is the .cpp file for a sensor that can detect human oxygen saturation and heart rate.
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author      PengKaixing(kaixing.peng@dfrobot.com)
 * @version  V1.0.0
 * @date  2021-06-21
 * @url https://github.com/DFRobot/DFRobot_BloodOxygen_S
 */
#include "DFRobot_BloodOxygen_S.h"
uint8_t MAX30102Com = 0;
static uint16_t calculate_CRC(uint8_t *data, uint8_t len)
{
  uint16_t crc = 0xFFFF;
  for( uint8_t pos = 0; pos < len; pos++)
  {
    crc ^= (uint16_t)data[ pos ];
    for(uint8_t i = 8; i != 0; i--)
    {
      if((crc & 0x0001) != 0){
        crc >>= 1;
        crc ^= 0xA001;
      }else{
        crc >>= 1;
      }
    }
  }
  crc = ((crc & 0x00FF) << 8) | ((crc & 0xFF00) >> 8);
  return crc;
}

void DFRobot_BloodOxygen_S::getHeartbeatSPO2(void)
{
  uint8_t SPO2Valid;
  int8_t HeartbeatValid;
  uint8_t rbuf[8];
  readReg(0x0C,rbuf,8);
  static uint8_t heartbeat_count;
  uint32_t SPO2_all_val=0;
  uint32_t heartbeat_all_val=0;
  _sHeartbeatSPO2.SPO2 = rbuf[0];
  if(_sHeartbeatSPO2.SPO2 == 0)
  {
    _sHeartbeatSPO2.SPO2 = -1;
  }
  _sHeartbeatSPO2.Heartbeat = ((uint32_t)rbuf[2] << 24) | ((uint32_t)rbuf[3] << 16) | ((uint32_t)rbuf[4] << 8) | ((uint32_t)rbuf[5]);
  if (_sHeartbeatSPO2.Heartbeat == 0)
  {
    _sHeartbeatSPO2.Heartbeat = -1;
  }
}

float DFRobot_BloodOxygen_S::getTemperature_C(void)
{
  uint8_t temp_buf[2];
  readReg(0x14, temp_buf, 2);
  float Temperature = temp_buf[0] * 1.0 + temp_buf[1] / 100.0;
  return Temperature;
}

void DFRobot_BloodOxygen_S::setBautrate(ebautrate bautrate)
{
  uint8_t w_buf[2];
  w_buf[0] = (uint8_t)(bautrate >> 8);
  w_buf[1] = (uint8_t)bautrate;
  writeReg(0x06, w_buf, sizeof(w_buf));
  delay(100);
  w_buf[0] = 0x00;
  w_buf[1] = 0x01;
  writeReg(0x1A, w_buf, sizeof(w_buf));
  delay(1000);
}

uint32_t DFRobot_BloodOxygen_S::getBautrate(void)
{
  uint8_t r_buf[2];
  readReg(0x06, r_buf, sizeof(r_buf));
  uint16_t baudrate_type = (uint16_t)r_buf[0] << 8 | (uint16_t)r_buf[1];
  switch (baudrate_type)
  {
    case 0:
      return 1200;
      break;
    case 1:
      return 2400;
      break;
    case 3:
      return 9600;
      break;
    case 5:
      return 19200;
      break;
    case 6:
      return 38400;
      break;
    case 7:
      return 57600;
      break;
    case 8:
      return 115200;
      break;
    default:
      return 9600;
      break;
  }
}

void DFRobot_BloodOxygen_S::sensorStartCollect(void)
{
  uint8_t wbuf[2]={0,1};
  writeReg(0x20,wbuf,2);
}

void DFRobot_BloodOxygen_S::sensorEndCollect(void)
{
  uint8_t wbuf[2] = {0, 2};
  writeReg(0x20, wbuf, 2);
}

//I2C underlying communication
DFRobot_BloodOxygen_S_I2C::DFRobot_BloodOxygen_S_I2C(SoftwareTwoWire *pWire, uint8_t addr)
{
  _pWire = pWire;
  this->_I2C_addr = addr;
}

DFRobot_BloodOxygen_S_I2C::DFRobot_BloodOxygen_S_I2C(TwoWire *pWire, uint8_t addr)
{
  _pWire1 = pWire;
  this->_I2C_addr = addr;
}

bool DFRobot_BloodOxygen_S_I2C::begin(uint8_t addr)
{
  this->_I2C_addr = addr;
  if(MAX30102Com == 1){
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

void DFRobot_BloodOxygen_S_I2C::writeReg(uint16_t reg_addr, uint8_t *data_buf, uint8_t len)
{
  if(MAX30102Com == 1){
    _pWire->beginTransmission(this->_I2C_addr);
    _pWire->write(reg_addr);
    for (uint8_t i = 0; i < len; i++)
    {
      _pWire->write(data_buf[i]);
    }
    _pWire->endTransmission();
  }else{
    _pWire1->beginTransmission(this->_I2C_addr);
    _pWire1->write(reg_addr);
    for (uint8_t i = 0; i < len; i++)
    {
      _pWire1->write(data_buf[i]);
    }
    _pWire1->endTransmission();
  }

}

int16_t DFRobot_BloodOxygen_S_I2C::readReg(uint16_t reg_addr, uint8_t *data_buf, uint8_t len)
{
  int i = 0;
  if(MAX30102Com == 1){
    _pWire->beginTransmission(this->_I2C_addr);
    _pWire->write(reg_addr);
    if (_pWire->endTransmission() != 0)
    {
      return -1;
    }
    _pWire->requestFrom((uint8_t)this->_I2C_addr, (uint8_t)len);
    while (_pWire->available())
    {
      data_buf[i++] = _pWire->read();
    }
  }else{
    _pWire1->beginTransmission(this->_I2C_addr);
    _pWire1->write(reg_addr);
    if (_pWire1->endTransmission() != 0)
    {
      return -1;
    }
    _pWire1->requestFrom((uint8_t)this->_I2C_addr, (uint8_t)len);
    while (_pWire1->available())
    {
      data_buf[i++] = _pWire1->read();
    }
  }

  return len;
}

DFRobot_BloodOxygen_S_I2C SEN0518_1(&SOF_WIRE1, 0x57);
DFRobot_BloodOxygen_S_I2C SEN0518_2(&Wire1, 0x57);