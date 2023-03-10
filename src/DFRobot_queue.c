#include "DFRobot_queue.h"
#include "stdio.h"

struct sQueueData *cumsgBufHead=NULL;
struct sQueueData *cumsgBufTail=NULL;

uint32_t queuedata = 0;

void cuappEnqueue(uint8_t ok, uint8_t down, uint8_t back, uint8_t down_num,  uint8_t last_page){
    struct sQueueData *p;
    p = (struct sQueueData*)malloc(sizeof(struct sQueueData));
    if(p == NULL){
        return;
    }
    p->next   = NULL;
    p->before = NULL;
    if(cumsgBufHead==NULL){
        cumsgBufHead=p;
        cumsgBufTail=p;
    }else{
        cumsgBufTail->next = p;
        p->before = cumsgBufTail;
        cumsgBufTail = p;
    }
    p->ok = ok;
    p->down = down;
    p->back = back;
    p->downNum = down_num;
    p->isLastPage = last_page;
    queuedata += 1;
}

struct sQueueData *cuappQueueTail( void ){
    struct sQueueData *p;
    p = cumsgBufTail;
    if(cumsgBufTail != NULL){
        cumsgBufTail = p->before;
    }
    return p;
}

struct sQueueData *getQueueTail(void){
    struct sQueueData *p;
    p = cumsgBufTail;
    return p;
}

struct sQueueData *getQueueHead(void){
    struct sQueueData *p;
    p = cumsgBufHead;
    return p;
}


pCmdPacktet_t i2cCumsgBufHead=NULL;
//pCmdPacktet_t i2cCumsgBufTail=NULL;

//uint32_t i2cqueuedata = 0;

pCmdPacktet_t i2cCuappEnqueue(uint16_t len){
    pCmdPacktet_t p;
    p = (pCmdPacktet_t)malloc(sizeof(sCmdPacktet_t) + len);
    if(i2cCumsgBufHead) free(i2cCumsgBufHead);
    i2cCumsgBufHead = p;
    //p->next = NULL;
    //p->before = NULL;
    //if(i2cCumsgBufHead==NULL){
    //    i2cCumsgBufHead=p;
    //    i2cCumsgBufTail=p;
    //}else{
    //    i2cCumsgBufTail->next = p;
    //    p->before = i2cCumsgBufTail;
    //    i2cCumsgBufTail = p;
    //}
    p->flag = 0;
    p->len = len;
    p->index = 0;
    return p;
}
/*
pCmdPacktet_t i2cCuappDequeue( void ){
    pCmdPacktet_t p;
    p = i2cCumsgBufHead;
    if(i2cCumsgBufHead != NULL){
        i2cCumsgBufHead = p->next;
    }
    return p;
}
pCmdPacktet_t i2cCuappQueueTail(void){
    pCmdPacktet_t p;
    p = i2cCumsgBufTail;
    if(i2cCumsgBufTail){
        i2cCumsgBufTail = i2cCumsgBufTail->before;
    }
    return p;
}
pCmdPacktet_t i2cGetQueueHead(void){
    pCmdPacktet_t p;
    p = i2cCumsgBufHead;
    return p;
}
*/

pSendPacktet_t i2cSendBufHead=NULL;
//pSendPacktet_t i2cSendBufTail=NULL;

//uint32_t i2csendqueue = 0;
pSendPacktet_t i2cSendCuappEnqueue(uint16_t len){
    pSendPacktet_t p;
    p = (pSendPacktet_t)malloc(sizeof(sSendPacktet_t) + len);
    //p->next = NULL;
    if(i2cSendBufHead) free(i2cSendBufHead);
    i2cSendBufHead = p;
    //if(i2cSendBufHead==NULL){
    //    i2cSendBufHead=p;
    //    i2cSendBufTail=p;
    //}else{
    //    i2cSendBufTail->next = p;
    //    i2cSendBufTail = p;
    //}
    p->flag = 0;
    p->total = len;
    p->index = 0;
    return p;
}
/*
pSendPacktet_t i2cSendCuappDequeue( void ){
    pSendPacktet_t p;
    p = i2cSendBufHead;
    if(i2cSendBufHead != NULL){
        i2cSendBufHead = p->next;
    }
    return p;
}
pSendPacktet_t i2cSendCuappQueueTail( void ){
    pSendPacktet_t p;
    p = i2cSendBufTail;
    return p;
}
*/
pSendPacktet_t i2cSendGetQueueHead(void){
    pSendPacktet_t p;
    p = i2cSendBufHead;
    return p;
}