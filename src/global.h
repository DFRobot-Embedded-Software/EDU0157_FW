/*!
 * @file global.h
 * @brief 为了防止变量被重复定义，导致编译不过，所以在这里声明了此项目中需要用到的所有的全局变量
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-08-02
 * @url https://github.com/DFRobot-Embedded-Software/DFR0999-FW
 */
#ifndef GLOBAL_H
#define GLOBAL_H

#include <Arduino.h>
#include <Wire.h>
#include "DFRobot_PCF8563T.h"
#include "software_iic.h"
#include <Adafruit_NeoPixel.h>
#include "DFRobot_MAX17043.h"



#ifdef __cplusplus
extern "C"{
#endif
#include "DFRobot_queue.h"

#ifdef __cplusplus
} // extern "C"
#endif

//#define ENABLE_UD_OPERATION_DBG
#ifdef ENABLE_UD_OPERATION_DBG
#define GLOBAL_DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define GLOBAL_DBG(...)
#endif


#define EEPROM_TOTAL_SIZE 256

/******以下变量和函数在 ud.cpp 文件中定义******/
#define I2CMODE 1
#define UARTMODE 2


#define W2812_PIN             6
#define TMR1302S_EXTEND_PIN   7
//#define SCO1_INPUT_PIN        11
#define TMR1302S_U9_PIN       12
#define TMR1302S_U6_PIN       13
#define TMR1302S_U11_PIN      14
#define TMR1302S_U13_PIN      15
#define FLASH_IO3_PIN         20
#define FLASH_IO2_PIN         21
#define HM3445_PIN            23
//#define MAX17043_POWER_PIN    24
//#define SC01_POWER_PIN        22

#define TMR1302S_POWER_PIN    22
#define EXTEND_POWER          24
#define WIRE_POWER_PIN        9

#define COLOR_GREEN           1
#define COLOR_BLUE            2
#define COLOR_FLICKER1        3//I2C模式下开启存储
#define COLOR_FLICKER2        4//UART模式下开启存储
#define COLOR_ERROR           5//有传感器初始化失败
#define COLOR_OFF             6 //关闭状态
#define COLOR_ADDR_REPETITION 7//地址冲突

#define DATA_SAVE             0
#define DATA_NOT_STORE        1

#define ACCEPT_START          0
#define ACCEPT_DATA_55        1
#define ACCEPT_DATA_AA        2
#define ACCEPT_DATA_LENGTH    3
#define ACCEPT_DATA_READ      4

#define ACCEPT_ERROR           0
#define ACCEPT_SUCCESS         1
#define DATA_NOT_FOUNT         2


typedef struct{
  uint8_t communicationMode;//通信模式
  uint8_t ligthState;//灯光状态
  uint8_t saveState;//是否开启存储
  //uint8_t touchState;//触摸状态
  uint32_t sampleRate;//采样速率
  uint16_t interrcount;//用于测量风速的中断计数
  float    windCup;//风杯半径
  uint8_t extendDevice;
  uint32_t delayTime;
  uint8_t saveStateTime;
  uint8_t interruptData;
  uint8_t colorcount;
}sGeneral_t;




#define OBJECT_IF0_REF_VOLTAGE   3300.0 //3.3V
#define OBJECT_IF0_RESOLUTION    4096.0 //
//接口0对应的IO引脚
extern uint8_t OBJECT_IF0_A_D_PIN;
//接口1对应的I2C对象
//extern SoftwareTwoWire OBJECT_SOF_WIRE1;
//接口2对应的I2C对象
//extern SoftwareTwoWire OBJECT_SOF_WIRE2;

//接口1对应的I2C对象初始化
extern void SOF_WIRE1_INIT();


//定义函数的指针，此函数用来传感器初始化
typedef struct sensorAttr *(*I2C_SENSOR_SETUP)(uint8_t ifn, uint8_t address);
//定义函数的指针，此函数用来获取传感器实际数据
typedef void (*I2C_SENSOR_FUN)(uint8_t ifn, struct keyValue *);


//可以用链表解决I2C连接多个传感器的问题
//每个传感器有一个或多个属性值，单一属性值的表示为 数据名:数值值 单位
struct keyValue{
  struct keyValue *next; ///< 指向下一个属性结构体
  String key;            ///< 数据名
  String value;          ///< 数据值
  String unit;           ///< 数据单位
};

