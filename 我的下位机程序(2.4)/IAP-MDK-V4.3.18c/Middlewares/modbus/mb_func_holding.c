/* 
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (c) 2006 Christian Walter <wolti@sil.at>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: mbfuncholding.c,v 1.12 2007/02/18 23:48:22 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

#include "mb_func_holding.h"
#include "mb_frame.h"
#include "mb_serial.h"

#include "mb_cb.h"
#include "mb_type.h"

/* ----------------------- Defines ------------------------------------------*/
#define MB_PDU_FUNC_READ_ADDR_OFF               ( MB_PDU_DATA_OFF + 0)
#define MB_PDU_FUNC_READ_REGCNT_OFF             ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_READ_SIZE                   ( 4 )
#define MB_PDU_FUNC_READ_REGCNT_MAX             ( 0x007D )	//??????????????????????????????

#define MB_PDU_FUNC_WRITE_ADDR_OFF              ( MB_PDU_DATA_OFF + 0)
#define MB_PDU_FUNC_WRITE_VALUE_OFF             ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_WRITE_SIZE                  ( 4 )

#define MB_PDU_FUNC_WRITE_MUL_ADDR_OFF          ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF        ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF       ( MB_PDU_DATA_OFF + 4 )
#define MB_PDU_FUNC_WRITE_MUL_VALUES_OFF        ( MB_PDU_DATA_OFF + 5 )
#define MB_PDU_FUNC_WRITE_MUL_SIZE_MIN          ( 5 )
#define MB_PDU_FUNC_WRITE_MUL_REGCNT_MAX        ( 0x0078 )	//??????????????????????????????

#define MB_PDU_FUNC_READWRITE_READ_ADDR_OFF     ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_FUNC_READWRITE_READ_REGCNT_OFF   ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_READWRITE_WRITE_ADDR_OFF    ( MB_PDU_DATA_OFF + 4 )
#define MB_PDU_FUNC_READWRITE_WRITE_REGCNT_OFF  ( MB_PDU_DATA_OFF + 6 )
#define MB_PDU_FUNC_READWRITE_BYTECNT_OFF       ( MB_PDU_DATA_OFF + 8 )
#define MB_PDU_FUNC_READWRITE_WRITE_VALUES_OFF  ( MB_PDU_DATA_OFF + 9 )
#define MB_PDU_FUNC_READWRITE_SIZE_MIN          ( 9 )

/* ----------------------- Static functions ---------------------------------*/
eMBException    prveMBError2Exception( eMBErrorCode eErrorCode );

/* ----------------------- Start implementation -----------------------------*/

#if MB_FUNC_WRITE_HOLDING_ENABLED > 0
/*
06H-????????????????????????

????????????	|	?????????	| RegAddr_H	|	RegAddr_L	| DATA_H	| DATA_L	|	CRC_H	|	CRC_L
*/
eMBException eMBFuncWriteHoldingRegisterSlave( MB_VAR_PORT *p_mb_var_port ) 
{
	uint16_t          usRegAddress;
	eMBException    eStatus = MB_EX_NONE;
	eMBErrorCode    eRegStatus;

	uint8_t *buffer = p_mb_var_port->rxdbuffer;
	
	if( p_mb_var_port->cur_rxd_frame_len == ( MB_PDU_FUNC_WRITE_SIZE + MB_PDU_SIZE_MIN ) )
	{
		
		usRegAddress = ( uint16_t )( buffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_WRITE_ADDR_OFF)] << 8 );
		usRegAddress |= ( uint16_t )( buffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_WRITE_ADDR_OFF + 1)] );
