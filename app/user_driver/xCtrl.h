/*******************                    科 技 有 限 公 司 *********************/
/* 工程名称：					                      	      */
/* 文件名称：   					      		      */
/* 作    者：			 					      */
/* 完成日期：								      */
/* 当前版本：						      		      */
/* 更新记录: 								      */
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
*函数名称：void xCtrlInit(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void xCtrlInit(void);


/*******************************************************************************
*函数名称：void xCtrlOnOff(char index, xCtrlFlagTypeDef valueFlag)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void xCtrlOnOff(char index, xCtrlFlagTypeDef valueFlag);


#ifdef __cplusplus
}
#endif

#endif
