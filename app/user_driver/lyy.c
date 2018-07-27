/*******************                    科 技 有 限 公 司 *********************/
/* 工程名称：					                      	      */
/* 文件名称：   					      		      */
/* 作    者：			 					      */
/* 完成日期：								      */
/* 当前版本：						      		      */
/* 更新记录: 								      */
/******************************************************************************/
#include "../include/user_config.h"
#include "lyy.h"


static pLyySend_t  pLyySend;
static uint32_t    lyySn = 0x01;
static uint32_t    lyyGiftSn = 0x00;
static uint32_t    lyyPaySn = 0x00;

extern char	wifiRssiValue;


#define MAX_RING_PAY_COUNT              10
#define MAX_RING_PAY_TIMEOUT            5*60*1000 // xuejungong 20182727 5*60*1000*10ms

typedef enum{
  MARK_PAY      =  0x01,
  MARK_TIME     =  0x02
}payStatusTypedef;

typedef struct{
  char          status;
  char          sn[6];
  char          value;
  uint32_t      tick;
}lyyRingPay_t;

static lyyRingPay_t  lyyRingPay[MAX_RING_PAY_COUNT];
static char          lyyRingIndex = 0x00;


static char        lyyLoginMark = 0x00;   //大端数据格式
static char        lyyPayID[6];

#ifdef INTERFACE_DEVICE
//===============接口板系列===================
    #ifdef INTERFACE_DIP
    const version_t   SoftwareVersion={
     1501, "JS02M_V1.0.3","I_OM26_DIP_S1.1.0.1",
    };
    #else
    #ifdef SONGWANG
    const version_t   SoftwareVersion={
     2001, "WS02M_V1.0.5","I_OM26_SW_S1.1.0.1",
    };
    #else
    const version_t   SoftwareVersion={
     //1602, "WS02M_V1.0.5","I_OM26_S1.1.0.2",
     1603, "WS02M_V1.0.5","I_OM26_S1.1.0.3",
    };
    #endif
    #endif

#else
//===============娃娃机系列====================
    const version_t   SoftwareVersion={
     //2301, "WS02M_V1.0.5","W_OM26_S1.1.0.1",
     //2302, "WS02M_V1.0.5","W_OM26_S1.1.0.2",
     2401,   "NDJ_WIFI-V2.00","N_WIFI_S1.0.0.0",
    };

#endif


