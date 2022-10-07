/**********************************************************************************************************
 * @�ļ�     StartTask.c
 * @˵��     ����ϵͳ�����ļ�
 * @�汾  	 V1.0
 * @����     ��־��
 * @����     2020.1
**********************************************************************************************************/
#include "main.h"
uint32_t CPU_high_water;

/*�������ȼ���ֵԽ�ͣ����ȼ�Խ��*/

#define START_TASK_PRIO 1  //�������ȼ�
#define START_STK_SIZE 512 //�����ջ1
static TaskHandle_t StartTask_Handler; //������

#define CPU_TASK_PRIO 2  //�������ȼ�
#define CPU_STK_SIZE 128 //�����ջ2
static TaskHandle_t CPUTask_Handler; //������

#define CHAR_TASK_PRIO 17  //�������ȼ�
#define CHAR_STK_SIZE 128 //�����ջ
static TaskHandle_t Char_Handler; //������

#define GRAPHIC_TASK_PRIO 17  //�������ȼ�
#define GRAPHIC_STK_SIZE 128 //�����ջ
static TaskHandle_t Graphic_Handler; //������

#define OFFLINE_CHECK_TASK_PRIO 18  //�������ȼ�
#define OFFLINE_CHECK_STK_SIZE 128 //�����ջ
static TaskHandle_t OfflineCheckTask_Handler; //������

#define POWERCONTROL_TASK_PRIO 17  //�������ȼ�
#define POWERCONTROL_STK_SIZE 512 //�����ջ3
TaskHandle_t PowerControlTask_Handler; //������

#define JUDGERECEIVE_TASK_PRIO 20  //�������ȼ�
#define JUDGERECEIVE_STK_SIZE 512 //�����ջ3
TaskHandle_t JudgeReceiveTask_Handler; //������

#define CHASSIS_TASK_PRIO 18  //�������ȼ�
#define CHASSIS_STK_SIZE 512 //�����ջ4
TaskHandle_t ChassisTask_Handler; //������

#define JUMPCAL_TASK_PRIO 18  //�������ȼ�
#define JUMPCAL_TASK_SIZE 512 //�����ջ4
TaskHandle_t JumpCalTask_Handler; //������

/**********************************************************************************************************
*�� �� ��: start_task
*����˵��: ������������
*��    ��: *pvParameters
*�� �� ֵ: ��
**********************************************************************************************************/
void start_task(void *pvParameters)
{
  taskENTER_CRITICAL();//�����ٽ���
//								
//	xTaskCreate((TaskFunction_t)CPU_task,          //������
//                (const char *)"CPU_task",          //��������
//                (uint16_t)CPU_STK_SIZE,            //�����ջ��С
//                (void *)NULL,                        //���ݸ��������Ĳ���
//                (UBaseType_t)CPU_TASK_PRIO,        //�������ȼ�
//                (TaskHandle_t *)&CPUTask_Handler); //������
								
	xTaskCreate((TaskFunction_t)CharSendtask,          //������
								(const char *)"Char_task",          //��������
								(uint16_t)CHAR_STK_SIZE,            //�����ջ��С
								(void *)NULL,                        //���ݸ��������Ĳ���
								(UBaseType_t)CHAR_TASK_PRIO,        //�������ȼ�
									(TaskHandle_t *)&Char_Handler); //������
							
	xTaskCreate((TaskFunction_t)GraphicSendtask,          //������
								(const char *)"Graphic_task",          //��������
								(uint16_t)GRAPHIC_STK_SIZE,            //�����ջ��С
								(void *)NULL,                        //���ݸ��������Ĳ���
								(UBaseType_t)GRAPHIC_TASK_PRIO,        //�������ȼ�
									(TaskHandle_t *)&Graphic_Handler); //������
	
	xTaskCreate((TaskFunction_t)Chassis_task,          //������
                (const char *)"Chassis_task",          //��������
                (uint16_t)CHASSIS_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)CHASSIS_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&ChassisTask_Handler); //������
								
	xTaskCreate((TaskFunction_t)Offline_Check_task,          //������
                (const char *)"Offline_Check_task",          //��������
                (uint16_t)OFFLINE_CHECK_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)OFFLINE_CHECK_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&OfflineCheckTask_Handler); //������
								
	xTaskCreate((TaskFunction_t)PowerControl_task,          //������
                (const char *)"PowerControl_task",          //��������
                (uint16_t)POWERCONTROL_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)POWERCONTROL_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&PowerControlTask_Handler); //������
								
	xTaskCreate((TaskFunction_t)JudgeReceive_task,          //������
                (const char *)"JudgeReceive_task",          //��������
                (uint16_t)JUDGERECEIVE_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)JUDGERECEIVE_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&JudgeReceiveTask_Handler); //������
								
//		xTaskCreate((TaskFunction_t)JumpCal_task,          //������
//                (const char *)"JumpCal_task",          //��������
//                (uint16_t)JUMPCAL_TASK_SIZE,            //�����ջ��С
//                (void *)NULL,                        //���ݸ��������Ĳ���
//                (UBaseType_t)JUMPCAL_TASK_PRIO,        //�������ȼ�
//                (TaskHandle_t *)&JumpCalTask_Handler); //������
//								
	vTaskDelete(StartTask_Handler); //ɾ����ʼ����
								
  taskEXIT_CRITICAL();            //�˳��ٽ���
}

uint8_t CPU_RunInfo1[200]; //������������ʱ����Ϣ �ֱ��ǣ������� ����״̬ ���ȼ� ʣ��ջ �������
uint8_t CPU_RunInfo2[200]; //������������ʱ����Ϣ �ֱ��ǣ������� ���м���  ʹ����
/**********************************************************************************************************
*�� �� ��: CPU_task
*����˵��: CPU����
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void CPU_task(void *pvParameters)
{
	
   while (1) {
    memset(CPU_RunInfo1,0,200); //��Ϣ����������
 
    vTaskList((char *)&CPU_RunInfo1); //��ȡ��������ʱ����Ϣ
    
    memset(CPU_RunInfo2,0,200); //��Ϣ����������
 
    vTaskGetRunTimeStats((char *)&CPU_RunInfo2);
    vTaskDelay(1000); /* ��ʱ 500 �� tick */
		 
#if INCLUDE_uxTaskGetStackHighWaterMark
        CPU_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}
/**********************************************************************************************************
*�� �� ��: startTast
*����˵��: ������ʼ������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void startTast(void)
{
    xTaskCreate((TaskFunction_t)start_task,          //������
                (const char *)"start_task",          //��������
                (uint16_t)START_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)START_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&StartTask_Handler); //������
}


