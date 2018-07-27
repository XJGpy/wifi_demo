/*******************                    科 技 有 限 公 司 *********************/
/* 工程名称：					                      	      */
/* 文件名称：   					      		      */
/* 作    者：			 					      */
/* 完成日期：								      */
/* 当前版本：						      		      */
/* 更新记录: 								      */
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
    char        mark[2];                        //数据结构标示
    char        upgrade;                        //升级标示位
    char        version[14];                    //软件版本信息
    char        subVer[14];                       //
    uint32_t     crc;                           //固件校验
    uint32_t     codeNum;                       //代码容量 
    uint32_t     packetTotal;                   //数据总包数
    uint32_t     packetIndex;                   //数据包索引
    uint32_t     total;
    uint32_t     saveAddr;                      //升级数据包存放地址

    char         subVersion[14];                       //
    uint32_t     subCrc;                           //固件校验
    uint32_t     subCodeNum;                       //代码容量 
    uint32_t     subPacketTotal;                   //数据总包数
    uint32_t     subPacketIndex;                   //数据包索引
    uint32_t     subTotal;
    char         subMark;
  }__attribute__((aligned(1), packed)) upgrade_t;
 
  
  typedef struct{
   char           key[MAX_PRIVATE_KEY_SIZE];             //公用密钥
   char           pass[MAX_PASSWORD_SIZE];               //用户密码
   upgrade_t      upgrade;
  }__attribute__((aligned(1), packed)) sysInfor_t;


#ifdef NDJ_Device                // xuejungong 20180705 扭蛋机升级包数据

  typedef struct{
    char        mark[2];                        //数据结构标示
    char        upgrade;                        //升级标示位
    char        version[14];                    //软件版本信息
    uint32_t     crc;                           //固件校验
    uint32_t     codeNum;                       //代码容量
    uint32_t     packetTotal;                   //数据总包数
    uint32_t     packetIndex;                   //数据包索引
    uint32_t     total;
    uint32_t     saveAddr;                      //升级数据包存放地址
  }__attribute__((aligned(4), packed))  ndj_upgrade_t;

#endif

/*******************************************************************************
*函数名称：void sysInforInit(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void sysInforInit(void);


/*******************************************************************************
*函数名称：sysInfor_t *sysInforGet(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
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
