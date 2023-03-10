/**
 * @file rtc.cpp
 * @brief 实时时钟模块使用方法
 *
 * @copyright   Copyright (c) 2022 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-08-02
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 * 
 */
#include "global.h"
#include "string.h"
sPCF8563TTime_t time;
DFRobot_PCF8563T rtc;
char data[50]={0};
/**
 * @fn rtc_setup
 * @brief 实时时钟模块初始化
 * 
 */
void rtc_setup(){
    rtc.begin(2022,8,9,2,23,58,0);  //设置时间：2022/08/09，周二，00点00分00秒
}

/**
 * @fn get_rtc
 * @brief 获取实时时钟模块时间
 * 
 * @return sPCF8563TTime_t 返回时钟模块结构体，参数解释如下:
 * @n  second   秒，  范围0~59
 * @n  minute   分钟，范围0~59
 * @n  hour     小时，范围0~23
 * @n  day      天，  范围1~31
 * @n  week     周，  范围0~6
 * @n  month    月，  范围1~12
 * @n  year     年， 大于2000
 */
sPCF8563TTime_t get_rtc(){
    return rtc.getTime();
}
/**
 * @fn getHourMinute
 * @brief 以07:01格式获取时分的字符串表示
 * @return String 时间: 时:分
 */
uint32_t t_ms    = 0;
uint32_t t_count = 0;
uint8_t t_min = 0;
String getHourMinute(sPCF8563TTime_t time){
    String t = "";
    // uint32_t tr = 0;
    // sPCF8563TTime_t time = rtc.getTime();
    // if(sDeviceReg.REG_REFRESH_TIME){
    //     if(time.hour < 10) t += '0';
    //     t += String(time.hour) + ':';
    //     if(time.minute < 10) t += '0';
    //     t += String(time.minute) + ':';
    //     if(time.second < 10) t += '0';
    //     t += String(time.second);
    // }else{
    //     if(time.minute < 10) t += '0';
    //     t += String(time.minute) + ':';
    //     if(time.second < 10) t += '0';
    //     t += String(time.second) + '.';
    //     if((time.second != t_min)){
    //         t_min = time.second;
    //         t_ms = millis();
    //     }
    //     tr = (millis() - t_ms)%1000;
    //     tr = ((tr * 100)/1000.0); 
    //     if(tr < 10) t += '0';
    //     t += String(tr);
    // }
    return t;
}

/**
 * @fn getHourMinuteSecond
 * @brief 以07:01:01格式获取时分秒的字符串表示
 * 
 * @return String 时间: 时:分
 */
String getHourMinuteSecond(){
    String str = "";
    sPCF8563TTime_t time = rtc.getTime();
    str += String(time.year);
    str += '_';
    if(time.month < 10) str += '0';
    str += String(time.month) + '_';
    if(time.day < 10) str += '0';
    str += String(time.day) + '_';
    if(time.hour < 10) str += '0';
    str += String(time.hour) + ':';
    if(time.minute < 10) str += '0';
    str += String(time.minute) + ':';
    if(time.second < 10) str += '0';
    str += String(time.second);
    return str;
}

String convertHourMinuteSecond(sPCF8563TTime_t t){
    String rlt = "";
    if(t.hour < 10) rlt += "0";
    rlt += String(t.hour) + ":";
    if(t.minute < 10) rlt += "0";
    rlt += String(t.minute) + ":";
    if(t.second < 10) rlt += "0";
    rlt += String(t.second);
    return rlt;
}

char* convertDataFileName(sPCF8563TTime_t t){
    //if(t.year < 1000) rlt += '0';
    //if(t.year < 100) rlt += '0';
    //if(t.year < 10) rlt += '0';
    // rlt += String(t.year) + '_';
    // if(t.month < 10) rlt += '0';
    // rlt += String(t.month) + '_';
    // if(t.day < 10) rlt += '0';
    // rlt += String(t.day) + '_';

    // if(t.hour < 10) rlt += '0';
    // rlt += String(t.hour) + '_';
    // if(t.minute < 10) rlt += '0';
    // rlt += String(t.minute) + '_';
    // if(t.second < 10) rlt += '0';
    // rlt += String(t.second) + ".CSV";
    strcat(data,((String)t.year).c_str());
    strcat(data,"_");
    if(t.month < 10) strcat(data,"0");
    strcat(data,((String)t.month).c_str());
    strcat(data,"_");
    if(t.day < 10) strcat(data,"0");
    strcat(data,((String)t.day).c_str());
    strcat(data,"_");
    if(t.hour < 10) strcat(data,"0");
    strcat(data,((String)t.hour).c_str());
    strcat(data,"_");
    if(t.minute < 10)strcat(data,"0");
    strcat(data,((String)t.minute).c_str());
    strcat(data,"_");
    if(t.second < 10) strcat(data,"0");
    strcat(data,((String)t.second).c_str());
    strcat(data,".CSV");
    return data;
}
