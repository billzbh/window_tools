/*-------------------------------------------

	Name		:	str.c
	Description	:	
	Author		:	yanghb@keyou.cn
	Version		:	v1.0

---------------------------------------------*/
#include "str.h"


// ��'0'~'9'��'A'~'F'�ַ���ɵ��ַ����������ΪHexʮ��������������hex������
uchar DES_StringToHex (char* str, uchar len, uchar* hex)
{
	uint i;
	uchar temp;
	uchar tmp;
	uchar cnt = 0;

	for(i=0; i<len; )
	{
		temp = str[i++];
		if(temp >= '0' && temp <= '9') tmp = (temp - '0') + 0x00;
		else
		if(temp >= 'A' && temp <= 'F') tmp = (temp - 'A') + 0x0A;
		else
		if(temp >= 'a' && temp <= 'f') tmp = (temp - 'a') + 0x0A;
		else return 0;

		tmp <<= 4;
		if(i >= len) temp = '0'; // ��0
		else		 temp = str[i++];
		if(temp >= '0' && temp <= '9') tmp |= ((temp - '0') + 0x00);
		else
		if(temp >= 'A' && temp <= 'F') tmp |= ((temp - 'A') + 0x0A);
		else
		if(temp >= 'a' && temp <= 'f') tmp |= ((temp - 'a') + 0x0A);
		else return 0;

		hex[cnt++] = tmp;
	}
	return cnt;
}


// ��Hexʮ�����������Ϊ'0'~'9'��'A'~'F'
uchar HexToDES_String (char* str, uchar len, uchar* hex)
{
	uchar i;
	uchar temp;
	uchar cnt = 0;

	for(i=0; i<len; i++)
	{
		temp = hex[i] >> 4;
		if(temp < 0x0a) str[cnt++] =(char)(temp + '0');
		else			str[cnt++] =(char)(temp - 0x0a + 'A');
		temp = hex[i] & 0x0F;
		if(temp < 0x0a) str[cnt++] =(char)(temp + '0');
		else			str[cnt++] =(char)(temp - 0x0a + 'A');
	}
	return cnt;
}

// ��'0'~'9'�ַ���ɵ��ַ���ת������ʮ��������
uint StringToDec (char* str, uchar len)
{
	uchar i;
	uchar temp;
	uint dat = 0;
	uint tmp;

	for(i=0; i<len; i++)
	{
		if(str[i] < '0' || str[i] > '9') return 0;
		temp = str[i] - '0';
		tmp = (unsigned int)(dat * 10);
		dat = tmp + temp;
	}
	return dat;
}


// ����ʮ��������תΪ2λ�ַ�
void DecToAscii2(char* str, uint dec)
{
	uint  temp = dec;
	char *pt  = str;

	*pt++ = (temp/10 + '0');
	*pt++ = (temp%10 + '0');
}


// ����ʮ��������תΪ4λ�ַ�
void DecToAscii4(char* str, uint dec)
{
	uint  temp = dec;
	char *pt  = str;

	*pt++ = (temp/1000 + '0');
	temp %= 1000;
	*pt++ = (temp/100 + '0');
	temp %= 100;
	*pt++ = (temp/10 + '0');
	*pt++ = (temp%10 + '0');
}


void my_strcpy (uchar *dest,const uchar * src,uint len)
{
	 uint i = len;

	 while(i--) *dest++ = *src++;
}


uchar my_strcmp (uchar *str1,uchar *str2,uint len)
{
	 uint i = 0;

	 for(i=0; i<len; i++) if(*str1++ != *str2++) return 0xff;

	 return 0;
}


uchar my_strlen(uchar *str)
{
	uchar len = 0;

	while( str[len] ) len++;

	return len;
}


/* ��'0'~'9'��'A'~'F'��ASCII���ַ�ת��ΪHEX(BCD��) */
uchar AsciiToHex(uchar ascii)
{
	uchar temp = ascii;
	 if(temp >= '0' && temp <= '9') return( (temp - '0') + 0x0 );
	 else if(temp >= 'A' && temp <= 'F') return( (temp - 'A') + 0x0a);
	 else if(temp >= 'a' && temp <= 'f') return( (temp - 'a') + 0x0a);
	 return 0;
}


/* ��HEX(BCD��)ת��ΪASCII���ַ� */
uchar HexToAscii(uchar hex)
{
	uchar temp = hex;
	 temp &= 0x0f;

	 if(temp >= 0x0a) return ( (temp - 0x0a) + 'A');
	 else			 return ( (temp - 0x00) + '0');
}

