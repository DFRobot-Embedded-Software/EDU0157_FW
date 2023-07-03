/*!
 * @file sensor_i2c.h
 * @brief UART类型传感器注册
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-08-02
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 */

#include "sensor_i2c.h"

struct I2CSensorListTable *I2C_SENSOR_HEAD = NULL;
struct I2CSensorListTable *I2C_SENSOR_TAIL = NULL;
uint32_t  I2C_SENSOR_COUNT = 0;

struct i2cAddrIndexList *I2C_ADDR_SKU_HEAD = NULL;
struct i2cAddrIndexList *I2C_ADDR_SKU_TAIL = NULL;
struct i2cAddrIndexList *I2C_SKU_SEL_HEAD  = NULL;
struct i2cAddrIndexList *I2C_SKU_SEL_TAIL  = NULL;
uint8_t I2C_SKU_SEL_COUNT  = 0;
uint8_t I2C_ADDR_SKU_COUNT = 0;

struct I2CSensorInfo *I2C_SENSOR_INFO_HEAD = NULL;
struct I2CSensorInfo *I2C_SENSOR_INFO_TAIL = NULL;
extern sGeneral_t *pGeneral;

void I2C_SENSOR_REGISTER(){
  
  I2C_SENSOR_ENQUEUE("SEN0497", "Temp&Humi",     SEN0497_SETUP, SEN0497_FUN);
  I2C_SENSOR_ENQUEUE("SEN0514", "Air Quality",   SEN0514_SETUP, SEN0514_FUN);
  I2C_SENSOR_ENQUEUE("SEN0334", "Temp&Humi",     SEN0334_SETUP, SEN0334_FUN);
  I2C_SENSOR_ENQUEUE("SEN0228", "Ambient Light", SEN0228_SETUP, SEN0228_FUN);
  I2C_SENSOR_ENQUEUE("DFR0216", "UNO",           DFR0216_SETUP, DFR0216_FUN);
  I2C_SENSOR_ENQUEUE("SEN0322", "O2",            SEN0322_SETUP, SEN0322_FUN);
  I2C_SENSOR_ENQUEUE("SEN0364", "Visible light", SEN0364_SETUP, SEN0364_FUN);

  I2C_SENSOR_ENQUEUE("SEN0517", "Pressure",      SEN0517_SETUP, SEN0517_FUN);

  //I2C_SENSOR_ENQUEUE("SEN0456", "Air Pressure",  SEN0456_SETUP, SEN0456_FUN);
  I2C_SENSOR_ENQUEUE("SEN0529", "Geomagnetic",   SEN0529_SETUP, SEN0529_FUN);
  I2C_SENSOR_ENQUEUE("SEN0518", "Heart rate",    SEN0518_SETUP, SEN0518_FUN);
  I2C_SENSOR_ENQUEUE("SEN0304", "Distance",      SEN0304_SETUP, SEN0304_FUN);
  I2C_SENSOR_ENQUEUE("SEN0321", "O3",            SEN0321_SETUP, SEN0321_FUN);
  I2C_SENSOR_ENQUEUE("SEN0414", "Temp&Humi",    SEN0414_SETUP, SEN0414_FUN);
  
  //I2C_SENSOR_ENQUEUE("SEN0498", "Temp&Humi",   SEN0497_SETUP, SEN0497_FUN);
  I2C_SENSOR_ENQUEUE("SEN0460", "PM2.5",       SEN0460_SETUP, SEN0460_FUN);
  I2C_SENSOR_ENQUEUE("SEN0206", "IR Temp",     SEN0206_SETUP, SEN0206_FUN);
  I2C_SENSOR_ENQUEUE("SEN0291", "Power meter", SEN0291_SETUP, SEN0291_FUN);
  I2C_SENSOR_ENQUEUE("SEN0536", "Power meter", SEN0536_SETUP, SEN0536_FUN);
  I2C_SENSOR_ENQUEUE("TEL0157", "GNSS",        TEL0157_SETUP, TEL0157_FUN);
  I2C_SENSOR_ENQUEUE("MAX17043", "MAX17043",   MAX17043_SETUP, MAX17043_FUN);
  //I2C_SENSOR_ENQUEUE("PCF8563T", "PCF8563T",   PCF8563T_SETUP, PCF8563T_FUN);

  
   
  //以后如果增加了新的传感器，则继续注册
}

