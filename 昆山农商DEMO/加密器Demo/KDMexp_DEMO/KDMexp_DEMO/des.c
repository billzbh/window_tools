/*-------------------------------------------

	Name		:	des.c
	Description	:	
	Author		:	yanghb@keyou.cn
	Version		:	v1.0

---------------------------------------------*/

#include "des.h"

#include "str.h"

// 为百富科技定制2010.6.21
#define BAIFU_2010_6_21

#ifdef BAIFU_2010_6_21
#include "d3des.h"	
#endif

// 对8字节数据进行单倍长数据DES加密
void Des_Encrypt(uchar *inData,uchar *outData,const uchar *ekey)
{
	// 根据具体MCU编写该算法
#ifdef BAIFU_2010_6_21
	deskey(ekey,EN0);
	des(inData,outData);
#endif
}

// 对8字节数据进行单倍长数据DES解密
void Des_Decrypt(uchar *inData,uchar *outData,const uchar *dkey)
{
	// 根据具体MCU编写该算法
#ifdef BAIFU_2010_6_21
	deskey(dkey,DE1);
	des(inData,outData);
#endif
}


////////////////////////////////////////////////////////


/*
*	功能：对长度为len的数据data进行单DES加密
*	要求：数据长度len必须为8的整数倍,密钥长度为8字节即单倍长
*	说明：data		待加密的数据
*		  	   len		数据长度
*		  	   ekey	      密钥
*	返回：0成功  -1失败
*/
int des1_encrypt(uchar *data,uint len,const uchar *ekey)
{
	uchar *ptdata = data;
	uint  sect	   = len / 8; // 数据每8字节一段划分
	uint  i;

	if((len % 8) != 0) return -1; // 数据长度不满足条件

	for(i=0; i<sect; i++)
	{
		Des_Encrypt(ptdata,ptdata,ekey);
		ptdata += 8;
	}

	return 0;
}


/*
*	功能：对长度为len的数据data进行单DES解密
*	要求：数据长度len必须为8的整数倍,密钥长度为8字节即单倍长
*	说明：data		待解密的数据
*		 	   len		数据长度
*			   dkey	      密钥
*	返回：0成功  -1失败
*/
int des1_decrypt(uchar *data,uint len,const uchar *dkey)
{
	uchar *ptdata = data;
	uint  sect	   = len / 8; // 数据每8字节一段划分
	uint  i;

	if((len % 8) != 0) return -1; // 数据长度不满足条件

	for(i=0; i<sect; i++)
	{
		Des_Decrypt(ptdata,ptdata,dkey);
		ptdata += 8;
	}
	
	return 0;
}


/*
*	功能：对长度为len的数据data进行3DES加密
*	要求：数据长度len必须为8的整数倍，模式为0时，密钥长度须为16及双倍长；模式为1时，密钥长度须为24即三倍长
*	说明：data		待加密的数据
*		 	   len		数据长度
*			   ekey	       密钥
*		  	   mode		=0（双倍长3DES）	=1（三倍长3DES）
*/
int des3_encrypt(uchar *data,uint len,const uchar *ekey,uchar mode)
{
	unsigned char *ptdata = data;
	unsigned int  sect	   = len / 8; // 数据每8字节一段划分
	unsigned int  i;

	if((len % 8) != 0) return -1; // 数据长度不满足条件

	for(i=0; i<sect; i++)
	{
		Des_Encrypt(ptdata,ptdata,ekey);	// k1+
		Des_Decrypt(ptdata,ptdata,ekey+8);	// k2-
		if(mode == 0) Des_Encrypt(ptdata,ptdata,ekey); // (k3 = k1)+
		else Des_Encrypt(ptdata,ptdata,ekey+16);	// (k3=k3)+

		ptdata += 8;
	}
	return 0;
}


/*
*	功能：对长度为len的数据data进行3DES解密
*	要求：数据长度len必须为8的整数倍，模式为0时，密钥长度须为16及双倍长；模式为1时，密钥长度须为24即三倍长
*	说明：data		待解密的数据
*		 	   len		数据长度
*		 	   dkey	      密钥
*		  	  mode		=0（双倍长3DES）	=1（三倍长3DES）
*/
int des3_decrypt(uchar *data,uint len,const uchar *dkey,uchar mode)
{
	uchar *ptdata = data;
	uint  sect	   = len / 8; // 数据每8字节一段划分
	uint  i;

	if((len % 8) != 0) return -1; // 数据长度不满足条件

	for(i=0; i<sect; i++)
	{
		if(mode == 0) Des_Decrypt(ptdata,ptdata,dkey); // (k3=k1)-
		else Des_Decrypt(ptdata,ptdata,dkey+16); // (k3=k3)-
		Des_Encrypt(ptdata,ptdata,dkey+8);	// k2+
		Des_Decrypt(ptdata,ptdata,dkey);	// k1-
		
		ptdata += 8;
	}
	return 0;
}


/*
*	功能：计算校验值
*	要求：
*	说明：sec		密钥明文(Hex)
*		  len		密钥明文长度
*		  chk		校验值(6位字符)
*	返回：0 成功 -1 失败
*/
int CalculateCheck (const uchar *sec,uchar len,uchar *chk)
{
	unsigned char key[24];
	unsigned char zero[8];
	unsigned char blk;
	unsigned char i,j;

	blk = len/8;

	memcpy(key,sec,len);
	memset(zero,0,sizeof(zero));// 填充8字节0x0

	if(blk == 1) des1_encrypt(zero,sizeof(zero),key);
	else if(blk == 2) des3_encrypt(zero,sizeof(zero),key,0);
	else if(blk == 3) des3_encrypt(zero,sizeof(zero),key,1);

	for(i=0,j=0; i<8; i++)
	{
		key[j++] = HexToAscii((unsigned char)(zero[i] >> 4));
		key[j++] = HexToAscii((unsigned char)(zero[i] & 0x0f));
	}

	memcpy(chk,key,6); // 输出校验值6位字符

	return 0;
}