struct sensorAttr{
    uint8_t count;         ///< 传感器属性个数，例如温湿度传感器,可以获取温度和湿度2个属性，这里的值为2，它也表示attr指向的链表的个数
    struct keyValue *attr; ///< 指向传感器的属性集合
};

struct I2CSensorBase{
  String sku;                       ///<传感器名称
  String name;                      ///<传感器SKU
  I2C_SENSOR_SETUP setup;           ///<传感器初始化函数指针
  I2C_SENSOR_FUN   call;            ///<传感器数据采集调用函数指针
};

//此结构体用于注册I2C传感器的信息列表，包含所有支持的传感器，根据SKU索引
struct I2CSensorListTable{
  struct I2CSensorListTable *next; ///< 指向下一个 I2CSensorListTable 结构体
  struct I2CSensorBase *base;      ///< 指向I2C传感器属性，包括SKU，传感器名，以及其他函数
};

//有一个功能是要求能够根据检测到的I2C地址，确定是哪个传感器
//由于一个I2C地址可以对应多个传感器，所以我们需要链表来记录哪些传感器是同一个地址
//一个传感器也可以对应多个I2C地址(部分传感器可以修改地址)
struct i2cSKU{
    struct i2cSKU *next;         ///< 指向下一个SKU结构体
    union{
      struct{
        uint8_t if1  : 1;        ///< 此传感器已经在接口1上线，0：离线，1：在线
        uint8_t if2  : 1;        ///< 此传感器已经在接口2上线，0：离线，1：在线
        uint8_t sel1 : 1;        ///< 接口1上是否检测到了一对多(1个I2C地址对应多个SKU)的传感器
        uint8_t sel2 : 1;        ///< 接口2上是否检测到了一对多(1个I2C地址对应多个SKU)的传感器
        uint8_t ifcsv: 4;
      };
      uint8_t online;                 ///< 检测传感器是否在线
    };
    
    struct I2CSensorBase *base;
};

//创造一个根据I2C地址索引传感器属性的结构体
struct i2cAddrIndexList{
  struct i2cAddrIndexList *next;
  uint8_t addr;
  uint8_t count;             ///< 某个I2C地址对应的传感器个数
  union{
      struct{
        uint8_t if1  : 1;        ///< 表示此地址能在接口1上索引到，0：离线，1：在线
        uint8_t if2  : 1;        ///< 表示此地址能在接口2上索引到，0：离线，1：在线
        uint8_t sel1 : 1;        ///< 接口1上是否检测到了一对多(1个I2C地址对应多个SKU)的传感器
        uint8_t sel2 : 1;        ///< 接口2上是否检测到了一对多(1个I2C地址对应多个SKU)的传感器
        uint8_t sel : 1;         ///< 选择了接口0，还是1
        uint8_t ifcsv: 3;
      };
       uint8_t online;                 ///< 表示此地址能在接口上索引到
  };
  struct i2cSKU *head;
  struct i2cSKU *tail;
};

///////////////////////////////////////接口类///////////////////////////


struct I2CSensorInfo{
  struct I2CSensorBase *base;
  struct sensorAttr *attr;  ///< 指向结构体 sensorAttr
  uint8_t addr;             ///< 传感器I2C地址
  struct I2CSensorInfo *next;  ///< 指向下一个结构体，一个I2C接口可以连接多个I2C地址传感器 
};


// 接口信息，当接口处于I2C模式下时，接口上能连接1个或多个传感器
struct interfaceInfo{
  union{
    struct{
      uint8_t mode  :1; ///< 0: Analog/I2C 模式， 1: Digital/UART模式
      uint8_t change:1; ///< 表示接口模式是否切换，1：切换了， 0: 未切换
      uint8_t init  :1; ///< 表示接口初始化，若初始化完成，该值将永远为0
      uint8_t setup :1; ///< 表示传感器初始化，若初始化完成，该值将永远为0
      uint8_t sel   :1; ///< 此位只在I2C模式下生效，用于检测该接口上是否检测到了1对多的传感器(1个I2C地址对应多个SKU) 
      uint8_t clear :1; ///< 此位用于检测和清除接口所对应的所有I2C的标志，及地址链表
      uint8_t funrsv:2; ///< 保留位
    };
    uint8_t fun;
  };
  uint8_t count;         ///< 此接口上连接的传感器的个数
  void *head;
  void *tail;
};


