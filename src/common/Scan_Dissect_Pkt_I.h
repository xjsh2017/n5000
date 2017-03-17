#if !defined(_SCAN_DISSECT_PKT_I_H)
#define _SCAN_DISSECT_PKT_I_H
/**
 * @brief       ���ݰ������ӿ�
 * @use			wirshark���ݰ�������װ�ӿ�
 * @author      �����
 * @version     ver1.0
 * @date        2012/10/17
 * example
 * @code
 *
 *
 *
 * @endcode
 */
#include "Scan_Dissect_Pkt_Struct.h"

#ifdef OS_WINDOWS

#ifdef PKT_DISSECT_DLL
//�������ݰ����ͣ��������ݰ�
extern "C"  __declspec( dllexport ) XJ_DISSECT_PKT*  xj_dissect_packet(const char* cdatabuffer,int nbufferlength);
//�������ݰ����ͣ��������ݰ�_windows����������
extern "C"  __declspec( dllexport ) XJ_DISSECT_PKT* xj_dissect_packet_offline_mms(const char* cdatabuffer,unsigned int nbufferlength,unsigned int nframenum,unsigned int nfile_off,unsigned int ncum_bytes,unsigned int nseconds,unsigned nuseconds);
//�ͷ����ݰ�
extern "C"  __declspec( dllexport )              int xj_cleanup_packet(XJ_DISSECT_PKT* xj_dissect_pkt);
//�ͷŻỰ�����絥��ץ���ļ��������̣�
extern "C"  __declspec( dllexport )              void xj_cleanup_dissect();
#else 
//�������ݰ����ͣ��������ݰ�
extern "C"  __declspec( dllimport ) XJ_DISSECT_PKT*  xj_dissect_packet(const char* cdatabuffer,int nbufferlength);
//�ͷ����ݰ�
extern "C"  __declspec( dllimport )             int  xj_cleanup_packet(XJ_DISSECT_PKT* xj_dissect_pkt);
#endif
#endif //OS_WINDOWS

#ifdef OS_LINUX
#ifdef __cplusplus
extern "C"
{
#endif
//�������ݰ����ͣ��������ݰ�
	XJ_DISSECT_PKT*  xj_dissect_packet(const char* cdatabuffer,int nbufferlength);
//�ͷ����ݰ�
int  xj_cleanup_packet(XJ_DISSECT_PKT* xj_dissect_pkt);
//�ͷŻỰ�����絥��ץ���ļ��������̣�
void xj_cleanup_dissect();
#ifdef __cplusplus
}
#endif
#endif//OS_LINUX

#endif//#define _SCAN_DISSECT_PKT_I_H