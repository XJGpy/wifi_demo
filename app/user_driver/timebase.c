/*
 * timebase.c
 *
 *  Created on: 2018��4��14��
 *      Author: huang
 */

#include "../include/user_config.h"
#include "time.h"

LOCAL os_timer_t sysTick;
LOCAL os_timer_t sysTime;
LOCAL os_timer_t rtcTick;

static uint32_t sysTickCount;
static uint32_t sysTimeCount;
static uint32_t	rtcTickCount;

#include "tmr.h"
extern void xTmrCallback(pTickFunc pTick);

/*******************************************************************************
 *�������ƣ�uint32_t getTick(void)
 *����˵������ȡʱ������
 *�����������
 *������������ص�ǰʱ������
 *����˵����
 *******************************************************************************/
uint32_t  getTick(void)
{
	uint32_t counter;
	counter = sysTickCount;
	return counter;
}


/*******************************************************************************
*�������ƣ�char tickTimeout(uint32_t sysTick,uint32_t timeout)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
char tickTimeout(uint32_t sysTick,uint32_t timeout)
{
	uint32_t curTick =0x00;
	uint32_t diff =0x00;

	curTick = getTick();

	if(curTick < sysTick)
	{
		diff = 0xFFFFFFFF - sysTick + curTick;
	}
	else
	{
		diff = curTick - sysTick;
	}

	return (diff >= timeout/TICK_BASE)?SET:RESET;

}


/*******************************************************************************
*�������ƣ�void tickRenewTime(uint32_t *pSysTick)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
void tickRenewTime(uint32_t *pSysTick)
{
	*pSysTick = getTick();
}

/*******************************************************************************
 *�������ƣ�uint32_t getTimeTick(void)
 *����˵������ȡʱ������
 *�����������
 *������������ص�ǰʱ������
 *����˵����
 *******************************************************************************/
uint32_t  getTimeTick(void) {
	uint32_t counter;

	counter = sysTimeCount;

	return counter;
}


/*******************************************************************************
 *�������ƣ�uint32_t getRTCTick(void)
 *����˵������ȡʱ������
 *�����������
 *������������ص�ǰʱ������
 *����˵����
 *******************************************************************************/
uint32_t  getRTCTick(void)
{
	uint32_t counter;

	counter = rtcTickCount;

	return counter;
}

/*******************************************************************************
 *�������ƣ�void ICACHE_FLASH_ATTR tick_callback(void)
 *����˵����
 *���������
 *���������
 *�� �� ֵ��
 *����˵����
 *******************************************************************************/
void ICACHE_FLASH_ATTR tick_callback(void)
{
	sysTickCount++;
	xLedTime();
	xTrigerTime();
	xTmrCallback(getTick);
	ringPayTickEvent();
}


/*******************************************************************************
 *�������ƣ�void ICACHE_FLASH_ATTR time_callback(void)
 *����˵����
 *���������
 *���������
 *�� �� ֵ��
 *����˵����
 *******************************************************************************/
void ICACHE_FLASH_ATTR time_callback(void)
{
	sysTimeCount++;
	xTmrCallback(getTimeTick);
}

/*******************************************************************************
 *�������ƣ�
 *����˵����void ICACHE_FLASH_ATTR rtc_callback(void)
 *���������
 *���������
 *�� �� ֵ��
 *����˵����
 *******************************************************************************/
void ICACHE_FLASH_ATTR rtc_callback(void)
{
	rtcTickCount++;
	xTmrCallback(getRTCTick);
	upgradeEvent();
}
/*******************************************************************************
 *�������ƣ�void ICACHE_FLASH_ATTR sysTimeInit(void)
 *����˵����
 *���������
 *���������
 *�� �� ֵ��
 *����˵����
 *******************************************************************************/
void ICACHE_FLASH_ATTR sysTimeInit(void)
{
	//10ms
	os_timer_disarm(&sysTick);
	os_timer_setfn(&sysTick, (os_timer_func_t *)tick_callback , NULL);
	os_timer_arm(&sysTick, TICK_BASE, 1);

	//100ms
	os_timer_disarm(&sysTime);
	os_timer_setfn(&sysTime, (os_timer_func_t *)time_callback , NULL);
	os_timer_arm(&sysTime, TIME_BASE, 1);

	//1000ms
	os_timer_disarm(&rtcTick);
	os_timer_setfn(&rtcTick, (os_timer_func_t *)rtc_callback , NULL);
	os_timer_arm(&rtcTick, RTC_BASE, 1);
}

/*******************************************************************************
*�������ƣ�int setDatetime(uint16_t syear,uint8_t smon,uint8_t sday,
					     uint8_t hour,uint8_t min,uint8_t sec)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
int setDatetime(uint16_t syear,uint8_t smon,uint8_t sday,
                uint8_t hour,uint8_t min,uint8_t sec)

{

  uint32_t seccount =0x00;

  os_timer_disarm(&rtcTick);

  seccount = dateTimeToSeconds(syear,smon,sday,hour,min,sec);

  rtcTickCount = seccount;

  os_timer_arm(&rtcTick, 1000, 1);

  return 0x00;
}

/*******************************************************************************
*�������ƣ�void getDatetime(struct tm *pDateTime)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
void getDatetime(struct tm *pDateTime)
{
	struct tm tm;
    uint32_t seccount =rtcTickCount;

    gmtime_r((const time_t *)&seccount,&tm);

    memcpy(pDateTime,&tm,sizeof(struct tm));
}

/*******************************************************************************
*�������ƣ�uint32_t  dateTimeToSeconds(uint16_t syear,uint8_t smon,uint8_t sday,
									  uint8_t hour,uint8_t min,uint8_t sec)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
uint32_t dateTimeToSeconds(uint16_t syear,uint8_t smon,uint8_t sday,
						   uint8_t hour,uint8_t min,uint8_t sec)
{
  struct tm tx;

  if(syear < 2000||syear > 2099)     return 0x00;//syear��Χ2000-2099���˴����÷�ΧΪ2000-2099

  tx.tm_sec = sec;
  tx.tm_min = min;
  tx.tm_hour = hour;
  tx.tm_mday = sday;

  if((smon >= 1) && (smon <= 12)){
	tx.tm_mon = smon - 1;
  }
  else {
	return 0x00;
  }

  tx.tm_year = syear - 1900;

  tx.tm_isdst = 0;

  return mktime(&tx);
}


/*******************************************************************************
*�������ƣ�int8_t secondsToDateTime(uint32_t seconds,struct tm *pDateTime)
*����˵��������תΪ��Ӧ����ʱ��
*���������seconds������  pdateTime������ʱ���ַָ��
*�����������
*�� �� ֵ����
*����˵����// tm_wday 0 - 6 (0 = sunday) // tm_mon 0 - 11 (0 = Jan) // tm_year = current year - 1900
*******************************************************************************/
int8_t secondsToDateTime(uint32_t seconds,struct tm *pDateTime)
{
  if(pDateTime == NULL) return -1;

#if defined ( __ICCARM__ )
  struct tm *gt;
  gt = gmtime((const time_t *)&seconds);
  memcpy(pDateTime,gt,sizeof(struct tm));
#elif defined (__CC_ARM)
  struct tm tm;
  gmtime_r((const time_t *)&sec,&tm);
  memcpy(pDateTime,&tm,sizeof(struct tm));
#endif

  return 0;
}





