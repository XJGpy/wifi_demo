/*
 * user_task.c
 *
 *  Created on: 2018��4��13��
 *      Author: lyy
 */
#include "user_config.h"
#include "lwip/api.h"
#define TCP_Client_RX_BUFSIZE 1024
#define TCP_CLIENT_TX_BUFSIZE 256

typedef struct {
	char		buff[TCP_CLIENT_TX_BUFSIZE];
	uint16_t	len;
	char		flag;
	err_t		result;
}sendbuf_t;

static sendbuf_t sendbuf;
char	wifiRssiValue;

xSemaphoreHandle wifi_alive;
xTaskHandle TcpClientHandle;
xTaskHandle WifiLinkHandle;

/*******************************************************************************
*�������ƣ� void wifiTask(void *pvParameters)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
 void wifiTask(void *pvParameters)
{
	uint8_t status = 0x00;

	vSemaphoreCreateBinary(wifi_alive);
	xSemaphoreTake(wifi_alive, 0);

	if (wifi_get_opmode() != STATION_MODE) {
		wifi_set_opmode(STATION_MODE);
		vTaskDelay(1000 / portTICK_RATE_MS);
		system_restart();
	}

	while (1)
	{
		APP_TRACE_DBG("\r\n=======WiFi: Connecting to WiFi========\r\n");
		wifi_station_connect();
		struct station_config *config = (struct station_config *) zalloc(
				sizeof(struct station_config));
		sprintf(config->ssid, pSettings->link.user);
		sprintf(config->password, pSettings->link.pass);
		wifi_station_set_config(config);
		free(config);
		status = wifi_station_get_connect_status();
		int8_t retries = 30;

		while ((status != STATION_GOT_IP) && (retries > 0))
		{
			status = wifi_station_get_connect_status();

			if (status == STATION_WRONG_PASSWORD)
			{
				APP_TRACE_DBG("WiFi: Wrong password\n");
				break;
			}
			else if (status == STATION_NO_AP_FOUND)
			{
				APP_TRACE_DBG("WiFi: AP not found\n");
				break;
			}
			else if (status == STATION_CONNECT_FAIL)
			{
				APP_TRACE_DBG("WiFi: Connection failed\n");
				break;
			}

			redLedFlash(500,500,1);
			vTaskDelay(1000 / portTICK_RATE_MS);
			--retries;
		}

		if (status == STATION_GOT_IP)
		{
			APP_TRACE_DBG("\r\n=============WiFi: Connected==========\r\n");
			xTaskCreate(TCP_Client, "server", 2048, NULL, (tskIDLE_PRIORITY + 2), &TcpClientHandle);
			vTaskDelay(1000 / portTICK_RATE_MS);
		}

		while ((status = wifi_station_get_connect_status()) == STATION_GOT_IP)
		{
			xSemaphoreGive(wifi_alive);
			 //APP_TRACE_DBG("WiFi: Alive\n");
			//xLedStart(0x01, 100, 100, 1);
			wifiRssiValue = (~wifi_station_get_rssi()) + 1;
			//APP_TRACE_DBG("\r\nWiFi Rssi: %d\r\n",wifiRssiValue);
			vTaskDelay(1000 / portTICK_RATE_MS);
		}

		APP_TRACE_DBG("\r\n=============WiFi: Disconnected============\r\n");
		xMarkSetValue(MARK_DEVICE_LOGIN,RESET);
		xTmrStop(TMR_HEART);
		sendbuf.flag = RESET;
		vTaskDelete(TcpClientHandle);

		wifi_station_disconnect();
		vTaskDelay(1000 / portTICK_RATE_MS);
	}
}


 /*******************************************************************************
 *�������ƣ� void mainTask(void *pvParameters)
 *����˵����
 *���������
 *���������
 *�� �� ֵ��
 *����˵����
 *******************************************************************************/
 void mainTask(void *pvParameters)
{
	while (1)
	{
		xEvent(EVENT_HIGH,20/TICKS_PERIOD_MS);
		ringPayEvent();

		if(xMarkGetValue(MARK_DEVICE_LOGIN))
		{
			ringEventLoop();
		}
		//coinSimulateEvent();
	}
}

 /*******************************************************************************
 *�������ƣ� void initTask(void *pvParameters)
 *����˵����
 *���������
 *���������
 *�� �� ֵ��
 *����˵����
 *******************************************************************************/
