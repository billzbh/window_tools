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


// 将'0'~'9'、'A'~'F'字符组成的字符串两两组合为Hex十六进制数，返回hex数长度
extern uchar DES_StringToHex (char* str, uchar len, uchar* hex);

// 将Hex十六进制数拆分为'0'~'9'、'A'~'F'
extern uchar HexToDES_String (char* str, uchar len, uchar* hex);

// 将'0'~'9'字符组成的字符串转成整型十进制数据
extern uint StringToDec (char* str, uchar len);

// 整型十进制数据转为2位字符
extern void DecToAscii2(char* str, uint dec);

// 整型十进制数据转为4位字符
extern void DecToAscii4(char* str, uint dec);


extern void my_strcpy (uchar *dest,const uchar * src,uint len);
// 相同返回0，否则0xff
extern uchar my_strcmp (uchar *str1,uchar *str2,uint len);
extern uchar my_strlen(uchar *str);

/* 将'0'~'9'和'A'~'F'的ASCII码字符转换为HEX(BCD码) */
extern uchar AsciiToHex(uchar ascii);

/* 将HEX(BCD码)转换为ASCII码字符 */
extern uchar HexToAscii(uchar hex);



#ifdef __cplusplus
}
#endif

#endif
