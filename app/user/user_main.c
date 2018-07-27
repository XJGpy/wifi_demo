/*
 * ESPRSSIF MIT License
 *
 * Copyright (c) 2015 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "esp_common.h"
#include "user_config.h"

extern char	wifiRssiValue;
static uint32_t upgradeAddr = 0x00;

#define PLUG_KEY_NUM			1

LOCAL struct keys_param keys;
LOCAL struct single_key_param *single_key[PLUG_KEY_NUM];

/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;
        case FLASH_SIZE_64M_MAP_1024_1024:
            rf_cal_sec = 2048 - 5;
            break;
        case FLASH_SIZE_128M_MAP_1024_1024:
            rf_cal_sec = 4096 - 5;
            break;
        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}


/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
uint32_t ICACHE_FLASH_ATTR flashWrite(uint32_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
{
	SpiFlashOpResult  result = SPI_FLASH_RESULT_OK;
	uint32_t *pWrite_buff = (uint32_t *)pBuffer;
//	uint32_t writeCount= NumToWrite/4;
//	uint32_t writeCount_tmp = NumToWrite%4;
//
//	if((NumToWrite%4))
//		writeCount = writeCount+1;

	if((WriteAddr % SECTOR_SIZE) == 0) {
		spi_flash_erase_sector(WriteAddr / SECTOR_SIZE);
	}

	result = spi_flash_write(WriteAddr,pWrite_buff,NumToWrite);
	if(result == SPI_FLASH_RESULT_OK)			return NumToWrite;
	else										return 0x00;
}

/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void ICACHE_FLASH_ATTR flashRead(uint32_t WriteAddr,uint8_t *pBuffer,uint16_t NumToRead)
{
	uint32_t *pRead_buff = (uint32_t *)pBuffer;

	spi_flash_read(WriteAddr,pRead_buff,NumToRead);

}


/*******************************************************************************
*函数名称：void getSystemInfor(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void systemInfor(void)
{
    uint32_t userbinaddr = 0x00;

    struct rst_info *rst_info = system_get_rst_info();
    APP_TRACE_DBG("reset reason: %x\n", rst_info->reason);

	if(rst_info->reason == REASON_WDT_RST ||
			rst_info->reason == REASON_EXCEPTION_RST ||
				rst_info->reason == REASON_SOFT_WDT_RST)
	{
		if (rst_info->reason == REASON_EXCEPTION_RST)
		{
			APP_TRACE_DBG("Fatal exception (%d):\n", rst_info->exccause);
		}
		APP_TRACE_DBG("epc1=0x%08x, epc2=0x%08x, epc3=0x%08x, excvaddr=0x%08x, depc=0x%08x\n",
				rst_info->epc1, rst_info->epc2, rst_info->epc3, rst_info->excvaddr, rst_info->depc);
	}

   	userbinaddr = system_get_userbin_addr(); //获取程序

	if(userbinaddr < 0X81000)
	{
		upgradeAddr = 0X101000;
    }
	else
	{
    	upgradeAddr = 0X1000;
    }

	APP_TRACE_DBG("\r\nupgrade_userbin:%02X\r\n",system_upgrade_userbin_check());
	APP_TRACE_DBG("\r\nuserbin_addr:%02X\r\n",userbinaddr);
	APP_TRACE_DBG("\r\nboot_mode:%02X\r\n",system_get_boot_mode());
}

/*******************************************************************************
 *函数名称：uint32_t getUpgradAddr(void)
 *函数说明：
 *输入参数：
 *输出参数：
 *返 回 值：
 *其他说明：
 *******************************************************************************/
uint32_t getUpgradAddr(void)
{
	return upgradeAddr;
}


/*******************************************************************************
 *函数名称：void xPortInit(void)
 *函数说明：
 *输入参数：
 *输出参数：
 *返 回 值：
 *其他说明：
 *******************************************************************************/
