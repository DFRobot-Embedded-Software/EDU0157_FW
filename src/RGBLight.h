/**
 * @file RGBLight.h
 * @author your name (you@domain.com)
 * @brief RGB 颜色输出
 * @version 0.1
 * @date 2023-03-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef RGBLIGHT_H
#define RGBLIGHT_H
#include <Adafruit_NeoPixel.h>
typedef enum{
    eYellow,
    eRed,
    eGreen,
    eBlue,
    eViolet,
    eBlack,
    eCyan,//配置文件错误
}eColor_t;

/**
 * @brief 设置RGB灯颜色
 * 
 * @param color RGB灯颜色
 */
void setRGBColor(eColor_t color);
/**
 * @brief RGB颜色输出
 * 
 * @param c 输出颜色
 * @param wait 颜色间隔时间
 */
void colorWipe(uint32_t c, uint8_t wait);
/**
 * @brief  初始化RGB灯
 * 
 */
void initRGB(void);

#endif
