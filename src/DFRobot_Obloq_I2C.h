#ifndef _DFROBOT_OBLOQ_I2C_H
#define _DFROBOT_OBLOQ_I2C_H
#include <Arduino.h>
//#include <SimpleList.h>
#include <Wire.h>


#define DEBUGGER 0
#define SUBSCRIBE_UPPER_LIMIT 5

typedef uint8_t   BYTE;

#define DEBUGGER_LOG(PARAM, ...)     \
    if (DEBUGGER || PARAM->debug)    \
    {                                \
        Serial1.print("[");           \
        Serial1.print(__FUNCTION__);  \
        Serial1.print("(): ");        \
        Serial1.print(__LINE__);      \
        Serial1.print(" ] ");         \
        Serial1.println(__VA_ARGS__); \
    }

#define MUTEX_TICK(PARAM, ...)            \
    if (!PARAM->tickLock)                 \
    {                                     \
        DEBUGGER_LOG(PARAM, __VA_ARGS__); \
        PARAM->tickLock = true;           \
        PARAM->tick();                    \
        PARAM->tickLock = false;          \
    }
                    

#if defined(NRF5) || defined(NRF52833)
#define CREATE_TASK(HANDLE, ...) create_fiber(HANDLE, ##__VA_ARGS__);
#elif defined(ARDUINO_ARCH_RP2040)
#define CREATE_TASK(HANDLE, NAME, OBJ) xTaskCreate(HANDLE, NAME, 256, OBJ, 1, nullptr);
#else
#define CREATE_TASK(HANDLE, ...) HANDLE(__VA_ARGS__)
#endif
#if defined(ARDUINO_ARCH_RP2040)

#define RUN_TOPIC_STATIC(INDEX, PARAM)                       \
    if (!this->mqttCbBusy[INDEX] && this->mqttHandle[INDEX]) \
    {                                                        \
        switch (INDEX + RETURN_COMMAND_MQTT_TOPIC0)          \
        {                                                    \
        case RETURN_COMMAND_MQTT_TOPIC0:                     \
            CREATE_TASK(topic_task0, "topic_task0",PARAM);   \
            break;                                           \
        case RETURN_COMMAND_MQTT_TOPIC1:                     \
            CREATE_TASK(topic_task1, "topic_task1",PARAM);   \
            break;                                           \
        case RETURN_COMMAND_MQTT_TOPIC2:                     \
            CREATE_TASK(topic_task2, "topic_task2",PARAM);   \
            break;                                           \
        case RETURN_COMMAND_MQTT_TOPIC3:                     \
            CREATE_TASK(topic_task3, "topic_task3",PARAM);   \
            break;                                           \
        case RETURN_COMMAND_MQTT_TOPIC4:                     \
            CREATE_TASK(topic_task4, "topic_task4",PARAM);   \
            break;                                           \
        }                                                    \
    }

#else

#define RUN_TOPIC_STATIC(INDEX, PARAM)                       \
    if (!this->mqttCbBusy[INDEX] && this->mqttHandle[INDEX]) \
    {                                                        \
        switch (INDEX + RETURN_COMMAND_MQTT_TOPIC0)          \
        {                                                    \
        case RETURN_COMMAND_MQTT_TOPIC0:                     \
            CREATE_TASK(topic_task0, PARAM);                 \
            break;                                           \
        case RETURN_COMMAND_MQTT_TOPIC1:                     \
            CREATE_TASK(topic_task1, PARAM);                 \
            break;                                           \
        case RETURN_COMMAND_MQTT_TOPIC2:                     \
            CREATE_TASK(topic_task2, PARAM);                 \
            break;                                           \
        case RETURN_COMMAND_MQTT_TOPIC3:                     \
            CREATE_TASK(topic_task3, PARAM);                 \
            break;                                           \
        case RETURN_COMMAND_MQTT_TOPIC4:                     \
            CREATE_TASK(topic_task4, PARAM);                 \
            break;                                           \
        }                                                    \
    }

#endif

#define WRITE_PARAMETER(COMMAND, DATA) \
    write(CommandWriteIndex(COMMAND), DATA);
