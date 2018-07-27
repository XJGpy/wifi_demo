/*******************                    �� �� �� �� �� ˾ *********************/
/* �������ƣ�					                      	      				*/
/* �ļ����ƣ�   					      		      							*/
/* ��    �ߣ�			 					      							*/
/* ������ڣ�								      							*/
/* ��ǰ�汾��						      		      						*/
/* ���¼�¼: 								      							*/
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
*�������ƣ�void xTrigerInit(char xNum, char getFlag,
                  char priority,
                  xTrigerCallback callback)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�xTrigerStateTypeDef xTrigerReadValue(char xNum)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�void xTrigerEnable(char index,char enableFlag)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
static uint32_t  xTrigerGetDoubleTime(char xNum,xTrigerStateTypeDef  statueTemp)
{
	xTriger_t *pTriger = &xTriger[xNum];
   
	return (statueTemp == TRIGER_RISING)?pTriger->rising:pTriger->falling;
}


/*******************************************************************************
*�������ƣ�
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�void xTrigerTime(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�char xTrigerIndexIsIdle(char index)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�char xTrigerIsIdle(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�xTriger_t * xTrigerGetInfor(xTRIGERTypeDef  xNum)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
xTriger_t *xTrigerGetInfor(xTRIGERTypeDef  xNum)
{
  if(xNum >= MAX_TRIGER)   return NULL;
  
  xTriger_t *pTriger = &xTriger[xNum];
  return pTriger;
}

/*******************************************************************************
*�������ƣ�void xTrigerSetCallback(xTRIGERTypeDef  xNum,xTrigerCallback_t callback)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�void xTrigerReset(xTRIGERTypeDef  xNum,char priority)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
void xTrigerReset(xTRIGERTypeDef xNum,char priority)
{
  xTriger_t *pTriger = &xTriger[xNum];

  if(pTriger->bit.bitFlag.enable == RESET)
    xTrigerEnable(xNum,SET);
  
  pTriger->state = TRIGER_NULL;
  
  tickRenewTime(&pTriger->start);
}








