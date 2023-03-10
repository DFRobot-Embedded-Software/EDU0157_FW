/*!
 * @file global.cpp
 * @brief 为了防止变量被重复定义，导致编译不过，所以在这里定义了此项目中需要用到的所有的全局变量
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-08-02
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 */

#include "global.h"
DFRobot_MAX17043 power;
sGeneral_t gereral;
sGeneral_t *pGeneral_t = &gereral;
float wSpeed = 0.00;
float voltage = 0.00;
float percentage = 0.00;
String timeData = "";


Adafruit_NeoPixel strip = Adafruit_NeoPixel(2, W2812_PIN, NEO_GRB + NEO_KHZ800);
uint16_t angle = 0;
uint8_t readState = ACCEPT_START;


void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  pGeneral_t->colorcount++;
  if(pGeneral_t->colorcount == 50){
    pGeneral_t->colorcount = 0;
    strip.show();
  }
    
}
String widDir = "N";
uint8_t dir = 0;
String getCompassData(void)
{
    
//dir |= digitalRead(TMR1302S_U13_PIN)<<3 | digitalRead(TMR1302S_U6_PIN)<<2 | digitalRead(TMR1302S_U9_PIN)<<1 | digitalRead(TMR1302S_U11_PIN)<<0;
    GLOBAL_DBG(dir,HEX);
    if((dir == 0x0E && angle == 45) || (dir == 0x0E && angle == 315)){
        angle=0;  
        widDir = "N";
    }
    if((dir == 0x03 && angle == 90) || (dir == 0x03 && angle == 0)){
        angle=45;  
        widDir = "NE";
    } 
    if((dir == 0x0D && angle == 135) || (dir == 0x0D && angle == 45)){
        angle=90;   
        widDir = "E";
    }
    if((dir == 0x06 && angle == 180) || (dir == 0x06 && angle == 90)){
        angle=135;  
        widDir = "SE";
     }
    if((dir == 0x0B && angle == 225) || (dir == 0x0B && angle == 135)){
        angle=180; 
        widDir = "S";
    }
    if((dir == 0x0C && angle == 270) || (dir == 0x0C && angle == 180)){
        angle=225;  
        widDir = "SW";
    }
    if((dir == 0x07 && angle == 315) || (dir == 0x07 && angle == 225)){
        angle=270;   
        widDir = "W";
    }
    if((dir == 0x09 && angle == 0) || (dir == 0x09 && angle == 270)){
        angle=315;
        widDir = "NW";
    }
    return widDir;
}

void windSpeed(){
  (pGeneral_t->interrcount)++;
}

void A()
{ 
    pGeneral_t->interruptData = 1;
 // dir = 0;
  //dir |= digitalRead(TMR1302S_U13_PIN)<<3 | /*B*/digitalRead(TMR1302S_U6_PIN)<<2 | /*C*/digitalRead(TMR1302S_U9_PIN)<<1 | /*D*/digitalRead(TMR1302S_U11_PIN)<<0;
  //Serial.println("a");
  //getCompassData();
}

void B()
{
    pGeneral_t->interruptData = 1;
  //dir = 0;
  //dir |= digitalRead(TMR1302S_U13_PIN)<<3 | /*B*/digitalRead(TMR1302S_U6_PIN)<<2 | /*C*/digitalRead(TMR1302S_U9_PIN)<<1 | /*D*/digitalRead(TMR1302S_U11_PIN)<<0;
   //Serial.println("b");
  //getCompassData();
}

void C()
{
    pGeneral_t->interruptData = 1;
  //dir = 0;
  //dir |= digitalRead(TMR1302S_U13_PIN)<<3 | /*B*/digitalRead(TMR1302S_U6_PIN)<<2 | /*C*/digitalRead(TMR1302S_U9_PIN)<<1 | /*D*/digitalRead(TMR1302S_U11_PIN)<<0;
   //Serial.println("c");
 //getCompassData();
}

