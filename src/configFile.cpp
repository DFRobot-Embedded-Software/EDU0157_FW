#include "configFile.h"
#include "general.h"
#include "DFRobot_PCF8563T.h"
extern sGeneral_t *pGeneral;
extern FatFileSystem fatfs;
uint8_t annotationState = 0;
extern sPCF8563TTime_t rtt;
char *fileName = NULL;//存储文件名称
extern struct sDTUData *DTU_HEAD;
extern struct sDTUData* DTU_TAIL;

uint8_t dtuState = 0;

void creatorStandardConfigFileAndDTUFile(void){
  File32 myfile;

  if(!fatfs.exists("Config.txt")){//查看config文件是否存在
    CONFIG_DBG("no standard config file");
    myfile = fatfs.open("Config.txt",O_WRITE|O_CREAT|O_SYNC);
    if(!myfile){
       CONFIG_DBG("open  config file error");
      return;
    }
    CONFIG_DBG("open config file");
    //myfile.write("#\r\n");
    myfile.write("Communication:I2C\r\n");
    //myfile.write(" \r\n");

    //myfile.write("#\r\n");
    myfile.write("Sample_Rate:30 S\r\n");
    //myfile.write(" \r\n");

    //myfile.write("#\r\n");
    myfile.write("Record:ON\r\n");
    //myfile.write(" \r\n");

    //myfile.write("#\r\n");
    // myfile.write("Radial:23.75\r\n");
    // myfile.write(" \r\n");

    //myfile.write("#\r\n");
    myfile.write("Delay_Record:10\r\n");
    //myfile.write(" \r\n");

    //myfile.write("#\r\n");
    myfile.write("Light_Switch:OFF\r\n");
    //myfile.write(" \r\n");

    //myfile.write("#\r\n");
    myfile.write("Low_Power:OFF\r\n");
    //myfile.write(" \r\n");
    myfile.flush();
    myfile.close();
  }

  if(!fatfs.exists("DTU_config.txt")){
    CONFIG_DBG("no DTU_config file");
    myfile = fatfs.open("DTU_config.txt",O_WRITE|O_CREAT|O_SYNC);
    if(!myfile){
       CONFIG_DBG("open  DTU_config error");
      return;
    }
    myfile.write("#=====================\r\n");
    myfile.write("DTU_Switch:OFF\r\n");
    myfile.write("Method:WiFi\r\n");
    myfile.write(" \r\n");

    myfile.write("#----------------\r\n");
    myfile.write("Lora_ID: \r\n");
    myfile.write(" \r\n");

    myfile.write("#----------------\r\n");
    myfile.write("WiFi_SSID:\r\n");
    myfile.write("WiFi_PASSWORD:\r\n");
    myfile.write(" \r\n");

    myfile.write("#=====================================\r\n");
    myfile.write("Platform:Easyiot\r\n");
    myfile.write(" \r\n");
    myfile.write("#-------------\r\n");
    myfile.write("@Siot_v2\r\n");
    myfile.write("Server_IP:192.168.\r\n");
    myfile.write("Port:1883\r\n");
    myfile.write("Iot_ID:root\r\n");
    myfile.write("Iot_PWD:dfrobot\r\n");
    myfile.write("Save:1\r\n");
    myfile.write("Topic_Speed:siot/\r\n");
    myfile.write("Topic_Dir:siot/\r\n");
    myfile.write("Topic_Temp:siot/\r\n");
    myfile.write("Topic_Humi:siot/\r\n");
    myfile.write("Topic_Pressure:siot/\r\n");
    myfile.write("Topic_Battery:siot/\r\n");
    myfile.write(" \r\n");

    myfile.write("#-------------\r\n");
    myfile.write("@Siot_v1\r\n");
    myfile.write("Server_IP:192.168.\r\n");
    myfile.write("Port:1883\r\n");
    myfile.write("Iot_ID:root\r\n");
    myfile.write("Iot_PWD:dfrobot\r\n");
    myfile.write("Save:1\r\n");
    myfile.write("Topic_Speed:siot\r\n");
    myfile.write("Topic_Dir:\r\n");
    myfile.write("Topic_Temp:\r\n");
    myfile.write("Topic_Humi:\r\n");
    myfile.write("Topic_Pressure:\r\n");
    myfile.write("Topic_Battery:\r\n");
    myfile.write(" \r\n");

    myfile.write("#-------------\r\n");
    myfile.write("@Easyiot\r\n");
    myfile.write("Server_IP:192.168.\r\n");
    myfile.write("Port:1883\r\n");
    myfile.write("Iot_ID:root\r\n");
    myfile.write("Iot_PWD:dfrobot\r\n");
    myfile.write("Save:1\r\n");
    myfile.write("Topic_Speed:siot\r\n");
    myfile.write("Topic_Dir:\r\n");
    myfile.write("Topic_Temp:\r\n");
    myfile.write("Topic_Humi:\r\n");
    myfile.write("Topic_Pressure:\r\n");
    myfile.write("Topic_Battery:\r\n");
    myfile.write(" \r\n");
    myfile.flush();
    myfile.close();
  }
}

