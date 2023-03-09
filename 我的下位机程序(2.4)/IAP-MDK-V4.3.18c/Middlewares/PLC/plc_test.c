#include "plc_switch.h"
#include "plc_bridge.h"
#include "plc_beep.h"
#include "plc_delay.h"
#include "plc_motor.h"
#include "plc_test.h"
#include "mb_cb.h"
#include "mb_master.h"
#include "plc_button.h"
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
	
/***************************************************************
*E5678δ��
*E8��ʱ��λ���˱���
*E5��������
*E6 ֹͣ����
*E7  ��ɫ�������ݶ���λ
****************************************************************/
	
/*�Զ������*/
static uint32_t flag_one = 0;//200->6.6mm
static uint32_t flag_two = 0;//1000->1mm
static uint32_t flag_tf = 26520;//400->1mm

static int32_t flag_motor_five = 0;//570mm
static int32_t flag_motor_six = 0;//0mm
static int32_t flag_motor_seven = 0;//0mm

//static uint32_t flag_motor_astone_two = 0;//0mm
//static uint8_t start_buttom = 0;  //����ʱΪ1��������ʱΪ0
static uint8_t m1position = 0;  //Ϊ1ʱ��ʾƵ��ʱM1��λ�ã�Ϊ0ʱ��ʾM1��ʵʱλ��
	


//static uint8_t finishRunFlag = 0;
	
void motor_astaone(void);
void motor_astatwo(void);
void motor_steptf(void);
void motor_step_waste(void);

void motor_step_waste_M5(void);
void motor_step_waste_M6(void);
void motor_step_waste_M7(void);
void button_con(void);
	
//void finishRun();
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

	/*�ŷ����M1*/
	//motor_astaone();	

	/*�ŷ����M2*/
	//motor_astatwo();
	
	/*ͬ�����M3��M4��ʼ�˶���������*/
	//motor_steptf();
	
	/*�����Ʒ��ȡ����*/
	//motor_step_waste();
	//�������˳�����
	switch (pusRegHoldingBuf[34])
	{
		case 1:		//M1 M2 (M3 M4) M5 M6 M7
			
			motor_astaone();	
			
			motor_astatwo();	
			
			motor_steptf();		//ͬ�����M3��M4��ʼ�˶���������
			
			//motor_step_waste();	//�����Ʒ��ȡ����/
		
			motor_step_waste_M5();		//ȡ��Ʒ�������M5��ʼ�˶�
		
			motor_step_waste_M6();		//ȡ��Ʒ�������M6��ʼ�˶�
		
			motor_step_waste_M7();		//ȡ��Ʒ�������M7��ʼ�˶�
			break;
		
		case 2:		//M2 (M3 M4) M1 M5 M6 M7
			
			motor_astatwo();	//�ŷ����M2//
			
			motor_steptf();		//ͬ�����M3��M4��ʼ�˶���������//
			
			motor_astaone();	//�ŷ����M1//
			
			//motor_step_waste();	//�����Ʒ��ȡ����//
		
			motor_step_waste_M5();		//ȡ��Ʒ�������M5��ʼ�˶�
		
			motor_step_waste_M6();		//ȡ��Ʒ�������M6��ʼ�˶�
		
			motor_step_waste_M7();		//ȡ��Ʒ�������M7��ʼ�˶�
			break;
		default:
			pusRegHoldingBuf[34] = 1;
			break;
	}
		
}

  
void Close_read_camera()
{
	//M2�����շ�Χ��
	if(flag_two == 67000 || flag_two == 126000 || flag_two == 185000 || flag_two == 244000 || flag_two == 303000 || flag_two == 362000 )
	{
		//M34�����շ�Χ��
		//if(flag_tf == 26120)
		{
			pusRegHoldingBuf[30] = 99;		//���һ������
		}
	}
}
	
uint8_t plc_motor_pos_flag(uint8_t flag1)
{
	if(flag1==1)
	{
		//M2�����շ�Χ��
		if(flag_two == 67000 || flag_two == 126000 || flag_two == 185000 || flag_two == 244000 || flag_two == 303000 || flag_two == 362000 )
		{
			//M34�����շ�Χ��
			//if(flag_tf == 26120)
			{
				if(pusRegHoldingBuf[36] == 0)
				{
					if(pusRegHoldingBuf[28] == 1)
						pusRegHoldingBuf[10] = 1;	//�ϲ������������
					else
						pusRegHoldingBuf[10] = 6;	//�ϲ�����������
				}
				else 
				{
					if(pusRegHoldingBuf[28] == 1)
						pusRegHoldingBuf[10] = 8;	//�²������������
					else
						pusRegHoldingBuf[10] = 9;	//�²�����������
				}
			}
			
				
		}
		
	}
	return 0;
}