void D()
{
    pGeneral_t->interruptData = 1;
  //dir = 0;
  //dir |= digitalRead(TMR1302S_U13_PIN)<<3 | /*B*/digitalRead(TMR1302S_U6_PIN)<<2 | /*C*/digitalRead(TMR1302S_U9_PIN)<<1 | /*D*/digitalRead(TMR1302S_U11_PIN)<<0;
   //Serial.println("d");
  //getCompassData();
}

void begin()
{  
  setGreen();
  pGeneral_t->interrcount = 0;
  pGeneral_t->saveState = DATA_NOT_STORE;
  pGeneral_t->communicationMode = I2CMODE;
  pGeneral_t->sampleRate = 1000;
  pGeneral_t->ligthState =COLOR_ERROR;
  pGeneral_t->interrcount = 0;
  pGeneral_t->interruptData = 0;
  //调试串口初始化
  Serial.begin(115200);
  Wire1.setSDA(2);
  Wire1.setSCL(3);
  Wire1.begin();
  pinMode(TMR1302S_POWER_PIN,OUTPUT);
  pinMode(WIRE_POWER_PIN,OUTPUT);
  pinMode(EXTEND_POWER,OUTPUT);
  digitalWrite(TMR1302S_POWER_PIN, HIGH);
  digitalWrite(WIRE_POWER_PIN,HIGH);
  digitalWrite(EXTEND_POWER,LOW);

  // //pinMode(SCO1_INPUT_PIN,INPUT);//触摸输入
  // //使能
  pinMode(TMR1302S_U9_PIN,INPUT);
  pinMode(TMR1302S_U6_PIN,INPUT);
  pinMode(TMR1302S_U11_PIN,INPUT);
  pinMode(TMR1302S_U13_PIN,INPUT);
  pinMode(TMR1302S_EXTEND_PIN,INPUT);
  ud_setup();//初始化U盘
  delay(1000);
  configfileRead();//解析配置文件中内容
 //delay(100);
  I2C_SENSOR_REGISTER();
  I2C_ADDR_SKU_REGISTER();
  SOF_WIRE1_INIT();
  I2C_scan();
  sensor_setup();
  (pGeneral_t->communicationMode == I2CMODE)? i2cSlaveInit():initSerialSlave();//通过模式不用初始化为不同通信方式

  //rtc_setup();
  //attachInterrupt(digitalPinToInterrupt(SCO1_INPUT_PIN), interruptTouch, CHANGE);

  attachInterrupt(digitalPinToInterrupt(TMR1302S_EXTEND_PIN), windSpeed, CHANGE);

  attachInterrupt(digitalPinToInterrupt(TMR1302S_U13_PIN), D, CHANGE);
  attachInterrupt(digitalPinToInterrupt(TMR1302S_U6_PIN), C, CHANGE);//a
  attachInterrupt(digitalPinToInterrupt(TMR1302S_U9_PIN), B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(TMR1302S_U11_PIN), A, CHANGE);
  if(power.begin() != 0){//初始化MAX17043
    GLOBAL_DBG("Power begin error !!!");
    //pGeneral_t->ligthState = COLOR_ERROR;
  }
  readState = ACCEPT_START;
  GLOBAL_DBG("begin ok");
}




void indicatorColor(void){
    switch(pGeneral_t->ligthState){
        case COLOR_GREEN:
            colorWipe(strip.Color(0, 50, 0), 50); // Green
            break;
        case COLOR_BLUE:
            colorWipe(strip.Color(0, 0, 50), 0); // Blue
            break;
        case COLOR_FLICKER1:
            pGeneral_t->ligthState = 0x87;
            colorWipe(strip.Color(50, 0, 20), 50); // Yellow
            break;
        case 0x87:
            pGeneral_t->ligthState = COLOR_FLICKER1;
            colorWipe(strip.Color(0, 50, 0), 50); // Green
            break;
        case COLOR_FLICKER2:
            colorWipe(strip.Color(50, 0, 20), 50); // Yellow
            colorWipe(strip.Color(0, 0, 50), 50); // Blue
            break;
        case COLOR_ERROR:
            colorWipe(strip.Color(50, 0, 0), 50); // Red
            break;
        case COLOR_OFF:
            colorWipe(strip.Color(0, 0, 0), 50); // Red
            break;
        default:
            break;
    }
}
void setGreen(void){
    colorWipe(strip.Color(0, 50, 0), 50); // Green
}

