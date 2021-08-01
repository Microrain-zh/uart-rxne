/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2021-03-12
 */
  
#include "uart_device.h"
#include "fifo.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include <stdarg.h>
#include <string.h>

extern DMA_HandleTypeDef hdma_usart3_rx;


/* 串口DMA数据结构 */
typedef struct
{
	FifoType rxFifo; /* 接收Fifo */
	uint8_t *dmaRxBuf; /* dma接收缓存 */
	uint16_t dmaRxBufSize; /* dma接收缓存大小 */
	uint16_t lastDmaRxSize; /* dma上一次接收数据大小 */
}UartDmaType;
	
/* 接收数据缓冲区 */
#define UART1_RX_BUF_SIZE    		 2048
/* DMA接收Buf */
#define UART1_DMA_RX_BUF_SIZE    256


static uint8_t g_uart3RxBuf[UART1_RX_BUF_SIZE];
uint8_t g_uart3DmaRxBuf[UART1_DMA_RX_BUF_SIZE];

#define RX_BUFF_SIZE 1
uint8_t g_rxBuffer[RX_BUFF_SIZE] = {0};

static UartDmaType g_uart1Device;


static void FifoLock(void);
static void FifoUnlock(void);
static uint16_t Uart1DmaGetRemainSize(void);


/* Uart接收初始化 */
void Uart1DeviceInit(void)
{
//	GPIO_InitTypeDef GPIO_InitStruct = {0};

//	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_RESET);
//	GPIO_InitStruct.Pin = GPIO_PIN_8;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_PULLUP;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);	 	
	
	FifoInit(&g_uart1Device.rxFifo, g_uart3RxBuf, UART1_RX_BUF_SIZE, FifoLock, FifoUnlock);
	g_uart1Device.dmaRxBuf = g_uart3DmaRxBuf;
	g_uart1Device.dmaRxBufSize = UART1_DMA_RX_BUF_SIZE;
	g_uart1Device.lastDmaRxSize = 0;


//	/* HAL库初始化时会清空DMA完成/半完成/错误登门中断，所以此处不必清除*/
//	__HAL_UART_ENABLE_IT(&huart3,UART_IT_IDLE);
//	__HAL_DMA_ENABLE_IT(&hdma_usart3_rx, DMA_IT_TC);
//	__HAL_DMA_ENABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
//	/* HAL_UART_Receive_DMA完成/错误中断等开启*/
//	HAL_UART_Receive_DMA(&huart3, g_uart3DmaRxBuf, UART1_DMA_RX_BUF_SIZE);
	/* 开启接收中断 */
	HAL_UART_Receive_IT(&huart3, (uint8_t *)g_rxBuffer, RX_BUFF_SIZE);
}

// 上锁
static void FifoLock(void)
{
//    __disable_irq();
}

// 解锁
static void FifoUnlock(void)
{
//	__enable_fiq();
}

/* DMA接收完成中断 */
void Uart1DmaRxCpltIsr(void)
{
	uint16_t recvSize;

	recvSize = g_uart1Device.dmaRxBufSize - g_uart1Device.lastDmaRxSize; // 接收数据量 = 总缓存-上一次接收数据大小
	DEBUG("%u\r\n",recvSize);
	FifoWrite(&g_uart1Device.rxFifo, &g_uart1Device.dmaRxBuf[g_uart1Device.lastDmaRxSize], recvSize); // 写入Fifo
	g_uart1Device.lastDmaRxSize = 0; 
}

/* DMA接收半传输中断 */
void Uart1DmaRxHalfCpltIsr(void)
{
	uint16_t recvTotalSize;
	uint16_t recvSize;

	recvTotalSize = g_uart1Device.dmaRxBufSize - Uart1DmaGetRemainSize();
	recvSize = recvTotalSize - g_uart1Device.lastDmaRxSize;
	DEBUG("%u\r\n",recvSize);
	FifoWrite(&g_uart1Device.rxFifo, &g_uart1Device.dmaRxBuf[g_uart1Device.lastDmaRxSize], recvSize);
	g_uart1Device.lastDmaRxSize = recvTotalSize;	
}

