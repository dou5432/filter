
#include "eth_spi.h"
#include "eth_w5500.h"
#include "eth_socket.h"
#include "eth_type.h"

/*
******************************************************************************
* ������  :
* ����    : 
* ����    : 
* ���    : 
* ����ֵ  : 
* ˵��    : 
******************************************************************************
*/
void ETH_SOCKET_WriteByte1(SOCKET_TypeDef s, uint16_t reg, uint8_t dat)
{
	//��W5500��SCSΪ�͵�ƽ	
	ETH_SPI_SetCS(LOW);
		
	//ͨ��SPIд16λ�Ĵ�����ַ
	EHT_SPI_WriteByte2(reg);
	
	//ͨ��SPIд�����ֽ�,1���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���
	EHT_SPI_WriteByte1(W5500_RWB_FDM1 | W5500_RWB_WRITE | (s*0x20 + 0x08));
	
	//д1���ֽ�����
	EHT_SPI_WriteByte1(dat);

	//��W5500��SCSΪ�ߵ�ƽ
	ETH_SPI_SetCS(HIGH); 
}

void ETH_SOCKET_WriteByte2(SOCKET_TypeDef s, uint16_t reg, uint16_t dat)
{
	//��W5500��SCSΪ�͵�ƽ
	ETH_SPI_SetCS(LOW);
			
	//ͨ��SPIд16λ�Ĵ�����ַ
	EHT_SPI_WriteByte2(reg);
	
	//ͨ��SPIд�����ֽ�,2���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���
	EHT_SPI_WriteByte1(W5500_RWB_FDM2 | W5500_RWB_WRITE | (s*0x20 + 0x08));
	
	//д16λ����
	EHT_SPI_WriteByte2(dat);

	//��W5500��SCSΪ�ߵ�ƽ
	ETH_SPI_SetCS(HIGH); 
}

void ETH_SOCKET_WriteByte4(SOCKET_TypeDef s, uint16_t reg, uint8_t *dat_ptr)
{
	//��W5500��SCSΪ�͵�ƽ
	ETH_SPI_SetCS(LOW);
			
	//ͨ��SPIд16λ�Ĵ�����ַ
	EHT_SPI_WriteByte2(reg);
	
	//ͨ��SPIд�����ֽ�,4���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���
	EHT_SPI_WriteByte1(W5500_RWB_FDM4 | W5500_RWB_WRITE | (s*0x20+0x08));

	//д������
	EHT_SPI_WriteByte1(*dat_ptr++);//д��1���ֽ�����
	EHT_SPI_WriteByte1(*dat_ptr++);//д��2���ֽ�����
	EHT_SPI_WriteByte1(*dat_ptr++);//д��3���ֽ�����
	EHT_SPI_WriteByte1(*dat_ptr++);//д��4���ֽ�����

	//��W5500��SCSΪ�ߵ�ƽ
	ETH_SPI_SetCS(HIGH); 
}

/*
******************************************************************************
* ������  :
* ����    : 
* ����    : 
* ���    : 
* ����ֵ  : 
* ˵��    : 
******************************************************************************
*/
uint8_t ETH_SOCKET_ReadByte1(SOCKET_TypeDef s, uint16_t reg)
{
	uint8_t value;

	//��W5500��SCSΪ�͵�ƽ
	ETH_SPI_SetCS(LOW);
			
	//ͨ��SPIд16λ�Ĵ�����ַ
	EHT_SPI_WriteByte2(reg);
	
	//ͨ��SPIд�����ֽ�,1���ֽ����ݳ���,������,ѡ��˿�s�ļĴ���
	EHT_SPI_WriteByte1(W5500_RWB_FDM1 | W5500_RWB_READ | (s*0x20 + 0x08));

	//��ȡ����
	value = EHT_SPI_ReadByte1();//��ȡ1���ֽ�����

	//��W5500��SCSΪ�ߵ�ƽ
	ETH_SPI_SetCS(HIGH);
	
	return value;//���ض�ȡ���ļĴ�������
}

