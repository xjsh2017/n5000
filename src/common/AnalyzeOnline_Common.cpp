#include "define_scan.h"
#include "AnalyzeOnline_Common.h"


/*
int Releas_message_errs(MESSAGE_ERROR_INFO_ARRAY_STRUCT* p_message_errs)
{
	int i=0;
	int j=0;
	int k=0;

	if (NULL==p_message_errs)
	{
		return RES_FAIL;
	}
	if (p_message_errs->p_msg_err)
	{
		//释放错误编码数组资源
		for (i=0; i<p_message_errs->n_msg_err; i++)
		{
			if (NULL!=p_message_errs->p_msg_err[i].p_errcode)
			{
				delete[] p_message_errs->p_msg_err[i].p_errcode;
				p_message_errs->p_msg_err[i].p_errcode = NULL;
			}

			if (NULL != p_message_errs->p_msg_err[i].p_asduerr)
			{
				//释放ASDU错误编码结构数组资源
				for (j=0; j<p_message_errs->p_msg_err[i].n_num_asduerr; j++)
				{
					if (NULL == p_message_errs->p_msg_err[i].p_asduerr[j].p_errcode)
					{
						continue;
					}
					for (k=0; k<p_message_errs->p_msg_err[i].p_asduerr[j].n_num_errcode; k++)
					{
						delete[] p_message_errs->p_msg_err[i].p_asduerr[j].p_errcode;
						p_message_errs->p_msg_err[i].p_asduerr[j].p_errcode = NULL;
					}
				}

				delete[] p_message_errs->p_msg_err[i].p_asduerr;
				p_message_errs->p_msg_err[i].p_asduerr = NULL;
			}
		}

		delete[] p_message_errs->p_msg_err;
		p_message_errs->p_msg_err = NULL;
	}

	delete p_message_errs;
	p_message_errs = NULL; 

	return RES_SUCCESS;
}*/

int release_da_err(MESSAGE_DATA_ERR_STRUCT* p_daerr, int n_num)
{
	int nRet = RES_SUCCESS;
	int i;
	
	if (NULL==p_daerr)
	{
		return RES_SUCCESS;
	}

	for (i=0; i<n_num; i++)
	{
		if (NULL!=p_daerr[i].p_errcode)
		{
			delete[] p_daerr[i].p_errcode;
			p_daerr[i].p_errcode = NULL;
		}
	}

	delete[] p_daerr;
	p_daerr = NULL;

	return nRet;
}

int release_asdu_err(MESSAGE_ASDU_ERROR_STRUCT *p_asduerr, int n_num)
{
	int nRet = RES_SUCCESS;
	int i;
	
	if (NULL==p_asduerr)
	{
		return RES_SUCCESS;
	}

	for (i=0; i<n_num; i++)
	{
		if (NULL!=p_asduerr[i].p_errcode)
		{
			delete[] p_asduerr[i].p_errcode;
			p_asduerr[i].p_errcode = NULL;
		}

		release_da_err(p_asduerr[i].p_daerr, p_asduerr[i].n_num_daerr);
	}

	delete[] p_asduerr;
	p_asduerr = NULL;
	
	return nRet;
}

int release_msg_err_info(MESSAGE_ERROR_INFO_STRUCT* p_msg_err, int n_num)
{
	int nRet = RES_SUCCESS;
	int i;
	if (NULL==p_msg_err)
	{
		return RES_SUCCESS;
	}

	for (i=0; i<n_num; i++)
	{
		if (NULL!=p_msg_err[i].p_errcode)
		{
			delete[] p_msg_err[i].p_errcode;
			p_msg_err[i].p_errcode = NULL;
		}

		release_asdu_err(p_msg_err[i].p_asduerr, p_msg_err[i].n_num_asduerr);
	}

	delete[] p_msg_err;
	p_msg_err = NULL;

	return nRet;
}

int Releas_message_errs(MESSAGE_ERROR_INFO_ARRAY_STRUCT* p_message_errs)
{
	int i=0;
	int j=0;
	int k=0;
	
	if (NULL==p_message_errs)
	{
		return RES_FAIL;
	}

	release_msg_err_info(p_message_errs->p_msg_err, p_message_errs->n_msg_err);

	delete p_message_errs;
	p_message_errs = NULL;
	
	return RES_SUCCESS;
}