void xPortInit(void)
{
	/**/
	if(xMarkGetValue(MARK_NDJ_IDC) == RESET)// xuejungong 20180724 配置了ID才允许进行485通信
	{
		system_uart_swap();   // xuejungong  20180727   配合esp硬件，将串口调换

		// xuejungong 2018727 GPIO_OUTPUT
	#if !defined DEBUG // debug gpio2和串口共用，需要区分开，正式版本不需要debug
		PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);   		//RED_LED
	#endif
		PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_GPIO1);   		//GREEN_LED
		PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);	  	//xuejungong 20180717 RS485_RE

		// xuejungong 2018727 GPIO_INPUT
		PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0RXD_U, FUNC_GPIO3);	  	//xuejungong 20180717 KEY_SC
		PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14);	  	//xuejungong 20180717 KEY_IN

		// xuejungong 20180717 GPIO_OUTPUT
	#if !defined DEBUG
		GPIO_AS_OUTPUT(2);
	#endif
		GPIO_AS_OUTPUT(1);
		GPIO_AS_OUTPUT(4);

		// xuejungong 2018727 GPIO_INPUT
		GPIO_AS_INPUT(3);
		GPIO_AS_INPUT(14);

		/**/
		xLedInit();// xuejungong 20180727 初始化LED等，设置为0

		xCtrlOnOff(CTRL_RS485_RE, CTRL_LOW);   // xuejungong 20180717  485 默认接收模式
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
void ICACHE_FLASH_ATTR paramRead(param_t *pInfor)
{
	system_param_load(PARAM_SPIFLASH_SEC,0,pInfor,sizeof(param_t));
}


/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
int ICACHE_FLASH_ATTR paramWrite(void *pTemp,uint16_t len)
{
  system_param_save_with_protect(PARAM_SPIFLASH_SEC,pTemp,len);

  return 0x00;
}




