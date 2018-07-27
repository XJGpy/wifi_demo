/*******************                    科 技 有 限 公 司 ************************/
/* 工程名称：																	*/
/* 文件名称：																	*/
/* 作    者：			 					      								*/
/* 完成日期：								      								*/
/* 当前版本：						      		      							*/
/* 更新记录: 								      								*/
/********************************************************************************/
#ifndef __RINGEVENT_H__
#define __RINGEVENT_H__

#ifdef __cplusplus
extern "C"
{
#endif 
  
#define MAX_RINGEVENT_COUNT					20
  
#define RINGEVENT_RESULT_OK			    	0x00
#define RINGEVENT_RESULT_DONE_FAIL		    0x01
#define RINGEVENT_RESULT_TIMEOUT		    0x02
#define RINGEVENT_RESULT_RESP              	0x03
  
  
  typedef enum{
    RING_IDLE,
    RING_DELAY,
    RING_START,
    RING_WAIT
  }ringTypeDef;
  
  typedef union{
    struct{
      uint8_t             count               :3;
      uint8_t             status              :2;
      uint8_t             steup               :3;
    }bitFlag;
    uint8_t            	  bitByte;
  }ringMark_t;
  
  typedef int(*pDone)(void *arg,uint16_t);
  typedef void(*pResult)(char);
  
  typedef struct{
    pDone			done;
    pResult			result;
    void			*pArg;
    uint16_t    	len;
    ringMark_t  	mark;
    uint32_t   		timeout;
    uint32_t		delay;
    uint32_t		wait;
    tmr_t			tmr;
  }ringEvent_t;
  
  
  /*******************************************************************************
  *函数名称：void ringEventInit(void)
  *函数说明：
  *输入参数：
  *输出参数：
  *返 回 值：  
  *其他说明：
  *******************************************************************************/
  extern void ringEventInit(void); 
  
  
  /*******************************************************************************
  *函数名称：void ringEventLoop(void)
  *函数说明：
  *输入参数：
  *输出参数：
  *返 回 值：  
  *其他说明：
  *******************************************************************************/
  extern void ringEventLoop(void);
  
  
  /*******************************************************************************
  *函数名称：int ringEventWrite(pDone		done,
  pResult		result,
  void			*pArg,
  uint16_t     len,
  uint8_t 		count,
  tmr_ticks_t	delay,
  pTickFunc	pTick)
  *函数说明：
  *输入参数：
  *输出参数：
  *返 回 值：  
  *其他说明：
  *******************************************************************************/
  extern int ringEventWrite(pDone		done,
			    pResult		result,
			    const void		*pArg,
			    uint16_t     	len,
                            tmr_ticks_t         timeout,
			    tmr_ticks_t		delay,
			    uint8_t 		count,
			    tmr_ticks_t		wait,
			    pTickFunc		pTick);
  
  
  /*******************************************************************************
  *函数名称：ringEvent_t *ringEventGetInfor(char index)
  *函数说明：
  *输入参数：
  *输出参数：
  *返 回 值：  
  *其他说明：
  *******************************************************************************/
  extern ringEvent_t *ringEventGetInfor(char index);
  
  /*******************************************************************************
  *函数名称：char ringEventIsEmpty(void)
  *函数说明：
  *输入参数：
  *输出参数：
  *返 回 值：  
  *其他说明：
  *******************************************************************************/
  extern char ringEventIsEmpty(void);  
  
  
  
  /*******************************************************************************
  *函数名称：void ringEventEndIndex(char index)
  *函数说明：
  *输入参数：
  *输出参数：
  *返 回 值：  
  *其他说明：
  *******************************************************************************/
  extern void ringEventEndIndex(char index);
  
  
  /*******************************************************************************
  *函数名称：void ringEvnetEnd(void)
  *函数说明：
  *输入参数：
  *输出参数：
  *返 回 值：  
  *其他说明：
  *******************************************************************************/
  extern void ringEvnetEnd(void);
  
  
  
#ifdef __cplusplus
}
#endif

#endif  

