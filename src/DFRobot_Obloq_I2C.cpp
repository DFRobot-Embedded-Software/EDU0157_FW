#include "DFRobot_Obloq_I2C.h"
#include "general.h"

extern sGeneral_t *pGeneral;
DFRobot_Obloq_I2C::DFRobot_Obloq_I2C(bool dbg) : Version("0.0"), IPAddress("0.0.0.0"), wireInterval(20),
                                                 recordTime(millis()), timer(false), timerDuration(0), timerStart(0), debug(dbg),
                                                 returnMessage(""), debugHandle(NULL), runningNode(RUNNING_NODE_NONE), returnStatus(RETURN_STATUS_NONE)
{
    //reset();
}

uint8_t DFRobot_Obloq_I2C::publish(const String topic, const String &message)
{
    DEBUGGER_LOG(this, ">=====> publish");
    uint64_t timeStart= millis();
    uint64_t timeEnd;
    uint8_t cmdbuffer[2];
    String _message = String(pGeneral->sDTUSave);
    _message += ":";
    _message += topic;
    _message += ":";
    _message += message;
    execute(COMMAND_NEW_PUBLICH,_message);
    delay(100);
    while (1)
    { timeEnd = millis();
        if((timeEnd - timeStart) > 1000){
            return 2;
        }
        if (read(cmdbuffer, 2, COMMAND_READTYPE_REG) != 2)
        {
            DEBUGGER_LOG(this, "read error");
        }else
        {
            if (cmdbuffer[0] == RETURN_COMMAND_RESERVE_0x0D && cmdbuffer[1] == 0x01){
                DEBUGGER_LOG(this, "sub OK");
                break;
            }
        }
        delay(200);
    }
    return 1;
}

uint8_t DFRobot_Obloq_I2C::wifiConnect(const String &ssid, const String &pwd)
{
    DEBUGGER_LOG(this, ">=====> wifi connect");
    // CHECK_RUNNING_NODE(RUNNING_NODE_PING,
    //                    peek(String("Wifi connect ") + String(ssid) + String(" ") + String(pwd), 0),
    //                    peek(String("Please reinitialize"), -1);
    //                    return;)
    // if (!idle())
    //     return;
    WRITE_PARAMETER(COMMAND_WRITE_WIFI_NAME, ssid);
    WRITE_PARAMETER(COMMAND_WRITE_WIFI_PASSWORD, pwd);
    execute(COMMAND_EXECUTE_WIFI_CONNECT);
    uint64_t timeStart= millis();
    uint64_t timeEnd;
    uint8_t cmdbuffer[20];
     while (1)
    { timeEnd = millis();
        if((timeEnd - timeStart) > 30000){
            return 2;
        }
        if (read(cmdbuffer, 2, COMMAND_READTYPE_REG) != 2)
        {
            DEBUGGER_LOG(this, "read error");
        }else
        {
            if (cmdbuffer[0] == RETURN_COMMAND_WIFI && cmdbuffer[1] == 0x03){
                DEBUGGER_LOG(this, "connect wifi");
                read(cmdbuffer, 2, COMMAND_READTYPE_REG);
                read(cmdbuffer, cmdbuffer[1], COMMAND_READTYPE_REG);
                return 1;
            }
               
        }
        delay(200);
    }
    runningNode = RUNNING_NODE_WIFI_CONNECT;
    return 0;
}

uint8_t DFRobot_Obloq_I2C::mqttConnect(const String &iotId,
                                    const String &iotPwd,
                                    const String &server,
                                    uint32_t port)
{
    DEBUGGER_LOG(this, ">=====> mqtt connect");
    // CHECK_RUNNING_NODE(RUNNING_NODE_WIFI_CONNECT,
    //                    peek(String("MQTT connect ") + String(server) + String(" ") + String(port) + String(" ") + String(iotId) + String(" ") + String(iotPwd), 0),
    //                    peek(String("Please reinitialize"), -1);
    //                    return )
    // if (!idle())
    //     return;
    WRITE_PARAMETER(COMMAND_WRITE_MQTT_SERVER, server);
    WRITE_PARAMETER(COMMAND_WRITE_MQTT_PORT, String(port));
    WRITE_PARAMETER(COMMAND_WRITE_MQTT_ID, iotId);
    WRITE_PARAMETER(COMMAND_WRITE_MQTT_PWD, iotPwd);
    execute(COMMAND_EXECUTE_MQTT_CONNECT);
    uint64_t timeStart= millis();
    uint64_t timeEnd;
    uint8_t cmdbuffer[2];
     while (1)
    { timeEnd = millis();
        if((timeEnd - timeStart) > 30000){
            return 2;
        }
        if (read(cmdbuffer, 2, COMMAND_READTYPE_REG) != 2)
        {
            DEBUGGER_LOG(this, "read error");
        }else
        {
            if (cmdbuffer[0] == RETURN_COMMAND_MQTT && cmdbuffer[1] == 0x01){
                DEBUGGER_LOG(this, "mqtt connetc");
                return 1;
            }
               
        }
        delay(200);
    }
    runningNode = RUNNING_NODE_MQTT_CONNECT;
    return 1;
}

void DFRobot_Obloq_I2C::reset()
{
    execute(COMMAND_EXECUTE_RESEAT);
}

