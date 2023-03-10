/*!
 * @file SoftwareTwoWire.cpp
 * @brief Arduino Software IIC library.
 * @n support AVR, FireBeetle ESP32/ESP8266/M0, microbit.
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2021-04-15
 * @https://github.com/DFRobot/DFRobot_SCT80S16B
 */
#include "software_iic.h"

SoftwareTwoWire::SoftwareTwoWire(uint8_t scl, uint8_t sda)
  :_scl(scl), _sda(sda), _t(5),_t1(0),_t2(0),_transmitting(0),_twiaddr(0),_txBufferIndex(0),_txBufferLength(0),_rxBufferIndex(0), _rxBufferLength(0),_error(0),_stop(false){
  memset(_txBuffer, 0, SOFT_TWO_WIRE_BUFFER_LENGTH);
  memset(_rxBuffer, 0, SOFT_TWO_WIRE_BUFFER_LENGTH);
}//100Kbps
  
SoftwareTwoWire::~SoftwareTwoWire(){}

void SoftwareTwoWire::begin(){
  pinMode(_scl, OUTPUT);
  pinMode(_sda, OUTPUT);
  digitalWrite(_scl, HIGH);
  digitalWrite(_sda, HIGH);
  _t1 = _t/2;
  _t2 = (_t/2) + ((_t%2) ? 1 : 0);
}

/*设置频率不太准*/
void SoftwareTwoWire::setClock(uint32_t freq){
  // if(freq > 400000)  freq = 400000;
  // _t = 1000000 / freq + ((1000000%freq) ? 1 : 0);
  // _t = (_t/2) + ((_t%2) ? 1 : 0);
  // _t1 = _t/2;
  // _t2 = (_t/2) + ((_t%2) ? 1 : 0);
}

void SoftwareTwoWire::beginTransmission(uint8_t addr){
  _transmitting = 1;
  _twiaddr = addr;
  _txBufferIndex = 0;
  _txBufferLength = 0;
}

uint8_t SoftwareTwoWire::endTransmission(bool sendStop){
  uint8_t ret = writeTo(true, sendStop);
  _txBufferIndex = 0;
  _txBufferLength = 0;
  _transmitting = 0;
  return ret;
}

uint8_t SoftwareTwoWire::requestFrom(uint8_t address, uint8_t quantity, bool sendStop){
  if(quantity > SOFT_TWO_WIRE_BUFFER_LENGTH) quantity=SOFT_TWO_WIRE_BUFFER_LENGTH;
  _twiaddr = address;
  uint8_t ret = readFrom(quantity, sendStop);
  _rxBufferIndex = 0;
  _rxBufferLength = ret;
  //Serial.print("ret=");Serial.println(ret);
  return ret;
}

size_t SoftwareTwoWire::write(uint8_t data){
  if(_txBufferLength >= SOFT_TWO_WIRE_BUFFER_LENGTH) return 0;
  _txBuffer[_txBufferIndex++] = data;
  _txBufferLength = _txBufferIndex;
  return 1;
}

size_t SoftwareTwoWire::write(const uint8_t *data, size_t len){
  for (size_t i = 0; i < len; ++i) {
      if (!write(data[i])) {
          return i;
      }
  }
  return len;
}

int SoftwareTwoWire::available(void){
  return _rxBufferLength - _rxBufferIndex;
}

int SoftwareTwoWire::read(void){
  int val = -1;
  if(_rxBufferIndex < _rxBufferLength){
    val = _rxBuffer[_rxBufferIndex++];
  }
  return val;
}
int SoftwareTwoWire::peek(void){
  int val = -1;
  if(_rxBufferIndex < _rxBufferLength){
    val = _rxBuffer[_rxBufferIndex];
  }
  return val;
}
void SoftwareTwoWire::flush(void){
  _txBufferIndex = 0;
  _txBufferLength = 0;
}

//起始信号：scl高电平期间，sda由高到低的跳变
void SoftwareTwoWire::startSignal(void){
  digitalWrite(_sda,HIGH);
  digitalWrite(_scl,HIGH);
  delayMicroseconds(5);
  digitalWrite(_sda,LOW);
  delayMicroseconds(5);
  digitalWrite(_scl,LOW);
  //delayMicroseconds(50);
}
//停止信号：scl高电平期间，sda由低到高的跳变
void SoftwareTwoWire::stopSignal(void){
  // digitalWrite(_sda,LOW);
  // delayMicroseconds(5);
  // digitalWrite(_scl,HIGH);
  // delayMicroseconds(5);
  // digitalWrite(_sda,HIGH);
  // delayMicroseconds(5);
   digitalWrite(_sda,LOW);
  digitalWrite(_scl,HIGH);
  delayMicroseconds(5);
  digitalWrite(_sda,HIGH);
  delayMicroseconds(5);
}

