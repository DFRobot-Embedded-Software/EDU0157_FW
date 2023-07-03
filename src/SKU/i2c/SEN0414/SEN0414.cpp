#include "SEN0414.h"
extern uint8_t shtc3Com;
static DFRobot_SHTC3 *getDFRobot_SHTC3(uint8_t ifn){
    DFRobot_SHTC3 *shtc3 = NULL;
    switch(ifn){
        case 1:
              shtc3 = &SHTC3_1;
              break;
        case 2:
              shtc3 = &SHTC3_2;
              break;
        default:
              return NULL;
    }
    shtc3Com = ifn;
    return shtc3;
}

struct sensorAttr* SEN0414_SETUP(uint8_t ifn, uint8_t address){
    DFRobot_SHTC3 *shtc3 = getDFRobot_SHTC3(ifn);
    uint8_t i = 0;
    struct sensorAttr *attr = NULL;
    struct keyValue* head = NULL;
    struct keyValue* tail = NULL;
    if(shtc3 == NULL) return NULL;
    shtc3->begin();
    shtc3->wakeup();
    while(shtc3->getDeviceID() == 0){
        i++;
        if(i == 5)
            return NULL;
        delay(1000);
    }
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

void SEN0414_FUN(uint8_t ifn, struct keyValue* head){
    DFRobot_SHTC3 *shtc3 = getDFRobot_SHTC3(ifn);
    uint8_t index = 0;
    if(shtc3 == NULL) return;
    if(head == NULL) return;
    shtc3->wakeup();
    float temp = shtc3->getTemperature(PRECISION_HIGH_CLKSTRETCH_ON);
    float humd = shtc3->getHumidity(PRECISION_HIGH_CLKSTRETCH_OFF);
    shtc3->sleep();
    struct keyValue* p = head;

    while(p){
        switch(index){
            case 0:
            {
                p->key   = "Temp";
                p->value = String(temp);
                p->unit  = "C";
            }
            break;
            case 1:
            {
                p->key   = "Humi";
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