#ifndef PARTITION_INFO_I_H_INCLUDE
#define PARTITION_INFO_I_H_INCLUDE

#ifdef OS_WINDOWS

#ifdef PART_INFO_DLL_FILE 
class _declspec(dllexport) CPART_INFO_I //导出类 

#else 
class _declspec(dllimport) CPART_INFO_I //导入类 

#endif//PART_INFO_DLL_FILE

#endif//OS_WINDOWS

#ifdef OS_LINUX
class CPART_INFO_I

#endif //OS_LINUX

{
protected:
	CPART_INFO_I(){};
			
public:
	//启动状态机线程，2分钟扫描磁盘一次
	virtual int  init_partition_info()=0;
	//NULL || serial文件记录的内容对应的当前分组盘符或路径
	virtual const char * get_cur_partition(int ngroup, bool bDev=false)=0;
	//-1 or X百分数(分区百分比文件读取)
	virtual int get_partition_usg_ex(const char *cPartition)=0;
	//停止状态机
	virtual int  release_partition_info()=0;
	//配置文件读取的状态启动条件
	virtual int  get_max_partition_usage()=0;
	//分区和路径映射
	virtual const char * get_partition_by_dev(const char *cDev)=0;
	virtual const char * get_dev_by_partition(const char *cPartition)=0;
	//分区对应的序号--diskclean用到
	virtual int get_serial(int nGroup, const char *cPartition)=0;	
	//NULL or 文件中分区号+1 对应的路径或者分区 
	virtual const char *get_partition_next_by_file(int ngroup,bool bDev)=0;
	//配置的分组的分区数
	virtual int get_partition_count(int ngroup)=0; 
	//计算路径对应分区使用百分比情况到文件，输出参数表示分区是否满了
	virtual int statfs_partition(int ngroup, int nSerial, bool &bFull)=0;
	//NULL or 序号成员当前值对应的 盘符或路径
	virtual const char * get_cur_partition_of_serialmember(int ngroup, bool bDev=false)=0;//获取当前盘符或路径（与记录serial的成员变量对应的）
};

#ifdef OS_WINDOWS

#ifdef PART_INFO_DLL_FILE 

//导出函数

extern "C"  __declspec( dllexport ) CPART_INFO_I * createPartInfo_I(char *cLogPath);
extern "C"  __declspec( dllexport ) void destroyPartInfo_I(CPART_INFO_I* theInterface);

#else 

//导入函数

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