/* 空闲中断 */
void Uart1DmaIdleIsr(void)
{
    uint16_t recvTotalSize;
    uint16_t recvSize;

//	uint16_t temp  = hdma_usart3_rx.Instance->NDTR;  
//	DEBUG("%u\r\n",temp);	
	recvTotalSize = g_uart1Device.dmaRxBufSize - Uart1DmaGetRemainSize();
//	DEBUG("%u,%u\r\n",g_uart1Device.dmaRxBufSize , Uart1DmaGetRemainSize());
	recvSize = recvTotalSize - g_uart1Device.lastDmaRxSize;
//	DEBUG("%u\r\n",recvSize);
//	DEBUG("%u,%02x,%02x\r\n",recvSize,g_uart1Device.dmaRxBuf[g_uart1Device.lastDmaRxSize],
//	g_uart1Device.dmaRxBuf[g_uart1Device.lastDmaRxSize+1]);
	FifoWrite(&g_uart1Device.rxFifo, &g_uart1Device.dmaRxBuf[g_uart1Device.lastDmaRxSize], recvSize);	
	g_uart1Device.lastDmaRxSize = recvTotalSize;
//	DEBUG("%04x\r\n",recvTotalSize);
}

/* 接收中断 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART3)//如果是串口1
	{
		FifoWrite(&g_uart1Device.rxFifo, g_rxBuffer, 1); // 写入Fifo
		g_uart1Device.lastDmaRxSize = 0; 		
	}
	
}




// 获取dma接收Buf剩余大小
static uint16_t Uart1DmaGetRemainSize(void)
{
    return  __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);
}

// 串口读数据
uint16_t Uart1Read(const uint8_t *buf, uint16_t size)
{
    return FifoRead(&g_uart1Device.rxFifo, (uint8_t *)buf, size);	
}


//void DMA1_Stream5_IRQHandler(void)
//{
//    if (__HAL_DMA_GET_FLAG(&hdma_usart2_rx, DMA_FLAG_TCIF1_5) != RESET) // 传输完成
//		{
//			  __HAL_DMA_CLEAR_FLAG(&hdma_usart2_rx, DMA_FLAG_TCIF1_5);
//        Uart1DmaRxCpltIsr();
//		}
//    if (__HAL_DMA_GET_FLAG(&hdma_usart2_rx, DMA_FLAG_HTIF1_5) != RESET) // 半传输 
//		{
//			  __HAL_DMA_CLEAR_FLAG(&hdma_usart2_rx, DMA_FLAG_HTIF1_5);
//        Uart1DmaRxHalfCpltIsr();
//		}		
////  HAL_DMA_IRQHandler(&hdma_usart2_rx);
//}

//void USART2_IRQHandler(void)
//{
//    if (__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE) != RESET)
//	  {
//        __HAL_UART_CLEAR_IDLEFLAG(&huart2);
//			
//		    Uart1DmaIdleIsr();
//	  }
////  HAL_UART_IRQHandler(&huart2);
//}

int g_current_dbg_level = LOG_LEVEL_DEBUG;

void log_fun(int level, const char *opt, const char* tag, int line, const char *func, const char *fmt, ...)
{
    if (level > g_current_dbg_level) {
            char msg_buf[20*1024];
            va_list ap; 
            va_start(ap,fmt);                                                                       
            sprintf(msg_buf,"%s/%s (%d): %s() ",opt, tag, line, func);
            vsprintf(msg_buf+strlen(msg_buf),fmt,ap);
            fprintf(stderr,"%s\n",msg_buf); /* 输出到标准输出 */
            va_end(ap);
    }
}

/* 设置 打印级别 */
void ModuleDebugInit(int level)
{
    g_current_dbg_level = level;
}

/************************ (C) COPYRIGHT Mr.Lee *****END OF FILE****/
