#include "eth_user.h"

#include "eth_spi.h"
#include "eth_w5500.h"
#include "eth_socket.h"

/*
********************************************************
* ��ʼ��
********************************************************
*/
void eth_user_init(uint16_t port_id)
{
	uint32_t delay;
	
	// Ӳ����λ
	ETH_SPI_SetRst(LOW);
	delay = 50 + EHT_GET_TICK();
	while(delay >= EHT_GET_TICK());
	
	ETH_SPI_SetRst(HIGH);
	delay = 200 + EHT_GET_TICK();
	while(delay >= EHT_GET_TICK());

	// ��������
	ETH_W5500_LocalVarConfig();
	ETH_SOCKET_RemoteVarConfig(ETH_CONFIG_MODBUS_SOCKET, port_id, (void *)ETH_CONFIG_MODBUS_IP, ETH_CONFIG_MODBUS_REMOTE_PORT);	
}