void I2C_ADDR_SKU_REGISTER(void){
  
  I2C_ADDR_SKU_ENQUEUE(0x10, "SEN0228"); 
  I2C_ADDR_SKU_ENQUEUE(0x11, "SEN0304"); //1~127均可，可通过软件更改地址，默认0x11
  
  //I2C_ADDR_SKU_ENQUEUE(0x10, "SEN0529"); 
  //I2C_ADDR_SKU_ENQUEUE(0x11, "SEN0529"); 
  //I2C_ADDR_SKU_ENQUEUE(0x12, "SEN0529"); 
  
  I2C_ADDR_SKU_ENQUEUE(0x13, "SEN0529"); 
  
  //I2C_ADDR_SKU_ENQUEUE(0x16, "SEN0456");
  //I2C_ADDR_SKU_ENQUEUE(0x17, "SEN0456");
  //I2C_ADDR_SKU_ENQUEUE(0x18, "SEN0456");
  //I2C_ADDR_SKU_ENQUEUE(0x19, "SEN0456");
  
  I2C_ADDR_SKU_ENQUEUE(0x19, "SEN0460");
  I2C_ADDR_SKU_ENQUEUE(0x20, "TEL0157");
  I2C_ADDR_SKU_ENQUEUE(0x38, "SEN0497");

  I2C_ADDR_SKU_ENQUEUE(0x36, "MAX17043");
  
  I2C_ADDR_SKU_ENQUEUE(0x39, "SEN0364");
  
  I2C_ADDR_SKU_ENQUEUE(0x40, "SEN0291");
  //I2C_ADDR_SKU_ENQUEUE(0x41, "SEN0291");
  //I2C_ADDR_SKU_ENQUEUE(0x44, "SEN0291");
  //I2C_ADDR_SKU_ENQUEUE(0x45, "SEN0291");
  
  I2C_ADDR_SKU_ENQUEUE(0x44, "SEN0334");
  I2C_ADDR_SKU_ENQUEUE(0x45, "SEN0334");

  //I2C_ADDR_SKU_ENQUEUE(0x51, "PCF8563T");

  I2C_ADDR_SKU_ENQUEUE(0x52, "SEN0514");
  I2C_ADDR_SKU_ENQUEUE(0x53, "SEN0514");
  
  I2C_ADDR_SKU_ENQUEUE(0x55, "DFR0216");
  
  I2C_ADDR_SKU_ENQUEUE(0x57, "SEN0518");
  
  I2C_ADDR_SKU_ENQUEUE(0x5A, "SEN0206");
  
  I2C_ADDR_SKU_ENQUEUE(0x62, "SEN0536");

  I2C_ADDR_SKU_ENQUEUE(0x63, "SEN0517");


  I2C_ADDR_SKU_ENQUEUE(0x70, "SEN0414");
  I2C_ADDR_SKU_ENQUEUE(0x71, "SEN0321");
  I2C_ADDR_SKU_ENQUEUE(0x72, "SEN0322");
  //I2C_ADDR_SKU_ENQUEUE(0x72, "SEN0321");
  I2C_ADDR_SKU_ENQUEUE(0x73, "SEN0321");

  //I2C_ADDR_SKU_ENQUEUE(0x70, "SEN0322");
  //I2C_ADDR_SKU_ENQUEUE(0x71, "SEN0322");
  
  //I2C_ADDR_SKU_ENQUEUE(0x73, "SEN0322");
  //以后如果增加了新的传感器，则继续注册
}



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

