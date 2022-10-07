/**********************************************************************************************************
 * @文件     StartTask.c
 * @说明     操作系统启动文件
 * @版本  	 V1.0
 * @作者     陈志鹏
 * @日期     2020.1
**********************************************************************************************************/
#include "main.h"
uint32_t CPU_high_water;

/*任务优先级数值越低，优先级越低*/

#define START_TASK_PRIO 1  //任务优先级
#define START_STK_SIZE 512 //任务堆栈1
static TaskHandle_t StartTask_Handler; //任务句柄

#define CPU_TASK_PRIO 2  //任务优先级
#define CPU_STK_SIZE 128 //任务堆栈2
static TaskHandle_t CPUTask_Handler; //任务句柄

#define CHAR_TASK_PRIO 17  //任务优先级
#define CHAR_STK_SIZE 128 //任务堆栈
static TaskHandle_t Char_Handler; //任务句柄

#define GRAPHIC_TASK_PRIO 17  //任务优先级
#define GRAPHIC_STK_SIZE 128 //任务堆栈
static TaskHandle_t Graphic_Handler; //任务句柄

#define OFFLINE_CHECK_TASK_PRIO 18  //任务优先级
#define OFFLINE_CHECK_STK_SIZE 128 //任务堆栈
static TaskHandle_t OfflineCheckTask_Handler; //任务句柄

#define POWERCONTROL_TASK_PRIO 17  //任务优先级
#define POWERCONTROL_STK_SIZE 512 //任务堆栈3
TaskHandle_t PowerControlTask_Handler; //任务句柄

#define JUDGERECEIVE_TASK_PRIO 20  //任务优先级
#define JUDGERECEIVE_STK_SIZE 512 //任务堆栈3
TaskHandle_t JudgeReceiveTask_Handler; //任务句柄

#define CHASSIS_TASK_PRIO 18  //任务优先级
#define CHASSIS_STK_SIZE 512 //任务堆栈4
TaskHandle_t ChassisTask_Handler; //任务句柄

#define JUMPCAL_TASK_PRIO 18  //任务优先级
#define JUMPCAL_TASK_SIZE 512 //任务堆栈4
TaskHandle_t JumpCalTask_Handler; //任务句柄

/**********************************************************************************************************
*函 数 名: start_task
*功能说明: 创建所有任务
*形    参: *pvParameters
*返 回 值: 无
**********************************************************************************************************/
void start_task(void *pvParameters)
{
  taskENTER_CRITICAL();//进入临界区
//								
//	xTaskCreate((TaskFunction_t)CPU_task,          //任务函数
//                (const char *)"CPU_task",          //任务名称
//                (uint16_t)CPU_STK_SIZE,            //任务堆栈大小
//                (void *)NULL,                        //传递给任务函数的参数
//                (UBaseType_t)CPU_TASK_PRIO,        //任务优先级
//                (TaskHandle_t *)&CPUTask_Handler); //任务句柄
								
	xTaskCreate((TaskFunction_t)CharSendtask,          //任务函数
								(const char *)"Char_task",          //任务名称
								(uint16_t)CHAR_STK_SIZE,            //任务堆栈大小
								(void *)NULL,                        //传递给任务函数的参数
								(UBaseType_t)CHAR_TASK_PRIO,        //任务优先级
									(TaskHandle_t *)&Char_Handler); //任务句柄
							
	xTaskCreate((TaskFunction_t)GraphicSendtask,          //任务函数
								(const char *)"Graphic_task",          //任务名称
								(uint16_t)GRAPHIC_STK_SIZE,            //任务堆栈大小
								(void *)NULL,                        //传递给任务函数的参数
								(UBaseType_t)GRAPHIC_TASK_PRIO,        //任务优先级
									(TaskHandle_t *)&Graphic_Handler); //任务句柄
	
	xTaskCreate((TaskFunction_t)Chassis_task,          //任务函数
                (const char *)"Chassis_task",          //任务名称
                (uint16_t)CHASSIS_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)CHASSIS_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&ChassisTask_Handler); //任务句柄
								
	xTaskCreate((TaskFunction_t)Offline_Check_task,          //任务函数
                (const char *)"Offline_Check_task",          //任务名称
                (uint16_t)OFFLINE_CHECK_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)OFFLINE_CHECK_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&OfflineCheckTask_Handler); //任务句柄
								
	xTaskCreate((TaskFunction_t)PowerControl_task,          //任务函数
                (const char *)"PowerControl_task",          //任务名称
                (uint16_t)POWERCONTROL_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)POWERCONTROL_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&PowerControlTask_Handler); //任务句柄
								
	xTaskCreate((TaskFunction_t)JudgeReceive_task,          //任务函数
                (const char *)"JudgeReceive_task",          //任务名称
                (uint16_t)JUDGERECEIVE_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)JUDGERECEIVE_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&JudgeReceiveTask_Handler); //任务句柄
								
//		xTaskCreate((TaskFunction_t)JumpCal_task,          //任务函数
//                (const char *)"JumpCal_task",          //任务名称
//                (uint16_t)JUMPCAL_TASK_SIZE,            //任务堆栈大小
//                (void *)NULL,                        //传递给任务函数的参数
//                (UBaseType_t)JUMPCAL_TASK_PRIO,        //任务优先级
//                (TaskHandle_t *)&JumpCalTask_Handler); //任务句柄
//								
	vTaskDelete(StartTask_Handler); //删除开始任务
								
  taskEXIT_CRITICAL();            //退出临界区
}

uint8_t CPU_RunInfo1[200]; //保存任务运行时间信息 分别是：任务名 任务状态 优先级 剩余栈 任务序号
uint8_t CPU_RunInfo2[200]; //保存任务运行时间信息 分别是：任务名 运行计数  使用率
/**********************************************************************************************************
*函 数 名: CPU_task
*功能说明: CPU任务
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void CPU_task(void *pvParameters)
{
	
   while (1) {
    memset(CPU_RunInfo1,0,200); //信息缓冲区清零
 
    vTaskList((char *)&CPU_RunInfo1); //获取任务运行时间信息
    
    memset(CPU_RunInfo2,0,200); //信息缓冲区清零
 
    vTaskGetRunTimeStats((char *)&CPU_RunInfo2);
    vTaskDelay(1000); /* 延时 500 个 tick */
		 
#if INCLUDE_uxTaskGetStackHighWaterMark
        CPU_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}
/**********************************************************************************************************
*函 数 名: startTast
*功能说明: 创建初始化任务
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void startTast(void)
{
    xTaskCreate((TaskFunction_t)start_task,          //任务函数
                (const char *)"start_task",          //任务名称
                (uint16_t)START_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)START_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&StartTask_Handler); //任务句柄
}


