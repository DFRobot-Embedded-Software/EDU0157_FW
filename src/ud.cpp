/*!
 * @file ud.cpp
 * @brief U盘相关定义，及CSV文件创建
 * @n 依赖文件:
 * @n Adafruit_SPIFlash https://github.com/adafruit/Adafruit_SPIFlash
 * @n SdFat https://github.com/adafruit/SdFat
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-08-02
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 */

#include "global.h"
#include "string.h"
#include "Adafruit_TinyUSB.h"
#include "SPI.h"
#include "SdFat.h"
#include "Adafruit_SPIFlash.h"
// uint32_t data_samping_time = 0;
// uint8_t recordState = false;
//uFSWMutex_t file_w_mutex;

//Adafruit_FlashTransport_RP2040 flashTransport;
//Adafruit_FlashTransport_QSPI flashTransport(/*pinSCK =*/18, /*pinCS =*/17, /* pinIO0 =*/19, /*pinIO1 =*/16, /* pinIO2 =*/21, /* pinIO3 =*/20);


Adafruit_FlashTransport_SPI flashTransport(17, &SPI);
Adafruit_SPIFlash flash(&flashTransport);
// file system object from SdFat
FatFileSystem fatfs;
//FatFile file;
// USB Mass Storage object
Adafruit_USBD_MSC usb_msc;

// Set to true when PC write to flash
//FatFile *file;
char *fileName = NULL;//存储文件名称
// Check if flash is formatted
bool fs_formatted;
extern sGeneral_t *pGeneral_t;
extern uint16_t skuData[];
extern DFRobot_PCF8563T rtc;
void ud_setup(){
  flash.begin();
  // Set disk vendor id, product id and revision with string up to 8, 16, 4 characters respectively
  usb_msc.setID("DFRobot", "External Flash", "1.0");
  //GLOBAL_DBG(flash.size());
  // Set callback
  usb_msc.setReadWriteCallback(msc_read_cb, msc_write_cb, msc_flush_cb);
  usb_msc.setCapacity(flash.size()/512, 512);//512
  usb_msc.setUnitReady(true);
  usb_msc.begin();

  // Init file system on the flash
  fs_formatted = fatfs.begin(&flash);
  if ( !fs_formatted ){
    GLOBAL_DBG("Failed to init files system, flash may not be formatted");
  }
  FsDateTime::setCallback(dateTime);
 
}

// call back for file timestamps
void dateTime(uint16_t* date, uint16_t* time) {   // hyy
  // DateTime now = RTC.now();
  sPCF8563TTime_t now = rtc.getTime();
  // Serial.print("now.year = ");Serial.println(now.year);
  // Serial.print("now.month = ");Serial.println(now.month);
  // Serial.print("now.day = ");Serial.println(now.day);
  // Serial.print("now.hour = ");Serial.println(now.hour);
  // Serial.print("now.minute = ");Serial.println(now.minute);
  // Serial.print("now.second = ");Serial.println(now.second);

  // return date using FAT_DATE macro to format fields
  *date = FAT_DATE(now.year, now.month, now.day);

  // return time using FAT_TIME macro to format fields
  *time = FAT_TIME(now.hour, now.minute, now.second);
}

// void csv_flag_scan(){
//   GLOBAL_DBG("csv_flag_scan");
//   if(file_w_mutex.usbwf) return;
//   GLOBAL_DBG("csv_flag_scan1");
  
//   if(sDeviceReg.REG_RECORD_ON){
//       file_w_mutex.programwf = 1;
//       sDeviceReg.REG_RTC = rtc.getTime();
//       if(sDeviceReg.REG_FILENAME.length() == 0){
//         sDeviceReg.REG_FILENAME = convertDataFileName(sDeviceReg.REG_RTC);
//         if(sDeviceReg.REG_FILENAME.length()){
//           File32 myfile = fatfs.open(sDeviceReg.REG_FILENAME, FILE_WRITE);
//           myfile.close();
//           sDeviceReg.REG_RECORD_UPDATE = 1; //是否需要写key
//         }
//       }else{
//         File32 myfile = fatfs.open(sDeviceReg.REG_FILENAME, FILE_WRITE);
//         myfile.seek(myfile.position());
//         myfile.write("\r\n");
//         myfile.close();
//         sDeviceReg.REG_RECORD_UPDATE = 0; //是否需要写key
//       }
      
