/*******************                    科 技 有 限 公 司 *********************/
/* 工程名称：					                      	      				*/
/* 文件名称：   					      		      							*/
/* 作    者：			 					      							*/
/* 完成日期：								      							*/
/* 当前版本：						      		      						*/
/* 更新记录: 								      							*/
/******************************************************************************/
#include "../include/user_config.h"
#include "xTriger.h"



//
const xTrigerTable_t     xTrigerTable[MAX_TRIGER]={
		GPIO_ID_PIN(14),GPIO_ID_PIN(3)
};

//
static xTriger_t  xTriger[MAX_TRIGER];

//const uint16_t xTrigerRising[MAX_TRIGER]={10,10,10,3000};
const uint16_t xTrigerRising[MAX_TRIGER]={10,10};

//const uint16_t xTrigerFalling[MAX_TRIGER]={10,10,10,100};
const uint16_t xTrigerFalling[MAX_TRIGER]={10,10};


/*******************************************************************************
*函数名称：void xTrigerInit(char xNum, char getFlag,
                  char priority,
                  xTrigerCallback callback)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void xTrigerInit(char xNum, char getFlag,xTrigerCallback_t callback)
{
  
	const xTrigerTable_t *pTable =NULL;

	if(xNum >= MAX_TRIGER)
		return;
  
	pTable =&xTrigerTable[xNum];
  
	memset(&xTriger[xNum],0x00,sizeof(xTriger_t));
  
	if(getFlag)
	{
		if(GPIO_INPUT_GET(pTable->pin))
		{
			xTriger[xNum].state= TRIGER_RISING;
		}
		else
		{
			xTriger[xNum].state= TRIGER_FALLING;
		}
	}
	else
	{
		xTriger[xNum].state= TRIGER_NULL;
	}
  
	xTriger[xNum].rising = xTrigerRising[xNum];
	xTriger[xNum].falling = xTrigerFalling[xNum];
  
	if(callback != NULL)
		xTriger[xNum].callback= callback;
}

/*******************************************************************************
*函数名称：xTrigerStateTypeDef xTrigerReadValue(char xNum)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
xTrigerStateTypeDef xTrigerReadValue(char xNum)
{
	const xTrigerTable_t *pTable =NULL;
  
	if(xNum >= MAX_TRIGER)
		return TRIGER_NULL;

	pTable =&xTrigerTable[xNum];
  
	if(GPIO_INPUT_GET(pTable->pin))
	{
		return  TRIGER_RISING;
	}
	else
	{
		return TRIGER_FALLING;
	}
}


/*******************************************************************************
*函数名称：void xTrigerEnable(char index,char enableFlag)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void xTrigerEnable(char xNum,char enableFlag)
{
  if(xNum >= MAX_TRIGER)                                return;
  
  if(xTriger[xNum].bit.bitFlag.enable == enableFlag) {
	return;
  }

  xTriger[xNum].bit.bitFlag.enable = enableFlag;
  

}

/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
static void xTrigerDone(char xNum)
{
	xTriger_t *pTriger =&xTriger[xNum];

	if(pTriger->last==pTriger->state)
		return;

	pTriger->last=pTriger->state;
  
	if(pTriger->callback)
	{
		pTriger->callback(xNum,pTriger->state);
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
static uint32_t  xTrigerGetDoubleTime(char xNum,xTrigerStateTypeDef  statueTemp)
{
	xTriger_t *pTriger = &xTriger[xNum];
   
	return (statueTemp == TRIGER_RISING)?pTriger->rising:pTriger->falling;
}


/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
static void xTrigerEvent(char xNum,xTrigerStateTypeDef  statueTemp)
{
	uint32_t  doubleTime =0x00;
	xTriger_t *pTriger = &xTriger[xNum];
  
	if(!pTriger->bit.bitFlag.enable)
		return;
  
	if(statueTemp == pTriger->state)
		return;

	pTriger->state = statueTemp;
  
	doubleTime = xTrigerGetDoubleTime(xNum,statueTemp);
  
	if((doubleTime/TICKS_PERIOD_MS)==0x00)
	{
		xTrigerDone(xNum);
		return;
	}
    
	tickRenewTime(&pTriger->start);
}

/*******************************************************************************
*函数名称：void xTrigerTime(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void xTrigerTime(void)
{
	uint32_t  doubleTime =0x00;
	static char xNum =0x00;
	xTriger_t *pTriger =NULL;
	xTrigerStateTypeDef state;
  
	for(xNum = 0x00;xNum < MAX_TRIGER; xNum++)
	{
		pTriger = &xTriger[xNum];
    
		if(!pTriger->bit.bitFlag.enable)
			continue;

		state =xTrigerReadValue(xNum);

		if(pTriger->state != state)
		{
			xTrigerEvent(xNum,state);
		}
		else
		{
			doubleTime = xTrigerGetDoubleTime(xNum,state);

			if(tickTimeout(pTriger->start,doubleTime))
			{
				xTrigerDone(xNum);
			}
		}
	}
}
        
        
/*******************************************************************************
*函数名称：char xTrigerIndexIsIdle(char index)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
char xTrigerIndexIsIdle(char index)
{
  xTriger_t *pTriger =NULL;
  pTriger = &xTriger[index];
  
  
  if((pTriger->state != pTriger->last)&&
     (pTriger->bit.bitFlag.enable)){
    return RESET;
  }
  else{
    return SET;
  }
}     


/*******************************************************************************
*函数名称：char xTrigerIsIdle(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
char xTrigerIsIdle(void)
{
  xTRIGERTypeDef  xNum;
  char retValue =0x00;
  xTriger_t *pTriger =NULL;
  
  for(xNum =(xTRIGERTypeDef)0;xNum < MAX_TRIGER; xNum++){
    pTriger = &xTriger[xNum];
    if((pTriger->state != pTriger->last)&&(pTriger->bit.bitFlag.enable)){
      break;
    }
  }
  
  if(xNum >= MAX_TRIGER)   retValue =0x01;
  
  return retValue;
  
}

/*******************************************************************************
*函数名称：xTriger_t * xTrigerGetInfor(xTRIGERTypeDef  xNum)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
xTriger_t *xTrigerGetInfor(xTRIGERTypeDef  xNum)
{
  if(xNum >= MAX_TRIGER)   return NULL;
  
  xTriger_t *pTriger = &xTriger[xNum];
  return pTriger;
}

/*******************************************************************************
*函数名称：void xTrigerSetCallback(xTRIGERTypeDef  xNum,xTrigerCallback_t callback)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void xTrigerSetCallback(xTRIGERTypeDef  xNum,xTrigerCallback_t callback)
{
  bool enableFlag =false;
  
  if(xNum >= MAX_TRIGER)   return;
  
  xTriger_t *pTriger = &xTriger[xNum];
  
  if(pTriger->bit.bitFlag.enable){
     enableFlag = pTriger->bit.bitFlag.enable;
     xTrigerEnable(xNum,RESET);
  }
  
  pTriger->callback =callback;
  
  if(enableFlag){
    xTrigerEnable(xNum,SET);
  }
}

/*******************************************************************************
*函数名称：void xTrigerReset(xTRIGERTypeDef  xNum,char priority)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void xTrigerReset(xTRIGERTypeDef xNum,char priority)
{
  xTriger_t *pTriger = &xTriger[xNum];

  if(pTriger->bit.bitFlag.enable == RESET)
    xTrigerEnable(xNum,SET);
  
  pTriger->state = TRIGER_NULL;
  
  tickRenewTime(&pTriger->start);
}








