/*******************                    科 技 有 限 公 司 *********************/
/* 工程名称：					                      	                      */
/* 文件名称：   					      		                              */
/* 作    者：			 					                                  */
/* 完成日期：								                                  */
/* 当前版本：						      		                              */
/* 更新记录: 								                                  */
/******************************************************************************/
#ifndef __PARAM_H__
#define __PARAM_H__

#ifdef __cplusplus
extern "C"
{
#endif 

#include "connect.h"    // xuejungong 20180615 add 添加升级包参数引进该头文件

#define MAX_NDJ_NUM             16   // xuejungong 20180623 一个通讯模块最多对应16个控制主板
  
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
    server_t            server;                     //服务器信息
    ftp_t               ftp;                        //FTP信息
    link_t              link;                       //GPRS APN
    send_t              send;                       //发送信息            
    billSys_t           bill;                       //纸钞机配置信息
    coinParam_t         coin;                       //电平检测
    privatKey_t         key;                        //通信密码及用户密码
    notesTotal_t        note;                       //交易记录
    interface_t         interface;                  //接口参数
#ifdef NDJ_Device
    char              ndj_num;                    // 扭蛋机存储序号，不能超过16个，超过则从0开始；
    NDJ_Para          ndj_para[MAX_NDJ_NUM];      // xuejungong 20180615 子设备参数
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
*函数名称：void paramInit(paraRead_f read,paraWrite_f write)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void  paramInit(paraRead_f read,paraWrite_f write);


/*******************************************************************************
*函数名称：void paramDefault(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void  paramDefault(void);


/*******************************************************************************
*函数名称：int paramSave(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern int  paramSave(void);


#ifdef __cplusplus
}
#endif


#endif
