/*!
 * @file SoftwareTwoWire.h
 * @brief Arduino Software IIC library.
 * @n support AVR, FireBeetle ESP32/ESP8266/M0, microbit.
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2021-04-22
 * @https://github.com/DFRobot/DFRobot_SCT80S16B
 */

#ifndef SOFTWARE_IIC_H
#define SOFTWARE_IIC_H

#include "Stream.h"
#include <Arduino.h>
#include "Wire.h"

#ifndef SOFT_TWO_WIRE_BUFFER_LENGTH
#define SOFT_TWO_WIRE_BUFFER_LENGTH 64
#endif

class SoftwareTwoWire: public Stream{
public:
  SoftwareTwoWire(uint8_t scl, uint8_t sda);
  ~SoftwareTwoWire();
  void begin();
  void setClock(uint32_t freq);
  void beginTransmission(uint8_t addr);
  uint8_t endTransmission(bool sendStop = true);
  uint8_t requestFrom(uint8_t address, uint8_t quantity, bool sendStop = true);
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *, size_t);
  virtual int available(void);
  virtual int read(void);
  virtual int peek(void);
  virtual void flush(void);

  void test(void);

  inline size_t write(unsigned long n) { return write((uint8_t)n); }
  inline size_t write(long n) { return write((uint8_t)n); }
  inline size_t write(unsigned int n) { return write((uint8_t)n); }
  inline size_t write(int n) { return write((uint8_t)n); }
  using Print::write;
  
protected:
  void startSignal(void);
  void stopSignal(void);
  void sendAck(uint8_t ack);
  uint8_t recvAck(void);
  uint8_t sendByte(uint8_t data);
  uint8_t readByte();
  uint8_t writeTo(bool wait, bool sendStop);
  uint8_t readFrom(uint8_t length, bool sendStop);

private:
  uint8_t  _scl;
  uint8_t  _sda;
  uint32_t _t;
  uint32_t _t1;
  uint32_t _t2;
  uint8_t  _transmitting;
  uint8_t  _twiaddr;
  uint8_t  _txBufferIndex;
  uint8_t  _txBufferLength;
  uint8_t  _rxBufferIndex;
  uint8_t  _rxBufferLength;
  uint8_t  _error;
  bool     _stop;
  uint8_t  _txBuffer[SOFT_TWO_WIRE_BUFFER_LENGTH];
  uint8_t  _rxBuffer[SOFT_TWO_WIRE_BUFFER_LENGTH];
};
//接口1对应的I2C对象
extern SoftwareTwoWire SOF_WIRE1;
//接口2对应的I2C对象
extern SoftwareTwoWire SOF_WIRE2;


#endif