/*******************                    科 技 有 限 公 司 *********************/
/* 工程名称：					                      	      */
/* 文件名称：   					      		      */
/* 作    者：			 					      */
/* 完成日期：								      */
/* 当前版本：						      		      */
/* 更新记录: 								      */
/******************************************************************************/
#ifndef __XMARK_H__
#define __XMARK_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef uint32_t           xMarkBitSize;  
/**/
  typedef enum{
    MARK_SYNC          	= 0x01,
    MARK_KEY_NET		= 0x02,
    MARK_DEVICE_LOGIN   = 0x04,

	// xuejungong 20180628 add for ndj status check
	MARK_NDJ_STA        = 0x10,
	// xuejungong 20180706 add for ndj upgrade
	MARK_NDJ_UPD        = 0x20,
	// xuejungong 20180706 add for ndj Smartconfig
	MARK_NDJ_SC         = 0x30,
	// xuejungong 20180706 add for ndj IDconfig
	MARK_NDJ_IDC        = 0x40,

  }xMarkTypeDef;
  
  
/*******************************************************************************
*函数名称：void xMarkInit(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void  xMarkInit(void);
  
/*******************************************************************************
*函数名称：void xMarkSetValue(char index,char valueFlag)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void  xMarkSetValue(char index,char valueFlag);


/*******************************************************************************
*函数名称：char xMartGetValue(char index)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern char  xMarkGetValue(char index);


/*******************************************************************************
*函数名称：char xMarkIsZero(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern char  xMarkIsZero(void);

#ifdef __cplusplus
}
#endif

#endif