/*******************************************************************************
*函数名称：void lyyInit(pLyySend_t sendCallback)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void lyyInit(pLyySend_t sendCallback)
{
	lyySn = 0x01;
	lyyPaySn = 0x00;
	memset(lyyPayID,0x00,sizeof(lyyPayID));
	memset(lyyRingPay,0x00,MAX_RING_PAY_COUNT *sizeof(lyyRingPay_t));
  
	if(sendCallback != NULL){
		pLyySend = sendCallback;
	}
}

/*******************************************************************************
*函数名称：void ringPayTickEvent(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void ringPayTickEvent(void)
{
	char index = 0x00;
	lyyRingPay_t *pLyyPay =  lyyRingPay;
    
	for(index = 0x00; index < MAX_RING_PAY_COUNT;index++)
	{
		pLyyPay =  &lyyRingPay[index];
      
		if(pLyyPay->status & MARK_TIME)
		{
			if(tickTimeout(pLyyPay->tick,MAX_RING_PAY_TIMEOUT))
			{
				pLyyPay->status &= ~MARK_TIME;
			}
		}
	}
}


/*******************************************************************************
*函数名称：void ringPayEvent(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void ringPayEvent(void)
{
    char index = 0x00;
    lyyRingPay_t *pLyyPay =  lyyRingPay;

    for(index = 0x00; index < MAX_RING_PAY_COUNT;index++)
    {
    	pLyyPay =  &lyyRingPay[index];

    	if(pLyyPay->status & MARK_PAY)
    	{
    		pLyyPay->status &= ~MARK_PAY;
    		payEventDone(pLyyPay->sn,pLyyPay->value);
    		break;
    	}
    }
}


/*******************************************************************************
*函数名称：void writePayEvent(char *pSn,char value)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
char writePayEvent(char *pSn,char value)
{
    char index = 0x00;
    lyyRingPay_t *pLyyPay =  lyyRingPay;
    
    for(index = 0x00;index <MAX_RING_PAY_COUNT;index++){
      pLyyPay =  &lyyRingPay[index];
      if(memcmp(pLyyPay->sn,pSn,6)==0x00){
        return 0x01;
      }
    }
    
    for(index = 0x00; index < MAX_RING_PAY_COUNT;index++){
      pLyyPay =  &lyyRingPay[index];
      
      if(pLyyPay->status == 0x00){
          break;
      }
    }
    
    if(index >= MAX_RING_PAY_COUNT){
      if(lyyRingIndex >= (MAX_RING_PAY_COUNT-1)){
         index = 0x00;
      }
      else{
        index = lyyRingIndex+1;
      }
        
      pLyyPay =  &lyyRingPay[index];
    }
    
    memcpy(pLyyPay->sn,pSn,6);
    pLyyPay->value = value;
    pLyyPay->tick = sysGetTick();
    pLyyPay->status =  MARK_PAY | MARK_TIME;
    
    lyyRingIndex = index;
    
    return 0x00;
}


/*******************************************************************************
*函数名称：void lyyInit(pLyySend_t sendCallback)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
void lyyRondomSn(char *pSn)
{

	struct tm dateTime;

    getDateTime(&dateTime);

    *pSn      = HexToDec((char)(dateTime.tm_year - 2000));
    *(pSn+1)  = HexToDec((char)dateTime.tm_mon);
    *(pSn+2)  = HexToDec((char)dateTime.tm_mday);
    *(pSn+3)  = HexToDec((char)dateTime.tm_hour);
    *(pSn+4)  = HexToDec((char)dateTime.tm_min);
    *(pSn+5)  = HexToDec((char)dateTime.tm_sec);

}



/*******************************************************************************
*函数名称：char isCmdResp(const char *pCmd)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
char isCmdResp(const char *pCmd)
{
  char index =0x00;
  ringEvent_t *pRingInfor = NULL;
  lyy_write_t *pInfor = NULL;
  ringPacket_t *pTemp = NULL;

  for(index =0x00; index < MAX_RINGEVENT_COUNT;index++){
    pRingInfor = ringEventGetInfor(index);

    if(pRingInfor->mark.bitByte == 0x00)  continue;

    pTemp = (ringPacket_t*)pRingInfor->pArg;
    pInfor = (lyy_write_t *)pTemp->buff;
    //APP_TRACE_DBG("\r\n  缓冲数据组%s命令应答数据包\r\n",pInfor->packet.attr);
    if(memcmp(pInfor->packet.attr,pCmd,strlen(pCmd))==0x00){
      //APP_TRACE_DBG("\r\n**********************%d缓冲数据队列 %s命令应答数据包\r\n",index,pInfor->packet.attr);
      break;
    }
  }

  if(index < MAX_RINGEVENT_COUNT){
    ringEventEndIndex(index);
    return SET;
  }
  else{
    return RESET;
  }
}

/*******************************************************************************
*函数名称：uint8_t checksum(const char *pTemp,uint16_t len)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
uint8_t lyyChecksum(const char *pTemp,uint16_t len)
{
  uint8_t sum = 0x00;
  char *pDes = (char *)pTemp;
  uint16_t count = len;
  
  while(count--){
    sum ^= *pDes;
    pDes++;
  }
  
  return sum;
}


/*******************************************************************************
*函数名称：int aesEncrypt(const char *pIn,const char *pKey,char *pOut,uint16_t len)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
int aesEncrypt(const char *pIn,const char *pKey,char *pOut,uint16_t len)
{
	uint16_t index =0x00;
	char  *pTemp = NULL;
	uint16_t total = len;
	char  *pSrc = NULL;
	char *pDes = NULL;
	char byte = 16-(total%16);
  
	//满足服务器解析，数据长度为16整数倍的话需再填充16个字节
	total +=16;
	total &= 0xFFF0;

  
	pTemp = memMalloc(2*total);

	if(pTemp == NULL)
		return  0x00;
  
	pSrc = pTemp;
	pDes = pTemp + total;
  
	memset(pSrc,byte,total);
	memset(pDes,0x00,total);
	memcpy(pSrc,pIn,len);
  
	for(index =0x00;index <total;index +=16)
	{
		AES128_ECB_encrypt((const uint8_t *)pSrc,(const uint8_t *)pKey,(uint8_t *)pDes);
		pSrc += 16;
		pDes += 16;
	}
  
	memcpy(pOut,pTemp+total,total);
	memFree(pTemp);
	return total;
}


/*******************************************************************************
*函数名称：int aesDecrypt(const char *pIn,const char *pKey,char *pOut,uint16_t len)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
int aesDecrypt(const char *pIn,const char *pKey,char *pOut,uint16_t len)
{
  char *pSrc = (char *)pIn;
  char *pDes = NULL;
  uint16_t index = 0x00;
  
  if((len %16 )!=0x00)         return 0x00; 
  
  pDes = pOut;
  
  for(index = 0x00;index < len; index +=16) {
    AES128_ECB_decrypt((const uint8_t*)pSrc,(const uint8_t *)pKey,(uint8_t *)pDes);
    pDes += 16;
    pSrc += 16;
  }
  
  return len;
}


/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
uint16_t packet(const char *pAttr,const char *pTemp,uint16_t len,const char repeatFlag)
{
    lyy_write_t *pLyy = NULL;
    uint16_t packetLen = 0x00;
    char *pValue = 0x00;
    
    pLyy = memMalloc(sizeof(lyy_write_t));
    if(pLyy == NULL)            return  0x00;
    
    memset(pLyy,0x00,sizeof(lyy_write_t));
    
    //
    memcpy(pLyy->packet.head,LYY_HEAD_MARK,2);
    memcpy(pLyy->packet.attr,pAttr,2);
    packetLen =  len + 8;
    
    pLyy->packet.len = EndianChange16(packetLen);
    
    //内容
    memcpy(pLyy->packet.id,pSettings->id,8);
    memcpy(pLyy->packet.id +8,pTemp,len);
    
    //校验
    pValue = (char *)(pLyy->packet.id + packetLen);
    *pValue = lyyChecksum(pLyy->packet.attr,packetLen +4);
    packetLen = len +1;
    
    APP_TRACE_DBG("\r\n=============SRC Packet:=================\r\n");
    char *pDes = (char *)pLyy;
    APP_HEX_STRING_DBG(pDes,packetLen+14);

    APP_TRACE_DBG("\r\n");
    
    const char noEncryptTable[][2]={"AL","A5"};
    char index = 0x00;
    char total = sizeof(noEncryptTable)/sizeof(noEncryptTable[0]);
    int retValue = 0x00;
    
    for(index = 0x00;index <total;index++)
    {
    	retValue = memcmp(noEncryptTable[index],pAttr,0x02);

    	if(retValue == 0x00)
    		break;
    }
    
    if(index >= total)
    {
    	packetLen = aesEncrypt(pLyy->packet.id +8,pSettings->key.key,pLyy->packet.id +8,len+1);

    	if(packetLen == 0x00)
    	{
    	}

    	pValue = (pLyy->packet.id +8 + packetLen-1);
    }


    pValue++;
    memcpy(pValue,LYY_TAIL_MARK,2);
    packetLen += 2;
    
    packetLen = packetLen + sizeof(pLyy->packet);
    
   // 调用应用层发送函数    
    if(pLyySend != NULL)
      pLyySend(pLyy,packetLen,repeatFlag);
    
    memFree(pLyy);
    
    return packetLen;
}

#ifdef NDJ_Device

// xuejungong 20180623
/*******************************************************************************
*函数名称：void lyyRssiPacket_NDJ(const char *pTemp,uint16_t len)
*函数说明：获取固件参数应答
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
//void lyyRevsionPacket_ndj(const char *pTemp, uint16_t len)
//{
//    //packet_ndj("AE", (const char *)pSn_id, (const char *)pTemp, len, RESET);
//	packet("AM",(const char *)pTemp,len,RESET);
//}

// xuejungong 20180623
/*******************************************************************************
*函数名称：void lyyDeviceRespPacket(char type,const char *pSn)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
//void lyyDeviceRespPacket_ndj(const char *pTemp, uint16_t len)
//{
//	//packet_ndj("A6", (const char *)pSn_id, (const char *)pTemp, len, RESET);
//	packet("AP",(const char *)pTemp,len,RESET);
//}

// xuejungong 20180623
/*******************************************************************************
*函数名称：void lyyDeviceRespPacket(char type,const char *pSn)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
//void lyyUpgradeRespPacket_ndj(const char *pTemp, uint16_t len)
//{
//	//packet_ndj("AC", (const char *)pSn_id, (const char *)pTemp, len, RESET);
//	packet("AQ",(const char *)pTemp,len,RESET);
//}

/*******************************************************************************
*函数名称：void lyyTermialAnswer(const char *pTemp,uint16_t len)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void lyyTermialAnswer(const char *pTemp,uint16_t len)
{
    packet("AK",(const char *)pTemp,len,RESET);
    memFree((char *)pTemp);
}


/*******************************************************************************
*函数名称：void lyyRevsionPacket_ndj(const char *pTemp)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void lyyRevsionPacket_ndj(const char *pTemp)
{
	ndj_revision_t *pRevsion = NULL;
    ndj_upgrade_t *pSys =ndjInforGet();
    pRevsion = memMalloc(sizeof(ndj_revision_t));

    if(pRevsion == NULL)
    	return;

    memset(pRevsion,0x00,sizeof(ndj_revision_t));
    memcpy(pRevsion->sn_id,pTemp,8);
    pRevsion->mark = 0x00;
    memcpy(pRevsion->revision,pSys->version,14);
    pRevsion->no = EndianChange16(pSys->packetIndex);
    packet("AM",(const char *)pRevsion,sizeof(ndj_revision_t),RESET);
    memFree(pRevsion);
}

/*******************************************************************************
*函数名称：void serverCheckParam_ndj(const char *pTemp)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void serverCheckParam_ndj(const char *pTemp)
{
	 lyycheck_t *pLyy = (lyycheck_t *)(&pTemp[8]);

	 switch(pLyy->opt){

	 case 0x01:  			//查询本地备份程序情况
		 lyyRevsionPacket_ndj(pTemp);
	      break;
	 }
}

/*******************************************************************************
*函数名称：void lyyDeviceRespPacket(char type,const char *pSn)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void lyyDeviceRespPacket_ndj(char type,const char *pSn)
{
	ndj_lyyresp_t *pResp = NULL;

	pResp = memMalloc(sizeof(ndj_lyyresp_t));

	if(pResp == NULL)
		return;

	memcpy(pResp->sn_id,pSn,8);
	memcpy(pResp->sn_cmd,pSn,6);
	pResp->ans = type;
	packet("AP",(const char *)pResp,sizeof(ndj_lyyresp_t),RESET);
	memFree(pResp);
}

/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void upgradeCtrlParse_ndj(const char *pTemp)
{
  //mcuInfor_t mcuInfor;

  ndj_upgrade_t *pSys = ndjInforGet();
  //stmFlashGetInfor(&mcuInfor);

  upgradectrl_t *pUpgrade =(upgradectrl_t *)&pTemp[8];

  memset((char *)pSys,0x00,sizeof(ndj_upgrade_t));

  pSys->packetTotal = EndianChange16(pUpgrade->packet);
  pSys->codeNum = EndianChange16(pUpgrade->total);

  memcpy(pSys->version,pUpgrade->revision,14);

  APP_TRACE_DBG("\r\n=======================升级数据包总数：%d\r\n",pSys->packetTotal);
  APP_TRACE_DBG("\r\n=======================升 级 文件长度：%d\r\n",pSys->codeNum);
  APP_TRACE_DBG("\r\n=======================升 级 软件版本：%s\r\n",pSys->version);

  pSys->packetIndex = 0x00;
  pSys->total = 0x00;

  lyyDeviceRespPacket_ndj(1,pTemp);
}

/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void upgradeDataParse_ndj(const char *pTemp)
{
	ndj_upgrade_t *pSys = ndjInforGet();
	ndj_upgradeans_t *pAns =NULL;
	uint32_t writeAddr = UPGRADE_DATA_ADDR_NDJ;
	int      retValue = 0x00;
	uint16_t index = 0x00;
	char *pRead = NULL;

	uint16_t len = (*pTemp << 0x08);

	len |= *(pTemp+1);
	len -=18;

	upgradedata_t *pUpgrade =(upgradedata_t *)(pTemp+18);//

	index = EndianChange16(pUpgrade->index);

	//其索引是否为连续，数据包长度是否正确
	if(((pSys->packetIndex +1) != index) || (len > UPGRADE_PACKET_SIZE_NDJ))
	{
	   APP_TRACE_DBG("\r\n>>>>>>>>>>>>>>>>>>>>>>>数据包长度错误\r\n");
	   APP_TRACE_DBG("\r\n>>>>>>>>>>>>>>>>>>>>>>>数据包序号：%d\r\n", index);
	   APP_TRACE_DBG("\r\n>>>>>>>>>>>>>>>>>>>>>>>数据包长度：%d\r\n", len);
       memset(&pSys,0x00,sizeof(ndj_upgrade_t));
       lyyErrorPacket(DOWNLOAD_ERR,0x00,4);
       return;
     }

	writeAddr += (index-1) *UPGRADE_PACKET_SIZE_NDJ;

	APP_TRACE_DBG("\r\n>>>>>>>>>>>>>>>>>>>>>>>升级数据索引：%d\r\n",index);
	APP_TRACE_DBG("\r\n>>>>>>>>>>>>>>>>>>>>>>>升级数据长度：%d\r\n",len);
	APP_TRACE_DBG("\r\n>>>>>>>>>>>>>>>>>>>>>>>数据存储地址：%x\r\n",writeAddr);

	retValue = flashWrite(writeAddr,pUpgrade->data,len);

	if(retValue == 0x00)
	{
		APP_TRACE_DBG("\r\n>>>>>>>>>>>>>>>>>>>>>>>数据存储操作失败!\r\n");
		lyyErrorPacket(DOWNLOAD_ERR,0x00,3);
		return;
	}

	pRead = memMalloc(len);

	if(pRead == NULL)
		return;

	memset(pRead,0x00,len);

	flashRead(writeAddr,pRead,len);

	retValue = memcmp(pRead,pUpgrade->data,len);
	if(retValue != 0x00)
	{
		APP_TRACE_DBG("\r\n>>>>>>>>>>>>>>>>>>>>>>>读取数据与写数据内容不一致!\r\n");
		lyyErrorPacket(DOWNLOAD_ERR,0x00,2);
		memFree(pRead);
		return;
	}

	memFree(pRead);

	pSys->packetIndex = index;
	pSys->total += len;

	if((len == (pSys->codeNum %UPGRADE_PACKET_SIZE))||(pSys->packetTotal == index))
	{
		APP_TRACE_DBG("\r\n设备升级成功！固件总长度：%d\r\n",pSys->total);

		memcpy(pSys->mark,UPGRADE_MARK,2);
		pSys->upgrade = SET;
		pSys->saveAddr =UPGRADE_DATA_ADDR_NDJ;

		flashWrite(UPGRADE_INFOR_ADDR_NDJ,(char *)pSys,sizeof(ndj_upgrade_t));

		//xMarkSetValue(MARK_NDJ_UPD, SET);  // 获取到升级数据包标志
		connectSendMsg(MSG_ID_UPGRADE_MSG,0x00, 0x00);  // 传递消息，开始为子设备升级
	}

	//数据包内容校验
	pSys->crc ^= upgradeChecksum(pUpgrade->data,len);

	pAns = memMalloc(sizeof(ndj_upgradeans_t));

	if(pAns ==NULL)
		return;

	memset(pAns,0x00,sizeof(ndj_upgradeans_t));

	memcpy(pAns->sn_id, &pTemp[10], 8);
	pAns->ans = 0x08;
	pAns->no = pUpgrade->index;

	packet("AQ",(const char *)pAns,sizeof(ndj_upgradeans_t),RESET);

	memFree(pAns);
}

#endif


/*******************************************************************************
*函数名称：void lyyNetPacket(const char *pKey,const char *pPass)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void lyyNetPacket(const char *pKey,const char *pPass)
{
   lyynet_t  *pLyyNet = NULL;
   uint8_t  len = strlen(SoftwareVersion.hw);
   uint32_t  sn = EndianChange32(lyySn);

   len += sizeof(lyynet_t);
   pLyyNet = memMalloc(len);
   if(pLyyNet == NULL)                  return;

   memset(pLyyNet,0x00,len);

   //序列号
   memcpy(pLyyNet->sn+2,&sn,4);
   lyySn++;

   //
   memcpy(pLyyNet->key,pKey,16);

   //
   memcpy(pLyyNet->pass,pPass,8);

   //
   memcpy(pLyyNet->pass+8,SoftwareVersion.hw,strlen(SoftwareVersion.hw));

   //
   packet("AL",(const char *)pLyyNet,len,RESET);

   //
   memFree(pLyyNet);

}


/*******************************************************************************
*函数名称：void lyySetLoginMark(char mark)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void lyySetLoginMark(char mark)
{
    if (mark != lyyLoginMark){
       lyyLoginMark = mark;
    } 
}


/*******************************************************************************
*函数名称：char lyyGetLoginMark(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
char lyyGetLoginMark(void)
{
    return lyyLoginMark; 
}

/*******************************************************************************
*函数名称：void lyyLoginPacket(const char *pPass)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void lyyLoginPacket(const char *pPass)
{
   uint16_t    version = 0x00;
   lyylogin_t  *pLyyLogin = NULL;
   char        logicMark = lyyGetLoginMark();
   uint32_t    sn = EndianChange32(lyySn);

   pLyyLogin = memMalloc(sizeof(lyylogin_t));
   if(pLyyLogin == NULL)                  return;

   memset(pLyyLogin,0x00,sizeof(lyylogin_t));

#if defined INTERFACE_DEVICE
   pLyyLogin->login[0]= 0x00;

   switch (logicMark)
   {
   case 0x00:
       pLyyLogin->login[1] = 35;
       break;

   default:
       pLyyLogin->login[1] = logicMark;
   break;
   }
#else
   memcpy(&pLyyLogin->login,LYY_DEVICE_MARK,2);
#endif
   
   //
   version = EndianChange16(SoftwareVersion.value);
   memcpy(&pLyyLogin->version,&version,2);

   //
   memcpy(pLyyLogin->pass,pPass,8);

   //
   packet("A1",(const char *)pLyyLogin,sizeof(lyylogin_t),RESET);

   //
   memFree(pLyyLogin);

}


/*******************************************************************************
*函数名称：void lyyLoginPacket(const char *pPass)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void lyyHeartPacket(char type)
{
   packet("A5",(const char *)&type,1,RESET);
}


/*******************************************************************************
*函数名称：void lyyDeviceRespPacket(char type,const char *pSn)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void lyyDeviceRespPacket(char type,const char *pSn)
{
  lyyresp_t *pResp = NULL;

  pResp = memMalloc(sizeof(lyyresp_t));
  if(pResp == NULL)                     return;

  memcpy(pResp->sn,pSn,6);

  pResp->ans = type;

  packet("A6",(const char *)pResp,sizeof(lyyresp_t),RESET);

  memFree(pResp);

}


/*******************************************************************************
*函数名称：void lyyComeinPacket(const char *pAttr,const uint32_t total,uint16_t inc)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void lyyComeinPacket(const char *pAttr,const uint32_t total,uint16_t inc)
{
    lyyInfor_t *pComeInfor = NULL;
    int retValue = 0x00;
    uint32_t tick =sysGetTick();

    if((lyyPaySn == 0x00)&&(memcmp(pAttr,"A2",2)==0x00)){
      srand(tick);
      lyyPaySn = rand();
    }

    if((lyyGiftSn == 0x00)&&(memcmp(pAttr,"A3",2)==0x00)){
      srand(tick);
      lyyGiftSn = rand();
    }

    uint32_t  sn = EndianChange32(lyyPaySn);

    pComeInfor = memMalloc(sizeof(lyyInfor_t));

    if(pComeInfor == NULL)                      return;

    memset(pComeInfor,0x00,sizeof(lyyInfor_t));

     //序列号
    retValue = memcmp(pAttr,"A4",2);

    if(retValue == 0x00){
      memcpy(pComeInfor->sn,lyyPayID,6);
    }
    else{
      retValue = memcmp(pAttr,"A2",2);
      if(retValue == 0x00){
         sn = EndianChange32(lyyPaySn);
         lyyPaySn++;
         APP_TRACE_DBG("\r\n=========SN:%d 设备投币总数：%d 增量：%d===================\r\n",lyyPaySn,total,inc);
      }
      else{
         sn = EndianChange32(lyyGiftSn);
         lyyGiftSn++;
         APP_TRACE_DBG("\r\n=========SN:%d 设备礼品总数：%d 增量：%d===================\r\n",lyyGiftSn,total,inc);
      }

      memcpy(pComeInfor->sn+2,&sn,4);
    }

    pComeInfor->total = EndianChange32(total);
    pComeInfor->inc = EndianChange16(inc);


   struct tm dateTime;
   getDatetime(&dateTime);

   pComeInfor->year = (dateTime.tm_year-2000);
   pComeInfor->month = dateTime.tm_mon;
   pComeInfor->day = dateTime.tm_mday;
   pComeInfor->hour = dateTime.tm_hour;
   pComeInfor->min = dateTime.tm_min;
   pComeInfor->sec = dateTime.tm_sec;

    packet(pAttr,(const char *)pComeInfor,sizeof(lyyInfor_t),SET);

    memFree(pComeInfor);
}


/*******************************************************************************
*函数名称：void lyyRssiPacket(const char value)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void lyyRssiPacket(const char value)
{
    lyyrssi_t *pRssi = NULL;

    pRssi = memMalloc(sizeof(lyyrssi_t));

    if(pRssi == NULL)                      return;

    memset(pRssi,0x00,sizeof(lyyrssi_t));
    pRssi->value = value;

    packet("AF",(const char *)pRssi,sizeof(lyyrssi_t),RESET);

    memFree(pRssi);
}


/*******************************************************************************
*函数名称：void lyyRssiPacket(const char value)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void lyyRevsionPacket(void)
{
    revision_t *pRevsion = NULL;
    sysInfor_t *pSys =sysInforGet();

    pRevsion = memMalloc(sizeof(revision_t));

    if(pRevsion == NULL)                      return;

    memset(pRevsion,0x00,sizeof(revision_t));

    pRevsion->mark = 0x00;


    memcpy(pRevsion->revision,pSys->upgrade.version,14);

    pRevsion->no = EndianChange16(pSys->upgrade.packetIndex);

    packet("AE",(const char *)pRevsion,sizeof(revision_t),RESET);

    memFree(pRevsion);

}


//
///*******************************************************************************
//*函数名称：void lyyNoteMngPacket(note_t *pNote)
//*函数说明：
//*输入参数：
//*输出参数：
//*返 回 值：
//*其他说明：
//*******************************************************************************/
//void lyyNoteMngPacket(terminalNote_t *pNote)
//{
//    ST_Time dateTime;
//
//
//    note_t *pMessage = NULL;
//    pMessage  = memMalloc(sizeof(note_t));
//    if (pMessage == NULL) {
//        return;
//    }
//
//    //APP_TRACE_DBG("\r\n=========故障上传 ===================\r\n");
//
//    memset(pMessage,0x00,sizeof(note_t));
//
//    getDateTime(&dateTime);
//
//    pMessage->year = (char)(dateTime.year-2000);
//    pMessage->month = dateTime.month;
//    pMessage->day = dateTime.day;
//    pMessage->hour = dateTime.hour;
//    pMessage->min = dateTime.minute;
//    pMessage->sec = dateTime.second;
//
//    memcpy(pMessage->sn,&pMessage->year,6);
//
//    memcpy(pMessage->note,pNote,sizeof(terminalNote_t));
//
//    packet("AH", (const char *)pMessage, sizeof(note_t), SET);
//
//    memFree(pMessage);
//}
//
//
///*******************************************************************************
//*函数名称：void lyyRssiPacket(const char value)
//*函数说明：
//*输入参数：
//*输出参数：
//*返 回 值：
//*其他说明：
//*******************************************************************************/
//void lyySubRevsionPacket(void)
//{
//    revision_t *pRevsion = NULL;
//    sysInfor_t *pSys =sysInforGet();
//
//    pRevsion = memMalloc(sizeof(revision_t));
//
//    if(pRevsion == NULL)                      return;
//
//    memset(pRevsion,0x00,sizeof(revision_t));
//
//    pRevsion->mark = pSys->upgrade.mark;
//
//    memcpy(pRevsion->revision, pSys->upgrade.subVersion, 14);
//
//    pRevsion->no = EndianChange16(pSys->upgrade.subPacketIndex);
//
//    packet("AM",(const char *)pRevsion,sizeof(revision_t),RESET);
//
//    memFree(pRevsion);
//}
//

