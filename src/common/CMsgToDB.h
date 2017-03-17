#include "xjlib.h"
#include "define_scan.h"
#include "PMC_Adapter_Proxy.h"    //��Ϣ������
#include "MC_STRUCT.h"            //��Ϣ���ĵĽṹ
#include "XJMsgCenterAPI.h"       //��Ϣ���ĺ궨�壬��Ϣ����������ڴ��ļ�
#include "CDBInterface.h"

#ifdef OS_WINDOWS
#ifdef M2D_DLL_FILE 
class _declspec(dllexport) CMsgToDB //������ 
#else 
class _declspec(dllimport) CMsgToDB //������ 
#endif //M2D_DLL_FILE
#endif //OS_WINDOWS

#ifdef OS_LINUX
class CMsgToDB
#endif //OS_LINUX
{
protected:
	
		CMsgToDB(){};

public:
	
	/**
	* @name		init
	* @brief	��ʼ����Դ
	* @param 	pDbCtrlStruct[in]: ���ݿ���ƽṹָ��
	* @return	RES_SUCCESS �ɹ�				RES_FAIL ʧ��
	*/
	virtual int init(DB_CTRL_STRUCT *pDbCtrlStruct, CDBInterface *pDBInterfaceObj) = 0;
	
	/**
	* @name		Release
	* @brief	�ͷ���Դ
	* @param 	void
	* @return	RES_SUCCESS �ɹ�				RES_FAIL ʧ��
	*/
	virtual int release(void) = 0;

};


#ifdef OS_WINDOWS
#ifdef M2D_DLL_FILE //������ 
extern "C"  __declspec( dllexport ) CMsgToDB * create_m2d_instance(void);
extern "C"  __declspec( dllexport ) void destroy_m2d_instance(CMsgToDB * theInterface);
#else //������ 
extern "C"  __declspec( dllimport ) CMsgToDB * create_m2d_instance();
extern "C"  __declspec( dllimport ) void destroy_m2d_instance(CMsgToDB * theInterface);
#endif //M2D_DLL_FILE
#endif //OS_WINDOWS

#ifdef OS_LINUX
#ifdef __cplusplus
extern "C"
{
#endif
	CMsgToDB * create_m2d_instance(void);
	void destroy_m2d_instance(CMsgToDB * theInterface);
#ifdef __cplusplus
}
#endif //
#endif //OS_LINUX