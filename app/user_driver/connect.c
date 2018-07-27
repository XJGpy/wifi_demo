/*******************                    �� �� �� �� �� ˾ *********************/
/* �������ƣ�					                      	      */
/* �ļ����ƣ�   					      		      */
/* ��    �ߣ�			 					      */
/* ������ڣ�								      */
/* ��ǰ�汾��						      		      */
/* ���¼�¼: 								      */
/******************************************************************************/
#include "../include/user_config.h"

//xTimerHandle                    ndjCheckTime;               // 10���Ӷ�ʱ�������ڻ�ȡndj����
//TimerHandle_t                    packetTime;
static pConnect_f               pConncetFunc;
static char                     rxBuff[MAX_CONNECT_PACKET_SIZE];
static xSemaphoreHandle         connectSemaphoreId;
static char                     connectSn;
static const char               *pQRcode ="http://m.leyaoyao.com/lyy/t/";
static char                     connectLoginMark = 0x00;   //������ݸ�ʽ
static char                     connectPhoneDisp = 0x00;
static char                     connectCmd;
static xQueueHandle             connectQueue;

static char                     ndj_CheckCount=0;
static char						ndj_SnID[10]={0x00,};  // xuejungong 20180628 ȫ�ֱ����������ж϶�Ӧ��Ť�����豸Ψһ��

// xuejungong 20180614 add for get version from server when need to update.
char g_SW_VER[14]={0x00,};
//static SemaphoreHandle_t        connectSemaphoreId_NDJ;
//
//static lyyTermialOption_t   connectOptionPacket,*pOptionPacket = &connectOptionPacket;
//
//static terminalInfor_t      terminalInfor,*pTerminalInfor =&terminalInfor;
//static terminalNote_t       terminalNote,*pTerminalNote = &terminalNote;
//
//static wwjAccount_t         wwjAccountMng,*pWwjAccountMng =&wwjAccountMng;
//static wwjAccountInc_t      wwjAccountInc,*pWwjAccountInc = &wwjAccountInc;
//static wwjAccountBack_t     wwjAccountBackMng,*pWwjAccountBackMng = &wwjAccountBackMng;
//static wwjAccountBack_t     wwjAccountBackLastMng,*pWwjAccountBackLastMng = &wwjAccountBackLastMng;
//
//static exChangeAccount_t    exChangeAccountMng,*pExchangeAccountMng = &exChangeAccountMng;
//static exChangeAccountInc_t exChangeInc,*pExchangeInc =&exChangeInc;

// xuejungong 20180615 add ��������
char connectResetTermial_NDJ(char id);
char connectPay_NDJ(char id);
char connectReplyRefund_NDJ(char id, uint8_t mode);
//static int packet_NDJ(char index, uint16_t id, char cmd, const char *pTemp, uint16_t len, char repeatFlag);
static void NDJ_Delay(int tms);
//static void NDJ_StopTimer(void);
//
//static void NDJ_CreateTimer(void);
//static void NDJ_ReStartTimer(void);


