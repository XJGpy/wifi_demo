/*******************                    �� �� �� �� �� ˾ *********************/
/* �������ƣ�					                      	      */
/* �ļ����ƣ�   					      		      */
/* ��    �ߣ�			 					      */
/* ������ڣ�								      */
/* ��ǰ�汾��						      		      */
/* ���¼�¼: 								      */
/******************************************************************************/
#ifndef __XTRIGER_H__
#define __XTRIGER_H__

#ifdef __cplusplus
extern "C"
{
#endif 

  
typedef enum{
    //COIN_IN1,
    //COIN_IN2,
    //GIFT_IN,

	KEY_IN, // xuejungong 20180717 add key for NDJ
	KEY_SC, // xuejungong 20180717 add smartconfig
    MAX_TRIGER
  }xTRIGERTypeDef;  

/**/
typedef enum{
    TRIGER_NULL,
    TRIGER_RISING,
    TRIGER_FALLING
}xTrigerStateTypeDef;


/**/
typedef struct{
  uint16_t                      pin;
}xTrigerTable_t;

/**/
typedef uint8_t         xTrgierFlagByte;
typedef union{
  struct{
    xTrgierFlagByte             enable      :1;
  }bitFlag;
  xTrgierFlagByte               byte;
}xTrigerBitUnion;


/**/
typedef void (*xTrigerCallback_t)(char xTriger,xTrigerStateTypeDef state);


/**/
typedef struct{
  xTrigerStateTypeDef           state;
  xTrigerStateTypeDef           last;
  xTrigerBitUnion               bit;        
  uint32_t                      start;
  uint32_t                      rising;
  uint32_t                      falling;
  //uint8_t                       priority;
  xTrigerCallback_t             callback;
}xTriger_t;



/*******************************************************************************
*�������ƣ�extern void xTrigerInit(char xNum, char getFlag,
                        char priority,xTrigerCallback callback);
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void xTrigerInit(char xNum, char getFlag,
                        xTrigerCallback_t callback);


/*******************************************************************************
*�������ƣ�xTrigerStateTypeDef xTrigerReadValue(char xNum)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern xTrigerStateTypeDef xTrigerReadValue(char xNum);


/*******************************************************************************
*�������ƣ�void xTrigerEnable(char index,char enableFlag)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void xTrigerEnable(char index,char enableFlag);


/*******************************************************************************
*�������ƣ�void xTrigerSetCallback(xTRIGERTypeDef  xNum,xTrigerCallback callback)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void xTrigerSetCallback(xTRIGERTypeDef  xNum,xTrigerCallback_t callback);


/*******************************************************************************
*�������ƣ�void xTrigerHandle(char index)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void xTrigerHandle(char index);


/*******************************************************************************
*�������ƣ�void xTrigerTime(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void xTrigerTime(void);


/*******************************************************************************
*�������ƣ�char xTrigerIndexIsIdle(char index)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern char xTrigerIndexIsIdle(char index);
 

/*******************************************************************************
*�������ƣ�char xTrigerIsIdle(void)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern char xTrigerIsIdle(void);


/*******************************************************************************
*�������ƣ�void xTrigerReset(xTRIGERTypeDef  xNum)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void xTrigerReset(xTRIGERTypeDef  xNum,char priority);



/*******************************************************************************
*�������ƣ�xTriger_t * xTrigerGetInfor(xTRIGERTypeDef  xNum)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern xTriger_t * xTrigerGetInfor(xTRIGERTypeDef  xNum);

#ifdef __cplusplus
}
#endif


#endif

