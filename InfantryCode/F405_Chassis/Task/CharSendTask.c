#include "main.h"
extern unsigned char JudgeSend[SEND_MAX_SIZE];
extern JudgeReceive_t JudgeReceive;
extern F405_typedef F405;
extern ChassisSpeed_t chassis;
int Char_Change_Array[7];					//0表示没变化，非0表示有变化
char LowHP_Flag,lastLowHP_Flag;									//低血量警报
ext_student_interactive_char_header_data_t custom_char_draw;  //自定义字符绘制

/**********************************************************************************************************
*函 数 名: JudgementCustomizeChar
*功能说明: 图形数据包分段打包
*形    参: 操作类型
*返 回 值: 无
**********************************************************************************************************/
void JudgementCustomizeChar(int Op_type)
{
		custom_char_draw.data_cmd_id=0x0110;//绘制字符

		custom_char_draw.sender_ID=JudgeReceive.robot_id;//发送者ID，机器人对应ID
		if(JudgeReceive.robot_id == 103)
				custom_char_draw.receiver_ID = 0x0167;
		if(JudgeReceive.robot_id == 104)
				custom_char_draw.receiver_ID = 0x0168;
		if(JudgeReceive.robot_id == 105)
				custom_char_draw.receiver_ID = 0x0169;
		if(JudgeReceive.robot_id == 3)
				custom_char_draw.receiver_ID = 0x0103;	
		if(JudgeReceive.robot_id == 4)
				custom_char_draw.receiver_ID = 0x0104;
		if(JudgeReceive.robot_id == 5)
				custom_char_draw.receiver_ID = 0x0105;

/*********************************自定义字符数据***********************************/
		referee_data_load_String(Op_type);
}