void DFRobot_Obloq_I2C::peek(String message, int8_t code)
{
    if (debugHandle)
        debugHandle(message, code);
    DEBUGGER_LOG(this, message);
}

void DFRobot_Obloq_I2C::timerInit(long time)
{
    timer = true;
    timerDuration = time;
    timerStart = millis();
}

bool DFRobot_Obloq_I2C::timerCount()
{
    if (timer)
    {
        if (millis() - timerStart >= timerDuration)
        {
            timer = false;
            return false;
        }
        return true;
    }
    return false;
}

bool DFRobot_Obloq_I2C::idle(long timeout)
{
    int count = 0;
    uint8_t cmdbuffer[2];
    timerInit(timeout);
    while (tickLock)
        delay(1);
    tickLock = true;
    while (1)
    {
        if (!timerCount())
        {
            peek(String("idle timeout"), -1);
            DEBUGGER_LOG(this, "idle timeout");
            tickLock = false;
            return false;
        }
        DEBUGGER_LOG(this, );
        if (read(cmdbuffer, 2, COMMAND_READTYPE_REG) != 2)
        {
            peek(String("idle error"), -1);
            DEBUGGER_LOG(this, "idle error");
            tickLock = false;
            return false;
        }
        {
            if (cmdbuffer[0] == RETURN_COMMAND_NONE && cmdbuffer[1] == RETURN_STATUS_NONE)
            // if (cmdbuffer[0] == 0x77 && cmdbuffer[1] == 0xFF)
                count++;
            else
                count = 0;
        }
        if (count >= 5)
        {
            tickLock = false;
            return true;
        }
    }
}

uint8_t DFRobot_Obloq_I2C::read(void *pBuf, size_t size, CommandReadTypeIndex type)
{
    delay(20);
    uint8_t result = readReg(COMMAND_WIRE_ADDRESS, type == COMMAND_READTYPE_REG ? COMMAND_WIRE_REGTSTER : COMMAND_WIRE_DATA, pBuf, size);
    if (DEBUGGER || debug)
    {
        String info = type == COMMAND_READTYPE_DATA ? "data:" : "reg:";
        for (int i = 0; i < size; ++i)
            info = info + String(" ") + String(((uint8_t *)pBuf)[i]);
        DEBUGGER_LOG(this, info);
    }
    return result;
}

void DFRobot_Obloq_I2C::write(CommandWriteIndex command, const String &data)
{
    uint8_t *buffer = (uint8_t *)malloc(sizeof(uint8_t) * (data.length() + 3));
    buffer[0] = COMMAND_TYPE_WRITE;
    buffer[1] = command;
    buffer[2] = data.length();
    memcpy(buffer + 3, data.c_str(), data.length());
    writeReg(COMMAND_WIRE_ADDRESS, COMMAND_WIRE_REGTSTER, buffer, data.length() + 3);
    free(buffer);
}

void DFRobot_Obloq_I2C::execute(CommandExecutEIndex command)
{
    uint8_t buffer[2] = {COMMAND_TYPE_EXECUTE, command};
    writeReg(COMMAND_WIRE_ADDRESS, COMMAND_WIRE_REGTSTER, buffer, 2);
    // delay(10);
}

void DFRobot_Obloq_I2C::execute(CommandExecutEIndex command, const String &data)
{
    uint8_t *buffer = (uint8_t *)malloc(sizeof(uint8_t) * (data.length() + 3));
    buffer[0] = COMMAND_TYPE_EXECUTE;
    buffer[1] = command;
    buffer[2] = data.length();
    memcpy(buffer + 3, data.c_str(), data.length());
    writeReg(COMMAND_WIRE_ADDRESS, COMMAND_WIRE_REGTSTER, buffer, data.length() + 3);
    free(buffer);
}

void DFRobot_Obloq_I2C::writeReg(uint8_t address, uint8_t reg, void *pBuf, size_t size)
{
    while (millis() - recordTime < wireInterval)
        delay(1);
    uint8_t *_pBuf = (uint8_t *)pBuf;
    Wire1.beginTransmission(address);
    Wire1.write(reg);
    if (size > 32)
    {
        uint16_t j = 1;
        for (uint8_t i = 0; i < size; i++)
        {
            if (i >= (31 * j))
            {
                Wire1.endTransmission(false);
                Wire1.beginTransmission(address);
                j++;
            }
            Wire1.write(_pBuf[i]);
        }
    }
    else
    {
        for (size_t i = 0; i < size; i++)
        {
            Wire1.write(_pBuf[i]);
        }
    }
    Wire1.endTransmission();
    recordTime = millis();
}

uint8_t DFRobot_Obloq_I2C::readReg(uint8_t address, uint8_t reg, void *pBuf, size_t size)
{
    if (pBuf == NULL)
    {
        return 0;
    }
    uint8_t *_pBuf = (uint8_t *)pBuf;
    Wire1.beginTransmission(address);
    Wire1.write(reg);
    if (reg == COMMAND_WIRE_REGTSTER)
    {
        Wire1.write(COMMAND_TYPE_READ);
        Wire1.write(COMMAND_READ);
    }
    if (Wire1.endTransmission() != 0)
    {
        return 0;
    }
    Wire1.requestFrom(address, size);
    for (size_t i = 0; i < size; i++)
    {
        _pBuf[i] = Wire1.read();
    }
    return size;
}


DFRobot_Obloq_I2C obloq(false);