void sensorDataCollect(void){
    voltage = power.readVoltage();
    percentage = power.readPercentage();
    sensor_call();
    timeData = getHourMinuteSecond();
    csvRecord();
}

void dataAnalysls(void* buf){
    uint8_t* pBuf = (uint8_t*)buf;
}

struct I2CSensorListTable *I2C_SENSOR_HEAD = NULL;
struct I2CSensorListTable *I2C_SENSOR_TAIL = NULL;
uint32_t  I2C_SENSOR_COUNT = 0;
void I2C_SENSOR_ENQUEUE(const char *sku, const char * name, I2C_SENSOR_SETUP setup, I2C_SENSOR_FUN fun){
    struct I2CSensorBase *base = NULL;
    struct I2CSensorListTable *p = NULL;
    base = new I2CSensorBase;
    if(base == NULL) return;
    p = new I2CSensorListTable;
    if(p == NULL) {
        delete(base);
        return;
    }
    base->sku  = String(sku);
    base->name = String(name);
    base->setup= setup;
    base->call = fun;
    p->next    = NULL;
    p->base    = base;
    if(I2C_SENSOR_HEAD == NULL){
        I2C_SENSOR_HEAD = p;
        I2C_SENSOR_TAIL = p;
    }else{
        I2C_SENSOR_TAIL->next = p;
        I2C_SENSOR_TAIL       = p;
    }
    I2C_SENSOR_COUNT += 1;
}

struct I2CSensorBase *getI2CSensor(const char *sku){
    struct I2CSensorListTable *p = I2C_SENSOR_HEAD;
    struct I2CSensorBase *base = NULL;
    while(p){
        base = p->base;
        if(base){
            if(base->sku.equals(String(sku))) return base;
        }
        p = p->next;
    }
    return NULL;
}

void I2C_ADDR_SKU_ENQUEUE(uint8_t addr, const char * sku){
    struct i2cAddrIndexList *p = I2C_ADDR_SKU_HEAD;
    struct i2cSKU *psku = NULL;
    struct I2CSensorBase *base = getI2CSensor(sku);
    if(base == NULL) return;
    while(p){
        if(p->addr == addr){
            //表示此地址已有
            psku = p->head;
            while(psku){
                if(psku->base){
                    if(psku->base->sku.equals(String(sku))) return;
                }
                psku = psku->next;
            }
            psku = new i2cSKU;
            if(psku == NULL) return;
            psku->base = base;
            psku->online = 0;
            psku->next = NULL;
            if(p->head == NULL){
                p->head = psku;
                p->tail = psku;
                p->count = 1;
            }else{
                p->tail->next = psku;
                p->tail       = psku;
                p->count += 1;
            }
            return;
        }
        p = p->next;
    }

    p = new i2cAddrIndexList;
    if(p == NULL) return;
    psku = new i2cSKU;
    if(psku == NULL){
        delete(p);
        return;
    }
    psku->next   = NULL;
    psku->online = 0;
    psku->base   = base;

    p->next = NULL;
    p->addr = addr;
    p->count  = 1;
    p->online = 0;
    p->head = psku;
    p->tail = psku;
    if(I2C_ADDR_SKU_HEAD == NULL){
        I2C_ADDR_SKU_HEAD = p;
        I2C_ADDR_SKU_TAIL = p;
        I2C_ADDR_SKU_COUNT = 1;
    }else{
        I2C_ADDR_SKU_TAIL->next = p;
        I2C_ADDR_SKU_TAIL       = p;
        I2C_ADDR_SKU_COUNT += 1;
    }
}

