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
#include "xled.h"

static xLedFlash_t      ledFlash[MAX_LED];

/*******************************************************************************
*�������ƣ�
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�void xLedInit(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�void xLedStart(char nLedMark,uint16_t ontime,uint16_t offtime,uint8_t cnt)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�void xLedTime(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�char xLedIsOK(char nLed)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�char xLedIsIdle(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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

