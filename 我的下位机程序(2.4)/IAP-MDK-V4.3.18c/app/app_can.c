#include "app_can.h"
#include "plc_can.h"

/*
��������
*/
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan->Instance == CAN1){
		plc_can_receive(hcan);
	}
}

