/*******************                    �� �� �� �� �� ˾ *********************/
/* �������ƣ�					                      	      */
/* �ļ����ƣ�   					      		      */
/* ��    �ߣ�			 					      */
/* ������ڣ�								      */
/* ��ǰ�汾��						      		      */
/* ���¼�¼: 								      */
/******************************************************************************/
#ifndef __SYSINFOR_H__
#define __SYSINFOR_H__

#include "connect.h"

#ifdef __cplusplus
extern "C"
{
#endif 
 
#define MAX_PRIVATE_KEY_SIZE                            16
#define MAX_PASSWORD_SIZE                               8



  
  /*******************************************/  
#define UPGRADE_MARK                            "\xAA\x55"
  typedef  struct{
    char        mark[2];                        //���ݽṹ��ʾ
    char        upgrade;                        //������ʾλ
    char        version[14];                    //����汾��Ϣ
    char        subVer[14];                       //
    uint32_t     crc;                           //�̼�У��
    uint32_t     codeNum;                       //�������� 
    uint32_t     packetTotal;                   //�����ܰ���
    uint32_t     packetIndex;                   //���ݰ�����
    uint32_t     total;
    uint32_t     saveAddr;                      //�������ݰ���ŵ�ַ

    char         subVersion[14];                       //
    uint32_t     subCrc;                           //�̼�У��
    uint32_t     subCodeNum;                       //�������� 
    uint32_t     subPacketTotal;                   //�����ܰ���
    uint32_t     subPacketIndex;                   //���ݰ�����
    uint32_t     subTotal;
    char         subMark;
  }__attribute__((aligned(1), packed)) upgrade_t;
 
  
  typedef struct{
   char           key[MAX_PRIVATE_KEY_SIZE];             //������Կ
   char           pass[MAX_PASSWORD_SIZE];               //�û�����
   upgrade_t      upgrade;
  }__attribute__((aligned(1), packed)) sysInfor_t;


#ifdef NDJ_Device                // xuejungong 20180705 Ť��������������

  typedef struct{
    char        mark[2];                        //���ݽṹ��ʾ
    char        upgrade;                        //������ʾλ
    char        version[14];                    //����汾��Ϣ
    uint32_t     crc;                           //�̼�У��
    uint32_t     codeNum;                       //��������
    uint32_t     packetTotal;                   //�����ܰ���
    uint32_t     packetIndex;                   //���ݰ�����
    uint32_t     total;
    uint32_t     saveAddr;                      //�������ݰ���ŵ�ַ
  }__attribute__((aligned(4), packed))  ndj_upgrade_t;

#endif

/*******************************************************************************
*�������ƣ�void sysInforInit(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void sysInforInit(void);


/*******************************************************************************
*�������ƣ�sysInfor_t *sysInforGet(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern sysInfor_t *sysInforGet(void);

// xuejungong 20180705 add
extern ndj_upgrade_t *ndjInforGet(void);
extern void ndjInforClean(void);
extern void IDConfig_Check(void);
extern void IDConfig_Save(void);

#ifdef __cplusplus
}
#endif


#endif
