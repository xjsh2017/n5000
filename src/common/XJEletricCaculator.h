#pragma once
const double PI = 3.14159265358979323846l;
// CDlgPageXiebo dialog
//�ṹ���� г��������
typedef struct XIEBO_RESULT
{
	int nxiebo_time;		//г������
	float fxiebo_result[4];	//г��������
	float fxiebo_ratio;		//г������ٷֱ�
	//     fxiebo_result[0]: ʵ��
	//     fxiebo_result[1]���鲿
	//     fxiebo_result[2]����Чֵ
	//     fxiebo_result[3]�����
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
//�ṹ����  ����ͨ����Ӧ��г����
typedef struct XIEBO_CHANNEL
{
	int 	n_channel_seqid;
	int     n_xiebotimes;
	float   f_fuzhi;          //ͨ����ֵ
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
	//��ֵ����Чֵ����
	void Cal_fuzhi(int* pValueList_In,float* pResult_Out,int nsampleCount);
	//n�� n��г������-ͨ�ü��㷽����û���ÿ��ټ��㹫ʽ
	void Cal_n_xiebo_n(int* pValueList_In,float* pResult_Out,int nsampleCount,int nxiebo);
	//16��������㡢��Ǽ���
	void Cal_1xiebo_16(int* pValueList_In,float* pResult_Out);
	//16��3��г������
	void Cal_3xiebo_16(int* pValueList_In,float* pResult_Out);
	//16��5��г������
	void Cal_5xiebo_16(int* pValueList_In,float* pResult_Out);
	//16��7��г������
	void Cal_7xiebo_16(int* pValueList_In,float* pResult_Out);
	//80��������㡢��Ǽ���
	void Cal_1xiebo_80(int* pValueList_In,float* pResult_Out);
	//80��3��г������
	void Cal_3xiebo_80(int* pValueList_In,float* pResult_Out);
	//80��5��г������
	void Cal_5xiebo_80(int* pValueList_In,float* pResult_Out);
	//80��7��г������
	void Cal_7xiebo_80(int* pValueList_In,float* pResult_Out);
	//ֱ����������
	float Cal_n_zhiliu(int* pValueList_In,int nsampleCount);
	//�����������
	void Cal_E1(float* pValueList_In,float* pResult_Out);
	//�����������
	void Cal_E2(float* pValueList_In,float* pResult_Out);
	//�����������
	void Cal_3E0(float* pValueList_In,float* pResult_Out);
};