// int *CreateAsduErr(vector<int> v_asduerr)
// {
// 	int nRet = RES_SUCCESS;
// 	int i=0;
// 	int *p_errcode = NULL;
// 
// 	if ( 0>=v_asduerr.size() )
// 	{
// 		return RES_FAIL;
// 	}
// 
// 	p_errcode = new int[v_asduerr.size()];
// 	for (i=0; i<v_asduerr.size(); i++)
// 	{
// 		p_errcode[i] = v_asduerr[i];
// 	}
// 
// 	return nRet;
// }
/*
int Msgerr2MsgerrInfo(MESSAGE_ERROR_STRUCT msgerr, MESSAGE_ERROR_INFO_STRUCT *p_msgerrInfo)
{
	int nRet = RES_SUCCESS;
	int i=0;
	map<int,vector<int> >::iterator Ite;
	map<int, ASDU_ERR_NUM_STRUCT>::iterator Ite_da;

	p_msgerrInfo->n_appid = msgerr.n_appid;
	p_msgerrInfo->n_error_class = msgerr.n_error_class;
	strcpy(p_msgerrInfo->c_avtm,msgerr.c_avtm);

	if (msgerr.v_err_numbers.size() > 0)
	{
		p_msgerrInfo->n_num_errcode = msgerr.v_err_numbers.size();
		p_msgerrInfo->p_errcode = new int[msgerr.v_err_numbers.size()];
		for (i=0; i<msgerr.v_err_numbers.size(); i++)
		{
			p_msgerrInfo->p_errcode[i] = msgerr.v_err_numbers[i];
		}
	}

	if (msgerr.map_asdu_2_errs.size() > 0)
	{
		p_msgerrInfo->n_num_asduerr = msgerr.map_asdu_2_errs.size();
		p_msgerrInfo->p_asduerr = new MESSAGE_ASDU_ERROR_STRUCT[msgerr.map_asdu_2_errs.size()];
		Ite = msgerr.map_asdu_2_errs.begin();
		i = 0;
		while (Ite != msgerr.map_asdu_2_errs.end())
		{
			p_msgerrInfo->p_asduerr[i].n_asdu_no = Ite->first;
			CreateAsduErr(Ite->second,&p_msgerrInfo->p_asduerr[i]);

			Ite_da = msgerr.map_asdu2daerr.find(Ite->first);
			if (Ite_da != msgerr.map_asdu2daerr.end())
			{
				p_msgerrInfo->p_asduerr[i].n_num_daerr = Ite_da->second.map_daseq_2_daerrs.size();
				p_msgerrInfo->p_asduerr[i].p_daerr = CreateDAerr(Ite_da->second);
			}
		
			Ite++;
			i++;
		}
	}
	

	return nRet;
}*/

int* create_errcode(vector<int> v_err_numbers)
{
	int *p_errcode = NULL;
	int i;

	if (v_err_numbers.size() <= 0)
	{
		return NULL;
	}

	p_errcode = new int[v_err_numbers.size()];
	for (i=0; i<v_err_numbers.size(); i++)
	{
		p_errcode[i] = v_err_numbers[i];
	}

	return p_errcode;
}

MESSAGE_ASDU_ERROR_STRUCT * create_gsda_err(map<int, vector<int> >map_daseq2errs)
{
	MESSAGE_ASDU_ERROR_STRUCT *p_asduerr = NULL;
	map<int,vector<int> >::iterator Ite;
	int i=0;

	if (map_daseq2errs.size()<=0)
	{
		return NULL;
	}

	p_asduerr = new MESSAGE_ASDU_ERROR_STRUCT[map_daseq2errs.size()];

	Ite = map_daseq2errs.begin();
	while (Ite != map_daseq2errs.end())
	{
		p_asduerr[i].n_asdu_no = Ite->first;
		p_asduerr[i].n_num_errcode = Ite->second.size();
		p_asduerr[i].p_errcode = create_errcode(Ite->second);

		Ite++;
		i++;
	}


	return p_asduerr;
}

