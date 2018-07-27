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
#include "coin.h"


//Ӳ�Ҽ���
static hardMoney_t   hardMoney,*pHardMoney = &hardMoney;

void xTrigersEvent(char xTriger,xTrigerStateTypeDef state);


/*******************************************************************************
*�������ƣ�void coinInGetValue(char index)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
void coinInGetValue(char index)
{
  pHardMoney->value[index] = xTrigerReadValue(index);
  
  APP_TRACE_DBG("\r\n[%d]������:%s\r\n", index,pHardMoney->value[index]==TRIGER_RISING?"�͵�ƽ":"�ߵ�ƽ");
     
}




/*******************************************************************************
*�������ƣ�void coinInit(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
void coinInit(void)
{
	if(xMarkGetValue(MARK_NDJ_IDC) == RESET) // xuejungong 20180724 ������ID���������485ͨ��
	{
		APP_TRACE_DBG("\r\n*********************xx Start EIR Config\r\n");
		char index = 0x00;
		pHardMoney = &hardMoney;

		memset(pHardMoney,0x00,sizeof(hardMoney_t));

		for(index = 0x00;index < MAX_TRIGER;index++)
		{
			xTrigerInit(index,SET,xTrigersEvent);
		}

		for(index = 0x00;index < MAX_TRIGER;index++)
		{
			xTrigerEnable(index,RESET);
			coinInGetValue(index);
			xTrigerEnable(index,SET);
		}
	}
}


// xuejungong 20180717 change
/*******************************************************************************
*�������ƣ�void coinOutEnable(char value)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
//void coinOutEnable(char value)
//{
//    if(value)
//        xCtrlOnOff(CTRL_MODE,CTRL_HIGH);
//    else
//        xCtrlOnOff(CTRL_MODE,CTRL_LOW);
//}

/*******************************************************************************
*�������ƣ�xTrigerStateTypeDef coinGetMode(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
xTrigerStateTypeDef coinGetMode(void)
{
    return pHardMoney->mode;
}


/*******************************************************************************
*�������ƣ�void coinInDone(char index,xTrigerStateTypeDef status)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
void coinInDone(char index,xTrigerStateTypeDef status)
{
  char coinInFlag = RESET;
  
  
  //��ƽ
  if(status != pHardMoney->value[index]){
    coinInFlag = SET;
  }
  
  //��⵽Ͷ��
  if(coinInFlag == SET){
    APP_TRACE_DBG("\n [%d]��⵽Ͷ�ҳɹ���\r\n",index);
    coinSimulateValue(1);
    coinSetTotalValue(1);
  }
}



/*******************************************************************************
*�������ƣ�void giftSetTotalValue(uint16_t value)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
void giftSetTotalValue(uint16_t value)
{
  tmr_t *pTmr = NULL;

  if(value > 0x00){
    
    pHardMoney->gift += value;

    pSettings->note.gift_total += value;
  }
  
  //
  pTmr = xTmrGetInfor(TMR_COIN);
  //������ʱ�ϴ�������
  if(pTmr->state == TMR_STOPPED){
    xTmrRestart(TMR_COIN); 
  }
}



/*******************************************************************************
*�������ƣ�void giftInDone(char index,xTrigerStateTypeDef status)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
void giftInDone(char index,xTrigerStateTypeDef status)
{
  char coinInFlag = RESET;
  tmr_t *pTmr = NULL;
  
  //��ƽ
  if(status != pHardMoney->value[index]){
    coinInFlag = SET;
  }
  
  //��⵽���
  if(coinInFlag == SET){
    APP_TRACE_DBG("\n [%d]��⵽��Ʒ�ɹ���\r\n",index);
    giftSetTotalValue(1);
  }
}


/*******************************************************************************
*�������ƣ�void  coinSendTotalInfor(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
void  coinSendTotalInfor(void)
{

  if(pHardMoney->total > 0x00){
    
    APP_TRACE_DBG("\n �ϴ����һ��Ͷ��������Ϊ%d�¼�\r\n",pHardMoney->total);
    
    lyyCoinPacket(pSettings->note.coin_total,pHardMoney->total);

    pHardMoney->total = 0x00;
  }
}


/*******************************************************************************
*�������ƣ�void  giftSendTotalInfor(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
void  giftSendTotalInfor(void)
{
   if(pHardMoney->gift > 0x00) {
    
    APP_TRACE_DBG("\n �ϴ����һ�μ����Ʒ������Ϊ%d�¼�\r\n",pHardMoney->gift);
    
    lyyGiftPacket(pSettings->note.gift_total, pHardMoney->gift);

    pHardMoney->gift = 0x00;
  }
}

/*******************************************************************************
*�������ƣ�void coinSetTotalValue(uint16_t value)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
void coinSetTotalValue(uint16_t value)
{
  tmr_t *pTmr = NULL;
  
  if(value > 0x00){
    
    pHardMoney->total += value;

    pSettings->note.coin_total += value;
  }
  
  //
  pTmr = xTmrGetInfor(TMR_COIN);
  //������ʱ�ϴ�������
  if(pTmr->state == TMR_STOPPED){
    xTmrRestart(TMR_COIN);
  }
}

/*******************************************************************************
*�������ƣ�void coinSimulateValue(uint16_t value)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
void coinSimulateValue(uint16_t value)
{
  if(value > 0x00){
    pHardMoney->in  += value;
    //APP_TRACE_DBG("\n Ͷ������ %d����\r\n",pHardMoney->in);
  }
}

/*******************************************************************************
*�������ƣ�void coinSimulateEvent(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
void coinSimulateEvent(void)
{
  uint16_t count = 0x00;
  char     coinFlag = 0x00;
  
  if(pHardMoney->in != pHardMoney->out){
    coinFlag = xLedIsOK(COIN_OUT);
    if(coinFlag){
      
      count = pHardMoney->in - pHardMoney->out;
      pHardMoney->in -= count;
      pHardMoney->out = 0x00;
      //ģ�������ź�
      
      APP_TRACE_DBG("\r\nͶ��������%d��\r\n",count);
      if (pSettings->coin.pulse == 0x00) {
          pSettings->coin.pulse = 60;
      }

      if (pSettings->coin.redial == 0x00) {
          pSettings->coin.redial = 60;
      }

      xLedStart(1 << COIN_OUT, pSettings->coin.pulse, pSettings->coin.redial, count);
      redLedFlash(pSettings->coin.pulse, pSettings->coin.redial,count);
    }
  }
}


/*******************************************************************************
*�������ƣ�void keyDone(char index,xTrigerStateTypeDef status)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
void keyDone(char index,xTrigerStateTypeDef status)
{
   if(status == TRIGER_FALLING)
   {
	   if(xMarkGetValue(MARK_NDJ_SC))//  xuejungong 2080723 add mark for avoiding system restart
	   {
		   xMarkSetValue(MARK_NDJ_SC,RESET); // xuejungong 20180723 add clean smartconfig mark
		   APP_TRACE_DBG("\r\n==============Smartconfig=============\r\n");
	   	   redLedFlash(100,0,1);
	   	   xTaskCreate(smartconfigTask, "smartconfigTask", 1*256, NULL, 1, NULL);
	   }
   }
}


/*******************************************************************************
*�������ƣ�void keyDone(char index,xTrigerStateTypeDef status)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
void keyDone_NDJ(char index,xTrigerStateTypeDef status)
{
	uint8_t i;
	if(status == TRIGER_FALLING){
		APP_TRACE_DBG("\r\n==============Cleaning Data=============\r\n");
		redLedFlash(100,0,1);

		APP_TRACE_DBG("\r\n============Ť����������%d===========\r\n", pSettings->ndj_num);
		for(i=0;i<pSettings->ndj_num;i++)
		{
			memset(pSettings->ndj_para[i].id, 0x00, 8);
			memset(pSettings->ndj_para[i].sw, 0x00, 14);
		}
		pSettings->ndj_num = 0; // ���Ť��������

		ndjInforClean();
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
static void xTrigersDone(xEventParam_t *pEvent)
{
  xTrigerStateTypeDef status =(xTrigerStateTypeDef)pEvent->event.event;

  APP_TRACE_DBG("\n[%02d] xTriger %s Event!\r\n",
                pEvent->event.index,
                pEvent->event.event==TRIGER_RISING?"Rising":"Falling");

  switch(pEvent->event.index){

//  case COIN_IN1:
//  case COIN_IN2:
//	  coinInDone(pEvent->event.index,pEvent->event.event);
//    break;
//
//
//  case GIFT_IN:
//      giftInDone(pEvent->event.index,pEvent->event.event);
//    break;

   case KEY_IN:
//	   if(status == TRIGER_FALLING){
//		   APP_TRACE_DBG("\r\n===============KEY_DOWN===================\r\n");
//	   }
	   keyDone_NDJ(pEvent->event.index,pEvent->event.event);
    break;

   case KEY_SC:
	   keyDone(pEvent->event.index,pEvent->event.event);
    break;

  default:
    break;
  }
}


/*******************************************************************************
*�������ƣ�void xTrigersEvent(char xTriger,xTrigerStateTypeDef state)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
void xTrigersEvent(char xTriger,xTrigerStateTypeDef state)
{
	xEventSend(EVENT_HIGH,xTrigersDone,xTriger,state);
}