//       sDeviceReg.REG_RECORD_ON = 0;
//       sDeviceReg.REG_RECORD_FLAG = 1;
//       //if(pageNum == 0) //oled_display(0);
//       recordState = true;
//       digitalWrite(RECORD_LED, recordState);
//     }
//     if(sDeviceReg.REG_RECORD_OFF){
//       sDeviceReg.REG_RECORD_OFF = 0;
//       sDeviceReg.REG_RECORD_FLAG = 0;
//       //if(pageNum == 0) oled_display(0);
//       recordState = false;
//       digitalWrite(RECORD_LED, recordState);
//       if(sDeviceReg.REG_RECORD_LONG){
//         sDeviceReg.REG_FILENAME = "";
//         sDeviceReg.REG_RECORD_LONG = 0;
//       }
//     }
// }

void configfileRead(){
  uint8_t buffer[200];
  char* str;
  File32 myfile;
  char* P;
  if(!fatfs.exists("config.txt")){//查看config文件是否存在
    GLOBAL_DBG("no config file");
    myfile = fatfs.open("config.txt",O_WRITE|O_CREAT|O_SYNC);
    if(!myfile){
       GLOBAL_DBG("open  config file error");
      return;
    }
    GLOBAL_DBG("open config file");
    myfile.write("Communication:I2C\r\n");
    myfile.write("Sample rate:1\r\n");
    myfile.write("Record:OFF\r\n");
    myfile.write("Radial:23.75\r\n");
    myfile.write("Delay record:10\r\n");
    myfile.write("Light Switch:ON\r\n");
    myfile.close();
    pGeneral_t->communicationMode = I2CMODE;
    pGeneral_t->ligthState = COLOR_GREEN;
  }
  uint16_t len = 0;
  myfile = fatfs.open("config.txt",FILE_READ);
  if(!myfile){
    GLOBAL_DBG("open config file error");
    return ;
  }
  GLOBAL_DBG("open config file");
  myfile.read(buffer,200);
  myfile.close();
  P = (char*)buffer;
  len = strlen(P);
  for(uint8_t i = 0; len > i;){
    str = strtok(P+i,"\r\n");
    i += strlen(str)+2;
    str = strtok(str,":");
    if(strcmp(str,"Communication") == 0){//模式检测
      str = strtok(NULL,":");
      GLOBAL_DBG("Communication");
      if(strcmp(str,"I2C") == 0){
        pGeneral_t->communicationMode = I2CMODE;
        GLOBAL_DBG("IIC");
      }else if(strcmp(str,"UART") == 0){
        pGeneral_t->communicationMode = UARTMODE;
        GLOBAL_DBG("UARTMODE");
      }
    }
    if(strcmp(str,"Sample rate") == 0){
      GLOBAL_DBG("Sample rate");
      str = strtok(NULL,":");
      GLOBAL_DBG(str);
      if(atoi(str) == 0){
        pGeneral_t->sampleRate = 1000;
      }else{
        pGeneral_t->sampleRate = atoi(str) * 1000;//获取存储速率
      }
    }
    if(strcmp(str,"Record") == 0){
      GLOBAL_DBG("Record");
      str = strtok(NULL,":");
      if(strcmp(str,"ON") == 0){
        pGeneral_t->saveState = DATA_SAVE;
      }else if(strcmp(str,"OFF") == 0){
        GLOBAL_DBG("OFF");
        pGeneral_t->saveState = DATA_NOT_STORE;
      }
    }
    if(strcmp(str,"Radial") == 0){
      str = strtok(NULL,":");
      pGeneral_t->windCup = String(str).toFloat();
    }
    if(strcmp(str,"Delay record") == 0){
      GLOBAL_DBG("Delay record");
      str = strtok(NULL,":");
      GLOBAL_DBG(str);
      pGeneral_t->delayTime = atoi(str) * 1000;
    }
    if(strcmp(str,"Light Switch") == 0){
      str = strtok(NULL,":");
      if(strcmp(str,"ON") == 0){
        if(pGeneral_t->communicationMode == I2CMODE){
          pGeneral_t->ligthState = COLOR_GREEN;
        }else{
          pGeneral_t->ligthState = COLOR_BLUE;
        }
      }else{
        pGeneral_t->ligthState = COLOR_OFF;
      }
    }
  }
  GLOBAL_DBG(pGeneral_t->sampleRate);
  GLOBAL_DBG(pGeneral_t->ligthState);
  GLOBAL_DBG(pGeneral_t->saveState);
  GLOBAL_DBG(pGeneral_t->delayTime);
}