MESSAGE_DATA_ERR_STRUCT* create_smv_da_err(map<int, vector<int> > map_daseq_2_daerrs)
{
	MESSAGE_DATA_ERR_STRUCT* p_daerr = NULL;
	int i=0;
	map<int, vector<int> >::iterator Ite;

	if (map_daseq_2_daerrs.size()<=0)
	{
		return NULL;
	}

	p_daerr = new MESSAGE_DATA_ERR_STRUCT[map_daseq_2_daerrs.size()];

	Ite = map_daseq_2_daerrs.begin();
	while (Ite != map_daseq_2_daerrs.end())
	{
		p_daerr[i].n_da_seq = Ite->first;
		p_daerr[i].n_num_errcode = Ite->second.size();
		p_daerr[i].p_errcode = create_errcode(Ite->second);

		Ite++;
		i++;
	}

	return p_daerr;
}

MESSAGE_ASDU_ERROR_STRUCT * create_asdu_err(map<int, ASDU_ERR_NUM_STRUCT> map_asdu_2_errs)
{
	MESSAGE_ASDU_ERROR_STRUCT *p_asduerr = NULL;
	map<int, ASDU_ERR_NUM_STRUCT>::iterator Ite;
	int i=0;

	if (map_asdu_2_errs.size()<=0)
	{
		return NULL;
	}
	p_asduerr = new MESSAGE_ASDU_ERROR_STRUCT[map_asdu_2_errs.size()];

	Ite = map_asdu_2_errs.begin();
	while (Ite != map_asdu_2_errs.end())
	{
		p_asduerr[i].n_asdu_no = Ite->first;
		p_asduerr[i].n_num_errcode = Ite->second.v_err_numbers.size();
		p_asduerr[i].p_errcode = create_errcode(Ite->second.v_err_numbers);

		p_asduerr[i].n_num_daerr = Ite->second.map_daseq_2_daerrs.size();
		p_asduerr[i].p_daerr = create_smv_da_err(Ite->second.map_daseq_2_daerrs);

		Ite++;
		i++;
	}

	return p_asduerr;
}

int Msgerr2MsgerrInfo(MESSAGE_ERROR_STRUCT msgerr, MESSAGE_ERROR_INFO_STRUCT *p_msgerrInfo)
{
	int nRet = RES_SUCCESS;
	int i=0;
	
	p_msgerrInfo->n_appid = msgerr.n_appid;
	p_msgerrInfo->n_error_class = msgerr.n_error_class;
	strcpy(p_msgerrInfo->c_avtm,msgerr.c_avtm);
	
	p_msgerrInfo->n_num_errcode = msgerr.v_err_numbers.size();
	p_msgerrInfo->p_errcode = create_errcode(msgerr.v_err_numbers);

	if (p_msgerrInfo->n_error_class==ERROR_CLASS_GOOSE)
	{
		p_msgerrInfo->n_num_asduerr = msgerr.map_gs_daseq_2_errs.size();
		p_msgerrInfo->p_asduerr = create_gsda_err(msgerr.map_gs_daseq_2_errs);
	}
	else if (p_msgerrInfo->n_error_class==ERROR_CLASS_SMV)
	{
		p_msgerrInfo->n_num_asduerr = msgerr.map_asdu_2_errs.size();
		p_msgerrInfo->p_asduerr = create_asdu_err(msgerr.map_asdu_2_errs);
	}
	
	return nRet;
}

MESSAGE_DATA_ERR_STRUCT* CreateDAerr(ASDU_ERR_NUM_STRUCT asdu_da_err)
{
	MESSAGE_DATA_ERR_STRUCT* p_daerr = NULL;
	int i=0;
	int j=0;
	map<int, vector<int> >::iterator Ite;

	int nSize = asdu_da_err.map_daseq_2_daerrs.size();
	if (nSize<=0)
	{
		return NULL;
	}

	p_daerr = new MESSAGE_DATA_ERR_STRUCT[nSize];
	Ite = asdu_da_err.map_daseq_2_daerrs.begin();
	while (Ite!=asdu_da_err.map_daseq_2_daerrs.end())
	{
		p_daerr[i].n_da_seq = Ite->first;

		p_daerr[i].n_num_errcode = Ite->second.size();
		if ( p_daerr[i].n_num_errcode> 0)
		{
			p_daerr[i].p_errcode = new int[Ite->second.size()];
			for (j=0; j<Ite->second.size(); j++)
			{
				p_daerr[i].p_errcode[j] = Ite->second[j];
			}
		}

		Ite++;
		i++;
	}
	
	return p_daerr;
}

