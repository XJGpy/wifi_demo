/*******************                    科 技 有 限 公 司 *********************/
/* 工程名称：					                      	      */
/* 文件名称：   					      		      */
/* 作    者：			 					      */
/* 完成日期：								      */
/* 当前版本：						      		      */
/* 更新记录: 								      */
/******************************************************************************/
#include "../include/user_config.h"
#include "xCtrl.h"

/**/
const xCTRLTable_t        xCtrlTable[MAX_CTRL]={
  
  {GPIO_ID_PIN(1),              CTRL_ON_LOW},           //led_green
  {GPIO_ID_PIN(2),              CTRL_ON_LOW},           //led_red
  //{GPIO_ID_PIN(16),             CTRL_ON_HIGH},           //coin_out
 // {GPIO_ID_PIN(5),              CTRL_ON_HIGH},           //mode_enable
  {GPIO_ID_PIN(4),              CTRL_ON_LOW},           //xuejungong 20180717 add RS_485_RE
};

/*******************************************************************************
*函数名称：void xCtrlOnOff(char index, xCtrlFlagTypeDef valueFlag)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void xCtrlOnOff(char index, xCtrlFlagTypeDef valueFlag)
{
    const xCTRLTable_t *pTable = NULL;

  if(index >= MAX_CTRL)                 return;
  
  pTable = &xCtrlTable[index];
  
  if(valueFlag == CTRL_HIGH){  // xuejungong 20180717  change
//		if(index == CTRL_COIN_OUT){
//			gpio16_output_set(SET);
//		}
//		else{
			GPIO_OUTPUT_SET(pTable->pin,SET);
//		}
  }
  else{
//		if(index == CTRL_COIN_OUT){
//			gpio16_output_set(RESET);
//		}
//		else{
			GPIO_OUTPUT_SET(pTable->pin,RESET);
//		}
  } 
}

/*******************************************************************************
*函数名称：void xCtrlInit(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void xCtrlInit(void)
{
  char index =0x00;
  const xCTRLTable_t *pTable =NULL;

  for(index = 0x00; index < MAX_CTRL; index++){

    pTable = &xCtrlTable[index];

    if(pTable->onFlag == CTRL_ON_LOW){
    	xCtrlOnOff(index,CTRL_HIGH);
    }
    else{
    	xCtrlOnOff(index,CTRL_LOW);
    }
  }
}