uint8_t SoftwareTwoWire::sendByte(uint8_t data){
  // for(int i=7; i>=0;i--){  
  //   //digitalWrite(_scl,LOW);
  //   if(data & (1<<i)){
  //     digitalWrite(_sda,HIGH);//在scl低电平期间改变sda电平的状态，并持续一段时间
  //   }else{
  //     digitalWrite(_sda,LOW);
  //   }
  //   delayMicroseconds(5); 
  //   digitalWrite(_scl,HIGH);
  //   delayMicroseconds(5);
  //   digitalWrite(_scl,LOW);
  // }
  // delayMicroseconds(5);
  // return recvAck();
   for(int i=7; i>=0;i--){  
    digitalWrite(_scl,LOW);
    if(data & (1<<i)){
      digitalWrite(_sda,HIGH);//在scl低电平期间改变sda电平的状态，并持续一段时间
    }else{
      digitalWrite(_sda,LOW);
    }
    delayMicroseconds(5); 
    digitalWrite(_scl,HIGH);
    delayMicroseconds(5);
  }
  digitalWrite(_scl,LOW);
  return recvAck();
}

uint8_t SoftwareTwoWire::readByte(){
  uint8_t data=0;
  pinMode(_sda,INPUT_PULLUP);
  for(int i = 7; i >= 0; i--){
    digitalWrite(_scl,LOW);
    delayMicroseconds(5);
    digitalWrite(_scl,HIGH);
    data |= (digitalRead(_sda) << i);
    delayMicroseconds(5);
  }
  digitalWrite(_scl,LOW);
  pinMode(_sda,OUTPUT);
  return data;
}

void SoftwareTwoWire::sendAck(uint8_t ack){
  digitalWrite(_scl,LOW);
  if(ack){/*ack=0,即发送ack信号，ack=1,即发送NACK信号*/
    digitalWrite(_sda,HIGH);
  }else{
    digitalWrite(_sda,LOW);
  }
  delayMicroseconds(5);
  digitalWrite(_scl,HIGH);
  delayMicroseconds(5);
  digitalWrite(_scl,LOW);
  if(!ack) digitalWrite(_sda,HIGH);
  //delayMicroseconds(20);
}
//0:有效应答， 1：无效应答，在ACK中检测时钟延展
uint8_t SoftwareTwoWire::recvAck(void){
  // uint8_t ack=0;  
  
  // digitalWrite(_scl,HIGH);
  // pinMode(_sda,INPUT_PULLUP);
  // ack=digitalRead(_sda);
  // delayMicroseconds(5);
  // digitalWrite(_scl,LOW);
  // delayMicroseconds(5);
  // pinMode(_sda,OUTPUT);
  // return ack;
    uint8_t ack=0;
  digitalWrite(_scl,LOW);
  pinMode(_sda,INPUT_PULLUP);
  delayMicroseconds(5);
  digitalWrite(_scl,HIGH);
  ack=digitalRead(_sda);
  delayMicroseconds(5);
  digitalWrite(_scl,LOW);
  pinMode(_sda,OUTPUT);
  digitalWrite(_sda,HIGH);
  delayMicroseconds(20);
  return ack;
}

uint8_t SoftwareTwoWire::writeTo(bool wait, bool sendStop){
  uint8_t ack;
  if(SOFT_TWO_WIRE_BUFFER_LENGTH < _txBufferLength) return 1;//tx buffer overflow

  _error = 0;// success
  _stop = sendStop;
  _twiaddr = ((_twiaddr << 1)|0);

  startSignal();
  if(sendByte(_twiaddr) != 0){
      _error = 2; //address send, nack received
      _txBufferLength = 0;
	  //Serial.println("+++++++++++++++++");
  }
   //Serial.print("_txBufferLength=");Serial.println(_txBufferLength);
  for(uint8_t i = 0; i < _txBufferLength; i++){
      if(sendByte(_txBuffer[i]) != 0){
          _error = 3;//data send, nack received
		  //Serial.println("111111111111111");
          break;
      }
  }
  if(sendStop) stopSignal();
  return _error;
}

uint8_t SoftwareTwoWire::readFrom(uint8_t length, bool sendStop){
  if(SOFT_TWO_WIRE_BUFFER_LENGTH < length) return 0;
  _stop = sendStop;
  _twiaddr = ((_twiaddr << 1) | 1);
  startSignal();
  if(sendByte(_twiaddr) != 0){
      _error = 2; //address send, nack received
      //Serial.println("++++++++++++++");
      length = 0;
  }
  //Serial.print("length = ");Serial.println(length);
  //Serial.print("_rxBufferIndex = ");Serial.println(_rxBufferIndex);
  for(uint8_t i = 0; i < length; i++){
      _rxBuffer[i] = readByte();
      if(i == (length - 1)){
          sendAck(1);//NACK
      }else{
          sendAck(0);//ACK
      }
  }
  if(sendStop) stopSignal();
  return length;
}



void SoftwareTwoWire::test(void){
  startSignal();
  sendByte(0xe0);//B0  e0
  //recvAck();
  stopSignal();
}


SoftwareTwoWire SOF_WIRE1(5, 4);
SoftwareTwoWire SOF_WIRE2(9, 8);