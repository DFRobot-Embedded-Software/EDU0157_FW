
#include "general.h"

extern sGeneral_t *pGeneral;
extern String timeData;
//config文件错误检测
uint64_t startTime=0;
uint64_t endTime = 0;
uint8_t state = 0;
//DTU配置文件
uint64_t startTime1=0;
uint8_t state1 = 0;

extern sPCF8563TTime_t rtt;

uint64_t windSpeedstartTime=0;
uint64_t delayTime=0;
extern float wSpeed;
uint8_t state2 = 0;
extern uint8_t _st;
uint64_t i = 0,count = 0;
uint8_t gnssState = 0;
uint64_t startTime3=0;
extern struct I2CSensorInfo *I2C_SENSOR_INFO_HEAD;

extern uint8_t dir;
uint32_t clk_sys_freq;
uint32_t clk_peri_freq;

uint32_t events = 0;

extern uint32_t irqstat;
extern String widDir;
float wSpeedRecord;

void setup(void)
{
  initBoard();//初始化主板
  startTime = millis();
  startTime1 = millis();
  startTime3 = millis();
  windSpeedstartTime = millis();
  delayTime =  millis();
  if(pGeneral->sLowPower == ON){//如果开起低功耗，需要检测卫星数量，如果卫星不足三颗，后面将不再获取定位信息
    struct I2CSensorInfo *info = I2C_SENSOR_INFO_HEAD;
    while(info){
      if(info->addr == 0x20){
        gnssState = 1;
        break; 
      }
      info = info->next;
    }
    if(gnssState == 1){
      while((endTime - startTime3) > 30000){
        endTime = millis();
        if(GNSS_2.getNumSatUsed() > 3){
          break;
        }
        delay(100);
      }
    }
  }
}

void loop(void)
{
  endTime = millis();
#if 1
  if(_st == 1){
    _st = 0;
    readUSBState();
  }
  
  if(pGeneral->sUsbState == ON){
    pGeneral->sUsbState = OFF;
    readUSBState();//查询USB是否在线
    }
    
  if(pGeneral->configState == ON){//如果配置文件出错
    if((endTime - startTime) > 2000){
    startTime = millis();
      if(state == 0){
        setRGBColor(eRed);
        state = 1;
      }else if(state == 1){
        setRGBColor(eBlack);
        state = 0;
      }
    }
  }

  if(pGeneral->DTUState == ON){//如果DTU配置文件错误
    if((endTime - startTime1) > 2000){
    startTime1 = millis();
      if(state1 == 0){
        setRGBColor(eViolet);
          state1 = 1;
      }else if(state1 == 1){
        setRGBColor(eBlack);
          state1 = 0;
      }
    }
  }


  if(state2 == 0){
    if((endTime - delayTime) > pGeneral->sDelayRecord){//延时存储
      pGeneral->saveStateTime = ON;
      state2 = 1;
    }
  }
  
#endif
  if(pGeneral->sWindFlage == 0){//正常获取风速
    if((endTime - windSpeedstartTime) > 3000){
      windSpeedstartTime = millis();
      //speed2 = ((3.14 * (pGeneral->sRadial /10.0) * 2.0 * pGeneral->interrcount) / 1.0 );
      if(pGeneral->interrcount == 0){
        wSpeed = 0;
      }else{
        wSpeedRecord = (((3.14 * (pGeneral->sRadial /10.0) * 2.0 * pGeneral->interrcount) / 1.0 )/100 + pGeneral->sOffset) * pGeneral->sLinearFactor;
        if(wSpeedRecord < 0){
          wSpeed = ((3.14 * (pGeneral->sRadial /10.0) * 2.0 * pGeneral->interrcount) / 1.0 )/100;
        }else{
          wSpeed = wSpeedRecord;
        }
      }
      
      pGeneral->interrcount = 0;
    }
  }else{//校准风速
    setRGBColor(eCyan);
    while(1){
      endTime = millis();
      if((endTime - windSpeedstartTime) > 3000){
        windSpeedstartTime = millis();
        wSpeedRecord = ((3.14 * (pGeneral->sRadial /10.0) * 2.0 * pGeneral->interrcount) / 1.0 );
        pGeneral->interrcount = 0;
      }
      i++;
      if(i == (1*3000 + 500)){
        if(pGeneral->sWindFlage == 1){
          pGeneral->sAtPresentSpeed1 = wSpeedRecord/100.0;
          writeAtpresent(pGeneral->sAtPresentSpeed1);
        }else if(pGeneral->sWindFlage == 2){
          pGeneral->sAtPresentSpeed2 = wSpeedRecord/100.0;
        }
        break;
      }
      delay(1);
    }
    RGBEnd(0);
    i = 0;
    pGeneral->sWindFlage =0;
  }
  
  if(pGeneral->sCommunicationMode == UART){
    uartloop();
  }else{
    i2cloop();
  }
#if 1
 if(pGeneral->sLowPower == ON){
    i++;
    if(i == 4000){
      i = 0;
      if(pGeneral->sFlashWrite == ON){
        PCF8563T_1.cleanState();
        sensor_call();
        timeData = PCF8563T_1.getHourMinuteSecond();
        csvRecord();
        delay(50);
        if(pGeneral->sDTUSwitch == ON){
          publishData();
        }
        setRGBColor(eBlack);
        powerOFF();
        delay(100);
        noInterrupts();
        xosc_dormant();
        initPeripheral();
        interrupts();
        wifi();
      }
    }
 }else{
  if( pGeneral->sReadData == ON){
    pGeneral->sReadData = OFF;
    PCF8563T_1.cleanState();
    timeData = PCF8563T_1.getHourMinuteSecond();
    sensor_call();
    csvRecord();
    delay(50);
    if(pGeneral->sDTUSwitch == ON){
      publishData();
    }
  }
}

#endif
delay(1);
}