// void I2C_ADDR_SKU_FLAG_CLEAR(uint8_t ifn){
//     struct i2cAddrIndexList *p = I2C_ADDR_SKU_HEAD;
//     while(p){
//         if(ifn){
//             p->if2  = 0;
//             p->sel2 = 0;
//         }else{
//             p->if1  = 0;
//             p->sel1 = 0;
//         }
//         p = p->next;
//     }
// }

struct i2cAddrIndexList *I2C_ADDR_SKU_HEAD = NULL;
struct i2cAddrIndexList *I2C_ADDR_SKU_TAIL = NULL;
struct i2cAddrIndexList *I2C_SKU_SEL_HEAD  = NULL;
struct i2cAddrIndexList *I2C_SKU_SEL_TAIL  = NULL;
uint8_t I2C_SKU_SEL_COUNT  = 0;
uint8_t I2C_ADDR_SKU_COUNT = 0;
bool I2C_SKU_SEL_ENQUEUE(struct i2cAddrIndexList *p){
    if(p == NULL){
        GLOBAL_DBG("p is null");
        return false;
    }
    if(p->head == NULL){
        GLOBAL_DBG("p->head is null");
        return false;
    }
    if(p->head->base == NULL){
        GLOBAL_DBG("p->head->base is null");
        return false;
    }
    
    struct i2cAddrIndexList *psensor = new i2cAddrIndexList;
    
    if(psensor == NULL) return false;
    
    psensor->next    = NULL;
    psensor->addr  = p->addr;
    psensor->count = p->count;
    psensor->online= p->online;
    psensor->head  = p->head;
    psensor->tail  = p->tail;

    if(I2C_SKU_SEL_HEAD == NULL){
        I2C_SKU_SEL_HEAD  = psensor;
        I2C_SKU_SEL_TAIL  = psensor;
        I2C_SKU_SEL_COUNT = 1;
    }else{
        I2C_SKU_SEL_TAIL->next = psensor;
        I2C_SKU_SEL_TAIL       = psensor;
        I2C_SKU_SEL_COUNT += 1;
    }
    return true;
}

void I2C_SKU_SEL_REMOVE(struct i2cAddrIndexList *p, uint8_t ifn){
    if((p == NULL) || (p->head = NULL) || (p->head->base == NULL)) return;
    struct i2cAddrIndexList *psensor = I2C_SKU_SEL_HEAD;
    struct i2cAddrIndexList *head = NULL;
    while(psensor){
        if(psensor->addr == p->addr){
            //到底需不需要移除
            struct i2cSKU *psku = psensor->head;
            if(ifn == 1) p->if1 = psensor->if1;
            if(ifn == 2) p->if2 = psensor->if2;
            if((ifn == 1) && psku) psku->if1 = 0;
            if((ifn == 2) && psku) psku->if2 = 0;
            
            if((ifn == 1) && (psensor->sel2 == 1)){
                psensor->sel1 = 0;
                return;
            }
            if((ifn == 2) && (psensor->sel1 == 1)){
                psensor->sel2 = 0;
                return;
            }
            if(head == NULL) head = psensor->next;
            else head->next = psensor->next;
            if(head == NULL){
                I2C_SKU_SEL_HEAD = NULL;
                I2C_SKU_SEL_TAIL = NULL;
            }else if(head->next == NULL){
                I2C_SKU_SEL_TAIL = head;
            }
            psensor->head = NULL;
            psensor->tail = NULL;
            delete(psensor);
            if(I2C_SKU_SEL_COUNT) I2C_SKU_SEL_COUNT -= 1;
            return;
        }
        head = psensor;
        psensor = psensor->next;
    }
}

