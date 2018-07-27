/*******************                    科 技 有 限 公 司 *********************/
/* 工程名称：					                      	                      */
/* 文件名称：   					      		                                  */
/* 作    者：			 					                                  */
/* 完成日期：								                                  */
/* 当前版本：						      		                              */
/* 更新记录: 								                                  */
/******************************************************************************/
#include "../include/user_config.h"
#include "xEvent.h"

//#define ENABLE_ISR

static xQueueHandle    xEventQueue[MAX_EVENT];
static char             xEventMark = 0x00;
const char xEventMaxTable[MAX_EVENT]={10,10};

#if defined ENABLE_ISR
extern unsigned portBASE_TYPE uxCriticalNesting;
#endif

/*******************************************************************************
*函数名称：void xEventInit(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void xEventInit(void)
{
	char index = 0x00;
  
	for(index =0x00;index<MAX_EVENT;index++)
	{
		xEventQueue[index]=xQueueCreate(xEventMaxTable[index], sizeof(xEvent_t));

		if(xEventQueue == NULL)
		{
			break;
		}
	}
}


/*******************************************************************************
*函数名称：int xEventSend(char index,pEventCallback callback,char event,char state)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
int xEventSend(char index,pEventCallback callback,char event,char state)
{
	portBASE_TYPE taskWoken = pdFALSE;
	portBASE_TYPE xReturn = pdTRUE;
	xEvent_t eventTemp;
	xEvent_t *pEvent =&eventTemp;
  
	if(index >= MAX_EVENT)
		return -1;
  
	if(xEventQueue[index]==NULL)
		return -2;
  
	pEvent->type = EVENT_COM;
	pEvent->param.event.index = event;
	pEvent->param.event.event = state;
	pEvent->callback = callback;

#if defined ENABLE_ISR
  if(uxCriticalNesting){
    xReturn=xQueueSendFromISR(xEventQueue[index], pEvent, &taskWoken);
    portEND_SWITCHING_ISR(taskWoken);
  }
  else{
#endif

    xReturn=xQueueSend(xEventQueue[index], pEvent, MAX_BLOCK_TIME);

#if defined ENABLE_ISR
  }
#endif
  
  	  if(xReturn == pdFALSE)
  	  {
  		  return -3;
  	  }
  
  	  xEventMark |= (1<<index);
  
  	  return 0x00;
}

/*******************************************************************************
*函数名称：int xEventSendBuff(char index,pEventCallback callback,const void *pInfor,uint16_t len)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
int xEventSendBuff(char index,pEventCallback callback,const void *pInfor,uint16_t len)
{
  portBASE_TYPE taskWoken = pdFALSE;
  portBASE_TYPE xReturn = pdTRUE;
  xEvent_t eventTemp;
  xEvent_t *pEvent =&eventTemp;
  char    *pTemp = NULL;
  
  if(index >= MAX_EVENT)         return -1;
  
  if(xEventQueue[index]==NULL)         return -2;
  
  pEvent->type = EVENT_BUFF;
  
  pTemp = memMalloc(len);
  if(pTemp == NULL)                    return -3;
  memcpy(pTemp,pInfor,len);
  pEvent->param.buff.pTemp = pTemp;
  pEvent->param.buff.len = len;
  
  pEvent->callback = callback;
  

#if defined ENABLE_ISR
  if(uxCriticalNesting){
    xReturn=xQueueSendFromISR(xEventQueue[index], pEvent, &taskWoken);
    portEND_SWITCHING_ISR(taskWoken);
  }
  else{
#endif
    xReturn=xQueueSend(xEventQueue[index], pEvent, MAX_BLOCK_TIME);
#if defined ENABLE_ISR
  }
#endif
  
  if(xReturn == pdFALSE){
    memFree(pEvent->param.buff.pTemp);
	return -4;
  }
  
  xEventMark |= (1<<index);
  
  return 0x00;
}



/*******************************************************************************
*函数名称：void xEventEnd(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void xEventEnd(void)
{
  xEvent_t eventTemp;
  xEvent_t *pEvent=&eventTemp;
  char index = 0x00;
  
  for(index =0x00;index<MAX_EVENT;index++){
    
    if(xEventQueue[index] == NULL)         continue;
    
    while(xQueueReceive(xEventQueue[index], pEvent, 0x00) != pdFAIL){
      
      if(pEvent->type == EVENT_BUFF)
        memFree(pEvent->param.buff.pTemp);
    }
    
    xEventMark &= ~(1<<index);
  }
}


/*******************************************************************************
*函数名称：char xEventIsEmpty(char index)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
char xEventIsEmpty(char index)
{
  if(xEventQueue[index]==NULL)                              return pdTRUE;
  
  if(xEventMark &(1<<index))                                return pdFALSE;
  
  if(uxQueueMessagesWaiting(xEventQueue[index]) > 0x00)     return pdFALSE;
  
  return pdTRUE;
}


/*******************************************************************************
*函数名称：void xEvent(char index,uint32_t xTicksToWait)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void xEvent(char index,uint32_t xTicksToWait)
{
	xEvent_t eventTemp;
	xEvent_t *pEvent = &eventTemp;
  
	if(index >= MAX_EVENT)
		return;
  
	if(xEventQueue[index] == NULL)
		return;
  
	if(xQueueReceive(xEventQueue[index], pEvent, xTicksToWait))
	{
		pEvent->callback(&pEvent->param);
    
		if(pEvent->type == EVENT_BUFF)
			memFree(pEvent->param.buff.pTemp);
    
		if(uxQueueMessagesWaiting(xEventQueue[index]) == 0x00)
		{
			xEventMark &= ~(1<<index);
		}
	}
}
