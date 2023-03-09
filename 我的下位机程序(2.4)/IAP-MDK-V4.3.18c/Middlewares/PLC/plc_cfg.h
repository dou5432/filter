#ifndef __PLC_CFG_H__
#define __PLC_CFG_H__

// 端口数量
#define CONFIG_PLC_MOTOR_PORT_COUNT			8
#define CONFIG_PLC_BLDC_PORT_COUNT					8
#define CONFIG_PLC_BRIDGE_PORT_COUNT		 		3
#define CONFIG_PLC_SWITCH_PORT_COUNT				16
#define CONFIG_PLC_INPUT_PORT_COUNT					17
#define CONFIG_PLC_DELAY_PORT_COUNT		 			4
#define CONFIG_PLC_BEEP_PORT_COUNT				 	1
#define CONFIG_PLC_BUTTON_HW_PORT_COUNT			3
#define CONFIG_PLC_PLC_PORT_COUNT						1


// 通信端口
#define CONFIG_PLC_MB_ADDRESS_INPUT			50000	//16*16
#define CONFIG_PLC_MB_ADDRESS_SWITCH		50256	//16*24
#define CONFIG_PLC_MB_ADDRESS_BRIDGE		50640	//3*16
#define CONFIG_PLC_MB_ADDRESS_BEEP			50688	//1*24
#define CONFIG_PLC_MB_ADDRESS_STEPMOTOR	50712	//16*26
#define CONFIG_PLC_MB_ADDRESS_EEPROM		57344	//8K

#endif

