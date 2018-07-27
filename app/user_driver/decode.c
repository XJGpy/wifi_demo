
/*******************                    科 技 有 限 公 司 *********************/
/* 工程名称：					                      				          */
/* 文件名称：   					      					                      */
/* 作    者：			 					      					          */
/* 完成日期：								      							  */
/* 当前版本：						      							          */
/* 更新记录: 								      							  */
/******************************************************************************/
#include "stdint.h"
#include "string.h"
#include "decode.h"


/*******************************************************************************
*函数名称：EndianChange16
*函数说明：16位大端模式数据转小端模式数据
*输入参数：待转换的数据
*输出参数：无
*返 回 值：转换后数据  
*其他说明：
*******************************************************************************/
uint16_t EndianChange16(const uint16_t usData)
{
  return ((usData >> 8) | (usData << 8));
}

/*******************************************************************************
*函数名称：EndianChange32
*函数说明：32位大端模式数据转小端模式数据
*输入参数：待转换的数据
*输出参数：无
*返 回 值：转换后数据  
*其他说明：
*******************************************************************************/
uint32_t EndianChange32(const uint32_t ulData)
{
  return (((ulData >> 24) & 0x000000FF) | ((ulData >> 8) & 0x0000FF00)
          
          | ((ulData << 8) & 0x00FF0000) | ((ulData << 24) & 0xFF000000));
}


/*******************************************************************************
*函数名称：char *strnstr(const char *s, const char *find, size_t slen)
*函数说明：指定字符串长度查找字符串
*输入参数：s:源字符串指针  find：查找字符串地址指针，slen：源字符串指定长度
*输出参数：无
*返 回 值：返回查找字符串位置  
*其他说明：
*******************************************************************************/
char *strnstr(const char *s1, const char *s2, uint16_t len)
{  
  uint16_t l2;
  
  l2 = strlen(s2);
  if (!l2)  
	return (char *)s1;
  
  while (len >= l2) {  
	len--;  
	if (!memcmp(s1, s2, l2))  
	  return (char *)s1;  
	s1++;  
  }  
  return NULL;  
} 




/*******************************************************************************
*函数名称：char * itoa(char * result, int32_t value, uint8_t width)
*函数说明：整数转十进制字符串
*输入参数：result：字符串存放地址指针  value：整数值  width：转换宽度
*输出参数：无
*返 回 值：返回转换结束地址指针  
*其他说明：
*******************************************************************************/
const char ITOA_CONST[] = "fedcba9876543210123456789abcdef";
char * itoa(char * result, int32_t value, uint8_t width)
{
  char *ptr = result, *eos, tmp_char;
  int32_t tmp_value;
  uint8_t len = 0;
  
  do
  {
	tmp_value = value;
	value /= 10;
	*ptr++ = ITOA_CONST[15 + (tmp_value - value * 10)];
	len++;
  } while (value != 0);
  
  while (width > 0 && len < width)
  {
	*ptr++ = '0';
	len++;
  }
  
  if (tmp_value < 0)
  {
	*ptr++ = '-';
	len++;
  }
  eos = ptr;
  *ptr-- = '\0';
  
  while (result < ptr)
  {
	tmp_char = *ptr;
	*ptr--= *result;
	*result++ = tmp_char;
  }
  return eos;
}


/*******************************************************************************
*函数名称：char * itoa16(char * result, int32_t value, uint8_t width)
*函数说明：整数转十六进制字符串
*输入参数：result：字符串存放地址指针  value：整数值  width：转换宽度
*输出参数：无
*返 回 值：返回转换结束地址指针  
*其他说明：
*******************************************************************************/
char * itoa16(char * result, int32_t value, uint8_t width)
{
  char *ptr = result, *eos, tmp_char;
  int32_t tmp_value;
  uint8_t len = 0;
  
  do
  {
	tmp_value = value;
	value /= 16;
	*ptr++ = "fedcba9876543210123456789abcdef" [15 + (tmp_value - value * 16)];
	len++;
  } while (value != 0);
  
  while (width > 0 && len < width)
  {
	*ptr++ = '0';
	len++;
  }
  
  if (tmp_value < 0)
  {
	*ptr++ = '-';
	len++;
  }
  eos = ptr;
  *ptr-- = '\0';
  
  while (result < ptr)
  {
	tmp_char = *ptr;
	*ptr--= *result;
	*result++ = tmp_char;
  }
  return eos;
}