void csvRecord(void){
  File32 myfile;
  if(pGeneral_t->saveStateTime == 1){
    if(pGeneral_t->saveState == DATA_SAVE){//需要存储到CSV文件中
      if(pGeneral_t->communicationMode == I2CMODE){
        pGeneral_t->ligthState = COLOR_FLICKER1;
      }else{
        pGeneral_t->ligthState = COLOR_FLICKER2;
      }
      if(fileName == NULL){
        GLOBAL_DBG("fileName NULL");
        String text = "Time,Wind Speed(m/s),Wind Direction,";//默认文件内容
        fileName = convertDataFileName(get_rtc());//aaa.csv
        GLOBAL_DBG(fileName);
        if(!fatfs.exists(fileName)){//查看想要创建的文件是否已经存在。如果不存在就创建
          myfile = fatfs.open(fileName,FILE_WRITE);
          text += dataName();
          text += "\r\n";
          myfile.write(text.c_str(),text.length());
          myfile.close();
          GLOBAL_DBG("creat CSV file ok");
        }
        }else{
        String data = dataSplitting();
        myfile = fatfs.open(fileName, FILE_WRITE);
        myfile.seek(myfile.position());
        myfile.write(data.c_str(),data.length());
        //myfile.write("0,0,0,0,0,0,0\r\n");
        myfile.close();
        GLOBAL_DBG("write data ok");
        GLOBAL_DBG(data);
      }
    }
  }
}
extern float wSpeed;
extern struct I2CSensorInfo *I2C_SENSOR_INFO_HEAD;
String dataSplitting(void){
struct I2CSensorInfo *p = I2C_SENSOR_INFO_HEAD;
  String str = "";
  str += getHourMinuteSecond();
  str += ",";
  str += String(wSpeed/100.0);
  str += ",";
  str += getCompassData();
  while(p){
    if(p->attr && p->attr->attr){
      struct keyValue *pkey = p->attr->attr;
      while(pkey){
        if(str.length() > 0) str += ",";
        str += pkey->value;
        pkey = pkey->next;
      }
    }
    p = p->next;
  }
  str += "\r\n";
  return str;
}

String dataName(void)
{
  String str = "";
  struct I2CSensorInfo *p = I2C_SENSOR_INFO_HEAD;
  while(p){
    if(p->attr && p->attr->attr){
      struct keyValue *pkey = p->attr->attr;
      while(pkey){
        if(str.length() > 0) str += ",";
        str += pkey->key;
        str += "(";
        str += pkey->unit;
        str += ")";
        pkey = pkey->next;
      }
    }
    p = p->next;
  }
  return str;
}

