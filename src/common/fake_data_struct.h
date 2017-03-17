#ifndef FAKE_DATA_STRUCT_H
#define FAKE_DATA_STRUCT_H
typedef struct _TS_TIMEVAL
{
	unsigned int GmtTime;//秒计时 1970年后的秒
	unsigned int us;     //微秒计时 可受头结构中的Sigfigs限制，精度为微秒或者纳秒
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
	unsigned short	MajorVersion;	/* 主版本号 */
	unsigned short	MinorVersion;	/* 副版本号 */
	unsigned int	ThisZone;	    /* 区域时间，32位，未用，默认为0 */
	unsigned int	Sigfigs;	    /*精确时间戳，32位 ，未用，默认为0值*/
	unsigned int	SnapLen;	    /* 所抓获的数据包最大长度*/
	unsigned int	Network;	    /* 链路层类型 */
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