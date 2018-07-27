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

#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#include "esp_common.h"
#include "esp_system.h"
#include "gpio.h"
#include "uart.h"


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#include "user_task.h"
#include "../user_driver/ringbuf.h"
#include "../user_driver/tmr.h"
#include "../user_driver/xTriger.h"
#include "../user_driver/xCtrl.h"
#include "../user_driver/xled.h"
#include "../user_driver/param.h"
#include "../user_driver/xmark.h"
#include "../user_driver/xEvent.h"
#include "../user_driver/xTmr.h"
#include "../user_driver/sysInfor.h"
#include "../user_driver/ringEvent.h"
#include "../user_driver/timebase.h"
#include "../user_driver/dbg.h"
#include "../user_driver/coin.h"
#include "../user_driver/lyy.h"

// xuejungong 20180717 add
//#include "../user_driver/connect.h"

//#define ID_SETTING
#define DEBUG
#define TEST_SERVER                 // xuejungong 20180720 Test
//#define DESIGN_SERVER               // xuejungong 20180720 Design


#define TICKS_PERIOD_MS				portTICK_RATE_MS
#define MAX_BLOCK_TIME				100
#define __UINT32_T_MAX__			0xFFFFFFFF
#define SET							pdTRUE
#define RESET						pdFALSE

#define memMalloc					os_malloc
#define memFree						os_free
#define sysGetTick					xTaskGetTickCount


#define LINK_HEART_PERIOD			40
#define LINK_RDY_PERIOD				30
#define LINK_CSQ_PERIOD				60
#define LINK_COIN_PERIOD			10

// xuejungong 20180718 add ndj check status 5min
#define NDJ_CHECK_PERIOD			300
// xuejungong 20180718 add smartconfig check status 20s
#define NDJ_SC_PERIOD				20

#define UPGRADE_PACKET_SIZE			512
#define SECTOR_SIZE					4096  //一个扇区4K

#define UPGRADE_FLAG_IDLE           0x00
#define UPGRADE_FLAG_START          0x01
#define UPGRADE_FLAG_FINISH         0x02

#define PARAM_SPIFLASH_SEC			0x3A0
#define UPGRADE_SPIFLASH_SEC		(PARAM_SPIFLASH_SEC + 4)

//xuejungong 20180614 add 扭蛋机 子设备升级包存储位置
#define UPGRADE_INFOR_ADDR                         	0x201000  // xuejungong 20180725  固件参数存储区
#define UPGRADE_INFOR_ADDR_NDJ                     	0x202000  // xuejungong 20180725  扭蛋机固件参数存储区
#define UPGRADE_DATA_ADDR_NDJ                      	0x203000  // xuejungong 20180725  扭蛋机固件数据存储区
#define IDCONFIG_ADDR                     			0x21C000  // xuejungong 20180725 idconfig标志位
#define UPGRADE_PACKET_SIZE_NDJ                     512

//#define AP_SSID						"leyaoyao2.4"
//#define AP_PASSWORD					"leyaoyao168"


#define grnLedFlash(ontime,offtime,count)         { if(xLedIsOK(GRN_LED)) xLedStart(1<<GRN_LED,ontime,offtime,count);}
#define redLedFlash(ontime,offtime,count)         { if(xLedIsOK(RED_LED)) xLedStart(1<<RED_LED,ontime,offtime,count);}
#define ledFlash(ontime,offtime,count)			  { xLedStart(1<<GRN_LED|1<<RED_LED,ontime,offtime,count);}


#define MAX_RING_PACKET_SIZE		256
#define MAX_DONE_TIMEOUT			10000
typedef struct{
   char			buff[MAX_RING_PACKET_SIZE];
   uint16_t 	len;
}ringPacket_t;

#endif

