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
#include "coin.h"


//硬币计数
static hardMoney_t   hardMoney,*pHardMoney = &hardMoney;

void xTrigersEvent(char xTriger,xTrigerStateTypeDef state);


/*******************************************************************************
*函数名称：void coinInGetValue(char index)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void coinInGetValue(char index)
{
  pHardMoney->value[index] = xTrigerReadValue(index);
  
  APP_TRACE_DBG("\r\n[%d]脉冲检测:%s\r\n", index,pHardMoney->value[index]==TRIGER_RISING?"低电平":"高电平");
     
}




/*******************************************************************************
*函数名称：void coinInit(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void coinInit(void)
{
	if(xMarkGetValue(MARK_NDJ_IDC) == RESET) // xuejungong 20180724 配置了ID才允许进行485通信
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
*函数名称：void coinOutEnable(char value)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
//void coinOutEnable(char value)
//{
//    if(value)
//        xCtrlOnOff(CTRL_MODE,CTRL_HIGH);
//    else
//        xCtrlOnOff(CTRL_MODE,CTRL_LOW);
//}

/*******************************************************************************
*函数名称：xTrigerStateTypeDef coinGetMode(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
xTrigerStateTypeDef coinGetMode(void)
{
    return pHardMoney->mode;
}


/*******************************************************************************
*函数名称：void coinInDone(char index,xTrigerStateTypeDef status)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void coinInDone(char index,xTrigerStateTypeDef status)
{
  char coinInFlag = RESET;
  
  
  //电平
  if(status != pHardMoney->value[index]){
    coinInFlag = SET;
  }
  
  //检测到投币
  if(coinInFlag == SET){
    APP_TRACE_DBG("\n [%d]检测到投币成功！\r\n",index);
    coinSimulateValue(1);
    coinSetTotalValue(1);
  }
}



/*******************************************************************************
*函数名称：void giftSetTotalValue(uint16_t value)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
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
  //启动定时上传币数量
  if(pTmr->state == TMR_STOPPED){
    xTmrRestart(TMR_COIN); 
  }
}



/*******************************************************************************
*函数名称：void giftInDone(char index,xTrigerStateTypeDef status)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void giftInDone(char index,xTrigerStateTypeDef status)
{
  char coinInFlag = RESET;
  tmr_t *pTmr = NULL;
  
  //电平
  if(status != pHardMoney->value[index]){
    coinInFlag = SET;
  }
  
  //检测到礼口
  if(coinInFlag == SET){
    APP_TRACE_DBG("\n [%d]检测到礼品成功！\r\n",index);
    giftSetTotalValue(1);
  }
}


/*******************************************************************************
*函数名称：void  coinSendTotalInfor(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void  coinSendTotalInfor(void)
{

  if(pHardMoney->total > 0x00){
    
    APP_TRACE_DBG("\n 上传最后一次投币总数量为%d事件\r\n",pHardMoney->total);
    
    lyyCoinPacket(pSettings->note.coin_total,pHardMoney->total);

    pHardMoney->total = 0x00;
  }
}


/*******************************************************************************
*函数名称：void  giftSendTotalInfor(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void  giftSendTotalInfor(void)
{
   if(pHardMoney->gift > 0x00) {
    
    APP_TRACE_DBG("\n 上传最后一次检测礼品总数量为%d事件\r\n",pHardMoney->gift);
    
    lyyGiftPacket(pSettings->note.gift_total, pHardMoney->gift);

    pHardMoney->gift = 0x00;
  }
}

/*******************************************************************************
*函数名称：void coinSetTotalValue(uint16_t value)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
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
  //启动定时上传币数量
  if(pTmr->state == TMR_STOPPED){
    xTmrRestart(TMR_COIN);
  }
}

/*******************************************************************************
*函数名称：void coinSimulateValue(uint16_t value)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void coinSimulateValue(uint16_t value)
{
  if(value > 0x00){
    pHardMoney->in  += value;
    //APP_TRACE_DBG("\n 投币总数 %d：！\r\n",pHardMoney->in);
  }
}

/*******************************************************************************
*函数名称：void coinSimulateEvent(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
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
      //模拟脉冲信号
      
      APP_TRACE_DBG("\r\n投币数量：%d！\r\n",count);
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
*函数名称：void keyDone(char index,xTrigerStateTypeDef status)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
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
*函数名称：void keyDone(char index,xTrigerStateTypeDef status)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void keyDone_NDJ(char index,xTrigerStateTypeDef status)
{
	uint8_t i;
	if(status == TRIGER_FALLING){
		APP_TRACE_DBG("\r\n==============Cleaning Data=============\r\n");
		redLedFlash(100,0,1);

		APP_TRACE_DBG("\r\n============扭蛋机数量：%d===========\r\n", pSettings->ndj_num);
		for(i=0;i<pSettings->ndj_num;i++)
		{
			memset(pSettings->ndj_para[i].id, 0x00, 8);
			memset(pSettings->ndj_para[i].sw, 0x00, 14);
		}
		pSettings->ndj_num = 0; // 清楚扭蛋机数据

		ndjInforClean();
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
*函数名称：void xTrigersEvent(char xTriger,xTrigerStateTypeDef state)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void xTrigersEvent(char xTriger,xTrigerStateTypeDef state)
{
	xEventSend(EVENT_HIGH,xTrigersDone,xTriger,state);
}
