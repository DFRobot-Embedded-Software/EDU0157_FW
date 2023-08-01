#include "general.h"

sGeneral_t gereral;
sGeneral_t *pGeneral = &gereral;
float wSpeed = 0.00;
//DFRobot_PCF8563T rtc;
String timeData = "";
extern sPCF8563TTime_t rtt;

struct sDTUData *DTU_HEAD = NULL;
struct sDTUData* DTU_TAIL = NULL;
extern float wSpeed;
extern String widDir;

extern struct I2CSensorInfo *I2C_SENSOR_INFO_HEAD;
extern Adafruit_SPIFlash flash;
extern FatFileSystem fatfs;

void initBoard(void)
{
  // Serial1.setRX(1);
  // Serial1.setTX(0);
  // Serial1.begin(9600);
  
  #if 1
  initStruct();//初始化通用结构体
  
  initPin();//初始化功能引脚
  powerON();//打开电源
  initRGB();//初始化RGB灯
  setRGBColor(pGeneral->sRGBColor);
  
  initUdisk();//初始化U盘
  delay(1200);
  Serial.begin(115200);
  Wire1.setSDA(2);
  Wire1.setSCL(3);
  Wire1.begin();
  EEPROM.begin(512);
  
  #endif
  
  //while ( !Serial ) delay(10);//等待串口打开
  
  SOF_WIRE1_INIT();
  I2C_SENSOR_REGISTER();
  I2C_ADDR_SKU_REGISTER();
  I2C_scan();
  sensor_setup();
  sensor_call();
  #if 1
  creatorStandardConfigFileAndDTUFile();//初始化U盘中文件,如果没有就进行创建
  creatorLogFile();//创建log记录文件
  readstandardConfigFile();//获取标准配置文件中内容
  DTUParsing();//解析DTU文件
  #endif
  configBoard();//开始初始化板载传感器

  timeData = PCF8563T_1.getHourMinuteSecond();
  Serial.println("init OK");
}

void initStruct(void)
{
  pGeneral->sFlashWrite = ON;
  pGeneral->sUsbState = OFF;
  pGeneral->sCommunicationMode = I2C;
  pGeneral->sRGBColor = eYellow;
  pGeneral->sStandardConfigFileState = 0;
  pGeneral->sRadial = 23.75;
  pGeneral->sRecord = OFF;
  pGeneral->sLightSwitch = OFF;
  pGeneral->sLowPower = OFF;
  pGeneral->sDelayRecord = 10 *1000;
  pGeneral->sSampleRate = 1;
  pGeneral->sUnit = SECOND;
  pGeneral->sDTUSwitch = OFF;
  pGeneral->configState = OFF;
  pGeneral->DTUState = OFF;
  pGeneral->timeConfig = OFF;
  pGeneral->saveStateTime = OFF;
  pGeneral->sReadData = OFF;
  pGeneral->sDtuState = 0;
  pGeneral->sRecord= 0;
  pGeneral->sOffset = -0.36;
  pGeneral->sLinearFactor = 1.29;
  pGeneral->sWindFlage =0;
}


void configBoard(void){
  
  if(pGeneral->sLowPower == ON){
    pGeneral->sLightSwitch = OFF;
  }
  if(pGeneral->timeConfig == ON){
    PCF8563T_1.setTime(rtt);
    pGeneral->timeConfig = OFF;
  }
  if(pGeneral->sLightSwitch == OFF){
    setRGBColor(eBlack);
  }else{
    if(pGeneral->sDTUSwitch == ON){
      setRGBColor(eViolet);
    }else{
      if(pGeneral->sCommunicationMode == I2C){
        setRGBColor(eGreen);
      }else if(pGeneral->sCommunicationMode == UART){  
        setRGBColor(eBlue);
      }
    }
  }
  
  interruptConfig();
  
  if(pGeneral->sLowPower == ON){
    gpio_set_dormant_irq_enabled(8,GPIO_IRQ_EDGE_FALL,true);
    gpio_set_dormant_irq_enabled(12,GPIO_IRQ_EDGE_RISE|GPIO_IRQ_EDGE_FALL,true);
    gpio_set_dormant_irq_enabled(13,GPIO_IRQ_EDGE_RISE|GPIO_IRQ_EDGE_FALL,true);
    gpio_set_dormant_irq_enabled(14,GPIO_IRQ_EDGE_RISE|GPIO_IRQ_EDGE_FALL,true);
    gpio_set_dormant_irq_enabled(15,GPIO_IRQ_EDGE_RISE|GPIO_IRQ_EDGE_FALL,true);
  }
  
 
  
  if(pGeneral->sLowPower == ON){
    if(pGeneral->sUnit == SECOND){
      if(pGeneral->sSampleRate < 60){
        pGeneral->sSampleRate = 60;
      }
    }
  }else{
    (pGeneral->sCommunicationMode == I2C)? i2cSlaveInit():initSerialSlave();
  }
  wifi();
  PCF8563T_1.setCountDown();//设置采样周期
  initEeprom();
}


