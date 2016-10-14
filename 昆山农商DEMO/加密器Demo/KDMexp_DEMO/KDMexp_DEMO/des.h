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


// ��8�ֽ����ݽ��е���������DES����
extern void Des_Encrypt(uchar *inData,uchar *outData,const uchar *ekey);

// ��8�ֽ����ݽ��е���������DES����
extern void Des_Decrypt(uchar *inData,uchar *outData,const uchar *dkey);


/*
*	���ܣ��Գ���Ϊlen������data���е�DES����
*	Ҫ�����ݳ���len����Ϊ8��������,��Կ����Ϊ8�ֽڼ�������
*	˵����data		�����ܵ�����
*		  	   len		���ݳ���
*		  	   ekey	      ��Կ
*	���أ�0�ɹ�  -1ʧ��
*/
extern int des1_encrypt(uchar *data,uint len,const uchar *ekey);

/*
*	���ܣ��Գ���Ϊlen������data���е�DES����
*	Ҫ�����ݳ���len����Ϊ8��������,��Կ����Ϊ8�ֽڼ�������
*	˵����data		�����ܵ�����
*		 	   len		���ݳ���
*			   dkey	      ��Կ
*	���أ�0�ɹ�  -1ʧ��
*/
extern int des1_decrypt(uchar *data,uint len,const uchar *dkey);

/*
*	���ܣ��Գ���Ϊlen������data����3DES����
*	Ҫ�����ݳ���len����Ϊ8����������ģʽΪ0ʱ����Կ������Ϊ16��˫������ģʽΪ1ʱ����Կ������Ϊ24��������
*	˵����data		�����ܵ�����
*		 	   len		���ݳ���
*			   ekey	       ��Կ
*		  	   mode		=0��˫����3DES��	=1��������3DES��
*/
extern int des3_encrypt(uchar *data,uint len,const uchar *ekey,uchar mode);

/*
*	���ܣ��Գ���Ϊlen������data����3DES����
*	Ҫ�����ݳ���len����Ϊ8����������ģʽΪ0ʱ����Կ������Ϊ16��˫������ģʽΪ1ʱ����Կ������Ϊ24��������
*	˵����data		�����ܵ�����
*		 	   len		���ݳ���
*		 	   dkey	      ��Կ
*		  	  mode		=0��˫����3DES��	=1��������3DES��
*/
extern int des3_decrypt(uchar *data,uint len,const uchar *dkey,uchar mode);

/*
*	���ܣ�����У��ֵ
*	Ҫ��
*	˵����sec		��Կ����(Hex)
*		  len		��Կ���ĳ���
*		  chk		У��ֵ(6λ�ַ�)
*	���أ�0 �ɹ� -1 ʧ��
*/
extern int CalculateCheck (const uchar *sec,uchar len,uchar *chk);



#ifdef __cplusplus
}
#endif

#endif