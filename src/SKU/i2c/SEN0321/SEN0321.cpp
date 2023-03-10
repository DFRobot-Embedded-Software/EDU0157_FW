/*!
 * @file SEN0321.cpp
 * @brief SEN0321 驱动定义
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-10-11
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 */
#include "SEN0321.h"

static DFRobot_OzoneSensor *getDFRobot_SEN0321(uint8_t ifn){
    DFRobot_OzoneSensor *Ozone = NULL;
    switch(ifn){
        case 1:
              Ozone = &SEN0321_1;
              break;
        case 2:
              Ozone = &SEN0321_2;
              break;
        default:
              return NULL;
    }
    return Ozone;
}

struct sensorAttr* SEN0321_SETUP(uint8_t ifn, uint8_t address){
    DFRobot_OzoneSensor *Ozone = getDFRobot_SEN0321(ifn);
    struct sensorAttr *attr = NULL;
    struct keyValue* head = NULL;
    struct keyValue* tail = NULL;

    if(Ozone == NULL) return NULL;
    
    if(Ozone->begin(address) == 0) return NULL; 
    Ozone->setModes(MEASURE_MODE_PASSIVE);

    attr = new  sensorAttr;
    if(attr == NULL) return NULL;
     
    attr->count = 0;
    attr->attr  = NULL;
    uint8_t num = 1;
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

void SEN0321_FUN(uint8_t ifn, struct keyValue* head){
    DFRobot_OzoneSensor *Ozone = getDFRobot_SEN0321(ifn);
    uint8_t index = 0;
    if(Ozone == NULL) return;
    if(head == NULL) return;
    int16_t ozoneConcentration = Ozone->readOzoneData(20);
    
  
    struct keyValue* p = head;

    while(p){
        switch(index){
            case 0:
            {
                p->key   = "O3";
                p->value = String(ozoneConcentration);
                p->unit  = "ppb";
            }
            break;
            default:
                 return;
        }
        index += 1;
        p = p->next;
    }
}

