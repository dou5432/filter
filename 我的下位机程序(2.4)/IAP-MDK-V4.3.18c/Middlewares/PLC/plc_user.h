#ifndef __PLC_USER_H__
#define __PLC_USER_H__

#include "stdint.h"

// �˿ڳ�ʼ��
void plc_user_init(void);

// ɨ��
uint8_t plc_user_scan(uint8_t enable);
	
#endif
