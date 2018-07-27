/*******************                    科 技 有 限 公 司 *********************/
/* 工程名称：					                      	      */
/* 文件名称：   					      		      */
/* 作    者：			 					      */
/* 完成日期：								      */
/* 当前版本：						      		      */
/* 更新记录: 								      */
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
*函数名称：extern void xTrigerInit(char xNum, char getFlag,
                        char priority,xTrigerCallback callback);
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void xTrigerInit(char xNum, char getFlag,
                        xTrigerCallback_t callback);


/*******************************************************************************
*函数名称：xTrigerStateTypeDef xTrigerReadValue(char xNum)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern xTrigerStateTypeDef xTrigerReadValue(char xNum);


/*******************************************************************************
*函数名称：void xTrigerEnable(char index,char enableFlag)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void xTrigerEnable(char index,char enableFlag);


/*******************************************************************************
*函数名称：void xTrigerSetCallback(xTRIGERTypeDef  xNum,xTrigerCallback callback)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void xTrigerSetCallback(xTRIGERTypeDef  xNum,xTrigerCallback_t callback);


/*******************************************************************************
*函数名称：void xTrigerHandle(char index)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void xTrigerHandle(char index);


/*******************************************************************************
*函数名称：void xTrigerTime(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void xTrigerTime(void);


/*******************************************************************************
*函数名称：char xTrigerIndexIsIdle(char index)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern char xTrigerIndexIsIdle(char index);
 

/*******************************************************************************
*函数名称：char xTrigerIsIdle(void)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern char xTrigerIsIdle(void);


/*******************************************************************************
*函数名称：void xTrigerReset(xTRIGERTypeDef  xNum)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern void xTrigerReset(xTRIGERTypeDef  xNum,char priority);



/*******************************************************************************
*函数名称：xTriger_t * xTrigerGetInfor(xTRIGERTypeDef  xNum)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern xTriger_t * xTrigerGetInfor(xTRIGERTypeDef  xNum);

#ifdef __cplusplus
}
#endif


#endif

