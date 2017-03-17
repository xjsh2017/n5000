#pragma execution_character_set("UTF-8")
#include "Msg2SmvPdu.h"
//#include "AnalyzeMMSData.h"

#include <cmath>
//#include <iostream.h>
#include<list>
using namespace std;

template <typename Integer>
Integer Reverse(Integer value)
{
	Integer result = value;
    volatile char *p1 = reinterpret_cast<char*>(&result);
    volatile char *p2 = p1 + sizeof(Integer) - 1;
    for (; p1 < p2; ++p1, --p2) {
        swap(*p1, *p2);
    }

    return result;
}

int CreateAsduInfo(SMV_INFO_STRUCT *p_info_struct)
{
	if( (NULL==p_info_struct) || (p_info_struct->n_asdu_num<=0) )
		return -1;

	p_info_struct->p_asdu_info_struct = new ASDU_INFO_STRUCT[p_info_struct->n_asdu_num];

	return 0;
}
int GetValueOffset(TLV_STRUCT *p_tlv)
{
	if(NULL==p_tlv)
		return -1;

	return p_tlv->n_lenlen +1;
}
//ASDU��������
//���ر�����������ռ�ֽ���
int MSIS_ASDU_Sum(const char *c_msg_buf,SMV_INFO_STRUCT *p_info_struct)
{
	if (NULL==c_msg_buf || NULL==p_info_struct)
	{
		return -1;
	}

	int n_tag=0;
	int n_offset=0;
	TLV_STRUCT tlv;

	//PDU�׽ṹ tagΪ60��L����Ϊһ���ֽڻ������ֽ�
	memset(&tlv,0,sizeof(tlv));

	ParseASN1_TLV(c_msg_buf,&tlv);
	n_offset=GetValueOffset(&tlv);
	
	//����ASDU����
	memset(&tlv,0,sizeof(tlv));
	n_offset+=ParseASN1_TLV(c_msg_buf+n_offset,&tlv);
	if(tlv.n_len==1)
		memcpy((char *)&p_info_struct->n_asdu_num,tlv.c_value,1);
	else if (tlv.n_len==2)
	{
		int ntemp=0;
		memcpy((char *)&ntemp,tlv.c_value,1);
		p_info_struct->n_asdu_num+=256*ntemp;
		ntemp = 0;
		memcpy((char *)&ntemp,tlv.c_value+1,1);
		p_info_struct->n_asdu_num+=ntemp;
	}
	else//�淶�涨ASDU����Ϊ1...65535���������2�ֽڡ��˴�����Ч�ʵ�
	{
		int ntemp=0;
		for (int i=0; i<tlv.n_len-1; i++)
		{
			memcpy((char *)&ntemp,&tlv.c_value[i],1);
			p_info_struct->n_asdu_num += pow(256.0,double(tlv.n_len-1-i))*ntemp;
		}
		ntemp = 0;
		memcpy((char *)&ntemp,&tlv.c_value[tlv.n_len-1],1);
		p_info_struct->n_asdu_num += ntemp;
	}
	
	//����sequence of 
	ParseASN1_TLV(c_msg_buf+n_offset,&tlv);
	n_offset+=GetValueOffset(&tlv);
	return n_offset;
}

//��̫������PDU�ײ���Ϣ
//���ر�����������ռ�ֽ���
int MSIS_Eth_Pdu(const char *c_msg_buf,SMV_INFO_STRUCT *p_info_struct)
{
	if (NULL==c_msg_buf || NULL==p_info_struct)
	{
		return -1;
	}
	int ntemp=0;
	//APPID
//	memcpy(temp,c_msg_buf,2);//test code
	memcpy((char *)&p_info_struct->n_app_id,c_msg_buf,2);
	//
	//��λ
	p_info_struct->n_app_id  = 0;
	memcpy((char *)&ntemp,c_msg_buf,1);
	p_info_struct->n_app_id+=256*ntemp;
	//��λ
	c_msg_buf+=1;
	memcpy((char *)&ntemp,c_msg_buf,1);
	p_info_struct->n_app_id+=ntemp;	

	c_msg_buf+=1;
	//��λ
	memcpy((char *)&ntemp,c_msg_buf,1);
	p_info_struct->n_msg_len+=256*ntemp;
	//��λ
	c_msg_buf+=1;
	memcpy((char *)&ntemp,c_msg_buf,1);
	p_info_struct->n_msg_len+=ntemp;	

	//��4���ֽ�Ԥ��


	return 8;
}

int ParseASDU_Total_Len(const char *c_msg_buf,int &n_tag,int &n_len)
{
	if (NULL==c_msg_buf)
	{
		return -1;
	}
	
	return -1;
}

