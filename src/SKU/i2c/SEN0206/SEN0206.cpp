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
#include "SEN0206.h"

static DFRobot_MLX90614_I2C *getDFRobot_SEN0206(uint8_t ifn){
    DFRobot_MLX90614_I2C *mlx90614 = NULL;
    switch(ifn){
        case 1:
              mlx90614 = &MLX90614_1;
              break;
        case 2:
              mlx90614 = &MLX90614_2;
              break;
        default:
              return NULL;
    }
    return mlx90614;
}

struct sensorAttr* SEN0206_SETUP(uint8_t ifn, uint8_t address){
    DFRobot_MLX90614_I2C *mlx90614 = getDFRobot_SEN0206(ifn);
    struct sensorAttr *attr = NULL;
    struct keyValue* head = NULL;
    struct keyValue* tail = NULL;

    if(mlx90614 == NULL) return NULL;
    
    if(mlx90614->begin(address) != 0) return NULL; 

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

void SEN0206_FUN(uint8_t ifn, struct keyValue* head){
    DFRobot_MLX90614_I2C *mlx90614 = getDFRobot_SEN0206(ifn);
    uint8_t index = 0;
    if(mlx90614 == NULL) return;
    if(head == NULL) return;

    float ambientTemp = mlx90614->getAmbientTempCelsius();
    float objectTemp  = mlx90614->getObjectTempCelsius();
  
    struct keyValue* p = head;

    while(p){
        switch(index){
            case 0:
            {
                p->key   = "Temp";
                p->value = String(ambientTemp);
                p->unit  = "C";
            }
            break;
            default:
                 return;
        }
        index += 1;
        p = p->next;
    }
}