/*******************************************************************************
*函数名称： void clientSendBuf(const char *pTemp,uint16_t len)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
int clientSendDone(void *pTemp,uint16_t len)
{
	clientSendBuf(pTemp,len);
	return 0x00;
}

/*******************************************************************************
*函数名称：int clientSendEvent(const char *pMessage,uint16_t len,
                             const char *ip,uint16_t port,
                             socketTypeDef type, uint16_t delay,
                             uint16_t time, uint8_t count)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
int clientSendEvent(const char *pMessage,uint16_t len,
                  const char *ip,uint16_t port,
                  char type, uint16_t delay,
                  uint16_t time, uint8_t count)
{
  //连接网络过长有问题
	ringPacket_t    *pInfor = NULL;
	int              retValue = 0x00;

	pInfor = memMalloc(sizeof(ringPacket_t));

	if(pInfor == NULL)
		return -32;

	memset(pInfor,0x00,sizeof(ringPacket_t));

	memcpy(pInfor->buff,pMessage,len);
	pInfor->len = len;

	retValue = ringEventWrite(clientSendDone,NULL,
				  pInfor->buff,pInfor->len,
				  MAX_DONE_TIMEOUT/TICKS_PERIOD_MS,
				  delay/TICKS_PERIOD_MS,
				  count,
				  time/TICKS_PERIOD_MS,getTick);

	memFree(pInfor);

	return retValue;
}



/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
int lyySendCallback(const void *pTemp,uint16_t len,char repeatFlag)
{
	int retValue = 0x00;
 	char  *pDes = (char *)pTemp;

	uint32_t waitTime = pSettings->send.wait;
	char    count = pSettings->send.count;

	APP_TRACE_DBG("\r\n==============Lyy Packet:============\r\n");
	APP_HEX_STRING_DBG(pDes,len);
	APP_TRACE_DBG("\r\n");

	if(repeatFlag== 0x00)
	{
		waitTime = 0x00;
		count = 0x01;
    }

    lyy_write_t  *pLyy = (lyy_write_t  *)pTemp;

    if((memcmp(pLyy->packet.attr,"AL",2) == 0x00)||
    		(memcmp(pLyy->packet.attr,"A1",2) == 0x00))
    {
    	xTmrRestart(TMR_RDY);
    	clientSendBuf(pTemp,len);
    }
    else
    {
      //设备没有登录
    	if(xMarkGetValue(MARK_DEVICE_LOGIN)==RESET)
    	{
    		if(memcmp(pLyy->packet.attr,"AH",2) != 0x00)
    		{
    			return -1;
            }
        }
      //
    	if((memcmp(pLyy->packet.attr,"A2",2) == 0x00)||
    			(memcmp(pLyy->packet.attr,"A3",2) == 0x00))
    	{
    		count = 0x01;
        }

    	clientSendEvent(pTemp,len,pSettings->server.ip,pSettings->server.port,0x00,
                      0x00,waitTime,count);
    }

    tmr_t *pTmr = NULL;
    pTmr =xTmrGetInfor(TMR_HEART);

    if(pTmr->state != TMR_STOPPED)
    {
    	xTmrRestart(TMR_HEART);
    }

    return retValue;
}

/*******************************************************************************
*函数名称：void privateKeyGen(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void privateKeyGen(void)
{
	const char *pEncode ="leyaoyaoleyaoyao";
	u32 tick = xTaskGetTickCount();
	char uidTemp[20];
	char index = 0x00;

	char *pTemp =NULL;
	int  randValue = 0x00;

	sysInfor_t *pInfor = sysInforGet();

	if(strlen(pSettings->key.key)> 0x00)
	{
		memcpy(pInfor->key,pSettings->key.key,MAX_PRIVATE_KEY_SIZE);
		memcpy(pInfor->pass,pSettings->key.pass,MAX_PASSWORD_SIZE);
		xMarkSetValue(MARK_KEY_NET,SET);
		return;
	}

	srand(tick);
	randValue = rand();

	//获取MCU的ID
	wifi_get_macaddr(STATION_IF, uidTemp);
	memcpy(pInfor->key,uidTemp,16);

	for (index = 0x00;index < 16;index++)
	{
		pInfor->key[index]^=*(pEncode+index);
	}
   //
	memcpy(pInfor->pass,uidTemp+12,MAX_PASSWORD_SIZE);
}


/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
static void appInit(void)
{
  //char unique_code[16];
	sysInfor_t *pInfor = sysInforGet();

	privateKeyGen();

	//登录 传送密钥间隔时间
	xTmrInit(TMR_RDY,LINK_RDY_PERIOD,getRTCTick);

	//发送心跳包间隔时间初始化40秒
	xTmrInit(TMR_HEART,LINK_HEART_PERIOD,getRTCTick);

	//发送信号间隔时间初始化60秒
	xTmrInit(TMR_CSQ,LINK_CSQ_PERIOD,getRTCTick);
	xTmrRestart(TMR_CSQ);

	//发送投币信息间隔时间初始化10秒
	xTmrInit(TMR_COIN,LINK_COIN_PERIOD,getRTCTick);

	//xuejungong 20180718 add ndj status check per 5min
	xTmrInit(TMR_NDJ_CHECK,NDJ_CHECK_PERIOD,getRTCTick);

	//xuejungong 20180718 add ndj status check per 5min
	xTmrInit(TMR_NDJ_SC,NDJ_SC_PERIOD,getRTCTick);

	lyyInit(lyySendCallback);

#if defined TEST_SERVER
	memset(pSettings->server.ip,0x00,sizeof(pSettings->server.ip));
	strcat(pSettings->server.ip,"sm.leyaoyao.com");
	pSettings->server.port = 4911;
	xMarkSetValue(MARK_KEY_NET,RESET);
	xMarkSetValue(MARK_DEVICE_LOGIN,RESET);

#elif defined DESIGN_SERVER
	memset(pSettings->server.ip,0x00,sizeof(pSettings->server.ip));
	strcat(pSettings->server.ip,"dm.leyaoyao.com");
	pSettings->server.port = 4922;
	xMarkSetValue(MARK_KEY_NET,RESET);
	xMarkSetValue(MARK_DEVICE_LOGIN,RESET);

#else
	memset(pSettings->server.ip,0x00,sizeof(pSettings->server.ip));
	strcat(pSettings->server.ip,"ehw.leyaoyao.com");
	pSettings->server.port = 4933;
	xMarkSetValue(MARK_KEY_NET,RESET);
	xMarkSetValue(MARK_DEVICE_LOGIN,RESET);
#endif

	APP_TRACE_DBG("\r\n 设备号 :");
	APP_HEX_STRING_DBG(pSettings->id,8);
	APP_TRACE_DBG("\r\n硬件版本:");
	APP_TRACE_DBG("%s",SoftwareVersion.hw);
	APP_TRACE_DBG("\r\n软件版本:");
	APP_TRACE_DBG("%s",SoftwareVersion.sw);
	APP_TRACE_DBG("\r\n通信密码:");
	APP_HEX_STRING_DBG(pInfor->key,16);
	APP_TRACE_DBG("\r\n用户密码:");
	APP_HEX_STRING_DBG(pInfor->pass,8);
	APP_TRACE_DBG("\r\n服务器地址:");

	if(strlen(pSettings->server.ip)==0x04)
	{
		APP_TRACE_DBG("%d.%d.%d.%d",pSettings->server.ip[0],pSettings->server.ip[1],
							pSettings->server.ip[2],pSettings->server.ip[3]);
	}
	else
	{
		APP_TRACE_DBG("%s",pSettings->server.ip);
	}

	APP_TRACE_DBG("\r\n服务器端口:");
	APP_TRACE_DBG("%d",pSettings->server.port);
}


/*******************************************************************************
 *函数名称：void loginEvent(void)
 *函数说明：
 *输入参数：
 *输出参数：
 *返 回 值：
 *其他说明：
 *******************************************************************************/
