
#include "SYS_MODEL_STRUCT.h"       //ϵͳģ��ͷ�ļ�
#include "WAVE_RECORD_STRUCT.h"     //¼����ģ��ͷ�ļ�
#include "SYS_CONFIGMODEL_STRUCT.h" //������ģ��ͷ�ļ�
#ifdef OS_WINDOWS

#ifdef CFG_DLL_MNGR 
//���������ļ�
extern "C"  __declspec( dllexport ) int cfgmngr_initialize(const char* ccfgfilefullpath);
//�ͷ�
extern "C"  __declspec( dllexport ) int cfgmngr_uninitialize(void);
//��ȡģ���ļ�ָ��
extern "C"  __declspec( dllexport ) SCANNER* cfgmngr_get_sysmodel(void);
//���ݲ����˿�ϵͳ��Ż�ȡ��Ӧ�Ĳ���װ��ָ�� 
extern "C"  __declspec( dllexport ) SNIFFER* cfgmngr_get_sniffer(int nsys_portseq);

//���ݲ����˿�ϵͳ��Ż�ȡ��Ӧ�Ĳ����忨ָ��
extern "C"  __declspec( dllexport ) SNIFFER_CARD* cfgmngr_get_sniffercard(int nsys_portseq);
//���ݲ����˿�ϵͳ��Ż�ȡ��Ӧ�Ĳ����˿�ָ��
extern "C"  __declspec( dllexport ) SNIFFER_PORT* cfgmngr_get_snifferport(int nsys_portseq);
//��ȡ�ɼ��˿ڶ���
extern "C"  __declspec( dllexport ) SNIFFER_PORT** cfgmnfr_get_snifferportlist(int& nlistsize); 
//����ip��ַ����mac��ַ��ѯ����Ӧ��ied
extern "C"  __declspec( dllexport ) SNIFFER_APP*   cfgmngr_get_ied_bynetidaddr(char* cNetaddr);
//����appid��ѯ����Ӧ��iedװ��ָ��
extern "C"  __declspec( dllexport ) SNIFFER_APP*   cfgmngr_get_ied_byappid(int nappid);
//����ϵͳ��Ų�����ϸ��Ϣ
extern "C"  __declspec( dllexport ) SNIFFER_APP*   cfgmngr_get_ied_bysysid(int nsysid);
//��ȡָ��ģ��ָ��������ֵ������������ִ�Сд
extern "C"  __declspec( dllexport ) CFG_ITEM* cfgmngr_get_cfgitemvalue(const char* c_model_name,const char* c_item_name);
//��ȡָ��װ��ָ��������ֵ������������ִ�Сд
extern "C"  __declspec( dllexport )CFG_ITEM* cfgmngr_get_sniffer_cfgitemvalue(int nsnifferid, const char* c_model_name,const char* c_item_name);
//����ָ��ģ��ָ��������ֵ������������ִ�Сд
extern "C"  __declspec( dllexport ) int cfgmngr_set_cfgitemvalue(const char* c_model_name,const char* c_item_name);

//����ϵͳ��Ų�����ϸ��Ϣ
extern "C"  __declspec( dllexport ) APP_CHANNEL_RELATION* cfgmngr_get_app_4y_cfginfo(int nsysid);

//����ied��name��ȡ����
extern "C"  __declspec( dllexport ) TS_IED*   cfgmngr_get_scdied_byname(char* cName);
//��ȡIED����
extern "C"  __declspec( dllexport ) TS_IED**  cfgmngr_get_scdiedlist(int& nlistsize);
//����IED����
extern "C"  __declspec( dllexport ) int      cfgmngr_BuildScdIedExRefRelation(TS_IED* ptsIed);
//��ȡͨ������
extern "C"  __declspec( dllexport ) CFG_REC_CHANNEL*  cfgmngr_get_cfgrecchannel_byscdref(char* cscdref);

#else 

