#ifndef SENSOR_I2C_H
#define SENSOR_I2C_H
#include <Arduino.h>
#include "sku.h"
#include "../general.h"
#include "../configFile.h"
#include "software_iic.h"


#ifdef SENSOR_I2C_DBG_ENABLE
#define SENSOR_I2C_DBG(...) {Serial.print("sensor_i2c ");Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define SENSOR_I2C_DBG(...)
#endif

//定义函数的指针，此函数用来传感器初始化
typedef struct sensorAttr *(*I2C_SENSOR_SETUP)(uint8_t ifn, uint8_t address);
//定义函数的指针，此函数用来获取传感器实际数据
typedef void (*I2C_SENSOR_FUN)(uint8_t ifn, struct keyValue *);

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

//创造一个根据I2C地址索引传感器属性的结构体
struct i2cAddrIndexList{
  struct i2cAddrIndexList *next;
  uint8_t addr;
  uint8_t count;             ///< 某个I2C地址对应的传感器个数
  uint8_t port;              //接口
  // union{
  //     struct{
  //       uint8_t if1  : 1;        ///< 表示此地址能在接口1上索引到，0：离线，1：在线
  //       uint8_t if2  : 1;        ///< 表示此地址能在接口2上索引到，0：离线，1：在线
  //       uint8_t sel1 : 1;        ///< 接口1上是否检测到了一对多(1个I2C地址对应多个SKU)的传感器
  //       uint8_t sel2 : 1;        ///< 接口2上是否检测到了一对多(1个I2C地址对应多个SKU)的传感器
  //       uint8_t sel : 1;         ///< 选择了接口0，还是1
  //       uint8_t ifcsv: 3;
  //     };
  uint8_t online;                 ///< 表示此地址能在接口上索引到
  // };
  struct i2cSKU *head;
  struct i2cSKU *tail;
};

//有一个功能是要求能够根据检测到的I2C地址，确定是哪个传感器
//由于一个I2C地址可以对应多个传感器，所以我们需要链表来记录哪些传感器是同一个地址
//一个传感器也可以对应多个I2C地址(部分传感器可以修改地址)
struct i2cSKU{
    struct i2cSKU *next;         ///< 指向下一个SKU结构体
    // union{
    //   struct{
    //     uint8_t if1  : 1;        ///< 此传感器已经在接口1上线，0：离线，1：在线
    //     uint8_t if2  : 1;        ///< 此传感器已经在接口2上线，0：离线，1：在线
    //     uint8_t sel1 : 1;        ///< 接口1上是否检测到了一对多(1个I2C地址对应多个SKU)的传感器
    //     uint8_t sel2 : 1;        ///< 接口2上是否检测到了一对多(1个I2C地址对应多个SKU)的传感器
    //     uint8_t ifcsv: 4;
    //   };
    //   uint8_t online;                 ///< 检测传感器是否在线
    // };
    
    struct I2CSensorBase *base;
};

//可以用链表解决I2C连接多个传感器的问题
//每个传感器有一个或多个属性值，单一属性值的表示为 数据名:数值值 单位
struct keyValue{
  struct keyValue *next; ///< 指向下一个属性结构体
  String key;            ///< 数据名
  String value;          ///< 数据值
  String unit;           ///< 数据单位
};

struct I2CSensorInfo{
  struct I2CSensorBase *base;
  struct sensorAttr *attr;  ///< 指向结构体 sensorAttr
  uint8_t port;///<确定传感器是在那个I2C接口上
  uint8_t addr;             ///< 传感器I2C地址
  struct I2CSensorInfo *next;  ///< 指向下一个结构体，一个I2C接口可以连接多个I2C地址传感器 
};




/**
 * @fn I2C_SENSOR_REGISTER
 * @brief I2C传感器基本信息注册，每加入一个新的I2C传感器就更新这个注册函数
 */
void I2C_SENSOR_REGISTER(void);

/**
 * @fn I2C_SENSOR_REGISTER
 * @brief I2C传感器地址和SKU对应关系注册，每加入一个新的I2C传感器就更新这个注册函数
 */
void I2C_ADDR_SKU_REGISTER(void);

/**
 * @fn I2C_SENSOR_ENQUEUE
 * @brief I2C传感器信息入队
 * 
 * @param sku    I2C传感器的SKU
 * @param name   I2C传感器的名称
 * @param setup  I2C传感器的初始化函数
 * @param fun    I2C传感器的数据采集函数
 */
void I2C_SENSOR_ENQUEUE(const char *sku, const char * name, I2C_SENSOR_SETUP setup, I2C_SENSOR_FUN fun);

/**
 * @fn I2C_ADDR_SKU_ENQUEUE
 * @brief I2C传感器地址和SKU信息入队
 * 
 * @param addr 7位I2C地址
 * @param sku 7位传感器货号代码
 */
void I2C_ADDR_SKU_ENQUEUE(uint8_t addr, const char * sku);

/**
 * @fn I2C_SKU_SEL_ENQUEUE
 * @brief I2C传感器入队
 * 
 * @param p  指向struct i2cAddrIndexList *指针列表
 * @return true 
 * @return false 
 */
bool I2C_SKU_SEL_ENQUEUE(struct i2cAddrIndexList *p);

/**
 * @fn I2C_SKU_SEL_REMOVE
 * @brief 将I2C传感器从I2C_SKU_SEL_HEAD链表中移除
 * 
 * @param p  指向struct i2cAddrIndexList *指针列表
 * @param ifn I2C接口号，1：I2C&UART1，2：I2C&UART2
 */
void I2C_SKU_SEL_REMOVE(struct i2cAddrIndexList *p, uint8_t ifn);

/**
 * @fn I2C_ADDR_SEL_REMOVE_CLEAR
 * @brief 清除I2C_SKU_SEL_HEAD链表
 * 
 * @param ifn I2C接口号，1：I2C&UART1，2：I2C&UART2
 */
void I2C_ADDR_SEL_REMOVE_CLEAR(uint8_t ifn);

/**
 * @brief 根据SKU获取I2C传感器的基本信息：SKU，产品名，初始化函数，采集数据函数
 * 
 * @param sku 7位传感器货号代码
 * @return struct I2CSensorBase* 
 */
struct I2CSensorBase *getI2CSensor(const char *sku);
/**
 * @fn I2C_scan
 * @brief 如果I2C&UART1,I2C&UART2中一个或两个被配置为I2C模式的时候，会自动扫描接口上连接的I2C传感器
 * @n 如果扫描的I2C地址对应一个SKU，则会将它加入接口列表中，如果是对应多个，则会入队I2C_SKU_SEL_HEAD链表，
 * 并职位相应的标志
 */
void I2C_scan(void);

void SOF_WIRE1_INIT();
/**
 * @brief 将不在线传感器移除采集列表
 * 
 * @param p 传入比对数据
 */
void REMOVE_DATA_COLLECTION(struct i2cAddrIndexList *p);

/**
 * @fn sensor_setup
 * @brief 传感器转接板上连接的传感器初始化
 */
void sensor_setup();

/**
 * @fn sensor_call
 * @brief 传感器转接板上连接的传感器数据采集
 */
void sensor_call();

void deleteNode(struct I2CSensorInfo* head, uint8_t addr);


#endif