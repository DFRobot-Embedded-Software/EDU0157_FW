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
#include "MAX17043.h"
extern uint8_t max17043Com;
extern sGeneral_t *pGeneral;

static DFRobot_MAX17043 *getDFRobot_MAX17043(uint8_t ifn){
    DFRobot_MAX17043 *max17043 = NULL;
    switch(ifn){
        case 1:
              max17043 = &MAX17043_1;
              break;
        case 2:
              max17043 = &MAX17043_2;
              break;
        default:
              return NULL;
    }
    max17043Com = ifn;
    return max17043;
}


struct sensorAttr* MAX17043_SETUP(uint8_t ifn, uint8_t address){
    DFRobot_MAX17043 *max17043 = getDFRobot_MAX17043(ifn);
    struct sensorAttr *attr = NULL;
    struct keyValue* head = NULL;
    struct keyValue* tail = NULL;

    if(max17043 == NULL) return NULL;
    if(max17043->begin() != 0){
      pGeneral->sRGBColor = eRed;
      writeLog("MAX17043","sensor init error");
      return NULL;
    } 

    attr = new  sensorAttr;
    if(attr == NULL) return NULL;
     
    attr->count = 0;
    attr->attr  = NULL;
    uint8_t num = 2;
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

void MAX17043_FUN(uint8_t ifn, struct keyValue* head){
    DFRobot_MAX17043 *max17043 = getDFRobot_MAX17043(ifn);
    uint8_t index = 0;
    if(max17043 == NULL) return;
    if(head == NULL) return;

    float voltage = max17043->readVoltage();
    float battery  = max17043->readPercentage();

    struct keyValue* p = head;

    while(p){
        switch(index){
            case 0:
            {
                p->key   = "Battery";
                p->value = String(battery);
                p->unit  = "%";
            }
            break;
            case 1:
            {
                p->key   = "Voltage";
                p->value = String(voltage);
                p->unit  = "mV";
            }
            break;
            default:
                 return;
        }
        index += 1;
        p = p->next;
    }
}