int MSIS_ASDU(const char *c_msg_buf,ASDU_INFO_STRUCT *p_asdu,bool b_parseData)
{
	if (NULL==c_msg_buf || NULL==p_asdu)
	{
		return -1;
	}

	int n_tag=0;
	int n_offset=0;
	TLV_STRUCT tlv;

	//�ж�ASDU�׽ṹ tagΪ30��L����Ϊһ���ֽڻ������ֽ�
	ParseASN1_TLV(c_msg_buf, &tlv);
	n_offset = GetValueOffset(&tlv);
	p_asdu->n_asdu_len = tlv.n_len;

	//����֡ASDU��������
	n_offset += ParseASDU_Atr(c_msg_buf+n_offset, p_asdu);

	//���dataset�ܳ���
	ParseASN1_TLV(c_msg_buf+n_offset, &tlv);
	int n_dataset_len = tlv.n_len;
	n_offset += GetValueOffset(&tlv);

	if (b_parseData)
	{
		//�������ݼ�������
		n_offset += ParseASDU_DataSet(c_msg_buf+n_offset, p_asdu, n_dataset_len);
	}
	else
	{
		n_offset += n_dataset_len;
	}

	return n_offset;
}

int ConvertValue_Int(TLV_STRUCT &tlv)
{
	int nRet=0;
	if(tlv.n_len==1)
		memcpy((char *)&nRet,tlv.c_value,1);
	else
	{
		int ntemp = 0;
		for (int i=0; i<tlv.n_len-1; i++)
		{
			ntemp = 0;
			memcpy((char *)&ntemp, tlv.c_value+i, 1);
			nRet += pow(256.0,double(tlv.n_len-i-1))*ntemp;
		}
		memcpy((char *)&ntemp, tlv.c_value+tlv.n_len-1, 1);
		nRet += ntemp;
	}
	return nRet;
}
unsigned int ConvertValue_uInt(TLV_STRUCT &tlv)
{
	unsigned int nRet=0;
	int ntemp=0;
	char *p_tmp = NULL;

	p_tmp = tlv.c_value;

	if(tlv.n_len==1)//һ�ֽ�
		memcpy((char *)&nRet,tlv.c_value,1);
	else if (tlv.n_len==2)//���ֽ�
	{
		nRet = (unsigned char)p_tmp[1] + ( ((unsigned char)p_tmp[0]) << 8 );
	}
	else if (tlv.n_len==3)//���ֽ�
	{
		nRet = (unsigned char)p_tmp[2] + (((unsigned char)p_tmp[1]) << 8) + (((unsigned char)p_tmp[0]) << 16);
	}
	else if (tlv.n_len==4)//���ֽ�
	{
		nRet = (unsigned char)p_tmp[3] + (((unsigned char)p_tmp[2]) << 8) + (((unsigned char)p_tmp[1]) << 16) + (((unsigned char)p_tmp[0]) << 24);
	}
	else//�������ֽ�
	{
		int ntemp = 0;
		for (int i=0; i<tlv.n_len-1; i++)
		{
			ntemp = 0;
			memcpy((char *)&ntemp, tlv.c_value+i, 1);
			nRet += pow(256.0,double(tlv.n_len-i-1))*ntemp;
		}
		ntemp = 0;
		memcpy((char *)&ntemp, tlv.c_value+tlv.n_len-1, 1);
		nRet += ntemp;
	}
	return nRet;
}

int ParseASDU_Atr(const char *c_msg_buf,ASDU_INFO_STRUCT *p_asdu)
{
	if (NULL==c_msg_buf || NULL==p_asdu)
	{
		return -1;
	}

	int n_offset=0;
	TLV_STRUCT tlv;
	int ntemp=0;
	while(1)	//�����ݼ���Ա
	{
		memset(&tlv,0,sizeof(tlv));
		//����TLV
		ntemp=ParseASN1_TLV(c_msg_buf+n_offset,&tlv);
		if(tlv.n_tag==0x87)
			break;
		n_offset+=ntemp;
		switch(tlv.n_tag)
		{
		case 0x80:			//svID
			memcpy(p_asdu->c_svID,tlv.c_value,tlv.n_len);
			break;	
		case 0x81:			//dataset
			memcpy(p_asdu->c_dataset,tlv.c_value,tlv.n_len);
			break;
		case 0x82:			//smpCnt	�淶��ΪUint16
			p_asdu->n_smpCnt=ConvertValue_uInt(tlv);
			break;
		case 0x83:			//confRev		//д��Ϊ1		�淶��ΪUint32
//			p_asdu->n_confRev=1;
			p_asdu->n_confRev=ConvertValue_uInt(tlv);//modified on 20120701 by ljm����ʱ����Ϊunsigned int���淶��unsigned int
			break;
		case 0x84:			//refrTm
			memcpy(p_asdu->c_refrTm,tlv.c_value,tlv.n_len);
			break;
		case 0x85:			//smpSynch
			p_asdu->b_smpSynch=(int)ConvertValue_uInt(tlv);
			break;
		case 0x86:			//smpRate	�淶��ΪUint16
			p_asdu->n_smpRate=ConvertValue_uInt(tlv);
			break;
		default:			//Ĭ�����
			break;
		}
	}
	
	return n_offset;
}
int CreateSMVDataStruct(ASDU_INFO_STRUCT *p_asdu,int n_data_num)
{
	if( (NULL==p_asdu) || (n_data_num<=0) )
		return -1;
	
	p_asdu->p_smv_data_struct = new SMV_DATA_STRUCT[n_data_num];
	p_asdu->n_data_num  = n_data_num;                           //�������ݸ���
	return 0;
}

