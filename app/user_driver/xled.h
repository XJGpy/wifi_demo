/*******************                    科 技 有 限 公 司 *********************/
/* 工程名称：					                      	      */
/* 文件名称：   					      		      */
/* 作    者：			 					      */
/* 完成日期：								      */
/* 当前版本：						      		      */
/* 更新记录: 								      */
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
*函数名称：void xLedInit(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void  xLedInit(void);

/*******************************************************************************
*函数名称：void xLedStart(char nLedMark,uint16_t ontime,uint16_t offtime,uint8_t cnt)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void  xLedStart(char nLedMark,uint16_t ontime,uint16_t offtime,uint8_t cnt);


/*******************************************************************************
*函数名称：void xLedTime(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void  xLedTime(void);



/*******************************************************************************
*函数名称：char xLedIsOK(char nLed)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern char  xLedIsOK(char nLed);

/*******************************************************************************
*函数名称：char xLedIsIdle(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern char  xLedIsIdle(void);

#ifdef __cplusplus
}
#endif

#endif