/*******************************************************************************
*�������ƣ�void rondomSn(char *pSn)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
void rondomSn(char *pSn)
{
	struct tm  dateTime;
	static char mark = 0;
	static int addval = 0;

	getDatetime(&dateTime);

    switch(mark)
    {
    case 0:
        *pSn      = HexToDec((char)(dateTime.tm_year - 2000));
        *(pSn+1)  = HexToDec((char)dateTime.tm_mon);
        *(pSn+2)  = HexToDec((char)dateTime.tm_mday);
        *(pSn+3)  = HexToDec((char)dateTime.tm_hour);
        *(pSn+4)  = HexToDec((char)dateTime.tm_min);
        *(pSn+5)  = HexToDec((char)dateTime.tm_sec);
    	mark++;
    	break;

    case 1:
        *pSn      = HexToDec((char)dateTime.tm_mon);
        *(pSn+1)  = HexToDec((char)dateTime.tm_mday);
        *(pSn+2)  = HexToDec((char)(dateTime.tm_year - 2000));
        *(pSn+3)  = HexToDec((char)dateTime.tm_hour);
        *(pSn+4)  = HexToDec((char)dateTime.tm_min);
        *(pSn+5)  = HexToDec((char)dateTime.tm_sec);
    	mark++;
    	break;

    case 2:
        *pSn      = HexToDec((char)dateTime.tm_sec);
        *(pSn+1)  = HexToDec((char)dateTime.tm_mday);
        *(pSn+2)  = HexToDec((char)dateTime.tm_min);
        *(pSn+3)  = HexToDec((char)dateTime.tm_mon);
        *(pSn+4)  = HexToDec((char)(dateTime.tm_year - 2000));
        *(pSn+5)  = HexToDec((char)dateTime.tm_hour);
    	mark++;
    	break;

    case 3:
		*pSn      = HexToDec((char)dateTime.tm_hour);
		*(pSn+1)  = HexToDec((char)dateTime.tm_mon);
		*(pSn+2)  = HexToDec((char)dateTime.tm_min);
		*(pSn+3)  = HexToDec((char)dateTime.tm_mday);
		*(pSn+4)  = HexToDec((char)dateTime.tm_sec);
		*(pSn+5)  = HexToDec((char)(dateTime.tm_year - 2000));
    	mark = 0;
       	break;
    }

    *(pSn+5) += addval;
    addval++;
    if(addval>=0xfffffffe)
    {
    	addval = 0;
    }
}

// xuejungong 20180621 add
/*******************************************************************************
*�������ƣ�void connectSendWait(void)
*����˵����ÿ�η���ǰ��ʱһ�����ʱ��
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
//static void connectSendWait(void)
//{
//	uint16_t time=0;
//	tick_t *pTick = getTickInfor();
//	srand(pTick->tick);
//	//time = randomTime();
//	time = rand()%200;       // xuejungong 20180626  ��200�ڵ������
//	vTaskDelay(time/portTICK_RATE_MS);
//}

// xuejungong 20180704 add
/*******************************************************************************
*�������ƣ�void connectSendWait(void)
*����˵����ÿ�η���ǰ��ʱһ�����ʱ��
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
//static char randnum(void)
//{
//	char time=0;
//	tick_t *pTick = getTickInfor();
//	srand(pTick->tick);
//	//time = randomTime();
//	time = rand()%100;       // xuejungong 20180626  ��200�ڵ������
//	return time;
//}

/*******************************************************************************
*�������ƣ�
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
//static void connectDone(xEventParam_t *pEvent)
//{
//    char *pBuff = NULL;
//
//    APP_TRACE_DBG("\r\n--------------From_NDJ_CONNECT REC!-----------\r\n");
//
//    uint16_t count = xSerialRxCount((UARTNumTypeDef)pEvent->event.index);
//
//    if(count > 0x00){
//      count++;
//      pBuff = memMalloc(count);
//      if(pBuff !=NULL){
//        memset(pBuff,0x00,count);
//        xSerialRead((UARTNumTypeDef)pEvent->event.index,pBuff,count-1);
//
//        APP_TRACE_DBG("\r\n %d Connect Uart RX:",getTick());
//        APP_HEX_STRING_DBG(pBuff,count);
//        APP_TRACE_DBG("\r\n");
//
//        //xuejungong 20180613  ��������
//        connectParse(pBuff,count);
//
//        memFree(pBuff);
//      }
//    }
//}

/*******************************************************************************
*�������ƣ�static void packetTimeCallback(xTimerHandle pxTimer) 
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
//static void packetTimeCallback(xTimerHandle pxTimer)
//{
//    xEventSend(EVENT_HIGH,connectDone,CONNECT_UART_NUM,0x00);
//}

/*******************************************************************************
*�������ƣ�
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
//static void connectRxIdleCallback(void)
//{
//    portBASE_TYPE taskWoken = pdFALSE;
//    xTimerResetFromISR(packetTime,&taskWoken);
//    portEND_SWITCHING_ISR(taskWoken);
//}


/*******************************************************************************
*�������ƣ�void connectBaudInit(pConnect_f callback)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
//void connectBaudInit(void)
//{
//    UART_WaitTxFifoEmpty(UART0);
//
//    UART_ConfigTypeDef uart_config;
//    uart_config.baud_rate    = BIT_RATE_38400;
//    uart_config.data_bits     = UART_WordLength_8b;
//    uart_config.parity          = USART_Parity_None;
//    uart_config.stop_bits     = USART_StopBits_1;
//    uart_config.flow_ctrl      = USART_HardwareFlowControl_None;
//    uart_config.UART_RxFlowThresh = 120;
//    uart_config.UART_InverseMask = UART_None_Inverse;
//    UART_ParamConfig(UART0, &uart_config);
//
//    ETS_UART_INTR_ENABLE();
//}



/*******************************************************************************
*�������ƣ�void connectSerialHandle(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
//void connectSerialHandle(void)
//{
//  int retValue = 0x00;
//  retValue=xSerialHandle(CONNECT_UART_NUM);
//
//  if(retValue == INT_RX_IDLE){
//    __no_operation();
//  }
//
//  if(retValue == INT_TX_IDLE){
//    __no_operation();
//  }
//}



/*******************************************************************************
*�������ƣ�void connectInit(pConnect_f callback)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
void connectInit(pConnect_f callback)
{
	//connectBaudInit();
    //packetTime = xTimerCreate("packetTime",MAX_PACKET_TIME/portTICK_RATE_MS,pdFALSE,(void *)0,packetTimeCallback);
     
    if(callback != NULL){
      pConncetFunc = callback;
    }

    vSemaphoreCreateBinary(connectSemaphoreId);
    if(connectSemaphoreId != NULL){
    	xSemaphoreTake(connectSemaphoreId,portMAX_DELAY);
    }
  
    // xuejungong 20180626 for timer check ndj
    //NDJ_CreateTimer();

    xMarkSetValue(MARK_NDJ_UPD,SET);  // �����жϱ�־
    xTmrRestart(TMR_NDJ_CHECK);      // xuejungong 20180718 add timer for check ndj status
}


/*******************************************************************************
*�������ƣ�
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
static char checkSum(const char *pData,uint16_t Len)
{
    uint16_t i=0;
    char temp = 0;

    for(i=0;i<Len;i++){
        temp += *pData++;
    }

    return temp;
}


/*******************************************************************************
*�������ƣ�static char connectChksum(const char *pTemp,uint16_t len)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
static char connectChksum(const char *pTemp,uint16_t len)
{
    char chksum = 0x00;
    uint16_t index  = 0x00;

    for (index = 0x00;index <len; index++){
        chksum ^= *pTemp;
        pTemp++;
    }

    return chksum;
}

/*******************************************************************************
*�������ƣ�static char packet(char index,char cmd,const char *pTemp,char len,char repeatFlag)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
static int packet(char index,char cmd,const char *pTemp,uint16_t len,char repeatFlag)
{
    connectPacket_t  *pConnect = NULL;
    uint16_t i = 0x00;
    char chksum = 0x00;
    uint16_t chksumCount = 0x00;
    char *pChksum = NULL;
    uint16_t packetLen = 0x00;
    u32 iRet = 0x00;

    pConnect = memMalloc(sizeof(connectPacket_t));
    if (pConnect == NULL)                                return -1;

    memset(pConnect,0x00,sizeof(connectPacket_t));


    pConnect->packet.head = CONNECT_HEAD_MARK;

    if (len >= 0xFF-3){
      pConnect->packet.len =  0xFF;
    }
    else{
     pConnect->packet.len = len+3;                  // index cmd check
    }

    pConnect->packet.index = index;
    pConnect->packet.cmd = cmd;

    if ((pTemp != NULL)&&(len > 0x00)){
        memcpy(&pConnect->packet.cmd + 1, pTemp, len);
    }

    if (pConnect->packet.len == 0xFF){
        chksumCount = len+3;
    }
    else{
        chksumCount = pConnect->packet.len;
    }

    //У��ͼ���
    pChksum = &pConnect->packet.len;

#ifdef SONGWANG
    chksum = checkSum(pChksum,chksumCount);
#else
    chksum = connectChksum(pChksum,chksumCount);
#endif

    //У���
    packetLen = sizeof(pConnect->packet) + len;
    pConnect->buff[packetLen] = chksum;

    //APP_TRACE_DBG("\r\n���ݰ�У�鳤�ȣ�%d\r\n",packetLen);

    //����
   packetLen++;
   pConnect->buff[packetLen] = CONNECT_TAIL_MARK;
   packetLen++;

   connectCmd = pConnect->packet.cmd;

    //�ص�����
    if (pConncetFunc != NULL){
        for (i = 0x00;i < MAX_CONNECT_SEND_COUNT;i++){

        	//connectSendWait();  // xuejungong 20180626 �����ʱ����
            pConncetFunc(pConnect, packetLen, repeatFlag);

            if(repeatFlag){
              iRet = xSemaphoreTake(connectSemaphoreId,3000/portTICK_RATE_MS);
              if (iRet == pdTRUE){
                  APP_TRACE_DBG("\r\n------------Revice CONNECT Packet Resp!---------\r\n");
                  break;
                }
              else{
                  APP_TRACE_DBG("\r\n------------CONNECT Timeout!---------\r\n");
              }
            }
            else{               //����Ӧ��
                break;
            }
        }
    }

    connectCmd = 0x00;

    memFree(pConnect);


    if (i < MAX_CONNECT_SEND_COUNT){
        return 0x00;
    }
    else{
	#ifdef NDJ_Device  // ֻΪŤ����ʹ��
          // xuejungonng 20180628 add �ط�4�κ�û���յ�Ӧ��ʱ���Ϸ���������Ť�������ư�����
//          if(i == MAX_CONNECT_SEND_COUNT-1)//
//          {
        	  APP_TRACE_DBG("\r\n=========״̬��ѯû��Ӧ��===================\r\n");
        	  // ���
        	  ndj_Option_Status_t *pNdj_sta = NULL;
        	  pNdj_sta = memMalloc(sizeof(ndj_Option_Status_t));
        	  if(pNdj_sta == NULL)   					return  0x01;
        	  memset(pNdj_sta,0x00,sizeof(ndj_Option_Status_t));

        	  rondomSn(pNdj_sta->sn_cmd);
        	  pNdj_sta->len = 14;   //������+3
        	  pNdj_sta->index = (uint16_t)index<<8;	//
        	  pNdj_sta->cmd = cmd;	//

        	  // ����Ϊ����������
        	  memcpy(pNdj_sta->sn_id, ndj_SnID, 8);// ��ȡ�豸Ψһ��
        	  pNdj_sta->ndj_id = 0xff;   // 0xff ������ư�
        	  pNdj_sta->ndj_cmd = 0x16;  // 0x16  ָ���ϱ�����
        	  pNdj_sta->ndj_sta = 1;     // �豸����

        	  APP_TRACE_DBG("\r\n=========�ϱ��������豸�Ѿ�����===================\r\n");
        	  connectSendMsg(MSG_ID_HEART_MSG, (uint32_t)pNdj_sta, 21);  // ������Ϣ
//          }
		#endif
        return  0x01;
    }

    return  0x00;
}

// xuejungong 20180623 add
/*******************************************************************************
*�������ƣ�
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
static void connectStatusAns(char *pTemp,uint16_t len)
{
    packet(NDJ_DEVICE_MARK, NDJ_CMD, (const char *)pTemp, len, false);// ֱ��͸��  xuejungong 20180623 ���ȿ���������
}

// xuejungong 20180623 add
/*******************************************************************************
*�������ƣ�
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
void connectOption(char *pTemp,uint16_t len)
{
    int iRet = 0x00;
    //lyyTermialOption_t *pOption = (lyyTermialOption_t *)pTemp;

    APP_TRACE_DBG("\r\nOption Data:");
    APP_HEX_STRING_DBG(pTemp,len);

    APP_TRACE_DBG("\r\nOption Data Len =%d \r\n", len);

    //packet(NDJ_DEVICE_MARK, NDJ_CMD, (const char *)&pOption->buff[10], len-10, false);// ֱ��͸��  xuejungong 20180623 ���ȿ���������
    packet(NDJ_DEVICE_MARK, NDJ_CMD, (const char *)pTemp, len, false);// ֱ��͸��  xuejungong 20180623 ���ȿ���������
    memFree(pTemp);    // xuejungong 20180625 test
}

//xuejungong  20180625 add
/*******************************************************************************
*�������ƣ�void connectGetRevsion(char *pTemp,uint16_t len)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
char connectGetRevsion(char *pTemp,uint16_t len)
{
    int iRet = 0x00;
    ndj_UpgradePara_t *pUpgradePara = NULL;
    pUpgradePara = memMalloc(sizeof(ndj_UpgradePara_t));

    if (pUpgradePara == NULL)                                return -1;

    memset(pUpgradePara,0x00,sizeof(ndj_UpgradePara_t));

    memcpy(pUpgradePara->sn_id, pTemp, 8);
    pUpgradePara->id = 0xff;					// ��Կ��ư�
    pUpgradePara->cmd = 0x07;                    // ��������ѯ
    pUpgradePara->mark = pTemp[8];

    APP_TRACE_DBG("\r\nGetRev Data:");
    APP_HEX_STRING_DBG(pTemp,len);

    APP_TRACE_DBG("\r\n===============��ȡ�̼�����������======================\r\n");

    packet(NDJ_DEVICE_MARK, NDJ_CMD, (const char *)pUpgradePara, sizeof(ndj_UpgradePara_t), false);// ֱ��͸��
    memFree(pTemp);// xuejungong 20180625 test
    memFree(pUpgradePara);
}

//xuejungong  20180625 add
/*******************************************************************************
*�������ƣ�void connectSetRevision(char *pTemp,uint16_t len)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
char connectSetRevision(char *pTemp,uint16_t len)
{
    int iRet = 0x00;
    ndj_respPara_t *pRespPara = NULL;
    pRespPara = memMalloc(sizeof(ndj_respPara_t));

    if (pRespPara == NULL)                                return -1;

    memset(pRespPara,0x00,sizeof(ndj_respPara_t));

    memcpy(pRespPara->sn_id, pTemp, 8);
    pRespPara->id = 0xff;
    pRespPara->cmd = 0x08;                             // ���������̼�����Ӧ��
    memcpy((char *)&pRespPara->ndj_swp, &pTemp[8], len-8);

    APP_TRACE_DBG("\r\nSetRev Data:");
    APP_HEX_STRING_DBG(pTemp,len);

    APP_TRACE_DBG("\r\n===============���ù̼�����������======================\r\n");

    //lyyTermialAnswer((const char *)pOption->buff, pOption->packet.len);

    packet(NDJ_DEVICE_MARK, NDJ_CMD, (const char *)pRespPara, sizeof(ndj_respPara_t), false);// ֱ��͸��
    memFree(pTemp);
    memFree(pRespPara);
}

/*******************************************************************************
*�������ƣ�void connectUpgrade(const char *pUpgrade,uint16_t len)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
char connectUpgrade(char *pTemp,uint16_t len)
{
    int iRet = 0x00;
    uint16_t length = len-8;// xuejungong 20180628 �ܳ��ȼ�ȥ id+���ݰ���� �õ�������ʵ�ʳ���

    ndj_UpgradeData_t *pUpgradedat = NULL;
    pUpgradedat = memMalloc(sizeof(ndj_UpgradeData_t));

    if (pUpgradedat == NULL)                                return -1;

    memset(pUpgradedat,0x00,sizeof(ndj_UpgradeData_t));

    memcpy(pUpgradedat->sn_id, pTemp, 8);
    pUpgradedat->id = 0xff;
    pUpgradedat->cmd = 0x09;                             // ���������̼�����Ӧ��
    pUpgradedat->ndj_swd.len = length<<8 | length>>8;
//    pUpgradedat->ndj_swd.index = (uint16_t)pTemp[8]<<8 + (uint16_t)pTemp[9];
//    memcpy(pUpgradedat->ndj_swd.buf, &pTemp[10], len-8);
    memcpy((char *)&pUpgradedat->ndj_swd.index, &pTemp[8], len-8);


    APP_TRACE_DBG("\r\n===============�·�����������======================\r\n");
    //APP_HEX_STRING_DBG(pUpgradedat,len);

    //lyyTermialAnswer((const char *)pOption->buff, pOption->packet.len);

    packet(NDJ_DEVICE_MARK, NDJ_CMD, (const char *)pUpgradedat, len+4, false);// ֱ��͸��
    memFree(pTemp);
    memFree(pUpgradedat);
}

// xuejungong 20180705 add �㲥����Ť����
/*******************************************************************************
*�������ƣ�void connectUpgradeRd(void)
*����˵������������2�����ڷ������ݰ�����
*���������const char *pSn  ���豸ID/����ֵΪ 00000000ʱ��Ϊ�㲥ģʽ
*���������none
*�� �� ֵ�� none
*����˵����
*******************************************************************************/
void connectUpgradeRd(void)
{
	ndj_upgrade_t *pSys = ndjInforGet();
	uint16_t i = 0;
	uint16_t length = 0;
	uint16_t index = 0;
	uint16_t codenum = 0;
	uint32_t writeAddr = UPGRADE_DATA_ADDR_NDJ;

	index = pSys->packetTotal;
	codenum = pSys->codeNum;
	APP_TRACE_DBG("\r\n===============��ʼ����Ť�����豸======================\r\n");
	APP_TRACE_DBG("\r\n===============���ݰ�������%d======================\r\n", index);
	APP_TRACE_DBG("\r\n===============���ݰ���С��%d======================\r\n", codenum);
    for(i=1;i<=index;i++)
    {
    	ndj_UpgradeData_t *pUpgradedat = NULL;
    	pUpgradedat = memMalloc(sizeof(ndj_UpgradeData_t));

    	if (pUpgradedat == NULL)                                return;

    	memset(pUpgradedat,0x00,sizeof(ndj_UpgradeData_t));

		pUpgradedat->ndj_swd.flow = index<<8|index>>8;			// �������
		pUpgradedat->ndj_swd.buff_size = codenum<<8|codenum>>8;	// �������
		memcpy(pUpgradedat->ndj_swd.ves, pSys->version, 14); 	// �̼��汾��

    	pUpgradedat->id = 0xff;
    	pUpgradedat->cmd = 0x09;                             // ���������̼�����Ӧ��

    	if(i==index)                                // ���һ�����ݣ�����512
    	{
    		APP_TRACE_DBG("\r\n===============���һ������======================\r\n");
    		length = codenum-UPGRADE_PACKET_SIZE_NDJ*(index-1);
    	}
    	else
    	{
    		length = UPGRADE_PACKET_SIZE_NDJ;
    	}

    	pUpgradedat->ndj_swd.len = length<<8 | length>>8;// �������
    	pUpgradedat->ndj_swd.index = (uint16_t)i<<8 | (uint16_t)i>>8;// �������

    	APP_TRACE_DBG("\r\n===============�·�������������ţ�%d======================\r\n", i);
    	APP_TRACE_DBG("\r\n===============���ݰ���ȡ��ַ��%x=========================\r\n", writeAddr);
    	APP_TRACE_DBG("\r\n===============���ݰ����ȣ�%d============================\r\n", length);

    	flashRead(writeAddr,pUpgradedat->ndj_swd.buf,length);

    	packet(NDJ_DEVICE_MARK, NDJ_CMD, (const char *)pUpgradedat, length+32, false);   // ֱ��͸��
    	memFree(pUpgradedat);
    	writeAddr += UPGRADE_PACKET_SIZE_NDJ;

    	vTaskDelay(1000/portTICK_RATE_MS);  //  ��ʱ10ms

    	//if(!(i%10))
    		//watchDogFeed(SET);                 //  ��������ʱ��λ
    }
}

