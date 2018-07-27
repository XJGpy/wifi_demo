/*******************                    科 技 有 限 公 司 *********************/
/* 工程名称：					                      	                      */
/* 文件名称：   					      		                              */
/* 作    者：			 					                                  */
/* 完成日期：								                                  */
/* 当前版本：						      		                              */
/* 更新记录: 								                                  */
/******************************************************************************/
#include "../include/user_config.h"
#include "tmr.h"
#include "xTmr.h"


static tmr_t            xTmr[MAX_TMR];

/*******************************************************************************
*函数名称：void xTmrInit(xTmrTypeDef index,const tmr_ticks_t delay_in_ticks,pTickFunc pTick)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void xTmrInit(xTmrTypeDef index,const tmr_ticks_t delay_in_ticks,pTickFunc pTick)
{
  tmr_t *pTmr = NULL;
  
  if(index >= MAX_TMR)
	  return;
  
  pTmr = &xTmr[index];
  
  tmr_init(pTmr,delay_in_ticks,pTick);
}

/*******************************************************************************
*函数名称：void xTmrRestart(xTmrTypeDef index)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void xTmrRestart(xTmrTypeDef index)
{
  tmr_t *pTmr = NULL;
  if(index >= MAX_TMR)          return;
  
  pTmr = &xTmr[index];
  tmr_restart(pTmr);
}

/*******************************************************************************
*函数名称：void xTmrStop(xTmrTypeDef index)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void xTmrStop(xTmrTypeDef index)
{
  tmr_t *pTmr = NULL;
  if(index >= MAX_TMR)          return;
  
  pTmr = &xTmr[index];
  tmr_stop(pTmr);
}

/*******************************************************************************
*函数名称：void xTmrPrioed(xTmrTypeDef index,const tmr_ticks_t delay_in_ticks)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void xTmrPrioed(xTmrTypeDef index,const tmr_ticks_t delay_in_ticks)
{
  tmr_t *pTmr = NULL;
  if(index >= MAX_TMR)          return;
  
  pTmr = &xTmr[index];
  tmr_prioed(pTmr,delay_in_ticks);
}


/*******************************************************************************
*函数名称：char xTmrExpired(xTmrTypeDef index)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
char xTmrExpired(xTmrTypeDef index)
{
	tmr_t *pTmr = NULL;

	if(index >= MAX_TMR)
		return SET;
  
	pTmr = &xTmr[index];
	return tmr_has_expired(pTmr);
}

/*******************************************************************************
*函数名称：tmr_t *sysTmrGetInfor(xTmrTypeDef index)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
tmr_t *xTmrGetInfor(xTmrTypeDef index)
{
	tmr_t *pTmr = NULL;

	if(index >= MAX_TMR)
		return NULL;
  
	pTmr = &xTmr[index];
	return pTmr;
}


/*******************************************************************************
*函数名称：char xTmrIsEnd(xTmrTypeDef index)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
char xTmrIsEnd(xTmrTypeDef index)
{
  tmr_t *pTmr = NULL;
  
  pTmr = xTmrGetInfor(index);
  
  return (pTmr->state != TMR_STOPPED)?RESET:SET;
}

