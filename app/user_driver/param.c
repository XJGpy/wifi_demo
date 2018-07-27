/*******************                    �� �� �� �� �� ˾ *********************/
/* �������ƣ�					                      	                      */
/* �ļ����ƣ�   					      		                              */
/* ��    �ߣ�			 					                                  */
/* ������ڣ�								                                  */
/* ��ǰ�汾��						      		                              */
/* ���¼�¼: 								                                  */
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
*�������ƣ�
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�void paramDefault(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
void  paramDefault(void)
{
   if(pSettings == NULL)             return;
   
   //���в�������Ϊ0
   memset(pSettings,0x00,sizeof(param_t));
   
   //������Ĭ��ֵ����
   memcpy(&pSettings->server,&serverDefaultTable,sizeof(serverDefaultTable));
   
   
   //����GPRSĬ�ϲ���
   memcpy(&pSettings->link,&linkDefault,sizeof(link_t));
   
   //�ϴ�Ĭ�ϲ���
   pSettings->send.count = MAX_SEND_COUNT;
   pSettings->send.wait  = MAX_SEND_WAIT;
   
   //�ұһ�����
   pSettings->coin.pulse = DEFAULT_PLUSE_TIME;
   pSettings->coin.redial = DEFAULT_PLUSE_REDIAL;
   
   char index = 0x00;
   for(index =0x00;index< MAX_BILL_DEVICE_COUNT;index++){
	   pSettings->bill.bill_accepter[index]= DEVICE_ITL;
   }
   
   //ͨ������
   
   paramSave();
}


/*******************************************************************************
*�������ƣ�void paramInit(paraRead_f read,paraWrite_f write)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�int paramSave(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