/*******************************************************************************
*函数名称：int String2ToBytes(const char* pSrc, char* pDst, int nSrcLength)
*函数说明：可打印字符串转换为字节数据
*输入参数：pSrc - 源字符串指针   nSrcLength - 源字符串长度
*输出参数：pDst - 目标数据指针
*返 回 值：目标数据长度  
*其他说明："C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}
*******************************************************************************/
int String2ToBytes(const char* pSrc, char* pDst, int nSrcLength)
{
  int i;
  
  for (i = 0; i < nSrcLength; i += 2)
  {
	// 输出高4位
	if ((*pSrc >= '0') && (*pSrc <= '9'))
	{
      *pDst = (*pSrc - '0') << 4;
	}
	else
	{
      *pDst = (*pSrc - 'A' + 10) << 4;
	}
	
	pSrc++;
	
	// 输出低4位
	if ((*pSrc>='0') && (*pSrc<='9'))
	{
      *pDst |= *pSrc - '0';
	}
	else
	{
      *pDst |= *pSrc - 'A' + 10;
	}
	
	pSrc++;
	pDst++;
  }
  
  // 返回目标数据长度
  return (nSrcLength / 2);
}

/*******************************************************************************
*函数名称：int Bytes2ToString(const char* pSrc, char* pDst, int nSrcLength)
*函数说明：字节数据转换为可打印字符串
*输入参数：pSrc - 源数据指针  nSrcLength - 源数据长度
*输出参数：pDst - 目标字符串指针
*返 回 值：目标数据长度   
*其他说明：{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01"
*******************************************************************************/
int Bytes2ToString(const char* pSrc, char* pDst, int nSrcLength)
{
  const char hexTab[]="0123456789ABCDEF";	// 0x0-0xf的字符查找表
  int i;
  
  for (i = 0; i < nSrcLength; i++)
  {
	*pDst++ = hexTab[*pSrc >> 4];		// 输出高4位
	*pDst++ = hexTab[*pSrc & 0x0f];	// 输出低4位
	pSrc++;
  }
  
  // 输出字符串加个结束符
  *pDst = '\0';   
  
  // 返回目标字符串长度
  return (nSrcLength * 2);
}





/*******************************************************************************
*函数名称：uint16_t AsciiToBcd( char *bcd, const char *asc,  uint16_t len,  char fmt)
*函数说明：
*输入参数：
*输出参数：
*返 回 值：  
*其他说明：
*******************************************************************************/
uint16_t AsciiToBcd( char *bcd, const char *asc,  uint16_t len,  char fmt)
{
  uint16_t i, odd;
  uint8_t ch; 
  
  
  odd = len & 0x01;
  
  if ( odd && !fmt )
  {   
	*bcd++ = (*asc++) & 0x0F;
  }
  len >>= 1;
  
  for (i = 0; i < len; i++)
  {   
	ch = (*asc++) << 4;        
	ch |= (*asc++) & 0x0F;       
	*bcd++ = ch; 
  }
  
  
  if ( odd && fmt )
  {   
	*bcd = (*asc) << 4;
  }
  return ( i + odd );
}



/*******************************************************************************
*函数名称：HexToDec
*函数说明：将一个ASCII字符码转换为数值
*输入参数：Hex被转换数据
*输出参数：无
*返 回 值：转换后的数值  
*其他说明：【注意】：p_HexData其值应当小于100，会出现异常。
*******************************************************************************/
uint8_t HexToDec(uint8_t Hex)
{
  return (((Hex/10) << 4) | (Hex % 10));
}


/*******************************************************************************
*函数名称：DecToHex
*函数说明：将一个数值转换为十六进制
*输入参数：Bcd被转换数据
*输出参数：无
*返 回 值：转换后十六进制  
*其他说明：
*******************************************************************************/
uint8_t DecToHex(uint8_t Bcd)
{
  return ((((Bcd&0xf0)>>4)*10)+(Bcd&0x0f));
}


/*******************************************************************************
*函数名称：void insertionSort(uint16_t *numbers, uint32_t array_size) 
*函数说明：数据从小到大排列
*输入参数：numbers：数据地址指针    array_size：数据长度
*输出参数：数组内容从小到大排列
*返 回 值：无 
*其他说明：
*******************************************************************************/
void insertionSort(uint16_t *numbers, uint32_t array_size) 
{
  uint32_t i, j;
  uint32_t index;
  
  for (i=1; i < array_size; i++) {
	index = numbers[i];
	j = i;
	while ((j > 0) && (numbers[j-1] > index)) {
      numbers[j] = numbers[j-1];
      j = j - 1;
	}
	numbers[j] = index;
  }
}

/*******************************************************************************
*函数名称：uint32_t interquartileMean(uint16_t *array, uint32_t numOfSamples)
*函数说明：数据去掉最小值与最大值计算其平均值
*输入参数：numbers：数据地址指针    array_size：数据长度
*输出参数：无
*返 回 值：返回其平均值 
*其他说明：
*******************************************************************************/
uint32_t interquartileMean(uint16_t *array, uint32_t numOfSamples)
{
  uint32_t sum=0;
  uint32_t  index, maxindex;
  /* discard  the lowest and the highest data samples */ 
  maxindex = 3 * numOfSamples / 4;
  for (index = (numOfSamples / 4); index < maxindex; index++){
	sum += array[index];
  }
  /* return the mean value of the remaining samples value*/
  return ( sum / (numOfSamples / 2) );
}