uint16_t ETH_SOCKET_ReadByte2(SOCKET_TypeDef s, uint16_t reg)
{
	uint16_t value;

	//��W5500��SCSΪ�͵�ƽ
	ETH_SPI_SetCS(LOW);
			
	//ͨ��SPIд16λ�Ĵ�����ַ
	EHT_SPI_WriteByte2(reg);
	
	//ͨ��SPIд�����ֽ�,2���ֽ����ݳ���,������,ѡ��˿�s�ļĴ���
	EHT_SPI_WriteByte1(W5500_RWB_FDM2 | W5500_RWB_READ | (s*0x20 + 0x08));

	//��ȡ����
	value = EHT_SPI_ReadByte1()<<8;//��ȡ��λ����
	value |= EHT_SPI_ReadByte1();//��ȡ��λ����

	//��W5500��SCSΪ�ߵ�ƽ
	ETH_SPI_SetCS(HIGH);
	
	//���ض�ȡ���ļĴ�������
	return value;
}

/*
******************************************************************************
* ������  :
* ����    : 
* ����    : 
* ���    : 
* ����ֵ  : 
* ˵��    : 
******************************************************************************
*/
uint16_t ETH_SOCKET_ReadBuffer(SOCKET_TypeDef s, uint8_t *dat_ptr, uint8_t size)
{
	uint16_t rx_size, w_rx_addr, i;
	
	//��ȡ���ݳߴ�
	rx_size = ETH_SOCKET_ReadByte2(s, SOCKETn_RX_RSR);
	if(rx_size == 0) 		//û���յ������򷵻�
		return 0;
	if(rx_size > 1460) 
		rx_size = 1460;
		
	if(rx_size >= size)	//���ߴ�
		rx_size = size;
	
	//��ȡw5500���ͻ�����ʼ��ַ
	w_rx_addr  = ETH_SOCKET_ReadByte2(s, SOCKETn_RX_RD) & (S_RX_SIZE - 1);

	//��W5500��SCSΪ�͵�ƽ
	ETH_SPI_SetCS(LOW);

	//д16λ��ַ
	EHT_SPI_WriteByte2(w_rx_addr);
	
	//д�����ֽ�,N���ֽ����ݳ���,������,ѡ��˿�s�ļĴ���
	EHT_SPI_WriteByte1(W5500_RWB_VDM | W5500_RWB_READ | (s*0x20 + 0x18));
	
	//��ȡ����
	if((w_rx_addr + rx_size) < S_RX_SIZE)	{//�������ַδ����W5500���ջ������Ĵ���������ַ
		for(i = 0; i < rx_size; i++){
			*dat_ptr = EHT_SPI_ReadByte1();
			dat_ptr++;
		}
	}else	{//�������ַ����W5500���ջ������Ĵ���������ַ
	
		//ǰ���
		w_rx_addr = S_RX_SIZE - w_rx_addr;
		for(i = 0; i < w_rx_addr; i++){
			*dat_ptr = EHT_SPI_ReadByte1();
			dat_ptr++;
		}
		ETH_SPI_SetCS(HIGH);

		//����
		ETH_SPI_SetCS(LOW);

		EHT_SPI_WriteByte2(0x00);
		EHT_SPI_WriteByte1(W5500_RWB_VDM | W5500_RWB_READ | (s*0x20 + 0x18));

		for(; i < rx_size; i++){
			*dat_ptr = EHT_SPI_ReadByte1();
			dat_ptr++;
		}
	}
	ETH_SPI_SetCS(HIGH);

	//����ʵ�������ַ,���´ζ�ȡ���յ������ݵ���ʼ��ַ
	w_rx_addr = (w_rx_addr + rx_size) & (S_RX_SIZE - 1);
	ETH_SOCKET_WriteByte2(s, SOCKETn_RX_RD, w_rx_addr);
	
	//����������������
	ETH_SOCKET_WriteByte1(s, SOCKETn_CR, SOCKET_RECV);
	
	return rx_size;//���ؽ��յ����ݵĳ���
}

