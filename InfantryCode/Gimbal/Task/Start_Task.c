#include "Start_Task.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tim7.h"
unsigned portBASE_TYPE Mark[8];		//观察任务堆栈使用情况
uint32_t CPU_high_water;

/*任务优先级数值越低，优先级越低*/

#define START_TASK_PRIO 1  //任务优先级
#define START_STK_SIZE 128 //任务堆栈
static TaskHandle_t StartTask_Handler; //任务句柄

#define CPU_TASK_PRIO 2  //任务优先级
#define CPU_STK_SIZE 128 //任务堆栈w
static TaskHandle_t CPUTask_Handler; //任务句柄

#define GIMBAL_TASK_PRIO 20  //任务优先级
#define GIMBAL_STK_SIZE 128 //任务堆栈
static TaskHandle_t GimbalTask_Handler; //任务句柄

#define CHASSIS_TASK_PRIO 18  //任务优先级
#define CHASSIS_STK_SIZE 128 //任务堆栈
static TaskHandle_t ChassisTask_Handler; //任务句柄

#define OFFLINE_CHECK_TASK_PRIO 3  //任务优先级
#define OFFLINE_CHECK_STK_SIZE 128 //任务堆栈
static TaskHandle_t OfflineCheckTask_Handler; //任务句柄

#define RCRECEIVE_TASK_PRIO 25  //任务优先级
#define RCRECEIVE_STK_SIZE 128 //任务堆栈
TaskHandle_t RCReceiveTask_Handler; //任务句柄

#define PCRECEIVE_TASK_PRIO 25  //任务优先级
#define PCRECEIVE_STK_SIZE 128 //任务堆栈
TaskHandle_t PCReceiveTask_Handler; //任务句柄

#define MODE_CHOOSE_TASK_PRIO 16  //任务优先级
#define MODE_CHOOSE_STK_SIZE 128 //任务堆栈
static TaskHandle_t ModeChooseTask_Handler; //任务句柄

#define SHOOT_TASK_PRIO 17  //任务优先级
#define SHOOT_STK_SIZE 128 //任务堆栈
static TaskHandle_t ShootTask_Handler; //任务句柄

#define TX2_TASK_PRIO 19  //任务优先级
#define TX2_STK_SIZE 128 //任务堆栈
static TaskHandle_t TX2Task_Handler; //任务句柄


void start_task(void *pvParameters)
{
  taskENTER_CRITICAL();
								
	xTaskCreate((TaskFunction_t)Chassis_task,          //任务函数
                (const char *)"Chassis_task",          //任务名称
                (uint16_t)CHASSIS_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)CHASSIS_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&ChassisTask_Handler); //任务句柄

	xTaskCreate((TaskFunction_t)Gimbal_task,          //任务函数
                (const char *)"Gimbal_task",          //任务名称
                (uint16_t)GIMBAL_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)GIMBAL_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&GimbalTask_Handler); //任务句柄
								
	xTaskCreate((TaskFunction_t)Shoot_task,          //任务函数
                (const char *)"Shoot_task",          //任务名称
                (uint16_t)SHOOT_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)SHOOT_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&ShootTask_Handler); //任务句柄
								
	xTaskCreate((TaskFunction_t)ModeChoose_task,          //任务函数
                (const char *)"ModeChoose_task",          //任务名称
                (uint16_t)MODE_CHOOSE_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)MODE_CHOOSE_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&ModeChooseTask_Handler); //任务句柄
								
	xTaskCreate((TaskFunction_t)TX2_task,          //任务函数
                (const char *)"TX2_task",          //任务名称
                (uint16_t)TX2_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)TX2_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&TX2Task_Handler); //任务句柄
								
	xTaskCreate((TaskFunction_t)Offline_Check_task,          //任务函数
                (const char *)"Offline_Check_task",          //任务名称
                (uint16_t)OFFLINE_CHECK_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)OFFLINE_CHECK_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&OfflineCheckTask_Handler); //任务句柄
								
	xTaskCreate((TaskFunction_t)RCReceive_task,            //任务函数
                (const char *)"RCReceive_task",          //任务名称
                (uint16_t)RCRECEIVE_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)RCRECEIVE_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&RCReceiveTask_Handler); //任务句柄
	
	xTaskCreate((TaskFunction_t)PCReceive_task,            //任务函数
                (const char *)"PCReceive_task",          //任务名称
                (uint16_t)PCRECEIVE_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)PCRECEIVE_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&PCReceiveTask_Handler); //任务句柄
								
