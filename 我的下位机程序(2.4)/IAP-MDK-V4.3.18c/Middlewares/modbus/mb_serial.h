#ifndef __MB_SERIAL_H__
#define __MB_SERIAL_H__

#include "mb_type.h"

// ��ʼ��
uint8_t mb_serial_init( uint8_t ucPORT, uint32_t ulBaudRate, uint8_t ucDataBits, eMBParity eParity );

// RS485����
void mb_serial_rs485_dir(uint8_t dir);
	
// ��������
uint8_t mb_serial_receive(UART_HandleTypeDef *huart, uint8_t *buff);

// ��������
void mb_serial_rs485_send(UART_HandleTypeDef *huart, uint8_t *buf);
uint8_t mb_serial_send(MB_VAR_PORT *p_mb_var_port);

// ����һ֡���
void mb_serial_txd_frame_insert(MB_VAR_PORT *p_mb_var_port);
void mb_serial_rxd_frame_insert(MB_VAR_PORT *p_mb_var_port);

// ɾ��һ֡���
void mb_serial_txd_frame_delete(MB_VAR_PORT *p_mb_var_port);
void mb_serial_rxd_frame_delete(MB_VAR_PORT *p_mb_var_port);

// ��һ֡������Ϣ
void mb_serial_frame_info(MB_VAR_PORT *p_mb_var_port, uint8_t *p_pos, uint8_t *p_len);

#endif

