#include "mb_tcp.h"
#include "mb_serial.h"
#include "mb_timer.h"

#include "eth_user.h"
#include "eth_socket.h"
#include "eth_w5500.h"
#include "eth_type.h"

#define MB_TCP_TID          	0
#define MB_TCP_PID          	2
#define MB_TCP_LEN          	4
#define MB_TCP_UID          	6
#define MB_TCP_FUNC         	7

#define MB_TCP_PROTOCOL_ID  	0   /* 0 = Modbus Protocol */

static uint16_t usTID, usPID;

typedef enum{
	CONFIG_ETH_USER_STEP_IR = 0,
	CONFIG_ETH_USER_STEP_LISTEN,
	CONFIG_ETH_USER_STEP_ESTABLISHED,
	CONFIG_ETH_USER_STEP_SR,
}ETH_USER_STEP;

typedef struct
{ 
	uint8_t step;
	
	uint8_t status;
	
	uint8_t boot;
 
	uint32_t time_tick;
}ETH_USER_PORT;

ETH_USER_PORT g_eth_user_port;

/*
*****************************************************************
* ��ʼ��
*****************************************************************
*/
eMBErrorCode mb_tcp_init(uint8_t id, uint8_t ucSlaveAddress, uint16_t ucPort)
{
	MB_VAR_PORT *p_mb_var_port = g_mb_var_port + id;
	eMBErrorCode    eStatus = MB_ENOERR;
	
	// Ӳ���˿ڳ�ʼ��
	eth_user_init(ucPort);
	
	// �ӻ���ַ��δʹ�õ���
	p_mb_var_port->address = ucSlaveAddress;
	
	// ����������־λ
	g_eth_user_port.boot = TRUE;
	
	return eStatus;
}

/*
*****************************************************************
* ��������
*****************************************************************
*/
void mb_tcp_receive(uint8_t id, uint8_t context)
{
	MB_VAR_PORT *p_mb_var_port = g_mb_var_port + id;
	uint8_t buf[CONFIG_MB_RXD_BUFFER_SIZE];
	uint8_t len, i, flag = TRUE;
	
	while(1){
		len = ETH_SOCKET_ReadBuffer(ETH_CONFIG_MODBUS_SOCKET, buf, CONFIG_MB_RXD_BUFFER_SIZE);
		if(len >= CONFIG_MB_RXD_BUFFER_SIZE){
			flag = FALSE;
		}else{
			break;
		}
	}
	
	if(len == 0 || flag == FALSE){
		return;
	}
	
	for(i = 0; i < len; i++){
		p_mb_var_port->rxdbuffer[p_mb_var_port->rxdbufpos] = buf[i];
		p_mb_var_port->rxdbufpos = CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->rxdbufpos, 1);
	}
	
	mb_serial_rxd_frame_insert(p_mb_var_port);
}

/*
*****************************************************************
* ����֡����
*****************************************************************
*/
uint8_t mb_tcp_snd_frame(uint8_t id)
{
	MB_VAR_PORT *p_mb_var_port = g_mb_var_port + id;
	
	//�������ж�
	if(p_mb_var_port->txdframecnt == p_mb_var_port->txdframepos)
		return FALSE;
	
	// ���ͼ��
	if(mb_timer_snd_outtime(p_mb_var_port) == FALSE)
		return FALSE;
	
	//��������
	mb_serial_send(p_mb_var_port);
	
	return TRUE;
}

/*
*****************************************************************
* ֡�ж�
*****************************************************************
*/
uint8_t mb_tcp_rev_adjust(uint8_t id)
{
	MB_VAR_PORT *p_mb_var_port = g_mb_var_port + id;
	
	//������֡�ж�
	if(p_mb_var_port->rxdframecnt != p_mb_var_port->rxdframepos)
		return TRUE;
	else 
		return FALSE;
}

/*
*****************************************************************
* ����֡����
*****************************************************************
*/
void mb_tcp_frame_conform(uint8_t id)
{
	MB_VAR_PORT *p_mb_var_port = g_mb_var_port + id;
	
	p_mb_var_port->txdbuffer[CONFIG_MB_TXD_BUFFER_POS(p_mb_var_port->cur_txd_frame_pos, MB_TCP_TID)]     = usTID >> 8U;
	p_mb_var_port->txdbuffer[CONFIG_MB_TXD_BUFFER_POS(p_mb_var_port->cur_txd_frame_pos, MB_TCP_TID + 1)] = usTID & 0xFF;
	
	p_mb_var_port->txdbuffer[CONFIG_MB_TXD_BUFFER_POS(p_mb_var_port->cur_txd_frame_pos, MB_TCP_PID)]     = usPID >> 8U;
	p_mb_var_port->txdbuffer[CONFIG_MB_TXD_BUFFER_POS(p_mb_var_port->cur_txd_frame_pos, MB_TCP_PID + 1)] = usPID & 0xFF;

	p_mb_var_port->txdbuffer[CONFIG_MB_TXD_BUFFER_POS(p_mb_var_port->cur_txd_frame_pos, MB_TCP_LEN)]     = ( p_mb_var_port->cur_txd_frame_len + 1 ) >> 8U;
	p_mb_var_port->txdbuffer[CONFIG_MB_TXD_BUFFER_POS(p_mb_var_port->cur_txd_frame_pos, MB_TCP_LEN + 1)] = ( p_mb_var_port->cur_txd_frame_len + 1 ) & 0xFF;

	p_mb_var_port->txdbuffer[CONFIG_MB_TXD_BUFFER_POS(p_mb_var_port->cur_txd_frame_pos, MB_TCP_UID)] = p_mb_var_port->address;
	
	// ����һ֡���
	mb_serial_txd_frame_insert(p_mb_var_port);
}