#define CHECK_RETURN_COMMAND(COMMAND)          \
    (COMMAND >= RETURN_COMMAND_NONE &&         \
     COMMAND <= RETURN_COMMAND_MQTT_PUBLISH && \
     COMMAND != RETURN_COMMAND_RESERVE_0x0B && \
     COMMAND != RETURN_COMMAND_RESERVE_0x0C && \
     COMMAND != RETURN_COMMAND_RESERVE_0x0D && \
     COMMAND != RETURN_COMMAND_RESERVE_0x0E && \
     COMMAND != RETURN_COMMAND_RESERVE_0x0F)

typedef void (*MqttHandle)(String &message);
typedef void (*DebugHandle)(String &message, int8_t code);
typedef MqttHandle MsgHandleCb;

class MessageInfo
{
public:
    bool available;
    String message;
};

enum CommandWireIndex
{
    COMMAND_WIRE_ADDRESS = 0x16,
    COMMAND_WIRE_REGTSTER = 0x1E,
    COMMAND_WIRE_DATA = 0x22,
};

enum CommandType
{
    COMMAND_TYPE_READ,
    COMMAND_TYPE_WRITE,
    COMMAND_TYPE_EXECUTE,
};

enum CommandWriteIndex
{
    COMMAND_WRITE_NONE,
    COMMAND_WRITE_WIFI_NAME,
    COMMAND_WRITE_WIFI_PASSWORD,
    COMMAND_WRITE_MQTT_SERVER,
    COMMAND_WRITE_MQTT_PORT,
    COMMAND_WRITE_MQTT_ID,
    COMMAND_WRITE_MQTT_PWD,
    COMMAND_WRITE_HTTP_IP,
    COMMAND_WRITE_HTTP_PORT,
    COMMAND_WRITE_MQTT_CLIENTID
};

enum CommandExecutEIndex
{
    COMMAND_EXECUTE_NONE,
    COMMAND_EXECUTE_PING,
    COMMAND_EXECUTE_WIFI_CONNECT,
    COMMAND_EXECUTE_WIFI_RECONNECT,
    COMMAND_EXECUTE_WIFI_DISCONNECT,
    COMMAND_EXECUTE_MQTT_CONNECT,
    COMMAND_EXECUTE_MQTT_SUBSCRIBE0,
    COMMAND_EXECUTE_MQTT_SUBSCRIBE1,
    COMMAND_EXECUTE_MQTT_SUBSCRIBE2,
    COMMAND_EXECUTE_MQTT_SUBSCRIBE3,
    COMMAND_EXECUTE_MQTT_SUBSCRIBE4,
    COMMAND_EXECUTE_MQTT_PUBLISH0,
    COMMAND_EXECUTE_MQTT_PUBLISH1,
    COMMAND_EXECUTE_MQTT_PUBLISH2,
    COMMAND_EXECUTE_MQTT_PUBLISH3,
    COMMAND_EXECUTE_MQTT_PUBLISH4,
    COMMAND_EXECUTE_HTTP_GET,
    COMMAND_EXECUTE_HTTP_POST,
    COMMAND_EXECUTE_HTTP_PUT,
    COMMAND_EXECUTE_VERSION,
    COMMAND_EXECUTE_ONENETMQTT_CONNECT,
    COMMAND_EXECUTE_RESEAT = 0X17,
    COMMAND_NEW_PUBLICH = 0x18,
};

enum CommandReadIndex
{
    COMMAND_READ = 0x06,
};

enum CommandReadTypeIndex
{
    COMMAND_READTYPE_REG,
    COMMAND_READTYPE_DATA
};

enum ReturnCommandIndex
{
    RETURN_COMMAND_NONE,
    RETURN_COMMAND_PING,
    RETURN_COMMAND_WIFI,
    RETURN_COMMAND_WIFI_IP,
    RETURN_COMMAND_MQTT,
    RETURN_COMMAND_MQTT_SUBSCRIBE,
    RETURN_COMMAND_MQTT_TOPIC0,
    RETURN_COMMAND_MQTT_TOPIC1,
    RETURN_COMMAND_MQTT_TOPIC2,
    RETURN_COMMAND_MQTT_TOPIC3,
    RETURN_COMMAND_MQTT_TOPIC4,
    RETURN_COMMAND_RESERVE_0x0B,
    RETURN_COMMAND_RESERVE_0x0C,
    RETURN_COMMAND_RESERVE_0x0D,
    RETURN_COMMAND_RESERVE_0x0E,
    RETURN_COMMAND_RESERVE_0x0F,
    RETURN_COMMAND_HTTP,
    RETURN_COMMAND_HTTP_ERROR,
    RETURN_COMMAND_VERSION,
    RETURN_COMMAND_MQTT_PUBLISH
};