void ETH_SOCKET_ReadByteN(SOCKET_TypeDef s, uint8_t *dat_ptr, uint16_t len)
{
	uint16_t rx_size, w_rx_addr, i;
	uint16_t rx_len = len;
	//��ȡ���ݳߴ�
	rx_size = ETH_SOCKET_ReadByte2(s, SOCKETn_RX_RSR);
	if(rx_size == 0) //û���յ������򷵻�
		return;
	if(rx_size > 1460) 
		rx_size = 1460;

	//��ȡw5500���ͻ�����ʼ��ַ
	w_rx_addr = ETH_SOCKET_ReadByte2(s, SOCKETn_RX_RD) & (S_RX_SIZE - 1);

	//��W5500��SCSΪ�͵�ƽ
	ETH_SPI_SetCS(LOW);

	//д16λ��ַ
	EHT_SPI_WriteByte2(w_rx_addr);
	
	//д�����ֽ�,N���ֽ����ݳ���,������,ѡ��˿�s�ļĴ���
	EHT_SPI_WriteByte1(W5500_RWB_VDM | W5500_RWB_READ | (s*0x20 + 0x18));
	
	//��ȡ����
	if((w_rx_addr + rx_size) < S_RX_SIZE)	{//�������ַδ����W5500���ջ������Ĵ���������ַ
		for(i = 0; i < rx_size && len != 0; i++, len--){
			*dat_ptr = EHT_SPI_ReadByte1();
			dat_ptr++;
		}
		
		if(len <= 0)
			goto exit;
	}else	{//�������ַ����W5500���ջ������Ĵ���������ַ
		//ǰ���
		w_rx_addr = S_RX_SIZE - w_rx_addr;
		for(i = 0; i < w_rx_addr && len != 0; i++, len--){
			*dat_ptr = EHT_SPI_ReadByte1();
			dat_ptr++;
		}
		if(len <= 0)
			goto exit;
		ETH_SPI_SetCS(HIGH);

		//����
		ETH_SPI_SetCS(LOW);

		EHT_SPI_WriteByte2(0x00);
		EHT_SPI_WriteByte1(W5500_RWB_VDM | W5500_RWB_READ | (s*0x20 + 0x18));

		for(; i < rx_size && len != 0; i++, len--){
			*dat_ptr = EHT_SPI_ReadByte1();
			dat_ptr++;
		}
		if(len <= 0)
			goto exit;
	}
	
exit:
	ETH_SPI_SetCS(HIGH);

	//����ʵ�������ַ,���´ζ�ȡ���յ������ݵ���ʼ��ַ
	w_rx_addr = (w_rx_addr + rx_len) & (S_RX_SIZE - 1);
	ETH_SOCKET_WriteByte2(s, SOCKETn_RX_RD, w_rx_addr);
	
	//����������������
	ETH_SOCKET_WriteByte1(s, SOCKETn_CR, SOCKET_RECV);
}

uint16_t ETH_SOCKET_WriteBuffer(SOCKET_TypeDef s, uint8_t *dat_ptr, uint16_t size)
{
	uint16_t i, w_tx_addr, w_tx_mark;

	//�����UDPģʽ,�����ڴ�����Ŀ��������IP�Ͷ˿ں�
	if((ETH_SOCKET_ReadByte1(s, SOCKETn_MR) & 0x0f) != SOCK_UDP){//���Socket��ʧ��	
		ETH_SOCKET_WriteByte4(s, SOCKETn_DIPR,   ETH_SOCKET_Struct.remoteConfig[0].udp_dst_ip);//����Ŀ������IP  		
		ETH_SOCKET_WriteByte2(s, SOCKETn_DPORTR, ETH_SOCKET_Struct.remoteConfig[0].udp_dst_port[0]*256 + ETH_SOCKET_Struct.remoteConfig[0].udp_dst_port[1]);//����Ŀ�������˿ں�				
	}

	//��ȡw5500���ͻ�����ʼ��ַ
	w_tx_mark = w_tx_addr = ETH_SOCKET_ReadByte2(s, SOCKETn_TX_WR);
	w_tx_addr &= (S_TX_SIZE - 1);//����ʵ�ʵ������ַ

	//��W5500��SCSΪ�͵�ƽ
	ETH_SPI_SetCS(LOW);
	
	//д16λ��ַ
	EHT_SPI_WriteByte2(w_tx_addr);
	
	//д�����ֽ�,N���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���
	EHT_SPI_WriteByte1(W5500_RWB_VDM | W5500_RWB_WRITE | (s*0x20 + 0x10));

	//д������
	if((w_tx_addr + size) < S_TX_SIZE)	{//�������ַδ����W5500���ͻ������Ĵ���������ַ
		for(i = 0; i < size; i++)//ѭ��д��size���ֽ�����
			EHT_SPI_WriteByte1(*dat_ptr++);//д��һ���ֽڵ�����		
	}else	{//�������ַ����W5500���ͻ������Ĵ���������ַ
		//ǰ���
		uint16_t len = S_TX_SIZE - w_tx_addr;
		for(i = 0; i < len; i++)//ѭ��д��ǰw_rx_addr���ֽ�����
			EHT_SPI_WriteByte1(*dat_ptr++);//д��һ���ֽڵ�����
		
		ETH_SPI_SetCS(HIGH); //��W5500��SCSΪ�ߵ�ƽ

		//����
		ETH_SPI_SetCS(LOW);//��W5500��SCSΪ�͵�ƽ

		EHT_SPI_WriteByte2(0x00);//д16λ��ַ
		EHT_SPI_WriteByte1(W5500_RWB_VDM | W5500_RWB_WRITE | (s*0x20 + 0x10));//д�����ֽ�,N���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���

		for(; i < size; i++)	{//ѭ��д��size-w_rx_addr���ֽ�����
			EHT_SPI_WriteByte1(*dat_ptr++);//д��һ���ֽڵ�����
		}
	}
	ETH_SPI_SetCS(HIGH); //��W5500��SCSΪ�ߵ�ƽ

	//����ʵ�������ַ,���´�д���������ݵ��������ݻ���������ʼ��ַ
	w_tx_mark += size;
	ETH_SOCKET_WriteByte2(s, SOCKETn_TX_WR, w_tx_mark);
	
	//����������������
	ETH_SOCKET_WriteByte1(s, SOCKETn_CR, SOCKET_SEND);
	
	return size;
}

