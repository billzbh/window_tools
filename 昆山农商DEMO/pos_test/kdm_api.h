/*-------------------------------------------

	Name		:	kdm_api.h
	Description	:	
	Author		:	yanghb@keyou.cn
	Version		:	v1.0

---------------------------------------------*/

#ifndef KDM_API_H
#define KDM_API_H


#ifdef __cplusplus
extern "C" {
#endif


// ���ն��豸�Ӵ��ڽ��յ�����Կ�ַ������ݰ����з�������
// ���룺rxLen			���������ݰ�����
// 		 rxData			����������
// �����Deviceid		�ն��豸ҵ����
// 		 decryptedTMK	�ն�����Կ����
// ���أ�0��Ч���ݰ���1����ɹ�
extern unsigned char dispatchData(unsigned char rxLen, unsigned char* rxData, char* Deviceid, char* decryptedTMK);



// ���ն��豸��������Կ�ַ��������ݽ������ݷ������
// ���룺result			=0��ʼ��ʧ�ܣ�=1��ʼ���ɹ�
// 		 txLen			txData�����ݳ���
// 		 txData			�ն��豸��ǰʱ�䣬��"100312103048",10��3��12��10ʱ30��48��
// �����response		���з�װ��ı��ģ��ն��豸�ɽ��ñ���ֱ�ӷ��͵���Կ�ַ���
// ���أ����������ݱ��ĳ���
extern unsigned char assembleMsg(unsigned char result, unsigned char txLen, unsigned char* txData, unsigned char* response);


#ifdef __cplusplus
}
#endif

#endif
