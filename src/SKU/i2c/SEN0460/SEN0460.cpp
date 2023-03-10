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
#include "SEN0460.h"

static DFRobot_AirQualitySensor *getDFRobot_AirQ(uint8_t ifn){
    DFRobot_AirQualitySensor *airq = NULL;
    switch(ifn){
        case 1:
              airq = &AIRQ_1;
              break;
        case 2:
              airq = &AIRQ_2;
              break;
        default:
              return NULL;
    }
    return airq;
}

struct sensorAttr* SEN0460_SETUP(uint8_t ifn, uint8_t address){
    DFRobot_AirQualitySensor *airq = getDFRobot_AirQ(ifn);
    struct sensorAttr *attr = NULL;
    struct keyValue* head = NULL;
    struct keyValue* tail = NULL;

    if(airq == NULL) return NULL;
    
    if(airq->begin(address) == 0) return NULL; 

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

void SEN0460_FUN(uint8_t ifn, struct keyValue* head){
    DFRobot_AirQualitySensor *airq = getDFRobot_AirQ(ifn);
    uint8_t index = 0;
    if(airq == NULL) return;
    if(head == NULL) return;

    uint16_t PM1_0 = airq->gainParticleConcentration_ugm3(PARTICLE_PM1_0_STANDARD);
    uint16_t PM2_5 = airq->gainParticleConcentration_ugm3(PARTICLE_PM2_5_STANDARD);
    uint16_t PM10 = airq->gainParticleConcentration_ugm3(PARTICLE_PM10_STANDARD);
   
    struct keyValue* p = head;

    while(p){
        switch(index){
            case 0:
            {
                p->key   = "PM1.0";
                p->value = String(PM1_0);
                p->unit  = "ug/m3";
            }
            break;
            case 1:
            {
                p->key   = "PM2.5";
                p->value = String(PM2_5);
                p->unit  = "ug/m3";

            }
                 break;
            case 2:
            {
                p->key   = "PM10";
                p->value = String(PM10);
                p->unit  = "ug/m3";

            }
                 break;
            default:
                 return;
        }
        index += 1;
        p = p->next;
    }
}

