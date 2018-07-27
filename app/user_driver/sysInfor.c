/*******************                    �� �� �� �� �� ˾ *********************/
/* �������ƣ�					                      	                      */
/* �ļ����ƣ�   					      		                              */
/* ��    �ߣ�			 					                                  */
/* ������ڣ�								                                  */
/* ��ǰ�汾��						      		                              */
/* ���¼�¼: 								                                  */
/******************************************************************************/
#include "../include/user_config.h"
#include "sysInfor.h"

static sysInfor_t       sysInfor;

#ifdef NDJ_Device   // xuejungong 20180705 add

static ndj_upgrade_t    ndj_upgrade_info;

/*******************************************************************************
*�������ƣ�ndj_upgrade_t *ndjInforGet(void)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
ndj_upgrade_t *ndjInforGet(void)
{
  return &ndj_upgrade_info;
}

/*******************************************************************************
*�������ƣ�void ndjInforClean(void)
*����˵������������̼�������Ť������
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
void ndjInforClean(void)
{
	memset(&ndj_upgrade_info,0x00,sizeof(ndj_upgrade_t));
	flashWrite(UPGRADE_INFOR_ADDR_NDJ,(char *)&ndj_upgrade_info,sizeof(ndj_upgrade_t));
}

#endif

/*******************************************************************************
*�������ƣ�void sysInforInit(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
void sysInforInit(void)
{
	memset(&sysInfor,0x00,sizeof(sysInfor_t));
	flashRead(UPGRADE_INFOR_ADDR,(char *)&sysInfor.upgrade,sizeof(upgrade_t));

	if(sysInfor.upgrade.upgrade == 0xff)
	{
	  memset(&sysInfor,0x00,sizeof(sysInfor_t));
	  APP_TRACE_DBG("\r\n********************10 sysinfo = 0\r\n");
	}

#ifdef NDJ_Device   // xuejungong 20180705 add
	memset(&ndj_upgrade_info,0x00,sizeof(ndj_upgrade_t));
	flashRead(UPGRADE_INFOR_ADDR_NDJ,(char *)&ndj_upgrade_info,sizeof(ndj_upgrade_t));

	if(ndj_upgrade_info.upgrade == 0xff)
	{
	  memset(&ndj_upgrade_info,0x00,sizeof(ndj_upgrade_t));
	  APP_TRACE_DBG("\r\n********************11 ndjinfo = 0\r\n");
	}
#endif
}


/*******************************************************************************
*�������ƣ�sysInfor_t *sysInforGet(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
sysInfor_t *sysInforGet(void)
{
  return &sysInfor;
}


// xuejungong 20180724 add for IDConfig
/*******************************************************************************
*�������ƣ�
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
void IDConfig_Check(void)
{
	char ID_Mark = 0xff;
	flashRead(IDCONFIG_ADDR,(char *)&ID_Mark,sizeof(char));
	printf("\r\n========== Get ID_Mark=%d ===========", ID_Mark);

	if(!ID_Mark)
	{
		printf("\r\n========= ID_Mark Already Save=========\r\n");
		xMarkSetValue(MARK_NDJ_IDC, RESET);
	}
	else
	{
		printf("\r\n========= ID_Mark Config Now=========\r\n");
		xMarkSetValue(MARK_NDJ_IDC, SET);
	}
}

// xuejungong 20180724 add for IDConfig
/*******************************************************************************
*�������ƣ�
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
void IDConfig_Save(void)
{
	char ID_Mark = 0x00;
	flashWrite(IDCONFIG_ADDR,(char *)&ID_Mark,sizeof(char));

	IDConfig_Check();
}


