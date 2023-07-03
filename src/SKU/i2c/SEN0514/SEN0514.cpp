/*!
 * @file SEN0514.cpp
 * @brief SEN0514 驱动定义
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-08-02
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 */
#include "SEN0514.h"
extern uint8_t es160Com;
static DFRobot_ENS160_I2C *getDFRobot_ENS160(uint8_t ifn){
    DFRobot_ENS160_I2C *es160 = NULL;
    switch(ifn){
        case 1:
              es160 = &ENS160_1;
              break;
        case 2:
              es160 = &ENS160_2;
              break;
        default:
              return NULL;
    }
    es160Com = ifn;
    return es160;
}

struct sensorAttr* SEN0514_SETUP(uint8_t ifn, uint8_t address){
    DFRobot_ENS160_I2C *es160 = getDFRobot_ENS160(ifn);
    struct sensorAttr *attr = NULL;
    struct keyValue* head = NULL;
    struct keyValue* tail = NULL;

    if(es160 == NULL) return NULL;
    
    if(es160->begin(address) != 0) return NULL; 
    es160->setPWRMode(ENS160_STANDARD_MODE);
    es160->setTempAndHum(/*temperature=*/25.0, /*humidity=*/50.0);

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

void SEN0514_FUN(uint8_t ifn, struct keyValue* head){
    DFRobot_ENS160_I2C *es160 = getDFRobot_ENS160(ifn);
    uint8_t index = 0;
    if(es160 == NULL) return;
    if(head == NULL) return;

    es160->getENS160Status();
    uint8_t AQI = es160->getAQI();
    uint16_t TVOC = es160->getTVOC();
    uint16_t ECO2 = es160->getECO2();
   
    struct keyValue* p = head;

    while(p){
        switch(index){
            case 0:
            {
                p->key   = "AQI";
                p->value = String(AQI);
                p->unit  = "";
            }
            break;
            case 1:
            {
                p->key   = "TVOC";
                p->value = String(TVOC);
                p->unit  = "ppb";

            }
            break;
            case 2:
            {
                p->key   = "ECO2";
                p->value = String(ECO2);
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

