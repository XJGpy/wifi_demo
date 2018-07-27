/*******************                    �� �� �� �� �� ˾ *********************/
/* �������ƣ�					                      	      */
/* �ļ����ƣ�   					      		      */
/* ��    �ߣ�			 					      */
/* ������ڣ�								      */
/* ��ǰ�汾��						      		      */
/* ���¼�¼: 								      */
/******************************************************************************/
#ifndef __CONNECT_H__
#define __CONNECT_H__

// xuejungong  20180613 add timer 50ms
#define MAX_PACKET_TIME								50
#define NDJ_Device									1

// xuejungong 20180613 add for NDJ(Ť������
#ifdef NDJ_Device

#define CONNECT_UART_NUM                            UART_NUM_3

#define NDJ_CMD                                     0x34
#define MAX_CONNECT_PACKET_SIZE                     1024

#define NDJ_CHECK_S                                 300*1000    // 5min

#define NDJ_DEVICE_MARK                             0x15      // Ť�����豸����

typedef struct{
	char            sn_id[8]; 		 // �豸Ψһ��
    char        	addr_ID;    // �豸ID
    char       		cmd;	    // ����
    char			buff[MAX_CONNECT_PACKET_SIZE];// ��������
}__attribute__((aligned(1), packed)) NDJ_Packet;

typedef struct{
	char			id[8];    	// ���豸Ψһ��
	char            sw[14];		// �̼��汾��
	char            sta;	  	// ���豸״̬��0���ߣ�1����
	char            err;		// ���豸״̬û��Ӧ�����

//	char            sw[4];		// �̼��汾��
//	char            hw[4]; 		// Ӳ���汾��
//	char            timer;   	// һ����Ϸʱ��
//	char       		coin_num;	// һ���������
}__attribute__((aligned(1), packed)) NDJ_Para;// Ť��������

typedef struct{
	char            mark;    // �̼�����λ���Ƿ��й̼�
    uint16_t        flow;    // ����������
    uint16_t        buff_size;   // ��������С
    char            ves[14]; // �̼��汾��
    char            bk[4];   // ����Ԥ����
}__attribute__((aligned(1), packed)) NDJ_SWPara;//����������

typedef struct{
    uint16_t        flow;    	// ����������
    uint16_t        buff_size;  // ��������С
    char            ves[14]; 	// �̼��汾��
	uint16_t        index;    	// ���������
	uint16_t        len;      	// ����������
    char            buf[512];   // ����������
}__attribute__((aligned(1), packed)) NDJ_SWData;//����������

typedef struct{
	char			incode;     // ������
	char            sta;		// ״̬
	uint16_t        inc;    	// ����
    uint16_t        total;  	// ����
}__attribute__((aligned(1), packed)) NDJ_Bill;//�˵���Ϣ

// xuejungong  20180623 add ��ȡ�����̼�����
  typedef struct{
    char          	sn_id[8];
    char          	id;
    char            cmd;
    char       		mark;
  }ndj_UpgradePara_t;

  // xuejungong  20180623 add ͬ���̼�����
  typedef struct{
	  char          sn_id[8];
	  char          id;
	  char          cmd;
	  NDJ_SWPara    ndj_swp;
  }__attribute__((aligned(1), packed)) ndj_respPara_t;

  // xuejungong  20180623 add ͬ���̼�����
  typedef struct{
	  char          sn_id[8];
	  char          id;
	  char          cmd;
	  NDJ_SWData    ndj_swd;
  }__attribute__((aligned(1), packed)) ndj_UpgradeData_t;

  typedef  struct{
	char            sn_id[8]; 		 // �豸Ψһ��
  	char            sn_cmd[6]; 		 // ����Ψһ��
  	char            buf[MAX_CONNECT_PACKET_SIZE];		//  ��������

  }ndj_lyypacket_t;

// �ն�Ӧ��  xuejungong 20180625 add
  typedef  struct{
	  char          sn_cmd[6];
	  char            cmd; 		 //
	  uint16_t       index;
	  char            len;
	  char           buf[64];
  }__attribute__((aligned(1), packed)) ndj_Option_t;

  // xuejungong 20180628 �����ϱ�Ť����������
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
#define MSG_ID_PAY_MSG                      MSG_ID_USER_START+0x100  // ֧���Ϸ�
#define MSG_ID_BILL_MSG                     MSG_ID_USER_START+0x101  // �˵�
#define MSG_ID_REF_MSG                    	MSG_ID_USER_START+0x102  // �˿�
#define MSG_ID_OPT_MSG                    	MSG_ID_USER_START+0x103  // �������豸
#define MSG_ID_UPGRADE_MSG                  MSG_ID_USER_START+0x104  // �������豸
#define MSG_ID_SET_REVSION_MSG              MSG_ID_USER_START+0x105	 // ���ù̼�����
#define MSG_ID_GET_REVSION_MSG              MSG_ID_USER_START+0x106  // ��ȡ�̼�����
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
*�������ƣ�void connectBaudInit(pConnect_f callback)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
extern void connectBaudInit(void);



/*******************************************************************************
*�������ƣ�void connectInit(pConnect_f callback)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
extern void connectInit(pConnect_f callback);




/*******************************************************************************
*�������ƣ�void connectSerialHandle(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void connectSerialHandle(void);




/*******************************************************************************
*�������ƣ�void setLoginMark(char mark)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
extern void setLoginMark(char mark);




/*******************************************************************************
*�������ƣ�char getLoginMark(void)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
extern char getLoginMark(void);



/*******************************************************************************
*�������ƣ�void connectTimeTransAccount(void)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
extern void connectTimeTransAccount(void);




/*******************************************************************************
*�������ƣ�void connectTimeGetAccountInc(void)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
extern void connectTimeGetAccountInc(void);




/*******************************************************************************
*�������ƣ�void connectTimeGetAccountTotal(void)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
extern void connectTimeGetAccountTotal(void);




/*******************************************************************************
*�������ƣ�void connectParse(const char *pTemp,uint16_t len)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
extern void connectParse(const char *pTemp,uint16_t len);



/*******************************************************************************
*�������ƣ�void connectSendMsg(u32 msgId,u32 param1,u32 param2)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
extern void connectSendMsg(u32 msgId,u32 param1,u32 param2);
//extern void connectSendMsg(u32 msgId,char *param1,u32 param2);



/*******************************************************************************
*�������ƣ�portTASK_FUNCTION_PROTO(connectTask, pvParameters)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
extern portTASK_FUNCTION_PROTO(connectTask, pvParameters);


// xuejungong 20180619 ����ʹ�ã�ֻ���ͣ�������
char connectReplyRefund_NDJ(char id, uint8_t mode);
char connectPay_NDJ(char id);
// xuejungong 20180621 add
extern char connectRecCheck(void);
extern int connectSnIDCheck(char *pSn_id);
extern char connectSWCheck(char *pSn_id, char *pSn_ves);
//extern char connectSnIDCheck(char *pSn_id);
extern void connectOption(char *pTemp,uint16_t len);

extern void ndj_TimerCallback(void);// ���Ť����״̬
//void ndj_TimerCallback(TimerHandle_t pxTimer);// ���Ť����״̬

extern int connectCallback(const void *pTemp,uint16_t len,char repeatFlag);
//extern void connectParse(const char *pTemp,uint16_t len);

#endif
