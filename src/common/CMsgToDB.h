#include "xjlib.h"
#include "define_scan.h"
#include "PMC_Adapter_Proxy.h"    //消息导出类
#include "MC_STRUCT.h"            //消息中心的结构
#include "XJMsgCenterAPI.h"       //消息中心宏定义，消息分类均定义在此文件
#include "CDBInterface.h"

#ifdef OS_WINDOWS
#ifdef M2D_DLL_FILE 
class _declspec(dllexport) CMsgToDB //导出类 
#else 
class _declspec(dllimport) CMsgToDB //导入类 
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
	* @brief	初始化资源
	* @param 	pDbCtrlStruct[in]: 数据库控制结构指针
	* @return	RES_SUCCESS 成功				RES_FAIL 失败
	*/
	virtual int init(DB_CTRL_STRUCT *pDbCtrlStruct, CDBInterface *pDBInterfaceObj) = 0;
	
	/**
	* @name		Release
	* @brief	释放资源
	* @param 	void
	* @return	RES_SUCCESS 成功				RES_FAIL 失败
	*/
	virtual int release(void) = 0;

};


#ifdef OS_WINDOWS
#ifdef M2D_DLL_FILE //导出类 
extern "C"  __declspec( dllexport ) CMsgToDB * create_m2d_instance(void);
extern "C"  __declspec( dllexport ) void destroy_m2d_instance(CMsgToDB * theInterface);
#else //导入类 
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