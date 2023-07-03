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
#include "SEN0529.h"
extern uint8_t bmm150Com;
static DFRobot_BMM150_I2C *getDFRobot_BMM150(uint8_t ifn){
    DFRobot_BMM150_I2C *bmm150 = NULL;
    switch(ifn){
        case 1:
              bmm150 = &BMM150_1;
              break;
        case 2:
              bmm150 = &BMM150_2;
              break;
        default:
              return NULL;
    }
    bmm150Com = ifn;
    return bmm150;
}

struct sensorAttr* SEN0529_SETUP(uint8_t ifn, uint8_t address){
    DFRobot_BMM150_I2C *bmm150 = getDFRobot_BMM150(ifn);
    struct sensorAttr *attr = NULL;
    struct keyValue* head = NULL;
    struct keyValue* tail = NULL;

    if(bmm150 == NULL) return NULL;
    
    if(bmm150->begin(address) != 0) return NULL; 
    bmm150->setOperationMode(BMM150_POWERMODE_NORMAL);
    bmm150->setPresetMode(BMM150_PRESETMODE_HIGHACCURACY);
    bmm150->setMeasurementXYZ();

    attr = new  sensorAttr;
    if(attr == NULL) return NULL;
     
    attr->count = 0;
    attr->attr  = NULL;
    uint8_t num = 4;
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

void SEN0529_FUN(uint8_t ifn, struct keyValue* head){
    DFRobot_BMM150_I2C *bmm150 = getDFRobot_BMM150(ifn);
    uint8_t index = 0;
    if(bmm150 == NULL) return;
    if(head == NULL) return;
    sBmm150MagData_t magData = bmm150->getGeomagneticData();
    float compassDegree = bmm150->getCompassDegree();
  
    struct keyValue* p = head;

    while(p){
        switch(index){
            case 0:
            {
                p->key   = "Angle_N";
                p->value = String(compassDegree);
                p->unit  = "";
            }
            break;
            case 1:
            {
                p->key   = "Mag_X";
                p->value = String(magData.x);
                p->unit  = "uT";

            }
                 break;
            case 2:
            {
                p->key   = "Mag_Y";
                p->value = String(magData.y);
                p->unit  = "uT";

            }
                break;
            case 3:
            {
                p->key   = "Mag_Z";
                p->value = String(magData.z);
                p->unit  = "uT";

            }
                 break;
            default:
                 return;
        }
        index += 1;
        p = p->next;
    }
}

