/*
 * Copyright (c) Mr.Lee 2021 All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2021-02-19
 */
#include "stdint.h"
#include "stdio.h" 
#ifndef __UART_DEVICE_H
#define __UART_DEVICE_H


#define __DEDUG__

#ifdef __DEDUG__
#define DEBUG(format, ...) printf("[File:%s, Line:%d] "format, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define DEBUG(format, ...)
#endif


#ifdef __cplusplus
extern "C" {
#endif
 
void Uart1DeviceInit(void); 
uint16_t Uart1Read(const uint8_t *buf, uint16_t size); 
 
void Uart1DmaRxCpltIsr(void); // 完成中断 
void Uart1DmaRxHalfCpltIsr(void); // 半完成中断 
void Uart1DmaIdleIsr(void);



/* 1. 设置打印级别 */
enum {
        LOG_LEVEL_NONE,
        LOG_LEVEL_DEBUG,
        LOG_LEVEL_INFO,
        LOG_LEVEL_WARN,
        LOG_LEVEL_ERROR,
        LOG_LEVEL_FATAL,
};

/* 2. log 打印 重写 */
void log_fun(int level, const char *opt, const char* tag, int line, const char *func, const char *fmt, ...);

/* 3. 各打印级别宏 */
/*
 *@LOG_DBG
 */ 
#define LOG_DBG(tag, fmt, ...) \
  log_fun(LOG_LEVEL_DEBUG, "D", tag , __LINE__, __func__, fmt, ##__VA_ARGS__)

/*
 *@LOG_INFO
 */ 
#define LOG_INFO(tag, fmt, ...)  \
  log_fun(LOG_LEVEL_INFO, "I", tag , __LINE__, __func__, fmt, ##__VA_ARGS__)

/*
 *@LOG_WARN
 */ 
#define LOG_WARN(tag, fmt, ...)  \
  log_fun(LOG_LEVEL_WARN, "W", tag , __LINE__, __func__, fmt, ##__VA_ARGS__)

/*
 *@LOG_ERR
 */ 
#define LOG_ERR(tag, fmt, ...)  \
  log_fun(LOG_LEVEL_ERROR, "E", tag , __LINE__, __func__, fmt, ##__VA_ARGS__)

/*
 *@LOG_FATAL
 */ 
#define LOG_FATAL(tag, fmt, ...)  \
  log_fun(LOG_LEVEL_FATAL, "F", tag , __LINE__, __func__, fmt, ##__VA_ARGS__)




 
#ifdef __cplusplus
}
#endif

#endif /* __UART_DEVICE_H */

 /************************ (C) COPYRIGHT Mr.Lee *****END OF FILE****/

