#include "windCup.h"
extern sGeneral_t *pGeneral;
void initEeprom(void)
{
  uint8_t head = EEPROM.read(WIND_HEAD_ADDR);
  int16_t _offset,_linearfactor,stand1,atpresent;
  if(head != 0x55){
    uint8_t dataH = (2375 >> 8) &0xff;
    uint8_t dataL = 2375 & 0xff;
    int16_t _linearfactor2 = 0.46 * 100;
    uint8_t o_H = 0,o_L = 0,l_H = 0,l_L = 0;
    int16_t _offset2 = 0.77 * 100;
    o_H = (_offset2>>8) & 0xff;
    o_L = _offset2&0xff;
    l_H = (_linearfactor2>>8) & 0xff;
    l_L =  _linearfactor2&0xff; 
    EEPROM.write(WIND_HEAD_ADDR, 0x55);
    EEPROM.write(WIND_CUP_H, dataH);
    EEPROM.write(WIND_CUP_L, dataL);
    EEPROM.write(OFFSET_H, o_H);
    EEPROM.write(OFFSET_L, o_L);
    EEPROM.write(LINEARFACTOR_H, l_H);
    EEPROM.write(LINEARFACTOR_L, l_L);
    EEPROM.commit();
    pGeneral->sRadial = 23.75;
    pGeneral->sOffset = 0.77;
    pGeneral->sLinearFactor = 0.46;
  }else{
    _offset = (EEPROM.read(OFFSET_H)<<8 | EEPROM.read(OFFSET_L));
    _linearfactor = (EEPROM.read(LINEARFACTOR_H)<<8 | EEPROM.read(LINEARFACTOR_L));
    stand1 = (EEPROM.read(STAND1_H)<<8 | EEPROM.read(STAND1_L));
    atpresent = (EEPROM.read(ATPRESENT_H)<<8 | EEPROM.read(ATPRESENT_L));
    pGeneral->sRadial = (float)(EEPROM.read(WIND_CUP_H)<<8 | EEPROM.read(WIND_CUP_L))/100.0;
    pGeneral->sOffset = (float)_offset/100.0;
    pGeneral->sLinearFactor = (float)_linearfactor/100.0;
    pGeneral->sStandWindSpeed1 = (float)stand1 / 100.0;
    pGeneral->sAtPresentSpeed1 = (float)atpresent/100.0;
  }
}

void writeRadius(uint8_t *buf)
{
  EEPROM.write(WIND_CUP_H, buf[0]);
  EEPROM.write(WIND_CUP_L, buf[1]);
  EEPROM.commit();
}

void writeData(float offset,float linearfactor){
  int16_t _linearfactor2 = linearfactor * 100;
  uint8_t o_H = 0,o_L = 0,l_H = 0,l_L = 0;
  int16_t _offset2 = offset * 100;
  o_H = (_offset2>>8) & 0xff;
  o_L = _offset2&0xff;
  l_H = (_linearfactor2>>8) & 0xff;
  l_L =  _linearfactor2&0xff;
  EEPROM.write(OFFSET_H, o_H);
  EEPROM.write(OFFSET_L, o_L);
  EEPROM.write(LINEARFACTOR_H, l_H);
  EEPROM.write(LINEARFACTOR_L,l_L);
  EEPROM.commit();
  //delay(1000);
}
void writeStand1(uint8_t *buf){
  EEPROM.write(STAND1_H, buf[0]);
  EEPROM.write(STAND1_L, buf[1]);
  EEPROM.commit();
  //delay(500);
}
void writeAtpresent(float data){
  uint16_t _data = data * 100;
  uint8_t s_H = 0,s_L = 0;
  s_H = (_data>>8)&0XFF;
  s_L = _data&0XFF;
  EEPROM.write(ATPRESENT_H,s_H);
  EEPROM.write(ATPRESENT_L, s_L);
  EEPROM.commit();
  //delay(500);
}

