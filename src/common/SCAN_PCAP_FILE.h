#ifndef SCAN_PCAP_FILE_STRUCT
#define SCAN_PCAP_FILE_STRUCT

#define	PCAP_MAGIC			0xa1b2c3d4               //ÍøÂç×Ö½ÚÐò
#define	PCAP_SWAPPED_MAGIC		0xd4c3b2a1           //Ð¡¶Ë×Ö½ÚÐò
#define	PCAP_MODIFIED_MAGIC		0xa1b2cd34           
#define	PCAP_SWAPPED_MODIFIED_MAGIC	0x34cdb2a1
#define	PCAP_NSEC_MAGIC			0xa1b23c4d           //ÄÉÃë°æ±¾¸ñÊ½
#define	PCAP_SWAPPED_NSEC_MAGIC		0x4d3cb2a1

/* "libpcap" file header (minus magic number). */
typedef struct _PCAP_HEADER {
	unsigned short	MajorVersion;	/* major version number */
	unsigned short	MinorVersion;	/* minor version number */
	unsigned int	ThisZone;	/* GMT to local correction */
	unsigned int	Sigfigs;	/* accuracy of timestamps */
	unsigned int	SnapLen;	/* max length of captured packets, in octets */
	unsigned int	Network;	/* data link type */
}PCAP_HEADER, *PPCAP_HEADER;


/* "libpcap" record header. */
typedef struct _PCAPREC_HEADER {
	unsigned int	Sec;		/* timestamp seconds */
	unsigned int	Usec;	/* timestamp microseconds (nsecs for PCAP_NSEC_MAGIC) */
	unsigned int	InclLen;	/* number of octets of packet saved in file */
	unsigned int	OrigLen;	/* actual length of packet */
}PCAPREC_HEADER, *PPCAPREC_HEADER;


typedef struct _ADV_PACKET_HEADER {
	unsigned int	Sec;		/* timestamp seconds */
	unsigned int	Usec;	/* timestamp microseconds (nsecs for PCAP_NSEC_MAGIC) */
	unsigned int	Len;	/* number of octets of packet saved in file */
}ADV_PACKET_HEADER, *PADV_PACKET_HEADER;

#endif  /*AT_STRUCT_H_HEADER_INCLUDED_88888888*/