/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2021-03-12
 */

#include "fifo.h"
#include <stdlib.h>
#include "uart_device.h"

//Fifo初始化
void FifoInit(FifoType *pFifo, uint8_t *pFifoBuf, uint32_t size, LockFun lock, LockFun unLock)
{
    pFifo->buf = pFifoBuf;
    pFifo->bufSize = size;
    pFifo->occupySize = 0;
    pFifo->pWrite = pFifo->buf;
    pFifo->pRead = pFifo->buf;
	  pFifo->lock = lock;
	  pFifo->unlock = unLock;
}

//Fifo释放
void FifoRelease(FifoType *pFifo)
{
    pFifo->buf = NULL;
    pFifo->bufSize = 0;
    pFifo->occupySize = 0;
    pFifo->pWrite = NULL;
    pFifo->pRead = NULL;
	  pFifo->lock = NULL;
	  pFifo->unlock = NULL;	
}

/*
 * 功能：向Fifo写入数据
 * 参数：pFifo 指向Fifo的指针
 *       pBuf 指向待写入数据的指针
 *       size 写入数量
 * 返回值：写入的数量
 */
uint32_t FifoWrite(FifoType *pFifo, const uint8_t *pBuf, uint32_t size)
{
    uint32_t wSize = 0;
	  uint32_t freeSize = 0;
	
	  if ((size==0) || (pFifo==NULL) || (pBuf==NULL))
		{
		    return 0;
		}
		freeSize = FifoGetFreeSize(pFifo);
		if (freeSize == 0)
		{
		    return 0;	
		}		
		if (freeSize < size)
		{
		    size = freeSize;
		}
		wSize = size;
		if (pFifo->lock != NULL)
		{
		    pFifo->lock();
		}
		while (wSize-- > 0)
		{
		    *pFifo->pWrite++ = *pBuf++; // 写入写指针指向的buf
        if (pFifo->pWrite >= (pFifo->buf + pFifo->bufSize))
				{
            pFifo->pWrite = pFifo->buf; // 如果写到buf尾，从头开始写
				}
				pFifo->occupySize++;
		}
		if (pFifo->unlock != NULL)
		{
		    pFifo->unlock();
		}
		
		return size;
}

/*
 * 功能：读取Fifo的数据
 * 参数：pFifo 指向Fifo的指针
 *       pBuf 指向要读取的缓冲区
 *       size 读取数量
 * 返回值：读取的数量
 */
uint32_t FifoRead(FifoType *pFifo, uint8_t *pBuf, uint32_t size)
{
    uint32_t rSize = 0;
    uint32_t occupySize = 0;

	  if ((size==0) || (pFifo==NULL) || (pBuf==NULL))
		{
			return 0;
		}
    occupySize = FifoGetOccupySize(pFifo);
		if (occupySize == 0)
		{
			return 0;
		}
    if (occupySize < size)
		{
        size = occupySize;
		}
		rSize = size;
		if (pFifo->lock != NULL)
		{
		    pFifo->lock();
		}
    while (rSize-- > 0)
		{
        *pBuf++ = *pFifo->pRead++;
			  if (pFifo->pRead >= (pFifo->buf + pFifo->bufSize))
				{
				    pFifo->pRead = pFifo->buf; // 读到尾从头开始读
				}
				pFifo->occupySize--;
		}
		
		if (pFifo->unlock != NULL)
		{
		    pFifo->unlock();
		}
//		DEBUG("%08x\r\n",size);
		return size;		
}

// 获取Fifo总大小
uint32_t FifoGetTotalSize(FifoType *pFifo)
{
    if (pFifo == NULL)
		{
        return 0;
		}
		return pFifo->bufSize;
}

// 获取Fifo中空闲缓冲的大小
uint32_t FifoGetFreeSize(FifoType *pFifo)
{
    if (pFifo == NULL)
		{
        return 0;
		}
		return (pFifo->bufSize - FifoGetOccupySize(pFifo));		
}

// 获取已使用buf大小
uint32_t FifoGetOccupySize(FifoType *pFifo)
{
    if (pFifo == NULL)
		{
        return 0;
		}   
    return pFifo->occupySize;		
}


/************************ (C) COPYRIGHT Mr.Lee *****END OF FILE****/
