/*-------------------------------------------

	Name		:	str.h
	Description	:	
	Author		:	yanghb@keyou.cn
	Version		:	v1.0

---------------------------------------------*/

#ifndef MY_STRING_H
#define MY_STRING_H

#include "typedef.h"



#ifdef __cplusplus
extern "C" {
#endif


// ��'0'~'9'��'A'~'F'�ַ���ɵ��ַ����������ΪHexʮ��������������hex������
extern uchar DES_StringToHex (char* str, uchar len, uchar* hex);

// ��Hexʮ�����������Ϊ'0'~'9'��'A'~'F'
extern uchar HexToDES_String (char* str, uchar len, uchar* hex);

// ��'0'~'9'�ַ���ɵ��ַ���ת������ʮ��������
extern uint StringToDec (char* str, uchar len);

// ����ʮ��������תΪ2λ�ַ�
extern void DecToAscii2(char* str, uint dec);

// ����ʮ��������תΪ4λ�ַ�
extern void DecToAscii4(char* str, uint dec);


extern void my_strcpy (uchar *dest,const uchar * src,uint len);
// ��ͬ����0������0xff
extern uchar my_strcmp (uchar *str1,uchar *str2,uint len);
extern uchar my_strlen(uchar *str);

/* ��'0'~'9'��'A'~'F'��ASCII���ַ�ת��ΪHEX(BCD��) */
extern uchar AsciiToHex(uchar ascii);

/* ��HEX(BCD��)ת��ΪASCII���ַ� */
extern uchar HexToAscii(uchar hex);



#ifdef __cplusplus
}
#endif

#endif
