/*******************                    科 技 有 限 公 司 *********************/
/* 工程名称：					                      	      */
/* 文件名称：   					      		      */
/* 作    者：			 					      */
/* 完成日期：								      */
/* 当前版本：						      		      */
/* 更新记录: 								      */
/******************************************************************************/
#ifndef __CONNECT_H__
#define __CONNECT_H__

// xuejungong  20180613 add timer 50ms
#define MAX_PACKET_TIME								50
#define NDJ_Device									1

// xuejungong 20180613 add for NDJ(扭蛋机）
#ifdef NDJ_Device

#define CONNECT_UART_NUM                            UART_NUM_3

#define NDJ_CMD                                     0x34
#define MAX_CONNECT_PACKET_SIZE                     1024

#define NDJ_CHECK_S                                 300*1000    // 5min

#define NDJ_DEVICE_MARK                             0x15      // 扭蛋机设备类型

typedef struct{
	char            sn_id[8]; 		 // 设备唯一码
    char        	addr_ID;    // 设备ID
    char       		cmd;	    // 命令
    char			buff[MAX_CONNECT_PACKET_SIZE];// 数据区域
}__attribute__((aligned(1), packed)) NDJ_Packet;

typedef struct{
	char			id[8];    	// 子设备唯一码
	char            sw[14];		// 固件版本号
	char            sta;	  	// 子设备状态，0在线，1离线
	char            err;		// 子设备状态没有应答次数

//	char            sw[4];		// 固件版本号
//	char            hw[4]; 		// 硬件版本号
//	char            timer;   	// 一局游戏时间
//	char       		coin_num;	// 一局所需币数
}__attribute__((aligned(1), packed)) NDJ_Para;// 扭蛋机参数

typedef struct{
	char            mark;    // 固件控制位，是否有固件
    uint16_t        flow;    // 数据流数量
    uint16_t        buff_size;   // 升级包大小
    char            ves[14]; // 固件版本号
    char            bk[4];   // 数据预留区
}__attribute__((aligned(1), packed)) NDJ_SWPara;//升级包参数

typedef struct{
    uint16_t        flow;    	// 数据流数量
    uint16_t        buff_size;  // 升级包大小
    char            ves[14]; 	// 固件版本号
	uint16_t        index;    	// 升级包序号
	uint16_t        len;      	// 升级包长度
    char            buf[512];   // 升级包数据
}__attribute__((aligned(1), packed)) NDJ_SWData;//升级包数据

typedef struct{
	char			incode;     // 自增码
	char            sta;		// 状态
	uint16_t        inc;    	// 增量
    uint16_t        total;  	// 总量
}__attribute__((aligned(1), packed)) NDJ_Bill;//账单信息

// xuejungong  20180623 add 获取升级固件参数
  typedef struct{
    char          	sn_id[8];
    char          	id;
    char            cmd;
    char       		mark;
  }ndj_UpgradePara_t;

  // xuejungong  20180623 add 同步固件参数
  typedef struct{
	  char          sn_id[8];
	  char          id;
	  char          cmd;
	  NDJ_SWPara    ndj_swp;
  }__attribute__((aligned(1), packed)) ndj_respPara_t;

  // xuejungong  20180623 add 同步固件参数
  typedef struct{
	  char          sn_id[8];
	  char          id;
	  char          cmd;
	  NDJ_SWData    ndj_swd;
  }__attribute__((aligned(1), packed)) ndj_UpgradeData_t;

  typedef  struct{
	char            sn_id[8]; 		 // 设备唯一码
  	char            sn_cmd[6]; 		 // 命令唯一码
  	char            buf[MAX_CONNECT_PACKET_SIZE];		//  发送数据

  }ndj_lyypacket_t;

// 终端应答  xuejungong 20180625 add
  typedef  struct{
	  char          sn_cmd[6];
	  char            cmd; 		 //
	  uint16_t       index;
	  char            len;
	  char           buf[64];
  }__attribute__((aligned(1), packed)) ndj_Option_t;

  // xuejungong 20180628 用于上报扭蛋机不在线
  typedef  struct{
	  char          	sn_cmd[6];
	  char            	cmd; 		 //
	  uint16_t       	index;
	  char            	len;
	  char          	sn_id[8];
	  char              ndj_id;
	  char              ndj_cmd;
	  char              ndj_sta;
  }__attribute__((aligned(1), packed)) ndj_Option_Status_t;

