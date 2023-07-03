#include "power.h"

#define TMR1302S_EXTEND_PIN   7
//#define SCO1_INPUT_PIN        11
#define TMR1302S_U9_PIN       12
#define TMR1302S_U6_PIN       13
#define TMR1302S_U11_PIN      14
#define TMR1302S_U13_PIN      15
#define HM3445_PIN            23
//#define MAX17043_POWER_PIN    24
//#define SC01_POWER_PIN        22

#define PIN_23    23
#define EXTEND_POWER          24
#define WIRE_POWER_PIN        9
#define DTU_POWER             26
#define GNSS_POWER            27

#define RTCINT                8

extern sGeneral_t *pGeneral;

void initPin(void)
{
  pinMode(TMR1302S_EXTEND_PIN,INPUT);
  if(pGeneral->sLowPower == OFF){
    pinMode(RTCINT,INPUT_PULLUP);
  }
  pinMode(TMR1302S_U9_PIN,INPUT);
  pinMode(TMR1302S_U6_PIN,INPUT);
  pinMode(TMR1302S_U11_PIN,INPUT);
  pinMode(TMR1302S_U13_PIN,INPUT);
  pinMode(PIN_23,OUTPUT);
  pinMode(WIRE_POWER_PIN,OUTPUT);
  pinMode(EXTEND_POWER,OUTPUT);
  pinMode(DTU_POWER,OUTPUT);
  pinMode(GNSS_POWER,OUTPUT);
}


void powerON(void)
{ 
  digitalWrite(PIN_23, HIGH);
  digitalWrite(WIRE_POWER_PIN,HIGH);//
  digitalWrite(EXTEND_POWER,LOW);//LOW
  digitalWrite(DTU_POWER,HIGH);//HIGH
  digitalWrite(GNSS_POWER,HIGH);//HIGH
}

void powerOFF(void){
  digitalWrite(PIN_23, LOW);
  digitalWrite(WIRE_POWER_PIN,LOW);//
  digitalWrite(EXTEND_POWER,HIGH);//HIGH
  digitalWrite(DTU_POWER,LOW);//LOW
  digitalWrite(GNSS_POWER,LOW);//LOW
}

void windSpeed(void){
  (pGeneral->interrcount)++;
}

void readData(void){
  pGeneral->sReadData = ON;
}

void gpio_callbackF(uint gpio, uint32_t events) {
  switch(gpio){
    case 7:
      if (events  == GPIO_IRQ_EDGE_FALL) {
        (pGeneral->interrcount)++;
      }
    break;
    case 8:
    if (events  == GPIO_IRQ_EDGE_FALL) {
        pGeneral->sReadData = ON;
      }
    break;
    case 12:
    case 13:
    case 14:
    case 15:
    if ((events  ==  GPIO_IRQ_EDGE_RISE) || (events  ==  GPIO_IRQ_EDGE_FALL)) {
      getCompassData();
    }
    break;
    default:
      //getCompassData();
    break;
  }
}

void interruptConfig(void)
{
  if(pGeneral->sLowPower == OFF){
    attachInterrupt(TMR1302S_EXTEND_PIN, windSpeed, FALLING);
    attachInterrupt(RTCINT, readData, FALLING);
    attachInterrupt(digitalPinToInterrupt(TMR1302S_U13_PIN), D, CHANGE);//D
    attachInterrupt(digitalPinToInterrupt(TMR1302S_U6_PIN), C, CHANGE);//C
    attachInterrupt(digitalPinToInterrupt(TMR1302S_U9_PIN), B, CHANGE);//B
    attachInterrupt(digitalPinToInterrupt(TMR1302S_U11_PIN), A, CHANGE);//A
  }else{
    gpio_set_irq_enabled_with_callback(7, GPIO_IRQ_EDGE_FALL, true, &gpio_callbackF);
    gpio_set_irq_enabled_with_callback(8, GPIO_IRQ_EDGE_FALL, true, &gpio_callbackF);
    gpio_set_irq_enabled_with_callback(12, GPIO_IRQ_EDGE_RISE|GPIO_IRQ_EDGE_FALL, true, &gpio_callbackF);
    gpio_set_irq_enabled_with_callback(13, GPIO_IRQ_EDGE_RISE|GPIO_IRQ_EDGE_FALL, true, &gpio_callbackF);
    gpio_set_irq_enabled_with_callback(14, GPIO_IRQ_EDGE_RISE|GPIO_IRQ_EDGE_FALL, true, &gpio_callbackF);
    gpio_set_irq_enabled_with_callback(15, GPIO_IRQ_EDGE_RISE|GPIO_IRQ_EDGE_FALL, true, &gpio_callbackF);
  }
}

