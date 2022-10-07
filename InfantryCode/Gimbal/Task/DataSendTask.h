#ifndef __DATASENDTASK_H__
#define __DATASENDTASK_H__

typedef struct{
	char SuperPowerLimit;	//0为超级电容关闭，不为0则开启使用超级电容
	char Chassis_Flag;			//0代表正常模式  1代表小陀螺  2代表单挑模式  3表示掉电
	char Mag_Flag;					//0表示弹仓盖关闭，1为打开
	char Laser_Flag;				//0表示激光关闭，1为打开
	short Gimbal_100;				//pitch角度,乘了10之后发
	char Gimbal_Flag;				//0为正常模式  1为辅瞄
	char Graphic_Init_Flag;	//0为进入初始化模式，1为初始化结束
	char Freq_state;			  //0表示正常射频 1表示高射频
	/*打包数据*/
	char Send_Pack1;	
  char Fric_Flag;	
}F405_typedef;

void BodanCan1Send(short a);
void F405Can1Send(F405_typedef *F405_Send);
void GimbalCan2Send(short X,short Y);
void ChassisCan1Send(short *carSpeedx,short *carSpeedy,short *carSpeedw);

void USART6_SendtoPC(void);
void Tx2_Off_CheckAndSet(u8* Buff);
void TX2_task(void *pvParameters);
void FrictionBodanCan2Send(short X,short Y,short Z);
#define Tx2_Off 0xff				//关机模式
#define Tx2_Small_Buff 0x10	//小符模式
#define Tx2_Big_Buff 0x20		//大符模式
#define Tx2_Armor 0x30			//辅瞄模式

#endif