// 存放I2C传感器 SKU，产品名，初始化函数，采集数据函数等信息的链表
extern struct I2CSensorListTable *I2C_SENSOR_HEAD;
extern struct I2CSensorListTable *I2C_SENSOR_TAIL;
// 记录I2C传感器链表长度
extern uint32_t  I2C_SENSOR_COUNT;
/**
 * @fn I2C_SENSOR_ENQUEUE
 * @brief I2C传感器信息入队
 * 
 * @param sku    I2C传感器的SKU
 * @param name   I2C传感器的名称
 * @param setup  I2C传感器的初始化函数
 * @param fun    I2C传感器的数据采集函数
 */
extern void I2C_SENSOR_ENQUEUE(const char *sku, const char * name, I2C_SENSOR_SETUP setup, I2C_SENSOR_FUN fun);
/**
 * @fn I2C_SENSOR_REGISTER
 * @brief I2C传感器基本信息注册，每加入一个新的I2C传感器就更新这个注册函数
 */
extern void I2C_SENSOR_REGISTER();
/**
 * @brief 根据SKU获取I2C传感器的基本信息：SKU，产品名，初始化函数，采集数据函数
 * 
 * @param sku 7位传感器货号代码
 * @return struct I2CSensorBase* 
 */
extern struct I2CSensorBase *getI2CSensor(const char *sku);

//对于I2C传感器而言，一个I2C地址可能对应多个SKU，一个SKU可能会有多个传感器，为了节省空间花销，设计了以下链表用以完善这个功能
//存放I2C传感器 地址和SKU对应信息的链表，这里一个I2C地址可能对应多个SKU，一个SKU可能会有多个传感器
extern struct i2cAddrIndexList *I2C_ADDR_SKU_HEAD;
extern struct i2cAddrIndexList *I2C_ADDR_SKU_TAIL;
extern uint8_t I2C_ADDR_SKU_COUNT;
/**
 * @fn I2C_ADDR_SKU_ENQUEUE
 * @brief I2C传感器地址和SKU信息入队
 * 
 * @param addr 7位I2C地址
 * @param sku 7位传感器货号代码
 */
extern void I2C_ADDR_SKU_ENQUEUE(uint8_t addr, const char * sku);
/**
 * @fn I2C_SENSOR_REGISTER
 * @brief I2C传感器地址和SKU对应关系注册，每加入一个新的I2C传感器就更新这个注册函数
 */
extern void I2C_ADDR_SKU_REGISTER();
/**
 * @fn I2C_ADDR_SKU_FLAG_CLEAR
 * @brief 清除I2C_ADDR_SKU_HEAD链表中对应接口的所有传感器的选中标志
 * 
 * @param ifn I2C接口号，1：I2C&UART1，2：I2C&UART2
 */
//extern void I2C_ADDR_SKU_FLAG_CLEAR(uint8_t ifn);

// 此链表是为了解决功能：根据采集到的I2C地址，来自动调用对应的传感器驱动，如果有一个地址对应多个SKU该怎么办
//如果I2C1和I2C2接口采集到的传感器是一个地址对应多个SKU，此时就需要将此I2C地址相关的信息入队
extern struct i2cAddrIndexList *I2C_SKU_SEL_HEAD;
extern struct i2cAddrIndexList *I2C_SKU_SEL_TAIL;
// 记录传感器转接板采集到的一个地址对应多个传感器链表的个数
extern uint8_t I2C_SKU_SEL_COUNT;
/**
 * @fn I2C_SKU_SEL_ENQUEUE
 * @brief I2C传感器入队
 * 
 * @param p  指向struct i2cAddrIndexList *指针列表
 * @return true 
 * @return false 
 */
extern bool I2C_SKU_SEL_ENQUEUE(struct i2cAddrIndexList *p);
/**
 * @fn I2C_SKU_SEL_REMOVE
 * @brief 将I2C传感器从I2C_SKU_SEL_HEAD链表中移除
 * 
 * @param p  指向struct i2cAddrIndexList *指针列表
 * @param ifn I2C接口号，1：I2C&UART1，2：I2C&UART2
 */
