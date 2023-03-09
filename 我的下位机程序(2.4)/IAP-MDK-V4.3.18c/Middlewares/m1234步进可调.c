#include "plc_switch.h"
#include "plc_bridge.h"
#include "plc_beep.h"
#include "plc_delay.h"
#include "plc_motor.h"

#include "plc_test.h"
#include "mb_cb.h"
#include "mb_master.h"
#ifdef CONFIG_PLC_TEST_SCAN
	#define CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_SIZE	4
	#define CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK	(CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_SIZE - 1)
	typedef struct
	{
		uint8_t  step;
		PLC_TYPE_FLOW_INFO node_buf[CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_SIZE];
		uint8_t 					 node_pos;
		
		uint8_t  id;
	}PLC_FLOW_TEST_MOTOR_PORT;

	PLC_FLOW_TEST_MOTOR_PORT g_plc_flow_test_motor_port;


	#define CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_SIZE	4
	#define CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK	(CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_SIZE - 1)
	typedef struct
	{
		uint8_t  step;
		PLC_TYPE_FLOW_INFO node_buf[CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_SIZE];
		uint8_t 					 node_pos;
		
		uint8_t  id;
	}PLC_FLOW_TEST_SWITCH_PORT;
	PLC_FLOW_TEST_SWITCH_PORT g_plc_flow_test_switch_port;
	
	
	
	
	#define CONFIG_PLC_TEST_DCMOTOR_FLOW_NODE_BUF_SIZE	4
	#define CONFIG_PLC_TEST_DCMOTOR_FLOW_NODE_BUF_MASK	(CONFIG_PLC_TEST_DCMOTOR_FLOW_NODE_BUF_SIZE - 1)
	typedef struct
	{
		uint8_t  step;
		PLC_TYPE_FLOW_INFO node_buf[CONFIG_PLC_TEST_DCMOTOR_FLOW_NODE_BUF_SIZE];
		uint8_t 					 node_pos;
		
		uint8_t  id;
	}PLC_FLOW_TEST_DCMOTOR_PORT;

	PLC_FLOW_TEST_DCMOTOR_PORT g_plc_flow_test_dcmotor_port;
	
	/*�Զ������*/
	;
	static uint32_t flag_one=30303;
	static uint32_t flag_two=279000;//1000->1mm
	static uint32_t flag_tf=26520;//400->1mm
	/*
	***********************************************************************
	* ���ư����
	***********************************************************************
	*/
	
	void plc_flow_test_motor_scan(void)
	{
		uint8_t id = 0;
		
		// �ϵ����
		for(id = 0;  id < CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_SIZE; id++){
			if(g_plc_flow_test_motor_port.node_buf[id].stat == TRUE)
				return;
		}

	
		if(pusRegHoldingBuf[6]==1)//E16��λ
	{
		if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0){
			plc_motor_set_step_rst_flow(0, 0,\
			10000, 5000, 0.4,\
			15, 0, 0, -1,\
			CONFIG_PLC_FEEDBACK_FLOW, 0,\
			g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
			}//pusRegHoldingBuf[2]=2;	
	}
			/*����������*/
		if(pusRegHoldingBuf[6]==2)//E15��λ
	{
		plc_motor_set_step_pos_flow(0, flag_one-500,\
					8000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
		flag_one=flag_one-500;
			pusRegHoldingBuf[6]=99;	
}
		
		if(pusRegHoldingBuf[6]==3)//E15��λ
	{
		plc_motor_set_step_pos_flow(0, 30303,\
					8000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
		//flag=flag+500;
			pusRegHoldingBuf[6]=99;	
}
	if(pusRegHoldingBuf[2]==1)/*�Ҹ�λ*/
	{
		if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0){
			plc_motor_set_step_rst_flow(1, 0,\
			64000, 15000, 0.4,\
			0, 0, 0, -1,\
			CONFIG_PLC_FEEDBACK_FLOW, 0,\
			g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
			}
	}
			/*����������*/
	if(pusRegHoldingBuf[2]==2)//�����ƶ�1mm
	{
		plc_motor_set_step_pos_flow(1, flag_two+1000,\
					64000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
						g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
		flag_two=flag_two+1000;
		pusRegHoldingBuf[2]=99;		
	}		
	
	if(pusRegHoldingBuf[2]==3)	//�����ƶ�	
	{
	
	plc_motor_set_step_pos_flow(1, flag_two-1000,\
					64000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
						g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
				flag_two=flag_two-1000;	
			pusRegHoldingBuf[2]=99;	
	}
	if(pusRegHoldingBuf[2]==4)//���Ҹ�λ
	{			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0){
					plc_motor_set_step_rst_flow(1, 0,\
						64000, 15000, 0.4,\
						1, 0, 0, 1,\
						CONFIG_PLC_FEEDBACK_FLOW, 0,\
						g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
					}					
				}
			
					//pusRegHoldingBuf[4]=0;return;
				//flag++;
		
	
		if(pusRegHoldingBuf[2]==5)//�����ƶ�279mm
	{
		plc_motor_set_step_pos_flow(1, flag_two,\
					64000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
						g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
		pusRegHoldingBuf[2]=99;		
	}	
		
		/*ͬ�����M3��M4��ʼ�˶���������*/
	//	void motor_steptf();
		if(pusRegHoldingBuf[4]==10)/*���¸�λ*/
		{if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0){
			plc_motor_set_step_rst_flow(3, 0,\
					8000, 500, 0.1,\
					2, 0, 0, -1,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);}	
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0){
			plc_motor_set_step_rst_flow(2, 0,\
					8000, 500, 0.1,\
					2, 0, 0, -1,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);	
					
			}
		}
	if(pusRegHoldingBuf[4]==1)/*����0.1mm*/
		{plc_motor_set_step_pos_flow(2, flag_tf-40,\
					8000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
		plc_motor_set_step_pos_flow(3, flag_tf-40,\
					8000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
		flag_tf=flag_tf-40;
	pusRegHoldingBuf[4]=99;}
	
	if(pusRegHoldingBuf[4]==2){/*����1mm*/
		plc_motor_set_step_pos_flow(2, flag_tf-400,\
					8000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
		plc_motor_set_step_pos_flow(3, flag_tf-400,\
					8000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
		flag_tf=flag_tf-400;
	pusRegHoldingBuf[4]=99;}
	
	
	if(pusRegHoldingBuf[4]==3){/*����0.1mm*/
		plc_motor_set_step_pos_flow(2, flag_tf+40,\
					8000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
		plc_motor_set_step_pos_flow(3, flag_tf+40,\
					8000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
		flag_tf=flag_tf+40;
	pusRegHoldingBuf[4]=99;}
	
	if(pusRegHoldingBuf[4]==4){/*����1mm*/
		plc_motor_set_step_pos_flow(2, flag_tf+400,\
					8000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
		plc_motor_set_step_pos_flow(3, flag_tf+400,\
					8000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
		flag_tf=flag_tf+400;
	pusRegHoldingBuf[4]=99;}
	
	if(pusRegHoldingBuf[4]==5){/*����1mm*/
		plc_motor_set_step_pos_flow(2, flag_tf,\
					8000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
		plc_motor_set_step_pos_flow(3, flag_tf,\
					8000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
		//flag=flag-400;
	///pusRegHoldingBuf[4]=99;
		}
	
}
		
		

	void plc_flow_test_switch_scan(void)
{
		uint8_t id = 0;
		
		// �ϵ����
		for(id = 0;  id < CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_SIZE; id++){
			if(g_plc_flow_test_switch_port.node_buf[id].stat == TRUE)
				return;
		}

		switch(g_plc_flow_test_switch_port.step)
		{
			case 0:		
				plc_switch_set_flow(g_plc_flow_test_switch_port.id,\
					1, 0, 500, 500, 3,\
					-1, 0,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
			break;
				
			case 1:			
				plc_delay_flow(1, 1000,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
			
				g_plc_flow_test_switch_port.id = (g_plc_flow_test_switch_port.id + 1)%16;
				g_plc_flow_test_switch_port.step = 0;
			return;
		}
		
		g_plc_flow_test_switch_port.step++;
	}
	
	void plc_flow_test_dcmotor_scan(void)
	{
		uint8_t id = 0;
		
		// �ϵ����
		for(id = 0;  id < CONFIG_PLC_TEST_DCMOTOR_FLOW_NODE_BUF_SIZE; id++){
			if(g_plc_flow_test_dcmotor_port.node_buf[id].stat == TRUE)
				return;
		}

		switch(g_plc_flow_test_dcmotor_port.step)
		{
			case 0:					
				plc_bridge_flow(g_plc_flow_test_dcmotor_port.id, 20, 1, 0, 3000,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_dcmotor_port.node_buf, &g_plc_flow_test_dcmotor_port.node_pos, CONFIG_PLC_TEST_DCMOTOR_FLOW_NODE_BUF_MASK);
			break;
				
			case 1:			
				plc_delay_flow(2, 1000,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_dcmotor_port.node_buf, &g_plc_flow_test_dcmotor_port.node_pos, CONFIG_PLC_TEST_DCMOTOR_FLOW_NODE_BUF_MASK);
			break;
			
			case 2:					
				plc_bridge_flow(g_plc_flow_test_dcmotor_port.id, 20, 0, 0, 3000,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_dcmotor_port.node_buf, &g_plc_flow_test_dcmotor_port.node_pos, CONFIG_PLC_TEST_DCMOTOR_FLOW_NODE_BUF_MASK);
			break;
		
			case 3:
				plc_delay_flow(2, 1000,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_dcmotor_port.node_buf, &g_plc_flow_test_dcmotor_port.node_pos, CONFIG_PLC_TEST_DCMOTOR_FLOW_NODE_BUF_MASK);
			
			g_plc_flow_test_dcmotor_port.id = (g_plc_flow_test_dcmotor_port.id + 1)%3;
			g_plc_flow_test_dcmotor_port.step = 0;
			return;
		}
		
		g_plc_flow_test_dcmotor_port.step++;
	}
	
	void plc_flow_test_scan(void)
	{
		plc_flow_test_motor_scan();
		plc_flow_test_switch_scan();
		plc_flow_test_dcmotor_scan();
	}
	
#endif
