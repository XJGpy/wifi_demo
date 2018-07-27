/*******************                    科 技 有 限 公 司 *********************/
/* 工程名称：					                      	                      */
/* 文件名称：   					      		                              */
/* 作    者：			 					                                  */
/* 完成日期：								                                  */
/* 当前版本：						      		                              */
/* 更新记录: 								                                  */
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
*函数名称：void xTmrInit(xTmrTypeDef index,const tmr_ticks_t delay_in_ticks,pTickFunc pTick)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void xTmrInit(xTmrTypeDef index,const tmr_ticks_t delay_in_ticks,pTickFunc pTick);


/*******************************************************************************
*函数名称：void xTmrRestart(xTmrTypeDef index)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void xTmrRestart(xTmrTypeDef index);


/*******************************************************************************
*函数名称：void xTmrStop(xTmrTypeDef index)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void xTmrStop(xTmrTypeDef index);


/*******************************************************************************
*函数名称：void xTmrPrioed(xTmrTypeDef index,const tmr_ticks_t delay_in_ticks)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void xTmrPrioed(xTmrTypeDef index,const tmr_ticks_t delay_in_ticks);


/*******************************************************************************
*函数名称：char sysTmrExpired(xTmrTypeDef index)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern char xTmrExpired(xTmrTypeDef index);


/*******************************************************************************
*函数名称：tmr_t *xTmrGetInfor(xTmrTypeDef index)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern tmr_t *xTmrGetInfor(xTmrTypeDef index);


/*******************************************************************************
*函数名称：char xTmrIsEnd(xTmrTypeDef index)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern char xTmrIsEnd(xTmrTypeDef index);

#endif