/*******************************************************************************
*函数名称：void lyyRssiPacket(const char value)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void lyyParamPacket(void)
{
    lyyparam_t *pParam = NULL;
    uint16_t valueTemp = 0x00;

    pParam = memMalloc(sizeof(lyyparam_t));

    if(pParam == NULL)                      return;

    memset(pParam,0x00,sizeof(lyyparam_t));

   pParam->width_1 = pSettings->coin.pulse;
   pParam->width_1 = EndianChange16(pParam->width_1);

   pParam->redial_1 = pSettings->coin.redial;
   pParam->redial_1 = EndianChange16(pParam->redial_1);

   pParam->width_2 = pParam->width_1;
   pParam->redial_2 =pParam->redial_1;

   pParam->width_2 = EndianChange16(SoftwareVersion.value);
   pParam->redial_2 =EndianChange16(SoftwareVersion.value);

   if(coinGetMode()==TRIGER_RISING){
       if (pSettings->coin.value==0) {
           pSettings->coin.value = 0x01;
       }
   }
   else{
       if (pSettings->coin.value==0x01) {
           pSettings->coin.value = 0x00;
       }
   }

   pParam->lever = pSettings->coin.value;
   pParam->bill =  0x00;

   pParam->baud =  pSettings->interface.uart_brate;
   pParam->interface = pSettings->interface.uart_type;

   pParam->monny = 0xFF;

    packet("AA",(const char *)pParam,sizeof(lyyparam_t),RESET);

    memFree(pParam);
}


///*******************************************************************************
//*函数名称：void lyyGpsPacket(const char *pGpsInfor)
//*函数说明：
//*输入参数：
//*输出参数：
//*返 回 值：
//*其他说明：
//*******************************************************************************/
//void lyyGpsPacket(const char *pGpsInfor)
//{
//    lyygps_t *pGps = NULL;
//
//    pGps = memMalloc(sizeof(lyygps_t));
//
//    if(pGps == NULL)                      return;
//
//    memset(pGps,0x00,sizeof(lyygps_t));
//
//    pGps->result = 0x01;
//
//    memcpy(pGps->gps,pGpsInfor,strlen(pGpsInfor));
//
//    packet("A9",(const char *)pGps,sizeof(lyygps_t),RESET);
//
//    memFree(pGps);
//}
//