void readstandardConfigFile(void)
{
  File32 myfile;
  size_t fileLen = 0;
  char* buf;
  char* str;
  myfile = fatfs.open("Config.txt",FILE_READ);
  if(!myfile){
   CONFIG_DBG("open standard config file error");
    return;
  }
  CONFIG_DBG("open standard config file");
  fileLen = myfile.fileSize();
  CONFIG_DBG("fileLen:");
  CONFIG_DBG(fileLen);
  buf = (char*)malloc(fileLen*sizeof(char));
  myfile.read(buf,fileLen);
  myfile.close();
  for(uint8_t i = 0; i < fileLen;){//开始解析数据
    str = strtok(buf+i,"\r\n");
    i += strlen(str)+2;
    standardConfigParsing(str);
  }
  
  
  if(pGeneral->timeConfig == ON){//用配置文件配置完时间之后，将配置文件跟新
    myfile = fatfs.open("Config.txt",FILE_READ);
    if(!myfile){
      CONFIG_DBG("open standard config file error");
      return;
    }
    myfile.read(buf,fileLen);
    myfile.close();
    delay(500);
    if(!fatfs.remove("Config.txt")){
      CONFIG_DBG("remove file error!!!");
    }
    updataConfig(buf);
  }
  free(buf);
}

void standardConfigParsing(char *str)
{
  CONFIG_DBG(str);
  uint8_t state = 0;
  char* _str = strtok(str,":");
  if(_str[0] == 35){//检测到注释头
    return;
  }else if(strcmp(_str,"Communication") == 0){//模式选择
    _str = strtok(NULL,":");
    if(strCompare(_str,"i2c") == 1){
      pGeneral->sCommunicationMode = I2C;
      state = 1;
    }
    if(strCompare(_str,"uart") == 1){
      pGeneral->sCommunicationMode = UART;
      state = 1;
    }
  }else if(strcmp(_str,"Sample_Rate") == 0){
    uint16_t time=0,ret = 0;
    _str = strtok(NULL,":");
    time = atoi(_str);
    if(time > 255)
      time = 255;
    if(time < 1)
      time = 1;
    pGeneral->sSampleRate =  time;
    ret = parsingSampleRate(_str);
    if(ret == 0){
      pGeneral->sUnit = SECOND;
      CONFIG_DBG("SECOND");
      state = 1;
    }
    if(ret == 1){
      CONFIG_DBG("MINUTE");
      CONFIG_DBG(pGeneral->sSampleRate);
      pGeneral->sUnit = MINUTE;
      state = 1;
    }
    
  }else if(strcmp(_str,"Record") == 0){//存储配置
    _str = strtok(NULL,":");
    if(strCompare(_str,"on") == 1){
      pGeneral->sRecord = ON;
      state = 1;
    }
    if(strCompare(_str,"off") == 1){
      pGeneral->sRecord = OFF;
      state = 1;
    }
  }
  /*else if(strcmp(_str,"Radial") == 0){
    _str = strtok(NULL,":");
    pGeneral->sRadial = String(_str).toFloat();
    state = 1;
  }*/
  else if(strcmp(_str,"Delay_Record") == 0){
    uint16_t time;
    _str = strtok(NULL,":");
    time = atoi(_str);
    if(time < 10){
      time = 10;
    }else if(time > 60){
      time = 60;
    }
    pGeneral->sDelayRecord = time * 1000;
    state = 1;
  }else if(strcmp(_str,"Light_Switch") == 0){
    _str = strtok(NULL,":");
    if(strCompare(_str,"on") == 1){
      pGeneral->sLightSwitch = ON;
      state = 1;
    }
    if(strCompare(_str,"off") == 1){
      pGeneral->sLightSwitch = OFF;
      state = 1;
    }
  }else if(strcmp(_str,"Low_Power") == 0){
    _str = strtok(NULL,":");
    if(strCompare(_str,"on") == 1){
      pGeneral->sLowPower = ON;
      state = 1;
    }
    if(strCompare(_str,"off") == 1){
      pGeneral->sLowPower = OFF;
      state = 1;
    }
  }else if(strcmp(_str,"Time") == 0){//解析时间
    CONFIG_DBG("time");
    _str = strtok(NULL,"Time");
    if(timeRead(_str)){
      state = 1;
      pGeneral->timeConfig = ON;
    }else{
      state = 0;
    }
  }else if(_str[0] == 32){
    return;
  }else if(_str[0] == 0x0D){
    return;
  }else if(_str[0] == 0x0A){
    return;
  }else{
    String log = (String)str + " error";
    String location = "standardConfig";
    writeLog(location,log);
  }
  CONFIG_DBG(state);
  if(state == 0){
    String log = (String)str + " error";
    String location = "standardConfig";
    writeLog(location,log);
    pGeneral->configState = ON;
  }

}

