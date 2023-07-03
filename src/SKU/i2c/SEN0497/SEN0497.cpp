/*!
 * @file SEN0497.h
 * @brief SEN0497 驱动声明
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-08-02
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 */
#include "SEN0497.h"
extern uint8_t dht20Com;
static DFRobot_DHT20 *getDFRobot_DHT20(uint8_t ifn){
    DFRobot_DHT20 *dht20 = NULL;
    switch(ifn){
        case 1:
              dht20 = &DHT20_1;
              break;
        case 2:
              dht20 = &DHT20_2;
              break;
        default:
              return NULL;
    }
    dht20Com = ifn;
    return dht20;
}

struct sensorAttr* SEN0497_SETUP(uint8_t ifn, uint8_t address){
    DFRobot_DHT20 *dht20 = getDFRobot_DHT20(ifn);
    struct sensorAttr *attr = NULL;
    struct keyValue* head = NULL;
    struct keyValue* tail = NULL;

    if(dht20 == NULL) return NULL;
    
    if(dht20->begin(address) != 0) return NULL; 

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

void SEN0497_FUN(uint8_t ifn, struct keyValue* head){
    DFRobot_DHT20 *dht20 = getDFRobot_DHT20(ifn);
    uint8_t index = 0;
    if(dht20 == NULL) return;
    if(!dht20->isOnline()) return;
    if(head == NULL) return;
   
    float temp = dht20->getTemperature();
    float humd = dht20->getHumidity();
    //Serial.println();
    //Serial.print("temp = ");Serial.println(temp);
    //Serial.print("humd = ");Serial.println(humd);
    //Serial.println();
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

