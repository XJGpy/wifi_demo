/*******************                    �� �� �� �� �� ˾ ************************/
/* �������ƣ�					                      	      					*/
/* �ļ����ƣ�   					      		      							*/
/* ��    �ߣ�			 					      								*/
/* ������ڣ�								      								*/
/* ��ǰ�汾��						      		      							*/
/* ���¼�¼: 								      								*/
/********************************************************************************/
#include "../include/user_config.h"
#include "stdio.h"
#include "stdarg.h"
#include "decode.h"
#include "dbg.h"


static dbgParam_t   dbgParam;

/*******************************************************************************
*�������ƣ�void dbgInit(putFunc_t pPutBuff)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
void dbgInit(putFunc_t pPutBuff,uint16_t size)
{
  dbgParam.put = pPutBuff;
  dbgParam.size = size;
}


/*******************************************************************************
*�������ƣ�void dbgWriteBuff(const char *string,uint16_t len)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
void dbgWriteBuff(const char *string,uint16_t len)
{
  uint16_t bytes_to_write = len;
  
  if(dbgParam.put == NULL)
	  return;
  
  if(bytes_to_write >= dbgParam.size)
  {
    bytes_to_write = dbgParam.size;
  }

  dbgParam.put((uint8_t *)string, bytes_to_write);
  
}

/*******************************************************************************
*�������ƣ�void dbgWriteFormatted(const char *format,...)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
void dbgWriteFormatted(const char *format,...)
{
  char *pTemp = NULL;
  
  if(dbgParam.put == NULL)
	  return;
  
  pTemp = memMalloc(dbgParam.size);

  if(pTemp == NULL)
	  return;

  memset(pTemp,0x00,dbgParam.size);
  
  va_list va;
  va_start(va, format);
  vsnprintf(pTemp, dbgParam.size-1, format, va);
  va_end(va);
  
  dbgWriteBuff(pTemp,strlen(pTemp));
  
  memFree(pTemp);
}

/*******************************************************************************
*�������ƣ�void dbgHexToString(const char *pBuff,uint16_t len)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
void dbgHexToString(const char *pBuff,uint16_t len)
{

    uint16_t MessageLength =2*(len+1);
    uint16_t count = len;
    char *pMessage =NULL;

    pMessage = memMalloc(MessageLength);
    
    if(pMessage ==NULL)
    	return;
    
    if(MessageLength >= dbgParam.size)
    {
       count = dbgParam.size/2-1;
    }
    
    memset(pMessage,0x00,MessageLength);
    Bytes2ToString(pBuff,pMessage,count);
    dbgParam.put(pMessage,strlen(pMessage));
    memFree(pMessage);
}