/*******************************************************************************
*函数名称：void lyyRssiPacket(const char value)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void lyyIpPacket(void)
{
    lyyip_t *pIp = NULL;

    pIp = memMalloc(sizeof(lyyip_t));

    if(pIp == NULL)                      return;

    memset(pIp,0x00,sizeof(lyyip_t));

    pIp->result = 0x01;

    memcpy(pIp->ip,pSettings->server.ip,strlen(pSettings->server.ip));

    packet("A8",(const char *)pIp,sizeof(lyyip_t),RESET);

    memFree(pIp);
}


///*******************************************************************************
//*函数名称：void lyyRssiPacket(const char value)
//*函数说明：
//*输入参数：
//*输出参数：
//*返 回 值：
//*其他说明：
//*******************************************************************************/
//void lyyCCIDPacket(void)
//{
//    lyyccid_t *pCCID = NULL;
//
//    moduleInfor_t *pInfor= getModuleInfor();
//
//    pCCID = memMalloc(sizeof(lyyccid_t));
//
//    if(pCCID == NULL)                      return;
//
//    memset(pCCID,0x00,sizeof(lyyccid_t));
//
//    pCCID->result = 0x01;
//
//    memcpy(pCCID->ccid,pInfor->ccid,20);
//
//    packet("A7",(const char *)pCCID,sizeof(lyyccid_t),RESET);
//
//    memFree(pCCID);
//}
//