void I2C_ADDR_SKU_ENQUEUE(uint8_t addr, const char * sku){
  struct i2cAddrIndexList *p = I2C_ADDR_SKU_HEAD;
  struct i2cSKU *psku = NULL;
  struct I2CSensorBase *base = getI2CSensor(sku);
  if(base == NULL) return;
  while(p){//查询地址是否有配置相同（定义暂时没有问题，保留项）
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
        //psku->online = 0;
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

bool I2C_SKU_SEL_ENQUEUE(struct i2cAddrIndexList *p){
  if(p == NULL){
    SENSOR_I2C_DBG("p is null");
    return false;
  }

  if(p->head == NULL){
    SENSOR_I2C_DBG("p->head is null");
    return false;
  }

  if(p->head->base == NULL){
    SENSOR_I2C_DBG("p->head->base is null");
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
    // struct i2cSKU *psku = psensor->head;
    // if(ifn == 1) p->if1 = psensor->if1;
    // if(ifn == 2) p->if2 = psensor->if2;
    // if((ifn == 1) && psku) psku->if1 = 0;
    // if((ifn == 2) && psku) psku->if2 = 0;
            
    // if((ifn == 1) && (psensor->sel2 == 1)){
    //   psensor->sel1 = 0;
    //   return;
    // }
    // if((ifn == 2) && (psensor->sel1 == 1)){
    //   psensor->sel2 = 0;
    //   return;
    // }
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
    // if(ifn){
    //   p->if2  = 0;
    //   p->sel2 = 0;
    // }else{
    //   p->if1  = 0;
    //   p->sel1 = 0;
    // }
    struct i2cSKU *psku  = p->head;
    struct i2cSKU *pskui = psku;
    while(psku){
      // if(ifn){
      //   psku->if2 = 0;
      // }else{
      //   psku->if1 = 0;
      // }
      // psku = psku->next;
      // if((p->sel1 == 0) && (p->sel2 == 0)){
      //   psku->base = NULL;
      //   delete(pskui);
      // }
      // pskui = psku;
    }
    p = p->next;
    // if((p->sel1 == 0) && (p->sel2 == 0)){
    //   delete(pi);
    // }
    pi = p;
  }
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


void I2C_scan(void){
  SENSOR_I2C_DBG("i2c_scan");
  //查询扩展板上有那些传感器
  struct i2cAddrIndexList *pi2c = I2C_ADDR_SKU_HEAD;
  struct I2CSensorInfo *pi3c = I2C_SENSOR_INFO_HEAD;
  while(pi2c){
    pi3c = I2C_SENSOR_INFO_HEAD;
    SOF_WIRE1.beginTransmission(pi2c->addr);
    if(SOF_WIRE1.endTransmission() == 0){
      SENSOR_I2C_DBG("SOF");
      SENSOR_I2C_DBG(pi2c->addr);
      while(pi3c){
        if((pi2c->addr) == (pi3c->addr)){
          pGeneral->sRGBColor = eRed;
          String log = "addr conflict ";
          log += (String)pi2c->addr;
          writeLog("I2C_scan",log);
        }
        pi3c = pi3c->next;
      }
      pi2c->online = 1;//将该地址设置为在线设备
      pi2c->port = SOFI2C;
      if(pi2c->head && pi2c->head->base){
        struct I2CSensorInfo *pinfo = new I2CSensorInfo;
        if(pinfo){
          pinfo->base = pi2c->head->base;
          pinfo->attr = NULL;
          pinfo->addr = pi2c->addr;
          pinfo->next = NULL;
          pinfo->port = SOFI2C;
          if(I2C_SENSOR_INFO_HEAD == NULL){
            I2C_SENSOR_INFO_HEAD = pinfo;
            I2C_SENSOR_INFO_TAIL = pinfo;
          }else{
            I2C_SENSOR_INFO_TAIL->next = pinfo;
            I2C_SENSOR_INFO_TAIL       = pinfo;
          }
        }else{
          writeLog("I2C_scan sof","pinfo is null");
          SENSOR_I2C_DBG(" I2C_scan sof ,pinfo is null");
        }
      }else{
        writeLog("I2C_scan sof","pi2c->head or pi2c->head->base is null");
        SENSOR_I2C_DBG(" I2C_scan sof ,pi2c->head or pi2c->head->base is null");
      }
   }else{//查询地址无应答
    // if((pi2c->online == 1) && (pi2c->port == SOFI2C)){//设备之前注册过，现在查询不在将他移除
    //   SENSOR_I2C_DBG(" I2C_scan sof , REMOVE_DATA_COLLECTION");
    //   REMOVE_DATA_COLLECTION(pi2c);
    // }
    delay(20);
    //SENSOR_I2C_DBG(pi2c->addr);
    Wire1.beginTransmission(pi2c->addr);
    if(Wire1.endTransmission() == 0){
      SENSOR_I2C_DBG("HARD");
      SENSOR_I2C_DBG(pi2c->addr);
      while(pi3c){
        if((pi2c->addr) == (pi3c->addr)){
          pGeneral->sRGBColor = eRed;
          String log = "addr conflict ";
          log += (String)pi2c->addr;
          writeLog("I2C_scan",log);
          SENSOR_I2C_DBG(" I2C_scan hard , addr conflict");
        }
        pi3c = pi3c->next;
      }
      pi2c->online = 1;//将该地址设置为在线设备
      pi2c->port = HARI2C;
      if(pi2c->head && pi2c->head->base){
        struct I2CSensorInfo *pinfo = new I2CSensorInfo;
        if(pinfo){
          pinfo->base = pi2c->head->base;
          pinfo->attr = NULL;
          pinfo->addr = pi2c->addr;
          pinfo->next = NULL;
          pinfo->port = HARI2C;
          if(I2C_SENSOR_INFO_HEAD == NULL){
            I2C_SENSOR_INFO_HEAD = pinfo;
            I2C_SENSOR_INFO_TAIL = pinfo;
          }else{
            I2C_SENSOR_INFO_TAIL->next = pinfo;
            I2C_SENSOR_INFO_TAIL       = pinfo;
          }
        }else{
          writeLog("I2C_scan","pinfo is null");
          SENSOR_I2C_DBG(" I2C_scan hard ,pinfo is null");
        }
      }else{
        writeLog("I2C_scan hard","pi2c->head or pi2c->head->base is null");
        SENSOR_I2C_DBG(" I2C_scan hard ,pi2c->head or pi2c->head->base is null");
      }
   }else{//查询地址无应答
    if((pi2c->online == 1) && (pi2c->port == HARI2C)){//设备之前注册过，现在查询不在将他移除
      SENSOR_I2C_DBG(" I2C_scan hard , REMOVE_DATA_COLLECTION");
      REMOVE_DATA_COLLECTION(pi2c);
    }
   }
   }
  delay(20);
  pi2c = pi2c->next;
 }
 pi3c = I2C_SENSOR_INFO_HEAD;
  if(pi3c == NULL){
    writeLog("I2C_scan","no sensor");
    SENSOR_I2C_DBG("no sensor");
    pGeneral->sRGBColor = eRed;
  }
    SENSOR_I2C_DBG("kk");
}

void SOF_WIRE1_INIT(){
    SOF_WIRE1.begin();
}

void REMOVE_DATA_COLLECTION(struct i2cAddrIndexList *p)
{
  struct I2CSensorInfo *phead = I2C_SENSOR_INFO_HEAD;
  struct I2CSensorInfo *pindex = NULL;
  struct i2cAddrIndexList *_p = p;
  while(phead){
    if(phead->addr == _p->addr){
      if(pindex == NULL) pindex = phead->next;
      else pindex->next =  phead->next;
      phead->base = NULL;
      phead->next = NULL;
      if(phead->attr){
        struct keyValue *pkey  = phead->attr->attr;
        struct keyValue *pkeyi = pkey;
        while(pkey){
          pkey = pkey->next;
          delete(pkeyi);
          pkeyi = pkey;
        }
        delete(phead->attr);
      }
      delete(phead);
      if(pindex == NULL){
        I2C_SENSOR_INFO_HEAD = NULL;
        I2C_SENSOR_INFO_TAIL = NULL;
      }else if(pindex->next == NULL){
        I2C_SENSOR_INFO_TAIL = pindex;
      }
      _p->count = 0;
      return;
    }
  pindex = phead;
  phead = phead->next;
  }
}

void deleteNode(struct I2CSensorInfo* head, uint8_t addr) {
    // 如果链表为空，直接返回
    if (head == NULL) {
        return;
    }

    // 如果要删除的节点是头结点，直接将头结点指向下一个节点
    if (head->addr == addr) {
        struct I2CSensorInfo* temp = head;
        head = head->next;
        free(temp);
        return;
    }

    // 如果要删除的节点不是头结点，遍历链表找到对应节点并删除
    struct I2CSensorInfo* currentNode = head;
    while (currentNode->next != NULL && currentNode->next->addr != addr) {
        currentNode = currentNode->next;
    }
    if (currentNode->next != NULL) {
        struct I2CSensorInfo* temp = currentNode->next;
        currentNode->next = temp->next;
        free(temp);
    }else{
      SENSOR_I2C_DBG("deleteNode null");
    }
}

void sensor_setup(){
  struct I2CSensorInfo *info = I2C_SENSOR_INFO_HEAD;
  while(info){
     SENSOR_I2C_DBG(info->port);
     SENSOR_I2C_DBG(info->addr);
    if(info && (info->attr == NULL) && info->base && info->base->setup) info->attr = info->base->setup(info->port, info->addr);
    info = info->next;
    delay(10);
  }
  SENSOR_I2C_DBG("sensor_setup ok");
}

void sensor_call(){
  struct I2CSensorInfo *info = I2C_SENSOR_INFO_HEAD;
  while(info){
    if(info && info->base && info->attr && info->base->call) info->base->call(info->port, info->attr->attr);
    info = info->next;
     delay(10);
  }
}
