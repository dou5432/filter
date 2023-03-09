#ifndef __MB_ASCII_H__
#define __MB_ASCII_H__

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif

#include "stdint.h"
#include "mb_type.h"

// ��ʼ��
eMBErrorCode mb_ascii_init(uint8_t id, uint8_t ucSlaveAddress, uint8_t ucPort, uint32_t ulBaudRate, eMBParity eParity);

// ��������
void mb_ascii_receive(uint8_t id, uint8_t context);

// ����֡����
uint8_t mb_ascii_snd_frame(uint8_t id);

// ����֡����
void mb_ascii_frame_conform(uint8_t id);

// ֡�ж�
uint8_t mb_ascii_rev_adjust(uint8_t id);

// ֡�ֶΣ��ӵ�ַ + ֡���� + ֡���ȣ�
uint8_t mb_ascii_segmentation( uint8_t id, uint8_t *cur_frame_address, uint8_t *cur_frame_pos, uint16_t *cur_frame_len );

#ifdef __cplusplus
PR_END_EXTERN_C
#endif
#endif
