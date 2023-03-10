/*!
 * @file SEN0322.cpp
 * @brief SEN0322 驱动定义
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-08-02
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 */
#include "SEN0322.h"

static DFRobot_OxygenSensor *getDFRobot_Oxygen(uint8_t ifn){
    DFRobot_OxygenSensor *oxygen = NULL;
    switch(ifn){
        case 1:
              oxygen = &Oxygen_1;
              break;
        case 2:
              oxygen = &Oxygen_2;
              break;
        default:
              return NULL;
    }
    return oxygen;
}

struct sensorAttr* SEN0322_SETUP(uint8_t ifn, uint8_t address){
    DFRobot_OxygenSensor *oxygen = getDFRobot_Oxygen(ifn);
    struct sensorAttr *attr = NULL;
    struct keyValue* head = NULL;
    struct keyValue* tail = NULL;

    if(oxygen == NULL) return NULL;
    
    if(oxygen->begin(address) != true) return NULL; 
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

void SEN0322_FUN(uint8_t ifn, struct keyValue* head){
    DFRobot_OxygenSensor *oxygen = getDFRobot_Oxygen(ifn);
    uint8_t index = 0;
    if(oxygen == NULL) return;
    if(head == NULL) return;
   
    float O2 = oxygen->getOxygenData(10);
    
    struct keyValue* p = head;

    while(p){
        switch(index){
            case 0:
            {
                p->key   = "O2";
                p->value = String(O2);
                p->unit  = "%vol";
            }
            break;
            default:
                 return;
        }
        index += 1;
        p = p->next;
    }
}

