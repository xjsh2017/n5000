#pragma once
const double PI = 3.14159265358979323846l;
// CDlgPageXiebo dialog
//结构定义 谐波计算结果
typedef struct XIEBO_RESULT
{
	int nxiebo_time;		//谐波次数
	float fxiebo_result[4];	//谐波计算结果
	float fxiebo_ratio;		//谐波计算百分比
	//     fxiebo_result[0]: 实部
	//     fxiebo_result[1]：虚部
	//     fxiebo_result[2]：有效值
	//     fxiebo_result[3]：相角
	XIEBO_RESULT()
	{
		nxiebo_time      = 1;
		fxiebo_ratio     = 0;
		fxiebo_result[0] = 0.0f;
		fxiebo_result[1] = 0.0f;
		fxiebo_result[2] = 0.0f;
		fxiebo_result[3] = 0.0f;
	}
}_XIEBO_RESULT;
//结构定义  单个通道对应的谐波数
typedef struct XIEBO_CHANNEL
{
	int 	n_channel_seqid;
	int     n_xiebotimes;
	float   f_fuzhi;          //通道幅值
	XIEBO_RESULT* p_xiebo_result;
	XIEBO_CHANNEL()
	{
		n_channel_seqid = 0;
		p_xiebo_result  = 0;
		n_xiebotimes    = 0;
		f_fuzhi         = 0.0f;
	}
}_XIEBO_CHANNEL;
class CXJEletricCaculator
{
public:
	CXJEletricCaculator(void);
	~CXJEletricCaculator(void);
public:
	//幅值、有效值计算
	void Cal_fuzhi(int* pValueList_In,float* pResult_Out,int nsampleCount);
	//n点 n次谐波计算-通用计算方法，没有用快速计算公式
	void Cal_n_xiebo_n(int* pValueList_In,float* pResult_Out,int nsampleCount,int nxiebo);
	//16点基波计算、相角计算
	void Cal_1xiebo_16(int* pValueList_In,float* pResult_Out);
	//16点3次谐波计算
	void Cal_3xiebo_16(int* pValueList_In,float* pResult_Out);
	//16点5次谐波计算
	void Cal_5xiebo_16(int* pValueList_In,float* pResult_Out);
	//16点7次谐波计算
	void Cal_7xiebo_16(int* pValueList_In,float* pResult_Out);
	//80点基波计算、相角计算
	void Cal_1xiebo_80(int* pValueList_In,float* pResult_Out);
	//80点3次谐波计算
	void Cal_3xiebo_80(int* pValueList_In,float* pResult_Out);
	//80点5次谐波计算
	void Cal_5xiebo_80(int* pValueList_In,float* pResult_Out);
	//80点7次谐波计算
	void Cal_7xiebo_80(int* pValueList_In,float* pResult_Out);
	//直流分量计算
	float Cal_n_zhiliu(int* pValueList_In,int nsampleCount);
	//正序分量计算
	void Cal_E1(float* pValueList_In,float* pResult_Out);
	//负序分量计算
	void Cal_E2(float* pValueList_In,float* pResult_Out);
	//零序分量计算
	void Cal_3E0(float* pValueList_In,float* pResult_Out);
};

