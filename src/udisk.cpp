#include "udisk.h"

Adafruit_FlashTransport_SPI flashTransport(17, &SPI);
Adafruit_SPIFlash flash(&flashTransport);
// file system object from SdFat
FatFileSystem fatfs;
//FatFile file;
// USB Mass Storage object
Adafruit_USBD_MSC usb_msc;
Adafruit_USBD_Device usb_device;

extern sGeneral_t *pGeneral;
// Check if flash is formatted
bool fs_formatted;
extern char *fileName;
uint8_t _st = 0;

void initUdisk(void)
{
  flash.begin();
  // Set disk vendor id, product id and revision with string up to 8, 16, 4 characters respectively
  usb_msc.setID("DFRobot", "External Flash", "1.0");
  // Set callback
  usb_msc.setReadWriteCallback(msc_read_cb, msc_write_cb, msc_flush_cb);
  usb_msc.setCapacity(flash.size()/512, 512);//512
  usb_msc.setUnitReady(true);
  usb_msc.begin();

  // Init file system on the flash
  fs_formatted = fatfs.begin(&flash);
  if ( !fs_formatted ){
    UDISK_DBG("Failed to init files system, flash may not be formatted");
  }
  FsDateTime::setCallback(dateTime);
}

int32_t msc_read_cb (uint32_t lba, void* buffer, uint32_t bufsize)
{
  if(_st == 0){
    _st = 1;
    pGeneral->sUsbState = ON;
  }
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

// call back for file timestamps
void dateTime(uint16_t* date, uint16_t* time) {   // hyy
  // DateTime now = RTC.now();
  sPCF8563TTime_t now = PCF8563T_1.getTime();

  // return date using FAT_DATE macro to format fields
  *date = FAT_DATE(now.year, now.month, now.day);

  // return time using FAT_TIME macro to format fields
  *time = FAT_TIME(now.hour, now.minute, now.second);
}
  uint8_t creatFile= 0;
void readUSBState(void)
{
  if((!usb_device.suspended()) && usb_device.attach()){//USB连接
    pGeneral->sFlashWrite = OFF;
    pGeneral->sUsbState = ON;
    creatFile = 0;
  }else{//断开连接
    pGeneral->sFlashWrite = ON;
    if( creatFile == 0){
      creatFile = 1;
      fileName = NULL;
    }
  }
  
}