/*******************************************************************************
*�������ƣ�void connectSendMsg(u32 msgId,u32 param1,u32 param2)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
//void connectSendMsg(u32 msgId,char *param1,u32 param2)
void connectSendMsg(u32 msgId, u32 param1,u32 param2)
{
    connectMsg_t msg;

    memset(&msg,0x00,sizeof(connectMsg_t));

    msg.message = msgId;
    msg.param1 = param1;
    msg.param2 = param2;
    //memcpy(msg.param1, param1, param2);

    xQueueSend(connectQueue,&msg,MAX_PACKET_TIME/portTICK_RATE_MS);
}
//
//
/*******************************************************************************
*�������ƣ�portTASK_FUNCTION_PROTO(connectTask, pvParameters)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
portTASK_FUNCTION_PROTO(connectTask, pvParameters)
{
	connectMsg_t msg;
	int iRet = -1;
	connectQueue = xQueueCreate( MAX_CONNECT_MSG_COUNT, sizeof(connectMsg_t));

	if(connectQueue ==NULL)
	{
		vTaskDelete(NULL);
	}

    while(1)
    {
    	if(xMarkGetValue(MARK_DEVICE_LOGIN))
    	{
    		xQueueReceive(connectQueue,&msg,portMAX_DELAY);
    		APP_TRACE_DBG("\r\n<-- Connect Message Type:%X Param1:%X Param2:%X -->\r\n", msg.message, msg.param1, msg.param2);
    	}

		switch(msg.message)
		{
			case MSG_ID_OPT_MSG:
				connectOption((char *)msg.param1, msg.param2);
			break;

			case MSG_ID_GET_REVSION_MSG:
				connectGetRevsion((char *)msg.param1, msg.param2);
			break;

			case MSG_ID_SET_REVSION_MSG:
				connectSetRevision((char *)msg.param1, msg.param2);
			break;

			case MSG_ID_UPGRADE_MSG:
				xTmrStop(TMR_NDJ_CHECK);				 	// ֹͣ������ʱ����5���ӻ�ȡһ���豸��Ϣ��
				xMarkSetValue(MARK_NDJ_UPD, RESET);  		// ȡ�������������ݰ���־
				APP_TRACE_DBG("\r\n===============�������ݽ��մ洢���======================\r\n");
				connectUpgradeRd();
				xMarkSetValue(MARK_NDJ_UPD, SET);  	   		// ��ȡ�����������ݰ���־
				xTmrRestart(TMR_NDJ_CHECK); 	  			// ����������ʱ����5���ӻ�ȡһ���豸��Ϣ��
			break;

			case MSG_ID_HEART_MSG:
				lyyTermialAnswer((char *)msg.param1, msg.param2);
				break;

			default:
				break;
		}
    	vTaskDelay(10/portTICK_RATE_MS);  //  ��ʱ10ms
    }
}


/*******************************************************************************
*�������ƣ�void connectParse_NDJ(const char *pTemp)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
void connectParse_NDJ(const char *pTemp, uint16_t len)
{
	uint8_t ret = 0;
	uint8_t index = 0;
	uint16_t buf_len = 0;
	int      cmp_val = 0;   // xuejungong 20180628 ���ڱȽ��豸Ψһ��
	char     ves_tmp[14]={0x00,};// xuejungong 20180707 ���ڱȽϹ̼��汾��
	NDJ_Packet *pNdj_data = (NDJ_Packet *)&pTemp[3];
	buf_len = len-13; // xuejunngong 20180625 add ��ȡbuf�������ݳ���+id

	// ����Ӧ��
	ndj_lyypacket_t *pNdj_packet = NULL;
	pNdj_packet = memMalloc(sizeof(ndj_lyypacket_t));
	if(pNdj_packet == NULL)                     return;
	memset(pNdj_packet,0x00,sizeof(ndj_lyypacket_t));
	rondomSn(pNdj_packet->sn_cmd);// xuejungong 20180628  ʹ�����������������Ψһ��

	// �ն�Ӧ��
	ndj_Option_t *pNdj_option = NULL;
	pNdj_option = memMalloc(sizeof(ndj_Option_t));
	if(pNdj_option == NULL)   					return;
	memset(pNdj_option,0x00,sizeof(ndj_Option_t));

	rondomSn(pNdj_option->sn_cmd);// xuejungong 20180628  ʹ�����������������Ψһ��
	pNdj_option->len = pTemp[0];
	pNdj_option->index = (uint16_t)pTemp[1]<<8;
	pNdj_option->cmd = pTemp[2];
	memcpy(pNdj_option->buf, &pTemp[3], pNdj_option->len-3);

	connectSnIDCheck(pNdj_data->sn_id); // xuejungong 20180726 �յ��������ݶ������б����

	switch(pNdj_data->cmd)
	{
	case 0x10:
			APP_TRACE_DBG("\r\n=========��һ���ϵ�״̬Ӧ��===================\r\n");
			pNdj_data->cmd = 0x10;    //  ͨ�Ű�Ӧ��Ť����
			connectStatusAns((char *)pNdj_data, 10); // ����Ϊ10 Ψһ��+id+cmd ������

			memcpy(ves_tmp, &pNdj_data->buff[28], 14);
			connectSWCheck(pNdj_data->sn_id, ves_tmp);

			connectSendMsg(MSG_ID_HEART_MSG, (uint32_t)pNdj_option, len+7);  // ������Ϣ
			break;

	case 0x15:
		APP_TRACE_DBG("\r\n=========״̬��ѯӦ��===================\r\n");
		cmp_val = memcmp(pNdj_data->sn_id, ndj_SnID, 8);
		if(cmp_val == 0x00)
		{
			xSemaphoreGive(connectSemaphoreId);   // xuejungong 20180628 add �ͷ�2ֵ�ź�
		}
		//lyyTermialAnswer((const char*)pNdj_option, len+7);
		connectSendMsg(MSG_ID_HEART_MSG, (uint32_t)pNdj_option, len+7);  // ������Ϣ
		break;

	default:
		APP_TRACE_DBG("\r\n============To_WebServer_����Ӧ��=================\r\n");
		//lyyTermialAnswer((const char*)pNdj_option, len+7);
		connectSendMsg(MSG_ID_HEART_MSG, (uint32_t)pNdj_option, len+7);  // ������Ϣ
		break;
	}
	memFree(pNdj_packet);
}

/*******************************************************************************
*�������ƣ�void connectParse(const char *pTemp,uint16_t len)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
void connectParse(const char *pTemp,uint16_t len)
{
   connectPacket_t  *pConnect = (connectPacket_t *)pTemp;
   char  chkCode = 0x00;

   if(pConnect->packet.head != CONNECT_HEAD_MARK)
   {
	   APP_TRACE_DBG("����ͷ����\r\n");
	   return;
   }
   APP_TRACE_DBG("head = %02x\r\n", pConnect->packet.head);

   if (len < sizeof(pConnect->packet))
   {
	   APP_TRACE_DBG("���ݳ��ȴ���\r\n");
	   return;
   }

   APP_TRACE_DBG("len = %02x\r\n", pConnect->packet.len);
   APP_TRACE_DBG("cmd = %02x\r\n", pConnect->packet.cmd);

   //APP_TRACE_DBG("pConnect->packet.len = %d\r\n", sizeof(pConnect->packet));
   //APP_TRACE_DBG("pConnect->packet.len = %d\r\n",pConnect->packet.len);

#ifdef SONGWANG
   chkCode = checkSum(&pConnect->packet.len,pConnect->packet.len);
#else
   chkCode = connectChksum(&pConnect->packet.len,pConnect->packet.len);
   APP_TRACE_DBG("chkCode = %02x\r\n", chkCode);
#endif

   APP_TRACE_DBG("pConnect->buff[%d] = %x\r\n",pConnect->packet.len+1,pConnect->buff[pConnect->packet.len+1]);
   if (pConnect->buff[pConnect->packet.len+1]!= chkCode)
   {
	   APP_TRACE_DBG("����У�������\r\n");
	   return;
   }

#ifndef  NDJ_Device    // ʹ��Ť����ʱ����ʹ�ø�2ֵ����
   if (connectCmd == pConnect->packet.cmd){
       xSemaphoreGive(connectSemaphoreId);
   }
#endif

   switch (pConnect->packet.cmd){

   case 0x34:   //��ȡ�豸��Ϣ
		connectParse_NDJ(&pConnect->packet.head+1, pConnect->packet.len); //  ����Ҫ������
       break;

   default:
       break;
   }

}

// xuejungong 20180621 add
/*******************************************************************************
*�������ƣ�char connectRecCheck(void)
*����˵�����ж������Ƿ�ռ�ߣ����ڷ���ǰ
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
char connectRecCheck(void)
{
	char value = 0;
	char value_tmp=0;
	char ret=0;
	uint8_t i = 0;
//	value_tmp = GPIO_ReadInputDataBit(CTRL_RS485_RX_GPIO_PORT, CTRL_RS485_RX_PIN);
	value_tmp = GPIO_INPUT_GET(GPIO_ID_PIN(13));
	APP_TRACE_DBG("\r\n============value_tmp = %d=============\r\n", value_tmp);

	for(i=0;i<11;i++)// ������ 38400bit/s �������bufΪ522byte,��4176bit,��Ҫʱ��108ms
	{
		//value = GPIO_ReadInputDataBit(CTRL_RS485_RX_GPIO_PORT, CTRL_RS485_RX_PIN);
		value = GPIO_INPUT_GET(GPIO_ID_PIN(13));
		APP_TRACE_DBG("\r\n============value = %d=============\r\n", value);
		if(value != value_tmp) // ���ֵ�ƽ�仯ʱ��˵��������
		{
			ret = 0;
			APP_TRACE_DBG("\r\n============RX Level Change=============\r\n");
			break;
		}
		else if(i==10)
		{
			ret = 1;
		}
		vTaskDelay(10/portTICK_RATE_MS);
	}
	return ret;
}

// xuejungong 20180625 add
/*******************************************************************************
*�������ƣ�int connectSnIDCheck(char *pSn_id)
*����˵��������Ƿ��Ѿ�����
*����������豸ΨһID
*���������
*�� �� ֵ�� 1���µ��豸   0���豸�Ѵ���
*����˵����
*******************************************************************************/
int connectSnIDCheck(char *pSn_id)
{
	uint8_t i=0;
	uint8_t retValue;
	uint16_t index = 0;
	int ret = 0;    //
	if(pSettings->ndj_num>0)
	{
		for(i=0;i<pSettings->ndj_num;i++)
		{
			retValue = memcmp(pSn_id,pSettings->ndj_para[i].id,8);
			if((retValue != 0x00) && (i==pSettings->ndj_num-1))  // ���豸
			{
				APP_TRACE_DBG("\r\n==========�µ�Ť�����豸==========\r\n");
				index = pSettings->ndj_num;
				memcpy(pSettings->ndj_para[index].id, pSn_id, 8);
				//memcpy(pSettings->ndj_para[index].sw, pSn_ves, 14);

				if(index >MAX_NDJ_NUM)
				{
					pSettings->ndj_num = MAX_NDJ_NUM;
				}
				else
					pSettings->ndj_num++;

				ret = index;    // �µ��豸
				break;
			}
			else if(retValue == 0x00)             // ƥ�䵽���ʵ����豸ID
			{
				//return 1;
				APP_TRACE_DBG("\r\n==========Ť�����豸�Ѵ���==========\r\n");
				//pSettings->ndj_num --;
				ret = -1;    					// �豸�Ѵ���
				break;
			}
		}
	}
	else
	{
		pSettings->ndj_num ++;

		if(index >MAX_NDJ_NUM)
		{
			pSettings->ndj_num = MAX_NDJ_NUM;
		}

		APP_TRACE_DBG("\r\n==========�µ�Ť�����豸==========\r\n");
		index = pSettings->ndj_num;
		memcpy(pSettings->ndj_para[index-1].id, pSn_id, 8);
		//memcpy(pSettings->ndj_para[index-1].sw, pSn_ves, 14);

		ret = 0;    // �µ��豸
	}

	paramSave();// ����������flash��
	return ret;
}
/*******************************************************************************
*�������ƣ�char connectSnIDCheck(char *pSn_id)
*����˵��������Ƿ��Ѿ�����
*����������豸ΨһID
*���������
*�� �� ֵ�� 0���µ��豸   1���豸�Ѵ���
*����˵����
*******************************************************************************/
//char connectSnIDCheck(char *pSn_id)
char connectSWCheck(char *pSn_id, char *pSn_ves)
{
	uint8_t tmp = 0;
	ndj_upgrade_t *pSys = ndjInforGet();
	uint8_t i=0;
	int sta = 0;    //

	sta = connectSnIDCheck(pSn_id);
	APP_TRACE_DBG("\r\n==========Ť�����豸������%d==========\r\n", pSettings->ndj_num);

	if(sta>=0)     // xuejungong 20180726 staΪ�洢���
	{
		memcpy(pSettings->ndj_para[sta].sw, pSn_ves, 14);//xuejungong 20180726  �洢�̼��汾��
	}

	APP_TRACE_DBG("\r\n==========Ť�����豸�̼��汾���ж�==========\r\n");

	if(xMarkGetValue(MARK_NDJ_UPD))
	{
		if(pSys->upgrade);
		{
			APP_TRACE_DBG("\r\n==========�ϱ��Ĺ̼��汾��==========\r\n");
			APP_HEX_STRING_DBG(pSn_ves,14);
			APP_TRACE_DBG("\r\n");
			APP_TRACE_DBG("\r\n==========�洢�Ĺ̼��汾��==========\r\n");
			APP_HEX_STRING_DBG(pSys->version,14);
			APP_TRACE_DBG("\r\n");

			tmp = memcmp(pSn_ves,pSys->version,14);
			if(tmp && pSys->packetTotal<1024 && pSys->packetTotal>0)// �汾�Ų�һ��  ����
			{
				APP_TRACE_DBG("\r\n==========Ť�����豸�汾̫����Ҫ����==========\r\n");
				connectSendMsg(MSG_ID_UPGRADE_MSG, 0x00, 0x00);  // ������Ϣ����ʼΪ���豸����
			}
		}
	}

	paramSave();// ����������flash��
}