MESSAGE_ERROR_INFO_ARRAY_STRUCT* CreateMsgErrArray(vector<MESSAGE_ERROR_STRUCT> message_err_lst)
{
	MESSAGE_ERROR_INFO_ARRAY_STRUCT* pMsgErrArry = NULL;
	int i=0;
	if (0 >= message_err_lst.size())
	{
		return NULL;
	}

	pMsgErrArry = new MESSAGE_ERROR_INFO_ARRAY_STRUCT;
	pMsgErrArry->n_msg_err = message_err_lst.size();

	pMsgErrArry->p_msg_err = new MESSAGE_ERROR_INFO_STRUCT[message_err_lst.size()];
	for (i=0; i<message_err_lst.size(); i++)
	{
		Msgerr2MsgerrInfo(message_err_lst[i],&pMsgErrArry->p_msg_err[i]);
	}

	return pMsgErrArry;
}

bool is_one_hex(char c_number)
{
	bool bRet = false;

	switch (c_number)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'a':
	case 'A':
	case 'b':
	case 'B':
	case 'c':
	case 'C':
	case 'd':
	case 'D':
	case 'e':
	case 'E':
	case 'f':
	case 'F':
		bRet = true;
		break;

	default:
		break;
	}

	return bRet;
}

bool is_tow_hex(char *p_number)
{
	if (NULL==p_number)
	{
		return false;
	}

	if (2!=strlen(p_number))
	{
		return false;
	}
	if (false==is_one_hex(p_number[0]) || false==is_one_hex(p_number[1]))
	{
		return true;
	}

	return false;
}

int str_cmp_i(const char*psrc, const char* pdest)
{
	int m = 0;
#ifdef OS_WINDOWS
	m = strcmpi(psrc, pdest);
#else
	m = strcasecmp(psrc, pdest);
#endif

	return m;
}

bool mac_error_smv(char *p_mac)
{
	char *p_tmp = NULL;
	if (NULL == p_mac)
	{
		return true;
	}

	unsigned int uTmp_4 = 0;
	unsigned short uTmp_2 = 0;

	//MAC地址前4字节
	memcpy(&uTmp_4, (unsigned char*)p_mac, 4);

	//MAC地址后两字节
	p_tmp = p_mac + 4;
	memcpy(&uTmp_2, (unsigned char*)p_tmp, 2);

	//广播地址
	if (0XFFFFFFFF == uTmp_4 && 0XFFFF == uTmp_2)
	{
		return false;
	}

	if (0X04CD0C01 != uTmp_4)//SMV组播地址 应该为01-0c-cd-04-xx-xx
	{
		return true;
	}

	if (uTmp_2 >= 0 && uTmp_2 <= 0XFFFF)
	{
		return false;
	}
	
	return true;

/*	if (TYPE_SMV_MAC == n_type)
	{
		if (0X04CD0C01 != uTmp_4)//SMV组播地址 应该为01-0c-cd-04-xx-xx
		{
			return true;
		}
	}
	else if (TYPE_GOOSE_MAC == n_type)
	{
		if (0X01CD0C01 != uTmp_4)//GOOSE组播地址 应该为01-0c-cd-01-xx-xx
		{
			return true;
		}
	}

	if (uTmp_2 >= 0 && uTmp_2 <= 0XFF)
	{
		return false;
	}

	return true;*/

}


