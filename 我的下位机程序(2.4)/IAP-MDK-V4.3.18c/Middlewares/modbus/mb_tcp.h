#ifndef __MB_TCP_H__
#define __MB_TCP_H__

#include "mb_type.h"

// ��ʼ��
eMBErrorCode mb_tcp_init(uint8_t id, uint8_t ucSlaveAddress, uint16_t ucPort);

// ��������
void mb_tcp_receive(uint8_t id, uint8_t context);

// ����֡����
uint8_t mb_tcp_snd_frame(uint8_t id);

// ֡�ж�
uint8_t mb_tcp_rev_adjust(uint8_t id);

// ����֡����
void mb_tcp_frame_conform(uint8_t id);

// ֡�ֶΣ��ӵ�ַ + ֡���� + ֡���ȣ�
uint8_t mb_tcp_segmentation( uint8_t id, uint8_t *cur_frame_address, uint8_t *cur_frame_pos, uint16_t *cur_frame_len );

// ɨ�����
void mb_tcp_receive_poll(uint8_t id);
#endif