// xuejungong 20180625 add
/*******************************************************************************
*�������ƣ�char connectDeviceCheckDone(void)
*����˵����ѯ���豸��10����ִ��1��
*���������char *pSn Ϊ�豸Ψһ��
*���������
*�� �� ֵ�� ���Ͳ�ѯָ��
*����˵����
*******************************************************************************/
static void connectDeviceCheckDone(xEventParam_t *pEvent)
{
	NDJ_Packet *pndj_packet = NULL;
	uint8_t i = 0;
	int ret = 0;
	char index = 0;

	APP_TRACE_DBG("\r\n==========���ϱ���Ť�������� %d==========\r\n", pSettings->ndj_num);
    if(pSettings->ndj_num>0)
    {
    	for(i=0;i<pSettings->ndj_num;i++)
    	{
    		pndj_packet = memMalloc(sizeof(NDJ_Packet));

    		if (pndj_packet == NULL)                                return;

    		memset(pndj_packet,0x00,sizeof(NDJ_Packet));

    		memcpy(pndj_packet->sn_id, pSettings->ndj_para[i].id, 8);
    		memcpy(ndj_SnID, pSettings->ndj_para[i].id, 8);     // xuejungong 20180628 ��ȡ��Ҫ��ѯ���豸Ψһ�룬�����ж�
    		pndj_packet->addr_ID = 0xff;
    		pndj_packet->cmd = 0x15;                       // ��ȡ���� ��ʱ��ѯ

    		APP_TRACE_DBG("\r\n=========��ѯӦ���豸��ţ�%d================\r\n", i);

    		ret = packet(NDJ_DEVICE_MARK, NDJ_CMD, (const char *)pndj_packet, 10, true);// ֱ��͸��  ����10  sn_id+id+cmd
    		memFree(pndj_packet);

    		if(ret)// û�н��յ�Ӧ��
    		{
    			APP_TRACE_DBG("\r\n=========û���յ�״̬��ѯӦ�������%d================\r\n", pSettings->ndj_para[i].err);
				if(pSettings->ndj_para[i].err >=2)  // ����3��û��
				{
					pSettings->ndj_para[i].sta = 1;
					pSettings->ndj_para[i].err = 0;

					if(i == pSettings->ndj_num-1)  // �����ǰֻ��һ�����ߵ�ǰΪ���һ���豸
					{
						APP_TRACE_DBG("\r\n==========ɾ���б�����=============\r\n");
						memset(pSettings->ndj_para[i].id, 0x00, 8);//�����ɾ���豸�б�
					}
					else// ���򣬽����һ���豸������ǰ�豸�б���
					{
						APP_TRACE_DBG("\r\n==========�����б�����=============\r\n");
						index = pSettings->ndj_num-1;
						memcpy(pSettings->ndj_para[i].id, pSettings->ndj_para[index].id, 8);
						pSettings->ndj_para[index].sta = 1;  // �������豸���б�
						pSettings->ndj_para[index].err = 0;
					}
					pSettings->ndj_num--;// ����������1
				}
				else
				{
					pSettings->ndj_para[i].err ++;
				}
    		}
    		else
    		{
    			pSettings->ndj_para[i].err = 0;
    			pSettings->ndj_para[i].sta = 0;
    		}
    		paramSave();// ����������flash��
    	}
	}
    //xMarkSetValue(MARK_NDJ_STA,SET);
}