bool mac_error(char *p_mac, int n_type)
{
	char *p_tmp = NULL;
	bool b_error = false;
	char c_mac_addr[18];
	if (NULL == p_mac)
	{
		return true;
	}

	if (!str_cmp_i(p_mac,"ff-ff-ff-ff-ff-ff"))//广播地址
	{
		return false;
	}
	
	memset(c_mac_addr, 0, sizeof(c_mac_addr));
	strncpy(c_mac_addr,p_mac,12);
	
	if (TYPE_SMV_MAC == n_type)
	{
		if (0!=str_cmp_i(c_mac_addr,"01-0c-cd-04-"))//SMV组播地址
		{
			return true;
		}
	}
	else if (TYPE_GOOSE_MAC == n_type)
	{
		if (0!=str_cmp_i(c_mac_addr,"01-0c-cd-01-"))//GOOSE组播地址
		{
			return true;
		}
	}

	memset(c_mac_addr, 0, sizeof(c_mac_addr));
	strncpy(c_mac_addr,p_mac+12,5);
	unsigned int ntmp1=0;
	unsigned int ntmp2=0;
	sscanf(c_mac_addr, "%02x-%02x", &ntmp1, &ntmp2);
	if ( (ntmp1>=0 && ntmp1<=0xff) && (ntmp2>=0 && ntmp2<=0xff) )
//	if ( (ntmp1>=0 && ntmp1<=0x01) && (ntmp2>=0 && ntmp2<=0xff) )//规范里定的主播地址范围
	{
		return false;
	}

	return true;
}

/*bool mac_error(char *p_mac, int n_type)
{
	char *p_tmp = NULL;
	bool b_error = false;
	char c_mac_addr[18];
	char c_tmp[3];
	if (NULL == p_mac)
	{
		return true;
	}

	memset(c_mac_addr, 0, 18);
	strncpy(c_mac_addr,p_mac,12);

	if (TYPE_SMV_MAC == n_type)
	{
		if (str_cmp_i(c_mac_addr,"01:0c:cd:04:") && str_cmp_i(c_mac_addr,"ff:ff:ff:ff:"))
		{
			return true;
		}
	}
	else if (TYPE_GOOSE_MAC == n_type)
	{
		if (str_cmp_i(c_mac_addr,"01:0c:cd:01:") && str_cmp_i(c_mac_addr,"ff:ff:ff:ff:"))
		{
			return true;
		}
	}

	strcpy(c_mac_addr,p_mac);

	//判断最后两位
	p_tmp = strrchr(c_mac_addr,'\:');
	p_tmp++;
	if (is_tow_hex(p_tmp))
	{
		return true;
	}

	//判断倒数第二个两位
	p_tmp--;
	p_tmp--;
	p_tmp--;
	memset(c_tmp,0,3);
	strncpy(c_tmp,p_tmp,2);
	if (is_tow_hex(c_tmp))
	{
		return true;
	}

	return b_error;
}*/

bool same_mms_data(MMS_DATA_STRUCT *p_data_fst, MMS_DATA_STRUCT *p_data_sec)
{
	int i=0;

	if (NULL==p_data_fst || NULL==p_data_sec)
	{
		return false;
	}

	if (p_data_fst->n_data_type != p_data_sec->n_data_type)
	{
		return false;
	}

	if (p_data_fst->n_struct_num != p_data_sec->n_struct_num)
	{
		return false;
	}
	if (0 == p_data_fst->n_struct_num)
	{
		if (strcmp(p_data_fst->c_data,p_data_sec->c_data))
		{
			return false;
		}
	}
	else
	{
		for (i=0; i<p_data_fst->n_struct_num; i++)
		{
			if (same_mms_data(p_data_fst->p_struct,p_data_sec->p_struct))
			{
				return false;
			}
		}
	}

	return true;
}

int zero_message_err(MESSAGE_ERROR_STRUCT &message_error)
{
//	message_error.n_error_class = ERROR_CLASS_UNKOWN;
	message_error.n_appid = 0;
	memset(message_error.c_avtm,0,sizeof(message_error.c_avtm));
	message_error.v_err_numbers.clear();
	message_error.map_asdu_2_errs.clear();
	message_error.map_gs_daseq_2_errs.clear();
	
	return RES_SUCCESS;
}

double getsec(char *c_avtm1, char *c_avtm2)
{
	double d_sec_interval = 0;
	unsigned int sec1 = 0;
	unsigned int usec1 = 0;
	unsigned int sec2 = 0;
	unsigned int usec2 = 0;
	int sec = 0;//间隔秒
	int usec = 0;//间隔微秒

	if (NULL==c_avtm1 || NULL==c_avtm2)
	{
		return 0;
	}

	sscanf(c_avtm1, "%d.%d", &sec1, &usec1);
	sscanf(c_avtm2, "%d.%d", &sec2, &usec2);
	sec = sec2-sec1;
	usec = usec2-usec1;

	//两帧报文的到达时间间隔
	d_sec_interval = (double)sec + (double)usec/(double)1000000;

	return d_sec_interval;
}

