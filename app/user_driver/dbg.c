/*******************                    科 技 有 限 公 司 ************************/
/* 工程名称：					                      	      					*/
/* 文件名称：   					      		      							*/
/* 作    者：			 					      								*/
/* 完成日期：								      								*/
/* 当前版本：						      		      							*/
/* 更新记录: 								      								*/
/********************************************************************************/
#include "../include/user_config.h"
#include "stdio.h"
#include "stdarg.h"
#include "decode.h"
#include "dbg.h"


static dbgParam_t   dbgParam;

/*******************************************************************************
*函数名称：void dbgInit(putFunc_t pPutBuff)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void dbgInit(putFunc_t pPutBuff,uint16_t size)
{
  dbgParam.put = pPutBuff;
  dbgParam.size = size;
}


/*******************************************************************************
*函数名称：void dbgWriteBuff(const char *string,uint16_t len)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
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
*函数名称：void dbgWriteFormatted(const char *format,...)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
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
*函数名称：void dbgHexToString(const char *pBuff,uint16_t len)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
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