/**********************************************************************************************************
*函 数 名: referee_data_load_String
*功能说明: 图形数据包装载字符串
*形    参: 操作类型
*返 回 值: 无
**********************************************************************************************************/
float c_pos_x[12] = {0.57,0.34,0.4,0.54,0.3, 0.41,0.64, 0.54,0.40,0.53,0.3,0.4};
float c_pos_y[12] = {0.65,0.15,0.05,0.1 ,0.1, 0.15,0.1, 0.05,0.1 ,0.15,0.5,0.7};
extern enum POWERSTATE_Typedef PowerState;
char change_cnt[7];
void referee_data_load_String(int Op_type)
{
	static int tick=0;
	static char Mag_State[2][6] = {"CLOSE","OPEN"};
	static char Gimbal_State[9][9] = {"OFF","Normal","Armor","BigBuf","Drop","SI","Jump","AutoShot","SmalBuf"};
	static char Chassis_State[5][9] = {"OFF","Normal","SelfPro","SOLO","JUMP"};
	static char Power_State[2][4] = {"Bat","Cap"};
	static char Freq_State[2][7] = {"Normal","High"};
	static char Friction[2][9]={"Firc off","Fric on"};
	/*初始化操作，轮流生成图层*/
	if(Op_type == Op_Init)
	{
		switch(tick%11)
		{
			/*静态字符*/
			case 0:
			/*******************************pitch 字符*********************************/
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 0;
			custom_char_draw.char_custom.grapic_data_struct.operate_tpye=Op_type;
			custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;
			custom_char_draw.char_custom.grapic_data_struct.layer=9;
			custom_char_draw.char_custom.grapic_data_struct.color=Orange;
			custom_char_draw.char_custom.grapic_data_struct.start_angle=25;
			custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen("PITCH:");
			custom_char_draw.char_custom.grapic_data_struct.width=2;
			custom_char_draw.char_custom.grapic_data_struct.start_x=c_pos_x[0]*SCREEN_LENGTH;
			custom_char_draw.char_custom.grapic_data_struct.start_y=c_pos_y[0]*SCREEN_WIDTH;
			memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
			strcpy(custom_char_draw.char_custom.data,"PITCH:");
			break;
			case 1:
			/*******************************Magazine 字符*********************************/
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 1;
			custom_char_draw.char_custom.grapic_data_struct.operate_tpye=Op_type;
			custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;
			custom_char_draw.char_custom.grapic_data_struct.layer=9;
			custom_char_draw.char_custom.grapic_data_struct.color=Orange;
			custom_char_draw.char_custom.grapic_data_struct.start_angle=25;
			custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen("MAG:");
			custom_char_draw.char_custom.grapic_data_struct.width=2;
			custom_char_draw.char_custom.grapic_data_struct.start_x=c_pos_x[1]*SCREEN_LENGTH;
			custom_char_draw.char_custom.grapic_data_struct.start_y=c_pos_y[1]*SCREEN_WIDTH;
			memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
			strcpy(custom_char_draw.char_custom.data,"MAG:");
			break;
			case 2:
			/*******************************Chassis 字符*********************************/
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 2;
			custom_char_draw.char_custom.grapic_data_struct.operate_tpye=Op_type;
			custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;
			custom_char_draw.char_custom.grapic_data_struct.layer=9;
			custom_char_draw.char_custom.grapic_data_struct.color=Orange;
			custom_char_draw.char_custom.grapic_data_struct.start_angle=25;
			custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen("CHASSIS:");
			custom_char_draw.char_custom.grapic_data_struct.width=3;
			custom_char_draw.char_custom.grapic_data_struct.start_x=c_pos_x[2]*SCREEN_LENGTH;
			custom_char_draw.char_custom.grapic_data_struct.start_y=c_pos_y[2]*SCREEN_WIDTH;
			memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
			strcpy(custom_char_draw.char_custom.data,"CHASSIS:");
			break;
			case 3:
			/*******************************Gimbal 字符*********************************/
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 3;
			custom_char_draw.char_custom.grapic_data_struct.operate_tpye=Op_type;
			custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;
			custom_char_draw.char_custom.grapic_data_struct.layer=9;
			custom_char_draw.char_custom.grapic_data_struct.color=Orange;
			custom_char_draw.char_custom.grapic_data_struct.start_angle=25;
			custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen("GIMBAL:");
			custom_char_draw.char_custom.grapic_data_struct.width=3;
			custom_char_draw.char_custom.grapic_data_struct.start_x=c_pos_x[3]*SCREEN_LENGTH;
			custom_char_draw.char_custom.grapic_data_struct.start_y=c_pos_y[3]*SCREEN_WIDTH;
			memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
			strcpy(custom_char_draw.char_custom.data,"GIMBAL:");
			break;
			case 4:
/*******************************能量状态*********************************/
PS:		custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 4;
			custom_char_draw.char_custom.grapic_data_struct.operate_tpye=Op_type;
			custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;
			custom_char_draw.char_custom.grapic_data_struct.layer=9;
			custom_char_draw.char_custom.grapic_data_struct.color=Orange;
			custom_char_draw.char_custom.grapic_data_struct.start_angle=25;
			custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen(Power_State[PowerState]);
			custom_char_draw.char_custom.grapic_data_struct.width=3;
			custom_char_draw.char_custom.grapic_data_struct.start_x=c_pos_x[4]*SCREEN_LENGTH;
			custom_char_draw.char_custom.grapic_data_struct.start_y=c_pos_y[4]*SCREEN_WIDTH;
			memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
			strcpy(custom_char_draw.char_custom.data,Power_State[PowerState]);
			break;
			/*可变状态字符*/
			case 5:
			/*******************************弹仓盖开关状态*********************************/
MAG:  custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 5;
			custom_char_draw.char_custom.grapic_data_struct.operate_tpye=Op_type;
			custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;
			custom_char_draw.char_custom.grapic_data_struct.layer=9;
			custom_char_draw.char_custom.grapic_data_struct.color=Green;
			custom_char_draw.char_custom.grapic_data_struct.start_angle=25;
			custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen(Mag_State[F405.Mag_Flag]);
			custom_char_draw.char_custom.grapic_data_struct.width=2;
			custom_char_draw.char_custom.grapic_data_struct.start_x=c_pos_x[5]*SCREEN_LENGTH;
			custom_char_draw.char_custom.grapic_data_struct.start_y=c_pos_y[5]*SCREEN_WIDTH;
			memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
			strcpy(custom_char_draw.char_custom.data,Mag_State[F405.Mag_Flag]);
			break;
			case 6:
			/*******************************云台状态*********************************/
GIMBAL:custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 6;
			custom_char_draw.char_custom.grapic_data_struct.operate_tpye=Op_type;
			custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;
			custom_char_draw.char_custom.grapic_data_struct.layer=9;
			custom_char_draw.char_custom.grapic_data_struct.color=Green;
			custom_char_draw.char_custom.grapic_data_struct.start_angle=25;
			custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen(Gimbal_State[F405.Gimbal_Flag]);
			custom_char_draw.char_custom.grapic_data_struct.width=2;
			custom_char_draw.char_custom.grapic_data_struct.start_x=c_pos_x[6]*SCREEN_LENGTH;
			custom_char_draw.char_custom.grapic_data_struct.start_y=c_pos_y[6]*SCREEN_WIDTH;
			memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
			strcpy(custom_char_draw.char_custom.data,Gimbal_State[F405.Gimbal_Flag]);
			break;
			case 7:
			/*******************************底盘状态*********************************/
CHASSIS:custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 7;
			custom_char_draw.char_custom.grapic_data_struct.operate_tpye=Op_type;
			custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;
			custom_char_draw.char_custom.grapic_data_struct.layer=9;
			custom_char_draw.char_custom.grapic_data_struct.color=Green;
			custom_char_draw.char_custom.grapic_data_struct.start_angle=25;
			custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen(Chassis_State[F405.Chassis_Flag]);
			custom_char_draw.char_custom.grapic_data_struct.width=2;
			custom_char_draw.char_custom.grapic_data_struct.start_x=c_pos_x[7]*SCREEN_LENGTH;
			custom_char_draw.char_custom.grapic_data_struct.start_y=c_pos_y[7]*SCREEN_WIDTH;
			memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
			strcpy(custom_char_draw.char_custom.data,Chassis_State[F405.Chassis_Flag]);
			break;
			case 8:
		/*******************************摩擦轮*********************************/
Fric: 
			
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 8;
			custom_char_draw.char_custom.grapic_data_struct.operate_tpye=Op_type;
			custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;
			custom_char_draw.char_custom.grapic_data_struct.layer=9;
			custom_char_draw.char_custom.grapic_data_struct.color=Orange;
			custom_char_draw.char_custom.grapic_data_struct.start_angle=25;
			custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen(Friction[F405.Fric_Flag]);
			custom_char_draw.char_custom.grapic_data_struct.width=3;
			custom_char_draw.char_custom.grapic_data_struct.start_x=c_pos_x[8]*SCREEN_LENGTH;
			custom_char_draw.char_custom.grapic_data_struct.start_y=c_pos_y[8]*SCREEN_WIDTH;
			memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
			strcpy(custom_char_draw.char_custom.data,Friction[F405.Fric_Flag]);
			break;
			case 9:
			/*******************************电容电压字符*********************************/
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 9;
			custom_char_draw.char_custom.grapic_data_struct.operate_tpye=Op_type;
			custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;
			custom_char_draw.char_custom.grapic_data_struct.layer=9;
			custom_char_draw.char_custom.grapic_data_struct.color=Orange;
			custom_char_draw.char_custom.grapic_data_struct.start_angle=25;
			custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen("CAP:     V");
			custom_char_draw.char_custom.grapic_data_struct.width=3;
			custom_char_draw.char_custom.grapic_data_struct.start_x=c_pos_x[9]*SCREEN_LENGTH;
			custom_char_draw.char_custom.grapic_data_struct.start_y=c_pos_y[9]*SCREEN_WIDTH;
			memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
			strcpy(custom_char_draw.char_custom.data,"CAP:     V");
			break;			
			/*******************************电容电压字符*********************************/			
			case 10:
Freq: custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 11;
			custom_char_draw.char_custom.grapic_data_struct.operate_tpye=Op_type;
			custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;
			custom_char_draw.char_custom.grapic_data_struct.layer=9;
			custom_char_draw.char_custom.grapic_data_struct.color= F405.Freq_state?Orange:Green;
			custom_char_draw.char_custom.grapic_data_struct.start_angle=25;
			custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen(Freq_State[F405.Freq_state]);
			custom_char_draw.char_custom.grapic_data_struct.width= 2;
			custom_char_draw.char_custom.grapic_data_struct.start_x=c_pos_x[11]*SCREEN_LENGTH;
			custom_char_draw.char_custom.grapic_data_struct.start_y=c_pos_y[11]*SCREEN_WIDTH;
			memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
			strcpy(custom_char_draw.char_custom.data,Freq_State[F405.Freq_state]);
			break;
		
			default:
			break;
		}
		tick++;
		return ;
		
	}else if(Op_type == Op_Change)		//如果是标志为修改
	{
		/*寻找是哪个发生了变化*/
		if(Char_Change_Array[0] == Op_Change)
		{
			if(change_cnt[0]>0)
			{
			 change_cnt[0] -- ;
			}
			else
			{
			Char_Change_Array[0] = Op_None;
			}
			goto MAG;
		}
		
		if(Char_Change_Array[1] == Op_Change)
		{
			if(change_cnt[1]>0)
			{
			 change_cnt[1] -- ;
			}
			else
			{
			Char_Change_Array[1] = Op_None;
			}
			goto GIMBAL;
		}
		
 		if(Char_Change_Array[2] == Op_Change)
		{
			if(change_cnt[2]>0)
			{
			 change_cnt[2] -- ;
			}
			else
			{
			Char_Change_Array[2] = Op_None;
			}
			goto CHASSIS;
		}
		
		if(Char_Change_Array[3] == Op_Change)
		{
			if(change_cnt[3]>0)
			{
			 change_cnt[3] -- ;
			}
			else
			{
			Char_Change_Array[3] = Op_None;
			}
			goto Fric;
		}
		
		if(Char_Change_Array[4] == Op_Change)
		{
			if(change_cnt[4]>0)
			{
			 change_cnt[4] -- ;
			}
			else
			{
			Char_Change_Array[4] = Op_None;
			}
			goto PS;	
		}
		
		if(Char_Change_Array[5] == Op_Change)  
		{
			if(change_cnt[5]>0)
			{
			 change_cnt[5] -- ;
			}
			else
			{
			Char_Change_Array[5] = Op_None;
			}
			/*******************************绘制删除 RUN 字符*********************************/
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 10;
			custom_char_draw.char_custom.grapic_data_struct.operate_tpye= (LowHP_Flag == 1 ?Op_Add : Op_Delete);
			custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;
			custom_char_draw.char_custom.grapic_data_struct.layer=8;
			custom_char_draw.char_custom.grapic_data_struct.color= Orange;
			custom_char_draw.char_custom.grapic_data_struct.start_angle=50;    //字体大小  
			custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen("RUN !!");
			custom_char_draw.char_custom.grapic_data_struct.width=6;           //宽度
			custom_char_draw.char_custom.grapic_data_struct.start_x=c_pos_x[10]*SCREEN_LENGTH;
			custom_char_draw.char_custom.grapic_data_struct.start_y=c_pos_y[10]*SCREEN_WIDTH;
			memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
			strcpy(custom_char_draw.char_custom.data,"RUN !!");
		}
		
		if(Char_Change_Array[6] == Op_Change)
		{
			if(change_cnt[6]>0)
			{
			 change_cnt[6] -- ;
			}
			else
			{
			Char_Change_Array[6] = Op_None;
			}
			goto Freq;	
		}
	}
}

