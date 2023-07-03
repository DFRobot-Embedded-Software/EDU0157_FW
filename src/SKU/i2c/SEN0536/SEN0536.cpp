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
#include "SEN0536.h"
extern uint8_t scd4xCom;
static DFRobot_SCD4X *getDFRobot_SCD4X(uint8_t ifn){
    DFRobot_SCD4X *scd4x = NULL;
    switch(ifn){
        case 1:
              scd4x = &SCD4X_1;
              break;
        case 2:
              scd4x = &SCD4X_2;
              break;
        default:
              return NULL;
    }
    scd4xCom = ifn;
    return scd4x;
}

struct sensorAttr* SEN0536_SETUP(uint8_t ifn, uint8_t address){
    DFRobot_SCD4X *scd4x = getDFRobot_SCD4X(ifn);
    struct sensorAttr *attr = NULL;
    struct keyValue* head = NULL;
    struct keyValue* tail = NULL;

    if(scd4x == NULL) return NULL;
    
    if(scd4x->begin(address) != true) return NULL; 
    scd4x->enablePeriodMeasure(SCD4X_STOP_PERIODIC_MEASURE);
    scd4x->setTempComp(4.0);
    scd4x->enablePeriodMeasure(SCD4X_START_PERIODIC_MEASURE);

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
DFRobot_SCD4X::sSensorMeasurement_t SCD4X_data;
void SEN0536_FUN(uint8_t ifn, struct keyValue* head){
    DFRobot_SCD4X *scd4x = getDFRobot_SCD4X(ifn);
    uint8_t index = 0;
    if(scd4x == NULL) return;
    if(head == NULL) return;

    if(scd4x->getDataReadyStatus()){
        scd4x->readMeasurement(&SCD4X_data);
    }
  
    struct keyValue* p = head;

    while(p){
        switch(index){
            case 0:
            {
                p->key   = "CO2";
                p->value = String(SCD4X_data.CO2ppm);
                p->unit  = "ppm";
            }
            break;
            default:
                 return;
        }
        index += 1;
        p = p->next;
    }
}

