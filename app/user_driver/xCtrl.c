/*******************                    �� �� �� �� �� ˾ *********************/
/* �������ƣ�					                      	      */
/* �ļ����ƣ�   					      		      */
/* ��    �ߣ�			 					      */
/* ������ڣ�								      */
/* ��ǰ�汾��						      		      */
/* ���¼�¼: 								      */
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
*�������ƣ�void xCtrlOnOff(char index, xCtrlFlagTypeDef valueFlag)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�void xCtrlInit(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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




