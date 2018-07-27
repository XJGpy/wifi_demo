/*******************                    �� �� �� �� �� ˾ *********************/
/* �������ƣ�					                      	                      */
/* �ļ����ƣ�   					      		                                  */
/* ��    �ߣ�			 					                                  */
/* ������ڣ�								                                  */
/* ��ǰ�汾��						      		                              */
/* ���¼�¼: 								                                  */
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
*�������ƣ�void xEventInit(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�int xEventSend(char index,pEventCallback callback,char event,char state)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�int xEventSendBuff(char index,pEventCallback callback,const void *pInfor,uint16_t len)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�void xEventEnd(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�char xEventIsEmpty(char index)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
char xEventIsEmpty(char index)
{
  if(xEventQueue[index]==NULL)                              return pdTRUE;
  
  if(xEventMark &(1<<index))                                return pdFALSE;
  
  if(uxQueueMessagesWaiting(xEventQueue[index]) > 0x00)     return pdFALSE;
  
  return pdTRUE;
}


/*******************************************************************************
*�������ƣ�void xEvent(char index,uint32_t xTicksToWait)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