//        usRegAddress++;

		/* Make callback to update the value. */
		p_mb_var_port->rxdbufcnt = CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_WRITE_VALUE_OFF);
		eRegStatus = eMBRegHoldingCB(p_mb_var_port, usRegAddress, 1, MB_REG_WRITE );

		/* If an error occured convert it into a Modbus exception. */
		if( eRegStatus != MB_ENOERR ){
				eStatus = prveMBError2Exception( eRegStatus );
		}
	}
	else
	{
		/* Can't be a valid request because the length is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return eStatus;
}

eMBException eMBFuncWriteHoldingRegisterMaster( MB_VAR_PORT *p_mb_var_port ) 
{
	eMBException eStatus = MB_EX_NONE;
	
	return eStatus;
}

#endif

#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED > 0
/*
10H-????????????????????????

???????????? | ????????? | ADDRE_H | ADDRE_L | REG_CNT_H | REG_CNT_L | ????????? | DATA1_H | DATA1_L | ... | DATAX_H | DATAX_L | CRC_H | CRC_L
*/
eMBException eMBFuncWriteMultipleHoldingRegisterSlave( MB_VAR_PORT *p_mb_var_port )
{
	uint16_t          usRegAddress;
	uint16_t          usRegCount;
	uint8_t           ucRegByteCount;

	eMBException    eStatus = MB_EX_NONE;
	eMBErrorCode    eRegStatus;

	uint8_t *buffer = p_mb_var_port->rxdbuffer;
	uint8_t *txdbuffer = p_mb_var_port->txdbuffer;
	
	if( p_mb_var_port->cur_rxd_frame_len >= ( MB_PDU_FUNC_WRITE_MUL_SIZE_MIN + MB_PDU_SIZE_MIN ) )
	{
		usRegAddress  = ( uint16_t )( buffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_WRITE_MUL_ADDR_OFF)] << 8 );	//?????????????????????
		usRegAddress |= ( uint16_t )( buffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_WRITE_MUL_ADDR_OFF + 1)]);
