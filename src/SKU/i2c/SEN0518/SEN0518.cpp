/*!
 * @file SEN0518.cpp
 * @brief SEN0518 驱动定义
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-10-11
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 */
#include "SEN0518.h"
extern uint8_t MAX30102Com;
static DFRobot_BloodOxygen_S_I2C *getDFRobot_SEN0518(uint8_t ifn){
    DFRobot_BloodOxygen_S_I2C *MAX30102 = NULL;
    switch(ifn){
        case 1:
              MAX30102 = &SEN0518_1;
              break;
        case 2:
              MAX30102 = &SEN0518_2;
              break;
        default:
              return NULL;
    }
    MAX30102Com = ifn;
    return MAX30102;
}

struct sensorAttr* SEN0518_SETUP(uint8_t ifn, uint8_t address){
    DFRobot_BloodOxygen_S_I2C *MAX30102 = getDFRobot_SEN0518(ifn);
    struct sensorAttr *attr = NULL;
    struct keyValue* head = NULL;
    struct keyValue* tail = NULL;

    if(MAX30102 == NULL) return NULL;
   
    
    if(MAX30102->begin(address) == 0) return NULL; 
    MAX30102->sensorStartCollect();

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

void SEN0518_FUN(uint8_t ifn, struct keyValue* head){
    DFRobot_BloodOxygen_S_I2C *MAX30102 = getDFRobot_SEN0518(ifn);
    uint8_t index = 0;
    if(MAX30102 == NULL) return;
    if(head == NULL) return;

    MAX30102->getHeartbeatSPO2();
  
    struct keyValue* p = head;
    while(p){
        switch(index){
            case 0:
            {
                p->key   = "SPO2";
                p->value = String(MAX30102->_sHeartbeatSPO2.SPO2);
                p->unit  = "%";
            }
            break;
            case 1:
            {
                p->key   = "Heart";
                p->value = String(MAX30102->_sHeartbeatSPO2.Heartbeat);
                p->unit  = "T/min";

            }
                 break;
            default:
                 return;
        }
        index += 1;
        p = p->next;
    }
}

