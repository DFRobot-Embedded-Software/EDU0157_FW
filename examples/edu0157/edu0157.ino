#include "global.h"
extern sGeneral_t *pGeneral_t;
uint64_t startTime=0;
uint64_t windSpeedstartTime=0;
uint64_t delayTime=0;
uint64_t Time=0;
extern float wSpeed;
extern uint8_t dir;

void setup(void)
{
  begin();//初始化板载传感器、通信协议选择
  startTime = millis();
  windSpeedstartTime = millis();
  delayTime =  millis();
  
}

void loop(void)
{
  Time = millis();
  (pGeneral_t->communicationMode == I2CMODE)? i2cloop():uartloop();

  if((Time - windSpeedstartTime) > 1000){
    windSpeedstartTime = millis();
    wSpeed = (3.14 * (pGeneral_t->windCup /10.0) * 2.0 * pGeneral_t->interrcount) / 1.0;
    pGeneral_t->interrcount = 0;
  }
  if((Time - delayTime) > pGeneral_t->delayTime){
   pGeneral_t->saveStateTime = 1;
  }
   if((Time - startTime) > pGeneral_t->sampleRate){
      startTime = millis();
      sensorDataCollect();
    }
  indicatorColor();
  if(pGeneral_t->interruptData == 1){
    pGeneral_t->interruptData = 0;
    dir = 0;
    dir |= digitalRead(TMR1302S_U13_PIN)<<3 | /*B*/digitalRead(TMR1302S_U6_PIN)<<2 | /*C*/digitalRead(TMR1302S_U9_PIN)<<1 | /*D*/digitalRead(TMR1302S_U11_PIN)<<0;
    getCompassData();
      //Serial.println("interrupt");
    }
//getCompassData();
  //Serial.println(pGeneral_t->windCup);
  delay(1);
}
