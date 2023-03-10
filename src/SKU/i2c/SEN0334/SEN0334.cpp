/*!
 * @file SEN0334.cpp
 * @brief SEN0334 驱动定义
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-08-02
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 */
#include "SEN0334.h"

static DFRobot_SHT3x *getDFRobot_SHT3x(uint8_t ifn){
    DFRobot_SHT3x *sht3x = NULL;
    switch(ifn){
        case 1:
              sht3x = &SHT3x_1;
              break;
        case 2:
              sht3x = &SHT3x_2;
              break;
        default:
              return NULL;
    }
    return sht3x;
}

struct sensorAttr* SEN0334_SETUP(uint8_t ifn, uint8_t address){
    DFRobot_SHT3x *sht3x = getDFRobot_SHT3x(ifn);
    struct sensorAttr *attr = NULL;
    struct keyValue* head = NULL;
    struct keyValue* tail = NULL;

    if(sht3x == NULL) return NULL;
    
    if(sht3x->begin() != 0) return NULL; 

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

void SEN0334_FUN(uint8_t ifn, struct keyValue* head){
    DFRobot_SHT3x *sht3x = getDFRobot_SHT3x(ifn);
    uint8_t index = 0;
    if(sht3x == NULL) return;
    if(head == NULL) return;
   
    float temp = sht3x->getTemperatureC();
    float humd = sht3x->getHumidityRH();
    
    struct keyValue* p = head;

    while(p){
        switch(index){
            case 0:
            {
                p->key   = "Temp_Air";
                p->value = String(temp);
                p->unit  = "C";
            }
            break;
            case 1:
            {
                p->key   = "Humi_Air";
                p->value = String(humd);
                p->unit  = "%RH";

            }
                 break;
            default:
                 return;
        }
        index += 1;
        p = p->next;
    }
}