/*
*****************************************************************
* ֡�ֶΣ��ӵ�ַ + ֡���� + ֡���ȣ�
*****************************************************************
*/
uint8_t mb_tcp_segmentation( uint8_t id, uint8_t *cur_frame_address, uint8_t *cur_frame_pos, uint16_t *cur_frame_len )
{
	MB_VAR_PORT *p_mb_var_port = g_mb_var_port + id;	
	uint8_t      frame_sta, frame_len;
	eMBErrorCode eStatus = MB_EIO;
	
	//��ȡ֡��Ϣ
	mb_serial_frame_info(p_mb_var_port, &frame_sta, &frame_len);	
	
	usTID  = p_mb_var_port->rxdbuffer[CONFIG_MB_RXD_BUFFER_POS(frame_sta, MB_TCP_TID)] << 8U;
	usTID |= p_mb_var_port->rxdbuffer[CONFIG_MB_RXD_BUFFER_POS(frame_sta, MB_TCP_TID + 1)];
	
	usPID  = p_mb_var_port->rxdbuffer[CONFIG_MB_RXD_BUFFER_POS(frame_sta, MB_TCP_PID)] << 8U;
	usPID |= p_mb_var_port->rxdbuffer[CONFIG_MB_RXD_BUFFER_POS(frame_sta, MB_TCP_PID + 1)];

	if( usPID == MB_TCP_PROTOCOL_ID )
	{
		*cur_frame_pos = CONFIG_MB_RXD_BUFFER_POS(frame_sta, MB_TCP_FUNC);
		*cur_frame_len = frame_len - MB_TCP_FUNC;
		
		eStatus = MB_ENOERR;

		/* Modbus TCP��ʹ���κε�ַ��α��Դ��ַ��ʹ�����ִ����֡��*/
		*cur_frame_address = p_mb_var_port->address;
	}
	else
	{
			eStatus = MB_EIO;
	}
	
	return eStatus;
}

/*
*****************************************************************
* ��ѯɨ�����
*****************************************************************
*/
void mb_tcp_receive_poll(uint8_t id)
{
	switch(g_eth_user_port.step)
	{		
		case CONFIG_ETH_USER_STEP_IR:
			g_eth_user_port.status = ETH_SOCKET_IrStatus(ETH_CONFIG_MODBUS_SOCKET);
			
			if(g_eth_user_port.status & SOCKET_IR_DISCON || g_eth_user_port.boot == TRUE){
				ETH_W5500_BaseRegConfig();	//��������
				ETH_W5500_LocalRegConfig();
				ETH_SOCKET_RemoteRegConfig(ETH_CONFIG_MODBUS_SOCKET);
				g_eth_user_port.step = CONFIG_ETH_USER_STEP_LISTEN;
			}else{
				g_eth_user_port.step = CONFIG_ETH_USER_STEP_SR;
			}
		break;
			
		case CONFIG_ETH_USER_STEP_LISTEN:
			if(g_eth_user_port.time_tick <= HAL_GetTick()){
				if(ETH_SOCKET_TcpListen(ETH_CONFIG_MODBUS_SOCKET) == FALSE){
					g_eth_user_port.time_tick = HAL_GetTick() + 10;
				}else{
					g_eth_user_port.boot = FALSE;
					g_eth_user_port.step = CONFIG_ETH_USER_STEP_ESTABLISHED;
				}
			}
		break;
			
		case CONFIG_ETH_USER_STEP_ESTABLISHED:
			if(ETH_SOCKET_SrStatus(ETH_CONFIG_MODBUS_SOCKET) == SOCK_ESTABLISHED){
				g_eth_user_port.step = CONFIG_ETH_USER_STEP_SR;
			}
		break;
			
		case CONFIG_ETH_USER_STEP_SR:			
			if(g_eth_user_port.status & SOCKET_IR_RECV){
				mb_tcp_receive(id, 0);
			}
			
			g_eth_user_port.step = CONFIG_ETH_USER_STEP_IR;
		break;
	}
}