/*******************************************************************************
*函数名称：void lyyErrorPacket(uint8_t errcode,uint16_t errdata)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void lyyErrorPacket(uint8_t errcode,uint8_t bill,uint16_t errdata)
{
    lyyerror_t *pError = NULL;
    uint32_t  sn = EndianChange32(lyySn);

    pError = memMalloc(sizeof(lyyerror_t));

    if(pError == NULL)                      return;

    memset(pError,0x00,sizeof(lyyerror_t));

    //序列号
    memcpy(pError->sn+2,&sn,4);
    lyySn++;

    //
    pError->code = errcode;

    //
    pError->bill = bill;

    //
    pError->error = EndianChange16(errdata);


    struct tm dateTime;
    getDatetime(&dateTime);

    pError->year = dateTime.tm_yday-2000;
    pError->month = dateTime.tm_mon;
    pError->day = dateTime.tm_mday;
    pError->hour = dateTime.tm_hour;
    pError->min = dateTime.tm_min;
    pError->sec = dateTime.tm_sec;


    packet("AG",(const char *)pError,sizeof(lyyerror_t),RESET);

    memFree(pError);
}


///*******************************************************************************
//*函数名称：
//*函数说明：
//*输入参数：
//*输出参数：
//*返 回 值：
//*其他说明：
//*******************************************************************************/
//uint8_t lyyCmpPaySn(const char * pSn)
//{
//  char index = 0x00;
//  char total = sizeof(lyyRingSn)/sizeof(lyyRingSn[0]);
//  int  retValue = 0x00;
//
//  for(index = 0;index < total;index++){
//    retValue = memcmp(lyyRingSn[index],pSn,6);
//    if(retValue == 0x00){
//      return SET;
//    }
//  }
//
//  memcpy(lyyRingSn[lyyRingIndex++],pSn,6);
//
//  lyyRingIndex %= total;
//
//  return RESET;
//}

