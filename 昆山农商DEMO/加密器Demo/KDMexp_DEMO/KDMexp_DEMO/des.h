/*-------------------------------------------

	Name		:	des.h
	Description	:	
	Author		:	yanghb@keyou.cn
	Version		:	v1.0

---------------------------------------------*/

#ifndef MY_DES_H
#define MY_DES_H

#include "typedef.h"


#ifdef __cplusplus
extern "C" {
#endif


// 对8字节数据进行单倍长数据DES加密
extern void Des_Encrypt(uchar *inData,uchar *outData,const uchar *ekey);

// 对8字节数据进行单倍长数据DES解密
extern void Des_Decrypt(uchar *inData,uchar *outData,const uchar *dkey);


/*
*	功能：对长度为len的数据data进行单DES加密
*	要求：数据长度len必须为8的整数倍,密钥长度为8字节即单倍长
*	说明：data		待加密的数据
*		  	   len		数据长度
*		  	   ekey	      密钥
*	返回：0成功  -1失败
*/
extern int des1_encrypt(uchar *data,uint len,const uchar *ekey);

/*
*	功能：对长度为len的数据data进行单DES解密
*	要求：数据长度len必须为8的整数倍,密钥长度为8字节即单倍长
*	说明：data		待解密的数据
*		 	   len		数据长度
*			   dkey	      密钥
*	返回：0成功  -1失败
*/
extern int des1_decrypt(uchar *data,uint len,const uchar *dkey);

/*
*	功能：对长度为len的数据data进行3DES加密
*	要求：数据长度len必须为8的整数倍，模式为0时，密钥长度须为16及双倍长；模式为1时，密钥长度须为24即三倍长
*	说明：data		待加密的数据
*		 	   len		数据长度
*			   ekey	       密钥
*		  	   mode		=0（双倍长3DES）	=1（三倍长3DES）
*/
extern int des3_encrypt(uchar *data,uint len,const uchar *ekey,uchar mode);

/*
*	功能：对长度为len的数据data进行3DES解密
*	要求：数据长度len必须为8的整数倍，模式为0时，密钥长度须为16及双倍长；模式为1时，密钥长度须为24即三倍长
*	说明：data		待解密的数据
*		 	   len		数据长度
*		 	   dkey	      密钥
*		  	  mode		=0（双倍长3DES）	=1（三倍长3DES）
*/
extern int des3_decrypt(uchar *data,uint len,const uchar *dkey,uchar mode);

/*
*	功能：计算校验值
*	要求：
*	说明：sec		密钥明文(Hex)
*		  len		密钥明文长度
*		  chk		校验值(6位字符)
*	返回：0 成功 -1 失败
*/
extern int CalculateCheck (const uchar *sec,uchar len,uchar *chk);



#ifdef __cplusplus
}
#endif

#endif