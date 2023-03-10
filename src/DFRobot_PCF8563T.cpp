/*!
 * @file DFRobot_PCF8563T.cpp
 * @brief 低功耗实时时钟和日历驱动库，提供可编程的时钟输出，中断输出，和低电压检测。
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-07-12
 * @https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 */

#include "DFRobot_PCF8563T.h"

/*控制和状态寄存器*/
#define REG_CNTRL_STATUS1_1  0x00 ///< control and status register 1
#define BIT_STATUS1_TEST1      7   ///< TEST1 function bit in REG_CNTRL_STATUS1_1 register, 0: normal mode, 1: EXT_CLK test mode 
#define MASK_STATUS1_TEST1     1   ///< TEST1 function mask
#define BIT_STATUS1_STOP       5   ///< 0: RTC source clock runs, 1: all RTC divider chain flip-flops are asynchronously set to logic 0; the RTC clock is stopped (CLKOUT at 32.768 kHz is still available)
#define MASK_STATUS1_STOP      1   ///< STOP function mask
#define BIT_STATUS1_TESTC      3   ///< 0: Power-On Reset (POR) override facility is disabled; set to logic 0 for normal operation, 1: Power-On Reset (POR) override may be enabled
#define MASK_STATUS1_TESTC     1   ///< TESTC function mask

#define REG_CNTRL_STATUS1_2  0x01  ///<  control and status register 2
#define BIT_STATUS2_TI_TP      4   ///< 0: INT is active when TF is active, 1:  INT pulses active
#define MASK_STATUS2_TI_TP     1   ///< TI_TP function mask
#define BIT_STATUS2_AF         3   ///< 0: read->alarm flag inactive, write-> alarm flag is cleared, 1:  read->alarm flag active, write->alarm flag remains unchanged
#define MASK_STATUS2_AF        1   ///< AF function mask
#define BIT_STATUS2_TF         2   ///< 0: read->timer flag inactive, write->timer flag is cleared, 1:  read->timer flag active, write->timer flag remains unchanged
#define MASK_STATUS2_TF        1   ///< TF function mask
#define BIT_STATUS2_AIE        1   ///< 0: alarm interrupt disabled, 1:  alarm interrupt enabled
#define MASK_STATUS2_AIE       1   ///< AIE function mask
#define BIT_STATUS2_TIE        0   ///< 0: timer interrupt disabled, 1:  timer interrupt enabled
#define MASK_STATUS2_TIE       1   ///< TIE function mask

/*时间和日期寄存器*/
#define REG_VL_SECONDS       0x02
#define REG_MINUTES          0x03
#define REG_HOURS            0x04
#define REG_DAYS             0x05
#define REG_WEEKDAYS         0x06
#define REG_CENTURY_MONTHS   0x07
#define REG_YEARS            0x08

/*警报寄存器*/
#define REG_ALARM_MINUTE     0x09
#define REG_ALARM_HOUR       0x0A
#define REG_ALARM_DAY        0x0B
#define REG_ALARM_WEEKDAY    0x0C

/*时钟控制寄存器*/
#define REG_CLKOUT_CTRL      0x0D

/*定时器寄存器*/
#define REG_TIMER_CTRL       0x0E ///< 定时器控制寄存器，控制定时器的时钟源，以及定时器的使能失能
#define REG_ALARM_TIMER      0x0F

typedef struct{
    union{
        struct{
            uint8_t secUnit:4;
            uint8_t secTen :3;
            uint8_t secVL  :1;
        };
        uint8_t second;
    };
    union{
        struct{
            uint8_t minUnit    :4;
            uint8_t minTen     :3;
            uint8_t minUnused  :1;
        };
        uint8_t minute;
    };
    union{
        struct{
            uint8_t hourUnit    :4;
            uint8_t hourTen     :2;
            uint8_t hourUnused  :2;
        };
        uint8_t hour;
    };
    union{
        struct{
            uint8_t dayUnit    :4;
            uint8_t dayTen     :2;
            uint8_t dayUnused  :2;
        };
        uint8_t day;
    };
    union{
        struct{
            uint8_t weekUnit    :3;
            uint8_t weekUnused  :5;
        };
        uint8_t week;
    };
    union{
        struct{
            uint8_t monUnit    :4;
            uint8_t monTen     :1;
            uint8_t monUnused  :2;
            uint8_t monC       :1;
        };
        uint8_t month;
    };
    union{
        struct{
            uint8_t yearUnit    :4;
            uint8_t yearTen     :4;
        };
        uint8_t year;
    };
}sTimeRegConfig_t, *pTimeRegConfig_t;
uint32_t  displayms;
DFRobot_PCF8563T::DFRobot_PCF8563T(TwoWire *pWire, uint8_t addr)
  :_pWire(pWire),_addr(addr){
    memset(&_t, 0, sizeof(_t));
}