extern void I2C_SKU_SEL_REMOVE(struct i2cAddrIndexList *p, uint8_t ifn);
/**
 * @fn I2C_ADDR_SEL_REMOVE_CLEAR
 * @brief 清除I2C_SKU_SEL_HEAD链表
 * 
 * @param ifn I2C接口号，1：I2C&UART1，2：I2C&UART2
 */
extern void I2C_ADDR_SEL_REMOVE_CLEAR(uint8_t ifn);
/**
 * @fn I2C_scan
 * @brief 如果I2C&UART1,I2C&UART2中一个或两个被配置为I2C模式的时候，会自动扫描接口上连接的I2C传感器
 * @n 如果扫描的I2C地址对应一个SKU，则会将它加入接口列表中，如果是对应多个，则会入队I2C_SKU_SEL_HEAD链表，
 * 并职位相应的标志
 */
extern void I2C_scan();

/**
 * @fn sensor_setup
 * @brief 传感器转接板上连接的传感器初始化
 */
extern void sensor_setup();

/**
 * @fn sensor_call
 * @brief 传感器转接板上连接的传感器数据采集
 */
extern void sensor_call();



//sGeneral_t *pGeneral_t;


  /**
   * @brief 初始化传感器、U盘、引脚等
   */
  void begin();
  /**
   * @brief 查看是否有config文件,如果没有就创建
   */
  extern void configfileRead();
  /**
   * @brief 初始化I2C
   */
  extern void i2cSlaveInit(void);
  /**
   * @brief I2C 从机接收数据
   */
  extern void i2cloop();
  /**
   * @brief UART 从机接收数据
   */
  extern void uartloop(void);

  /**
   * @brief 设置rtc初始时间
   * 
   */
  extern void rtc_setup(void);
  /**
   * @brief 获取指南针状态
   * 
   */
  extern String getCompassData(void);
  /**
   * @brief 获取RTC返回时间
   * 
   * @return sPCF8563TTime_t 
   */
  extern sPCF8563TTime_t get_rtc(void);
  /**
   * @brief 指示灯颜色
   * 
   */
  void indicatorColor(void);
  /**
   * @brief 存储CSV文件
   * 
   */
  void csvRecord(void);

  /**
   * @brief 获取数据，并判断是否需要存储
   * 
   */
  void sensorDataCollect(void);
  /**
   * @brief 获取文件名称
   * @return 返回文件名称
   */
  char *convertDataFileName(sPCF8563TTime_t t);
  /**
   * @brief 确定板载扩展传感器
   */
  //void iicScan(void);
  /**
   * @brief 获取扩展传感器数据
   */
  //void getExtendData(void);

  void clearRecvBuf();

  // void getExtendDeviceData(void);
  // void initExtendDevice(void);

  // void initDevice(uint16_t sku);
  // void getData(uint16_t sku,uint8_t state);

  void dataAnalysls(void* buf);

  void setGreen(void);
  void dateTime(uint16_t* date, uint16_t* time);


 

  String getHourMinuteSecond();
  String dataSplitting(void);

  String dataName(void);
  String extendData(uint16_t sku,uint8_t state);
  String convertHourMinuteSecond(sPCF8563TTime_t t);


  //extern void save(void);
 

// 用于互斥电脑写入文件和程序写入文件
//extern uFSWMutex_t file_w_mutex;
/**
 * @fn  msc_read_cb
 * @brief 读flash
 * 
 * @param lba       地址
 * @param buffer    存储读取数据的缓存
 * @param bufsize   读取字节的大小
 * @return int32_t 返回实际读取的字节
 */
extern int32_t msc_read_cb (uint32_t lba, void* buffer, uint32_t bufsize);
/**
 * @fn  msc_write_cb
 * @brief 写flash
 * 
 * @param lba       地址
 * @param buffer    指向写数据缓存
 * @param bufsize   写字节的大小
 * @return int32_t 返回实际写入的字节
 */
extern int32_t msc_write_cb (uint32_t lba, uint8_t* buffer, uint32_t bufsize);
/**
 * @fn msc_flush_cb
 * @brief 同步
 * 
 */
extern void msc_flush_cb (void);
/**
 * @fn ud_setup
 * @brief U盘初始化
 * 
 */
extern void ud_setup();
/**
 * @brief 初始化串口从机
 * 
 */
extern void initSerialSlave(void);
#endif

