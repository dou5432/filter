#include "eth_spi.h"
#include "eth_type.h"
#include "eth_w5500.h"

/*
******************************************************************************
* ������  : 
* ����    : ����Ƚ�
* ����    : 
* ���    : 
* ����ֵ  : 
* ˵��    :
******************************************************************************
*/
static uint8_t ETH_W5500_ArrayCompare(uint8_t *src, uint8_t *dst, uint8_t size)
{
	uint8_t i = 0;
	
	for(i = 0; i < size; i++){
		if(src[i] != dst[i])
			return FALSE;
	}
	return TRUE;
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
void ETH_W5500_WriteByte1(uint16_t reg, uint8_t dat)
{
	//��W5500��SCSΪ�͵�ƽ
	ETH_SPI_SetCS(LOW);

	//ͨ��SPIд16λ�Ĵ�����ַ
	EHT_SPI_WriteByte2(reg);
	
	//ͨ��SPIд�����ֽ�,1���ֽ����ݳ���,д����,ѡ��ͨ�üĴ���
	EHT_SPI_WriteByte1(W5500_RWB_FDM1 | W5500_RWB_WRITE | W5500_COMMON_R);
	
	//д1���ֽ�����
	EHT_SPI_WriteByte1(dat);

	//��W5500��SCSΪ�ߵ�ƽ
	ETH_SPI_SetCS(HIGH); 
}

void ETH_W5500_WriteByte2(uint16_t reg, uint16_t dat)
{
	//��W5500��SCSΪ�͵�ƽ
	ETH_SPI_SetCS(LOW);
		
	//ͨ��SPIд16λ�Ĵ�����ַ
	EHT_SPI_WriteByte2(reg);
	
	//ͨ��SPIд�����ֽ�,2���ֽ����ݳ���,д����,ѡ��ͨ�üĴ���
	EHT_SPI_WriteByte1(W5500_RWB_FDM2 | W5500_RWB_WRITE | W5500_COMMON_R);
	
	//д16λ����
	EHT_SPI_WriteByte2(dat);

	//��W5500��SCSΪ�ߵ�ƽ
	ETH_SPI_SetCS(HIGH); 
}

void ETH_W5500_WriteBuffer(uint16_t reg, uint8_t *dat_ptr, uint16_t size)
{
	uint16_t i;

	//��W5500��SCSΪ�͵�ƽ	
	ETH_SPI_SetCS(LOW);
		
	//ͨ��SPIд16λ�Ĵ�����ַ
	EHT_SPI_WriteByte2(reg);
	
	//ͨ��SPIд�����ֽ�,N���ֽ����ݳ���,д����,ѡ��ͨ�üĴ���
	EHT_SPI_WriteByte1(W5500_RWB_VDM | W5500_RWB_WRITE | W5500_COMMON_R);

	//ѭ������������size���ֽ�����д��W5500
	for(i=0; i<size; i++)
		EHT_SPI_WriteByte1(*dat_ptr++);//дһ���ֽ�����

	//��W5500��SCSΪ�ߵ�ƽ
	ETH_SPI_SetCS(HIGH); 
}

/*
******************************************************************
* ������  : 
* ����    : 
* ����    : 
* ���    : 
* ����ֵ  : 
* ˵��    : 
******************************************************************
*/
uint8_t ETH_W5500_ReadByte1(uint16_t reg)
{
	uint8_t value;

	//SCSΪ�͵�ƽ
	ETH_SPI_SetCS(LOW);
			
	//д������
	EHT_SPI_WriteByte2(reg);//ͨ��SPIд16λ�Ĵ�����ַ
	EHT_SPI_WriteByte1(W5500_RWB_FDM1 | W5500_RWB_READ | W5500_COMMON_R);//ͨ��SPIд�����ֽ�,1���ֽ����ݳ���,������,ѡ��ͨ�üĴ���

	//��������
	value = EHT_SPI_ReadByte1();

	//SCSΪ�ߵ�ƽ
	ETH_SPI_SetCS(HIGH);
	
	//���ض�ȡ���ļĴ�������
	return value;
}

void ETH_W5500_ReadBuffer(uint16_t reg, uint8_t *dat_ptr, uint16_t size)
{
	uint16_t idx = 0;
	
	//SCSΪ�͵�ƽ
	ETH_SPI_SetCS(LOW);
			
	//д������
	EHT_SPI_WriteByte2(reg);//ͨ��SPIд16λ�Ĵ�����ַ
	EHT_SPI_WriteByte1(W5500_RWB_VDM | W5500_RWB_READ | W5500_COMMON_R);//ͨ��SPIд�����ֽ�,1���ֽ����ݳ���,������,ѡ��ͨ�üĴ���

	//��������
	for(idx = 0; idx < size; idx++) {                  // Write data in loop
    dat_ptr[idx] = EHT_SPI_ReadByte1();
  }

	//SCSΪ�ߵ�ƽ
	ETH_SPI_SetCS(HIGH);
}
/*
******************************************************************
* ������  : 
* ����    : 
* ����    : 
* ���    : 
* ����ֵ  : 
* ˵��    : 
******************************************************************
*/
void ETH_W5500_WriteCompare(uint16_t reg, uint8_t *dat_ptr, uint16_t size)
{
	uint8_t i;
	uint8_t value;
	
	for(i = 0; i < size; i++)
	{
		do{
			ETH_W5500_WriteByte1(reg + i, dat_ptr[i]);
			value = ETH_W5500_ReadByte1(reg + i);
		}while(value != dat_ptr[i]);
	}
}

void ETH_W5500_WriteCheck(uint16_t reg, uint8_t *dat_ptr, uint16_t size)
{
	uint8_t rxd[12] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
	
	do{
		ETH_W5500_WriteBuffer(reg, dat_ptr, size);
		ETH_W5500_ReadBuffer(reg, rxd, size);
	}while(ETH_W5500_ArrayCompare(dat_ptr, rxd, size) == FALSE);
}	

/*******************************************************************************
* ������  : ETH_W5500_LocalVarConfig
* ����    : װ���������
* ����    : ��
* ���    : ��
* ����ֵ  : ��
* ˵��    : ���ء����롢�����ַ������IP��ַ���˿ںš�Ŀ��IP��ַ��Ŀ�Ķ˿ںš��˿ڹ���ģʽ
*******************************************************************************/
void ETH_W5500_LocalVarConfig(void)
{
	ETH_LOCAL_CONFIG_TypeDef *p_local_config_struct = &ETH_SOCKET_Struct.localConfig;
	
	//�������ز���
	p_local_config_struct->gateway[0] = ETH_CONFIG_LOCAL_GATEWAY[0]; 
	p_local_config_struct->gateway[1] = ETH_CONFIG_LOCAL_GATEWAY[1]; 
	p_local_config_struct->gateway[2] = ETH_CONFIG_LOCAL_GATEWAY[2]; 
	p_local_config_struct->gateway[3] = ETH_CONFIG_LOCAL_GATEWAY[3];
	//������������
	p_local_config_struct->submask[0] = ETH_CONFIG_LOCAL_SUBMASK[0]; 
	p_local_config_struct->submask[1] = ETH_CONFIG_LOCAL_SUBMASK[1]; 
	p_local_config_struct->submask[2] = ETH_CONFIG_LOCAL_SUBMASK[2];	
	p_local_config_struct->submask[3] = ETH_CONFIG_LOCAL_SUBMASK[3];
	//���������ַ
	p_local_config_struct->phyaddr[0] = ETH_CONFIG_LOCAL_PHYADDR[0]; 
	p_local_config_struct->phyaddr[1] = ETH_CONFIG_LOCAL_PHYADDR[1]; 
	p_local_config_struct->phyaddr[2] = ETH_CONFIG_LOCAL_PHYADDR[2]; 
	p_local_config_struct->phyaddr[3] = ETH_CONFIG_LOCAL_PHYADDR[3]; 
	p_local_config_struct->phyaddr[4] = ETH_CONFIG_LOCAL_PHYADDR[4]; 
	p_local_config_struct->phyaddr[5] = ETH_CONFIG_LOCAL_PHYADDR[5]; 
	//���ر���IP��ַ
	p_local_config_struct->localip[0] = ETH_CONFIG_LOCAL_IP[0];
	p_local_config_struct->localip[1] = ETH_CONFIG_LOCAL_IP[1];
	p_local_config_struct->localip[2] = ETH_CONFIG_LOCAL_IP[2];
	p_local_config_struct->localip[3] = ETH_CONFIG_LOCAL_IP[3];
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
void ETH_W5500_BaseRegConfig(void)
{
	uint8_t outtime_time[2] = {0x07, 0xd0};	
	uint8_t outtime_cnt[1] = {0x08};
	
	//�����λW5500,��1��Ч,��λ���Զ���0
	ETH_W5500_WriteByte1(W5500_REG_MR, W5500_REG_MR__RST);
	while((ETH_W5500_ReadByte1(W5500_REG_MR) & W5500_REG_MR__RST) != 0);
	
	//��������ʱ�䣬Ĭ��Ϊ2000(200ms) 
	//ÿһ��λ��ֵΪ100΢��,��ʼ��ʱֵ��Ϊ2000(0x07D0),����200����
	ETH_W5500_WriteCheck(W5500_REG_RTR, outtime_time, 2);

	//�������Դ�����Ĭ��Ϊ8�� 
	//����ط��Ĵ��������趨ֵ,�������ʱ�ж�(��صĶ˿��жϼĴ����е�Sn_IR ��ʱλ(TIMEOUT)�á�1��)
	ETH_W5500_WriteCheck(W5500_REG_RTR, outtime_cnt, 1);

	//�����жϣ��ο�W5500�����ֲ�ȷ���Լ���Ҫ���ж�����
	//IMR_CONFLICT��IP��ַ��ͻ�쳣�ж�,IMR_UNREACH��UDPͨ��ʱ����ַ�޷�������쳣�ж�
	ETH_W5500_WriteByte1(W5500_REG_IMR, W5500_REG_IMR__IM_IR7 | W5500_REG_IMR__IM_IR6);
	ETH_W5500_WriteByte1(W5500_REG_SIMR, W5500_REG_SIMR__S0_IMR);
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
void ETH_W5500_LocalRegConfig(void)
{
	ETH_LOCAL_CONFIG_TypeDef *p_local_config_struct = &ETH_SOCKET_Struct.localConfig;
	
	//���������ַ,PHY_ADDRΪ6�ֽ�uint8_t����,�Լ�����,����Ψһ��ʶ�����豸�������ֵַ
	//�õ�ֵַ��Ҫ��IEEE���룬����OUI�Ĺ涨��ǰ3���ֽ�Ϊ���̴��룬�������ֽ�Ϊ��Ʒ���
	//����Լ����������ַ��ע���һ���ֽڱ���Ϊż��
	ETH_W5500_WriteCheck(W5500_REG_SHAR, p_local_config_struct->phyaddr, 6);
	
	//������������(MASK)ֵ,SUB_MASKΪ4�ֽ�uint8_t����,�Լ�����
	//��������������������
	ETH_W5500_WriteCheck(W5500_REG_SUBR, p_local_config_struct->submask, 4);
	
	//��������(Gateway)��IP��ַ,gatewayΪ4�ֽ�uint8_t����,�Լ����� 
	//ʹ�����ؿ���ʹͨ��ͻ�������ľ��ޣ�ͨ�����ؿ��Է��ʵ��������������Internet
	ETH_W5500_WriteCheck(W5500_REG_GAR, p_local_config_struct->gateway, 4);
	
	//���ñ�����IP��ַ,IP_ADDRΪ4�ֽ�uint8_t����,�Լ�����
	//ע�⣬����IP�����뱾��IP����ͬһ�����������򱾻����޷��ҵ�����
	ETH_W5500_WriteCheck(W5500_REG_SIPR, p_local_config_struct->localip, 4);
}