char* getFileName(void){
  return  PCF8563T_1.convertDataFileName(PCF8563T_1.get_rtc());
}

void publishData(void)
{
  struct sDTUData * p = DTU_HEAD;
  struct I2CSensorInfo *p1 = I2C_SENSOR_INFO_HEAD;
  if(pGeneral->sDtuState == 1){//开DTU
    RGBStart(1);
    while(p){
      if(String(p->key) == "Speed"){
          obloq.publish(String(p->topic),String(wSpeed,1));
        }else if(String(p->key) == "Dir"){
          obloq.publish(String(p->topic),widDir);
        }
          while(p1){
            if(p1->attr && p1->attr->attr){
              struct keyValue *pkey = p1->attr->attr;
              while(pkey){
                if(pkey->key.equals(String(p->key))){
                  obloq.publish(String(p->topic),pkey->value);
                }
                pkey = pkey->next;
              }
            }
            p1 = p1->next;
          }
      p1 = I2C_SENSOR_INFO_HEAD;
      p = p->next;
    }
    RGBEnd(1);
   }
}

void wifi(void)
{
  if(pGeneral->sDTUSwitch == ON){//连接WiFi订阅topic
    obloq.reset();
    delay(1000);
    if(pGeneral->sUnit == SECOND){
      if(pGeneral->sSampleRate < 3){
        pGeneral->sSampleRate = 3;
      }
    }
    // WiFi连接
    if(obloq.wifiConnect(pGeneral->sDTUWIFISSID, pGeneral->sDTUWIFIPASSWORD) == 1){
      if(obloq.mqttConnect(pGeneral->sDTUID,pGeneral->sDTUPWD,pGeneral->sDTUIP,pGeneral->sDTUPort) == 1){
        pGeneral->sDtuState = 1;//初始联网成功
        pGeneral->sDTUConnectState = 1;//联网成功
        
      }else{
        setRGBColor(eYellow);
      }
    }else{
      setRGBColor(eYellow);
    }
  }
}

void initPeripheral(void)
{
  xosc_init();
  set_sys_clock_khz(125000000 / 1000, true);
  powerON();//打开电源
}

void RGBEnd(uint8_t location){
    //if(pGeneral->sLowPower == OFF){
        if(pGeneral->sLightSwitch == OFF){
            setRGBColor(eBlack);
        }else{
            if(pGeneral->sDTUSwitch == ON){
                if(location == 0){
                  RGBFundamental();
                }else{
                  if(pGeneral->sDTUConnectState == 1){
                    setRGBColor(eViolet);
                  }else{
                    setRGBColor(eYellow);
                  }
                }
              }else{
                RGBFundamental();
          }
        }
    //} 
}

void RGBStart(uint8_t location){
    //if(pGeneral->sLowPower == OFF){
        if(pGeneral->sLightSwitch == OFF){
            if(pGeneral->sDTUSwitch == ON){
                if(location == 0){
                  RGBFundamental();
                }else{
                  if(pGeneral->sDTUConnectState == 1){
                    setRGBColor(eViolet);
                  }else{
                    setRGBColor(eYellow);
                  }
                  
                }
            }else{
                RGBFundamental();
            }
        }else{
            setRGBColor(eBlack);
        }
    //}
}

void RGBFundamental(void)
{
  if(pGeneral->sCommunicationMode == I2C){
    setRGBColor(eGreen);
  }else if(pGeneral->sCommunicationMode == UART){
    setRGBColor(eBlue);
  }
}
