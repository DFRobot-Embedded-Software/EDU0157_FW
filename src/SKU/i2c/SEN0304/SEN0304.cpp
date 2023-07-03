/*!
 * @file SEN0304.cpp
 * @brief SEN0304 驱动定义
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-10-11
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 */
#include "SEN0304.h"
extern uint8_t urm09Com;
static DFRobot_URM09 *getDFRobot_URM09(uint8_t ifn){
    DFRobot_URM09 *urm09 = NULL;
    switch(ifn){
        case 1:
              urm09 = &URM09_1;
              break;
        case 2:
              urm09 = &URM09_2;
              break;
        default:
              return NULL;
    }
    urm09Com = ifn;
    return urm09;
}

struct sensorAttr* SEN0304_SETUP(uint8_t ifn, uint8_t address){
    DFRobot_URM09 *urm09 = getDFRobot_URM09(ifn);
    struct sensorAttr *attr = NULL;
    struct keyValue* head = NULL;
    struct keyValue* tail = NULL;

    if(urm09 == NULL) return NULL;
    
    if(urm09->begin(address) == 0) return NULL; 
    urm09->setModeRange(MEASURE_MODE_AUTOMATIC2 ,MEASURE_RANG_500);

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

void SEN0304_FUN(uint8_t ifn, struct keyValue* head){
    DFRobot_URM09 *urm09 = getDFRobot_URM09(ifn);
    uint8_t index = 0;
    if(urm09 == NULL) return;
    if(head == NULL) return;

    int16_t dist = urm09->getDistance();
  
    struct keyValue* p = head;

    while(p){
        switch(index){
            case 0:
            {
                p->key   = "Distance";
                p->value = String(dist);
                p->unit  = "cm";
            }
            break;
            default:
                 return;
        }
        index += 1;
        p = p->next;
    }
}

