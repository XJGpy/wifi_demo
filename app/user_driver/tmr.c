/*******************                    科 技 有 限 公 司 ***********************/
/* 工程名称：					                      						   */
/* 文件名称：   					      										   */
/* 作    者：			 					      							   */
/* 完成日期：								      							   */
/* 当前版本：						      									   */
/* 更新记录: 								      							   */
/********************************************************************************/
#include "../include/user_config.h"
#include "tmr.h"

/*******************************************************************************
*函数名称：void tmr_init(tmr_t *tmr,const tmr_ticks_t delay_in_ticks,pTickFunc pTick)
*函数说明：
*输入参数：
*输出参数：
*调用说明：
*******************************************************************************/
void tmr_init(tmr_t *tmr,const tmr_ticks_t delay_in_ticks,pTickFunc pTick)
{
	if(pTick == NULL)
    	return;
    
	tmr->pTick = pTick;

    // Save delay in case timer is restarted
    tmr->delay_in_ticks = delay_in_ticks;

    // Store start tick
    tmr->start_tick = tmr->pTick();

    // Set state to indicate that timer has started
    tmr->state = TMR_STOPPED;
}

/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*调用说明：
*******************************************************************************/
void tmr_start(tmr_t *tmr)
{
    if(tmr->pTick == NULL)
    	return;
    // Store start tick
    tmr->start_tick = tmr->pTick();

    // Set state to indicate that timer has started
    tmr->state = TMR_STARTED;
}

/*******************************************************************************
*函数名称：void tmr_prioed(tmr_t *tmr,const tmr_ticks_t delay_in_ticks)
*函数说明：
*输入参数：
*输出参数：
*调用说明：
*******************************************************************************/
void tmr_prioed(tmr_t *tmr,const tmr_ticks_t delay_in_ticks)
{
    if(tmr->pTick == NULL)
    	return;
	
	//tine is stop or expired
	if(tmr->state != TMR_STARTED)
	{
		tmr->delay_in_ticks = delay_in_ticks;
	}
	else
	{		//time is running
		tmr_ticks_t elapsed_count = tmr_ticks_elapsed(tmr);
		tmr->delay_in_ticks = delay_in_ticks;
		tmr->start_tick = tmr->pTick();
		tmr->start_tick += elapsed_count;
	}
	
}

/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*调用说明：
*******************************************************************************/
bool_t tmr_has_expired(tmr_t* tmr)
{
    tmr_ticks_t tick;
	tmr_ticks_t  diff;
	
	
	if(tmr->pTick == NULL)
		return TRUE;

    // See if timer has been stopped
    if (tmr->state == TMR_STOPPED)
    	return FALSE;

    // See if timer has already expired
    if (tmr->state == TMR_EXPIRED)
    	return TRUE;

    // Fetch current time
    tick = tmr->pTick();
	
	//
	if(tick >= tmr->start_tick)
		diff = tick - tmr->start_tick;
	else
		diff = __UINT32_T_MAX__ - tmr->start_tick + tick;
	  
    // Timer expire test
    if( diff < tmr->delay_in_ticks )
    {
    	return FALSE;
    }

    // Set state to indicate that timer has expired
    tmr->state = TMR_EXPIRED;

    return TRUE;
}

/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*调用说明：
*******************************************************************************/
void tmr_stop(tmr_t *tmr)
{
    // Stop timer
    tmr->state = TMR_STOPPED;
}

/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*调用说明：
*******************************************************************************/
void tmr_restart(tmr_t *tmr)
{
   if(tmr->pTick == NULL)
	   return;
   
   if(tmr->delay_in_ticks == 0x00)
	   return;
    // Store start tick
    tmr->start_tick = tmr->pTick();

    // Set state to indicate that timer has started
    tmr->state = TMR_STARTED;
}

/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*调用说明：
*******************************************************************************/
void tmr_reset(tmr_t *tmr)
{
    if(tmr->pTick == NULL)		 return;
	
    // Calculate and store new start tick
    tmr->start_tick += tmr->delay_in_ticks;

    // Set state to indicate that timer has started
    tmr->state = TMR_STARTED;
}

/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*调用说明：
*******************************************************************************/
void tmr_wait(const tmr_ticks_t delay_in_ticks,pTickFunc pTick)
{
    tmr_t tmr;
	
	if(pTick == NULL)			return;
	
	tmr.pTick = pTick;
	tmr.delay_in_ticks = delay_in_ticks;

    tmr_start(&tmr);

    while (!tmr_has_expired(&tmr)){
#ifdef WATCHDOG      
        watchDogFeed(false);
#endif
    }
}
/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*调用说明：
*******************************************************************************/
tmr_ticks_t tmr_ticks_elapsed(tmr_t *tmr)
{
  tmr_ticks_t diff;
  // Fetch current time
  tmr_ticks_t tick = tmr->pTick();
  //
  if(tick >= tmr->start_tick)
	diff = tick - tmr->start_tick;
  else
	diff = __UINT32_T_MAX__ - tmr->start_tick + tick;
  
  return diff;
}


/*******************************************************************************
*函数名称：char tmr_timeout(tmr_ticks_t sysTick,tmr_ticks_t timeout,pTickFunc pTick)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
char tmr_timeout(tmr_ticks_t sysTick,tmr_ticks_t timeout,pTickFunc pTick)
{
  uint32_t curTick =0x00;
  uint32_t diff =0x00;
  
  curTick = pTick();
  
  if(curTick < sysTick) { 
	diff = __UINT32_T_MAX__ - sysTick + curTick;
  }
  else{
	diff = curTick - sysTick;
  }
  
  return (diff >= timeout)?TRUE:FALSE;
  
}
				   
				   
/*******************************************************************************
*函数名称：void tmr_renew(tmr_ticks_t *pTickCount,pTickFunc pTick)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void tmr_renew(tmr_ticks_t *pTickCount,pTickFunc pTick)
{
  *pTickCount = pTick(); 
}


