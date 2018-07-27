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
#include "xled.h"

static xLedFlash_t      ledFlash[MAX_LED];

/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
static void  xLedOnOff(char nLed,ledOnOffTypeDef onoff)
{
   char mode =  coinGetMode();

   switch(nLed)
   {
   	   case GRN_LED:
   	   case RED_LED:
   		   if(onoff == LED_OFF)
   		   {
   			   xCtrlOnOff(nLed,CTRL_HIGH);
   		   }
   		   else{
   			   xCtrlOnOff(nLed,CTRL_LOW);
   		   }
   	   break;

//   case COIN_OUT:
//       if (mode == TRIGER_RISING){
//           if (onoff == LED_OFF){
//               xCtrlOnOff(CTRL_COIN_OUT, CTRL_HIGH);
//           } else{
//               xCtrlOnOff(CTRL_COIN_OUT, CTRL_LOW);
//           }
//       }
//       else{
//           if (onoff == LED_OFF){
//               xCtrlOnOff(CTRL_COIN_OUT, CTRL_LOW);
//           } else{
//               xCtrlOnOff(CTRL_COIN_OUT, CTRL_HIGH);
//           }
//       }
//       break;

   	   default:
   	   break;
   }
}


/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
static void  xLedFlash(char nLed)
{
	xLedFlash_t   *pFlash = &ledFlash[nLed];
  
	switch(pFlash->state)
	{
		case  FLASH_STATE_IDLE:

		break;

		case FLASH_STATE_START:
			if(pFlash->count == 0x00)
			{
				xLedOnOff(nLed,LED_OFF);
				pFlash->state = FLASH_STATE_IDLE;
			  break;
			}

			if(pFlash->offTime == 0x00)
			{
				xLedOnOff(nLed,LED_ON);
				pFlash->state = FLASH_STATE_IDLE;
				break;
			}

			pFlash->runTime = pFlash->onTime;
			pFlash->state = FLASH_STATE_ON;
			xLedOnOff(nLed,LED_ON);
		break;

		case FLASH_STATE_ON:
			pFlash->runTime--;

			if(pFlash->runTime==0x00)
			{
				pFlash->runTime=pFlash->offTime;
				pFlash->state = FLASH_STATE_OFF;
				xLedOnOff(nLed,LED_OFF);
			}
		break;

		case FLASH_STATE_OFF:
			pFlash->runTime--;

			if(pFlash->runTime==0x00)
			{
				if( pFlash->count >0x00)
				{
					pFlash->count--;
					if(pFlash->count==0x00)
					{
						xLedOnOff(nLed,LED_OFF);
						pFlash->state = FLASH_STATE_IDLE;
						break;
					}
				}
				pFlash->runTime = pFlash->onTime;
				pFlash->state = FLASH_STATE_ON;
				xLedOnOff(nLed,LED_ON);
			}
		break;

		default:
			memset(pFlash,0x00,sizeof(xLedFlash_t));
			pFlash->state= FLASH_STATE_IDLE;
			xLedOnOff(nLed,LED_OFF);
		break;
	}
}


/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
static void  xLedFlashStart(char nLed,uint16_t ontime,uint16_t offtime,uint8_t cnt)
{ 
    xLedFlash_t   *pFlash = &ledFlash[nLed];
    pFlash->state=FLASH_STATE_START;
    pFlash->count =cnt;
    pFlash->onTime =ontime;
    pFlash->offTime =offtime;
    pFlash->runTime =0x00;
}


/*******************************************************************************
*函数名称：void xLedInit(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void  xLedInit(void)
{
  char index =0x00;
  for(index = 0x00; index < MAX_LED; index++){
    memset(&ledFlash[index],0x00,sizeof(xLedFlash_t));
    ledFlash[index].state= FLASH_STATE_IDLE;
  }
}
/*******************************************************************************
*函数名称：void xLedStart(char nLedMark,uint16_t ontime,uint16_t offtime,uint8_t cnt)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void  xLedStart(char nLedMark,uint16_t ontime,uint16_t offtime,uint8_t cnt)
{
	char nled = 0x00;
  
	for(nled =0x00;nled<MAX_LED;nled++)
	{
		if(nLedMark & (1<<nled))
		{
			xLedFlashStart(nled,ontime/TICKS_PERIOD_MS,offtime/TICKS_PERIOD_MS,cnt);
		}
	}
}


/*******************************************************************************
*函数名称：void xLedTime(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void  xLedTime(void)
{
    char nled = 0x00;

    for(nled =0x00;nled<MAX_LED;nled++)
    {
    	xLedFlash(nled);
    }
}


/*******************************************************************************
*函数名称：char xLedIsOK(char nLed)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
char  xLedIsOK(char nLed)
{
  xLedFlash_t   *pFlash = NULL;
  
  pFlash = &ledFlash[nLed];
  
  if((pFlash->state != FLASH_STATE_IDLE)||(pFlash->count != 0x00)){
    return pdFALSE;
  }
  else{
    return pdTRUE;
  }
}

/*******************************************************************************
*函数名称：char xLedIsIdle(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
char  xLedIsIdle(void)
{
   char retValue =0x00;
   char nLed = 0x00;
   xLedFlash_t   *pFlash = NULL;
   
  for(nLed =0x00;nLed< MAX_LED;nLed++){
    pFlash = &ledFlash[nLed];
    if((pFlash->state != FLASH_STATE_IDLE)||(pFlash->count != 0x00)){
      break;
    }
  }
  
  if(nLed >= MAX_LED)   retValue =0x01;
   
   return retValue;
}

