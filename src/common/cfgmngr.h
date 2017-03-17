
#include "SYS_MODEL_STRUCT.h"       //系统模型头文件
#include "WAVE_RECORD_STRUCT.h"     //录波器模型头文件
#include "SYS_CONFIGMODEL_STRUCT.h" //配置项模型头文件
#ifdef OS_WINDOWS

#ifdef CFG_DLL_MNGR 
//加载配置文件
extern "C"  __declspec( dllexport ) int cfgmngr_initialize(const char* ccfgfilefullpath);
//释放
extern "C"  __declspec( dllexport ) int cfgmngr_uninitialize(void);
//获取模型文件指针
extern "C"  __declspec( dllexport ) SCANNER* cfgmngr_get_sysmodel(void);
//根据采样端口系统编号获取对应的采样装置指针 
extern "C"  __declspec( dllexport ) SNIFFER* cfgmngr_get_sniffer(int nsys_portseq);

//根据采样端口系统编号获取对应的采样板卡指针
extern "C"  __declspec( dllexport ) SNIFFER_CARD* cfgmngr_get_sniffercard(int nsys_portseq);
//根据采样端口系统编号获取对应的采样端口指针
extern "C"  __declspec( dllexport ) SNIFFER_PORT* cfgmngr_get_snifferport(int nsys_portseq);
//获取采集端口队列
extern "C"  __declspec( dllexport ) SNIFFER_PORT** cfgmnfr_get_snifferportlist(int& nlistsize); 
//根据ip地址或者mac地址查询到对应的ied
extern "C"  __declspec( dllexport ) SNIFFER_APP*   cfgmngr_get_ied_bynetidaddr(char* cNetaddr);
//根据appid查询到对应的ied装置指针
extern "C"  __declspec( dllexport ) SNIFFER_APP*   cfgmngr_get_ied_byappid(int nappid);
//根据系统编号查找详细信息
extern "C"  __declspec( dllexport ) SNIFFER_APP*   cfgmngr_get_ied_bysysid(int nsysid);
//获取指定模块指定项配置值，输入参数区分大小写
extern "C"  __declspec( dllexport ) CFG_ITEM* cfgmngr_get_cfgitemvalue(const char* c_model_name,const char* c_item_name);
//获取指定装置指定项配置值，输出参数区分大小写
extern "C"  __declspec( dllexport )CFG_ITEM* cfgmngr_get_sniffer_cfgitemvalue(int nsnifferid, const char* c_model_name,const char* c_item_name);
//设置指定模块指定项配置值，输入参数区分大小写
extern "C"  __declspec( dllexport ) int cfgmngr_set_cfgitemvalue(const char* c_model_name,const char* c_item_name);

//根据系统编号查找详细信息
extern "C"  __declspec( dllexport ) APP_CHANNEL_RELATION* cfgmngr_get_app_4y_cfginfo(int nsysid);

//根据ied的name获取对象
extern "C"  __declspec( dllexport ) TS_IED*   cfgmngr_get_scdied_byname(char* cName);
//获取IED队列
extern "C"  __declspec( dllexport ) TS_IED**  cfgmngr_get_scdiedlist(int& nlistsize);
//创建IED队列
extern "C"  __declspec( dllexport ) int      cfgmngr_BuildScdIedExRefRelation(TS_IED* ptsIed);
//获取通道队列
extern "C"  __declspec( dllexport ) CFG_REC_CHANNEL*  cfgmngr_get_cfgrecchannel_byscdref(char* cscdref);

#else 