uint8_t parsingSampleRate(char* str1)
{
  char* str;
  //if(str1[0] != 32){//检测到前没有空格
    if(strlen(str1) < 5){
      for(uint8_t i = 0; i < strlen(str1);i++){
        //if(str1[i] == 32){
          if((str1[i] == 115) || (str1[i + 1] == 115 -32))return 0;
          if((str1[i] == 109) || (str1[i + 1] == 109 -32))return 1;
        //}
      }
    }
  //}
  return 2;
}


uint8_t strCompare(char* str1, const char* str2)
{
  if(strlen(str1) == strlen(str2)){
    for(uint8_t i = 0;i < strlen(str2);i++){
      if((str1[i] == str2[i]) || (str1[i] == (str2[i] - 32))){
      }else{
        return 0;
      }
    }
    return 1;
  }
  return 0;
}

void creatorLogFile(void)
{
  File32 myfile;
  if(!fatfs.exists("Log.txt")){//查看config文件是否存在
    CONFIG_DBG("no log file");
    myfile = fatfs.open("Log.txt",FILE_WRITE);
    if(!myfile){
       CONFIG_DBG("open  log file error");
      return;
    }
    myfile.write("Log Start\r\n");
    myfile.close();
  }else{
    // myfile = fatfs.open("log.txt",FILE_WRITE);
    // if(!myfile){
    //    CONFIG_DBG("open  log file error");
    //   return;
    // }
    // myfile.write("LOG Start\r\n");
    // myfile.close();
  }
}

void writeLog(String location, String str)
{
  File32 myfile;
  String _str;
  _str += location;
  _str += ":";
  _str += str;
  _str += "\r\n";
  myfile = fatfs.open("Log.txt",FILE_WRITE);
    if(!myfile){
       CONFIG_DBG("open  Log  file error");
      return;
    }
    myfile.write(_str.c_str());
    myfile.close();
}


uint8_t timeRead(char* str)
{
  char buf[10]={0};
  uint8_t j = 0,k = 0;
  CONFIG_DBG(str);
  CONFIG_DBG(strlen(str));
  for(uint8_t i = 0;i < strlen(str);i++){ 
    if((str[i] == 0x2f) || (str[i] == 0x3A)){
      j = 0;
      switch(k){
        case 0:
          rtt.year = atoi(buf);
        break;
        case 1:
          rtt.month = atoi(buf);
        break;
        case 2:
          rtt.day = atoi(buf);
        break;
        case 3:
          rtt.hour = atoi(buf);
        break;
        default:
          rtt.minute = atoi(buf);
        break;
      }
      k++;
      CONFIG_DBG(buf);
      memset(buf,0,10);
      i++;
    }
    buf[j] = str[i];
    j++;

  }
  rtt.second = atoi(buf);
  rtt.week = 0;
  CONFIG_DBG(buf);
  return 1;
}

