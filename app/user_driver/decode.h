/*******************                    科 技 有 限 公 司 *********************/
/* 工程名称：					                      				          */
/* 文件名称：   					      					                  */
/* 作    者：			 					      					          */
/* 完成日期：								      							  */
/* 当前版本：						      							          */
/* 更新记录: 								      							  */
/******************************************************************************/
#ifndef __DECODE_H__
#define __DECODE_H__

#ifdef __cplusplus
extern "C"
{
#endif
    
    
/*******************************************************************************
*函数名称：B2LChange16
*函数说明：16位大端模式数据转小端模式数据
*输入参数：待转换的数据
*输出参数：无
*返 回 值：转换后数据  
*其他说明：
*******************************************************************************/
extern uint16_t EndianChange16(const uint16_t usData);


/*******************************************************************************
*函数名称：B2LChange32
*函数说明：32位大端模式数据转小端模式数据
*输入参数：待转换的数据
*输出参数：无
*返 回 值：转换后数据  
*其他说明：
*******************************************************************************/
extern uint32_t EndianChange32(const uint32_t ulData);
    
    
/*******************************************************************************
*函数名称：char *strnstr(const char *s, const char *find, size_t slen)
*函数说明：指定字符串长度查找字符串
*输入参数：s:源字符串指针  find：查找字符串地址指针，slen：源字符串指定长度
*输出参数：无
*返 回 值：返回查找字符串位置  
*其他说明：
*******************************************************************************/
extern char *strnstr(const char *s, const char *find, uint16_t slen);



/*******************************************************************************
*函数名称：char * itoa(char * result, int32_t value, uint8_t width)
*函数说明：整数转十进制字符串
*输入参数：result：字符串存放地址指针  value：整数值  width：转换宽度
*输出参数：无
*返 回 值：返回转换结束地址指针  
*其他说明：
*******************************************************************************/
extern char * itoa(char * result, int32_t value, uint8_t width);



/*******************************************************************************
*函数名称：char * itoa16(char * result, int32_t value, uint8_t width)
*函数说明：整数转十六进制字符串
*输入参数：result：字符串存放地址指针  value：整数值  width：转换宽度
*输出参数：无
*返 回 值：返回转换结束地址指针  
*其他说明：
*******************************************************************************/
extern char * itoa16(char * result, int32_t value, uint8_t width);


/*******************************************************************************
*函数名称：int String2ToBytes(const char* pSrc, char* pDst, int nSrcLength)
*函数说明：可打印字符串转换为字节数据
*输入参数：pSrc - 源字符串指针   nSrcLength - 源字符串长度
*输出参数：pDst - 目标数据指针
*返 回 值：目标数据长度  
*其他说明："C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}
*******************************************************************************/
extern int String2ToBytes(const char* pSrc, char* pDst, int nSrcLength);


/*******************************************************************************
*函数名称：int Bytes2ToString(const char* pSrc, char* pDst, int nSrcLength)
*函数说明：字节数据转换为可打印字符串
*输入参数：pSrc - 源数据指针  nSrcLength - 源数据长度
*输出参数：pDst - 目标字符串指针
*返 回 值：目标数据长度   
*其他说明：{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01"
*******************************************************************************/
extern int Bytes2ToString(const char* pSrc, char* pDst, int nSrcLength);



/*******************************************************************************
*函数名称：uint16_t AsciiToBcd( char *bcd, const char *asc,  uint16_t len,  char fmt)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
extern uint16_t AsciiToBcd( char *bcd, const char *asc,  uint16_t len,  char fmt);

/*******************************************************************************
*函数名称：HexToDec
*函数说明：将一个ASCII字符码转换为数值
*输入参数：Hex被转换数据
*输出参数：无
*返 回 值：转换后的数值  
*其他说明：【注意】：p_HexData其值应当小于100，会出现异常。
*******************************************************************************/
extern uint8_t HexToDec(uint8_t Hex);


/*******************************************************************************
*函数名称：DecToHex
*函数说明：将一个数值转换为十六进制
*输入参数：Bcd被转换数据
*输出参数：无
*返 回 值：转换后十六进制  
*其他说明：
*******************************************************************************/
extern uint8_t DecToHex(uint8_t Bcd);


/*******************************************************************************
*函数名称：void insertionSort(uint16_t *numbers, uint32_t array_size) 
*函数说明：数据从小到大排列
*输入参数：numbers：数据地址指针    array_size：数据长度
*输出参数：数组内容从小到大排列
*返 回 值：无 
*其他说明：
*******************************************************************************/
extern void insertionSort(uint16_t *numbers, uint32_t array_size);

/*******************************************************************************
*函数名称：uint32_t interquartileMean(uint16_t *array, uint32_t numOfSamples)
*函数说明：数据去掉最小值与最大值计算其平均值
*输入参数：numbers：数据地址指针    array_size：数据长度
*输出参数：无
*返 回 值：返回其平均值 
*其他说明：
*******************************************************************************/
extern uint32_t interquartileMean(uint16_t *array, uint32_t numOfSamples);


#ifdef __cplusplus
}
#endif

#endif

