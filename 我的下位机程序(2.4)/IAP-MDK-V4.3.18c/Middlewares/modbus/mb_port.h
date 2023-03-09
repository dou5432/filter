#ifndef __MB_PORT_H__
#define __MB_PORT_H__

#include "mb_type.h"
#include "mb_proto.h"
#include "mb_frame.h"

// �˿ڳ�ʼ��
eMBErrorCode mb_port_init(uint8_t id, eMBCommMode eCommMode, eMBMode eMode,\
	uint8_t ucSlaveAddress, uint16_t ucPort, uint32_t ulBaudRate, eMBParity eParity);

// ����
eMBErrorCode mb_port_poll( uint8_t id );

#endif