#else
#define MAX_CONNECT_PACKET_SIZE                     64
#define CONNECT_UART_NUM                            UART_NUM_4
#endif

#define CONNECT_BAUD                                38400

#define MAX_CONNECT_SEND_COUNT                      4
//#define MAX_CONNECT_PACKET_SIZE                     64   //xuejungong 20180613 add '//'
#define CONNECT_HEAD_MARK                           0xAA
#define CONNECT_TAIL_MARK                           0xDD


typedef union{
    struct {
        char        head;
        char        len;
        char    	index;
        char        cmd;
    }packet;
    char     buff[MAX_CONNECT_PACKET_SIZE];
}__attribute__((aligned(1), packed)) connectPacket_t;
        

//
#define MSG_ID_USER_START                   0x1000
#define MSG_ID_PAY_MSG                      MSG_ID_USER_START+0x100  // 支付上分
#define MSG_ID_BILL_MSG                     MSG_ID_USER_START+0x101  // 账单
#define MSG_ID_REF_MSG                    	MSG_ID_USER_START+0x102  // 退款
#define MSG_ID_OPT_MSG                    	MSG_ID_USER_START+0x103  // 控制子设备
#define MSG_ID_UPGRADE_MSG                  MSG_ID_USER_START+0x104  // 升级子设备
#define MSG_ID_SET_REVSION_MSG              MSG_ID_USER_START+0x105	 // 设置固件参数
#define MSG_ID_GET_REVSION_MSG              MSG_ID_USER_START+0x106  // 获取固件参数
#define MSG_ID_HEART_MSG                    MSG_ID_USER_START+0x107


#define MAX_CONNECT_MSG_COUNT               30

typedef struct{
    uint32_t  message;
    //char	  param1[];
    uint32_t  param1;
    uint32_t  param2;
}__attribute__((aligned(1), packed)) connectMsg_t;



typedef int (*pConnect_f)(const void *pTemp,uint16_t len,char repeatFlag);



/*******************************************************************************
*函数名称：void connectBaudInit(pConnect_f callback)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
extern void connectBaudInit(void);



/*******************************************************************************
*函数名称：void connectInit(pConnect_f callback)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
extern void connectInit(pConnect_f callback);




/*******************************************************************************
*函数名称：void connectSerialHandle(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void connectSerialHandle(void);




/*******************************************************************************
*函数名称：void setLoginMark(char mark)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
extern void setLoginMark(char mark);




/*******************************************************************************
*函数名称：char getLoginMark(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
extern char getLoginMark(void);



/*******************************************************************************
*函数名称：void connectTimeTransAccount(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
extern void connectTimeTransAccount(void);




/*******************************************************************************
*函数名称：void connectTimeGetAccountInc(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
extern void connectTimeGetAccountInc(void);




/*******************************************************************************
*函数名称：void connectTimeGetAccountTotal(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
extern void connectTimeGetAccountTotal(void);




/*******************************************************************************
*函数名称：void connectParse(const char *pTemp,uint16_t len)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
extern void connectParse(const char *pTemp,uint16_t len);



/*******************************************************************************
*函数名称：void connectSendMsg(u32 msgId,u32 param1,u32 param2)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
extern void connectSendMsg(u32 msgId,u32 param1,u32 param2);
//extern void connectSendMsg(u32 msgId,char *param1,u32 param2);



/*******************************************************************************
*函数名称：portTASK_FUNCTION_PROTO(connectTask, pvParameters)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
extern portTASK_FUNCTION_PROTO(connectTask, pvParameters);


// xuejungong 20180619 测试使用，只发送，不接收
char connectReplyRefund_NDJ(char id, uint8_t mode);
char connectPay_NDJ(char id);
// xuejungong 20180621 add
extern char connectRecCheck(void);
extern int connectSnIDCheck(char *pSn_id);
extern char connectSWCheck(char *pSn_id, char *pSn_ves);
//extern char connectSnIDCheck(char *pSn_id);
extern void connectOption(char *pTemp,uint16_t len);

extern void ndj_TimerCallback(void);// 检测扭蛋机状态
//void ndj_TimerCallback(TimerHandle_t pxTimer);// 检测扭蛋机状态

extern int connectCallback(const void *pTemp,uint16_t len,char repeatFlag);
//extern void connectParse(const char *pTemp,uint16_t len);

#endif
