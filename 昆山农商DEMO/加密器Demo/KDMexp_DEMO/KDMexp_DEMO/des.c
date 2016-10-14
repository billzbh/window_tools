/*-------------------------------------------

	Name		:	des.c
	Description	:	
	Author		:	yanghb@keyou.cn
	Version		:	v1.0

---------------------------------------------*/

#include "des.h"

#include "str.h"

// Ϊ�ٸ��Ƽ�����2010.6.21
#define BAIFU_2010_6_21

#ifdef BAIFU_2010_6_21
#include "d3des.h"	
#endif

// ��8�ֽ����ݽ��е���������DES����
void Des_Encrypt(uchar *inData,uchar *outData,const uchar *ekey)
{
	// ���ݾ���MCU��д���㷨
#ifdef BAIFU_2010_6_21
	deskey(ekey,EN0);
	des(inData,outData);
#endif
}

// ��8�ֽ����ݽ��е���������DES����
void Des_Decrypt(uchar *inData,uchar *outData,const uchar *dkey)
{
	// ���ݾ���MCU��д���㷨
#ifdef BAIFU_2010_6_21
	deskey(dkey,DE1);
	des(inData,outData);
#endif
}


////////////////////////////////////////////////////////


/*
*	���ܣ��Գ���Ϊlen������data���е�DES����
*	Ҫ�����ݳ���len����Ϊ8��������,��Կ����Ϊ8�ֽڼ�������
*	˵����data		�����ܵ�����
*		  	   len		���ݳ���
*		  	   ekey	      ��Կ
*	���أ�0�ɹ�  -1ʧ��
*/
int des1_encrypt(uchar *data,uint len,const uchar *ekey)
{
	uchar *ptdata = data;
	uint  sect	   = len / 8; // ����ÿ8�ֽ�һ�λ���
	uint  i;

	if((len % 8) != 0) return -1; // ���ݳ��Ȳ���������

	for(i=0; i<sect; i++)
	{
		Des_Encrypt(ptdata,ptdata,ekey);
		ptdata += 8;
	}

	return 0;
}


/*
*	���ܣ��Գ���Ϊlen������data���е�DES����
*	Ҫ�����ݳ���len����Ϊ8��������,��Կ����Ϊ8�ֽڼ�������
*	˵����data		�����ܵ�����
*		 	   len		���ݳ���
*			   dkey	      ��Կ
*	���أ�0�ɹ�  -1ʧ��
*/
int des1_decrypt(uchar *data,uint len,const uchar *dkey)
{
	uchar *ptdata = data;
	uint  sect	   = len / 8; // ����ÿ8�ֽ�һ�λ���
	uint  i;

	if((len % 8) != 0) return -1; // ���ݳ��Ȳ���������

	for(i=0; i<sect; i++)
	{
		Des_Decrypt(ptdata,ptdata,dkey);
		ptdata += 8;
	}
	
	return 0;
}


/*
*	���ܣ��Գ���Ϊlen������data����3DES����
*	Ҫ�����ݳ���len����Ϊ8����������ģʽΪ0ʱ����Կ������Ϊ16��˫������ģʽΪ1ʱ����Կ������Ϊ24��������
*	˵����data		�����ܵ�����
*		 	   len		���ݳ���
*			   ekey	       ��Կ
*		  	   mode		=0��˫����3DES��	=1��������3DES��
*/
int des3_encrypt(uchar *data,uint len,const uchar *ekey,uchar mode)
{
	unsigned char *ptdata = data;
	unsigned int  sect	   = len / 8; // ����ÿ8�ֽ�һ�λ���
	unsigned int  i;

	if((len % 8) != 0) return -1; // ���ݳ��Ȳ���������

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
*	���ܣ��Գ���Ϊlen������data����3DES����
*	Ҫ�����ݳ���len����Ϊ8����������ģʽΪ0ʱ����Կ������Ϊ16��˫������ģʽΪ1ʱ����Կ������Ϊ24��������
*	˵����data		�����ܵ�����
*		 	   len		���ݳ���
*		 	   dkey	      ��Կ
*		  	  mode		=0��˫����3DES��	=1��������3DES��
*/
int des3_decrypt(uchar *data,uint len,const uchar *dkey,uchar mode)
{
	uchar *ptdata = data;
	uint  sect	   = len / 8; // ����ÿ8�ֽ�һ�λ���
	uint  i;

	if((len % 8) != 0) return -1; // ���ݳ��Ȳ���������

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
*	���ܣ�����У��ֵ
*	Ҫ��
*	˵����sec		��Կ����(Hex)
*		  len		��Կ���ĳ���
*		  chk		У��ֵ(6λ�ַ�)
*	���أ�0 �ɹ� -1 ʧ��
*/
int CalculateCheck (const uchar *sec,uchar len,uchar *chk)
{
	unsigned char key[24];
	unsigned char zero[8];
	unsigned char blk;
	unsigned char i,j;

	blk = len/8;

	memcpy(key,sec,len);
	memset(zero,0,sizeof(zero));// ���8�ֽ�0x0

	if(blk == 1) des1_encrypt(zero,sizeof(zero),key);
	else if(blk == 2) des3_encrypt(zero,sizeof(zero),key,0);
	else if(blk == 3) des3_encrypt(zero,sizeof(zero),key,1);

	for(i=0,j=0; i<8; i++)
	{
		key[j++] = HexToAscii((unsigned char)(zero[i] >> 4));
		key[j++] = HexToAscii((unsigned char)(zero[i] & 0x0f));
	}

	memcpy(chk,key,6); // ���У��ֵ6λ�ַ�

	return 0;
}