void loginEvent(void)
{
	tmr_t *pTmr = NULL;
	sysInfor_t *pSysInfor = sysInforGet();

  //发送设备密钥数据
	if(xMarkGetValue(MARK_KEY_NET)==RESET)
	{
		pTmr =xTmrGetInfor(TMR_RDY);

		if(pTmr->state != TMR_STOPPED)
		{
			return;
		}

		lyyNetPacket(pSysInfor->key,pSysInfor->pass);
		xTmrRestart(TMR_RDY);
	}
	//发送设备登陆数据包
	if(xMarkGetValue(MARK_DEVICE_LOGIN)==RESET)
	{
		pTmr =xTmrGetInfor(TMR_RDY);

		if(pTmr->state != TMR_STOPPED)
		{
			return;
		}

		lyyLoginPacket(pSysInfor->pass);
		xTmrRestart(TMR_RDY);
		return;
	}
	 //启动发送心跳包时间机制
	pTmr = xTmrGetInfor(TMR_HEART);

	if(pTmr->state == TMR_STOPPED)
	{
		xTmrRestart(TMR_HEART);
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
static void xTmrDone(xEventParam_t *pEvent)
{
	//int retValue = 0x00;
	APP_TRACE_DBG("\n[%02d] xAlarmTmr Timeout Event!\r\n",pEvent->event.index);

	switch(pEvent->event.index)
	{
		case TMR_RDY: //定时查询连接服务器状态

		case TMR_LINK: //连接失败重新连接
			xTmrStop((xTmrTypeDef)pEvent->event.index);
		break;

		case TMR_HEART:
			if(xMarkGetValue(MARK_DEVICE_LOGIN))
			{
				xTmrRestart((xTmrTypeDef)pEvent->event.index);
				lyyHeartPacket(0);
			}
			else
			{
				xTmrStop((xTmrTypeDef)pEvent->event.index);
			}
		break;

		case TMR_COIN:
			if(xMarkGetValue(MARK_DEVICE_LOGIN))
			{
				coinSendTotalInfor();
				giftSendTotalInfor();
			}
			xTmrStop((xTmrTypeDef)pEvent->event.index);
		break;

		case TMR_CSQ:
			xTmrRestart((xTmrTypeDef)pEvent->event.index);

			if(xMarkGetValue(MARK_DEVICE_LOGIN))
			{
				lyyRssiPacket(wifiRssiValue);
			}
		break;

		case TMR_NDJ_CHECK:
			xTmrRestart((xTmrTypeDef)pEvent->event.index);
			ndj_TimerCallback();
		break;

		case TMR_NDJ_SC:
			APP_TRACE_DBG("\r\n===========SmartConfig Fail and Ready To Restart============\r\n");
			system_restart();
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
static void xTmrEvent(char index)
{
   xEventSend(EVENT_HIGH,xTmrDone,index,0x00);
}


/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void xTmrCallback(pTickFunc pTick)
{
	char index = 0x00;
	tmr_t *pTmr = NULL;

	for(index = 0x00;index < MAX_TMR;index++)
	{
		pTmr = xTmrGetInfor((xTmrTypeDef)index);

		if(pTmr->pTick != pTick)
			continue;

		if(xTmrExpired((xTmrTypeDef)index))
		{
			xTmrEvent(index);
		}
	}
}

/*******************************************************************************
*函数名称：void dbgDone(const void *pTemp,uint16_t len)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
void dbgDone(const void *pTemp,uint16_t len)  // xuejungong 20180727 debug输出接口
{
	char *p = (char *)pTemp;
    uint16_t index = 0x00;

	for(index=0x00;index<len;index++)
	{
		uart_tx_one_char(UART1, *p);
		p++;
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
void ICACHE_FLASH_ATTR upgradeEvent(void)
{
	sysInfor_t *pSys = sysInforGet();

	if(pSys->upgrade.upgrade ==RESET)
		return;

	if(ringEventIsEmpty()==RESET)
		return;

	APP_TRACE_DBG("UPGRADE_FINISH\r\n");
	system_upgrade_flag_set(UPGRADE_FLAG_FINISH);
	system_upgrade_reboot();
	pSys->upgrade.upgrade  = RESET;
}



/*******************************************************************************
 *函数名称：void prvSetupHardware(void)
 *函数说明：
 *输入参数：
 *输出参数：
 *返 回 值：
 *其他说明：
 *******************************************************************************/
void prvSetupHardware(void)
{
    uart_init_new();  // xuejungong 20180723 硬件串口初始化

    //
    dbgInit(dbgDone,1024);  // xuejungong 20180723 debug接口函数初始化

    //
    APP_TRACE_DBG("SDK version:%s\n", system_get_sdk_version());

    //获取芯片信息
    systemInfor();    // xuejungongg 20180723 获取芯片数据

    //
    sysInforInit();       // xuejungong 20180723  升级参数/密钥初始化

	//
	xPortInit();	// xuejungongg 20180723  IO口初始化

	//
	coinInit();		// xuejungong 20180723  IO口触发/中断初始化

	//
	sysTimeInit();   // xuejungong 20180723   定时器初始化（ms，100ms, 1000ms）

	//
	paramInit(paramRead,paramWrite); // xuejungong 20180723   参数初始化

	//
	xEventInit();			// xuejungong  20180723  事件队列初始化

   //
    appInit();			// xuejungong 20180723  引用层数据初始化

    //
    connectInit(connectCallback); // xuejungong 20180717 add  485通信初始化

	//
	ledFlash(100/TICKS_PERIOD_MS,100/TICKS_PERIOD_MS,10);

}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void user_init(void)
{
	prvSetupHardware(); 										// xuejungong 20180723 部分硬件接口，定时器，参数等初始化
	xTaskCreate(initTask, "initTask", 1*256, NULL, 1, NULL); 	// xuejungong 20180723 部分任务创建入口，并启动
}

