/*******************                    �� �� �� �� �� ˾ ************************/
/* �������ƣ�					                      	      					*/
/* �ļ����ƣ�   					      		      							    */
/* ��    �ߣ�			 					      								*/
/* ������ڣ�								      								*/
/* ��ǰ�汾��						      		      							*/
/* ���¼�¼: 								      								*/
/********************************************************************************/
#ifndef __DBG_H__
#define __DBG_H__
#ifdef __cplusplus

extern "C"
{
#endif 

#define DEBUG
  

typedef void(*putFunc_t)(const void *,uint16_t);

typedef struct{
  putFunc_t      put;
  uint16_t       size;
}dbgParam_t;

  
/*******************************************************************************
*�������ƣ�void dbgInit(putFunc_t pPutBuff,uint16_t len)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void dbgInit(putFunc_t pPutBuff,uint16_t len);


/*******************************************************************************
*�������ƣ�void dbgWriteBuff(const char *string,uint16_t len)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void dbgWriteBuff(const char *string,uint16_t len);


/*******************************************************************************
*�������ƣ�void dbgWriteFormatted(const char *format,...)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void dbgWriteFormatted(const char *format,...);

/*******************************************************************************
*�������ƣ�void dbgHexToString(const char *pBuff,uint16_t len)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
*******************************************************************************/
extern void dbgHexToString(const char *pBuff,uint16_t len);


#define  TRACE_LEVEL_OFF                               0
#define  TRACE_LEVEL_INFO                              1
#define  TRACE_LEVEL_DBG                               2

#ifdef DEBUG
#define  APP_TRACE_LEVEL                TRACE_LEVEL_DBG
#else
#define  APP_TRACE_LEVEL                TRACE_LEVEL_OFF
#endif

#define  APP_TRACE                      dbgWriteFormatted
#define  APP_BUFF						dbgWriteBuff
#define  APP_HEXTOSTRING				dbgHexToString


#define  APP_TRACE_INFO(FMT, ...)       ((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_TRACE(FMT, ## __VA_ARGS__)) : (void)0)
#define  APP_TRACE_DBG(FMT, ...)        ((APP_TRACE_LEVEL >= TRACE_LEVEL_DBG)   ? (void)(APP_TRACE(FMT, ## __VA_ARGS__)) : (void)0)

#define  APP_BUFF_INFO(FMT, ...)  		((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_BUFF(FMT, ## __VA_ARGS__)) : (void)0)
#define  APP_BUFF_DBG(FMT, ...)    		((APP_TRACE_LEVEL >= TRACE_LEVEL_DBG)   ? (void)(APP_BUFF(FMT, ## __VA_ARGS__)) : (void)0)

#define  APP_HEX_STRING_INFO(FMT, ...)  ((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(dbgHexToString(FMT, ## __VA_ARGS__)) : (void)0)
#define  APP_HEX_STRING_DBG(FMT, ...)   ((APP_TRACE_LEVEL >= TRACE_LEVEL_DBG)   ? (void)(dbgHexToString(FMT, ## __VA_ARGS__)) : (void)0)


#ifdef __cplusplus
}
#endif

#endif
