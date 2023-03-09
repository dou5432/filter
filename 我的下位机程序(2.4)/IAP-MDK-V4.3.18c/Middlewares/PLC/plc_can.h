#ifndef __PLC_CAN_H__
#define __PLC_CAN_H__

#include "plc_type.h"
#include "plc_cfg.h"

// ��ʼ��
uint8_t plc_can_init(CAN_HandleTypeDef *hcan);

// ��������
void plc_can_receive(CAN_HandleTypeDef *hcan);

// ��������
void plc_can_send(CAN_HandleTypeDef *hcan, uint32_t std_id, uint8_t *buf, uint8_t len);

#endif
