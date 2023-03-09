#ifndef __MB_TIMER_H__
#define __MB_TIMER_H__

#include "mb_type.h"

// ʱ��ˢ��
void mb_timer_rcv_update(MB_VAR_PORT *p_mb_var_port);
void mb_timer_snd_update(MB_VAR_PORT *p_mb_var_port);

// ʱ��ʧЧ
void mb_timer_rcv_disable(MB_VAR_PORT *p_mb_var_port);
	
// ��ʱ�ж�
uint8_t mb_timer_rcv_outtime(MB_VAR_PORT *p_mb_var_port);
uint8_t mb_timer_snd_outtime(MB_VAR_PORT *p_mb_var_port);
	
#endif