//        usRegAddress++;

		usRegCount  = ( uint16_t )buffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF)]<<8;
		usRegCount |= ( uint16_t )buffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF + 1)];
	
		ucRegByteCount = buffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF)];//DATA????????????

		if( ( usRegCount >= 1 ) && ( usRegCount <= MB_PDU_FUNC_WRITE_MUL_REGCNT_MAX ) && ( ucRegByteCount == ( uint8_t ) ( 2 * usRegCount ) ) )
		{
			/* ????????? */
			p_mb_var_port->rxdbufcnt = CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_WRITE_MUL_VALUES_OFF);
			eRegStatus = eMBRegHoldingCB( p_mb_var_port, usRegAddress, usRegCount, MB_REG_WRITE );

			/* ?????? */
			p_mb_var_port->cur_txd_frame_pos = p_mb_var_port->txdbufpos;
			if(p_mb_var_port->mode == MB_RTU || p_mb_var_port->mode == MB_ASCII)
				p_mb_var_port->txdbufpos = CONFIG_MB_TXD_BUFFER_POS(p_mb_var_port->txdbufpos, 1);
			else
				p_mb_var_port->txdbufpos = CONFIG_MB_TXD_BUFFER_POS(p_mb_var_port->txdbufpos, 7);

			p_mb_var_port->cur_txd_frame_len = MB_PDU_FUNC_OFF;

			txdbuffer[p_mb_var_port->txdbufpos] = MB_FUNC_WRITE_MULTIPLE_REGISTERS;	//?????????
			p_mb_var_port->txdbufpos = CONFIG_MB_TXD_BUFFER_POS(p_mb_var_port->txdbufpos, 1);
			p_mb_var_port->cur_txd_frame_len++;
			
			txdbuffer[p_mb_var_port->txdbufpos] = (usRegAddress>>8) & 0xFF;	//??????
			p_mb_var_port->txdbufpos = CONFIG_MB_TXD_BUFFER_POS(p_mb_var_port->txdbufpos, 1);
			p_mb_var_port->cur_txd_frame_len++;
			
			txdbuffer[p_mb_var_port->txdbufpos] = usRegAddress & 0xFF;
			p_mb_var_port->txdbufpos = CONFIG_MB_TXD_BUFFER_POS(p_mb_var_port->txdbufpos, 1);
			p_mb_var_port->cur_txd_frame_len++;
			
			txdbuffer[p_mb_var_port->txdbufpos] = (usRegCount>>8) & 0xFF;	//???????????????
			p_mb_var_port->txdbufpos = CONFIG_MB_TXD_BUFFER_POS(p_mb_var_port->txdbufpos, 1);
			p_mb_var_port->cur_txd_frame_len++;
			
			txdbuffer[p_mb_var_port->txdbufpos] = usRegCount & 0xFF;
			p_mb_var_port->txdbufpos = CONFIG_MB_TXD_BUFFER_POS(p_mb_var_port->txdbufpos, 1);
			p_mb_var_port->cur_txd_frame_len++;
			
			/* If an error occured convert it into a Modbus exception. */
			if( eRegStatus != MB_ENOERR ){
				eStatus = prveMBError2Exception( eRegStatus );
			}else{
				/* The response contains the function code, the starting
				 * address and the quantity of registers. We reuse the
				 * old values in the buffer because they are still valid.
				 */
				p_mb_var_port->cur_txd_frame_len = MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF;
			}
		}
		else
		{
			eStatus = MB_EX_ILLEGAL_DATA_VALUE;
		}
	}
	else
	{
		/* Can't be a valid request because the length is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return eStatus;
}

eMBException eMBFuncWriteMultipleHoldingRegisterMaster( MB_VAR_PORT *p_mb_var_port )
{
	uint16_t          usRegAddress;
	uint16_t          usRegCount;
	
	eMBException    eStatus = MB_EX_NONE;
	
	uint8_t *buffer = p_mb_var_port->rxdbuffer;
	
	if( p_mb_var_port->cur_rxd_frame_len >= ( MB_PDU_FUNC_WRITE_MUL_SIZE_MIN ) )
	{
		usRegAddress  = ( uint16_t )( buffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_WRITE_MUL_ADDR_OFF)] << 8 );	//?????????????????????
		usRegAddress |= ( uint16_t )( buffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_WRITE_MUL_ADDR_OFF + 1)]);

		usRegCount  = ( uint16_t )buffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF)]<<8;
		usRegCount |= ( uint16_t )buffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF + 1)];
	}
	
	p_mb_var_port->mb_var_master_buf[p_mb_var_port->master_buf_pos].pos = CONFIG_MASTER_POS_FREE;
	
	p_mb_var_port->master_buf_pos = -1;
	p_mb_var_port->out_time_mark  = 0;
	
	mb_serial_txd_frame_delete(p_mb_var_port);
	
	return eStatus;
}

#endif

#if MB_FUNC_READ_HOLDING_ENABLED > 0
/*
03H-??????????????????

???????????? | ????????? | REG_ADDR_H | REG_ADDR_L | REG_CNT_H | REG_CNT_L | CRC_H | CRC_L
*/
eMBException eMBFuncReadHoldingRegisterSlave( MB_VAR_PORT *p_mb_var_port )
{
	uint16_t          usRegAddress;
	uint16_t          usRegCount;

	eMBException    eStatus = MB_EX_NONE;
	eMBErrorCode    eRegStatus;

	uint8_t *rxdbuffer = p_mb_var_port->rxdbuffer;
	uint8_t *txdbuffer = p_mb_var_port->txdbuffer;
	
	if( p_mb_var_port->cur_rxd_frame_len == ( MB_PDU_FUNC_READ_SIZE + MB_PDU_SIZE_MIN ) )
	{
		usRegAddress = ( uint16_t )( rxdbuffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_READ_ADDR_OFF)] << 8 );
		usRegAddress |= ( uint16_t )( rxdbuffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_READ_ADDR_OFF + 1)] );
