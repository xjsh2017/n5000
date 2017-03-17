/********************************************************************
	created:	2010/04/08
	created:	8:4:2010   14:40
	filename: 	PMC_Adapter_Proxy.h
	file path:	D:\WorkSpace\Tiger\工作区\开发\代码\mc_proxy
	file base:	PMC_Adapter_Proxy
	file ext:	h
	author:		rwp
	
	purpose:	封装深圳分部的消息中心库（以DLL形式提供）支持按用户的要求启动不同模式的
				服务，用于传递用户定义的消息或数据类型。
*********************************************************************/
#ifndef PMC_ADAPTER_PROXY_H_HEADER_INCLUDED_B447F3A7
#define PMC_ADAPTER_PROXY_H_HEADER_INCLUDED_B447F3A7

#include "XJMsgCenterAPI.h"              //各种宏定义
#include "MC_STRUCT.h"

//##ModelId=4BB55FB0038E
/**
 * @brief       class name: CPMC_Adapter_Proxy
 * @use			导出类提供给用户的使用接口函数
 * @author		rwp   
 * @version     ver1.0
 * @date        
 *
 * example
 * @code
 *
 *
 *
 * @endcode
 */


#ifdef OS_WINDOWS
#ifdef DLL_FILE_MC 
class _declspec(dllexport) CPMC_Adapter_Proxy //导出类 
#else 
class _declspec(dllimport) CPMC_Adapter_Proxy //导入类 
#endif
#endif //OS_WINDOWS

