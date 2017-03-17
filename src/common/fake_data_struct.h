#ifndef FAKE_DATA_STRUCT_H
#define FAKE_DATA_STRUCT_H
typedef struct _TS_TIMEVAL
{
	unsigned int GmtTime;//���ʱ 1970������
	unsigned int us;     //΢���ʱ ����ͷ�ṹ�е�Sigfigs���ƣ�����Ϊ΢���������
	_TS_TIMEVAL()
	{
		GmtTime = 0;
		us      = 0;
	}

}TS_TIMEVAL;
typedef struct _TS_PCAP_PKTHDR{
	TS_TIMEVAL ts;	/* time stamp */
	unsigned int caplen;	/* length of portion present */
	unsigned int len;	    /* length this packet (off wire) */
	_TS_PCAP_PKTHDR()
	{
		caplen = 0;
		len    = 0;
	}
}TS_PCAP_PKTHDR;
struct _TS_PCAP_HEADER {
	unsigned int    Magic;
	unsigned short	MajorVersion;	/* ���汾�� */
	unsigned short	MinorVersion;	/* ���汾�� */
	unsigned int	ThisZone;	    /* ����ʱ�䣬32λ��δ�ã�Ĭ��Ϊ0 */
	unsigned int	Sigfigs;	    /*��ȷʱ�����32λ ��δ�ã�Ĭ��Ϊ0ֵ*/
	unsigned int	SnapLen;	    /* ��ץ������ݰ���󳤶�*/
	unsigned int	Network;	    /* ��·������ */
	_TS_PCAP_HEADER()
	{
		Magic = 0xa1b2c3d4;//#define PCAP_MAGIC 0xa1b2c3d4
		MajorVersion = 2;
		MinorVersion = 4;
		ThisZone = 0;
		Sigfigs = 0;
		SnapLen = 0x00010000;
		Network = 1;
	}
};
typedef _TS_PCAP_HEADER TS_PCAP_HEADER;
#endif