/*
******************************************************************************
* ������  : 
* ����    : 
* ����    : 
* ���    : 
* ����ֵ  :
* ˵��    :
******************************************************************************
*/
void ETH_SOCKET_RemoteVarConfig(SOCKET_TypeDef s_id, uint16_t l_port/*5000*/,\
	uint8_t *d_ip,/*192.168.1.100*/ uint16_t d_port/*6000*/)
{
	ETH_REMOTE_CONFIG_TypeDef *p_remote_config = &ETH_SOCKET_Struct.remoteConfig[s_id];
	
	//���ض˿�0�Ķ˿ں�5000 
	p_remote_config->local_port[0] = l_port/256; p_remote_config->local_port[1] = l_port%256;
	
	//���ض˿�0��Ŀ��IP��ַ
	p_remote_config->tcp_dst_ip[0] = d_ip[0]; p_remote_config->tcp_dst_ip[1] = d_ip[1]; 
	p_remote_config->tcp_dst_ip[2] = d_ip[2];   p_remote_config->tcp_dst_ip[3] = d_ip[3];
	
	//���ض˿�0��Ŀ�Ķ˿ں�6000
	p_remote_config->tcp_dst_port[0] = d_port/256; p_remote_config->tcp_dst_port[1] = d_port%256;
	
	//���ض˿�0�Ĺ���ģʽ,TCP�ͻ���ģʽ
	p_remote_config->mode   = socket_mode_tcp_client;
}