//			usRegAddress++;

		usRegCount = ( uint16_t )( rxdbuffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_READ_REGCNT_OFF)] << 8 );
		usRegCount = ( uint16_t )( rxdbuffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_READ_REGCNT_OFF + 1)] );

		/* Check if the number of registers to read is valid. If not
		 * return Modbus illegal data value exception. 
		 */
		if( ( usRegCount >= 1 ) && ( usRegCount <= MB_PDU_FUNC_READ_REGCNT_MAX ) )
		{
			p_mb_var_port->cur_txd_frame_pos = p_mb_var_port->txdbufpos;
			if(p_mb_var_port->mode == MB_RTU || p_mb_var_port->mode == MB_ASCII)
				p_mb_var_port->txdbufpos = CONFIG_MB_TXD_BUFFER_POS(p_mb_var_port->txdbufpos, 1);
			else
				p_mb_var_port->txdbufpos = CONFIG_MB_TXD_BUFFER_POS(p_mb_var_port->txdbufpos, 7);
			
			/* Set the current PDU data pointer to the beginning. */
			p_mb_var_port->cur_txd_frame_len = MB_PDU_FUNC_OFF;

			/* First byte contains the function code. */
			txdbuffer[p_mb_var_port->txdbufpos] = MB_FUNC_READ_HOLDING_REGISTER;	//?????????
			p_mb_var_port->txdbufpos = CONFIG_MB_TXD_BUFFER_POS(p_mb_var_port->txdbufpos, 1);
			p_mb_var_port->cur_txd_frame_len++;

			/* Second byte in the response contain the number of bytes. */
			txdbuffer[p_mb_var_port->txdbufpos] = ( uint8_t ) ( usRegCount * 2 );//??????????????????
			p_mb_var_port->txdbufpos = CONFIG_MB_TXD_BUFFER_POS(p_mb_var_port->txdbufpos, 1);
			p_mb_var_port->cur_txd_frame_len++;

			/* Make callback to fill the buffer. */
			eRegStatus = eMBRegHoldingCB( p_mb_var_port, usRegAddress, usRegCount, MB_REG_READ );
			/* If an error occured convert it into a Modbus exception. */
			if( eRegStatus != MB_ENOERR ){
				eStatus = prveMBError2Exception( eRegStatus );
			}
			else{
				p_mb_var_port->cur_txd_frame_len += usRegCount * 2;
			}
		}
		else
		{
			eStatus = MB_EX_ILLEGAL_DATA_VALUE;
		}
	}
	else
	{
		/* Can't be a valid request because the length is incorrect. */
		eStatus = MB_EX_ILLEGAL_DATA_VALUE;
	}
	return eStatus;
}

eMBException eMBFuncReadHoldingRegisterMaster( MB_VAR_PORT *p_mb_var_port )
{
	uint16_t usRegCount;
	uint8_t  i = 0;
	
	eMBException    eStatus = MB_EX_NONE;
	
	uint8_t *buffer = p_mb_var_port->rxdbuffer;
	uint8_t *rxdbuffer = p_mb_var_port->rxdbuffer;
	
	if( p_mb_var_port->cur_rxd_frame_len >= ( 1 /*MB_PDU_FUNC_READ_SIZE*/ ) )
	{
		usRegCount  = ( uint16_t )( buffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, 1)]/2 );	//?????????????????????
	}
			
	for(i = 0; i < usRegCount*2; i++){
		p_mb_var_port->mb_var_master_buf[p_mb_var_port->master_buf_pos].buf[i] = rxdbuffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, 1 + 1 + i)];
	}
	p_mb_var_port->mb_var_master_buf[p_mb_var_port->master_buf_pos].pos = CONFIG_MASTER_POS_RESULT;
	
	p_mb_var_port->master_buf_pos = -1;
	p_mb_var_port->out_time_mark  = 0;
	
	mb_serial_txd_frame_delete(p_mb_var_port);
	
	return eStatus;
}

#endif

#if MB_FUNC_READWRITE_HOLDING_ENABLED > 0

