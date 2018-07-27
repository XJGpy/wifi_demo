/*******************                    科 技 有 限 公 司 *********************/
/* 工程名称：					                      	                      */
/* 文件名称：   					      		                              */
/* 作    者：			 					                                  */
/* 完成日期：								                                  */
/* 当前版本：						      		                              */
/* 更新记录: 								                                  */
/******************************************************************************/
#include "../include/user_config.h"
#include "param.h"

static paramInfor_t  paramInfor;

param_t*pSettings = NULL;


const server_t  serverDefaultTable[MAX_SERVER_COUNT]={
  {{"ehw.leyaoyao.com"},4933}
};

const link_t  linkDefault={
  "CMNET",
  "leyaoyao_old",
  "leyaoyao168"
};



/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
static uint32_t  paramCrc(const char *pTemp,uint16_t len)
{
  uint16_t index = 0x00;
  uint32_t crc = 0x00;
  
  for(index = 0x00;index < len; index++){
    crc += pTemp[index];
  }
  
  return crc;  
}


/*******************************************************************************
*函数名称：void paramDefault(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void  paramDefault(void)
{
   if(pSettings == NULL)             return;
   
   //所有参数设置为0
   memset(pSettings,0x00,sizeof(param_t));
   
   //服务器默认值参数
   memcpy(&pSettings->server,&serverDefaultTable,sizeof(serverDefaultTable));
   
   
   //连接GPRS默认参数
   memcpy(&pSettings->link,&linkDefault,sizeof(link_t));
   
   //上传默认参数
   pSettings->send.count = MAX_SEND_COUNT;
   pSettings->send.wait  = MAX_SEND_WAIT;
   
   //兑币机参数
   pSettings->coin.pulse = DEFAULT_PLUSE_TIME;
   pSettings->coin.redial = DEFAULT_PLUSE_REDIAL;
   
   char index = 0x00;
   for(index =0x00;index< MAX_BILL_DEVICE_COUNT;index++){
	   pSettings->bill.bill_accepter[index]= DEVICE_ITL;
   }
   
   //通信密码
   
   paramSave();
}


/*******************************************************************************
*函数名称：void paramInit(paraRead_f read,paraWrite_f write)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void  paramInit(paraRead_f read,paraWrite_f write)
{
	uint32_t crc = 0x00;
	uint8_t crc_size = 0x00;
  
	memset(&paramInfor,0x00,sizeof(paramInfor_t));
	paramInfor.read = read;
	paramInfor.write = write;
  
	pSettings = &paramInfor.param;
  
	if(paramInfor.read)
		paramInfor.read(pSettings);
  
	crc_size = sizeof(pSettings->crc);
	crc = paramCrc((char *)&pSettings->crc+crc_size,sizeof(param_t)-crc_size);

	if((pSettings->crc != crc) ||
			(pSettings->crc == 0xFFFFFFFF)||
				(pSettings->crc == 0x00000000))
	{
		paramDefault();
	}
}


/*******************************************************************************
*函数名称：int paramSave(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
int  paramSave(void)
{
  uint8_t crc_size = 0x00;
  if(pSettings == NULL)                 return -1;
  if(paramInfor.write == NULL)          return -2;
  
  crc_size = sizeof(pSettings->crc);
  pSettings->crc = 0x00;
  pSettings->crc = paramCrc((char *)&pSettings->crc+crc_size,sizeof(param_t)-crc_size);
  
  return paramInfor.write(pSettings,sizeof(param_t));
  
}
