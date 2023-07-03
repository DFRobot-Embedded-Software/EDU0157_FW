/*!
 * @file DFR0216.cpp
 * @brief DFR0216 驱动定义
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-08-02
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 */
#include "DFR0216.h"
#include "../../software_iic.h"
#define REG_WIND_DIR     0x00
#define REG_WIND_SPEED   0x02
#define REG_PRESSURE     0x06
#define REG_TEMPERATURE  0x0A
#define REG_HUMIDITY     0x0E
static uint8_t _addr = 0x55;

static SoftwareTwoWire *getDFRobot_UNO(uint8_t ifn){
    SoftwareTwoWire *uno = NULL;
    switch(ifn){
        case 1:
              uno = &SOF_WIRE1;
              break;
        case 2:
              uno = &SOF_WIRE2;
              break;
        default:
              return NULL;
    }
    return uno;
}

static int getWindDirection(uint8_t ifn){
    //int wid = 0;
    uint16_t wid;
    uint8_t *pBuf = (uint8_t *)&wid;
    SoftwareTwoWire *uno = getDFRobot_UNO(ifn);
    uno->beginTransmission(_addr);
    uno->write(REG_WIND_DIR);
    uno->write(2);
    if(uno->endTransmission() != 0) return int(wid);
    uno->requestFrom(_addr, 2);
    for(int i = 0; i < 2; i++){
        pBuf[i] = uno->read();
    }
    return int(wid);
}

static float getWindSpeed(uint8_t ifn){
    float wid = 0;
    uint8_t *pBuf = (uint8_t *)&wid;
    SoftwareTwoWire *uno = getDFRobot_UNO(ifn);
    uno->beginTransmission(_addr);
    uno->write(REG_WIND_SPEED);
    uno->write(sizeof(float));
    if(uno->endTransmission() != 0) return int(wid);
    uno->requestFrom(_addr, sizeof(float));
    for(int i = 0; i < sizeof(float); i++){
        pBuf[i] = uno->read();
    }
    return wid;
}

static float getAirPressure(uint8_t ifn){
    float wid = 0;
    uint8_t *pBuf = (uint8_t *)&wid;
    SoftwareTwoWire *uno = getDFRobot_UNO(ifn);
    uno->beginTransmission(_addr);
    uno->write(REG_PRESSURE);
    uno->write(sizeof(float));
    if(uno->endTransmission() != 0) return int(wid);
    uno->requestFrom(_addr, sizeof(float));
    for(int i = 0; i < sizeof(float); i++){
        pBuf[i] = uno->read();
    }
    return wid;
}

static float getTemperatureC(uint8_t ifn){
    float wid = 0;
    uint8_t *pBuf = (uint8_t *)&wid;
    SoftwareTwoWire *uno = getDFRobot_UNO(ifn);

    uno->beginTransmission(_addr);
    uno->write(REG_TEMPERATURE);
    uno->write(sizeof(float));
    if(uno->endTransmission() != 0) return int(wid);
    uno->requestFrom(_addr, sizeof(float));
    for(int i = 0; i < sizeof(float); i++){
        pBuf[i] = uno->read();
    }
    return wid;
}

static float getHumidityRH(uint8_t ifn){
    float wid = 0;
    uint8_t *pBuf = (uint8_t *)&wid;
    SoftwareTwoWire *uno = getDFRobot_UNO(ifn);
    uno->beginTransmission(_addr);
    uno->write(REG_HUMIDITY);
    uno->write(sizeof(float));
    if(uno->endTransmission() != 0) return int(wid);
    uno->requestFrom(_addr, sizeof(float));
    for(int i = 0; i < sizeof(float); i++){
        pBuf[i] = uno->read();
    }
    return wid;
}

struct sensorAttr* DFR0216_SETUP(uint8_t ifn, uint8_t address){
    SoftwareTwoWire *uno = getDFRobot_UNO(ifn);
    struct sensorAttr *attr = NULL;
    struct keyValue* head = NULL;
    struct keyValue* tail = NULL;
    _addr = address;

    uno->beginTransmission(_addr);
    if(uno->endTransmission() != 0) return NULL;
    
    attr = new  sensorAttr;
    if(attr == NULL) return NULL;
     
    attr->count = 0;
    attr->attr  = NULL;
    uint8_t num = 5;
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

void DFR0216_FUN(uint8_t ifn, struct keyValue* head){
    SoftwareTwoWire *uno = getDFRobot_UNO(ifn);
    uint8_t index = 0;
    if(uno == NULL) return;
    if(head == NULL) return;

    int windDir       = getWindDirection(ifn);   ///< 风向, int占用2个字节
    float windSpeed   = getWindSpeed(ifn);     ///< 风速, float占用4个字节
    float pressure    = getAirPressure(ifn);      ///< 压强, float占用4个字节
    float temperature = getTemperatureC(ifn);   ///< 温度, float占用4个字节
    float humidity    = getHumidityRH(ifn);      ///< 湿度, float占用4个字节 
   
    struct keyValue* p = head;

    while(p){
        switch(index){
            case 0:
            {
                p->key   = "Direc_W";
                p->value = String(windDir);
                p->unit  = "";
            }
            break;
            case 1:
            {
                p->key   = "Speed_W";
                p->value = String(windSpeed);
                p->unit  = "m/s";

            }
            break;
            case 2:
            {
                p->key   = "Pressure";
                p->value = String(pressure);
                p->unit  = "hPa";

            }
            break;
            case 3:
            {
                p->key   = "Temperature";
                p->value = String(temperature);
                p->unit  = "C";

            }
            break;
            case 4:
            {
                p->key   = "Humidity";
                p->value = String(humidity);
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

