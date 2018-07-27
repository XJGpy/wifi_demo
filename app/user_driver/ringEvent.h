/*******************                    �� �� �� �� �� ˾ ************************/
/* �������ƣ�																	*/
/* �ļ����ƣ�																	*/
/* ��    �ߣ�			 					      								*/
/* ������ڣ�								      								*/
/* ��ǰ�汾��						      		      							*/
/* ���¼�¼: 								      								*/
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
  *�������ƣ�void ringEventInit(void)
  *����˵����
  *���������
  *���������
  *�� �� ֵ��  
  *����˵����
  *******************************************************************************/
  extern void ringEventInit(void); 
  
  
  /*******************************************************************************
  *�������ƣ�void ringEventLoop(void)
  *����˵����
  *���������
  *���������
  *�� �� ֵ��  
  *����˵����
  *******************************************************************************/
  extern void ringEventLoop(void);
  
  
  /*******************************************************************************
  *�������ƣ�int ringEventWrite(pDone		done,
  pResult		result,
  void			*pArg,
  uint16_t     len,
  uint8_t 		count,
  tmr_ticks_t	delay,
  pTickFunc	pTick)
  *����˵����
  *���������
  *���������
  *�� �� ֵ��  
  *����˵����
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
  *�������ƣ�ringEvent_t *ringEventGetInfor(char index)
  *����˵����
  *���������
  *���������
  *�� �� ֵ��  
  *����˵����
  *******************************************************************************/
  extern ringEvent_t *ringEventGetInfor(char index);
  
  /*******************************************************************************
  *�������ƣ�char ringEventIsEmpty(void)
  *����˵����
  *���������
  *���������
  *�� �� ֵ��  
  *����˵����
  *******************************************************************************/
  extern char ringEventIsEmpty(void);  
  
  
  
  /*******************************************************************************
  *�������ƣ�void ringEventEndIndex(char index)
  *����˵����
  *���������
  *���������
  *�� �� ֵ��  
  *����˵����
  *******************************************************************************/
  extern void ringEventEndIndex(char index);
  
  
  /*******************************************************************************
  *�������ƣ�void ringEvnetEnd(void)
  *����˵����
  *���������
  *���������
  *�� �� ֵ��  
  *����˵����
  *******************************************************************************/
  extern void ringEvnetEnd(void);
  
  
  
#ifdef __cplusplus
}
#endif

#endif  

