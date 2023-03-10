/*!
 * @file SEN0364.cpp
 * @brief SEN0364 驱动定义
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-08-02
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 */
#include "SEN0364.h"

static DFRobot_AS7341 *getDFRobot_SEN0364(uint8_t ifn){
    DFRobot_AS7341 *as7341 = NULL;
    switch(ifn){
        case 1:
              as7341 = &as7341_1;
              break;
        case 2:
              as7341 = &as7341_2;
              break;
        default:
              return NULL;
    }
    return as7341;
}

struct sensorAttr* SEN0364_SETUP(uint8_t ifn, uint8_t address){
    //Serial.print("SEN0364_SETUP begin: ");Serial.println(address);
    DFRobot_AS7341 *as7341 = getDFRobot_SEN0364(ifn);
    struct sensorAttr *attr = NULL;
    struct keyValue* head = NULL;
    struct keyValue* tail = NULL;

    if(as7341 == NULL) return NULL;
    
    if(as7341->begin() != 0) return NULL; 

    attr = new  sensorAttr;
    if(attr == NULL) return NULL;
     
    attr->count = 0;
    attr->attr  = NULL;
    uint8_t num = 8;
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
    //Serial.println("SEN0364_SETUP begin end");
    return attr;
}

void SEN0364_FUN(uint8_t ifn, struct keyValue* head){
    DFRobot_AS7341 *as7341 = getDFRobot_SEN0364(ifn);
    //Serial.println("SEN0364_FUN begin: ");
    uint8_t index = 0;
    if(as7341 == NULL) return;
    if(head == NULL) return;

    as7341->startMeasure(as7341->eF1F4ClearNIR);
    DFRobot_AS7341::sModeOneData_t data1 = as7341->readSpectralDataOne();
    as7341->startMeasure(as7341->eF5F8ClearNIR);
    DFRobot_AS7341::sModeTwoData_t data2 = as7341->readSpectralDataTwo();
    
    struct keyValue* p = head;

    while(p){
        switch(index){
            case 0:
            {
                p->key   = "405-425nm";
                p->value = String(data1.ADF1);
                p->unit  = "";
            }
            break;
            case 1:
            {
                p->key   = "435-455nm";
                p->value = String(data1.ADF2);
                p->unit  = "";
            }
            break;
            case 2:
            {
                p->key   = "470-490nm";
                p->value = String(data1.ADF3);
                p->unit  = "";
            }
            break;
            case 3:
            {
                p->key   = "505-525nm";
                p->value = String(data1.ADF4);
                p->unit  = "";
            }
            break;
            case 4:
            {
                p->key   = "545-565nm";
                p->value = String(data2.ADF5);
                p->unit  = "";
            }
            break;
            case 5:
            {
                p->key   = "580-600nm";
                p->value = String(data2.ADF6);
                p->unit  = "";
            }
            break;
            case 6:
            {
                p->key   = "620-640nm";
                p->value = String(data2.ADF7);
                p->unit  = "";
            }
            break;
            case 7:
            {
                p->key   = "670-690nm";
                p->value = String(data2.ADF8);
                p->unit  = "";
            }
            break;
            default:
                 return;
        }
        index += 1;
        p = p->next;
    }
    //Serial.println("SEN0364_FUN end ");
}