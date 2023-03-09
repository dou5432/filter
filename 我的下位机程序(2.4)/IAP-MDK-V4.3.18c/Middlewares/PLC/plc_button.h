#ifndef __PLC_BUTTON_H__
#define __PLC_BUTTON_H__

#include "plc_type.h"
#include "plc_cfg.h"
#include "plc_input.h"


void plc_button_run_input(void);
void plc_button_run_switch(void);
void plc_button_run_bridge(void);
void plc_button_run_beep(void);
void plc_button_run_delay(void);
void plc_button_run_motor(void);

void plc_button_stop_input(void);
void plc_button_stop_switch(void);
void plc_button_stop_bridge(void);
void plc_button_stop_beep(void);
void plc_button_stop_delay(void);

// ��Ӧ
void plc_button_run(void);
void plc_button_stop(void);

// ��ʼ������
void plc_button_init(void);

// ����Ӳ������
void plc_button_set(uint8_t id, uint8_t input_id, void (*func)(void));

// ɨ��
void plc_button_scan(void);

#endif
