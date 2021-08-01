/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2021-03-21
 */


/* Includes ------------------------------------------------------------------*/
#include "dwt_delay.h"
#include "stm32f4xx_hal.h"

//void delayUs(uint32_t i)
//{
//	uint32_t temp;
//	SysTick->LOAD=9*i;		 //对重载寄存器赋重装载值    1微秒计数9次
//	SysTick->CTRL=0X01;		 //开启systick功能,不开启中断，时钟源HCLK/8==9MHz  //当为0x05为KCLK时钟即72MHz
//	SysTick->VAL=0;		   	 //清零计数器
//	do
//	{
//		temp=SysTick->CTRL;		   //读取当前到计数值
//	}
//	while((temp&0x01)&&(!(temp&(1<<16))));	 //等Systick计数比较标志位置1
//	SysTick->CTRL=0;	//关闭计数器
//	SysTick->VAL=0;		//清空计数器
//}

//void delayMs(uint32_t i)
//{
//	uint32_t temp;
//	SysTick->LOAD=9000*i;	  
//	SysTick->CTRL=0X01;		
//	SysTick->VAL=0;			
//	do
//	{
//		temp=SysTick->CTRL;	 
//	}
// 	while((temp&0x01)&&(!(temp&(1<<16))));	
//	SysTick->CTRL=0;	
//	SysTick->VAL=0;		
//}




//CTRL 控制寄存器
//第0位：ENABLE，Systick 使能位 
//（0：关闭Systick功能；1：开启Systick功能）	
//第1位：TICKINT，Systick 中断使能位
//（0：关闭Systick中断；1：开启Systick中断）	
//第2位：CLKSOURCE，Systick时钟源选择
//（0：使用HCLK/8 作为Systick时钟；1：使用HCLK作为Systick时钟）	
//第16位：COUNTFLAG，Systick计数比较标志，如果在上次读取本寄存器后，SysTick 已经数到了0，则该位为1。如果读取该位，该位将自动清零



/*************************************************************************************************************************************/
  /*
    在Cortex-M里面有一个外设叫DWT(Data Watchpoint and Trace)，
    该外设有一个32位的寄存器叫CYCCNT，它是一个向上的计数器，
    记录的是内核时钟运行的个数，最长能记录的时间为：
    10.74s=2的32次方/400000000
    (假设内核频率为400M，内核跳一次的时间大概为1/400M=2.5ns)
    当CYCCNT溢出之后，会清0重新开始向上计数。
    使能CYCCNT计数的操作步骤：
    1、先使能DWT外设，这个由另外内核调试寄存器DEMCR的位24控制，写1使能
    2、使能CYCCNT寄存器之前，先清0
    3、使能CYCCNT寄存器，这个由DWT_CTRL(代码上宏定义为DWT_CR)的位0控制，写1使能
    */


#define  DWT_CR      *(uint32_t *)0xE0001000           //数据观察点和跟踪控制寄存器
#define  DWT_CYCCNT  *(volatile uint32_t *)0xE0001004  //DWT当前PC采样器周期计数寄存器
#define  DEM_CR      *(uint32_t *)0xE000EDFC           //调试异常和监控控制寄存器
#define  DBGMCU_CR   *(uint32_t *)0xE0042004           //MCU调试模块控制寄存器，详细内容参考《stm32中文参考手册》调试支持（DBG）章节，747页


#define  DEM_CR_TRCENA                   (1 << 24)      //DEM_CR调试异常和监控控制寄存器的Bit24，该位必须为1，使能调试和跟踪模块的使用

#define  DWT_CR_CYCCNTENA                (1 <<  0)      //置为DWT_CR控制寄存器的CYCCNTENA位，使能DWT_CYCCNT计数器

#define		CYCCNT_US		(168000000/1000000)


/**
  * @brief  初始化时间戳
  * @param  无
  * @retval 无
  * @note   使用延时函数前，必须调用本函数
  */
void DelayInit(void)
{  
	/* 使能DWT外设 */
    DEM_CR         |= (uint32_t)DEM_CR_TRCENA;         /* Enable Cortex-M3's DWT CYCCNT reg. 使能调试和跟踪模块的使用                  */
	/* DWT CYCCNT寄存器计数清0 */
    DWT_CYCCNT      = (uint32_t)0u;                    //初始化当前PC采样器周期计数器的计数值为0
	/* 使能Cortex-M DWT CYCCNT寄存器 */
    DWT_CR         |= (uint32_t)DWT_CR_CYCCNTENA;     //使能DWT_CYCCNT计数器。如果不使能，则计数器不执行计
                                                        //数操作，因此不会产生PC采样或CYCCNTENA事件。在正常
                                                        //使用时，CYCCNT计数器应由调试器初始化为0。
}

/**
  * @brief  读取当前时间戳
  * @param  无
  * @retval 当前时间戳，即DWT_CYCCNT寄存器的值
  */
uint32_t GetCycCnt(void)
{
    return (uint32_t)(DWT_CYCCNT);
}


void DelayU(uint32_t us)
{
	uint32_t tickstart = (uint32_t)(DWT_CYCCNT);
	uint32_t wait = us*CYCCNT_US;
	
	while((uint32_t)(DWT_CYCCNT) - tickstart < wait)
	{
	}
}

void DelayM(uint32_t ms)
{
	DelayU(ms *1000);
}

void DelayS(uint32_t s)
{
	DelayU(s*1000000);
}

void DelayCyc(uint32_t cyc)
{
    uint32_t cycdly = (uint32_t)(DWT_CYCCNT) + cyc;
    
	while((uint32_t)(DWT_CYCCNT) < cycdly);
}



/************************ (C) COPYRIGHT Mr.Lee *****END OF FILE****/