/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void serverCheckParam(const char *pTemp)
{
   lyycheck_t *pLyy = (lyycheck_t *)(pTemp);
   //moduleInfor_t *pInfor= getModuleInfor();

   switch(pLyy->opt){

   case 0x01:   //查询本地配置参数
     lyyParamPacket();
     break;

   case 0x02:  //查询GPS定位信息(G510)
     //cellGetLocInfor(NULL);
     break;

   case 0x03:  //查询设备IP
     lyyIpPacket();
     break;

   case 0x04:   //查询设备上投币数、退礼数、上分数
     break;

   case 0x07:  //查询CCID
     //lyyCCIDPacket();
     break;

   case 0x08:  //查询本地备份程序情况
     lyyRevsionPacket();
     break;

   case 0x09:   //查询信号强度
      lyyRssiPacket(wifiRssiValue);
     break;

   case 0x0A:   //查夜子设备软件版本
     //connectSendMsg(MSG_ID_GET_REVSION_MSG,0x00,0x00);
     break;

   default:
     break;
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
void serverResetParam(const char *pTemp)
{
   lyyrest_t *pInfor = (lyyrest_t *)pTemp;

   switch(pInfor->opt){

   case 0x01:
     pSettings->note.coin_total = EndianChange32(pInfor->value);
     //更新保存数据
     paramSave();
     break;

   case 0x02:
     pSettings->note.gift_total = EndianChange32(pInfor->value);
     //更新保存数据
     paramSave();
     break;

   case 0x03:
     pSettings->note.pay_total = EndianChange32(pInfor->value);
     //更新保存数据
     paramSave();
     break;

   default:
     break;
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
void serverAns(const char *pTemp)
{
  sysInfor_t *pSysfor = sysInforGet();
  lyyresp_t *pResp = (lyyresp_t *) pTemp;

  switch(pResp->ans){

  case 0x01:                          //登录成功
    isCmdResp("A1");
    xMarkSetValue(MARK_DEVICE_LOGIN,SET);
    xTmrStop(TMR_RDY);
    break;

  case 2:                             //本地投币
    isCmdResp("A2");
    break;

  case 3:                             //退礼记录
    isCmdResp("A3");
    break;

  case 4:                             //移动支付上分接受
    isCmdResp("A4");
    break;

  case 5://故障
    isCmdResp("AG");
    break;

  case 6://终端运行参数
    isCmdResp("AH");
    break;

  case 7://通信日志
    isCmdResp("AI");
    break;

  case 11:
    isCmdResp("AL");
    if(strlen(pSysfor->key) > 0x00){
      xMarkSetValue(MARK_KEY_NET,SET);
      memcpy(pSettings->key.key,pSysfor->key,16);
      memcpy(pSettings->key.pass,pSysfor->pass,8);
      //保存数据
      paramSave();
      //xMarkSetValue(MARK_SAVE_PARAM,SET);
      //saveParamEvent();
    }
    xTmrStop(TMR_RDY);
    break;

  default:
    break;
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
void upgradeCtrlParse(const char *pTemp)
{
  sysInfor_t *pSys = sysInforGet();

  upgradectrl_t *pUpgrade =(upgradectrl_t *)pTemp;

  memset(&pSys->upgrade,0x00,sizeof(upgrade_t));

  pSys->upgrade.packetTotal = EndianChange16(pUpgrade->packet);
  pSys->upgrade.codeNum = EndianChange16(pUpgrade->total);

  memcpy(pSys->upgrade.version,pUpgrade->revision,14);

  APP_TRACE_DBG("\r\n=======================升级数据包总数：%d\r\n",pSys->upgrade.packetTotal);
  APP_TRACE_DBG("\r\n=======================升 级 文件长度：%d\r\n",pSys->upgrade.codeNum);
  APP_TRACE_DBG("\r\n=======================升 级 软件版本：%s\r\n",pSys->upgrade.version);

  pSys->upgrade.packetIndex = 0x00;
  pSys->upgrade.total = 0x00;

  lyyDeviceRespPacket(7,pTemp);

}



///*******************************************************************************
//*函数名称：
//*函数说明：
//*输入参数：
//*输出参数：
//*返 回 值：
//*其他说明：
//*******************************************************************************/
//void upgradeSubCtrlParse(const char *pTemp)
//{
//  sysInfor_t *pSys = sysInforGet();
//
//  upgradectrl_t *pUpgrade =(upgradectrl_t *)pTemp;
//
//  memset(&pSys->upgrade,0x00,sizeof(upgrade_t));
//
//  pSys->upgrade.subPacketTotal = EndianChange16(pUpgrade->packet);
//  pSys->upgrade.subCodeNum = EndianChange16(pUpgrade->total);
//
//  memcpy(pSys->upgrade.subVersion,pUpgrade->revision,14);
//
//  APP_TRACE_DBG("\r\n=======================子设备升级数据包总数：%d\r\n",pSys->upgrade.subPacketTotal);
//  APP_TRACE_DBG("\r\n=======================子设备升 级 文件长度：%d\r\n",pSys->upgrade.subCodeNum);
//  APP_TRACE_DBG("\r\n=======================子设备升 级 软件版本：%s\r\n",pSys->upgrade.subVersion);
//
//  pSys->upgrade.subPacketIndex = 0x00;
//  pSys->upgrade.subTotal = 0x00;
//
//  lyyDeviceRespPacket(12,pTemp);
//
//}
//

/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
uint32_t upgradeChecksum(const char *pTemp,uint16_t len)
{
  uint32_t  sum = 0x00;
  char *pDes = (char *)pTemp;
  uint16_t count = len;

  while(count--){
    sum ^= *pDes;
    pDes++;
  }

  return sum;
}


/*******************************************************************************
*函数名称：void upgradeDataParse(const char *pTemp)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void upgradeDataParse(const char *pTemp)
{
    sysInfor_t *pSys = sysInforGet();
    upgradeans_t *pAns =NULL;
    uint32_t writeAddr = getUpgradAddr();
    int      retValue = 0x00;
    uint16_t index = 0x00;
    char *pRead = NULL;
    uint16_t writeLen = 0x00;

    uint16_t len = (*pTemp << 0x08);

    len |= *(pTemp+1);
    len -=10;

    upgradedata_t *pUpgrade =(upgradedata_t *)(pTemp+10);

    index = EndianChange16(pUpgrade->index);

    //其索引是否为连续，数据包长度是否正确
    if(((pSys->upgrade.packetIndex +1) != index) ||
       (len > UPGRADE_PACKET_SIZE))  {
         memset(&pSys->upgrade,0x00,sizeof(upgrade_t));
         lyyErrorPacket(DOWNLOAD_ERR,0x00,4);
         return;
       }

  writeAddr += (index-1) *UPGRADE_PACKET_SIZE;

  APP_TRACE_DBG("\r\n>>>>>>>>>>>>>>>>>>>>>>>升级数据索引：%d\r\n",index);
  APP_TRACE_DBG("\r\n>>>>>>>>>>>>>>>>>>>>>>>升级数据长度：%d\r\n",len);
  APP_TRACE_DBG("\r\n>>>>>>>>>>>>>>>>>>>>>>>数据存储地址：%X\r\n",writeAddr);

  writeLen = flashWrite(writeAddr,pUpgrade->data,len);
  if(writeLen != len){
    APP_TRACE_DBG("\r\n>>>>>>>>>>>>>>>>>>>>>>>数据存储操作失败!\r\n");
    lyyErrorPacket(DOWNLOAD_ERR,0x00,3);
    return;
  }

  pRead = memMalloc(len);
  if(pRead == NULL)                          return;
  memset(pRead,0x00,len);

  flashRead(writeAddr,pRead,len);

  APP_TRACE_DBG("\r\n %X Read Packet:",writeAddr);
  APP_HEX_STRING_DBG(pRead,len);
  APP_TRACE_DBG("\r\n");

  retValue = memcmp(pRead,pUpgrade->data,len);
  if(retValue != 0x00){
    APP_TRACE_DBG("\r\n>>>>>>>>>>>>>>>>>>>>>>>读取数据与写数据内容不一致!\r\n");
    lyyErrorPacket(DOWNLOAD_ERR,0x00,2);
    memFree(pRead);
    return;
  }

  memFree(pRead);


  pSys->upgrade.packetIndex = index;
  pSys->upgrade.total += len;

  if((len == (pSys->upgrade.codeNum % UPGRADE_PACKET_SIZE))||
      (index == pSys->upgrade.packetTotal)) {
      //writeUpgradeInfor();
      pSys->upgrade.upgrade = TRUE;
      APP_TRACE_DBG("\r\n设备升级成功！固件总长度：%d\r\n",pSys->upgrade.total);
      flashWrite(UPGRADE_INFOR_ADDR,(char *)&pSys->upgrade,sizeof(upgrade_t));
  }

    //数据包内容校验
    pSys->upgrade.crc ^= upgradeChecksum(pUpgrade->data,len);


    pAns = memMalloc(sizeof(upgradeans_t));
    if(pAns ==NULL)                       return;

    memset(pAns,0x00,sizeof(upgradeans_t));

    pAns->ans = 0x08;

    pAns->no = pUpgrade->index;

    packet("AC",(const char *)pAns,sizeof(upgradeans_t),RESET);

    memFree(pAns);
}


///*******************************************************************************
//*函数名称：void upgradeDataSubParse(const char *pTemp)
//*函数说明：
//*输入参数：
//*输出参数：
//*返 回 值：
//*其他说明：
//*******************************************************************************/
//void upgradeDataSubParse(const char *pTemp)
//{
//    sysInfor_t *pSys = sysInforGet();
//    upgradeans_t *pAns =NULL;
//    uint32_t writeAddr = UPGRADE_DATA_ADDR;
//    int      retValue = 0x00;
//    uint16_t index = 0x00;
//    char *pRead = NULL;
//    uint16_t writeLen = 0x00;
//
//    uint16_t len = (*pTemp << 0x08);
//
//    len |= *(pTemp+1);
//    len -=10;
//
//    upgradedata_t *pUpgrade =(upgradedata_t *)(pTemp+10);
//
//    index = EndianChange16(pUpgrade->index);
//
//    //其索引是否为连续，数据包长度是否正确
//    if(((pSys->upgrade.subPacketIndex +1) != index) ||
//       (len > UPGRADE_PACKET_SIZE))  {
//         //memset(&pSys->upgrade,0x00,sizeof(upgrade_t));
//         lyyErrorPacket(DOWNLOAD_ERR,0x00,4);
//         return;
//       }
//
//
//  APP_TRACE_DBG("\r\n>>>>>>>>>>>>>>>>>>>>>>>升级数据索引：%d\r\n",index);
//  APP_TRACE_DBG("\r\n>>>>>>>>>>>>>>>>>>>>>>>升级数据长度：%d\r\n",len);
//
//
//  //redLedFlash(100,100,1);
//
//
//  pSys->upgrade.subPacketIndex = index;
//  pSys->upgrade.subTotal += len;
//
//  if(len == (pSys->upgrade.subCodeNum %UPGRADE_PACKET_SIZE)){
//      APP_TRACE_DBG("\r\n设备升级成功！固件总长度：%d\r\n",pSys->upgrade.total);
//  }
//
//
//
//    pAns = memMalloc(sizeof(upgradeans_t));
//    if(pAns ==NULL)                       return;
//
//    memset(pAns,0x00,sizeof(upgradeans_t));
//
//    pAns->ans = 0x09;
//
//    pAns->no = pUpgrade->index;
//
//    packet("AC",(const char *)pAns,sizeof(upgradeans_t),RESET);
//
//    memFree(pAns);
//}
//


/*******************************************************************************
*函数名称：void payEventDone(char *pSn,char value)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void payEventDone(char *pSn,char value)
{
    //复制支付命令ID
    memcpy(lyyPayID,pSn,6);

#if defined INTERFACE_DEVICE
     connectSendMsg(MSG_ID_PAY_MSG,value,0x00);
#else
    //coinSimulateValue(value);
    pSettings->note.pay_total += value;
#endif
}


/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void cloudPayParse(const char *pTemp)
{
    char payValue =  0x00;
    lyycloudpay_t *pPay = (lyycloudpay_t *)pTemp;
    uint16_t value = EndianChange16(pPay->value);

    lyyDeviceRespPacket(1,pPay->sn);

    payValue = writePayEvent(pPay->sn,value);

    if(payValue > 0x00){
      APP_TRACE_DBG("\r\n支付队列已经存在!\r\n");
    }
    else{
      APP_TRACE_DBG("\r\n进入支付队列!\r\n");
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
void lyyParamParse(const char *pTemp)
{
    //char index = 0x00;
    lyyparam_t *pParam = (lyyparam_t *)pTemp;

    pSettings->coin.pulse = EndianChange16(pParam->width_1);
    pSettings->coin.redial = EndianChange16(pParam->redial_1);

    pSettings->coin.value = (leverTypedef)pParam->lever;

#if defined INTERFACE_DEVICE
    pSettings->interface.uart_type = pParam->interface;

    if (pSettings->interface.uart_brate != pParam->baud) {
        pSettings->interface.uart_brate = pParam->baud;
        connectBaudrateInit();
    }
#endif

    //保证参数
    paramSave();

    lyyDeviceRespPacket(5,(const char *)&pParam->width_1);
}


/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void lyySyncTimeParse(const char *pTemp)
{
    lyysync_t *pTime = (lyysync_t *)pTemp;

    APP_TRACE_DBG("\r\n服务器同步时间参数：%02d-%02d-%02d %02d:%02d:%02d\r\n",
                  pTime->year,pTime->mon,pTime->day,pTime->hour,pTime->min,pTime->sec);

    setDatetime(2000+pTime->year,pTime->mon,pTime->day,pTime->hour,pTime->min,pTime->sec);

}


/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void lyyDomainNameParse(const char *pTemp)
{
    char index = 0x00;

    uint16_t len = (*pTemp << 0x08);
    len |= *(pTemp+1);

    lyysetip_t *pServer = (lyysetip_t *)(pTemp+10);

    //服务器IP地址不一样
    if(memcmp(pSettings->server.ip,pServer->ip,len-8)!=0x00){

      memset(pSettings->server.ip,0x00,sizeof(pSettings->server.ip));

      memcpy(pSettings->server.ip,pServer->ip,len-8);

      //重新上传通信密码及登录数据包
      xMarkSetValue(MARK_KEY_NET,RESET);
      xMarkSetValue(MARK_DEVICE_LOGIN,RESET);

      //保存数据
      paramSave();
    }

    lyyDeviceRespPacket(4,(const char *)pServer->ip);

}



///*******************************************************************************
//*函数名称：
//*函数说明：
//*输入参数：
//*输出参数：
//*返 回 值：
//*其他说明：
//*******************************************************************************/
//void lyyTermialAnswer(const char *pTemp,uint16_t len)
//{
//    packet("AK",(const char *)pTemp,len,RESET);
//}
//
//
///*******************************************************************************
//*函数名称：
//*函数说明：
//*输入参数：
//*输出参数：
//*返 回 值：
//*其他说明：
//*******************************************************************************/
//void lyyTermialParamAnswer(connectPacket_t  *pConnect)
//{
//    lyyTermialParam_t *pConfig = NULL;
//
//    pConfig = memMalloc(sizeof(lyyTermialParam_t));
//
//    if (pConfig == NULL)        return;
//
//    lyyRondomSn(pConfig->packet.sn);
//    pConfig->packet.device = lyyGetLoginMark();
//
//    pConfig->packet.device = EndianChange16(pConfig->packet.device);
//    pConfig->packet.len = pConnect->packet.len -3;
//    memcpy(&pConfig->packet.len + 1, &pConnect->packet.cmd+1,pConfig->packet.len);
//
//    packet("AJ", (const char *)pConfig, sizeof(pConfig->packet)+pConfig->packet.len, RESET);
//
//    memFree(pConfig);
//}
//
//
//
//
//
//



/*******************************************************************************
*函数名称：void lyyParse(const char *pTemp,uint16_t len)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void lyyParse(const char *pTemp,uint16_t len)
{
	int retValue = 0x00;
	uint8_t checksum = 0x00;
	uint8_t chksum = 0x00;
	uint16_t crc = 0x00;
	uint16_t crc_value = 0x00;
	char *pDes = NULL;
	uint16_t packetLen = 0x00;
	lyy_read_t *pLyy = (lyy_read_t *)pTemp;

	retValue = memcmp(pLyy->packet.head,LYY_HEAD_MARK,strlen(LYY_HEAD_MARK));

	if(retValue != 0x00)
		return;

	if(pLyy->packet.attr[0] != 'B')
		return;

	APP_TRACE_DBG("\r\n======================%02s======================\r\n",pLyy->packet.attr);
	//设备ID是否正确
	retValue = memcmp(pLyy->packet.id,pSettings->id,8);

	if(retValue != 0x00)
		return;

	packetLen = EndianChange16(pLyy->packet.len);
  //数据校验是否正确
	if((pLyy->packet.attr[1] != 'B')&&(pLyy->packet.attr[1] != 'F'))
	{
		checksum = *((char *)(pLyy->packet.id + packetLen));
		chksum = lyyChecksum(pLyy->packet.attr,packetLen+4);

		if(chksum != checksum)
			return;
	}
	else
	{
		crc = *((uint16_t *)(pLyy->packet.id + packetLen));
		crc_value = memoryCRC(pLyy->packet.attr,packetLen+4);
		crc_value = EndianChange16(crc_value);

		if(crc_value != crc)
		{
			APP_TRACE_DBG("\r\n*********************************CRC Error!\r\n");
			return;
		}
	}

	switch(pLyy->packet.attr[1])
	{
		case '1':           //移动支付命令
			cloudPayParse(pLyy->packet.id + 8);
		break;

		case '2':            //时间同步
			lyySyncTimeParse(pLyy->packet.id + 8);
		break;

		case '3':            //域名修改
			lyyDomainNameParse((const char *)&pLyy->packet.len);
		break;

		case '4':            //控制板参数设置
			lyyParamParse(pLyy->packet.id + 8);
		break;

		case '5':            //服务器心跳包
#if defined INTERFACE_DEVICE
      connectSendMsg(MSG_ID_PAY_HEART_MSG,SET,0x00);
#else
      		lyyHeartPacket(1);
#endif
      	break;

		case '6':            //重置投币数、退礼数、上分数
			serverResetParam(pLyy->packet.id + 8);
		break;

		case'7':          //查询指令
			serverCheckParam(pLyy->packet.id + 8);
		break;

		case'8':         //应答数据包
			serverAns(pLyy->packet.id + 8);
		break;

		case '9':            //测试上分
		break;

		case 'A':            //升级控制
			upgradeCtrlParse(pLyy->packet.id + 8);
		break;

		case 'B':            //升级数据流
			upgradeDataParse((const char *)&pLyy->packet.len);
		break;

// xuejungong 20180623 add
#ifdef NDJ_Device

		case 'C':             //终端操作
			pDes = memMalloc(packetLen-16);

			if (pDes)
			{
				memcpy(pDes, pLyy->packet.id+16, packetLen-16);
				connectSendMsg(MSG_ID_OPT_MSG,(uint32_t)pDes,packetLen-16);
			}

		break;

		case 'E':
			APP_TRACE_DBG("\r\n********************************BE_NDJ_REC!==xuejungong===\r\n");
			upgradeCtrlParse_ndj((const char*)&pLyy->packet.id+8);
		break;

		case 'F':
			APP_TRACE_DBG("\r\n********************************BF_NDJ_REC!==xuejungong===\r\n");
			upgradeDataParse_ndj((const char*)&pLyy->packet.len);
		break;

		case 'G':
			APP_TRACE_DBG("\r\n********************************BG_NDJ_REC!==xuejungong===\r\n");
			serverCheckParam_ndj((const char*)&pLyy->packet.id+8);
		break;

#endif

		default:
		break;
  }
}

/*******************************************************************************
*函数名称：void lyyPacketCallback(const char *pBuff,uint16_t len)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void lyyPacketCallback(const char *pBuff,uint16_t len)
{
	char *pTemp = (char *)pBuff;
	char *pDecode = NULL;
	char *pHead = NULL;
	uint16_t packetLen = 0x00;
	lyy_read_t *pLyy = NULL;
	uint16_t i = 0x00;
	sysInfor_t *pSysInfor = sysInforGet();

	pDecode = memMalloc(len);
	if(pDecode == NULL)
		return;
	//数据全部复制
	memcpy(pDecode,pBuff,len);
	pHead = pDecode;

PRO_LYY:
	pHead = strstr(pHead,LYY_HEAD_MARK);

	if(pHead != NULL)
	{
		pLyy = (lyy_read_t *)pHead;
  		packetLen = EndianChange16(pLyy->packet.len);
  		packetLen -= sizeof(pLyy->packet.id);

  		if((packetLen % 16) == 0x00)
  			packetLen += 16;

  		packetLen += 15;
  		packetLen &= 0xFFF0;

  		aesDecrypt(pHead+sizeof(pLyy->packet),pSysInfor->key,pHead+sizeof(pLyy->packet),packetLen);

  		//接收数据处理
  		packetLen += (sizeof(pLyy->packet)+2);

  		//解密后完整数据包打印
  		pTemp = pHead;
  		APP_TRACE_DBG("\r\n %d Decrypt Packet:",packetLen);
  		APP_HEX_STRING_DBG(pTemp,packetLen);
  		APP_TRACE_DBG("\r\n");

  		//数据包处理
  		lyyParse(pHead,packetLen);

  		if(packetLen < len)
  		{
  			pHead = pHead+ packetLen;
  			APP_TRACE_DBG("\r\n ---------------Mutil packet------------ \r\n");
  			goto PRO_LYY;
  		}
	}
  //
	memFree(pDecode);
}