//加载配置文件
extern "C"  __declspec( dllimport ) int cfgmngr_cfgmngr_initialize(const char* ccfgfilefullpath);
//释放
extern "C"  __declspec( dllimport ) int cfgmngr_cfgmngr_uninitialize(void);
//获取模型文件指针
extern "C"  __declspec( dllimport ) SCANNER* cfgmngr_get_sysmodel(void);
//根据采样端口系统编号获取对应的采样装置指针 
extern "C"  __declspec( dllimport ) SNIFFER* cfgmngr_get_sniffer(int nsys_portseq);
//根据采样端口系统编号获取对应的采样板卡指针
extern "C"  __declspec( dllimport ) SNIFFER_CARD* cfgmngr_get_sniffercard(int nsys_portseq);
//根据采样端口系统编号获取对应的采样端口指针
extern "C"  __declspec( dllimport ) SNIFFER_PORT*  cfgmngr_get_snifferport(int nsys_portseq);
//获取采集端口队列
extern "C"  __declspec( dllimport ) SNIFFER_PORT** cfgmnfr_get_snifferportlist(int& nlistsize); 
//根据ip地址或者mac地址查询到对应的ied
extern "C"  __declspec( dllimport ) SNIFFER_APP*   cfgmngr_get_ied_bynetidaddr(char* cNetaddr);
//根据系统编号查找详细信息
extern "C"  __declspec( dllimport ) SNIFFER_APP*   cfgmngr_get_ied_bysysid(int nsysid);
//根据appid查询到对应的ied装置指针
extern "C"  __declspec( dllimport ) SNIFFER_APP*   cfgmngr_get_ied_byappid(int nappid);
//获取指定模块指定项配置值，输入参数区分大小写，要求模块名称为大写，配置项名称小写
extern "C"  __declspec( dllimport ) CFG_ITEM* cfgmngr_get_cfgitemvalue(const char* c_model_name,const char* c_item_name);
//获取指定装置指定项配置值，输出参数区分大小写
extern "C"  __declspec( dllimport )CFG_ITEM* cfgmngr_get_sniffer_cfgitemvalue(int nsnifferid, const char* c_model_name,const char* c_item_name);
//设置指定模块指定项配置值，输入参数区分大小写，要求模块名称为大写，配置项名称小写
extern "C"  __declspec( dllimport ) int cfgmngr_set_cfgitemvalue(const char* c_model_name,const char* c_item_name);
//根据系统编号查找详细信息
extern "C"  __declspec( dllimport ) APP_CHANNEL_RELATION* cfgmngr_get_app_4y_cfginfo(int nsysid);

//根据ied的name获取对象
extern "C"  __declspec( dllimport ) TS_IED*   cfgmngr_get_scdied_byname(char* cName);
//获取IED队列
extern "C"  __declspec( dllimport ) TS_IED**  cfgmngr_get_scdiedlist(int& nlistsize);
//创建IED队列
extern "C"  __declspec( dllimport ) int      cfgmngr_BuildScdIedExRefRelation(TS_IED* ptsIed);
//获取通道队列
extern "C"  __declspec( dllimport ) CFG_REC_CHANNEL*  cfgmngr_get_cfgrecchannel_byscdref(char* cscdref);

#endif 
#endif //OS_WINDOWS

#ifdef OS_LINUX

#ifdef __cplusplus
extern "C"
{
#endif
	//加载配置文件
	int cfgmngr_initialize(const char* ccfgfilefullpath);
	//释放
	int cfgmngr_uninitialize(void);
	//获取模型文件指针
	SCANNER* cfgmngr_get_sysmodel(void);
	//根据采样端口系统编号获取对应的采样装置指针 
	SNIFFER* cfgmngr_get_sniffer(int nsys_portseq);
	//根据采样端口系统编号获取对应的采样板卡指针
	SNIFFER_CARD* cfgmngr_get_sniffercard(int nsys_portseq);
	//根据采样端口系统编号获取对应的采样端口指针
	SNIFFER_PORT* cfgmngr_get_snifferport(int nsys_portseq);
	//获取采集端口队列
	SNIFFER_PORT** cfgmnfr_get_snifferportlist(int& nlistsize);
	//根据ip地址或者mac地址查询到对应的ied
	SNIFFER_APP*   cfgmngr_get_ied_bynetidaddr(char* cNetaddr);
	//根据appid查询到对应的ied装置指针
	SNIFFER_APP*   cfgmngr_get_ied_byappid(int nappid);
	//根据系统编号查找详细信息
	SNIFFER_APP*   cfgmngr_get_ied_bysysid(int nsysid);
	//获取指定模块指定项配置值，输入参数区分大小写
	CFG_ITEM* cfgmngr_get_cfgitemvalue(const char* c_model_name,const char* c_item_name);
	//获取指定装置指定项配置值，输出参数区分大小写
	CFG_ITEM* cfgmngr_get_sniffer_cfgitemvalue(int nsnifferid, const char* c_model_name,const char* c_item_name);
	//设置指定模块指定项配置值，输入参数区分大小写
	int cfgmngr_set_cfgitemvalue(const char* c_model_name,const char* c_item_name);
	//根据系统编号查找详细信息
	APP_CHANNEL_RELATION* cfgmngr_get_app_4y_cfginfo(int nsysid);
	//根据ied的name获取对象
	TS_IED*   cfgmngr_get_scdied_byname(char* cName);
	//获取IED队列
	TS_IED**  cfgmngr_get_scdiedlist(int& nlistsize);
	//创建IED队列
	int      cfgmngr_BuildScdIedExRefRelation(TS_IED* ptsIed);
	//获取通道队列
	CFG_REC_CHANNEL*  cfgmngr_get_cfgrecchannel_byscdref(char* cscdref);

#ifdef __cplusplus
}
#endif


#endif //OS_LINUX