#ifndef PARTITION_INFO_I_H_INCLUDE
#define PARTITION_INFO_I_H_INCLUDE

#ifdef OS_WINDOWS

#ifdef PART_INFO_DLL_FILE 
class _declspec(dllexport) CPART_INFO_I //������ 

#else 
class _declspec(dllimport) CPART_INFO_I //������ 

#endif//PART_INFO_DLL_FILE

#endif//OS_WINDOWS

#ifdef OS_LINUX
class CPART_INFO_I

#endif //OS_LINUX

{
protected:
	CPART_INFO_I(){};
			
public:
	//����״̬���̣߳�2����ɨ�����һ��
	virtual int  init_partition_info()=0;
	//NULL || serial�ļ���¼�����ݶ�Ӧ�ĵ�ǰ�����̷���·��
	virtual const char * get_cur_partition(int ngroup, bool bDev=false)=0;
	//-1 or X�ٷ���(�����ٷֱ��ļ���ȡ)
	virtual int get_partition_usg_ex(const char *cPartition)=0;
	//ֹͣ״̬��
	virtual int  release_partition_info()=0;
	//�����ļ���ȡ��״̬��������
	virtual int  get_max_partition_usage()=0;
	//������·��ӳ��
	virtual const char * get_partition_by_dev(const char *cDev)=0;
	virtual const char * get_dev_by_partition(const char *cPartition)=0;
	//������Ӧ�����--diskclean�õ�
	virtual int get_serial(int nGroup, const char *cPartition)=0;	
	//NULL or �ļ��з�����+1 ��Ӧ��·�����߷��� 
	virtual const char *get_partition_next_by_file(int ngroup,bool bDev)=0;
	//���õķ���ķ�����
	virtual int get_partition_count(int ngroup)=0; 
	//����·����Ӧ����ʹ�ðٷֱ�������ļ������������ʾ�����Ƿ�����
	virtual int statfs_partition(int ngroup, int nSerial, bool &bFull)=0;
	//NULL or ��ų�Ա��ǰֵ��Ӧ�� �̷���·��
	virtual const char * get_cur_partition_of_serialmember(int ngroup, bool bDev=false)=0;//��ȡ��ǰ�̷���·�������¼serial�ĳ�Ա������Ӧ�ģ�
};

#ifdef OS_WINDOWS

#ifdef PART_INFO_DLL_FILE 

//��������

extern "C"  __declspec( dllexport ) CPART_INFO_I * createPartInfo_I(char *cLogPath);
extern "C"  __declspec( dllexport ) void destroyPartInfo_I(CPART_INFO_I* theInterface);

#else 

//���뺯��

extern "C"  __declspec( dllimport ) CPART_INFO_I * createPartInfo_I(char *cLogPath);
extern "C"  __declspec( dllimport ) void destroyPartInfo_I(CPART_INFO_I* theInterface);

#endif //PART_INFO_DLL_FILE
#endif //OS_WINDOWS

#ifdef OS_LINUX
#ifdef __cplusplus
extern "C"
{
#endif
	
	CPART_INFO_I * createPartInfo_I(char *cLogPath);
	void destroyPartInfo_I(CPART_INFO_I* theInterface);
	
#ifdef __cplusplus
}
#endif

#endif //OS_LINUX



#endif /*PARTITION_INFO_I_H_INCLUDE*/