/**********************************************************************************************************
*函 数 名: Char_Change_Check
*功能说明: 检测有没有字符的变动，或者进行字符图层的初始化
*形    参: 无
*返 回 值: 
**********************************************************************************************************/

int Char_Change_Check(void)
{
	int i;
	static char last_Mag,last_Gimbal,last_Chassis,last_Laser,last_Freq,last_Fric,last_PowerState;	//记录上次状态
	static int delete_flag;
	char Mag_flag,Gimbal_flag,Chassis_flag,Laser_flag,Freq_state,Fric_flag;

	/*用于图形界面初始化*/
	if(F405.Graphic_Init_Flag == 0)		
	{
//		delete_flag = 0;
		return Op_Init;	//返回Init,会使一直发送Add，添加所有图层
	}
//	else delete_flag++;
	
		
	/*读取云台发送的各种状态*/
	Mag_flag = F405.Mag_Flag;
	Gimbal_flag = F405.Gimbal_Flag;
	Chassis_flag = F405.Chassis_Flag;
	Fric_flag = F405.Fric_Flag;
	Freq_state = F405.Freq_state;
	LowHP_Flag = JudgeReceive.maxHP * 0.35 > JudgeReceive.remainHP ? 1:0;
	
	/*有变化，标志各个位*/
	if(last_Mag != Mag_flag) 
	{
		Char_Change_Array[0] = Op_Change;
    change_cnt[0] = 2;               // 修改2次避免丢帧
	}
	if(Gimbal_flag != last_Gimbal) 
	{
			Char_Change_Array[1] = Op_Change;
			change_cnt[1] = 2;
	}
	if(Chassis_flag != last_Chassis )
	{
			Char_Change_Array[2] = Op_Change;
      change_cnt[2] = 2;
	}
  if(Fric_flag != last_Fric ) 
	{
		Char_Change_Array[3]=Op_Change;	
		change_cnt[3] = 2;
	}
	if(PowerState != last_PowerState) 
	{
		Char_Change_Array[4]=Op_Change;
		change_cnt[4] = 2;
	}
	if(LowHP_Flag != lastLowHP_Flag)
	{
		Char_Change_Array[5]=Op_Change;
		change_cnt[5] = 2;	  
	}
	if(Freq_state != last_Freq)
	{
		Char_Change_Array[6]=Op_Change;
		change_cnt[6] = 2;	  
	}
	
	
	/*保存这次标志和上次比较*/
	last_Mag = Mag_flag;
	last_Gimbal = Gimbal_flag;
	last_Chassis = Chassis_flag;
	last_Fric = Fric_flag;
	last_Freq = Freq_state;
	last_PowerState=PowerState;
	lastLowHP_Flag = LowHP_Flag;
	
	
	/*检索有没有发生变化，如果有变化则返回修改图层*/
	for(i = 0;i<7;i++)
	{
		if(Char_Change_Array[i] == Op_Change)
			return Op_Change;
	}
	
	return Op_None;	//否则返回空操作，此时不会发送东西
}
/**********************************************************************************************************
*函 数 名: Load_Char_Init
*功能说明: 装载字符初始化提示
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Load_Char_Init(char Init_Flag)
{
	custom_char_draw.data_cmd_id=0x0110;//绘制字符

	custom_char_draw.sender_ID=JudgeReceive.robot_id;//发送者ID，机器人对应ID
	if(JudgeReceive.robot_id == 103)
			custom_char_draw.receiver_ID = 0x0167;
	if(JudgeReceive.robot_id == 104)
			custom_char_draw.receiver_ID = 0x0168;
	if(JudgeReceive.robot_id == 105)
			custom_char_draw.receiver_ID = 0x0169;
	if(JudgeReceive.robot_id == 3)
			custom_char_draw.receiver_ID = 0x0103;	
	if(JudgeReceive.robot_id == 4)
			custom_char_draw.receiver_ID = 0x0104;
	if(JudgeReceive.robot_id == 5)
			custom_char_draw.receiver_ID = 0x0105;
	
	/*******************************Init:B 字符*********************************/
	custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;  //作为客户端索引的图形名
	custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
	custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 9;
	custom_char_draw.char_custom.grapic_data_struct.operate_tpye=Init_Flag?Op_Delete:Op_Add;
	custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;         //类型：字符
	custom_char_draw.char_custom.grapic_data_struct.layer=9;                //图层数：
	custom_char_draw.char_custom.grapic_data_struct.color=Green;
	custom_char_draw.char_custom.grapic_data_struct.start_angle=25;            //字体大小
	custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen("Initing Press B To Hide"); //字符长度
	custom_char_draw.char_custom.grapic_data_struct.width=4;                   //线条宽度
	custom_char_draw.char_custom.grapic_data_struct.start_x=0.34*SCREEN_LENGTH; //起点x坐标
	custom_char_draw.char_custom.grapic_data_struct.start_y=0.56*SCREEN_WIDTH;  //起点y坐标
	custom_char_draw.char_custom.grapic_data_struct.end_x=0;                   
	custom_char_draw.char_custom.grapic_data_struct.end_y=0;
	custom_char_draw.char_custom.grapic_data_struct.radius = 0;
	memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
	strcpy(custom_char_draw.char_custom.data,"Initing Press B To Hide");
}
/**********************************************************************************************************
*函 数 名: CharSendtask
*功能说明: 字符发送任务
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
int charTask_cnt=0;
void CharSendtask(void *pvParameters)
{
	 static int char_change_state;
   while (1) {
    
			/*字符，英文*/
			char_change_state = Char_Change_Check();
			if(char_change_state)			//检查有没有变化，没有变化就不发，节省带宽
			{
				JudgementCustomizeChar(char_change_state);
				if(char_change_state != Op_None)
					referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&custom_char_draw,sizeof(custom_char_draw));
			}
			
		  IWDG_Feed();//喂狗
			charTask_cnt++;
			vTaskDelay(50); 
		 
#if INCLUDE_uxTaskGetStackHighWaterMark
		
#endif
    }
}
