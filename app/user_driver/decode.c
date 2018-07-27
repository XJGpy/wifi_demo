
/*******************                    �� �� �� �� �� ˾ *********************/
/* �������ƣ�					                      				          */
/* �ļ����ƣ�   					      					                      */
/* ��    �ߣ�			 					      					          */
/* ������ڣ�								      							  */
/* ��ǰ�汾��						      							          */
/* ���¼�¼: 								      							  */
/******************************************************************************/
#include "stdint.h"
#include "string.h"
#include "decode.h"


/*******************************************************************************
*�������ƣ�EndianChange16
*����˵����16λ���ģʽ����תС��ģʽ����
*�����������ת��������
*�����������
*�� �� ֵ��ת��������  
*����˵����
*******************************************************************************/
uint16_t EndianChange16(const uint16_t usData)
{
  return ((usData >> 8) | (usData << 8));
}

/*******************************************************************************
*�������ƣ�EndianChange32
*����˵����32λ���ģʽ����תС��ģʽ����
*�����������ת��������
*�����������
*�� �� ֵ��ת��������  
*����˵����
*******************************************************************************/
uint32_t EndianChange32(const uint32_t ulData)
{
  return (((ulData >> 24) & 0x000000FF) | ((ulData >> 8) & 0x0000FF00)
          
          | ((ulData << 8) & 0x00FF0000) | ((ulData << 24) & 0xFF000000));
}


/*******************************************************************************
*�������ƣ�char *strnstr(const char *s, const char *find, size_t slen)
*����˵����ָ���ַ������Ȳ����ַ���
*���������s:Դ�ַ���ָ��  find�������ַ�����ַָ�룬slen��Դ�ַ���ָ������
*�����������
*�� �� ֵ�����ز����ַ���λ��  
*����˵����
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
*�������ƣ�char * itoa(char * result, int32_t value, uint8_t width)
*����˵��������תʮ�����ַ���
*���������result���ַ�����ŵ�ַָ��  value������ֵ  width��ת�����
*�����������
*�� �� ֵ������ת��������ַָ��  
*����˵����
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
*�������ƣ�char * itoa16(char * result, int32_t value, uint8_t width)
*����˵��������תʮ�������ַ���
*���������result���ַ�����ŵ�ַָ��  value������ֵ  width��ת�����
*�����������
*�� �� ֵ������ת��������ַָ��  
*����˵����
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
*�������ƣ�int String2ToBytes(const char* pSrc, char* pDst, int nSrcLength)
*����˵�����ɴ�ӡ�ַ���ת��Ϊ�ֽ�����
*���������pSrc - Դ�ַ���ָ��   nSrcLength - Դ�ַ�������
*���������pDst - Ŀ������ָ��
*�� �� ֵ��Ŀ�����ݳ���  
*����˵����"C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}
*******************************************************************************/
int String2ToBytes(const char* pSrc, char* pDst, int nSrcLength)
{
  int i;
  
  for (i = 0; i < nSrcLength; i += 2)
  {
	// �����4λ
	if ((*pSrc >= '0') && (*pSrc <= '9'))
	{
      *pDst = (*pSrc - '0') << 4;
	}
	else
	{
      *pDst = (*pSrc - 'A' + 10) << 4;
	}
	
	pSrc++;
	
	// �����4λ
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
  
  // ����Ŀ�����ݳ���
  return (nSrcLength / 2);
}

/*******************************************************************************
*�������ƣ�int Bytes2ToString(const char* pSrc, char* pDst, int nSrcLength)
*����˵�����ֽ�����ת��Ϊ�ɴ�ӡ�ַ���
*���������pSrc - Դ����ָ��  nSrcLength - Դ���ݳ���
*���������pDst - Ŀ���ַ���ָ��
*�� �� ֵ��Ŀ�����ݳ���   
*����˵����{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01"
*******************************************************************************/
int Bytes2ToString(const char* pSrc, char* pDst, int nSrcLength)
{
  const char hexTab[]="0123456789ABCDEF";	// 0x0-0xf���ַ����ұ�
  int i;
  
  for (i = 0; i < nSrcLength; i++)
  {
	*pDst++ = hexTab[*pSrc >> 4];		// �����4λ
	*pDst++ = hexTab[*pSrc & 0x0f];	// �����4λ
	pSrc++;
  }
  
  // ����ַ����Ӹ�������
  *pDst = '\0';   
  
  // ����Ŀ���ַ�������
  return (nSrcLength * 2);
}





/*******************************************************************************
*�������ƣ�uint16_t AsciiToBcd( char *bcd, const char *asc,  uint16_t len,  char fmt)
*����˵����
*���������
*���������
*�� �� ֵ��  
*����˵����
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
*�������ƣ�HexToDec
*����˵������һ��ASCII�ַ���ת��Ϊ��ֵ
*���������Hex��ת������
*�����������
*�� �� ֵ��ת�������ֵ  
*����˵������ע�⡿��p_HexData��ֵӦ��С��100��������쳣��
*******************************************************************************/
uint8_t HexToDec(uint8_t Hex)
{
  return (((Hex/10) << 4) | (Hex % 10));
}


/*******************************************************************************
*�������ƣ�DecToHex
*����˵������һ����ֵת��Ϊʮ������
*���������Bcd��ת������
*�����������
*�� �� ֵ��ת����ʮ������  
*����˵����
*******************************************************************************/
uint8_t DecToHex(uint8_t Bcd)
{
  return ((((Bcd&0xf0)>>4)*10)+(Bcd&0x0f));
}


/*******************************************************************************
*�������ƣ�void insertionSort(uint16_t *numbers, uint32_t array_size) 
*����˵�������ݴ�С��������
*���������numbers�����ݵ�ַָ��    array_size�����ݳ���
*����������������ݴ�С��������
*�� �� ֵ���� 
*����˵����
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
*�������ƣ�uint32_t interquartileMean(uint16_t *array, uint32_t numOfSamples)
*����˵��������ȥ����Сֵ�����ֵ������ƽ��ֵ
*���������numbers�����ݵ�ַָ��    array_size�����ݳ���
*�����������
*�� �� ֵ��������ƽ��ֵ 
*����˵����
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





