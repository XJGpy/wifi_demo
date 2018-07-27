/*******************                    科 技 有 限 公 司 ************************/
/* 工程名称：																	*/
/* 文件名称：																	*/
/* 作    者：			 					      								*/
/* 完成日期：								      								*/
/* 当前版本：						      		      							*/
/* 更新记录: 								      								*/
/********************************************************************************/
#include "../include/user_config.h"
#include "tmr.h"
#include "ringbuf.h"
#include "ringEvent.h"


//#define SINGLE_RING

static ringEvent_t ringEvent[MAX_RINGEVENT_COUNT];
static char       ringWriteIndex = 0x00;

/*******************************************************************************
*函数名称：void ringEventInit(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void ringEventInit(void)
{
  ringEvent_t *pRingEvent = NULL;
  
  uint16_t size = sizeof(ringEvent_t);
  
  size *= MAX_RINGEVENT_COUNT;
  
  pRingEvent = &ringEvent[0];
  
  memset(pRingEvent,0x00,size);
  
}

/*******************************************************************************
*函数名称：void ringEventLoop(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void ringEventLoop(void)
{
	char index = 0x00;
	char i = 0x00;
	int  retValue = 0x00;
	ringEvent_t *pRingEvent = NULL;
  
	index = ringWriteIndex;
  
	for(i = 0x00;i < MAX_RINGEVENT_COUNT;i++)
	{
		index++;
    
		if(index >= MAX_RINGEVENT_COUNT)
		{
			index = 0x00;
		}
    
		pRingEvent = &ringEvent[index];
    
		switch(pRingEvent->mark.bitFlag.status)
		{
			case RING_IDLE:
				if(pRingEvent->mark.bitFlag.count > 0x00)
				{
					pRingEvent->mark.bitFlag.steup = 0x00;

					if(pRingEvent->delay > 0x00)
					{
						tmr_prioed(&pRingEvent->tmr,pRingEvent->delay);
						tmr_start(&pRingEvent->tmr);
						pRingEvent->mark.bitFlag.status = RING_DELAY;
					}
					else
					{
						pRingEvent->mark.bitFlag.status = RING_START;
					}
				}
			break;
      
			case RING_DELAY:
				if(tmr_has_expired(&pRingEvent->tmr))
				{
					pRingEvent->mark.bitFlag.status = RING_START;
				}
      
			break;
      
			case RING_START:
				if(pRingEvent->done!= NULL)
				{
					//事件执行超时机制
					if(pRingEvent->mark.bitFlag.steup == 0x00)
					{
						APP_TRACE_DBG("\n============================%d ringEvent Done start!\r\n",index);

						if(pRingEvent->timeout > 0x00)
						{
							tmr_prioed(&pRingEvent->tmr,pRingEvent->timeout);
						}
						else
						{
							tmr_prioed(&pRingEvent->tmr,0xFFFFFF);
						}

						tmr_restart(&pRingEvent->tmr);
					}
        
					retValue = pRingEvent->done(pRingEvent->pArg,pRingEvent->len);

					if(retValue < 0x00)
					{
						if(pRingEvent->result != NULL)
						{
							pRingEvent->result(RINGEVENT_RESULT_DONE_FAIL);
						}

						ringEventEndIndex(index);
						break;
					}
					else  //ready Event
					{
						if(tmr_has_expired(&pRingEvent->tmr))
						{
							pRingEvent->tmr.state = TMR_STOPPED;

							if(pRingEvent->result != NULL)
							{
								pRingEvent->result(RINGEVENT_RESULT_TIMEOUT);
							}
							//APP_TRACE_DBG("\n============================%d ringEvent Done Timeout!\r\n",index);
							ringEventEndIndex(index);
							break;
						}
					}
				}
				else
				{
					retValue = 0x00;
				}
				//next steup
				if(retValue == 0x00)
				{
					pRingEvent->mark.bitFlag.steup = 0x00;
					pRingEvent->mark.bitFlag.count--;
	
					if(pRingEvent->wait)   //time exe
					{
						tmr_prioed(&pRingEvent->tmr,pRingEvent->wait);
						tmr_restart(&pRingEvent->tmr);
						pRingEvent->mark.bitFlag.status = RING_WAIT;
					}
					else   //no time exe
					{
						//timeout Event
						if(pRingEvent->mark.bitFlag.count==0x00)
						{
							if(pRingEvent->result != NULL)
							{
								pRingEvent->result(RINGEVENT_RESULT_OK);
							}
							ringEventEndIndex(index);
							break;
						}
					}
				}
			break;
      
			case RING_WAIT:
				if(tmr_has_expired(&pRingEvent->tmr))
				{
					pRingEvent->mark.bitFlag.status = RING_IDLE;
					pRingEvent->tmr.state = TMR_STOPPED;
	
					//timeout Event
					if(pRingEvent->mark.bitFlag.count==0x00)
					{
						if(pRingEvent->result != NULL)
						{
							pRingEvent->result(RINGEVENT_RESULT_RESP);
						}
						ringEventEndIndex(index);
					}
				}
			break;
      
			default:
			break;
		}
    
#ifdef SINGLE_RING
    if(pRingEvent->mark.bitFlag.status == RING_IDLE)  continue;
    else                                              break;
#endif

	}
}

/*******************************************************************************
*函数名称：int ringEventWrite(pDone		    done,
			    pResult        result,
			    void		    *pArg,
			    uint16_t       len,
			    uint8_t 	    count,
			    tmr_ticks_t	delay,
			    pTickFunc	    pTick)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
int ringEventWrite(pDone		done,
		   pResult		result,
		   const void		*pArg,
		   uint16_t     	len,
           tmr_ticks_t      timeout,
		   tmr_ticks_t		delay,
		   uint8_t 		    count,
		   tmr_ticks_t		wait,
		   pTickFunc		pTick)
{
	char index = 0x00;
	char i =0x00;
	ringEvent_t *pRingEvent = NULL;
  
	if(done == NULL)
		return -1;
  
	for(index=0x00;index<MAX_RINGEVENT_COUNT;index++)
	{
		pRingEvent = &ringEvent[index];

		if(pRingEvent->mark.bitByte != 0x00)
			break;
	}
  
	if(index >= MAX_RINGEVENT_COUNT)
		index = 0x00;
  
	//查找有没有空间
	for(i = 0x00;i < MAX_RINGEVENT_COUNT;i++)
	{
		if(index >= MAX_RINGEVENT_COUNT)
		{
			index = 0x00;
		}
    
		pRingEvent = &ringEvent[index];

		if(pRingEvent->mark.bitByte == 0x00)
			break;
    
		index++;
	}

	if(i >= MAX_RINGEVENT_COUNT)
		return -2;
  
	pRingEvent->len = 0x00;
  
	if(pRingEvent->pArg != NULL)
	{
		memFree(pRingEvent->pArg);
		pRingEvent->pArg = NULL;
	}
  
	if((pArg != NULL)&&(len > 0x00))
	{
		pRingEvent->pArg = memMalloc(len);

		if(pRingEvent->pArg==NULL)
			return -3;
    
		memset(pRingEvent->pArg,0x00,len);
		memcpy(pRingEvent->pArg,pArg,len);
		pRingEvent->len = len;
	}
  
	pRingEvent->done = done;
	pRingEvent->result = result;
	pRingEvent->timeout = timeout;
	pRingEvent->delay = delay;
	pRingEvent->wait = wait;
	pRingEvent->mark.bitFlag.count = count;
	pRingEvent->mark.bitFlag.status = RING_IDLE;
  
	tmr_init(&pRingEvent->tmr,delay,pTick);
	ringWriteIndex = index;
	APP_TRACE_DBG("\r\n============================RingEvent Write Index %d!\r\n",index);
  
	return  index;
}


/*******************************************************************************
*函数名称：ringEvent_t *ringEventGetInfor(char index)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
ringEvent_t *ringEventGetInfor(char index)
{
  ringEvent_t *pRingEvent = NULL;
  
  if(index >= MAX_RINGEVENT_COUNT)				return NULL;
  
  pRingEvent = &ringEvent[index];
  
  return pRingEvent;
  
}


/*******************************************************************************
*函数名称：char ringEventIsEmpty(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
char ringEventIsEmpty(void)
{
	char index = 0x00;
	ringEvent_t *pRingEvent = NULL;
  
	for(index = 0x00;index < MAX_RINGEVENT_COUNT;index++)
	{
		pRingEvent = &ringEvent[index];

		if(pRingEvent->mark.bitByte != 0x00)
			break;
	}
  
	return index >= MAX_RINGEVENT_COUNT?SET:RESET;
}



/*******************************************************************************
*函数名称：void ringEventEndIndex(char index)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void ringEventEndIndex(char index)
{
  ringEvent_t *pRingEvent = NULL;
  
  pRingEvent = &ringEvent[index];
  
  if(pRingEvent->mark.bitByte == 0x00)      return;
 
  if(pRingEvent->pArg != NULL){
    memFree(pRingEvent->pArg);
    pRingEvent->pArg = NULL;
  }
  
  APP_TRACE_DBG("\r\n======================================%d RingEvent End!\r\n",index);
  
  memset(pRingEvent,0x00,sizeof(ringEvent_t));
  
  
  

}

/*******************************************************************************
*函数名称：void ringEvnetEnd(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void ringEvnetEnd(void)
{
  char index  = 0x00;
  for(index = 0x00;index < MAX_RINGEVENT_COUNT;index++){
    ringEventEndIndex(index);
  }
}