String extendData(uint16_t sku,uint8_t state)
{
  String str = ",";
  // uint32_t raw_counts = 0;
  // float luxData;
  // if(sku == skuData[0] ){//环境光传感器
  //   if(state == 0){
  //     raw_counts = sInputReg.SKU1DATA1 | sInputReg.SKU1DATA2 << 16;
  //   }else if(state == 1){
  //     raw_counts = sInputReg.SKU1DATA1 | sInputReg.SKU1DATA2 << 16;
  //   }
  //   luxData = als.scaleLux(raw_counts);
  //   str += (String)luxData;
  // }else if(sku == skuData[1]){
  //   if(state == 0){
  //     str += (String)sInputReg.SKU1DATA1 + "," +(String)sInputReg.SKU1DATA2 + "," +\
  //            (String)sInputReg.SKU1DATA3 + "," +(String)sInputReg.SKU1DATA4;
  //   }else if(state == 1){
  //     str += (String)sInputReg.SKU2DATA1 + "," +(String)sInputReg.SKU2DATA2 + "," +\
  //            (String)sInputReg.SKU2DATA3 + "," +(String)sInputReg.SKU2DATA4;
  //   }
  // }else if(sku == skuData[2]){
  //   if(state == 0){
  //     str += (String)sInputReg.SKU1DATA1 + "," +(String)sInputReg.SKU1DATA2 + "," +\
  //            (String)sInputReg.SKU1DATA3;
  //   }else if(state == 1){
  //     str += (String)sInputReg.SKU2DATA1 + "," +(String)sInputReg.SKU2DATA2 + "," +\
  //            (String)sInputReg.SKU2DATA3;
  //   }
  // }else if( sku == skuData[3]){
  //   if(state == 0){
  //     str += (String)sInputReg.SKU1DATA1 + "," +(String)sInputReg.SKU1DATA2 + "," +\
  //            (String)sInputReg.SKU1DATA3;
  //   }else if(state == 1){
  //     str += (String)sInputReg.SKU2DATA1 + "," +(String)sInputReg.SKU2DATA2 + "," +\
  //            (String)sInputReg.SKU2DATA3;
  //   }
  // }else if(sku == skuData[4]){

  //   if(state == 0){
  //     str += (String)sInputReg.SKU1DATA1 + "," +(String)sInputReg.SKU1DATA2 + "," +\
  //            (String)sInputReg.SKU1DATA3 + "," +(String)sInputReg.SKU1DATA4 + "," +\
  //            (String)sInputReg.SKU1DATA5 + "," +(String)sInputReg.SKU1DATA6 + "," +\
  //            (String)sInputReg.SKU1DATA7 + "," +(String)sInputReg.SKU1DATA8;
  //   }else if(state == 1){
  //     str += (String)sInputReg.SKU2DATA1 + "," +(String)sInputReg.SKU2DATA2 + "," +\
  //            (String)sInputReg.SKU2DATA3 + "," +(String)sInputReg.SKU2DATA4 + "," +\
  //            (String)sInputReg.SKU2DATA5 + "," +(String)sInputReg.SKU2DATA6 + "," +\
  //            (String)sInputReg.SKU2DATA7 + "," +(String)sInputReg.SKU2DATA8;
  //   }
  // }else if( sku == skuData[5]){
  //   if(state == 0){
  //     str += (String)sInputReg.SKU1DATA1 + "," +(String)sInputReg.SKU1DATA2 + "," +\
  //            (String)sInputReg.SKU1DATA3;
  //   }else if(state == 1){
  //     str += (String)sInputReg.SKU2DATA1 + "," +(String)sInputReg.SKU2DATA2 + "," +\
  //            (String)sInputReg.SKU2DATA3;
  //   }
  // }else if(sku == skuData[6]){
  //   if(state == 0){
  //     str += (String)sInputReg.SKU1DATA1;
  //   }else if(state == 1){
  //     str += (String)sInputReg.SKU2DATA1;
  //   }
  // }else if(sku == skuData[7]){
  //   if(state == 0){
  //     str += (String)sInputReg.SKU1DATA1;
  //   }else if(state == 1){
  //     str += (String)sInputReg.SKU2DATA1;
  //   }
  // }else if(sku == skuData[8]){
  //   if(state == 0){
  //     str += (String)sInputReg.SKU1DATA1;
  //   }else if(state == 1){
  //     str += (String)sInputReg.SKU2DATA1;
  //   }
  // }
  return str;
}

// void csv_record(){
//   //csv文件记录
//     if(file_w_mutex.usbwf) return;
//     if(sDeviceReg.REG_RECORD_FLAG && (sDeviceReg.REG_ANALOG || (sDeviceReg.REG_RECORD_IF0 && sDeviceReg.REG_IF0->count) || (sDeviceReg.REG_RECORD_IF1 && sDeviceReg.REG_IF1->count) || (sDeviceReg.REG_RECORD_IF2 && sDeviceReg.REG_IF2->count)) && sDeviceReg.REG_FILENAME.length()){
//       //bool record = false;
//       String keys = "Time";
//       String values = sDeviceReg.REG_TIME_STAMP + " ";
//       if(sDeviceReg.REG_REFRESH_TIME == 0) values = String("'") + values;
//       if(sDeviceReg.REG_RECORD_IF0){
//         if(sDeviceReg.REG_ANALOG){
//           float voltage_mv = (analogRead(OBJECT_IF0_A_D_PIN) * OBJECT_IF0_REF_VOLTAGE)/OBJECT_IF0_RESOLUTION;
//           keys += ',' + String("Analog") + String("(mV)");
//           values += ',' + String(voltage_mv);
//         }else{
//             if(sDeviceReg.REG_IF0 && (sDeviceReg.REG_IF0->change == 0) && sDeviceReg.REG_IF0->init){
//               struct ADSensorInfo *info = (struct ADSensorInfo *)sDeviceReg.REG_IF0->head;
//               if(info && info->attr){
//                 struct keyValue *pkey = info->attr->attr;
//                 while(pkey){
//                   keys += ',' + pkey->key + (pkey->unit.length() ? ('('+pkey->unit+')') : (' '));
//                   values += ',' + pkey->value;
//                   pkey = pkey->next;
//                 }
//               }
//             }
//         }
//       }
//       if(sDeviceReg.REG_RECORD_IF1 && sDeviceReg.REG_IF1 && (sDeviceReg.REG_IF1->change == 0) && sDeviceReg.REG_IF1->init){
//         switch(sDeviceReg.REG_IF1->mode){
//           case FUN_I2C:
//           {
//             struct I2CSensorInfo *info = (struct I2CSensorInfo *)sDeviceReg.REG_IF1->head;
//             while(info){
//               if(info && info->attr){
//                 struct keyValue *pkey = info->attr->attr;
//                   while(pkey){
//                   keys += ',' + pkey->key + (pkey->unit.length() ? ('('+pkey->unit+')') : (' '));
//                   values += ',' + pkey->value;
//                   pkey = pkey->next;
//                 }
//               }
//               info = info->next;
//             }
//             break;
//           }
//           case FUN_UART:
//           {
//             struct UARTSensorInfo *info = (struct UARTSensorInfo *)sDeviceReg.REG_IF1->head;
//             if(info && info->attr){
//               struct keyValue *pkey = info->attr->attr;
//               while(pkey){
//                 keys += ',' + pkey->key + (pkey->unit.length() ? ('('+pkey->unit+')') : (' '));
//                 values += ',' + pkey->value;
//                 pkey = pkey->next;
//               }
//             }
//             break;
//           }
//         }
//       }

