#if !defined(_SCAN_DISSECT_STRUCT_H)
#define _SCAN_DISSECT_STRUCT_H
/**
 * @brief       ���ݰ������ӿ�
 * @use			wirshark���ݰ�������װ�ӿڽṹ����
 * @author      �����
 * @version     ver1.0
 * @date        2012/10/17
 */
/* field types �����ڵ�����*/
enum xj_pkt_field_type_enum {
	PKT_FT_NONE,	/* used for text labels with no value */
	PKT_FT_PROTOCOL,
	PKT_FT_BOOLEAN,	/* TRUE and FALSE come from <glib.h> */
	PKT_FT_UINT8,
	PKT_FT_UINT16,
	PKT_FT_UINT24,	/* really a UINT32, but displayed as 3 hex-digits if FD_HEX*/
	PKT_FT_UINT32,
	PKT_FT_UINT64,
	PKT_FT_INT8,
	PKT_FT_INT16,
	PKT_FT_INT24,	/* same as for UINT24 */
	PKT_FT_INT32,
	PKT_FT_INT64,
	PKT_FT_FLOAT,
	PKT_FT_DOUBLE,
	PKT_FT_ABSOLUTE_TIME,
	PKT_FT_RELATIVE_TIME,
	PKT_FT_STRING,
	PKT_FT_STRINGZ,	/* for use with proto_tree_add_item() */
	PKT_FT_UINT_STRING,	/* for use with proto_tree_add_item() */
	/*FT_UCS2_LE, */    /* Unicode, 2 byte, Little Endian     */
	PKT_FT_ETHER,
	PKT_FT_BYTES,
	PKT_FT_UINT_BYTES,
	PKT_FT_IPv4,
	PKT_FT_IPv6,
	PKT_FT_IPXNET,
	PKT_FT_FRAMENUM,	/* a UINT32, but if selected lets you go to frame with that number */
	PKT_FT_PCRE,	/* a compiled Perl-Compatible Regular Expression object */
	PKT_FT_GUID,	/* GUID, UUID */
	PKT_FT_OID,		/* OBJECT IDENTIFIER */
	PKT_FT_EUI64,
	PKT_FT_NUM_TYPES /* last item number plus one */
};
/* ETHER_TYPE �����ڵ�����*/
enum xj_pkt_ether_type_enum//ͬ��const_scan.h::enum ETHER_TYPE::ETHER_TYPE_DROP��
{
    PKT_ETHER_TYPE_UNKOWN           =0,
	PKT_ETHER_TYPE_IEC61850_MMS     =1,
	PKT_ETHER_TYPE_IEC61850_GOOSE   =2,
	PKT_ETHER_TYPE_IEC61850_SMV     =3,
	PKT_ETHER_TYPE_IEC61850_SMV_91  =4,
	PKT_ETHER_TYPE_IEC61850_PTP_1588=5,
	PKT_ETHER_TYPE_ARP              =6,
	PKT_ETHER_TYPE_IP               =7,
	PKT_ETHER_TYPE_TCP              =8,
	PKT_ETHER_TYPE_UDP              =9,
	PKT_ETHER_TYPE_COTP             =13,
	PKT_ETHER_TYPE_TCP_FIN			    =14,//�ͷ�TCP���� 
	PKT_ETHER_TYPE_TCP_SYN_REQ		    =15,//����
	PKT_ETHER_TYPE_TCP_SYN_RESP	        =16,//��������
	PKT_ETHER_TYPE_TCP_RST			    =17,//��������
	PKT_PKT_ETHER_TYPE_TCP_PSH			=18,//��Ӧ�����ݣ����շ���Ҫ���촦��
	PKT_ETHER_TYPE_TCP_ACK			    =19,//��������
	PKT_ETHER_TYPE_TCP_URG			    =20,//��������
	PKT_ETHER_TYPE_TPKT                 =21,//add by yinzhehong 20130321
	PKT_ETHER_TYPE_DROP                     =22,//add by guohuawen 20151010 //��·�㶪������
};
typedef struct _xj_proto_node
{
	struct _xj_proto_node  *parent_node;       //���ڵ�
	struct _xj_proto_node  *pchildlist;        //�ӽڵ��ָ��
	int                    childlist_size;     //�ӽڵ����
	int				       start_pos;         //��Դ�����е���ʼλ��
	int				       length;			  //< ����ռ�ó���
	void*				   pkt_usrview;       //���ⲿӦ��ʹ��
	char		           *node_name;        //�ڵ�����
	char                   *node_name_detail; //�ڵ�������ϸ����
	char                   *node_desc;        //�ڵ�����
	int                     errortype;        //�ڵ�������� 
	xj_pkt_ether_type_enum node_type;         //�ڵ����� 0:�����ڵ� 1:��λ�ڵ� 2:�쳣�ڵ�        
} XJ_PROTO_NODE;
/**************ת��������ݰ�*******************/
typedef struct _xj_dissect_pkt
{
	xj_pkt_ether_type_enum ether_type;   //��������
	int          pkt_length;             //���ĳ���
	void*        pkt_data;               //ԭʼ����֡��Ӧָ��
	int          utc_s;                  //utc��
	int          utc_ms;                 //utc����
	XJ_PROTO_NODE * pprototreelist;      //Э����
	int           prototreelist_size;    //Э�������
	int           prototreelist_app_seq; //��ЧӦ�ò�Ĳ��
	char         mac_src[18];            //���Ͷ�MAC��ַ
	char         mac_dst[18];            //���ն�MAC��ַ
	char         ip_src[18];             //���Ͷ�IP��ַ
	char         ip_dst[18];             //���ն�IP��ַ
	int          port_src;               //���Ͷ˶˿�
	int          port_dst;               //���ն˶˿�
	
}XJ_DISSECT_PKT;

#endif
