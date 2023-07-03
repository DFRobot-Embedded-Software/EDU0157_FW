/*!
 * @file SEN0529.cpp
 * @brief SEN0529 驱动定义
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-10-11
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 */
#include "SEN0291.h"
extern uint8_t ina219Com;
static DFRobot_INA219_IIC *getDFRobot_INA219(uint8_t ifn){
    DFRobot_INA219_IIC *ina219 = NULL;
    switch(ifn){
        case 1:
              ina219 = &INA219_1;
              break;
        case 2:
              ina219 = &INA219_2;
              break;
        default:
              return NULL;
    }
    ina219Com = ifn;
    return ina219;
}

struct sensorAttr* SEN0291_SETUP(uint8_t ifn, uint8_t address){
    DFRobot_INA219_IIC *ina219 = getDFRobot_INA219(ifn);
    struct sensorAttr *attr = NULL;
    struct keyValue* head = NULL;
    struct keyValue* tail = NULL;

    if(ina219 == NULL) return NULL;
    
    if(ina219->begin(address) != true) return NULL; 
    ina219->linearCalibrate(1000, 1000);

    attr = new  sensorAttr;
    if(attr == NULL) return NULL;
     
    attr->count = 0;
    attr->attr  = NULL;
    uint8_t num = 3;
    while(num--){
        struct keyValue* p = new keyValue;
        if(p == NULL) return attr;
        p->next  = NULL;
        
        if(head == NULL){
            head = p;
            tail = p;
            attr->attr = p;
        }else{
            tail->next = p;
            tail       = p;
        }
        attr->count += 1;
    }
    return attr;
}

void SEN0291_FUN(uint8_t ifn, struct keyValue* head){
    DFRobot_INA219_IIC *ina219 = getDFRobot_INA219(ifn);
    uint8_t index = 0;
    if(ina219 == NULL) return;
    if(head == NULL) return;
    float vol = ina219->getBusVoltage_V();
    float cur = ina219->getCurrent_mA();
    float pow = ina219->getPower_mW();
  
    struct keyValue* p = head;

    while(p){
        switch(index){
            case 0:
            {
                p->key   = "Voltage";
                p->value = String(vol);
                p->unit  = "V";
            }
            break;
            case 1:
            {
                p->key   = "Current";
                p->value = String(cur);
                p->unit  = "mA";

            }
                 break;
            case 2:
            {
                p->key   = "Power";
                p->value = String(pow);
                p->unit  = "mW";

            }
                break;
            default:
                 return;
        }
        index += 1;
        p = p->next;
    }
}