void DFRobot_PCF8563T::begin(const __FlashStringHelper* date, const __FlashStringHelper* time){
    char buff[11];
    memcpy_P(buff, date, 11);
    _t.year = conv2d(buff + 9);
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
    switch (buff[0]) {
        case 'J': _t.month = buff[1] == 'a' ? 1 : _t.month = buff[2] == 'n' ? 6 : 7; break;
        case 'F': _t.month = 2; break;
        case 'A': _t.month = buff[2] == 'r' ? 4 : 8; break;
        case 'M': _t.month = buff[2] == 'r' ? 3 : 5; break;
        case 'S': _t.month = 9; break;
        case 'O': _t.month = 10; break;
        case 'N': _t.month = 11; break;
        case 'D': _t.month = 12; break;
    }
    _t.day = conv2d(buff + 4);
    memcpy_P(buff, time, 8);
    _t.hour = conv2d(buff);
    _t.minute = conv2d(buff + 3);
    _t.second = conv2d(buff + 6);

	_t.week = dayOfTheWeek(_t.year, _t.month, _t.day);

	begin(_t.year,_t.month,_t.day,_t.week,_t.hour,_t.minute,_t.second);
    displayms = millis();
}

void DFRobot_PCF8563T::begin(uint16_t year,uint8_t month,uint8_t day,uint8_t week,uint8_t hour,uint8_t minute,uint8_t second){
    sTimeRegConfig_t t;
    _t.year = year;
    _t.month = month;
    _t.day = day;
    _t.week = week;
    _t.hour = hour;
    _t.minute = minute;
    _t.second = second;
    if (year >= 2000)
        year -= 2000;
    t.second = decToBCD(second);
    t.minute = decToBCD(minute);
    t.hour   = decToBCD(hour);
    t.day    = decToBCD(day);
    t.week   = decToBCD(week);
    t.month = decToBCD(month);
    t.year   = decToBCD(year);
    writeData(REG_VL_SECONDS, &t, sizeof(t));
    //writeReg(REG_VL_SECONDS);
    //writeData(&t, sizeof(t));
}

void DFRobot_PCF8563T::setTime(sPCF8563TTime_t t){
    _t = t;
    begin(t.year,t.month,t.day,t.week,t.hour,t.minute,t.second);
}

sPCF8563TTime_t DFRobot_PCF8563T::getTime(){
    sTimeRegConfig_t tr;
    sPCF8563TTime_t tt;
    //sTimeRegConfig_t t;
    writeReg(REG_VL_SECONDS);
    readReg(&tr, sizeof(tr));

    tt.second = tr.secTen  * 10 + tr.secUnit;
    tt.minute = tr.minTen  * 10 + tr.minUnit;
    tt.hour   = tr.hourTen * 10 + tr.hourUnit;
    tt.day    = tr.dayTen  * 10 + tr.dayUnit;
    tt.week   = tr.weekUnit;
    tt.month = tr.monTen  * 10 + tr.monUnit;
    tt.year   = tr.yearTen * 10 + tr.yearUnit + 2000;
    return tt;
}

void DFRobot_PCF8563T::writeReg(uint8_t reg){
    _pWire->beginTransmission(_addr);
    _pWire->write(reg);
    _pWire->endTransmission();
}

void DFRobot_PCF8563T::writeData(void *pdata, int16_t len){
    if((pdata == NULL) || len == 0) return ;
    uint8_t *pBuf = (uint8_t *)pdata;
    _pWire->beginTransmission(_addr);
    _pWire->write(pBuf, len);
    _pWire->endTransmission();
}

void DFRobot_PCF8563T::writeData(uint8_t reg, void *pdata, int16_t len){
    if((pdata == NULL) || len == 0) return ;
    uint8_t *pBuf = (uint8_t *)pdata;
    _pWire->beginTransmission(_addr);
    _pWire->write(reg);
    _pWire->write(pBuf, len);
    _pWire->endTransmission();

}

int16_t DFRobot_PCF8563T::readReg(void *pdata, int16_t len){
    if((pdata == NULL) || len == 0) return 0;
    uint8_t *pBuf = (uint8_t *)pdata;
    _pWire->requestFrom(_addr, len);
    for(int16_t i = 0; i < len; i++){
        pBuf[i] = _pWire->read();
    }
    return len;
}

uint8_t DFRobot_PCF8563T::conv2d(const char* p){
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9')
        v = *p - '0';
    return 10 * v + *++p - '0';
}

uint8_t DFRobot_PCF8563T::dayOfTheWeek(uint16_t year, uint8_t month, uint8_t day)
{
	  uint16_t days =  date2days(year, month, day);
    return (days + 6) % 7; // 2000年1月1日是星期六，这一天会返回6
}

uint16_t DFRobot_PCF8563T::date2days(uint16_t year, uint8_t month, uint8_t day){
    if (year >= 2000)
        year -= 2000;
    uint16_t days = day;
    for (uint8_t i = 1; i < month; ++i){
        switch(i){
            case 2:
                  days += 28;
                  break;
            case 4:
            case 6:
            case 9:
            case 11:
                  days += 30;
                  break;
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
                  days += 31;
                  break;
        }
    }
    if (month > 2 && year % 4 == 0)
        ++days; //闰年
    //闰年分为普通闰年和世纪闰年
    //判断方法：公历年份（阳历）是4的倍数，且不是一百的倍数，为普通闰年，公历年份是整百数，且必须是400的倍数才是世纪闰年。
    //4年一闰，百年不润，400年再闰
    //2000年是闰年，2004是闰年，2100不是闰年，2104是闰年
    return days + 365 * year + (year + 3) / 4 - 1;
}

uint8_t DFRobot_PCF8563T::decToBCD(uint8_t num){
    return ((num / 10 * 16) + (num % 10));
}