double getsec_ex(char *c_avtm1, char *c_avtm2)
{
	double d_sec_interval = 0;
	unsigned int sec1 = 0;
	unsigned int usec1 = 0;
	unsigned int sec2 = 0;
	unsigned int usec2 = 0;
	int sec = 0;//间隔秒
	int usec = 0;//间隔微秒
	
	if (NULL==c_avtm1 || NULL==c_avtm2)
	{
		return 0;
	}
	
	memcpy((char*)&sec1,c_avtm1,4);
	memcpy((char*)&usec1,c_avtm1+4,4);	
	memcpy((char*)&sec2,c_avtm2,4);
	memcpy((char*)&usec2,c_avtm2+4,4);	

	sec = sec2-sec1;
	usec = usec2-usec1;
	
	//两帧报文的到达时间间隔
	d_sec_interval = (double)sec + (double)usec/(double)1000000;
	
	return d_sec_interval;
}

MESSAGE_ERROR_INFO_STRUCT* create_msg_err_info(MESSAGE_ERROR_STRUCT* pmsg_err)
{
	MESSAGE_ERROR_INFO_STRUCT* pmsg_err_info = NULL;

	if (NULL==pmsg_err)
	{
		return NULL;
	}

	pmsg_err_info = new MESSAGE_ERROR_INFO_STRUCT;
	Msgerr2MsgerrInfo_ex(pmsg_err, pmsg_err_info);

	return pmsg_err_info;
}

int Msgerr2MsgerrInfo_ex(MESSAGE_ERROR_STRUCT *pmsgerr, MESSAGE_ERROR_INFO_STRUCT *p_msgerrInfo)
{
	int nRet = RES_SUCCESS;
	int i=0;
	
	p_msgerrInfo->n_appid = pmsgerr->n_appid;
	p_msgerrInfo->n_error_class = pmsgerr->n_error_class;
//	strcpy(p_msgerrInfo->c_avtm,pmsgerr->c_avtm);//性能优化后删除
	//性能优化
	memcpy(p_msgerrInfo->c_avtm,pmsgerr->c_avtm, sizeof(pmsgerr->c_avtm));
	
	p_msgerrInfo->n_num_errcode = pmsgerr->v_err_numbers.size();
	p_msgerrInfo->p_errcode = create_errcode(pmsgerr->v_err_numbers);
	
	if (p_msgerrInfo->n_error_class==ERROR_CLASS_GOOSE)
	{
		p_msgerrInfo->n_num_asduerr = pmsgerr->map_gs_daseq_2_errs.size();
		p_msgerrInfo->p_asduerr = create_gsda_err(pmsgerr->map_gs_daseq_2_errs);
	}
	else if (p_msgerrInfo->n_error_class==ERROR_CLASS_SMV)
	{
		p_msgerrInfo->n_num_asduerr = pmsgerr->map_asdu_2_errs.size();
		p_msgerrInfo->p_asduerr = create_asdu_err(pmsgerr->map_asdu_2_errs);
	}
	
	return nRet;
}

MESSAGE_ERROR_INFO_ARRAY_STRUCT* CreateMsgErrArray_ex(vector<MESSAGE_ERROR_STRUCT> message_err_lst)
{
	MESSAGE_ERROR_INFO_ARRAY_STRUCT* pMsgErrArry = NULL;
	int i=0;
	if (0 >= message_err_lst.size())
	{
		return NULL;
	}
	
	pMsgErrArry = new MESSAGE_ERROR_INFO_ARRAY_STRUCT;
	pMsgErrArry->n_msg_err = message_err_lst.size();
	
	pMsgErrArry->p_msg_err = new MESSAGE_ERROR_INFO_STRUCT[message_err_lst.size()];
	for (i=0; i<message_err_lst.size(); i++)
	{
		Msgerr2MsgerrInfo_ex(&message_err_lst[i],&pMsgErrArry->p_msg_err[i]);
	}
	
	return pMsgErrArry;
}