//���������ļ�
extern "C"  __declspec( dllimport ) int cfgmngr_cfgmngr_initialize(const char* ccfgfilefullpath);
//�ͷ�
extern "C"  __declspec( dllimport ) int cfgmngr_cfgmngr_uninitialize(void);
//��ȡģ���ļ�ָ��
extern "C"  __declspec( dllimport ) SCANNER* cfgmngr_get_sysmodel(void);
//���ݲ����˿�ϵͳ��Ż�ȡ��Ӧ�Ĳ���װ��ָ�� 
extern "C"  __declspec( dllimport ) SNIFFER* cfgmngr_get_sniffer(int nsys_portseq);
//���ݲ����˿�ϵͳ��Ż�ȡ��Ӧ�Ĳ����忨ָ��
extern "C"  __declspec( dllimport ) SNIFFER_CARD* cfgmngr_get_sniffercard(int nsys_portseq);
//���ݲ����˿�ϵͳ��Ż�ȡ��Ӧ�Ĳ����˿�ָ��
extern "C"  __declspec( dllimport ) SNIFFER_PORT*  cfgmngr_get_snifferport(int nsys_portseq);
//��ȡ�ɼ��˿ڶ���
extern "C"  __declspec( dllimport ) SNIFFER_PORT** cfgmnfr_get_snifferportlist(int& nlistsize); 
//����ip��ַ����mac��ַ��ѯ����Ӧ��ied
extern "C"  __declspec( dllimport ) SNIFFER_APP*   cfgmngr_get_ied_bynetidaddr(char* cNetaddr);
//����ϵͳ��Ų�����ϸ��Ϣ
extern "C"  __declspec( dllimport ) SNIFFER_APP*   cfgmngr_get_ied_bysysid(int nsysid);
//����appid��ѯ����Ӧ��iedװ��ָ��
extern "C"  __declspec( dllimport ) SNIFFER_APP*   cfgmngr_get_ied_byappid(int nappid);
//��ȡָ��ģ��ָ��������ֵ������������ִ�Сд��Ҫ��ģ������Ϊ��д������������Сд
extern "C"  __declspec( dllimport ) CFG_ITEM* cfgmngr_get_cfgitemvalue(const char* c_model_name,const char* c_item_name);
//��ȡָ��װ��ָ��������ֵ������������ִ�Сд
extern "C"  __declspec( dllimport )CFG_ITEM* cfgmngr_get_sniffer_cfgitemvalue(int nsnifferid, const char* c_model_name,const char* c_item_name);
//����ָ��ģ��ָ��������ֵ������������ִ�Сд��Ҫ��ģ������Ϊ��д������������Сд
extern "C"  __declspec( dllimport ) int cfgmngr_set_cfgitemvalue(const char* c_model_name,const char* c_item_name);
//����ϵͳ��Ų�����ϸ��Ϣ
extern "C"  __declspec( dllimport ) APP_CHANNEL_RELATION* cfgmngr_get_app_4y_cfginfo(int nsysid);

//����ied��name��ȡ����
extern "C"  __declspec( dllimport ) TS_IED*   cfgmngr_get_scdied_byname(char* cName);
//��ȡIED����
extern "C"  __declspec( dllimport ) TS_IED**  cfgmngr_get_scdiedlist(int& nlistsize);
//����IED����
extern "C"  __declspec( dllimport ) int      cfgmngr_BuildScdIedExRefRelation(TS_IED* ptsIed);
//��ȡͨ������
extern "C"  __declspec( dllimport ) CFG_REC_CHANNEL*  cfgmngr_get_cfgrecchannel_byscdref(char* cscdref);

#endif 
#endif //OS_WINDOWS

#ifdef OS_LINUX

#ifdef __cplusplus
extern "C"
{
#endif
	//���������ļ�
	int cfgmngr_initialize(const char* ccfgfilefullpath);
	//�ͷ�
	int cfgmngr_uninitialize(void);
	//��ȡģ���ļ�ָ��
	SCANNER* cfgmngr_get_sysmodel(void);
	//���ݲ����˿�ϵͳ��Ż�ȡ��Ӧ�Ĳ���װ��ָ�� 
	SNIFFER* cfgmngr_get_sniffer(int nsys_portseq);
	//���ݲ����˿�ϵͳ��Ż�ȡ��Ӧ�Ĳ����忨ָ��
	SNIFFER_CARD* cfgmngr_get_sniffercard(int nsys_portseq);
	//���ݲ����˿�ϵͳ��Ż�ȡ��Ӧ�Ĳ����˿�ָ��
	SNIFFER_PORT* cfgmngr_get_snifferport(int nsys_portseq);
	//��ȡ�ɼ��˿ڶ���
	SNIFFER_PORT** cfgmnfr_get_snifferportlist(int& nlistsize);
	//����ip��ַ����mac��ַ��ѯ����Ӧ��ied
	SNIFFER_APP*   cfgmngr_get_ied_bynetidaddr(char* cNetaddr);
	//����appid��ѯ����Ӧ��iedװ��ָ��
	SNIFFER_APP*   cfgmngr_get_ied_byappid(int nappid);
	//����ϵͳ��Ų�����ϸ��Ϣ
	SNIFFER_APP*   cfgmngr_get_ied_bysysid(int nsysid);
	//��ȡָ��ģ��ָ��������ֵ������������ִ�Сд
	CFG_ITEM* cfgmngr_get_cfgitemvalue(const char* c_model_name,const char* c_item_name);
	//��ȡָ��װ��ָ��������ֵ������������ִ�Сд
	CFG_ITEM* cfgmngr_get_sniffer_cfgitemvalue(int nsnifferid, const char* c_model_name,const char* c_item_name);
	//����ָ��ģ��ָ��������ֵ������������ִ�Сд
	int cfgmngr_set_cfgitemvalue(const char* c_model_name,const char* c_item_name);
	//����ϵͳ��Ų�����ϸ��Ϣ
	APP_CHANNEL_RELATION* cfgmngr_get_app_4y_cfginfo(int nsysid);
	//����ied��name��ȡ����
	TS_IED*   cfgmngr_get_scdied_byname(char* cName);
	//��ȡIED����
	TS_IED**  cfgmngr_get_scdiedlist(int& nlistsize);
	//����IED����
	int      cfgmngr_BuildScdIedExRefRelation(TS_IED* ptsIed);
	//��ȡͨ������
	CFG_REC_CHANNEL*  cfgmngr_get_cfgrecchannel_byscdref(char* cscdref);

#ifdef __cplusplus
}
#endif


#endif //OS_LINUX