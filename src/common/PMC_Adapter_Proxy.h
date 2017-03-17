/********************************************************************
	created:	2010/04/08
	created:	8:4:2010   14:40
	filename: 	PMC_Adapter_Proxy.h
	file path:	D:\WorkSpace\Tiger\������\����\����\mc_proxy
	file base:	PMC_Adapter_Proxy
	file ext:	h
	author:		rwp
	
	purpose:	��װ���ڷֲ�����Ϣ���Ŀ⣨��DLL��ʽ�ṩ��֧�ְ��û���Ҫ��������ͬģʽ��
				�������ڴ����û��������Ϣ���������͡�
*********************************************************************/
#ifndef PMC_ADAPTER_PROXY_H_HEADER_INCLUDED_B447F3A7
#define PMC_ADAPTER_PROXY_H_HEADER_INCLUDED_B447F3A7

#include "XJMsgCenterAPI.h"              //���ֺ궨��
#include "MC_STRUCT.h"

//##ModelId=4BB55FB0038E
/**
 * @brief       class name: CPMC_Adapter_Proxy
 * @use			�������ṩ���û���ʹ�ýӿں���
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
class _declspec(dllexport) CPMC_Adapter_Proxy //������ 
#else 
class _declspec(dllimport) CPMC_Adapter_Proxy //������ 
#endif
#endif //OS_WINDOWS

#ifdef OS_LINUX
class CPMC_Adapter_Proxy
#endif //OS_LINUX
{
  protected:
	/**
	* @brief	CPMC_Adapter_Proxy	���캯��
	* @return	
	* @notes	��
	* @sample	��
	*/
	//CPMC_Adapter_Proxy(){};
  public:

    //##ModelId=4BB561A10167
	/**
	* @brief	Init	��ʼ����Ϣ���ģ�������Ϣ���Ŀ���η�װ�⣩
	* @param 	[In]int nServiceType	��������
	* @param 	[In]PFUNPUTMESSAGE pFunUserCallBack	�û��ص��������������ͱ��ĵ�����ģ�飩
	* @return	virtual int	0���ɹ� ������ʧ��
	* @notes	�����ķ�������������Ϣ���ġ������û��ص�������ַ
	* @sample	��
	*/
    virtual int Init(int nServiceType, PFUNPUTMESSAGE pFunUserCallBack,int nTopic=0x0001) = 0;
	
	/**
	* @brief	StartMcCenter	��ʼ����Ϣ���ģ�������Ϣ���Ŀ���η�װ�⣩��չ�ӿڣ������˽ڵ����ƣ��ڵ��ŵ����� add by yzh20120323
	* @param 	[In]MC_CONFIG_STRUCT* pConfig	��Ϣ��������ʱ�����ýӿ�
	* @param 	[In]PFUNPUTMESSAGE pFunUserCallBack	�û��ص��������������ͱ��ĵ�����ģ�飩
	* @return	virtual int	0���ɹ� ������ʧ��
	* @notes	�����ķ�������������Ϣ���ġ������û��ص�������ַ
	* @sample	��
	*/
    virtual int StartMcCenter(MC_CONFIG_STRUCT* pConfig) = 0;

    //##ModelId=4BB561AA0121
	/**
	* @brief	Release	�ͷ���Ϣ������Դ
	* @return	virtual int	0�� �ɹ� ������ʧ��
	* @notes	��
	* @sample	��
	*/
    virtual int Release() = 0;

    //##ModelId=4BB561BA0301
	/**
	* @brief	SendMsg	������Ϣ
	* @return	virtual int	0���ɹ� ������ʧ��
	* @notes	��
	* @sample	��
	*/
    virtual int SendMsg(T_MESSAGE& pOutMsg, int nPriority=0) = 0;

	//##ModelId=4BB561BA0301
	/**
	* @brief	SendMsg_DataMsg	����������Ϣ
	* @return	virtual int	0���ɹ� ������ʧ��
	* @notes	��
	* @sample	��
	*/
    virtual int SendMsg_DataMsg(DATA_MESSAGE& pOutDataMsg, int nPriority=0) = 0;
		//##ModelId=4BB561BA0301
	/**
	* @brief	SendMsg_DataMsg	����������Ϣ
	* @return	virtual int	0���ɹ� ������ʧ��
	* @notes	��
	* @sample	��
	*/
    virtual int SendMsg_FrameMsg(FRAME_MESSAGE& pOutFrameMsg, int nPriority=0) = 0;
	//##ModelId=4BB561BA0302
	/**
	* @brief	RegisterTopicType	ע������
	* @param 	[In]int nTopicVal	��������
	* @return	virtual int	0���ɹ� ������ʧ��
	* @notes	��
	* @sample	��
	*/
	virtual int RegisterTopicType(int nTopicVal) = 0;
	/**
	* @brief	RegisterUserCallBackFun	            ע��T_MESSAGE�ص�����
	* @param 	[In]PFUNPUTMESSAGE pUserCallbackFun	��������
	* @return	bool	true: �ɹ� false: ʧ��
	* @notes	��
	* @sample	��
	*/
	virtual bool RegisterUserCallBackFun(PFUNPUTMESSAGE pUserCallbackFun) = 0;

	/**
	* @brief	RegisterUserCallBackFun_DATAMESSAGE	            ע��PFUNMCDATAMESSAGE�ص�����
	* @param 	[In]PFUNMCCONNETMESSAGE pUserCallbackFun_ConnectMsg
	* @return	bool	true: �ɹ� false: ʧ��
	* @notes	��
	* @sample	��
	*/
	virtual bool RegisterUserCallBackFun_DATAMSG(PFUNMCDATAMESSAGE pUserCallbackFun_DataMsg) = 0;
    /**
	* @brief	RegisterUserCallBackFun_FRAMEMSG	            ע��MC_CONNECTMSG�ص�����
	* @param 	[In]PFUNMCCONNETMESSAGE pUserCallbackFun_ConnectMsg
	* @return	bool	true: �ɹ� false: ʧ��
	* @notes	��
	* @sample	��
	*/
	virtual bool RegisterUserCallBackFun_FRAMEMSG(PFUNMCFRMEMESSAGE pUserCallbackFun_FrameMsg) = 0;
	/**
	* @brief	GetAllConnetMsg	            �õ����нڵ�״̬�ӿ�
	* @param 	[In]int& nConnectNum        
	* @return	MC_CONNECTMSG*              û�нڵ��ʱ�򲻷���
	* @notes	��
	* @sample	��
	*/
	virtual  MC_CONNECTMSG* GetAllConnetMsg(int& nConnectNum)= 0;

	/**
	* @brief	RegisterUserCallBackFun_CONNETMSG	            ע��MC_CONNECTMSG�ص�����
	* @param 	[In]PFUNMCCONNETMESSAGE pUserCallbackFun_ConnectMsg
	* @return	bool	true: �ɹ� false: ʧ��
	* @notes	��
	* @sample	��
	*/
	virtual bool RegisterUserCallBackFun_CONNETMSG(PFUNMCCONNETMESSAGE pUserCallbackFun_ConnectMsg) = 0;

};

#ifdef OS_WINDOWS
#ifdef DLL_FILE_MC
//����������������
extern "C"  __declspec( dllexport ) CPMC_Adapter_Proxy * CreateMCInstance();
extern "C"  __declspec( dllexport ) void DestroyMCInstance(CPMC_Adapter_Proxy * theInterface);
#else 
//���빹����������
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
	* @use		���󴴽�����
	* @param 	[type] char * [name] cProType  ��Լ���������ַ���
	* @return	CMsgHandler *  
	*/
	CPMC_Adapter_Proxy * CreateMCInstance();

	/**
	* @brief	function name:"destroyMHInstance"
	* @use		�������ٺ���
	* @param 	[type] CMsgHandler * [name] theInterface  ����ָ��
	* @return	void
	*/
	void DestroyMCInstance(CPMC_Adapter_Proxy * theInterface);
#ifdef __cplusplus
}
#endif
#endif //OS_LINUX
#endif /* PMC_ADAPTER_PROXY_H_HEADER_INCLUDED_B447F3A7 */
