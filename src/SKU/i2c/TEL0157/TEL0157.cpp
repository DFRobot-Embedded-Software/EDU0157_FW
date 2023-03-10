/*!
 * @file TEL0157.cpp
 * @brief TEL0157 驱动定义
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-10-11
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 */
#include "TEL0157.h"

static DFRobot_GNSS_I2C *getDFRobot_GNSS_I2C(uint8_t ifn){
    DFRobot_GNSS_I2C *gnss = NULL;
    switch(ifn){
        case 1:
              gnss = &GNSS_1;
              break;
        case 2:
              gnss = &GNSS_2;
              break;
        default:
              return NULL;
    }
    return gnss;
}

struct sensorAttr* TEL0157_SETUP(uint8_t ifn, uint8_t address){
    DFRobot_GNSS_I2C *gnss = getDFRobot_GNSS_I2C(ifn);
    struct sensorAttr *attr = NULL;
    struct keyValue* head = NULL;
    struct keyValue* tail = NULL;

    if(gnss == NULL) return NULL;
    
    if(gnss->begin(address) != true) return NULL; 
    gnss->enablePower();      // Enable gnss power 
    /** Set GNSS to be used 
     *   eGPS              use gps
     *   eBeiDou           use beidou
     *   eGPS_BeiDou       use gps + beidou
     *   eGLONASS          use glonass
     *   eGPS_GLONASS      use gps + glonass
     *   eBeiDou_GLONASS   use beidou +glonass
     *   eGPS_BeiDou_GLONASS use gps + beidou + glonass
     */
    gnss->setGnss(eGPS_BeiDou_GLONASS);
    // gnss->setRgbOff();
    gnss->setRgbOn();
    // gnss->disablePower();      // Disable GNSS, the data will not be refreshed after disabling  

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

void TEL0157_FUN(uint8_t ifn, struct keyValue* head){
    DFRobot_GNSS_I2C *gnss = getDFRobot_GNSS_I2C(ifn);
    uint8_t index = 0;
    if(gnss == NULL) return;
    if(head == NULL) return;

    sLonLat_t lat = gnss->getLat();
    sLonLat_t lon = gnss->getLon();
    double high = gnss->getAlt();
  
    struct keyValue* p = head;

    while(p){
        switch(index){
            case 0:
            {
                p->key   = "Lat";
                p->value = String(lat.latitudeDegree, 6);
                p->unit  = "";
            }
            break;
            case 1:
            {
                p->key   = "Lon";
                p->value = String(lon.lonitudeDegree, 6);
                p->unit  = "";
            }
            break;
            case 2:
            {
                p->key   = "Altitude";
                p->value = String(high);
                p->unit  = "m";
            }
            break;
            default:
                 return;
        }
        index += 1;
        p = p->next;
    }
}

