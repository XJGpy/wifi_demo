/*
 * timebase.h
 *
 *  Created on: 2018年4月14日
 *      Author: huang
 */

#ifndef APP_USER_DRIVER_TIMEBASE_H_
#define APP_USER_DRIVER_TIMEBASE_H_

#include "time.h"

#define TICK_BASE				10
#define TIME_BASE				100
#define RTC_BASE				1000   // xuejungong 20180727 add

/*******************************************************************************
 *函数名称：void ICACHE_FLASH_ATTR sysTimeInit(void)
 *函数说明：
 *输入参数：
 *输出参数：
 *返 回 值：
 *其他说明：
 *******************************************************************************/
extern void ICACHE_FLASH_ATTR sysTimeInit(void);


/*******************************************************************************
 *函数名称：uint32_t getTick(void)
 *函数说明：获取时基计数
 *输入参数：无
 *输出参数：返回当前时基计数
 *调用说明：
 *******************************************************************************/
uint32_t  getTick(void);


/*******************************************************************************
*函数名称：void tickRenewTime(uint32_t *pSysTick)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
extern void tickRenewTime(uint32_t *pSysTick);


/*******************************************************************************
 *函数名称：uint32_t getRTCTick(void)
 *函数说明：获取时基计数
 *输入参数：无
 *输出参数：返回当前时基计数
 *调用说明：
 *******************************************************************************/
extern uint32_t  getRTCTick(void);

/*******************************************************************************
*函数名称：uint32_t  dateTimeToSeconds(uint16_t syear,uint8_t smon,uint8_t sday,
									  uint8_t hour,uint8_t min,uint8_t sec)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
extern uint32_t dateTimeToSeconds(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec);



/*******************************************************************************
*函数名称：void getDatetime(struct tm *pDateTime)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
extern void getDatetime(struct tm *pDateTime);

/*******************************************************************************
*函数名称：int setDatetime(uint16_t syear,uint8_t smon,uint8_t sday,
					     uint8_t hour,uint8_t min,uint8_t sec)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
extern int setDatetime(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec);





#endif /* APP_USER_DRIVER_TIMEBASE_H_ */