int Hex2Binary(char *cHex)
{
	int n_strlen = 0;
	char cTemp[MAX_MMS_INFO_DATA_LEN];
	long l_Temp;

	n_strlen = strlen(cHex);
	if (NULL == cHex)
	{
		return -1;
	}
	memset(cTemp,0,MAX_MMS_INFO_DATA_LEN);

	l_Temp=strtoul(cHex,NULL,16);
//	itoa(l_Temp, cHex, 2); 
	my_itoa(l_Temp, cHex, 2); 

	//ǰ�油��0
	while (strlen(cHex) < 4*n_strlen)
	{
		strcpy(cTemp, "0");
		strcat(cTemp, cHex);
		strcpy(cHex, cTemp);
	}
	return 0;
}

int ParseASDU_DataSet(const char *c_msg_buf,ASDU_INFO_STRUCT *p_asdu,int n_dataset_len)
{
	if (NULL==c_msg_buf || NULL==p_asdu)
	{
		return -1;
	}
//	const char *p_quality = NULL;
//	char c_data[5];
	int n_data_num=n_dataset_len/8;
	CreateSMVDataStruct(p_asdu,n_data_num);
	p_asdu->n_danum_actual = n_data_num;

	int n_temp=0;
	const char *p_buf_emp;
	for(int i=0; i<n_data_num; i++)
	{
		//Ч��̫�ͣ�2012-08-03ɾ��
//		memcpy((char *)&n_temp,c_msg_buf+8*i,4);
//		(p_asdu->p_smv_data_struct[i]).n_value=Reverse<int>(n_temp);

		//ֵ
		(p_asdu->p_smv_data_struct[i]).n_value = Buf2Int32(c_msg_buf+8*i);

		//����Ʒ��
		p_buf_emp = c_msg_buf+8*i+4;
		memcpy( (p_asdu->p_smv_data_struct[i]).c_quality, p_buf_emp, 4);//modified on 2012/8/9 Ϊ��������߷������ܣ�ͳһ����
// 		sprintf((p_asdu->p_smv_data_struct[i]).c_quality,"%02x%02x%02x%02x",(unsigned char)p_buf_emp[0],(unsigned char)p_buf_emp[1],
// 			(unsigned char)p_buf_emp[2],(unsigned char)p_buf_emp[3]);

//		strcpy(c_data,(p_asdu->p_smv_data_struct[i]).c_quality);
// 		Hex2Binary(c_data);//��������16����ת��Ϊ2����-�˴�Ч�ʼ�����£����齫�˺����رգ�Ʒ�ʵ�ת����Ӧ��ʱʹ��
 
// 		p_quality = c_data;
// 		p_quality = p_quality+strlen(c_data)-14;//�������14λ
 		//Ʒ������quality
// 		strcpy((p_asdu->p_smv_data_struct[i]).c_quality,p_quality);//delete by yzh 20120509
	}
	return n_dataset_len;
}

int my_itoa(int val, char* buf, int radix)
{

//   const int radix = 10;

    char* p;
    int a;        //every digit
    int len;
    char* b;    //start of the digit char
    char temp;

    p = buf;

    if (val < 0)
    {
        *p++ = '-';
        val = 0 - val;
    }

    b = p;

    do
    {
        a = val % radix;
        val /= radix;

        *p++ = a + '0';

    } while (val > 0);

    len = (int)(p - buf);

    *p-- = 0;

    //swap
    do
    {
        temp = *p;
        *p = *b;
        *b = temp;
        --p;
        ++b;

    } while (b < p);

    return len;
}

int Buf2Int32(const char* c_msg_buf)
{
	int ndestval = 0;
	unsigned int ntmpval = 0;
	unsigned char c1,c2,c3,c4;
//	unsigned int n_1=0,n_2=0,n_3=0,n_4=0;

	int nt = c_msg_buf[0] & 0X80;
	if ( nt == 0X80)//����
	{
		c1 = ~c_msg_buf[0];
		c2 = ~c_msg_buf[1];
		c3 = ~c_msg_buf[2];
		c4 = (~c_msg_buf[3]) + 1;
		
		ntmpval = c4 + (c3<<8) + (c2<<16) + (c1<<24);
		ndestval = -ntmpval;
	}
	else//����
	{
// 		n_1 = ((unsigned char)c_msg_buf[2]) << 8;
// 		n_2 = ((unsigned char)c_msg_buf[1]) << 16;
// 		n_3 = ((unsigned char)c_msg_buf[0]) << 24;
// 		ntest = (unsigned char)c_msg_buf[3] + n_1 + n_2 + n_3;
		ndestval = (unsigned char)c_msg_buf[3] + (((unsigned char)c_msg_buf[2]) << 8) + (((unsigned char)c_msg_buf[1]) << 16) + (((unsigned char)c_msg_buf[0]) << 24);
	}

	return ndestval;
}

