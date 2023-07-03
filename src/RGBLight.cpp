#include "RGBLight.h"

#define W2812_PIN             6

Adafruit_NeoPixel strip;

void setRGBColor(eColor_t color)
{
  switch(color){
    case eYellow:
      colorWipe(strip.Color(255, 255, 0), 50); 
      break;
    case eRed:
      colorWipe(strip.Color(255, 0, 0), 0); 
      break;
    case eGreen:
      colorWipe(strip.Color(0, 255, 0), 50); 
      break;
    case eBlue:
      colorWipe(strip.Color(0, 0, 255), 50); 
      break;
    case eViolet:
      colorWipe(strip.Color(255, 0, 255), 50); 
      break;
    case eBlack:
      colorWipe(strip.Color(0, 0, 0), 50); 
      break;
    case eCyan:
      colorWipe(strip.Color(0, 255, 255), 50); 
    default:
      break;
    }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

void initRGB(void)
{
  strip = Adafruit_NeoPixel(2, W2812_PIN, NEO_GRB + NEO_KHZ800);
}

