/*******************                    科 技 有 限 公 司 *********************/
/* 工程名称：					                      	      */
/* 文件名称：   					      		      */
/* 作    者：			 					      */
/* 完成日期：								      */
/* 当前版本：						      		      */
/* 更新记录: 								      */
/******************************************************************************/
#ifndef __LYY_H__
#define __LYY_H__

#ifdef __cplusplus
extern "C"
{
#endif

//#include "socket.h"
  
#define LYY_HEAD_MARK                   "*#"
#define LYY_TAIL_MARK                   "\\n"
  
  
#define FW_DOWNLOAD_OK                  1
#define FW_UPGRADE                      0x8A 
  
  
//#define LYY_DEVICE_MARK                 "\x00\x07"
#define LYY_DEVICE_MARK                 "\x00\x14"    // xuejungong 20180718 change


#define MAX_WRITE_PACKET_SIZE            128
#define MAX_READ_PACKET_SIZE             640
#define MAX_ID_SIZE                      8
  
  
#define DOWNLOAD_ERR          0XDE
  
/**/
  typedef union{
    struct{
      char              head[2];
      char              attr[2];
      uint16_t           len;
      char              id[MAX_ID_SIZE];
    }packet;
    char     buff[MAX_READ_PACKET_SIZE];
  }lyy_read_t;
  
  
  /**/
  typedef union{
    struct{
      char              head[2];
      char              attr[2];
      uint16_t          len;
      char              id[MAX_ID_SIZE];
    }packet;
    char     buff[MAX_WRITE_PACKET_SIZE];
  }lyy_write_t;
  
/*登录数据包 A1 */
  typedef  struct{
    char        login[2];
    uint16_t     version;
    char        device;                 //终端标示
    char        pass[8];
  }__attribute__((aligned(1), packed)) lyylogin_t;
   
/*投实币指令 A2 A3 A4*/
  typedef  struct{
    char            sn[6];
    uint32_t        total;
    uint16_t        inc;
    char            year;
    char            month;
    char            day;
    char            hour;
    char            min;
    char            sec;
  }__attribute__((aligned(1), packed))lyyInfor_t;
  
  
 /*通用应答指令 A6*/
  
  typedef enum{
    RESP_PAY            = 0x01,
    RESP_TEST_PAY       = 0x02,
    RESP_MB             = 0x03,
  }respTypeDef;
  
  typedef struct{
    char            sn[6];
    char            ans;
  }__attribute__((aligned(1), packed)) lyyresp_t;
  
  /*CCID指令 A7*/
  typedef  struct{
    char            result;
    char            ccid[20];
  }__attribute__((aligned(1), packed)) lyyccid_t;
  
  /*IP指令 A8*/
  typedef  struct{
    char            result;
    char            ip[30];
  }__attribute__((aligned(1), packed)) lyyip_t;
  
  
   /*GPS指令 A9*/
  typedef  struct{
    char            result;
    char            gps[50];
  }__attribute__((aligned(1), packed)) lyygps_t;
  
  
 /*控制参数指令 AA*/
  typedef  struct{
    uint16_t            width_1;
    uint16_t            redial_1;
    uint16_t            width_2;
    uint16_t            redial_2;
    char                lever;
    char                tl;
    char                zl;
    char                bill;
    char                monny;
    char                interface;
    char                baud;
    char                rev[2];
  }__attribute__((aligned(1), packed)) lyyparam_t;
  

    /*升级数据应答 AC*/
  typedef  struct{
    char                sn[6];
    char                ans;
    uint16_t             no;
  }__attribute__((aligned(1), packed)) upgradeans_t;
  
  
   /*升级固件参数上传 AE*/
  typedef  struct{
    char                mark;
    char                revision[14];
    uint16_t            no;
    char                rev[6];
  }__attribute__((aligned(1), packed)) revision_t;
  
  
  /*信号强度 AF*/
  typedef  struct{
    char                value;
    char                rev[3];
  }__attribute__((aligned(1), packed)) lyyrssi_t;
  
  
    /*故障AG*/
  typedef  struct{
    char                sn[6];
    char                code;
    char                bill;
    uint16_t            error;
    char                year;
    char                month;
    char                day;
    char                hour;
    char                min;
    char                sec;
  }__attribute__((aligned(1), packed)) lyyerror_t;


typedef struct
{
    u32   S_cpuid[3];  //?????è±?ID
    u8    ErrorCode;   //?í?ó?ú??
    u32   Account_LocalCoinSum; //?????ú????×???
    u32   Account_OutPrizeSum;  //????×???   
    u16   Account_GamesSum;     //???·×???
    u8    Account_SurplusGames; //???à???·????
    u16   Account_SurplusCoin;  //???à±???
    u32   Account_GetNoteSum;   //??±??ú????×???
    u32   Account_BackCoinSum;  //??±?×???
    u32   Account_FreeCoinSum;  //??·???±?×???
    
    u16   Account_LocalCoin;    //??±?????
    u16   Account_OutPrize;     //????????    
    u32   Account_NetCoinSum;   //??????±?×???   
    u16   Account_GetNote;      //??±?????    
    u16   Account_BackCoin;     //??±?????        
}__attribute__((aligned(1), packed)) note_infor_t;
  
  
  /*账目数据AH*/
  typedef  struct{
    char                sn[6];
    char                note[50];
    char                year;
    char                month;
    char                day;
    char                hour;
    char                min;
    char                sec;
    char                rev[2];
  }__attribute__((aligned(1), packed)) note_t;
  
  
    /*通信密钥及用户密码AL*/
  typedef  struct{
    char                sn[6];
    char                key[16];
    char                pass[8];
  }__attribute__((aligned(1), packed)) lyynet_t;


    /*终端参数上传命令应答 AJ*/
  typedef  union{
    struct{
      char            sn[6];
      uint16_t        device;
      char            len;
    } __attribute__((aligned(1), packed))packet;
    char      buff[256];
    
  }lyyTermialParam_t;



  /*终端操作命令应答 AK*/
  typedef  union{
    struct{
      char            sn[6];
      char            func;
      uint16_t        device;
      char            len;
    }__attribute__((aligned(1), packed)) packet;
    char      buff[256];
    
  }lyyTermialOption_t;
  

  /*子设备升级固件参数上传 AM*/   // xuejungong 20180705  add
 typedef struct{
   char                sn_id[8];
   char                mark;
   char                revision[14];
   uint16_t            no;
   char                rev[6];
 }__attribute__((aligned(1), packed)) ndj_revision_t;

 /*子设备升级固件参数上传 AP*/   // xuejungong 20180705  add
 typedef struct{
   char                	sn_id[8];
   char            		sn_cmd[6];
   char            		ans;
 }__attribute__((aligned(1), packed)) ndj_lyyresp_t;

 /*子设备升级固件参数上传 AQ*/   // xuejungong 20180705  add
 typedef struct{
   char                sn_id[8];
   char                sn[6];
   char                ans;
   uint16_t             no;
 }__attribute__((aligned(1), packed)) ndj_upgradeans_t;

  /*******************************************************/
  
  /*移动支付命令 B1*/
  typedef  struct{
    char                sn[6];
    uint16_t            value;
  }__attribute__((aligned(1), packed)) lyycloudpay_t;
  
  
    
  /*时间同步命令 B2*/
  typedef  struct{
    char                sec;
    char                min;
    char                hour;
    char                day;
    char                mon;
    char                week;
    char                year;
  }__attribute__((aligned(1), packed)) lyysync_t;
  
  
  
  /*域名修改 B3*/
  typedef  struct{
    char                ip[30];
  }__attribute__((aligned(1), packed)) lyysetip_t;
  
  
  /*设置投币数、礼品、上分数 B6*/
  typedef  struct{
    char                opt;
    uint16_t            value;
  }__attribute__((aligned(1), packed)) lyyrest_t;
  
  
  /*查询参数 B7*/
  typedef  struct{
    char                opt;
  }__attribute__((aligned(1), packed)) lyycheck_t;
  
  /*升级控制命令BA*/
  typedef  struct{
    char                mark;
    uint16_t            packet;
    uint16_t            total;
    char                revision[14];
    char                rev[4];
  }__attribute__((aligned(1), packed)) upgradectrl_t;
  
  
  /*升级数据流*/
  typedef  struct{
    uint16_t            index;
    char                data[512];
  }__attribute__((aligned(1), packed)) upgradedata_t;


  /**/
  typedef union{
    struct{
      char                sn[6];
      char                func;
      char                len;
    }packet;
     char                 buff[256];
  }__attribute__((aligned(1), packed)) option_t;
  
  
  
typedef int (*pLyySend_t)(const void *pTemp,uint16_t len,char repeatFlag);


typedef struct {
  uint16_t              value;
  char                  hw[64];
  char                  sw[64];
}__attribute__((aligned(1), packed)) version_t;

extern const version_t   SoftwareVersion;
  
/*******************************************************************************
*函数名称：void lyyInit(pLyySend_t sendCallback)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void lyyInit(pLyySend_t sendCallback);



/*******************************************************************************
*函数名称：void lyyParse(const socketInfor_t *pSocketInfor,const char *pTemp,uint16_t len)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void lyyParse(const char *pTemp,uint16_t len);
  
  
  
/*******************************************************************************
*函数名称：void lyyNetPacket(const char *pKey,const char *pPass)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void lyyNetPacket(const char *pKey,const char *pPass);



/*******************************************************************************
*函数名称：void lyyLoginPacket(const char *pPass)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void lyyLoginPacket(const char *pPass);


/*******************************************************************************
*函数名称：void lyyLoginPacket(const char *pPass)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void lyyHeartPacket(char type);


/*******************************************************************************
*函数名称：void lyyComeinPacket(const char *pAttr,const uint32_t total,uint16_t inc)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void lyyComeinPacket(const char *pAttr,const uint32_t total,uint16_t inc);


#define lyyCoinPacket(total,inc)  lyyComeinPacket("A2",total,inc)
#define lyyGiftPacket(total,inc)  lyyComeinPacket("A3",total,inc)
#define lyyPayPacket(total,inc)   lyyComeinPacket("A4",total,inc)


/*******************************************************************************
*函数名称：void lyyRssiPacket(const char value)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void lyyRssiPacket(const char value);



  
/*******************************************************************************
*函数名称：void lyyGpsPacket(const char *pGpsInfor)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void lyyGpsPacket(const char *pGpsInfor); 


/*******************************************************************************
*函数名称：void lyyPacketCallback(const char *pBuff,uint16_t len)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
extern void lyyPacketCallback(const char *pBuff,uint16_t len);


/*******************************************************************************
*函数名称：void payEventDone(char *pSn,char value)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
extern void payEventDone(char *pSn,char value);

/*******************************************************************************
*函数名称：
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
extern uint32_t upgradeChecksum(const char *pTemp,uint16_t len);

/*******************************************************************************
*函数名称：void lyyErrorPacket(uint8_t errcode,uint16_t errdata)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
extern void lyyErrorPacket(uint8_t errcode,uint8_t bill,uint16_t errdata);


#ifdef NDJ_Device
/*******************************************************************************
*函数名称：void lyyRssiPacket_NDJ(const char *pTemp,uint16_t len)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
//extern void lyyRevsionPacket_ndj(const char *pTemp, uint16_t len);

/*******************************************************************************
*函数名称：void lyyDeviceRespPacket(char type,const char *pSn)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
//extern void lyyDeviceRespPacket_ndj(const char *pTemp, uint16_t len);

/*******************************************************************************
*函数名称：void lyyUpgradeRespPacket_ndj(char type,const char *pSn)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：
*其他说明：
*******************************************************************************/
//extern void lyyUpgradeRespPacket_ndj(const char *pTemp, uint16_t len);

extern void lyyTermialAnswer(const char *pTemp,uint16_t len);

#endif


  
  
#ifdef __cplusplus
}
#endif	


#endif