enum ReturnStatusIndex
{
    RETURN_STATUS_NONE,
    RETURN_STATUS_PING_SUCCESS,
    RETURN_STATUS_PING_FAILURE,
    RETURN_STATUS_VERSION_SUCCESS,
    RETURN_STATUS_WIFI_IDLE,
    RETURN_STATUS_WIFI_CONNECTING,
    RETURN_STATUS_WIFI_CONNECTED,
    RETURN_STATUS_WIFI_GETIP_SUCCESS,
    RETURN_STATUS_MQTT_IDLE,
    RETURN_STATUS_MQTT_CONNECT_SUCCESS,
    RETURN_STATUS_MQTT_CONNECT_FAILURE,
    RETURN_STATUS_MQTT_SUBSCRIBE_IDLE,
    RETURN_STATUS_MQTT_SUBSCRIBE_SUCCESS,
    RETURN_STATUS_MQTT_SUBSCRIBE_LIMIT,
    RETURN_STATUS_MQTT_SUBSCRIBE_FAILURE,
    RETURN_STATUS_MQTT_RECEIVE_MESSAGE,
    RETURN_STATUS_MQTT_PUBLISH_SUCCESS,
    RETURN_STATUS_MQTT_PUBLISH_FAILURE,
    RETURN_STATUS_HTTP_SUCCESS,
    RETURN_STATUS_HTTP_FAILURE,
    RETURN_STATUS_UNKNOWN

};

enum RunningNodeIndex
{
    RUNNING_NODE_NONE,
    RUNNING_NODE_PING,
    RUNNING_NODE_VERSION,
    RUNNING_NODE_WIFI_CONNECT,
    RUNNING_NODE_WIFI_GETIP,
    RUNNING_NODE_MQTT_CONNECT,
    RUNNING_NODE_RUNING
};

class DFRobot_Obloq_I2C
{
public:
    bool debug;
    String Version;
    String IPAddress;

    enum Topicnum
    {
        topic_0,
        topic_1,
        topic_2,
        topic_3,
        topic_4
    };

    uint8_t publish(const String topic, const String &message);

    DFRobot_Obloq_I2C(bool dbg = false);
    uint8_t wifiConnect(const String &ssid, const String &pwd);
    uint8_t mqttConnect(const String &iotId,
                     const String &iotPwd,
                     const String &server,
                     uint32_t port);
    void reset();
    

private:
    bool timer = false;
    long timerDuration;
    long timerStart;
    int wireInterval;
    long recordTime;
    bool tickLock;
    //SimpleList<String> mqttTopicArray;
    String returnMessage;
    DebugHandle debugHandle;
    MqttHandle mqttHandle[SUBSCRIBE_UPPER_LIMIT];
    bool mqttCbBusy[SUBSCRIBE_UPPER_LIMIT];
    bool returnStatusList[RETURN_STATUS_UNKNOWN];
    RunningNodeIndex runningNode;
    ReturnStatusIndex returnStatus;
    MessageInfo mqtt_message[SUBSCRIBE_UPPER_LIMIT];


    bool idle(long timeout = 10000);
    
    //void background();
    void timerInit(long time);
    bool timerCount();
    void peek(String message, int8_t code);


    uint8_t read(void *pBuf, size_t size, CommandReadTypeIndex type);
    void write(CommandWriteIndex command, const String &data);
    void execute(CommandExecutEIndex command);
    void execute(CommandExecutEIndex command, const String &data);

    void writeReg(uint8_t address, uint8_t reg, void *pBuf, size_t size);
    uint8_t readReg(uint8_t address, uint8_t reg, void *pBuf, size_t size);
};
extern DFRobot_Obloq_I2C obloq;
#endif //_DFROBOT_OBLOQ_I2C_H
