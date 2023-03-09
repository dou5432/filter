#ifndef __MB_FUNC_HOLDING_H__
#define __MB_FUNC_HOLDING_H__

#include "mb_type.h"
#include "mb_proto.h"
/*
03H-�����ּĴ���

�ӻ���ַ | ������ | REG_ADDR_H | REG_ADDR_L | REG_CNT_H | REG_CNT_L | CRC_H | CRC_L
*/
#if MB_FUNC_READ_HOLDING_ENABLED > 0
	eMBException eMBFuncReadHoldingRegisterSlave( MB_VAR_PORT *p_mb_var_port );
	eMBException eMBFuncReadHoldingRegisterMaster( MB_VAR_PORT *p_mb_var_port );
#endif

/*
06H-д�������ּĴ���

�ӻ���ַ	|	������	| RegAddr_H	|	RegAddr_L	| DATA_H	| DATA_L	|	CRC_H	|	CRC_L
*/
#if MB_FUNC_WRITE_HOLDING_ENABLED > 0
	eMBException eMBFuncWriteHoldingRegisterSlave( MB_VAR_PORT *p_mb_var_port );
	eMBException eMBFuncWriteHoldingRegisterMaster( MB_VAR_PORT *p_mb_var_port ) ;
#endif

/*
10H-д������ּĴ���

�ӻ���ַ | ������ | ADDRE_H | ADDRE_L | REG_CNT_H | REG_CNT_L | �ֽ��� | DATA1_H | DATA1_L | ... | DATAX_H | DATAX_L | CRC_H | CRC_L
*/
#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED > 0
	eMBException eMBFuncWriteMultipleHoldingRegisterSlave( MB_VAR_PORT *p_mb_var_port );
	eMBException eMBFuncWriteMultipleHoldingRegisterMaster( MB_VAR_PORT *p_mb_var_port );
#endif

#if MB_FUNC_READWRITE_HOLDING_ENABLED > 0
	eMBException eMBFuncReadWriteMultipleHoldingRegisterSlave( MB_VAR_PORT *p_mb_var_port );
	eMBException eMBFuncReadWriteMultipleHoldingRegisterMaster( MB_VAR_PORT *p_mb_var_port );
#endif

#endif
