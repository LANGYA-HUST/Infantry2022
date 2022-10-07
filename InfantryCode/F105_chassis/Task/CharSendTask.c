#include "main.h"
extern unsigned char JudgeSend[SEND_MAX_SIZE];
extern JudgeReceive_t JudgeReceive;
extern F405_typedef F405;
extern ChassisSpeed_t chassis;
int Char_Change_Array[7];					//0��ʾû�仯����0��ʾ�б仯
char LowHP_Flag,lastLowHP_Flag;									//��Ѫ������
ext_student_interactive_char_header_data_t custom_char_draw;  //�Զ����ַ�����

/**********************************************************************************************************
*�� �� ��: JudgementCustomizeChar
*����˵��: ͼ�����ݰ��ֶδ��
*��    ��: ��������
*�� �� ֵ: ��
**********************************************************************************************************/
void JudgementCustomizeChar(int Op_type)
{
		custom_char_draw.data_cmd_id=0x0110;//�����ַ�

		custom_char_draw.sender_ID=JudgeReceive.robot_id;//������ID�������˶�ӦID
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

/*********************************�Զ����ַ�����***********************************/
		referee_data_load_String(Op_type);
}

/**********************************************************************************************************
*�� �� ��: referee_data_load_String
*����˵��: ͼ�����ݰ�װ���ַ���
*��    ��: ��������
*�� �� ֵ: ��
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
	/*��ʼ����������������ͼ��*/
	if(Op_type == Op_Init)
	{
		switch(tick%11)
		{
			/*��̬�ַ�*/
			case 0:
			/*******************************pitch �ַ�*********************************/
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
			/*******************************Magazine �ַ�*********************************/
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
			/*******************************Chassis �ַ�*********************************/
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
			/*******************************Gimbal �ַ�*********************************/
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
/*******************************����״̬*********************************/
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
			/*�ɱ�״̬�ַ�*/
			case 5:
			/*******************************���ָǿ���״̬*********************************/
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
			/*******************************��̨״̬*********************************/
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
			/*******************************����״̬*********************************/
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
		/*******************************Ħ����*********************************/
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
			/*******************************���ݵ�ѹ�ַ�*********************************/
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
			/*******************************���ݵ�ѹ�ַ�*********************************/			
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
		
	}else if(Op_type == Op_Change)		//����Ǳ�־Ϊ�޸�
	{
		/*Ѱ�����ĸ������˱仯*/
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
			/*******************************����ɾ�� RUN �ַ�*********************************/
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 10;
			custom_char_draw.char_custom.grapic_data_struct.operate_tpye= (LowHP_Flag == 1 ?Op_Add : Op_Delete);
			custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;
			custom_char_draw.char_custom.grapic_data_struct.layer=8;
			custom_char_draw.char_custom.grapic_data_struct.color= Orange;
			custom_char_draw.char_custom.grapic_data_struct.start_angle=50;    //�����С  
			custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen("RUN !!");
			custom_char_draw.char_custom.grapic_data_struct.width=6;           //���
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
*�� �� ��: Char_Change_Check
*����˵��: �����û���ַ��ı䶯�����߽����ַ�ͼ��ĳ�ʼ��
*��    ��: ��
*�� �� ֵ: 
**********************************************************************************************************/

int Char_Change_Check(void)
{
	int i;
	static char last_Mag,last_Gimbal,last_Chassis,last_Laser,last_Freq,last_Fric,last_PowerState;	//��¼�ϴ�״̬
	static int delete_flag;
	char Mag_flag,Gimbal_flag,Chassis_flag,Laser_flag,Freq_state,Fric_flag;

	/*����ͼ�ν����ʼ��*/
	if(F405.Graphic_Init_Flag == 0)		
	{
//		delete_flag = 0;
		return Op_Init;	//����Init,��ʹһֱ����Add���������ͼ��
	}
//	else delete_flag++;
	
		
	/*��ȡ��̨���͵ĸ���״̬*/
	Mag_flag = F405.Mag_Flag;
	Gimbal_flag = F405.Gimbal_Flag;
	Chassis_flag = F405.Chassis_Flag;
	Fric_flag = F405.Fric_Flag;
	Freq_state = F405.Freq_state;
	LowHP_Flag = JudgeReceive.maxHP * 0.35 > JudgeReceive.remainHP ? 1:0;
	
	/*�б仯����־����λ*/
	if(last_Mag != Mag_flag) 
	{
		Char_Change_Array[0] = Op_Change;
    change_cnt[0] = 2;               // �޸�2�α��ⶪ֡
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
	
	
	/*������α�־���ϴαȽ�*/
	last_Mag = Mag_flag;
	last_Gimbal = Gimbal_flag;
	last_Chassis = Chassis_flag;
	last_Fric = Fric_flag;
	last_Freq = Freq_state;
	last_PowerState=PowerState;
	lastLowHP_Flag = LowHP_Flag;
	
	
	/*������û�з����仯������б仯�򷵻��޸�ͼ��*/
	for(i = 0;i<7;i++)
	{
		if(Char_Change_Array[i] == Op_Change)
			return Op_Change;
	}
	
	return Op_None;	//���򷵻ؿղ�������ʱ���ᷢ�Ͷ���
}
/**********************************************************************************************************
*�� �� ��: Load_Char_Init
*����˵��: װ���ַ���ʼ����ʾ
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Load_Char_Init(char Init_Flag)
{
	custom_char_draw.data_cmd_id=0x0110;//�����ַ�

	custom_char_draw.sender_ID=JudgeReceive.robot_id;//������ID�������˶�ӦID
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
	
	/*******************************Init:B �ַ�*********************************/
	custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;  //��Ϊ�ͻ���������ͼ����
	custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
	custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 9;
	custom_char_draw.char_custom.grapic_data_struct.operate_tpye=Init_Flag?Op_Delete:Op_Add;
	custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;         //���ͣ��ַ�
	custom_char_draw.char_custom.grapic_data_struct.layer=9;                //ͼ������
	custom_char_draw.char_custom.grapic_data_struct.color=Green;
	custom_char_draw.char_custom.grapic_data_struct.start_angle=25;            //�����С
	custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen("Initing Press B To Hide"); //�ַ�����
	custom_char_draw.char_custom.grapic_data_struct.width=4;                   //�������
	custom_char_draw.char_custom.grapic_data_struct.start_x=0.34*SCREEN_LENGTH; //���x����
	custom_char_draw.char_custom.grapic_data_struct.start_y=0.56*SCREEN_WIDTH;  //���y����
	custom_char_draw.char_custom.grapic_data_struct.end_x=0;                   
	custom_char_draw.char_custom.grapic_data_struct.end_y=0;
	custom_char_draw.char_custom.grapic_data_struct.radius = 0;
	memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
	strcpy(custom_char_draw.char_custom.data,"Initing Press B To Hide");
}
/**********************************************************************************************************
*�� �� ��: CharSendtask
*����˵��: �ַ���������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
int charTask_cnt=0;
void CharSendtask(void *pvParameters)
{
	 static int char_change_state;
   while (1) {
    
			/*�ַ���Ӣ��*/
			char_change_state = Char_Change_Check();
			if(char_change_state)			//�����û�б仯��û�б仯�Ͳ�������ʡ����
			{
				JudgementCustomizeChar(char_change_state);
				if(char_change_state != Op_None)
					referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&custom_char_draw,sizeof(custom_char_draw));
			}
			
		  IWDG_Feed();//ι��
			charTask_cnt++;
			vTaskDelay(50); 
		 
#if INCLUDE_uxTaskGetStackHighWaterMark
		
#endif
    }
}
