/*!
 * @file SEN0517.cpp
 * @brief SEN0334 驱动定义
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-10-11
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 */
#include "SEN0517.h"

static DFRobot_ICP10111 *getDFRobot_ICP10111(uint8_t ifn){
    DFRobot_ICP10111 *icp = NULL;
    switch(ifn){
        case 1:
              icp = &ICP10111_1;
              break;
        case 2:
              icp = &ICP10111_2;
              break;
        default:
              return NULL;
    }
    return icp;
}

struct sensorAttr* SEN0517_SETUP(uint8_t ifn, uint8_t address){
    DFRobot_ICP10111 *icp = getDFRobot_ICP10111(ifn);
    struct sensorAttr *attr = NULL;
    struct keyValue* head = NULL;
    struct keyValue* tail = NULL;

    if(icp == NULL) return NULL;
    
    if(icp->begin(address) != 0) return NULL; 
    icp->setWorkPattern(icp->eNormal);

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

void SEN0517_FUN(uint8_t ifn, struct keyValue* head){
    DFRobot_ICP10111 *icp = getDFRobot_ICP10111(ifn);
    uint8_t index = 0;
    if(icp == NULL) return;
    if(head == NULL) return;
    float altitude = icp->getElevation();
    float pressure = icp->getAirPressure()/100.0;
    float air      = icp->getTemperature();

    struct keyValue* p = head;

    while(p){
        switch(index){
            case 0:
            {
                p->key   = "Altitude";
                p->value = String(altitude);
                p->unit  = "m";
            }
            break;
            case 1:
            {
                p->key   = "Pressure";
                p->value = String(pressure);
                p->unit  = "hPa";

            }
                 break;
            // case 2:
            // {
            //     p->key   = "Temp_Air";
            //     p->value = String(air);
            //     p->unit  = "C";

            // }
            //      break;
            default:
                 return;
        }
        index += 1;
        p = p->next;
    }
}

