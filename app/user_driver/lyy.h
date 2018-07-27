/*******************                    �� �� �� �� �� ˾ *********************/
/* �������ƣ�					                      	      */
/* �ļ����ƣ�   					      		      */
/* ��    �ߣ�			 					      */
/* ������ڣ�								      */
/* ��ǰ�汾��						      		      */
/* ���¼�¼: 								      */
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
  
/*��¼���ݰ� A1 */
  typedef  struct{
    char        login[2];
    uint16_t     version;
    char        device;                 //�ն˱�ʾ
    char        pass[8];
  }__attribute__((aligned(1), packed)) lyylogin_t;
   
/*Ͷʵ��ָ�� A2 A3 A4*/
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
  
  
 /*ͨ��Ӧ��ָ�� A6*/
  
  typedef enum{
    RESP_PAY            = 0x01,
    RESP_TEST_PAY       = 0x02,
    RESP_MB             = 0x03,
  }respTypeDef;
  
  typedef struct{
    char            sn[6];
    char            ans;
  }__attribute__((aligned(1), packed)) lyyresp_t;
  
  /*CCIDָ�� A7*/
  typedef  struct{
    char            result;
    char            ccid[20];
  }__attribute__((aligned(1), packed)) lyyccid_t;
  
  /*IPָ�� A8*/
  typedef  struct{
    char            result;
    char            ip[30];
  }__attribute__((aligned(1), packed)) lyyip_t;
  
  
   /*GPSָ�� A9*/
  typedef  struct{
    char            result;
    char            gps[50];
  }__attribute__((aligned(1), packed)) lyygps_t;
  
  
 /*���Ʋ���ָ�� AA*/
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
  

    /*��������Ӧ�� AC*/
  typedef  struct{
    char                sn[6];
    char                ans;
    uint16_t             no;
  }__attribute__((aligned(1), packed)) upgradeans_t;
  
  
   /*�����̼������ϴ� AE*/
  typedef  struct{
    char                mark;
    char                revision[14];
    uint16_t            no;
    char                rev[6];
  }__attribute__((aligned(1), packed)) revision_t;
  
  
  /*�ź�ǿ�� AF*/
  typedef  struct{
    char                value;
    char                rev[3];
  }__attribute__((aligned(1), packed)) lyyrssi_t;
  
  
    /*����AG*/
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
    u32   S_cpuid[3];  //?????����?ID
    u8    ErrorCode;   //?��?��?��??
    u32   Account_LocalCoinSum; //?????��????��???
    u32   Account_OutPrizeSum;  //????��???   
    u16   Account_GamesSum;     //???����???
    u8    Account_SurplusGames; //???��???��????
    u16   Account_SurplusCoin;  //???����???
    u32   Account_GetNoteSum;   //??��??��????��???
    u32   Account_BackCoinSum;  //??��?��???
    u32   Account_FreeCoinSum;  //??��???��?��???
    
    u16   Account_LocalCoin;    //??��?????
    u16   Account_OutPrize;     //????????    
    u32   Account_NetCoinSum;   //??????��?��???   
    u16   Account_GetNote;      //??��?????    
    u16   Account_BackCoin;     //??��?????        
}__attribute__((aligned(1), packed)) note_infor_t;
  
  
  /*��Ŀ����AH*/
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
  
  
    /*ͨ����Կ���û�����AL*/
  typedef  struct{
    char                sn[6];
    char                key[16];
    char                pass[8];
  }__attribute__((aligned(1), packed)) lyynet_t;


    /*�ն˲����ϴ�����Ӧ�� AJ*/
  typedef  union{
    struct{
      char            sn[6];
      uint16_t        device;
      char            len;
    } __attribute__((aligned(1), packed))packet;
    char      buff[256];
    
  }lyyTermialParam_t;



  /*�ն˲�������Ӧ�� AK*/
  typedef  union{
    struct{
      char            sn[6];
      char            func;
      uint16_t        device;
      char            len;
    }__attribute__((aligned(1), packed)) packet;
    char      buff[256];
    
  }lyyTermialOption_t;
  

  /*���豸�����̼������ϴ� AM*/   // xuejungong 20180705  add
 typedef struct{
   char                sn_id[8];
   char                mark;
   char                revision[14];
   uint16_t            no;
   char                rev[6];
 }__attribute__((aligned(1), packed)) ndj_revision_t;

 /*���豸�����̼������ϴ� AP*/   // xuejungong 20180705  add
 typedef struct{
   char                	sn_id[8];
   char            		sn_cmd[6];
   char            		ans;
 }__attribute__((aligned(1), packed)) ndj_lyyresp_t;

 /*���豸�����̼������ϴ� AQ*/   // xuejungong 20180705  add
 typedef struct{
   char                sn_id[8];
   char                sn[6];
   char                ans;
   uint16_t             no;
 }__attribute__((aligned(1), packed)) ndj_upgradeans_t;

  /*******************************************************/
  
  /*�ƶ�֧������ B1*/
  typedef  struct{
    char                sn[6];
    uint16_t            value;
  }__attribute__((aligned(1), packed)) lyycloudpay_t;
  
  
    
  /*ʱ��ͬ������ B2*/
  typedef  struct{
    char                sec;
    char                min;
    char                hour;
    char                day;
    char                mon;
    char                week;
    char                year;
  }__attribute__((aligned(1), packed)) lyysync_t;
  
  
  
  /*�����޸� B3*/
  typedef  struct{
    char                ip[30];
  }__attribute__((aligned(1), packed)) lyysetip_t;
  
  
  /*����Ͷ��������Ʒ���Ϸ��� B6*/
  typedef  struct{
    char                opt;
    uint16_t            value;
  }__attribute__((aligned(1), packed)) lyyrest_t;
  
  
  /*��ѯ���� B7*/
  typedef  struct{
    char                opt;
  }__attribute__((aligned(1), packed)) lyycheck_t;
  
  /*������������BA*/
  typedef  struct{
    char                mark;
    uint16_t            packet;
    uint16_t            total;
    char                revision[14];
    char                rev[4];
  }__attribute__((aligned(1), packed)) upgradectrl_t;
  
  
  /*����������*/
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
*�������ƣ�void lyyInit(pLyySend_t sendCallback)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void lyyInit(pLyySend_t sendCallback);



