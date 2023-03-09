#ifndef __MB_RTU_H__
#define __MB_RTU_H__

#include "mb_type.h"
// ��ʼ��
eMBErrorCode mb_rtu_init(uint8_t id, uint8_t ucSlaveAddress, uint8_t ucPort, uint32_t ulBaudRate, eMBParity eParity);

// ��������
void mb_rtu_receive(uint8_t id, uint8_t context);

// ֡�ж�
uint8_t mb_rtu_rev_adjust(uint8_t id);
uint8_t mb_rtu_snd_frame(uint8_t id);

// ֡�ֶΣ��ӵ�ַ + ֡���� + ֡���ȣ�
uint8_t mb_rtu_segmentation( uint8_t id, uint8_t *cur_frame_address, uint8_t *cur_frame_pos, uint16_t *cur_frame_len );

// ����֡����
void mb_rtu_frame_conform(uint8_t id);

// ������
uint8_t mb_rtu_out(uint8_t id, uint8_t code, uint16_t address, uint16_t count);

uint8_t mb_rtu_out_string(uint8_t id, uint8_t code, uint16_t address, uint8_t *buf);

#endif