//	xTaskCreate((TaskFunction_t)CPU_task,          //任务函数
//                (const char *)"CPU_task",          //任务名称
//                (uint16_t)CPU_STK_SIZE,            //任务堆栈大小
//                (void *)NULL,                        //传递给任务函数的参数
//                (UBaseType_t)CPU_TASK_PRIO,        //任务优先级
//                (TaskHandle_t *)&CPUTask_Handler); //任务句柄
//								
								
	vTaskDelete(StartTask_Handler); //删除开始任务
  taskEXIT_CRITICAL();            //退出临界区
}


uint8_t CPU_RunInfo1[200]; //保存任务运行时间信息 分别是：任务名 任务状态 优先级 剩余栈 任务序号
uint8_t CPU_RunInfo2[200]; //保存任务运行时间信息 分别是：任务名 运行计数  使用率

void CPU_task(void *pvParameters)
{
	
   while (1) {

#if INCLUDE_uxTaskGetStackHighWaterMark
    memset(CPU_RunInfo1,0,200); //信息缓冲区清零
 
    vTaskList((char *)&CPU_RunInfo1); //获取任务运行时间信息
    
    memset(CPU_RunInfo2,0,200); //信息缓冲区清零
 		Mark[0] = uxTaskGetStackHighWaterMark(StartTask_Handler );
		Mark[1] = uxTaskGetStackHighWaterMark(CPUTask_Handler);
		Mark[2] = uxTaskGetStackHighWaterMark(GimbalTask_Handler);
		Mark[3] = uxTaskGetStackHighWaterMark(ChassisTask_Handler);
		Mark[4] = uxTaskGetStackHighWaterMark(OfflineCheckTask_Handler);
		Mark[5] = uxTaskGetStackHighWaterMark(ModeChooseTask_Handler);
		Mark[6] = uxTaskGetStackHighWaterMark(ShootTask_Handler);
		Mark[7] = uxTaskGetStackHighWaterMark(TX2Task_Handler);
    CPU_high_water = uxTaskGetStackHighWaterMark(NULL);
    vTaskGetRunTimeStats((char *)&CPU_RunInfo2);
    vTaskDelay(1000); /* 延时 500 个 tick */
#endif
    }
}

void startTast(void)
{
    xTaskCreate((TaskFunction_t)start_task,          //任务函数
                (const char *)"start_task",          //任务名称
                (uint16_t)START_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)START_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&StartTask_Handler); //任务句柄
}
static char Lost=0;
void Robot_Stop()
{
   if(Lost==0)
	{	
	  taskENTER_CRITICAL();
		vTaskSuspend(ChassisTask_Handler);
		vTaskSuspend(GimbalTask_Handler);
		vTaskSuspend(ShootTask_Handler);
		FrictionBodanCan2Send(0,0,0);
		Lost=1;
		taskEXIT_CRITICAL();
	}
}

void Robot_Recover()
{
 if(Lost==1)
 {
  taskENTER_CRITICAL();
  vTaskResume(ChassisTask_Handler);
	vTaskResume(GimbalTask_Handler);
	vTaskResume(ShootTask_Handler);
	Lost=0;
  taskEXIT_CRITICAL();
 }

}
static char Shoot_Lost=0;
void Shoot_Stop()
{
   if(Shoot_Lost==0)
	{	
	  taskENTER_CRITICAL();
		vTaskSuspend(ShootTask_Handler);
		FrictionBodanCan2Send(0,0,0);
		Shoot_Lost=1;
		taskEXIT_CRITICAL();
	}
}

void Shoot_Recover()
{
 if(Shoot_Lost==1)
 {
  taskENTER_CRITICAL();
	vTaskResume(ShootTask_Handler);
	Shoot_Lost=0;
  taskEXIT_CRITICAL();
 }

}
extern float aim_yaw, aim_pitch;
extern PC_Receive_t PC_Receive;
extern Gimbal_Typedef Gimbal;
extern short armor_state;
extern Status_t Status;
void PC_Rst()
{
	if(Status.GimbalMode == Gimbal_BigBuf_Mode || Status.GimbalMode == Gimbal_SmlBuf_Mode)
	{
	PC_Receive.RCYaw = Gimbal.Yaw.MotorTransAngle;
	aim_yaw = Gimbal.Yaw.MotorTransAngle;
	}else
	{
	PC_Receive.RCYaw = Gimbal.Yaw.Gyro;
	aim_yaw = Gimbal.Yaw.Gyro;	
	}
	PC_Receive.RCPitch = Gimbal.Pitch.MotorTransAngle;
	aim_pitch = Gimbal.Pitch.MotorTransAngle;
	armor_state = ARMOR_NO_AIM;
}