/*******************                    �� �� �� �� �� ˾ *********************/
/* �������ƣ�					                      	      */
/* �ļ����ƣ�   					      		      */
/* ��    �ߣ�			 					      */
/* ������ڣ�								      */
/* ��ǰ�汾��						      		      */
/* ���¼�¼: 								      */
/******************************************************************************/
#ifndef __XCTRL_H__
#define __XCTRL_H__

#ifdef __cplusplus
extern "C"
{
#endif 



/**/
  typedef enum{
    CTRL_ON_LOW,
    CTRL_ON_HIGH
}xCTRLOnFlagTypeDef;

/**/
typedef struct{
  uint16_t              pin;
  xCTRLOnFlagTypeDef    onFlag;
}xCTRLTable_t;
                                                      
/**/
  typedef enum{
    CTRL_NULL,
    CTRL_LOW,
    CTRL_HIGH,
    CTRL_TOGGLE
  }xCtrlFlagTypeDef;

  /**/
  typedef enum{
    CTRL_LED_G,
    CTRL_LED_R,
    //CTRL_COIN_OUT,
	//CTRL_MODE,
	CTRL_RS485_RE,// xuejungong 2018717 add
    MAX_CTRL
  }xCtrlTypeDef;
  
  
/*******************************************************************************
*�������ƣ�void xCtrlInit(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void xCtrlInit(void);


/*******************************************************************************
*�������ƣ�void xCtrlOnOff(char index, xCtrlFlagTypeDef valueFlag)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void xCtrlOnOff(char index, xCtrlFlagTypeDef valueFlag);


#ifdef __cplusplus
}
#endif

#endif