/*
******************************************************************************
* ������  : 
* ����    : 
* ����    : 
* ���    : 
* ����ֵ  :
* ˵��    :
******************************************************************************
*/
void ETH_SOCKET_RemoteRegConfig(SOCKET_TypeDef s_id)
{
	ETH_REMOTE_CONFIG_TypeDef *p_remote_config = &ETH_SOCKET_Struct.remoteConfig[s_id];
	
	//���÷�Ƭ���ȣ��ο�W5500�����ֲᣬ��ֵ���Բ��޸�(����Ƭ�ֽ���=1460(0x5b4))
	ETH_SOCKET_WriteByte2(s_id, SOCKETn_MSSR, 1460);
	
	//������Socket�¼��ж�
	ETH_SOCKET_WriteByte1(s_id, SOCKETn_IMR, IMR_SENDOK | IMR_TIMEOUT | IMR_RECV | IMR_DISCON | IMR_CON);
	
	//���÷��ͻ������ͽ��ջ������Ĵ�С���ο�W5500�����ֲ�
	do{
		ETH_SOCKET_WriteByte1(s_id, SOCKETn_RXBUF_SIZE, 0x02);//Socket Rx memory size=2k
	}while(ETH_SOCKET_ReadByte1(s_id, SOCKETn_RXBUF_SIZE) != 0x02);
				
	do{
		ETH_SOCKET_WriteByte1(s_id, SOCKETn_TXBUF_SIZE, 0x02);//Socket Tx mempry size=2k
	}while(ETH_SOCKET_ReadByte1(s_id, SOCKETn_TXBUF_SIZE) != 0x02);
	
	do{
		ETH_SOCKET_WriteByte2(s_id, SOCKETn_TX_WR, 0);
	}while(ETH_SOCKET_ReadByte2(s_id, SOCKETn_TX_WR) != 0);
	
	do{
		ETH_SOCKET_WriteByte2(s_id, SOCKETn_RX_RD, 0);
	}while(ETH_SOCKET_ReadByte2(s_id, SOCKETn_RX_RD) != 0);
		
	//����ָ���˿�
	ETH_SOCKET_WriteByte2(s_id, SOCKETn_PORT,   p_remote_config->local_port[0]*256 + p_remote_config->local_port[1]);
	ETH_SOCKET_WriteByte2(s_id, SOCKETn_DPORTR, p_remote_config->tcp_dst_port[0]*256 + p_remote_config->tcp_dst_port[1]);
	ETH_SOCKET_WriteByte4(s_id, SOCKETn_DIPR,   p_remote_config->tcp_dst_ip);	
}

/*******************************************************************************
* ������  : ETH_SOCKET_Close
* ����    : ����ָ��Socket(0~7)Ϊ�ͻ�����Զ�̷���������
* ����    : s:���趨�Ķ˿�
* ���    : ��
* ����ֵ  : �ɹ�����TRUE(0xFF),ʧ�ܷ���FALSE(0x00)
* ˵��    : ������Socket�����ڿͻ���ģʽʱ,���øó���,��Զ�̷�������������
*			����������Ӻ���ֳ�ʱ�жϣ��������������ʧ��,��Ҫ���µ��øó�������
*			�ó���ÿ����һ��,�������������һ������
*******************************************************************************/
void ETH_SOCKET_Close(SOCKET_TypeDef s)
{
	ETH_SOCKET_WriteByte1(s, SOCKETn_CR, SOCKET_CLOSE);	//�رն˿�
}

/*
******************************************************************************
* ������  : 
* ����    : 
* ����    : 
* ���    : 
* ����ֵ  :
* ˵��    :
******************************************************************************
*/
void ETH_W5500_IrStatus(void)
{
	uint8_t err;
	
	err = ETH_W5500_ReadByte1(W5500_REG_IR);
	if(err != 0)
	{
		ETH_W5500_WriteByte1(W5500_REG_IR, (err & 0xf0));
		if(err & W5500_REG_IR__CONFLICT)//IP��ͻ
			;
		if(err & W5500_REG_IR__UNREACH)	//Ŀ�겻�ɵִ�
			;
		if(err & W5500_REG_IR__PPPoE)		//PPPoE���ӹر�
			;
		if(err & W5500_REG_IR__MP)			//Magic Packet
			;
	}
}

uint8_t ETH_SOCKET_IrStatus(uint8_t s_id)
{
	uint8_t state;
	
	state = ETH_SOCKET_ReadByte1(s_id, SOCKETn_IR);
	ETH_SOCKET_WriteByte1(s_id, SOCKETn_IR, state);
	
	return state;
}


