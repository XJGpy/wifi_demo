/*******************                    �� �� �� �� �� ˾ *********************/
/* �������ƣ�					                      	      */
/* �ļ����ƣ�   					      		      */
/* ��    �ߣ�			 					      */
/* ������ڣ�								      */
/* ��ǰ�汾��						      		      */
/* ���¼�¼: 								      */
/******************************************************************************/
#ifndef __XLED_H__
#define __XLED_H__

#ifdef __cplusplus
extern "C"
{
#endif 

typedef struct{
	char pin;
	char value;
}ledFlashConfig_t;
  
  
  typedef enum{
   GRN_LED,
   RED_LED,
   COIN_OUT,
   MAX_LED
  }xLedTypedef;
  
/**/
  typedef enum{
    LED_OFF,
    LED_ON
  }ledOnOffTypeDef;
  

 /**/
typedef struct{
  uint16_t      led;
  uint16_t      onTime;
  uint16_t      offTime;
  uint8_t       flashCount;
}flashParam_t;

/**/
typedef enum{
  FLASH_STATE_IDLE,
  FLASH_STATE_START,
  FLASH_STATE_ON,
  FLASH_STATE_OFF,
}ledStateTypeDef;

typedef struct{
  ledStateTypeDef       state;
  uint8_t               count;
  uint16_t              onTime;
  uint16_t              offTime;
  uint16_t              runTime;
}xLedFlash_t;
  
  

/*******************************************************************************
*�������ƣ�void xLedInit(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void  xLedInit(void);

/*******************************************************************************
*�������ƣ�void xLedStart(char nLedMark,uint16_t ontime,uint16_t offtime,uint8_t cnt)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void  xLedStart(char nLedMark,uint16_t ontime,uint16_t offtime,uint8_t cnt);


/*******************************************************************************
*�������ƣ�void xLedTime(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void  xLedTime(void);



/*******************************************************************************
*�������ƣ�char xLedIsOK(char nLed)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern char  xLedIsOK(char nLed);

/*******************************************************************************
*�������ƣ�char xLedIsIdle(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern char  xLedIsIdle(void);

#ifdef __cplusplus
}
#endif

#endif
