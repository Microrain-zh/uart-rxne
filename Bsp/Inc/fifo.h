/*
 * Copyright (c) Mr.Lee 2021 All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2021-02-19
 */

#ifndef __FIFO_H
#define __FIFO_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*LockFun)(void);

typedef struct
{
    uint8_t *buf; // 缓冲区
    uint32_t bufSize; // 缓冲区大小
    uint32_t occupySize; // 占用大小
    uint8_t *pWrite; // 写指针
    uint8_t *pRead; // 读指针
    void (*lock)(void); // 上锁
    void (*unlock)(void); // 解锁	
}FifoType;

void FifoInit(FifoType *pFifo, uint8_t *pFifoBuf, uint32_t size, LockFun lock, LockFun unLock);
void FifoRelease(FifoType *pFifo);
uint32_t FifoWrite(FifoType *pFifo, const uint8_t *pBuf, uint32_t size);
uint32_t FifoRead(FifoType *pFifo, uint8_t *pBuf, uint32_t size);
uint32_t FifoGetTotalSize(FifoType *pFifo);
uint32_t FifoGetFreeSize(FifoType *pFifo);
uint32_t FifoGetOccupySize(FifoType *pFifo);


#ifdef __cplusplus
}
#endif

#endif /* __FIFO_H */

/************************ (C) COPYRIGHT Mr. Lee *****END OF FILE****/