String widDir = "N";
uint8_t dir = 0;
uint16_t angle = 0;
void getCompassData1(void)
{
    dir = 0;
    dir |= digitalRead(TMR1302S_U13_PIN)<<3 | digitalRead(TMR1302S_U6_PIN)<<2 | digitalRead(TMR1302S_U9_PIN)<<1 | digitalRead(TMR1302S_U11_PIN)<<0;
    if((dir == 0x0E && angle == 45) || (dir == 0x0E && angle == 315)){
        angle=0;  
        widDir = "N";
    }
    if((dir == 0x03 && angle == 90) || (dir == 0x03 && angle == 0)){
        angle=45;  
        widDir = "NE";
    } 
    if((dir == 0x0D && angle == 135) || (dir == 0x0D && angle == 45)){
        angle=90;   
        widDir = "E";
    }
    if((dir == 0x06 && angle == 180) || (dir == 0x06 && angle == 90)){
        angle=135;  
        widDir = "SE";
     }
    if((dir == 0x0B && angle == 225) || (dir == 0x0B && angle == 135)){
        angle=180; 
        widDir = "S";
    }
    if((dir == 0x0C && angle == 270) || (dir == 0x0C && angle == 180)){
        angle=225;  
        widDir = "SW";
    }
    if((dir == 0x07 && angle == 315) || (dir == 0x07 && angle == 225)){
        angle=270;   
        widDir = "W";
    }
    if((dir == 0x09 && angle == 0) || (dir == 0x09 && angle == 270)){
        angle=315;
        widDir = "NW";
    }
}

/**
 * @brief Get the Compass Data object
 *           E
 * 
 *           U9
 * 
 *  N   U11       U6    S
 * 
 *           U13
 * 
 *           W
 * 
 * 
 * 
 * @n       U9   U6  U11  U13
 * @n GPIO  12   13   14   15
 * @n        1    0    1    0    N     0x0A     0x0b     0x0F
 * @n        0    1    0    1    NE    0x0D     0x05     0x04
 * @n        1    1    1    0    E     0x0C     0x0E     0x0A
 * @n        0    0    1    1    SE    0x0B     0x03     0x01
 * @n        1    1    0    1    S     0x09     0x0D     0x0E
 * @n        1    0    1    0    SW    0x08     0x0A     0x0E
 * @n        0    1    1    1    W     0x06     0x07     0x0F
 * @n        1    1    0    0    NW    0x0E     0x0C     0x08
 * @n        
 * @return String 
 */
static uint8_t lastdir = 0;
static uint8_t flage = 1;
void getCompassData(void)
{
  uint32_t pinAllValue = gpio_get_all();
  dir = (uint8_t)(pinAllValue >> 12 & 0x0F);
  switch(dir){
    case 0x0B:
      switch(lastdir){
        case 0x03:
        break;
        case 0x0D:
        break;
        case 0x0A:
        break;
         case 0x0E:
        break;
        default:
        widDir = "N";
        flage = 1;
      }
    break;
    case 0x05:
      switch(lastdir){
        case 0x03:
        break;
        case 0x07:
        break;
        case 0x0D:
        break;
        default:
        widDir = "NE";
        flage = 1;
      }
    break;
    case 0x0E:
      switch(lastdir){
        case 0x0A:
        break;
        case 0x07:
        break;
        case 0x0C:
        break;
        case 0x0D:
        break;
        default:
        widDir = "E";
        flage = 1;
      }
    break;
    case 0x03:
      switch(lastdir){
        case 0x0B:
        break;
        case 0x05:
        break;
        default:
        widDir = "SE";
        flage = 1;
      }
    break;
    case 0x0D:
      switch(lastdir){
        case 0x0B:
        break;
        case 0x05:
        break;
        case 0x07:
        break;
        default:
          widDir = "S";
          flage = 1;
      }
    break;
    case 0x0A:
      switch(lastdir){
        case 0x0B:
        break;
        case 0x0C:
        break;
        case 0x0E:
        break;
        case 0x03:
        break;
        case 0x05:
        break;
        default:
        widDir = "SW";
        flage = 1;
      }
    break;
    case 0x07:
      switch(lastdir){
        case 0x03:
        break;
        case 0x0B:
        break;
        case 0x05:
        break;
        default:
          widDir = "W";
          flage = 1;
      }
    break;
    case 0x0C:
      switch(lastdir){
        case 0x0E:
        break;
        case 0x05:
        break;
        case 0x0D:
        break;
        case 0x0A:
        break;
        default:
        widDir = "NW";
        flage = 1;
      }
    break;
    default:
    break;
    }
    if(flage == 1){
      flage = 0;
      lastdir = dir;
    }
}


void A(void)
{ 
  //dir = 0;
  //dir |= digitalRead(TMR1302S_U13_PIN)<<3 | /*B*/digitalRead(TMR1302S_U6_PIN)<<2 | /*C*/digitalRead(TMR1302S_U9_PIN)<<1 | /*D*/digitalRead(TMR1302S_U11_PIN)<<0;
  getCompassData1();
}

void B(void)
{
  //dir = 0;
  //dir |= digitalRead(TMR1302S_U13_PIN)<<3 | /*B*/digitalRead(TMR1302S_U6_PIN)<<2 | /*C*/digitalRead(TMR1302S_U9_PIN)<<1 | /*D*/digitalRead(TMR1302S_U11_PIN)<<0;
  getCompassData1();
}

void C(void)
{
  //dir = 0;
  //dir |= digitalRead(TMR1302S_U13_PIN)<<3 | /*B*/digitalRead(TMR1302S_U6_PIN)<<2 | /*C*/digitalRead(TMR1302S_U9_PIN)<<1 | /*D*/digitalRead(TMR1302S_U11_PIN)<<0;
 getCompassData1();
}

void D(void)
{
  // dir = 0;
  // dir |= digitalRead(TMR1302S_U13_PIN)<<3 | /*B*/digitalRead(TMR1302S_U6_PIN)<<2 | /*C*/digitalRead(TMR1302S_U9_PIN)<<1 | /*D*/digitalRead(TMR1302S_U11_PIN)<<0;
  getCompassData1();
}