void updataConfig(char* str)
{
  uint16_t i;
  uint16_t len;
  File32 myfile;
  char* _buf = str;
  len  = strlen(_buf);
  CONFIG_DBG(len);
  for(i = 0;i < len;i++){
    if(_buf[i] == 0X54){//T
      if(_buf[i + 1] == 0x69){//i
        if(_buf[i + 2] == 0x6d){//m
          if(_buf[i + 3] == 0x65){//e
            if(_buf[i + 4] == 0x3a){//:
                break;
            }
          }
        }
      }
    }
  }
  CONFIG_DBG(i);
  myfile = fatfs.open("test.txt",O_WRITE|O_CREAT|O_SYNC);
    if(!myfile){
       CONFIG_DBG("open  config file error");
      return;
    }
    myfile.write(_buf,i);
    myfile.close();
    delay(500);
    if(!fatfs.rename("test.txt","Config.txt")){
      CONFIG_DBG("change file name error!!!");
    }    
    
}

void csvRecord(void){
  File32 myfile;
  if(pGeneral->saveStateTime == ON){
    if(pGeneral->sRecord == ON){//需要存储到CSV文件中
      if(pGeneral->sFlashWrite == ON){
        RGBStart(0);
        if(fileName == NULL){
          CONFIG_DBG("fileName NULL");
          String text = "Time,Wind Speed(m/s),Wind Direction,";//默认文件内容
          fileName = getFileName();
          CONFIG_DBG(fileName);
          //noInterrupts();
          if(!fatfs.exists(fileName)){//查看想要创建的文件是否已经存在。如果不存在就创建
            String data = dataSplitting();
            myfile = fatfs.open(fileName,FILE_WRITE);
            text += dataName();
            text += "\r\n";
            myfile.seek(myfile.position());
            myfile.write(text.c_str(),text.length());
            myfile.write(data.c_str(),data.length());
            myfile.flush();
            myfile.sync();
            myfile.close();
            CONFIG_DBG("creat CSV file ok");
          }
          //interrupts();
        }else{
          String data = dataSplitting();
          myfile = fatfs.open(fileName, FILE_WRITE);
          myfile.seek(myfile.position());
          myfile.write(data.c_str(),data.length());
          myfile.flush();
          myfile.sync();
          myfile.close();
          //interrupts();
          CONFIG_DBG("write data ok");
          CONFIG_DBG(data);
        }
        RGBEnd(0);
       }
    }
  }
  
}
extern float wSpeed;
extern String timeData;
extern struct I2CSensorInfo *I2C_SENSOR_INFO_HEAD;
extern String widDir;
String dataSplitting(void){
struct I2CSensorInfo *p = I2C_SENSOR_INFO_HEAD;
  String str = "";
  str += timeData;
  str += ",";
  str += String(wSpeed,1);
  str += ",";
  str += widDir;
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

void DTUParsing(void)
{
  File32 myfile;
  size_t fileLen = 0;
  char* buf;
  char* str;
  myfile = fatfs.open("DTU_config.txt",FILE_READ);
  if(!myfile){
   CONFIG_DBG("open DTU config file error");
    return;
  }
  CONFIG_DBG("open DTU config file");
  fileLen = myfile.fileSize();
  CONFIG_DBG("fileLen:");
  CONFIG_DBG(fileLen);
  buf = (char*)malloc(fileLen*sizeof(char));
  myfile.read(buf,fileLen);
  myfile.close();
  for(uint16_t i = 0; i < fileLen;){//开始解析数据
    str = strtok(buf+i,"\r\n");
    i += strlen(str)+2;
    standardDTUParsing(str);
  }
}
void standardDTUParsing(char* str){
  CONFIG_DBG(str);
  uint8_t state = 0;
  
  char* _str = strtok(str,":");
  if(_str[0] == 35){//检测到注释头
    if(dtuState == 1){
      dtuState = 0;
      state = 1;
    }else{
      return;
    }
  }else if(_str[0] == 32){
    return;
  }else if(_str[0] == 0x0D){
    return;
  }else if(_str[0] == 0x0A){
    return;
  }else if(strcmp(_str,"DTU_Switch") == 0){
    _str = strtok(NULL,":");
    if(strCompare(_str,"on") == 1){
      pGeneral->sDTUSwitch = ON;
      state = 1;
    }
    if(strCompare(_str,"off") == 1){
      pGeneral->sDTUSwitch = OFF;
      state = 1;
    }
  }else if(strcmp(_str,"Method") == 0){
    _str = strtok(NULL,":");
    if(strcmp(_str,"WiFi") == 0){
      pGeneral->sDTUMethod = ON;
      state = 1;
    }
  }else if(strcmp(_str,"Lora_ID") == 0){
    _str = strtok(NULL,":");
    CONFIG_DBG(strlen(_str));
    if(strlen(_str) == 0){
      pGeneral->sDTULora_ID = random(1,255);
    }else{
      pGeneral->sDTULora_ID = atoi(_str);
    }
    state = 1;
  }else if(strcmp(_str,"WiFi_SSID") == 0){
    _str = strtok(NULL,":");
    pGeneral->sDTUWIFISSID = _str;
    state = 1;
  }else if(strcmp(_str,"WiFi_PASSWORD") == 0){
    _str = strtok(NULL,":");
    pGeneral->sDTUWIFIPASSWORD = _str;
    state = 1;
  }else if(strcmp(_str,"Platform") == 0){
    _str = strtok(NULL,":");
    if(strcmp(_str,"Easyiot") == 0){
      pGeneral->sDTUPlatform = EASYIOT;
    }
    if(strcmp(_str,"Siot_v2") == 0){
      pGeneral->sDTUPlatform = SIOT_V2;
    }
    if(strcmp(_str,"Siot_v1") == 0){
      pGeneral->sDTUPlatform = SIOT_V1;
    }
    state = 1;
  }else{
    if(dtuState == 1){
      if(strcmp(_str,"Server_IP") == 0){
        _str = strtok(NULL,":");
        pGeneral->sDTUIP = _str;
      }else if(strcmp(_str,"Port") == 0){
        _str = strtok(NULL,":");
        pGeneral->sDTUPort = atoi(_str);
      }else if(strcmp(_str,"Iot_ID") == 0){
        _str = strtok(NULL,":");
        pGeneral->sDTUID = _str;
      }else if(strcmp(_str,"Iot_PWD") == 0){
        _str = strtok(NULL,":");
        pGeneral->sDTUPWD = _str;
      }else if(strcmp(_str,"Save") == 0){
        _str = strtok(NULL,":");
        pGeneral->sDTUSave = _str;
      }else{
        addTopic(_str,strtok(NULL,":"));
      }
    }else{
      if(pGeneral->sDTUPlatform == EASYIOT){
        if(strcmp(_str,"@Easyiot") == 0){
          dtuState = 1;
        }
      }else if(pGeneral->sDTUPlatform == SIOT_V1){
        if(strcmp(_str,"@Siot_v1") == 0){
          dtuState = 1;
        }
      }else if(pGeneral->sDTUPlatform == SIOT_V2){
        if(strcmp(_str,"@Siot_v2") == 0){
          dtuState = 1;
        }
      }
      
    }
    state = 1;
  }
  CONFIG_DBG(state);
  if(state == 0){
    String log = (String)str + " error";
    String location = "standardConfig";
    writeLog(location,log);
    pGeneral->configState = ON;
  }
}

void addTopic(char* str,char* str1){
  struct sDTUData *p = new sDTUData;
  p->key = getKey(str);
  p->topic = str1;
  p->next = NULL;
  if(DTU_HEAD == NULL){
    DTU_TAIL = p;
    DTU_HEAD = p;
  }else{
    DTU_TAIL->next = p;
    DTU_TAIL = p;
  }
}

char* getKey(char* str)
{
  strtok(str,"_");
  return strtok(NULL,"_");
}