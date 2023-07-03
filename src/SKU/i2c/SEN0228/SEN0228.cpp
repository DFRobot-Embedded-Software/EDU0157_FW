/*!
 * @file SEN0228.cpp
 * @brief SEN0228 驱动定义
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-08-02
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 */
#include "SEN0228.h"
extern uint8_t veml7700Com;
static DFRobot_VEML7700 *getDFRobot_VEML7700(uint8_t ifn){
    DFRobot_VEML7700 *veml = NULL;
    switch(ifn){
        case 1:
              veml = &VEML7700_1;
              break;
        case 2:
              veml = &VEML7700_2;
              break;
        default:
              return NULL;
    }
    veml7700Com = ifn;
    return veml;
}

struct sensorAttr* SEN0228_SETUP(uint8_t ifn, uint8_t address){
    DFRobot_VEML7700 *veml = getDFRobot_VEML7700(ifn);
    struct sensorAttr *attr = NULL;
    struct keyValue* head = NULL;
    struct keyValue* tail = NULL;

    if(veml == NULL) return NULL;
    veml->begin();

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


float grn1(float u,float g,float * r)
{ int i,m;
double s,w,v,t;
s=65536.0; w=2053.0; v=13849.0;
t=0.0;
for (i=1; i<=12; i++)
{ *r=(*r)*w+v; m=(int)(*r/s);
*r=*r-m*s; t=t+(*r)/s;
}
t=u+g*(t-6.0);
return(t);
}

void SEN0228_FUN(uint8_t ifn, struct keyValue* head){
    DFRobot_VEML7700 *veml = getDFRobot_VEML7700(ifn);
    uint8_t index = 0;
    if(veml == NULL) return;
    if(head == NULL) return;
    
    float lux;
    veml->getALSLux(lux);
    struct keyValue* p = head;

    //float k = random(5);
    //float b = random(9);
    //int c = random(10);
    //b = b/10.0;

    //if(c % 2 == 0) k = -k;
    //if(k + b > 5) k -= b;
    //else k += b;

    while(p){
        switch(index){
            case 0:
            {
                p->key   = "Light";
                p->value = String(lux);
                p->unit  = "lx";
            }
            break;
            default:
                 return;
        }
        index += 1;
        p = p->next;
    }
}

/*
float grn1(float u,float g,float * r)
{ int i,m;
double s,w,v,t;
s=65536.0; w=2053.0; v=13849.0;
t=0.0;
for (i=1; i<=12; i++)
{ *r=(*r)*w+v; m=(int)(*r/s);
*r=*r-m*s; t=t+(*r)/s;
}
t=u+g*(t-6.0);
return(t);
}

*/