uint8_t ETH_SOCKET_SrStatus(uint8_t s_id)
{
	return ETH_SOCKET_ReadByte1(s_id, SOCKETn_SR);
}
/*******************************************************************************
* ������  : socket_udp
* ����    : ����ָ��Socket(0~7)ΪUDPģʽ
* ����    : s:���趨�Ķ˿�
* ���    : ��
* ����ֵ  : �ɹ�����TRUE(0xFF),ʧ�ܷ���FALSE(0x00)
* ˵��    : ���Socket������UDPģʽ,���øó���,��UDPģʽ��,Socketͨ�Ų���Ҫ��������
*			�ó���ֻ����һ�Σ���ʹW5500����ΪUDPģʽ
*******************************************************************************/
uint8_t socket_udp(SOCKET_TypeDef s)
{
	uint32_t delayT;
	
	ETH_SOCKET_WriteByte1(s, SOCKETn_MR, MR_UDP);			//����SocketΪUDPģʽ*/
	ETH_SOCKET_WriteByte1(s, SOCKETn_CR, SOCKET_OPEN);	//��Socket*/
	
	for(delayT = 0; delayT < 65535; delayT++);
	
	if(ETH_SOCKET_ReadByte1(s, SOCKETn_SR) != SOCK_UDP){//���Socket��ʧ��
		ETH_SOCKET_WriteByte1(s,SOCKETn_CR, SOCKET_CLOSE);
		return FALSE;
	}else{
		return TRUE;
	}
	
	//���������Socket�Ĵ򿪺�UDPģʽ����,������ģʽ��������Ҫ��Զ��������������
	//��ΪSocket����Ҫ��������,�����ڷ�������ǰ����������Ŀ������IP��Ŀ��Socket�Ķ˿ں�
	//���Ŀ������IP��Ŀ��Socket�Ķ˿ں��ǹ̶���,�����й�����û�иı�,��ôҲ��������������
}

/*******************************************************************************
* ������  : ETH_SOCKET_Connect
* ����    : ����ָ��Socket(0~7)Ϊ�ͻ�����Զ�̷���������
* ����    : s:���趨�Ķ˿�
* ���    : ��
* ����ֵ  : �ɹ�����TRUE(0xFF),ʧ�ܷ���FALSE(0x00)
* ˵��    : ������Socket�����ڿͻ���ģʽʱ,���øó���,��Զ�̷�������������
*			����������Ӻ���ֳ�ʱ�жϣ��������������ʧ��,��Ҫ���µ��øó�������
*			�ó���ÿ����һ��,�������������һ������
*******************************************************************************/
uint8_t ETH_SOCKET_Connect(SOCKET_TypeDef s)
{	
	uint8_t state;
	
	ETH_SOCKET_WriteByte1(s, SOCKETn_MR, MR_TCP);			//����socketΪTCPģʽ
	ETH_SOCKET_WriteByte1(s, SOCKETn_CR, SOCKET_OPEN);	//��Socket
	
	state = ETH_SOCKET_ReadByte1(s, SOCKETn_SR);
	if(state != SOCK_INIT)	{//���socket��ʧ��
		return FALSE;
	}
	
	ETH_SOCKET_WriteByte1(s, SOCKETn_CR, SOCKET_CONNECT);//����SocketΪConnectģʽ
	return TRUE;
}

/*******************************************************************************
* ������  : ETH_SOCKET_TcpListen
* ����    : ����ָ��Socket(0~7)��Ϊ�������ȴ�Զ������������
* ����    : s:���趨�Ķ˿�
* ���    : ��
* ����ֵ  : �ɹ�����TRUE(0xFF),ʧ�ܷ���FALSE(0x00)
* ˵��    : ������Socket�����ڷ�����ģʽʱ,���øó���,�ȵ�Զ������������
*			�ó���ֻ����һ��,��ʹW5500����Ϊ������ģʽ
*******************************************************************************/
uint8_t ETH_SOCKET_TcpListen(SOCKET_TypeDef s)
{
	//�ر�
	ETH_SOCKET_WriteByte1(s, SOCKETn_CR, SOCKET_CLOSE);
	
	//TCPģʽ & ��Socket
	ETH_SOCKET_WriteByte1( s, SOCKETn_MR, MR_TCP );
	ETH_SOCKET_WriteByte1( s, SOCKETn_CR, SOCKET_OPEN );
	if(ETH_SOCKET_ReadByte1( s, SOCKETn_SR ) != SOCK_INIT){
		return FALSE;
	}	
	
	//��������ģʽ
	ETH_SOCKET_WriteByte1(s, SOCKETn_CR, SOCKET_LISTEN);
	if(ETH_SOCKET_ReadByte1(s, SOCKETn_SR) != SOCK_LISTEN){
		return FALSE;
	}

	return TRUE;

	//���������Socket�Ĵ򿪺�������������,����Զ�̿ͻ����Ƿ�������������,����Ҫ�ȴ�Socket�жϣ�
	//���ж�Socket�������Ƿ�ɹ����ο�W5500�����ֲ��Socket�ж�״̬
	//�ڷ���������ģʽ����Ҫ����Ŀ��IP��Ŀ�Ķ˿ں�
}