// xuejungong 20180625 add
/*******************************************************************************
*�������ƣ�
*����˵������ʱ���ص�����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
void ndj_TimerCallback(void)
//void ndj_TimerCallback(xTimerHandle pxTimer)
{
	APP_TRACE_DBG("\r\n========Ť����״̬��ѯNDJ--check s/t========= \r\n");
    xEventSend(EVENT_LOW,connectDeviceCheckDone,0x00,0x00);
}

// xuejungong 20180625 add
/*******************************************************************************
*�������ƣ�void NDJ_CreateTimer(void)
*����˵����������ʱ��
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
//static void NDJ_CreateTimer(void)
//{
//	ndjCheckTime = xTimerCreate(    "ndj_timer",      // Just a text name, not used by the kernel.
//											 ( NDJ_CHECK_S/portTICK_PERIOD_MS ),   	// The timer period in ticks.
//											 pdTRUE,        		// The timers will auto-reload themselves when they expire.
//											 ( void * ) NULL,  	// Assign each timer a unique id equal to its array index.
//											 ndj_TimerCallback 	// Each timer calls the same callback when it expires.
//										  );
//
//	if( xTimerStart( ndjCheckTime, 0 ) != pdPASS ){
//
//			}
//}

// xuejungong 20180625 add
/*******************************************************************************
*�������ƣ�void NDJ_StopTimer(void)
*����˵����������ʱ��
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
//static void NDJ_StopTimer(void)
//{
//	if( xTimerStop( ndjCheckTime, 0 ) != pdPASS ){
//
//			}
//}

// xuejungong 20180625 add
/*******************************************************************************
*�������ƣ�void NDJ_ReStartTimer(void)
*����˵����������ʱ��
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
//static void NDJ_ReStartTimer(void)
//{
//	if( xTimerStart( ndjCheckTime, 0 ) != pdPASS ){
//
//			}
//}


// xuejungong 20180710 add
/*******************************************************************************
*�������ƣ�void NDJ_CreateTimer(void)
*����˵����������ʱ��
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
//static void NDJ_Delay(int tms)
//{
//	int i=0,j=0;
//	for(i=0;i<tms;i++)
//		for(j=0;j<1000;j++);
//}

// xuejungong 20180717 add
/*******************************************************************************
*�������ƣ�
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
int connectCallback(const void *pTemp,uint16_t len,char repeatFlag)
{
    //��ӡ���ݰ�����
    APP_TRACE_DBG("\r\n \r\n%d To_NDJ_CONNECT Packet:",getTick(), len);
    if(len>255)
    {
    	APP_HEX_STRING_DBG(pTemp,255);
    	vTaskDelay(10/portTICK_RATE_MS);
    	APP_HEX_STRING_DBG((char *)pTemp+255,len-255);
    	if((len-255-255)>0)
    		APP_HEX_STRING_DBG((char *)pTemp+255+255,len-255-255);
    }
    else
    {
    	APP_HEX_STRING_DBG(pTemp,len);
    }
    APP_TRACE_DBG("\r\n");

    // xuejungong 20180613 need to add judge for RS485
    //if(connectRecCheck())
    //{
    	xCtrlOnOff(CTRL_RS485_RE, CTRL_HIGH);
    	//xCtrlOnOff(CTRL_RS485_RE, CTRL_LOW);
    	xSerialWrite(UART0, (const char *)pTemp, len);

    	if(!xMarkGetValue(MARK_NDJ_UPD))                // xuejungong 20180721 �������⴦��
    	    vTaskDelay(50/portTICK_RATE_MS);
    	else
    		vTaskDelay(20/portTICK_RATE_MS);

    	xCtrlOnOff(CTRL_RS485_RE, CTRL_LOW);
    	//xCtrlOnOff(CTRL_RS485_RE, CTRL_HIGH);
    	if(!xMarkGetValue(MARK_NDJ_UPD))			 // xuejungong 20180721 �������⴦��
    		vTaskDelay(50/portTICK_RATE_MS);

    //}

    return 0x00;
}