void I2C_ADDR_SEL_REMOVE_CLEAR(uint8_t ifn){
    struct i2cAddrIndexList *p  = I2C_SKU_SEL_HEAD;
    struct i2cAddrIndexList *pi = p;
    while(p){
        if(ifn){
            p->if2  = 0;
            p->sel2 = 0;
        }else{
            p->if1  = 0;
            p->sel1 = 0;
        }
        struct i2cSKU *psku  = p->head;
        struct i2cSKU *pskui = psku;
        while(psku){
            if(ifn){
                psku->if2 = 0;
            }else{
                psku->if1 = 0;
            }
            psku = psku->next;
            if((p->sel1 == 0) && (p->sel2 == 0)){
                psku->base = NULL;
                delete(pskui);
            }
            pskui = psku;
        }
        p = p->next;
        if((p->sel1 == 0) && (p->sel2 == 0)){
            delete(pi);
        }
        pi = p;
    }
}

void SOF_WIRE1_INIT(){
    SOF_WIRE1.begin();
}

struct I2CSensorInfo *I2C_SENSOR_INFO_HEAD = NULL;
struct I2CSensorInfo *I2C_SENSOR_INFO_TAIL = NULL;
void I2C_scan(){
    //查询扩展板上有那些传感器
    struct i2cAddrIndexList *pi2c = I2C_ADDR_SKU_HEAD;
    struct I2CSensorInfo *pi3c = I2C_SENSOR_INFO_HEAD;
    uint8_t indec = 1;
    while(pi2c){
      struct I2CSensorInfo *pi3c = I2C_SENSOR_INFO_HEAD;
      //change和init用来判断I2C接口和相关配置是否初始化完成， mode用来判断是否为I2C接口模式
      //Serial.print("I2C addr SCANING ");Serial.print(indec++);Serial.print(": ");Serial.println(pi2c->addr, HEX);
      //
      ////for(int i = 1; i < 127; i++){
      //  SOF_WIRE1.beginTransmission(pi2c->addr);
      //  if(SOF_WIRE1.endTransmission() == 0){
      //    Serial.print("IF1 found addr: ");
      //    Serial.println(pi2c->addr, HEX);
      //  }
      //  SOF_WIRE2.beginTransmission(pi2c->addr);
      //  if(SOF_WIRE2.endTransmission() == 0){
      //    Serial.print("IF2 found addr: ");
      //    Serial.println(pi2c->addr, HEX);
      //  }
      ////}
      //Serial.println("END\n");
        SOF_WIRE1.beginTransmission(pi2c->addr);
        if(SOF_WIRE1.endTransmission() == 0){
          //GLOBAL_DBG(pi2c->addr);
          while(pi3c){
            if((pi2c->addr) == (pi3c->addr)){
              pGeneral_t->ligthState = COLOR_ADDR_REPETITION;
              GLOBAL_DBG("ADDR conflict");
            }
            pi3c = pi3c->next;
          }
           ////在接口1上检测到了地址为 p->addr 的传感器
          //if(pi2c->count == 1){
          //   //此地址只对应一个sku
          //   if(pi2c->if1 == 0){
              //此传感器未被加入采集列表
              pi2c->online = 1;//将该地址设置为在线设备
              if(pi2c->head && pi2c->head->base){
                struct I2CSensorInfo *pinfo = new I2CSensorInfo;
                if(pinfo){
                  pinfo->base = pi2c->head->base;
                  pinfo->attr = NULL;
                  pinfo->addr = pi2c->addr;
                  pinfo->next = NULL;
                  if(I2C_SENSOR_INFO_HEAD == NULL){
                    I2C_SENSOR_INFO_HEAD = pinfo;
                    I2C_SENSOR_INFO_TAIL = pinfo;
                    //sDeviceReg.REG_IF1->count = 1;
                  }else{
                    I2C_SENSOR_INFO_TAIL->next = pinfo;
                    I2C_SENSOR_INFO_TAIL       = pinfo;
                   // sDeviceReg.REG_IF1->count += 1;
                  }
                  //sDeviceReg.REG_IF1->head = phead;
                  //sDeviceReg.REG_IF1->tail= ptail;
                  pi2c->if1 = 1;
                  pi2c->head->if1 = 1;  //SKU被加入
                }else{
                  GLOBAL_DBG("IF1: pinfo is null");
                }
              }else{
                GLOBAL_DBG("IF1: pi2c->head or pi2c->head->base is null");
              }
          // }else{
          //   GLOBAL_DBG(pi2c->addr);
          //   GLOBAL_DBG(" already add in IF1.");
          // }
          // }else if(pi2c->count > 1){
          //   //此地址对应多个sku
          //   if(pi2c->sel1 == 0){
          //     //此地址还未被加入多地址选择列表 I2C_ADDR_SKU_HEAD
          //     pi2c->sel1 = 1;
          //     I2C_SKU_SEL_ENQUEUE(pi2c);
          //    // sDeviceReg.REG_IF1->sel = 1;
              
          //   }else{
              
          //   }
          //  }
        }
    //     else{//此处用来动态检测扩展用，暂时不用
    //       //未在接口1上检测到了地址为 p->addr 的传感器
    //       GLOBAL_DBG(p->addr, HEX);
    //       GLOBAL_DBG(" is not found in IF1");
    //       if(pi2c->sel1){
    //         //此地址还未从多地址选择列表移除
    //         I2C_SKU_SEL_REMOVE(pi2c, 1);
    //         pi2c->sel1 = 0;
    //       }
    //       if(pi2c->if1){
    //         //此地址还未从IF1数据采集列表移除
    //         struct I2CSensorInfo *phead = (struct I2CSensorInfo *)sDeviceReg.REG_IF1->head;
    //         struct I2CSensorInfo *pindex = NULL;
    //         while(phead){
    //           if(phead->addr == pi2c->addr){
    //             if(pindex == NULL) pindex = phead->next;
    //             else pindex->next =  phead->next;
                
    //             phead->base = NULL;
    //             phead->next = NULL;
    //             if(phead->attr){
    //               struct keyValue *pkey  = phead->attr->attr;
    //               struct keyValue *pkeyi = pkey;
    //               while(pkey){
    //                 pkey = pkey->next;
    //                 delete(pkeyi);
    //                 pkeyi = pkey;
    //               }
    //               delete(phead->attr);
    //             }
    //             delete(phead);
    //             if(pindex == NULL){
    //               sDeviceReg.REG_IF1->head = NULL;
    //               sDeviceReg.REG_IF1->tail = NULL;
    //             }else if(pindex->next == NULL){
    //               sDeviceReg.REG_IF1->tail = pindex;
    //             }
    //             if(sDeviceReg.REG_IF1->count) sDeviceReg.REG_IF1->count -= 1;
    //             break;
    //           }
    //           pindex = phead;
    //           phead = phead->next;
    //         }
    //         pi2c->if1 = 0;
    //       }
    //   }

    pi2c = pi2c->next;
    }
    pi3c = I2C_SENSOR_INFO_HEAD;
    if(pi3c == NULL)
      pGeneral_t->ligthState =COLOR_ERROR;
}

void sensor_setup(){
  struct I2CSensorInfo *info = I2C_SENSOR_INFO_HEAD;
  while(info){
    if(info && (info->attr == NULL) && info->base && info->base->setup) info->attr = info->base->setup(1, info->addr);
    GLOBAL_DBG(info->addr);
    info = info->next;
  }
}

void sensor_call(){
  struct I2CSensorInfo *info = I2C_SENSOR_INFO_HEAD;
  while(info){
    if(info && info->base && info->attr && info->base->call) info->base->call(1, info->attr->attr);
    info = info->next;
  }
}