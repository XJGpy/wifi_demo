/*******************                    �� �� �� �� �� ˾ *********************/
/* �������ƣ�					                      	                      */
/* �ļ����ƣ�   					      		                              */
/* ��    �ߣ�			 					                                  */
/* ������ڣ�								                                  */
/* ��ǰ�汾��						      		                              */
/* ���¼�¼: 								                                  */
/******************************************************************************/
#ifndef __PARAM_H__
#define __PARAM_H__

#ifdef __cplusplus
extern "C"
{
#endif 

#include "connect.h"    // xuejungong 20180615 add �������������������ͷ�ļ�

#define MAX_NDJ_NUM             16   // xuejungong 20180623 һ��ͨѶģ������Ӧ16����������
  
/*******************************************/    
#define MAX_ID_NUM              16

/*******************************************/  
#define MAX_PHONE_NUM           32
#define MAX_PHONE_COUNT         3
  typedef struct{
    char    phone[MAX_PHONE_NUM];
  }phone_t;
  
/*******************************************/ 
#define MAX_APN_NUM             32
#define MAX_USER_NUM            32
#define MAX_PASS_NUM            32

  typedef struct{
    char       apn[MAX_APN_NUM];
    char       user[MAX_USER_NUM];
    char       pass[MAX_PASS_NUM];
  }link_t;

/*******************************************/
#define MAX_SERVER_IP_NUM       32
#define MAX_SERVER_COUNT        2
  typedef struct{
    char        ip[MAX_SERVER_IP_NUM];
    uint16_t    port;
  }server_t; 
  
/*******************************************/
#define FTP_USER_COUNT                  64
#define FTP_PASS_COUNT                  64
#define FTP_FILE_COUNT                  64
  typedef struct{
    server_t            server;
    char                user[FTP_USER_COUNT];
    char                pass[FTP_PASS_COUNT];
    char                file[FTP_FILE_COUNT];
  }ftp_t;
  
  
/*******************************************/
#define MAX_SEND_COUNT          3
#define MAX_SEND_WAIT           30000
  typedef struct{
    char        type;
    char        count;
    uint32_t     wait;
  }send_t;
    
/**/
#define MAX_BILL_DEVICE_COUNT                   0x02
#define DEFAULT_PLUSE_TIME                      60
#define DEFAULT_PLUSE_REDIAL                    60
  
  typedef enum{
    COIN_HIGH,
    COIN_LOW,
  }leverTypedef;
  
  typedef struct{
    leverTypedef        value;
    uint16_t            pulse;
    uint16_t            redial;
  }coinParam_t;
  
  
  typedef enum{
    DEVICE_ICT = 1,
    DEVICE_ITL,
    DEVICE_PULSE,
    DEVICE_ICT_ITL,
    DEVICE_ICT_PULSE,
    DEVICE_ITL_PULSE,
    DEVICE_ST2,
  }billDeviceTypedef;

 
  typedef struct{
    billDeviceTypedef    bill_accepter[MAX_BILL_DEVICE_COUNT];               //
  }billSys_t;


  
/*******************************************/  
#define MAX_NET_KEY_SIZE                        16
#define MAX_USERNAME_SIZE                       8
#define MAX_PASSWOR_SIZE                        8
  typedef struct{
    char        key[MAX_NET_KEY_SIZE];
    char        user[MAX_USERNAME_SIZE];
    char        pass[MAX_PASSWOR_SIZE];
  }privatKey_t;
  
/*******************************************/ 
  typedef struct{
    uint32_t            coin_total;
    uint32_t            gift_total;
    uint32_t            pay_total;
  }notesTotal_t;
  
  /****************************************/
   typedef struct{
       char   uart_type;
       char   uart_brate;
   }interface_t;

/*******************************************/  
  typedef struct{
    uint32_t            crc;
    char               id[MAX_ID_NUM];
    server_t            server;                     //��������Ϣ
    ftp_t               ftp;                        //FTP��Ϣ
    link_t              link;                       //GPRS APN
    send_t              send;                       //������Ϣ            
    billSys_t           bill;                       //ֽ����������Ϣ
    coinParam_t         coin;                       //��ƽ���
    privatKey_t         key;                        //ͨ�����뼰�û�����
    notesTotal_t        note;                       //���׼�¼
    interface_t         interface;                  //�ӿڲ���
#ifdef NDJ_Device
    char              ndj_num;                    // Ť�����洢��ţ����ܳ���16�����������0��ʼ��
    NDJ_Para          ndj_para[MAX_NDJ_NUM];      // xuejungong 20180615 ���豸����
#endif
  }__attribute__((aligned(4), packed)) param_t;
  
typedef void (*paraRead_f)(param_t *pInfor);  
typedef int  (*paraWrite_f)( void*,uint16_t);

/*******************************************/ 
typedef struct{
  param_t          param;
  paraWrite_f      write;
  paraRead_f       read;
  
}paramInfor_t;


extern param_t*pSettings;


/*******************************************************************************
*�������ƣ�void paramInit(paraRead_f read,paraWrite_f write)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void  paramInit(paraRead_f read,paraWrite_f write);


/*******************************************************************************
*�������ƣ�void paramDefault(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void  paramDefault(void);


/*******************************************************************************
*�������ƣ�int paramSave(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern int  paramSave(void);


#ifdef __cplusplus
}
#endif


#endif