#ifdef OS_LINUX
class CPMC_Adapter_Proxy
#endif //OS_LINUX
{
  protected:
	/**
	* @brief	CPMC_Adapter_Proxy	构造函数
	* @return	
	* @notes	无
	* @sample	无
	*/
	//CPMC_Adapter_Proxy(){};
  public:

    //##ModelId=4BB561A10167
	/**
	* @brief	Init	初始化消息中心（深圳消息中心库二次封装库）
	* @param 	[In]int nServiceType	服务类型
	* @param 	[In]PFUNPUTMESSAGE pFunUserCallBack	用户回调函数（用于推送报文到调用模块）
	* @return	virtual int	0：成功 其它：失败
	* @notes	按传的服务类型启动消息中心、设置用户回调函数地址
	* @sample	无
	*/
    virtual int Init(int nServiceType, PFUNPUTMESSAGE pFunUserCallBack,int nTopic=0x0001) = 0;
	
	/**
	* @brief	StartMcCenter	初始化消息中心（深圳消息中心库二次封装库）扩展接口，增加了节点名称，节点编号等输入 add by yzh20120323
	* @param 	[In]MC_CONFIG_STRUCT* pConfig	消息中心启动时的配置接口
	* @param 	[In]PFUNPUTMESSAGE pFunUserCallBack	用户回调函数（用于推送报文到调用模块）
	* @return	virtual int	0：成功 其它：失败
	* @notes	按传的服务类型启动消息中心、设置用户回调函数地址
	* @sample	无
	*/
    virtual int StartMcCenter(MC_CONFIG_STRUCT* pConfig) = 0;

    //##ModelId=4BB561AA0121
	/**
	* @brief	Release	释放消息中心资源
	* @return	virtual int	0： 成功 其它：失败
	* @notes	无
	* @sample	无
	*/
    virtual int Release() = 0;

    //##ModelId=4BB561BA0301
	/**
	* @brief	SendMsg	发出消息
	* @return	virtual int	0：成功 其它：失败
	* @notes	无
	* @sample	无
	*/
    virtual int SendMsg(T_MESSAGE& pOutMsg, int nPriority=0) = 0;

	//##ModelId=4BB561BA0301
	/**
	* @brief	SendMsg_DataMsg	发出数据消息
	* @return	virtual int	0：成功 其它：失败
	* @notes	无
	* @sample	无
	*/
    virtual int SendMsg_DataMsg(DATA_MESSAGE& pOutDataMsg, int nPriority=0) = 0;
		//##ModelId=4BB561BA0301
	/**
	* @brief	SendMsg_DataMsg	发出数据消息
	* @return	virtual int	0：成功 其它：失败
	* @notes	无
	* @sample	无
	*/
    virtual int SendMsg_FrameMsg(FRAME_MESSAGE& pOutFrameMsg, int nPriority=0) = 0;
	//##ModelId=4BB561BA0302
	/**
	* @brief	RegisterTopicType	注册主题
	* @param 	[In]int nTopicVal	主题类型
	* @return	virtual int	0：成功 其它：失败
	* @notes	无
	* @sample	无
	*/
	virtual int RegisterTopicType(int nTopicVal) = 0;
	/**
	* @brief	RegisterUserCallBackFun	            注册T_MESSAGE回调函数
	* @param 	[In]PFUNPUTMESSAGE pUserCallbackFun	服务类型
	* @return	bool	true: 成功 false: 失败
	* @notes	无
	* @sample	无
	*/
	virtual bool RegisterUserCallBackFun(PFUNPUTMESSAGE pUserCallbackFun) = 0;

	/**
	* @brief	RegisterUserCallBackFun_DATAMESSAGE	            注册PFUNMCDATAMESSAGE回调函数
	* @param 	[In]PFUNMCCONNETMESSAGE pUserCallbackFun_ConnectMsg
	* @return	bool	true: 成功 false: 失败
	* @notes	无
	* @sample	无
	*/
	virtual bool RegisterUserCallBackFun_DATAMSG(PFUNMCDATAMESSAGE pUserCallbackFun_DataMsg) = 0;
    /**
	* @brief	RegisterUserCallBackFun_FRAMEMSG	            注册MC_CONNECTMSG回调函数
	* @param 	[In]PFUNMCCONNETMESSAGE pUserCallbackFun_ConnectMsg
	* @return	bool	true: 成功 false: 失败
	* @notes	无
	* @sample	无
	*/
	virtual bool RegisterUserCallBackFun_FRAMEMSG(PFUNMCFRMEMESSAGE pUserCallbackFun_FrameMsg) = 0;
	/**
	* @brief	GetAllConnetMsg	            得到所有节点状态接口
	* @param 	[In]int& nConnectNum        
	* @return	MC_CONNECTMSG*              没有节点的时候不返回
	* @notes	无
	* @sample	无
	*/
	virtual  MC_CONNECTMSG* GetAllConnetMsg(int& nConnectNum)= 0;

	/**
	* @brief	RegisterUserCallBackFun_CONNETMSG	            注册MC_CONNECTMSG回调函数
	* @param 	[In]PFUNMCCONNETMESSAGE pUserCallbackFun_ConnectMsg
	* @return	bool	true: 成功 false: 失败
	* @notes	无
	* @sample	无
	*/
	virtual bool RegisterUserCallBackFun_CONNETMSG(PFUNMCCONNETMESSAGE pUserCallbackFun_ConnectMsg) = 0;

};

#ifdef OS_WINDOWS
#ifdef DLL_FILE_MC
//导出构造析构函数
extern "C"  __declspec( dllexport ) CPMC_Adapter_Proxy * CreateMCInstance();
extern "C"  __declspec( dllexport ) void DestroyMCInstance(CPMC_Adapter_Proxy * theInterface);
#else 
//导入构造析构函数
extern "C"  __declspec( dllimport ) CPMC_Adapter_Proxy * CreateMCInstance();
extern "C"  __declspec( dllimport ) void DestroyMCInstance(CPMC_Adapter_Proxy* theInterface);
#endif 
#endif //OS_WINDOWS

#ifdef OS_LINUX
#ifdef __cplusplus
extern "C"
{
#endif
	/**
	* @brief	function name:"createMHInstance"
	* @use		对象创建函数
	* @param 	[type] char * [name] cProType  规约类型特征字符串
	* @return	CMsgHandler *  
	*/
	CPMC_Adapter_Proxy * CreateMCInstance();

	/**
	* @brief	function name:"destroyMHInstance"
	* @use		对象销毁函数
	* @param 	[type] CMsgHandler * [name] theInterface  对象指针
	* @return	void
	*/
	void DestroyMCInstance(CPMC_Adapter_Proxy * theInterface);
#ifdef __cplusplus
}
#endif
#endif //OS_LINUX
#endif /* PMC_ADAPTER_PROXY_H_HEADER_INCLUDED_B447F3A7 */
