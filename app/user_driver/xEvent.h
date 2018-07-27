/*******************                    �� �� �� �� �� ˾ *********************/
/* �������ƣ�					                      	                      */
/* �ļ����ƣ�   					      		                              */
/* ��    �ߣ�			 					                                  */
/* ������ڣ�								                                  */
/* ��ǰ�汾��						      		                              */
/* ���¼�¼: 								                                  */
/******************************************************************************/
#ifndef __XEVENT_H__
#define __XEVENT_H__

#include "user_config.h"


#ifdef __cplusplus
extern "C"
{
#endif 
  
  typedef enum{
    EVENT_LOW,
    EVENT_HIGH,
    MAX_EVENT
  }xEventNumTypeDef;
  
  typedef enum{
    EVENT_COM,
    EVENT_BUFF
  }xEventTypeDef;


  typedef union{
    
    struct{
      char            index;
      char            event;
    }event;
    
    struct{
      char            *pTemp;
      uint16_t         len;
    }buff;
    
  }xEventParam_t;

typedef void (*pEventCallback)(xEventParam_t *);

typedef struct{
    xEventTypeDef            type;
    xEventParam_t            param;  //
    pEventCallback           callback;
}xEvent_t;  
    


/*******************************************************************************
*�������ƣ�void xEventInit(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void xEventInit(void);


/*******************************************************************************
*�������ƣ�int xEventSend(char index,pEventCallback callback,char event,char state)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern int xEventSend(char index,pEventCallback callback,char event,char state);


/*******************************************************************************
*�������ƣ�int xEventSendBuff(char index,pEventCallback callback,const void *pInfor,uint16_t len)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern int xEventSendBuff(char index,pEventCallback callback,const void *pInfor,uint16_t len);


/*******************************************************************************
*�������ƣ�void xEventEnd(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void xEventEnd(void);


/*******************************************************************************
*�������ƣ�char xEventIsEmpty(char index)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern char xEventIsEmpty(char index);


/*******************************************************************************
*�������ƣ�void xEvent(char index,uint32_t xTicksToWait)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void xEvent(char index,uint32_t xTicksToWait);


#ifdef __cplusplus
}
#endif

#endif

