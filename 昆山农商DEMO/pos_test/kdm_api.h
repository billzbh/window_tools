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


// 对终端设备从串口接收到的密钥分发器数据包进行分析处理
// 输入：rxLen			待分析数据包长度
// 		 rxData			待分析数据
// 输出：Deviceid		终端设备业务编号
// 		 decryptedTMK	终端主密钥明文
// 返回：0无效数据包；1处理成功
extern unsigned char dispatchData(unsigned char rxLen, unsigned char* rxData, char* Deviceid, char* decryptedTMK);



// 对终端设备欲发往密钥分发器的数据进行数据封包处理
// 输入：result			=0初始化失败；=1初始化成功
// 		 txLen			txData内数据长度
// 		 txData			终端设备当前时间，如"100312103048",10年3月12日10时30分48秒
// 输出：response		进行封装后的报文，终端设备可将该报文直接发送到密钥分发器
// 返回：封包后的数据报文长度
extern unsigned char assembleMsg(unsigned char result, unsigned char txLen, unsigned char* txData, unsigned char* response);


#ifdef __cplusplus
}
#endif

#endif
