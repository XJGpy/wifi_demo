/*******************                    �� �� �� �� �� ˾ *********************/
/* �������ƣ�					                      	                      */
/* �ļ����ƣ�   					      		                              */
/* ��    �ߣ�			 					                                  */
/* ������ڣ�								                                  */
/* ��ǰ�汾��						      		                              */
/* ���¼�¼: 								                                  */
/******************************************************************************/
#include "../include/user_config.h"
#include "tmr.h"
#include "xTmr.h"


static tmr_t            xTmr[MAX_TMR];

/*******************************************************************************
*�������ƣ�void xTmrInit(xTmrTypeDef index,const tmr_ticks_t delay_in_ticks,pTickFunc pTick)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�void xTmrRestart(xTmrTypeDef index)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
void xTmrRestart(xTmrTypeDef index)
{
  tmr_t *pTmr = NULL;
  if(index >= MAX_TMR)          return;
  
  pTmr = &xTmr[index];
  tmr_restart(pTmr);
}

/*******************************************************************************
*�������ƣ�void xTmrStop(xTmrTypeDef index)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
void xTmrStop(xTmrTypeDef index)
{
  tmr_t *pTmr = NULL;
  if(index >= MAX_TMR)          return;
  
  pTmr = &xTmr[index];
  tmr_stop(pTmr);
}

/*******************************************************************************
*�������ƣ�void xTmrPrioed(xTmrTypeDef index,const tmr_ticks_t delay_in_ticks)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
void xTmrPrioed(xTmrTypeDef index,const tmr_ticks_t delay_in_ticks)
{
  tmr_t *pTmr = NULL;
  if(index >= MAX_TMR)          return;
  
  pTmr = &xTmr[index];
  tmr_prioed(pTmr,delay_in_ticks);
}


/*******************************************************************************
*�������ƣ�char xTmrExpired(xTmrTypeDef index)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�tmr_t *sysTmrGetInfor(xTmrTypeDef index)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�char xTmrIsEnd(xTmrTypeDef index)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
char xTmrIsEnd(xTmrTypeDef index)
{
  tmr_t *pTmr = NULL;
  
  pTmr = xTmrGetInfor(index);
  
  return (pTmr->state != TMR_STOPPED)?RESET:SET;
}