/*******************************************************************************
*�������ƣ�void lyyParse(const socketInfor_t *pSocketInfor,const char *pTemp,uint16_t len)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void lyyParse(const char *pTemp,uint16_t len);
  
  
  
/*******************************************************************************
*�������ƣ�void lyyNetPacket(const char *pKey,const char *pPass)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void lyyNetPacket(const char *pKey,const char *pPass);



/*******************************************************************************
*�������ƣ�void lyyLoginPacket(const char *pPass)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void lyyLoginPacket(const char *pPass);


/*******************************************************************************
*�������ƣ�void lyyLoginPacket(const char *pPass)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void lyyHeartPacket(char type);


/*******************************************************************************
*�������ƣ�void lyyComeinPacket(const char *pAttr,const uint32_t total,uint16_t inc)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void lyyComeinPacket(const char *pAttr,const uint32_t total,uint16_t inc);


#define lyyCoinPacket(total,inc)  lyyComeinPacket("A2",total,inc)
#define lyyGiftPacket(total,inc)  lyyComeinPacket("A3",total,inc)
#define lyyPayPacket(total,inc)   lyyComeinPacket("A4",total,inc)


/*******************************************************************************
*�������ƣ�void lyyRssiPacket(const char value)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void lyyRssiPacket(const char value);



  
/*******************************************************************************
*�������ƣ�void lyyGpsPacket(const char *pGpsInfor)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void lyyGpsPacket(const char *pGpsInfor); 


/*******************************************************************************
*�������ƣ�void lyyPacketCallback(const char *pBuff,uint16_t len)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
extern void lyyPacketCallback(const char *pBuff,uint16_t len);


/*******************************************************************************
*�������ƣ�void payEventDone(char *pSn,char value)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
extern void payEventDone(char *pSn,char value);

/*******************************************************************************
*�������ƣ�
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
extern uint32_t upgradeChecksum(const char *pTemp,uint16_t len);

/*******************************************************************************
*�������ƣ�void lyyErrorPacket(uint8_t errcode,uint16_t errdata)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
extern void lyyErrorPacket(uint8_t errcode,uint8_t bill,uint16_t errdata);


#ifdef NDJ_Device
/*******************************************************************************
*�������ƣ�void lyyRssiPacket_NDJ(const char *pTemp,uint16_t len)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
//extern void lyyRevsionPacket_ndj(const char *pTemp, uint16_t len);

/*******************************************************************************
*�������ƣ�void lyyDeviceRespPacket(char type,const char *pSn)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
//extern void lyyDeviceRespPacket_ndj(const char *pTemp, uint16_t len);

/*******************************************************************************
*�������ƣ�void lyyUpgradeRespPacket_ndj(char type,const char *pSn)
*����˵����
*���������
*���������
*�� �� ֵ��
*����˵����
*******************************************************************************/
//extern void lyyUpgradeRespPacket_ndj(const char *pTemp, uint16_t len);

extern void lyyTermialAnswer(const char *pTemp,uint16_t len);

#endif


  
  
#ifdef __cplusplus
}
#endif	


#endif
