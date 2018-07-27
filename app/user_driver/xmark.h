/*******************                    �� �� �� �� �� ˾ *********************/
/* �������ƣ�					                      	      */
/* �ļ����ƣ�   					      		      */
/* ��    �ߣ�			 					      */
/* ������ڣ�								      */
/* ��ǰ�汾��						      		      */
/* ���¼�¼: 								      */
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
*�������ƣ�void xMarkInit(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void  xMarkInit(void);
  
/*******************************************************************************
*�������ƣ�void xMarkSetValue(char index,char valueFlag)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void  xMarkSetValue(char index,char valueFlag);


/*******************************************************************************
*�������ƣ�char xMartGetValue(char index)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern char  xMarkGetValue(char index);


/*******************************************************************************
*�������ƣ�char xMarkIsZero(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern char  xMarkIsZero(void);

#ifdef __cplusplus
}
#endif

#endif