//       if(sDeviceReg.REG_RECORD_IF2 && sDeviceReg.REG_IF2 && (sDeviceReg.REG_IF2->change == 0) && sDeviceReg.REG_IF2->init){
//         switch(sDeviceReg.REG_IF2->mode){
//           case FUN_I2C:
//           {
//             struct I2CSensorInfo *info = (struct I2CSensorInfo *)sDeviceReg.REG_IF2->head;
//             while(info){
//               if(info && info->attr){
//                 struct keyValue *pkey = info->attr->attr;
//                   while(pkey){
//                   keys += ',' + pkey->key + (pkey->unit.length() ? ('('+pkey->unit+')') : (' '));
//                   values += ',' + pkey->value;
//                   pkey = pkey->next;
//                 }
//               }
//               info = info->next;
//             }
//             break;
//           }
//           case FUN_UART:
//           {
//             struct UARTSensorInfo *info = (struct UARTSensorInfo *)sDeviceReg.REG_IF2->head;
//             if(info && info->attr){
//               struct keyValue *pkey = info->attr->attr;
//               while(pkey){
//                 keys += ',' + pkey->key + (pkey->unit.length() ? ('('+pkey->unit+')') : (' '));
//                 values += ',' + pkey->value;
//                 pkey = pkey->next;
//               }
//             }
//             break;
//           }
//         }
//       }
//       digitalWrite(RECORD_LED, recordState);
//       recordState = !recordState; 
//       File32 myfile = fatfs.open(sDeviceReg.REG_FILENAME, FILE_WRITE);
//       myfile.seek(myfile.position());
//       if(sDeviceReg.REG_RECORD_UPDATE) {
//         sDeviceReg.REG_RECORD_UPDATE = 0;
//         keys += "\r\n";
//         myfile.write(keys.c_str(), keys.length());
//       }
//       values += "\r\n";
//       myfile.write(values.c_str(), values.length());
//       Serial.print("record .....");Serial.println(values.c_str());
//       myfile.close();
//     }
// }

int32_t msc_read_cb (uint32_t lba, void* buffer, uint32_t bufsize)
{
  
  return flash.readBlocks(lba, (uint8_t*) buffer, bufsize/512) ? bufsize : -1;
}

// Callback invoked when received WRITE10 command.
// Process data in buffer to disk's storage and 
// return number of written bytes (must be multiple of block size)
int32_t msc_write_cb (uint32_t lba, uint8_t* buffer, uint32_t bufsize)
{
  // Note: SPIFLash Block API: readBlocks/writeBlocks/syncBlocks
  // already include 4K sector caching internally. We don't need to cache it, yahhhh!!
  return flash.writeBlocks(lba, buffer, bufsize/512) ? bufsize : -1;
}

// Callback invoked when WRITE10 command is completed (status received and accepted by host).
// used to flush any pending cache.
void msc_flush_cb (void)
{
  // sync with flash
  
  flash.syncBlocks();

  // clear file system's cache to force refresh
  fatfs.cacheClear();
}