void initTask(void *pvParameters)
{
//
	if(xMarkGetValue(MARK_NDJ_IDC) == RESET)
	{
		APP_TRACE_DBG("\r\n*********************xx Start WIFI Task\r\n");
		if(strlen(pSettings->link.user)) // xuejungong 20180723  wifi��ssid����ʱ����wifi���ӵȲ���
		{
			xTaskCreate(wifiTask, "wifiTask", 2*256, NULL, 3, &WifiLinkHandle); // xuejungong 20180723 wifi���Ӳ��������Ӻ�̨����
		}
	}

	xTaskCreate(mainTask, "mainTask", 2*256, NULL, 4, NULL);     // xuejungong 20180723 ��������Ҫ���ڿ����������ݴ���
	xTaskCreate(connectTask, "connectTask", 4*256, NULL, 4, NULL); // xuejungong 20180718 485ͨ������

	while (1)
	{
		xEvent(EVENT_LOW,portMAX_DELAY);  // xuejungong 20180723  �����ȼ��¼����ղ�����
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
void ICACHE_FLASH_ATTR smartconfig_done(sc_status status, void *pdata)
{
    switch(status) {
        case SC_STATUS_WAIT:
        	APP_TRACE_DBG("SC_STATUS_WAIT\n");
            break;

        case SC_STATUS_FIND_CHANNEL:
        	APP_TRACE_DBG("SC_STATUS_FIND_CHANNEL\n");
            break;

        case SC_STATUS_GETTING_SSID_PSWD:
        	APP_TRACE_DBG("SC_STATUS_GETTING_SSID_PSWD\n");
            sc_type *type = pdata;
            if (*type == SC_TYPE_ESPTOUCH) {
            	APP_TRACE_DBG("SC_TYPE:SC_TYPE_ESPTOUCH\n");
            } else {
            	APP_TRACE_DBG("SC_TYPE:SC_TYPE_AIRKISS\n");
            }
            break;

        case SC_STATUS_LINK:
        	APP_TRACE_DBG("SC_STATUS_LINK\n");
            struct station_config *sta_conf = pdata;

            memset(pSettings->link.user,0x00,sizeof(pSettings->link.user));
            memcpy(pSettings->link.user,sta_conf->ssid,strlen(sta_conf->ssid));

            memset(pSettings->link.pass,0x00,sizeof(pSettings->link.pass));
            memcpy(pSettings->link.pass,sta_conf->password,strlen(sta_conf->password));

            paramSave();

            xTmrRestart(TMR_NDJ_SC);      // xuejungong 20180718 add timer for Smartconfig check

	        wifi_station_set_config(sta_conf);
	        wifi_station_disconnect();
	        wifi_station_connect();
            break;

        case SC_STATUS_LINK_OVER:
        	APP_TRACE_DBG("SC_STATUS_LINK_OVER\n");
            if (pdata != NULL) {
				//SC_TYPE_ESPTOUCH
                uint8 phone_ip[4] = {0};
                memcpy(phone_ip, (uint8*)pdata, 4);
                APP_TRACE_DBG("Phone ip: %d.%d.%d.%d\n",phone_ip[0],phone_ip[1],phone_ip[2],phone_ip[3]);
            }
            xTmrStop(TMR_NDJ_SC);				 	// xuejungong 20180723  stop timer for smartconfig

            smartconfig_stop();

            redLedFlash(00,100,0);

            xMarkSetValue(MARK_NDJ_SC,SET); // xuejungong 20180723 add smartconfig mark

            xTaskCreate(wifiTask, "wifiTask", 2*256, NULL,(tskIDLE_PRIORITY + 3), &WifiLinkHandle);

            xTaskCreate(TCP_Client, "server", 2048, NULL, (tskIDLE_PRIORITY + 2), &TcpClientHandle);

            break;
    }

}


/*******************************************************************************
*�������ƣ�void ICACHE_FLASH_ATTR smartconfigTask(void *pvParameters)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
void ICACHE_FLASH_ATTR smartconfigTask(void *pvParameters)
{
	if(TcpClientHandle != NULL)
	    vTaskDelete(TcpClientHandle);

	if(WifiLinkHandle != NULL)
		vTaskDelete(WifiLinkHandle);

	xMarkSetValue(MARK_DEVICE_LOGIN,RESET);
	xTmrStop(TMR_HEART);
	sendbuf.flag = RESET;

    smartconfig_start(smartconfig_done);

    vTaskDelete(NULL);
}


/*******************************************************************************
*�������ƣ� void clientSendBuf(const char *pTemp,uint16_t len)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
void clientSendBuf(const char *pTemp,uint16_t len)
{
	if((pTemp == NULL)||(len == 0x00))
		return;

	memset(&sendbuf,0x00,sizeof(sendbuf_t));

	memcpy(sendbuf.buff,pTemp,len);
	sendbuf.len = len;
	sendbuf.flag = SET;

    APP_TRACE_DBG("\r\n===============Send Cmd: %02s==========\r\n",pTemp+2);
}

/*******************************************************************************
*�������ƣ� void TCP_Client(void *pvParameter)
*����˵����
*���������
*���������
*�� �� ֵ ��
*����˵����
*******************************************************************************/
void TCP_Client(void *pvParameter)
{
    uint32_t date_len=0;
    err_t err,recv_err;
    static u16_t server_port,local_port;
    static ip_addr_t server_ipaddr,loca_ipaddr;
    struct pbuf *q;
    struct netconn *tcp_clientconn;

    char tcp_client_recvbuf[TCP_Client_RX_BUFSIZE +1];
//    xEventGroupWaitBits(wifi_event_group,CONNECTED_BIT,false,true,protMAX_DELAY);
    APP_TRACE_DBG("\r\n============TCP_Client is runnig!==============\r\n");
    LWIP_UNUSED_ARG(pvParameter);
    dns_init();
    server_port=pSettings->server.port;
    //IP4_ADDR(&server_ipaddr,118,89,21,146);

    if(strlen(pSettings->server.ip)== 0x04)
    {
    	IP4_ADDR(&server_ipaddr,pSettings->server.ip[0],
								 pSettings->server.ip[1],
								 pSettings->server.ip[2],
								 pSettings->server.ip[3]);
    }
    else
    {
    	if ((err = netconn_gethostbyname(pSettings->server.ip, &server_ipaddr)) == ERR_OK)
    	{
    		APP_TRACE_DBG("\r\n remote server ip:%d.%d.%d.%d\n",(server_ipaddr.addr&0xFF),
					(server_ipaddr.addr&0xFF00)>>8,
					(server_ipaddr.addr&0xFF0000)>>16,
					(server_ipaddr.addr&0xFF000000)>>24);
    	}
    	else
    	{

    	}
    }
    while(1)
    {
        tcp_clientconn = netconn_new(NETCONN_TCP);
        err = netconn_connect(tcp_clientconn,&server_ipaddr,server_port);

        if( err!=ERR_OK )
        {
            netconn_delete(tcp_clientconn);
        }
        else if(err == ERR_OK)
        {
            tcp_clientconn->recv_timeout=10;
            netconn_getaddr(tcp_clientconn,&loca_ipaddr,&local_port,1);
            APP_TRACE_DBG("\r\n=============Connect server=============\r\n");
            while(1)
            {
                struct netbuf *recvbuf;
                grnLedFlash(500,500,1);
                loginEvent();

                if(sendbuf.flag)
                {
                	sendbuf.flag = RESET;
                	APP_TRACE_DBG("\r\n==============Socket Send:==============\r\n");
                	APP_HEX_STRING_DBG(sendbuf.buff,sendbuf.len);
                	APP_TRACE_DBG("\r\n");
					err = netconn_write(tcp_clientconn,sendbuf.buff,sendbuf.len,NETCONN_COPY);

					if(err!= ERR_OK)
					{
						APP_TRACE_DBG(">>>>>>>Send error %d\r\n",err);
						goto NET_ERR;
					}
					else
					{
						APP_TRACE_DBG(">>>>>>>Send OK\r\n");
					}
                }

                if((recv_err=netconn_recv(tcp_clientconn,&recvbuf))==ERR_OK)
                {
                    memset(tcp_client_recvbuf,0,TCP_Client_RX_BUFSIZE);

                    for(q=recvbuf->p;q!=NULL;q=q->next)
                    {
                        if(q->len>(TCP_Client_RX_BUFSIZE-date_len))
                        {
                        	memcpy(tcp_client_recvbuf+date_len,q->payload,(TCP_Client_RX_BUFSIZE-date_len));
                        }
                        else
                        {
                        	memcpy(tcp_client_recvbuf+date_len,q->payload,q->len);
                        }

                        date_len+=q->len;

                        if(date_len > TCP_Client_RX_BUFSIZE)
                        	break;
                    }

                    grnLedFlash(100,100,1);
                    //��ӡ���ݰ�����
                    APP_TRACE_DBG("\r\n Socket Packet:");
                    APP_HEX_STRING_DBG(tcp_client_recvbuf,date_len);
                    APP_TRACE_DBG("\r\n");

                    lyyPacketCallback(tcp_client_recvbuf,date_len);
                    date_len=0;
                    //APP_TRACE_DBG("%s\r\n", tcp_client_recvbuf);
                    netbuf_delete(recvbuf);
                }
                else if(recv_err==ERR_CLSD)
                {
 NET_ERR:
 	 	 	 	 	xMarkSetValue(MARK_DEVICE_LOGIN,RESET);
                	xTmrStop(TMR_HEART);
                	sendbuf.flag = RESET;

                    netconn_close(tcp_clientconn);
                    netconn_delete(tcp_clientconn);
                    APP_TRACE_DBG("Close server\r\n");
                    break;
                }
                vTaskDelay(100/portTICK_RATE_MS);
            }
        }
    }
}




