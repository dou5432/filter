#include "app_user.h"
#include "app_config.h"
#include "app_type.h"

//#include "FreeRTOS.h"
//#include "semphr.h"
//#include "task.h"

//#include "amc_user.h"
//#include "mb_user.h"

/*
* ͨ�����ݴ洢�ռ�
*/
uint8_t usAppUserCommBuf[APP_USER_COMM_BUF_ROW_SIZE][APP_USER_COMM_BUF_COL_SIZE] = {1,2,3,4,5,6,7,8,9,10};

/**
* app����
*
*/

void APP_USER_CreateTask(void)
{	
	//amc����
//	xTaskCreate(AMC_USER_Task, "AMC_USER_Task", 1024, NULL, 1, NULL);
	
	//modbus����
//	xTaskCreate(MB_USER_Task, "MB_USER_Task", 1024, NULL, 2, NULL);
	
	//mqtt����
//	xTaskCreate(MQTT_USER_Task, "MQTT_USER_Task", 2048, NULL, 3, NULL);
	
	//canopen����
//	xTaskCreate(CO_USER_Task, "co_user_task", 1024, NULL, 4, NULL);
}

