/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2021-03-12
 */

#include "board.h"
#include "bsp.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "crc.h"
#include "dcmi.h"
#include "dma.h"
#include "rtc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"
#include "key.h"
#include "lcd_ssd1963.h"
#include "rs485.h"

#include "rtthread.h"

void SystemClock_Config(void);

static uint8_t heap_buf[10*1024];


void rt_hw_board_init()
{
	
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART3_UART_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_DCMI_Init();
  MX_FSMC_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_CRC_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
    BspInit();	
	
	rt_system_heap_init(heap_buf, heap_buf+sizeof(heap_buf)-1);
//    HAL_Init();
//    SystemClock_Config();

//    /* System Clock Update */
//    SystemCoreClockUpdate();

//    /* System Tick Configuration */
//    _SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);

//    /* Call components board initial (use INIT_BOARD_EXPORT()) */
//#ifdef RT_USING_COMPONENTS_INIT
//    rt_components_board_init();
//#endif

//#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
//    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
//#endif
}

void rt_hw_console_output(const char *str)
{
    /* empty console output */
	printf("%s", str);
}


void thread1_entry(void *parameter)
{
	while(1)
	{
		KeyStateMachine();
		if (g_keyComponent[MENU_KEY].keyEvent == KEY_RELEASE_EVENT)
		{
			g_keyComponent[MENU_KEY].keyEvent = KEY_NULL_EVENT;
			if (g_circleX < 560)
			{
				g_circleX += 20;
			}
			else if (g_circleX == 560)
			{
				g_circleX = 0;
			}
		}
		if (g_keyComponent[ENTER_KEY].keyEvent == KEY_RELEASE_EVENT)
		{
			g_keyComponent[ENTER_KEY].keyEvent = KEY_NULL_EVENT;
			if (g_circleY < 340)
			{
				g_circleY += 20;
			}
			else if (g_circleY == 340)
			{
				g_circleY = 0;
			}			
		}		
		if (g_keyComponent[EXIT_KEY].keyEvent == KEY_RELEASE_EVENT)
		{
			g_keyComponent[EXIT_KEY].keyEvent = KEY_NULL_EVENT;
			SendGetTempRequest();
		}		
		
		rt_thread_mdelay(1);
	}
}