void finishRst(uint8_t id)
{
	switch(id)
	{
		case 0:
			pusRegHoldingBuf[6] = 99;
			break;
		case 1:
			pusRegHoldingBuf[2] = 99;
			break;
		case 3:
			pusRegHoldingBuf[4] = 99;
			break;
		case 4:
			pusRegHoldingBuf[20] = 99;
			break;
		case 5:
			pusRegHoldingBuf[22] = 99;
			break;
		case 6:
			pusRegHoldingBuf[24] = 99;
			break;
		default:
			break;	
	}
	if(pusRegHoldingBuf[6] == 99 && pusRegHoldingBuf[2] == 99 && pusRegHoldingBuf[4] == 99 && pusRegHoldingBuf[20] == 99 && pusRegHoldingBuf[22] == 99 && pusRegHoldingBuf[24] == 99)
	{
		pusRegHoldingBuf[38] = 1;
	}
	else
	{
		//pusRegHoldingBuf[38] = 0;
	}
	return;
}

void finishRun(uint8_t id)
{
	switch(id)
	{
		case 0:
			pusRegHoldingBuf[6] = 99;
			break;
		case 1:
			pusRegHoldingBuf[2] = 99;
			break;
		case 3:
			pusRegHoldingBuf[4] = 99;
			break;
		case 4:
			pusRegHoldingBuf[20] = 99;
			break;
		case 5:
			pusRegHoldingBuf[22] = 99;
			break;
		case 6:
			pusRegHoldingBuf[24] = 99;
			break;
		default:
			break;	
	}
	return;
}