eMBException eMBFuncReadWriteMultipleHoldingRegisterSlave( MB_VAR_PORT *p_mb_var_port )
{
	uint16_t          usRegReadAddress;
	uint16_t          usRegReadCount;
	uint16_t          usRegWriteAddress;
	uint16_t          usRegWriteCount;
	uint8_t           ucRegWriteByteCount;

	eMBException    eStatus = MB_EX_NONE;
	eMBErrorCode    eRegStatus;

	uint8_t *rxdbuffer = p_mb_var_port->rxdbuffer;
	uint8_t *txdbuffer = p_mb_var_port->txdbuffer;
	
	if( p_mb_var_port->cur_rxd_frame_len >= ( MB_PDU_FUNC_READWRITE_SIZE_MIN + MB_PDU_SIZE_MIN ) )
	{
		usRegReadAddress = ( uint16_t )( rxdbuffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_READWRITE_READ_ADDR_OFF)] << 8U );
		usRegReadAddress |= ( uint16_t )( rxdbuffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_READWRITE_READ_ADDR_OFF + 1)] );
		usRegReadAddress++;

		usRegReadCount = ( uint16_t )( rxdbuffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_READWRITE_READ_REGCNT_OFF)] << 8U );
		usRegReadCount |= ( uint16_t )( rxdbuffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_READWRITE_READ_REGCNT_OFF + 1)] );
		
		usRegWriteAddress = ( uint16_t )( rxdbuffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_READWRITE_WRITE_ADDR_OFF)] << 8U );
		usRegWriteAddress |= ( uint16_t )( rxdbuffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_READWRITE_WRITE_ADDR_OFF + 1)] );
		usRegWriteAddress++;

		usRegWriteCount = ( uint16_t )( rxdbuffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_READWRITE_WRITE_REGCNT_OFF)] << 8U );
		usRegWriteCount |= ( uint16_t )( rxdbuffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_READWRITE_WRITE_REGCNT_OFF + 1)] );
		
		ucRegWriteByteCount = rxdbuffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_READWRITE_BYTECNT_OFF)];

		if( (usRegReadCount >= 1) && (usRegReadCount <= 0x7D) && (usRegWriteCount >= 1) && (usRegWriteCount <= 0x79) && ((2*usRegWriteCount) == ucRegWriteByteCount ) )
		{
			/* Make callback to update the register values. */
			p_mb_var_port->rxdbufcnt = CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_READWRITE_WRITE_VALUES_OFF);
			eRegStatus = eMBRegHoldingCB( p_mb_var_port, usRegWriteAddress, usRegWriteCount, MB_REG_WRITE );

			if( eRegStatus == MB_ENOERR )
			{
				p_mb_var_port->cur_txd_frame_pos = p_mb_var_port->txdbufpos;
				if(p_mb_var_port->mode == MB_RTU || p_mb_var_port->mode == MB_ASCII)
					p_mb_var_port->txdbufpos = CONFIG_MB_TXD_BUFFER_POS(p_mb_var_port->txdbufpos, 1);
				else
					p_mb_var_port->txdbufpos = CONFIG_MB_TXD_BUFFER_POS(p_mb_var_port->txdbufpos, 7);
				
				/* Set the current PDU data pointer to the beginning. */
				txdbuffer[p_mb_var_port->txdbufpos] = rxdbuffer[CONFIG_MB_RXD_BUFFER_POS(p_mb_var_port->cur_rxd_frame_pos, MB_PDU_FUNC_OFF)];
				p_mb_var_port->cur_txd_frame_len++;

				/* First byte contains the function code. */
				p_mb_var_port->txdbufpos = CONFIG_MB_TXD_BUFFER_POS(p_mb_var_port->txdbufpos, 1);
				txdbuffer[p_mb_var_port->txdbufpos] = MB_FUNC_READWRITE_MULTIPLE_REGISTERS;
				p_mb_var_port->cur_txd_frame_len++;

				/* Second byte in the response contain the number of bytes. */
				p_mb_var_port->txdbufpos = CONFIG_MB_TXD_BUFFER_POS(p_mb_var_port->txdbufpos, 1);
				txdbuffer[p_mb_var_port->txdbufpos] = ( uint8_t ) ( usRegReadCount * 2 );
				p_mb_var_port->cur_txd_frame_len++;

				/* Make the read callback. */
				eRegStatus = eMBRegHoldingCB( p_mb_var_port, usRegReadAddress, usRegReadCount, MB_REG_READ );
				if( eRegStatus == MB_ENOERR ){
						p_mb_var_port->cur_txd_frame_len += 2 * usRegReadCount;
				}
			}
			if( eRegStatus != MB_ENOERR ){
				eStatus = prveMBError2Exception( eRegStatus );
			}
		}
		else
		{
			eStatus = MB_EX_ILLEGAL_DATA_VALUE;
		}
	}
	return eStatus;
}

eMBException eMBFuncReadWriteMultipleHoldingRegisterMaster( MB_VAR_PORT *p_mb_var_port )
{
	eMBException eStatus = MB_EX_NONE;
	
	return eStatus;
}
#endif
