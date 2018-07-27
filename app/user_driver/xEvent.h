/*******************                    科 技 有 限 公 司 *********************/
/* 工程名称：					                      	                      */
/* 文件名称：   					      		                              */
/* 作    者：			 					                                  */
/* 完成日期：								                                  */
/* 当前版本：						      		                              */
/* 更新记录: 								                                  */
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
*函数名称：void xEventInit(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void xEventInit(void);


/*******************************************************************************
*函数名称：int xEventSend(char index,pEventCallback callback,char event,char state)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern int xEventSend(char index,pEventCallback callback,char event,char state);


/*******************************************************************************
*函数名称：int xEventSendBuff(char index,pEventCallback callback,const void *pInfor,uint16_t len)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern int xEventSendBuff(char index,pEventCallback callback,const void *pInfor,uint16_t len);


/*******************************************************************************
*函数名称：void xEventEnd(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void xEventEnd(void);


/*******************************************************************************
*函数名称：char xEventIsEmpty(char index)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern char xEventIsEmpty(char index);


/*******************************************************************************
*函数名称：void xEvent(char index,uint32_t xTicksToWait)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void xEvent(char index,uint32_t xTicksToWait);


#ifdef __cplusplus
}
#endif

#endif

