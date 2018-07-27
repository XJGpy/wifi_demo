/*******************                    �� �� �� �� �� ˾ *********************/
/* �������ƣ�					                      	                      */
/* �ļ����ƣ�   					      		                              */
/* ��    �ߣ�			 					                                  */
/* ������ڣ�								                                  */
/* ��ǰ�汾��						      		                              */
/* ���¼�¼: 								                                  */
/******************************************************************************/
#ifndef __XTMR_H__
#define __XTMR_H__


  typedef enum{ 
    //TMR_UART = 0x00,
    //TMR_CONSOLE,
    //TMR_PERIOD,
    TMR_RDY,
    TMR_LINK,
    TMR_CSQ,
    //TMR_MARK,
    TMR_HEART,
    TMR_COIN,
    //TMR_ACCOUNT,
    //TMR_ACCOUNT_INC,
    //TMR_ACCOUNT_TOTAL,
	TMR_NDJ_CHECK,  // xuejungong 20180718 add timer for check ndj
	TMR_NDJ_SC,     // xuejungong 20180721 add timer for smart config

    MAX_TMR
  }xTmrTypeDef;

/*
#define     TMR_UART            0x00
#define     TMR_PERIOD          0x01
#define     TMR_RDY             0x02
#define     TMR_LINK            0x03
#define     TMR_CSQ             0x04
#define     TMR_HEART           0x05
#define     TMR_CONSOLE         0x06
#define     TMR_MARK            0x07
#define     TMR_ACCOUNT         0x08    
#define     TMR_ACCOUNT_INC     0x09    
#define     TMR_ACCOUNT_TOTAL   0x0A
#define     MAX_TMR             0x0B 
*/
   
/*******************************************************************************
*�������ƣ�void xTmrInit(xTmrTypeDef index,const tmr_ticks_t delay_in_ticks,pTickFunc pTick)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void xTmrInit(xTmrTypeDef index,const tmr_ticks_t delay_in_ticks,pTickFunc pTick);


/*******************************************************************************
*�������ƣ�void xTmrRestart(xTmrTypeDef index)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void xTmrRestart(xTmrTypeDef index);


/*******************************************************************************
*�������ƣ�void xTmrStop(xTmrTypeDef index)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void xTmrStop(xTmrTypeDef index);


/*******************************************************************************
*�������ƣ�void xTmrPrioed(xTmrTypeDef index,const tmr_ticks_t delay_in_ticks)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void xTmrPrioed(xTmrTypeDef index,const tmr_ticks_t delay_in_ticks);


/*******************************************************************************
*�������ƣ�char sysTmrExpired(xTmrTypeDef index)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern char xTmrExpired(xTmrTypeDef index);


/*******************************************************************************
*�������ƣ�tmr_t *xTmrGetInfor(xTmrTypeDef index)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern tmr_t *xTmrGetInfor(xTmrTypeDef index);


/*******************************************************************************
*�������ƣ�char xTmrIsEnd(xTmrTypeDef index)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern char xTmrIsEnd(xTmrTypeDef index);

#endif
