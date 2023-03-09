#ifndef __ETH_CONFIG_H__
#define __ETH_CONFIG_H__

#include <stdint.h>

#ifndef TRUE
	#define TRUE	1
#endif

#ifndef FALSE
	#define FALSE	0
#endif

typedef enum
{ 
  socket_mode_tcp_server = 0x00,	//TCP������ģʽ
  socket_mode_tcp_client = 0x01,	//TCP�ͻ���ģʽ 
  socket_mode_udp        = 0x02	//UDP(�㲥)ģʽ 
}socket_mode;

typedef enum
{ 
	socket_start_ready      = 0x00,
	socket_start_init_ok	  = 0x01,	//�˿���ɳ�ʼ�� 
	socket_start_connect_ok = 0x02	//�˿��������,���������������� 
}socket_start;

typedef enum
{
	socket_state_ready 	  = 0x00,			
	socket_state_rx_flag  = 0x01,	//�˿ڽ��յ�һ�����ݰ� 
	socket_state_tx_finsh = 0x02	//�˿ڷ���һ�����ݰ���� 
}socket_state;
	
typedef struct
{
	//���������������
	uint8_t gateway[4];//����IP��ַ 
	uint8_t submask[4];	//�������� 
	uint8_t phyaddr[6];	//�����ַ(MAC) 
	
	uint8_t localip[4];	//����IP��ַ 
}ETH_LOCAL_CONFIG_TypeDef;

typedef struct
{
	uint8_t  *tx_buff;
	uint16_t  tx_cnt;
	uint16_t  tx_pro;
	uint16_t  tx_mask;	
}socketTxd;

typedef struct
{
	uint8_t  *rx_buff;
	uint16_t  rx_cnt;	
	uint16_t  rx_pro;
	uint16_t  rx_mask;	
	
	uint8_t   rx_event;
}socketRxd;

typedef struct
{
	//���������������
	uint8_t local_port[2];		//�����˿ں�(5000)
	
	uint8_t tcp_dst_ip[4];		//TCPĿ�ĵ�IP��ַ
	uint8_t tcp_dst_port[2];	//TCPĿ�ĵض˿ں�(6000)
	
	uint8_t udp_dst_ip[4];		//UDP(�㲥)ģʽ,Ŀ������IP��ַ
	uint8_t udp_dst_port[2];	//UDP(�㲥)ģʽ,Ŀ�������˿ں�
	
	//�˿ڵ�����ģʽ(0 = TCP������ģʽ��1 = TCP�ͻ���ģʽ�� 2 = UDP(�㲥)ģʽ)
	uint8_t mode;	
}ETH_REMOTE_CONFIG_TypeDef;

typedef struct
{
	uint8_t  enable;
	uint8_t  irq_flag;
  uint8_t	 irq_id;
	
	ETH_LOCAL_CONFIG_TypeDef  localConfig;
	
	ETH_REMOTE_CONFIG_TypeDef remoteConfig[8];
}ETH_SOCKET_TypeDef;

extern ETH_SOCKET_TypeDef ETH_SOCKET_Struct;

/* mqtt publish*/
typedef struct
{
	int8_t topic[16];
	int8_t message[32];
} ETH_MQTT_PUBLISH_TypeDef;

/* Local Var */
extern const uint8_t  ETH_CONFIG_LOCAL_GATEWAY[4];
extern const uint8_t  ETH_CONFIG_LOCAL_SUBMASK[4];
extern const uint8_t  ETH_CONFIG_LOCAL_PHYADDR[6];
extern const uint8_t  ETH_CONFIG_LOCAL_IP[4];
	
/* Modbus TCP */
extern const uint8_t  ETH_CONFIG_MODBUS_SOCKET;
extern const uint8_t  ETH_CONFIG_MODBUS_IP[4];
extern const uint16_t ETH_CONFIG_MODBUS_REMOTE_PORT;
extern const uint16_t ETH_CONFIG_MODBUS_LOCAL_PORT;

/* MQTT TCP */
extern const uint8_t  ETH_CONFIG_MQTT_SOCKET;
extern const uint8_t  ETH_CONFIG_MQTT_IP[4];
extern const uint16_t ETH_CONFIG_MQTT_REMOTE_PORT;
extern const uint16_t ETH_CONFIG_MQTT_LOCAL_PORT;

#endif