/**************************************************************************
*40006                 40001
*0  ��λ����һ��       1�����������̵�
*1  �������			  2��ֹͣ�������								
*2  ����			  3���������������Ƶ�
*3  ����			  
*4�������ͷ�
****************************************************************************/
void plc_flow_test_switch_scan(void)
{
	uint8_t id = 0;
		
	// �ϵ����
	for(id = 0;  id < CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_SIZE; id++)
	{
		if(g_plc_flow_test_switch_port.node_buf[id].stat == TRUE)
			return;
	}
		
	/*��ť����*/
	button_con();
	//	case 0:		/*�ӿں�    ������ʱ   ���ʱ��      ���ؼ���  x   x    */
		
	int dis, dis1, dis2; //disΪ���M1��ǰλ�ã�dis = dis1 * 256 + dis2
	
	if(plc_input_read_flow(15)==0 && plc_input_read_flow(0) == 0 && plc_input_read_flow(2) == 0 && plc_input_read_flow(10) == 0 && plc_input_read_flow(8) == 0 && plc_input_read_flow(12) == 0)
	{
		pusRegHoldingBuf[38] = 1;
	}
		
	switch (pusRegHoldingBuf[0])
    {
			
		//���������̵�
		case 1:
			pusRegHoldingBuf[0] = 98;
		  //pusRegHoldingBuf[10] = 0;	//���̿�������
			pusRegHoldingBuf[34] = 1;	//����������ȥ���������˳��M1 M2 M3 M4 M5 M6 M7
			pusRegHoldingBuf[6] = 0;	//M1��λ
			pusRegHoldingBuf[2] = 0;	//M2��λ
			pusRegHoldingBuf[4] = 0;	//M3M4��λ
			//pusRegHoldingBuf[12] = 0;	//M5M6M7��λ
			pusRegHoldingBuf[20] = 0;	//M5��λ
			pusRegHoldingBuf[22] = 0;	//M6��λ
			pusRegHoldingBuf[24] = 0;	//M7��λ
			  
			plc_button_run();
		//����������
			plc_switch_set_flow(0,\
				0, 0, 0, 0, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
		//��Ʋ���
			plc_switch_set_flow(1,\
				0, 0, 0, 0, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
		//�ƵƲ���	
			plc_switch_set_flow(2,\
				0, 0, 0, 0, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
		//�̵���		
			plc_switch_set_flow(3,\
				1, 0, 0, 0, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
			pusRegHoldingBuf[0] = 99;
			pusRegHoldingBuf[14] = 1;		//֪ͨ��λ���豸�������Ѱ���
			break;
				
		//ֹͣ�������	
		case 2:
			pusRegHoldingBuf[0] = 98;
			plc_button_stop();
		//�Ƶ���
			plc_switch_set_flow(2,\
				1,0,0, 0, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
		//��������
			plc_switch_set_flow(0,\
				1, 0, 0, 0, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
		//��Ʋ���	
			plc_switch_set_flow(1,\
				0, 0, 0, 0, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
		//�̵Ʋ���	
			plc_switch_set_flow(3,\
				0, 0, 0, 0, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);				
			break;
				
		//�������������Ƶ�
		case 3:
			pusRegHoldingBuf[0] = 97;
			//��������
			plc_switch_set_flow(0,\
				1, 0, 0, 0, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
			//��Ʋ���	
			plc_switch_set_flow(1,\
				0,  0, 0, 0, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
			//�Ƶ���	
			plc_switch_set_flow(2,\
				1, 0, 0, 0, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
			//�̵Ʋ���		
			plc_switch_set_flow(3,\
				0, 0, 0, 0, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
			break;
	//�������������̵�
		case 4:
			pusRegHoldingBuf[0] = 99;
		  //pusRegHoldingBuf[10] = 0;	//���̿�������
			pusRegHoldingBuf[34] = 1;	//����������ȥ���������˳��M1 M2 M3 M4 M5 M6 M7
			pusRegHoldingBuf[6] = 0;	//M1��λ
			pusRegHoldingBuf[2] = 0;	//M2��λ
			pusRegHoldingBuf[4] = 0;	//M3M4��λ
			//pusRegHoldingBuf[12] = 0;	//M5M6M7��λ
			pusRegHoldingBuf[20] = 0;	//M5��λ
			pusRegHoldingBuf[22] = 0;	//M6��λ
			pusRegHoldingBuf[24] = 0;	//M7��λ
			  
			plc_button_run();
			//����������
			plc_switch_set_flow(0,\
				0, 0, 0, 0, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
			//��Ʋ���
			plc_switch_set_flow(1,\
				0, 0, 0, 0, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
			//�ƵƲ���		
			plc_switch_set_flow(2,\
				0, 0, 0, 0, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
			//�̵���		
			plc_switch_set_flow(3,\
				1, 0, 0, 0, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
			pusRegHoldingBuf[14] = 1;		//֪ͨ��λ���豸�������Ѱ���
			break;
			
		//����ϵļ������������̵�
		case 5:
			pusRegHoldingBuf[0] = 99;	//99����Ż���
			//plc_button_run();
		//����������
			plc_switch_set_flow(0,\
				0, 0, 0, 0, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
		//�ƵƲ���
			plc_switch_set_flow(2,\
				0,0,0, 0, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
		//��Ʋ���	
			plc_switch_set_flow(1,\
				0, 0, 0, 0, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
		//�̵���
			plc_switch_set_flow(3,\
				1, 0, 0, 0, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);	
			break;
		default:
			break;				
	}
		
	switch(pusRegHoldingBuf[10])
	{
		//�������̿�������
		case 0:
			pusRegHoldingBuf[10] = 99;
			
			plc_switch_set_flow(8,\
				1, 0, 500, 500, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
				break;
			
		//����뻷��24V����
		case 1:
			pusRegHoldingBuf[10] = 98;
				
			plc_switch_set_flow(11,\
				1, 0, 10, 10, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
			
			plc_switch_set_flow(10,\
					1, 0, 10, 10, 1,\
					-1, 0,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
			
			m1position = 1;
			dis = plc_motor_cur_postion(0);
			dis1 = dis / 256;
			dis2 = dis % 256;
			pusRegHoldingBuf[8] = dis1;
			pusRegHoldingBuf[16] = dis2;
			pusRegHoldingBuf[10] = 99;
			pusRegHoldingBuf[26] = 1;		//��λ����ʼ��ȡ��Ƭ
			break;
			
		//���ƴ�Ʒ����(����)
		case 2:
			pusRegHoldingBuf[10] = 99;
			
			plc_switch_set_flow(8,\
				0, 0, 0, 0, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
			break;
			
		//���ƴ�Ʒ����(����)
		case 3:
			pusRegHoldingBuf[10] = 99;
			
			plc_switch_set_flow(8,\
				1, 0, 0, 0, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
			break;
			
		//�����ͷ�
		case 4:
			pusRegHoldingBuf[10] = 99;
			
			plc_switch_set_flow(4,\
					1, 0, 0, 0, 1,\
					-1, 0,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
			break;
			
		//���̶̹�
		case 5:
			pusRegHoldingBuf[10] = 99;
			
			plc_switch_set_flow(4,\
					0, 0, 0, 0, 1,\
					-1, 0,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
			break;
			
		//������ⴥ��
		case 6:
			pusRegHoldingBuf[10] = 98;

			plc_switch_set_flow(12,\
					1, 0, 10, 10, 1,\
					-1, 0,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
			
			plc_switch_set_flow(10,\
					1, 0, 10, 10, 1,\
					-1, 0,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
			m1position = 1;
			dis = plc_motor_cur_postion(0);
			dis1 = dis / 256;
			dis2 = dis % 256;
			pusRegHoldingBuf[8] = dis1;
			pusRegHoldingBuf[16] = dis2;
			pusRegHoldingBuf[10] = 99;
			pusRegHoldingBuf[26] = 1;		//��λ����ʼ��ȡ��Ƭ
			break;
			
		case 7:
			pusRegHoldingBuf[10] = 99;
			m1position = 0;
			break;
			
		//��������뻷��24V����
		case 8:
			pusRegHoldingBuf[10] = 98;
				
			plc_switch_set_flow(14,\
				1, 0, 10, 10, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
			
			plc_switch_set_flow(13,\
					1, 0, 10, 10, 1,\
					-1, 0,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
			
			m1position = 1;
			dis = plc_motor_cur_postion(0);
			dis1 = dis / 256;
			dis2 = dis % 256;
			pusRegHoldingBuf[8] = dis1;
			pusRegHoldingBuf[16] = dis2;
			pusRegHoldingBuf[10] = 99;
			pusRegHoldingBuf[26] = 1;		//��λ����ʼ��ȡ��Ƭ
			break;
		//����������ⴥ��
		case 9:
			pusRegHoldingBuf[10] = 98;

			plc_switch_set_flow(15,\
					1, 0, 10, 10, 1,\
					-1, 0,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
			
			plc_switch_set_flow(13,\
					1, 0, 10, 10, 1,\
					-1, 0,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);
			m1position = 1;
			dis = plc_motor_cur_postion(0);
			dis1 = dis / 256;
			dis2 = dis % 256;
			pusRegHoldingBuf[8] = dis1;
			pusRegHoldingBuf[16] = dis2;
			pusRegHoldingBuf[10] = 99;
			pusRegHoldingBuf[26] = 1;		//��λ����ʼ��ȡ��Ƭ
			break;
		//è�������Դ����
		/*
		case 10:
			pusRegHoldingBuf[10] = 98;
			//plc_button_stop();
		//����
			plc_switch_set_flow(6,\
				1,0,0, 0, 1,\
				-1, 0,\
				CONFIG_PLC_FEEDBACK_FLOW, 0,\
				g_plc_flow_test_switch_port.node_buf, &g_plc_flow_test_switch_port.node_pos, CONFIG_PLC_TEST_SWITCH_FLOW_NODE_BUF_MASK);

			break;
		*/

		default:
			break;
	}
		
	if (m1position == 0)  //Ϊ0ʱ��ʾʵʱ����
	{
		int dis = plc_motor_cur_postion(0);
		int dis1 = dis / 256;
		int dis2 = dis % 256;
		pusRegHoldingBuf[8] = dis1;
		pusRegHoldingBuf[16] = dis2;
	}
		

}
	
void plc_flow_test_scan(void)
{
	plc_flow_test_switch_scan();
	if(pusRegHoldingBuf[0]==99)//��ʵ�õ�switch��ʹ�õĸ��ø�����
		plc_flow_test_motor_scan();		
}
	
#endif
/***************************************************************************
*
*�ŷ����1  ��ַ40004
* 0����λ   1������6.6mm   2��ǰ��6.6     5������1mm    6��ǰ��1mm     (��ʼֵ�����ڵ�һ����׼λ����)
* 7����λ��ǰ���ĵ�1��׼ֵ���ڼ��ʱ��ʹ��
* 9����λ��ǰ���ĵڶ���׼ֵ�����õ���ȡ��Ʒʱʹ��
* 3������34.4mm       4��ǰ��34.4mm    ����ʼֵ���ڵڶ�����׼λ���ϣ�λ�û�δȷ����
* 9-30���ӵĹ��ܣ����˸�����ٶȸ�λ������  

*****************************************************************************/
void motor_astaone()
{	
	uint8_t id = 0;
		
	// �ϵ����
	for(id = 0;  id < CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_SIZE; id++)
	{
		if(g_plc_flow_test_motor_port.node_buf[id].stat == TRUE)
			return;
	}
		
	switch(pusRegHoldingBuf[6])
  {
		//M1���λ
		case 0:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[6] = 98;
				if(plc_input_read_flow(7)!=0)
				{
					//�����ټӱ���������λ,,,����ʹ��E8
					plc_motor_set_step_rst_flow(0, 0,\
						500, 100, 0.5,\
						15, 0, 0, -1,\
						CONFIG_PLC_FEEDBACK_FLOW, 0,\
						g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
					
					flag_one = 0;
					//pusRegHoldingBuf[6] = 10;
					//pusRegHoldingBuf[6] = 99;
				}		
			}
			
			break;
		
		//����462����			
		case 1:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[6] = 98;
		
				if(plc_input_read_flow(15) != 0)
				{
					//������λE16
					flag_one = flag_one - 15060;//����ֵ
					plc_motor_set_step_pos_flow(0, flag_one,\
						2000, 500, 4,\
						CONFIG_PLC_FEEDBACK_FLOW, 0,\
						g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
				}
				pusRegHoldingBuf[30] = 1;		//��ʼһ������
				pusRegHoldingBuf[6] = 99;
				
				if(pusRegHoldingBuf[36] == 0)
				{
					if(pusRegHoldingBuf[28] == 1)
						pusRegHoldingBuf[10] = 1;	//�ϲ������������
					else
						pusRegHoldingBuf[10] = 6;	//�ϲ�����������
				}
				else 
				{
					if(pusRegHoldingBuf[28] == 1)
						pusRegHoldingBuf[10] = 8;	//�²������������
					else
						pusRegHoldingBuf[10] = 9;	//�²�����������
				}
			}
			break;
		
		//ǰ��462����
		case 2:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[6] = 98;	
		
				if(plc_input_read_flow(14) != 0)
				{
					//ǰ����λE15
					flag_one = flag_one + 15060;//����ֵ
					plc_motor_set_step_pos_flow(0, flag_one,\
						2000, 500, 4,\
						CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
				}		
				pusRegHoldingBuf[30] = 1;		//��ʼһ������
				pusRegHoldingBuf[6] = 99;
				
				if(pusRegHoldingBuf[36] == 0)
				{
					if(pusRegHoldingBuf[28] == 1)
						pusRegHoldingBuf[10] = 1;	//�ϲ������������
					else
						pusRegHoldingBuf[10] = 6;	//�ϲ�����������
				}
				else 
				{
					if(pusRegHoldingBuf[28] == 1)
						pusRegHoldingBuf[10] = 8;	//�²������������
					else
						pusRegHoldingBuf[10] = 9;	//�²�����������
				}
			}
			break;
			
		
		
		//��һ��׼ֵ
		case 7:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[6] = 98;
			
				if(plc_input_read_flow(14) != 0)			
				{
					//ǰ����λE15
					plc_motor_set_step_pos_flow(0, 5550,\
					2000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
					flag_one = 5550;
				}
				
				//pusRegHoldingBuf[6] = 99;
			}
			break;
			
		//�ڶ���׼ֵ
		case 9:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[6] = 98;
			
				if((plc_input_read_flow(14) != 0))			
				{
					//ǰ����λE15
					flag_one = 348;
					plc_motor_set_step_pos_flow(0, flag_one,\
					2000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
				}
				
				//pusRegHoldingBuf[6] = 99;
			}
			break;
		
		//�����תͨ��
		case 20:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[6] = 98;	
		
				if(plc_input_read_flow(14) != 0)	//û�����򵼹��Ҳ���λ���أ�M1����ǰ��
				{
					
					//ǰ����λE15
					flag_one = pusRegHoldingBuf[18]*256 + pusRegHoldingBuf[32];//����ֵ
					plc_motor_set_step_pos_flow(0, flag_one,\
						10000, 100, 0.1,\
						CONFIG_PLC_FEEDBACK_FLOW, 0,\
						g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
				}		
				
				//pusRegHoldingBuf[6] = 99;	
			}
			break;
			
		default:
			pusRegHoldingBuf[6] = 99;
			break;
			
	}
}	
	
/***************************************************************************
*
*�ŷ����2  ��ַ40002
* 0���Ҷ˸�λ   1������6mm   2������6mm     3����˸�λ    4������59mm      5������59mm
* ��40008д����ֵ
*****************************************************************************/
void motor_astatwo()
{
	uint8_t id = 0;
		
	// �ϵ����
	for(id = 0;  id < CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_SIZE; id++)
	{
		if(g_plc_flow_test_motor_port.node_buf[id].stat == TRUE)
			return;
	}
	
	switch (pusRegHoldingBuf[2])
	{
		case 0:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[2] = 98;
				if(plc_input_read_flow(0) != 0)
				{
					
					//��λE1
					plc_motor_set_step_rst_flow(1, 0,\
					64000, 15000, 0.4,\
					0, 0, 0, -1,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
				}
				flag_two = 0;
				//pusRegHoldingBuf[2] = 99;	//��λ��ƽ����׼ֵ
			}
			break;
			
		//����59����
		case 4:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[2] = 98;
			
				if(plc_input_read_flow(1) != 0)//E2��λ
				{
					flag_two = flag_two + 59000;
					plc_motor_set_step_pos_flow(1, flag_two,\
					64000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);		
				}
				
				//pusRegHoldingBuf[2] = 99;
			}
			break;
		
		//����59����
		case 5:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[2] = 98;
			
				if(plc_input_read_flow(0) != 0 && flag_two - 59000 > 0)//E1��λ
				{
					flag_two = flag_two - 59000;
					
					plc_motor_set_step_pos_flow(1, flag_two,\
					64000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
						
				}
			
				//pusRegHoldingBuf[2] = 99;
			}
			break;
		
		//����295mm
		case 6:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[2] = 98;
			
				if(plc_input_read_flow(1) != 0 && flag_two == 0)//E1��λ
				{
					flag_two = flag_two + 295000;
					
					plc_motor_set_step_pos_flow(1, flag_two,\
					64000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
						
				}
				
				//pusRegHoldingBuf[2] = 99;
			}
			break;
		
		//����295mm
		case 7:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[2] = 98;
			
				if(plc_input_read_flow(0) != 0 && flag_two == 295000)//E1��λ
				{
					flag_two = flag_two - 295000;
					
					plc_motor_set_step_pos_flow(1, flag_two,\
					64000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
						
				}
				
				//pusRegHoldingBuf[2] = 99;
			}
			break;
			
		
		//ͨ����λ������
		case 20:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[2] = 98;
			
				if(plc_input_read_flow(1) != 0)//E1��λ
				{
					flag_two = (pusRegHoldingBuf[18]*256 + pusRegHoldingBuf[32])*10;
					
					plc_motor_set_step_pos_flow(1, flag_two,\
					25000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
						
				}
				
				//pusRegHoldingBuf[2] = 99;
			}
			break;
		default:
			break;
		
	}
}

/***************************************************************************
*
* �������3��4  ��ַ40003
* 0����λ   1������0.1mm   2������1mm     3������0.1mm    4������1mm    
* 5�������ƶ���׼ֵ
*
*****************************************************************************/

void motor_steptf()
{

	uint8_t id = 0;
		
		// �ϵ����
	for(id = 0;  id < CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_SIZE; id++)
	{
		if(g_plc_flow_test_motor_port.node_buf[id].stat == TRUE)
			return;
	}
	
	switch(pusRegHoldingBuf[4])
	{
		//���¸�λ
		case 0:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[4] = 98;
			
				if(plc_input_read_flow(2) != 0)//E3����
				{
					if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
					{
						plc_motor_set_step_rst_flow(2, 0,\
						8000, 500, 0.1,\
						2, 0, 0, -1,\
						CONFIG_PLC_FEEDBACK_FLOW, 0,\
						g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
					}	
					
					if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
					{
						plc_motor_set_step_rst_flow(3, 0,\
						8000, 500, 0.1,\
						2, 0, 0, -1,\
						CONFIG_PLC_FEEDBACK_FLOW, 0,\
						g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);	
						
					}
					flag_tf = 0;
				}
				
				//pusRegHoldingBuf[4] = 99;	
			}
			break;
		
		//����1����
		case 2:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[4] = 98;
			
				if(plc_input_read_flow(2) != 0)//����E3����
				{
					flag_tf = flag_tf - 400;
					plc_motor_set_step_pos_flow(2, flag_tf,\
						8000, 500, 0.4,\
						CONFIG_PLC_FEEDBACK_FLOW, 0,\
						g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
						
					plc_motor_set_step_pos_flow(3, flag_tf,\
						8000, 500, 0.4,\
						CONFIG_PLC_FEEDBACK_FLOW, 0,\
						g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);		
				
				}
				
				//pusRegHoldingBuf[4] = 99;
			}
			break;
			
		//����1����
		case 4:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[4] = 98;
			
				if(plc_input_read_flow(3) != 0)//����E4��λ����
				{
					flag_tf = flag_tf + 400;
					
					plc_motor_set_step_pos_flow(2, flag_tf,\
					8000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
					
					plc_motor_set_step_pos_flow(3, flag_tf,\
					8000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
				}
				
				//pusRegHoldingBuf[4] = 99;
			}
			break;
			
		//��λ��ƽ����׼ֵ
		case 5:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[4] = 98;
			
				if(plc_input_read_flow(3) != 0)
				{
					flag_tf = 26520;
					plc_motor_set_step_pos_flow(2, flag_tf,\
					8000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
					
					plc_motor_set_step_pos_flow(3, flag_tf,\
					8000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
				}
				
				//pusRegHoldingBuf[4] = 99;
			}
			break;
			
		case 6:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[4] = 98;
			
				if(plc_input_read_flow(3) != 0)
				{
					flag_tf = 26520;
					plc_motor_set_step_pos_flow(2, flag_tf,\
					8000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
					
					plc_motor_set_step_pos_flow(3, flag_tf,\
					8000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
				}
				
				//pusRegHoldingBuf[4] = 99;
			}
			break;
			
		case 20:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[4] = 98;
			
				if(plc_input_read_flow(3) != 0)
				{
					flag_tf = (pusRegHoldingBuf[18]*256 + pusRegHoldingBuf[32]);
					
					plc_motor_set_step_pos_flow(2, flag_tf,\
					12000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
					
					plc_motor_set_step_pos_flow(3, flag_tf,\
					12000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
				}
				
				//pusRegHoldingBuf[4] = 99;
			}
			break;
		default:
			break;
	}
}


/***************************************************************************
*
* �������5  ��ַ4000b
* 0����λ   
*
*****************************************************************************/
void motor_step_waste_M5()
{
	uint8_t id = 0;
	
	for(id = 0;  id < CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_SIZE; id++)
	{
		if(g_plc_flow_test_motor_port.node_buf[id].stat == TRUE)
			return;
	}
	
	switch(pusRegHoldingBuf[20])
	{
		case 0:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[20] = 98;		//��ʼ��4000b����
				//���M5��λ
				if(plc_input_read_flow(10) != 0)	//û����λ���أ�������λ
				{
					//�ٶ�64000��Ϊ500�����㰲ȫ
					plc_motor_set_step_rst_flow(4, 0,\
						5000, 500, 0.4,\
						10, 0, 0,1,\
						CONFIG_PLC_FEEDBACK_FLOW, 0,\
						g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
					
				}
				flag_motor_five = 0;
				//pusRegHoldingBuf[20] = 99;		//��4000b��������
			}
			break;
		//ͨ����λ������
		case 20:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[20] = 98;
			
				if(plc_input_read_flow(11) != 0)//E1��λ
				{
					flag_motor_five = 0 - ((pusRegHoldingBuf[18]*256 + pusRegHoldingBuf[32])*10);
					 
					plc_motor_set_step_pos_flow(4, flag_motor_five,\
						49000, 100, 0.2,\
						CONFIG_PLC_FEEDBACK_FLOW, 0,\
						g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
						
				}
				
				//pusRegHoldingBuf[20] = 99;
			}
			break;
		default:
			break;
	}
}

/***************************************************************************
*
* �������6  ��ַ4000c
* 0����λ   
*
*****************************************************************************/
void motor_step_waste_M6()
{
	uint8_t id = 0;
	
	for(id = 0;  id < CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_SIZE; id++)
	{
		if(g_plc_flow_test_motor_port.node_buf[id].stat == TRUE)
			return;
	}
	
	switch(pusRegHoldingBuf[22])
	{
		case 0:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[22] = 98;		//��ʼ��4000c����
				//���M5��λ
				if(plc_input_read_flow(8) != 0)		//û����λ���أ�������λ
				{
					//�ٶ�8000��Ϊ500�����㰲ȫ
					plc_motor_set_step_rst_flow(5, 0,\
						3000, 500, 0.4,\
						8, 0, 0,1,\
						CONFIG_PLC_FEEDBACK_FLOW, 0,\
						g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
						flag_motor_six = 0;
				}
				//pusRegHoldingBuf[22] = 99;		//��4000c��������
			}
			break;
		//ͨ����λ������
		case 20:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[22] = 98;
			
				if(plc_input_read_flow(9) != 0)//E1��λ
				{
					flag_motor_six = 0 - ((pusRegHoldingBuf[18]*256 + pusRegHoldingBuf[32])*10);
					
					plc_motor_set_step_pos_flow(5, flag_motor_six,\
						45000, 500, 0.4,\
						CONFIG_PLC_FEEDBACK_FLOW, 0,\
						g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
						
				}
				
				//pusRegHoldingBuf[22] = 99;
			}
			break;	
		default:
			break;
	}
}

/***************************************************************************
*
* �������7  ��ַ4000d
* 0����λ   
*
*****************************************************************************/
void motor_step_waste_M7()
{
	uint8_t id = 0;
	
	for(id = 0;  id < CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_SIZE; id++)
	{
		if(g_plc_flow_test_motor_port.node_buf[id].stat == TRUE)
			return;
	}
	
	switch(pusRegHoldingBuf[24])
	{
		case 0:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[24] = 98;		//��ʼ��4000d����
				//���M5��λ
				if(plc_input_read_flow(12) != 0)	//û����λ���أ�������λ
				{
					//�ٶ�8000��Ϊ500�����㰲ȫ
					plc_motor_set_step_rst_flow(6, 0,\
						6000, 500, 0.4,\
						12, 0, 0,1,\
						CONFIG_PLC_FEEDBACK_FLOW, 0,\
						g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
					
				}
				flag_motor_seven = 0;
				//pusRegHoldingBuf[24] = 99;		//��4000d��������
			}
			break;
		//ͨ����λ������
		case 20:
			if(plc_motor_cur_status(g_plc_flow_test_motor_port.id) == 0)
			{
				pusRegHoldingBuf[24] = 98;
			
				if(plc_input_read_flow(13) != 0)//E1��λ
				{
					flag_motor_seven = 0 - ((pusRegHoldingBuf[18]*256 + pusRegHoldingBuf[32])*10);
					
					plc_motor_set_step_pos_flow(6, flag_motor_seven,\
					25000, 500, 0.4,\
					CONFIG_PLC_FEEDBACK_FLOW, 0,\
					g_plc_flow_test_motor_port.node_buf, &g_plc_flow_test_motor_port.node_pos, CONFIG_PLC_TEST_MOTOR_FLOW_NODE_BUF_MASK);
						
				}
				
				//pusRegHoldingBuf[24] = 99;
			}
			break;	
		default:
			break;
	}
}

void button_con()
{
	if(plc_input_read(4) == 0)//��������
	{
		pusRegHoldingBuf[0] = 1;
	}
	
	if(plc_input_read(5) == 0)//ֹͣ����
	{
		pusRegHoldingBuf[0] = 2;
	}
	
	if(plc_input_read(6) == 0)//ֹͣ����
	{
		pusRegHoldingBuf[0] = 3;
	}
}

/*
                   _ooOoo_
                  o8888888o
                  88" . "88
                  (| -_- |)
                  O\  =  /O
               ____/`---'\____
             .'  \\|     |//  `.
            /  \\|||  :  |||//  \
           /  _||||| -:- |||||-  \
           |   | \\\  -  /// |   |
           | \_|  ''\---/''  |   |
           \  .-\__  `-`  ___/-. /
         ___`. .'  /--.--\  `. . __
      ."" '<  `.___\_<|>_/___.'  >'"".
     | | :  `- \`.;`\ _ /`;.`/ - ` : | |
     \  \ `-.   \_ __\ /__ _/   .-` /  /
======`-.____`-.___\_____/___.-`____.-'======
*/
