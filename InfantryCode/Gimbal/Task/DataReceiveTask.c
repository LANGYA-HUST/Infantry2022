/**********************************************************************************************************
 * @�ļ�     DataReceive.c
 * @˵��     ���պ���
 * @�汾  	 V1.0
 * @����     ��־��
 * @����     2020.1
**********************************************************************************************************/
/**********************************************************************************************************
 * @�ļ�     DataReceive.c
 * @˵��     ���պ���
 * @�汾  	 V2.0
 * @����     ����
 * @����     2022.3
**********************************************************************************************************/
#include "main.h"

RC_Ctl_t RC_Ctl;//ң��������
Gyro_Typedef GyroReceive;//����������
F105_Typedef F105;
PC_Receive_t PC_Receive;
BodanMotorReceive_Typedef BodanReceive;
char PitchMotor_ReceiveFlag;
short FrictionReceive[2];
float PitchMotorReceive,YawMotorReceive;//Pitch,Yaw����Ƕ�
float yaw_gyro_temp,pitch_gyro_temp;

Disconnect Robot_Disconnect;

extern ZeroCheck_Typedef ZeroCheck_Pitch;
extern Gimbal_Typedef Gimbal;
extern volatile long run_time_check;
extern Status_t Status;
extern short KalMan_doneflag;
extern char Judge_Lost;
char Chassis_ID;

extern uint8_t Remote_Receive_Flag ;
extern uint8_t PC_ReceiveFlag;
extern volatile unsigned char sbus_rx_buffer[18];
extern unsigned char PCbuffer[PC_RECVBUF_SIZE];

unsigned char tempPC[PC_RECVBUF_SIZE];//����Ҫ��Ϊȫ�ֱ�������Ȼcrc��ͨ��
short crcNopass;
short buffindex;
uint8_t CoolBuffState;
/**********************************************************************************************************
*�� �� ��: Can1Receive1
*����˵��: ���ʰ�Ͳ������ͨ��
*��    ��: rx_message1
*�� �� ֵ: ��
**********************************************************************************************************/
int error_cnt=0;
void Can1Receive0(CanRxMsg rx_message1)
{
	switch(rx_message1.StdId)
	{ 
		case 0x094:
		     memcpy(&F105.bulletSpeed,&rx_message1.Data[0], 4); 
				 memcpy(&Judge_Lost,&rx_message1.Data[4],1);
				 memcpy(&F105.RobotLevel,&rx_message1.Data[5],1);
				 memcpy(&F105.HeatCool17,&rx_message1.Data[6],1);
		     Robot_Disconnect.F105_DisConect=0;
		 break;		
		case 0x096:
		     memcpy(&CoolBuffState,&rx_message1.Data[0], 1); 
		break;		
	}
}

/**********************************************************************************************************
*�� �� ��: Can1Receive1
*����˵��: ���ʰ�Ͳ������ͨ��
*��    ��: rx_message1
*�� �� ֵ: ��
**********************************************************************************************************/
void Can1Receive1(CanRxMsg rx_message1)
{
	switch(rx_message1.StdId)
	{ 
		case 0x100:
		     memcpy(&F105.ChassisSpeedw, &rx_message1.Data[0], 2);  
		     memcpy(&F105.Remain_power, &rx_message1.Data[2], 2); 		
	    	 memcpy(&F105.IsShootAble, &rx_message1.Data[4], 1);
				 memcpy(&F105.RobotRed, &rx_message1.Data[5], 1);  
		     memcpy(&F105.BulletSpeedLevel, &rx_message1.Data[6], 1);
				 memcpy(&Chassis_ID,&rx_message1.Data[7],1);
		     Robot_Disconnect.F105_DisConect=0;
		 break;		
	}
}

/**********************************************************************************************************
*�� �� ��: Can2Receive0
*����˵��: Pitch,Yaw����ǶȽ���
*��    ��: rx_message0
*�� �� ֵ: ��
**********************************************************************************************************/
extern RobotInit_Struct Infantry;
short YawMotorSpeed,PitchMotorSpeed;
void Can2Receive0(CanRxMsg rx_message0)
{
	if(rx_message0.StdId == Infantry.PitchMotorID)
	{
				 PitchMotorReceive=rx_message0.Data[0]<<8 | rx_message0.Data[1];
				 Robot_Disconnect.PitchMotor_DisConnect=0;
		     if(!PitchMotor_ReceiveFlag)
				{
				ZeroCheck_Pitch.LastValue=PitchMotorReceive;
				}
		     PitchMotor_ReceiveFlag=1;	
	}
	else if(rx_message0.StdId == Infantry.YawMotorID)
	{
				 YawMotorReceive=rx_message0.Data[0]<<8 | rx_message0.Data[1];
		     YawMotorSpeed  =rx_message0.Data[2]<<8 | rx_message0.Data[3];
				 Robot_Disconnect.YawMotor_DisConnect=0;
	
	}
	else if(rx_message0.StdId == Infantry.BodanMotorID)
	{
         BodanReceive.Angle=rx_message0.Data[0]<<8 | rx_message0.Data[1];
				 BodanReceive.RealSpeed=rx_message0.Data[2]<<8 | rx_message0.Data[3];
				 Robot_Disconnect.Pluck_DisConnect=0;	
	}
	else if(rx_message0.StdId == Infantry.FricMotorID[0])
	{
      FrictionReceive[0]=rx_message0.Data[2]<<8|rx_message0.Data[3];
			Robot_Disconnect.Friction_DisConnect[0]=0;
	}	
	else if(rx_message0.StdId == Infantry.FricMotorID[1])
	{
      FrictionReceive[1]=rx_message0.Data[2]<<8|rx_message0.Data[3];
			Robot_Disconnect.Friction_DisConnect[1]=0;	
	}

}
/**********************************************************************************************************
*�� �� ��: Can2Receive1
*����˵��: ���������ݽ���
*��    ��: rx_message1
*�� �� ֵ: ��
**********************************************************************************************************/
void Can2Receive1(CanRxMsg rx_message1)
{
	switch(rx_message1.StdId)
	{ 
		 case 0x100:
		 {
			 memcpy(&pitch_gyro_temp, rx_message1.Data, 4);
			 memcpy(&GyroReceive.GY, &rx_message1.Data[4], 4);
			 GyroReceive.GY*= Infantry.gyro_pn;
			 //����ȫ
			 if(ABS(pitch_gyro_temp)<50.0f)
			 {
			 GyroReceive.PITCH = pitch_gyro_temp*Infantry.gyro_pn;
			 }
			 else
			 {
			 GyroReceive.PITCH = Gimbal.Pitch.MotorTransAngle;
			 }
		 }
		 break;
		 case 0x101:
		 {
			 memcpy(&yaw_gyro_temp, &rx_message1.Data, 4);
			 memcpy(&GyroReceive.GZ, &rx_message1.Data[4], 4);
			 //����ȫ
			 if(yaw_gyro_temp < 180.0f && yaw_gyro_temp > -180.0f)
			 {
			 GyroReceive.YAW = yaw_gyro_temp;
			 Robot_Disconnect.Gyro_DisConnect=0;
			 }
		 }
		 break;
//		 case 0x133:
//		 {
//			 memcpy(&GyroReceive.ROLL, &rx_message1.Data, 4);
//			 memcpy(&GyroReceive.GX, &rx_message1.Data[4], 4);
//		 }
	 }
}

/**********************************************************************************************************
*�� �� ��: PC_Receive
*����˵��: USART6��PCͨ�Ž������ݴ���   
*��    ��: rx_buffer[]
*�� �� ֵ: ��
**********************************************************************************************************/
int pc_yaw;
short pc_pitch;
short pc_x;
short pc_y;
short pc_z;
short distance;
short armor_state = ARMOR_NO_AIM;		//��ʾ�����ǲ������ҵ�Ŀ��
float aim_yaw, aim_pitch;
short tx2_last_receive_flag;	//��ʾ��û�����ݸ���
char tx2_receive_flag;	
extern float Buff_Yaw_Motor;
extern short pitch; 
extern int yaw;

void PCReceive(unsigned char PCReceivebuffer[])
{				
	run_time_check++;
	pc_pitch = (short)(PCReceivebuffer[2]<<8|PCReceivebuffer[3]);//���ﲻ��תΪfloat����Ȼ�������������Ϊ����
	pc_yaw = (int)(PCReceivebuffer[4]<<24|PCReceivebuffer[5]<<16|PCReceivebuffer[6]<<8|PCReceivebuffer[7]<<0);
	
	if(ABS( yaw - pc_yaw)<9000 && ABS( pitch - pc_pitch)<6000)  //��������
 {
	 aim_yaw =  (float)pc_yaw/100.0f;
	 aim_pitch = (float)pc_pitch/100.0f;

	 tx2_receive_flag = PCReceivebuffer[1];	//��Ϊ���±�־λ
	
  	if(tx2_receive_flag != tx2_last_receive_flag)
  	{
	  	armor_state = ARMOR_AIMED;	
	  }
	  else
	  {
		  armor_state = ARMOR_NO_AIM;	
	  }
	
	  tx2_last_receive_flag = tx2_receive_flag;

	  PC_Receive.RCdistance = (float)distance/10.0f;
	
	  Robot_Disconnect.PC_DisConnect=0;
	  tx2_last_receive_flag = tx2_receive_flag;
 }
}
/**********************************************************************************************************
*�� �� ��: RemoteReceive
*����˵��: ң�������ݽ���
*��    ��: rx_buffer[]
*�� �� ֵ: ��
**********************************************************************************************************/
char temp1,temp2,temp3;
void RemoteReceive(volatile unsigned char rx_buffer[])
{
	RC_Ctl.rc.ch0 = (rx_buffer[0]| (rx_buffer[1] << 8)) & 0x07ff; //!< Channel 0
	RC_Ctl.rc.ch1 = ((rx_buffer[1] >> 3) | (rx_buffer[2] << 5)) & 0x07ff; //!< Channel 1
	RC_Ctl.rc.ch2 = ((rx_buffer[2] >> 6) | (rx_buffer[3] << 2) | (rx_buffer[4] << 10)) & 0x07ff;//!< Channel 2
	RC_Ctl.rc.ch3 = ((rx_buffer[4] >> 1) | (rx_buffer[5] << 7)) & 0x07ff; //!< Channel 3
	RC_Ctl.rc.s1 = ((rx_buffer[5] >> 4)& 0x0003); //!< Switch left
	RC_Ctl.rc.s2 = ((rx_buffer[5] >> 6)& 0x0003);
	RC_Ctl.mouse.x = rx_buffer[6] | (rx_buffer[7] << 8); //!< Mouse X axis
	RC_Ctl.mouse.y = rx_buffer[8] | (rx_buffer[9] << 8); //!< Mouse Y axis
	RC_Ctl.mouse.z = rx_buffer[10] | (rx_buffer[11] << 8); //!< Mouse Z axis
	RC_Ctl.mouse.press_l = rx_buffer[12]; //!< Mouse Left Is Press ?
	RC_Ctl.mouse.press_r = rx_buffer[13]; //!< Mouse Right Is Press ?
	RC_Ctl.key.w = rx_buffer[14]&0x01; // KeyBoard value
	RC_Ctl.key.s = (rx_buffer[14]>>1)&0x01;
	RC_Ctl.key.a = (rx_buffer[14]>>2)&0x01;
	RC_Ctl.key.d = (rx_buffer[14]>>3)&0x01;
	RC_Ctl.key.shift =(rx_buffer[14]>>4)&0x01;
	RC_Ctl.key.ctrl = (rx_buffer[14]>>5)&0x01;
	RC_Ctl.key.q = (rx_buffer[14]>>6)&0x01;
	RC_Ctl.key.e = (rx_buffer[14]>>7)&0x01;	
	RC_Ctl.key.r = (rx_buffer[15])&0x01;
	RC_Ctl.key.f = (rx_buffer[15]>>1)&0x01;
	RC_Ctl.key.g = (rx_buffer[15]>>2)&0x01; 
	RC_Ctl.key.z = (rx_buffer[15]>>3)&0x01;
	RC_Ctl.key.x = (rx_buffer[15]>>4)&0x01;
	RC_Ctl.key.c = (rx_buffer[15]>>5)&0x01;
	RC_Ctl.key.v = (rx_buffer[15]>>6)&0x01;
	RC_Ctl.key.b = (rx_buffer[15]>>7)&0x01;
	temp1 = rx_buffer[16];
	temp2 = rx_buffer[17];
	if((RC_Ctl.rc.ch0-1024<20)&&(RC_Ctl.rc.ch0-1024>-20)) RC_Ctl.rc.ch0=1024;
	if((RC_Ctl.rc.ch1-1024<20)&&(RC_Ctl.rc.ch1-1024>-20)) RC_Ctl.rc.ch1=1024;
	if((RC_Ctl.rc.ch2-1024<20)&&(RC_Ctl.rc.ch2-1024>-20)) RC_Ctl.rc.ch2=1024;
	if((RC_Ctl.rc.ch3-1024<20)&&(RC_Ctl.rc.ch3-1024>-20)) RC_Ctl.rc.ch3=1024;	

	Robot_Disconnect.RC_DisConnect=0;
}


/**********************************************************************************************************
*�� �� ��: RC_Rst
*����˵��: ң�������ݸ�λ
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void RC_Rst(void)
{
		RC_Ctl.rc.ch0 = 1024;
		RC_Ctl.rc.ch1 = 1024;
		RC_Ctl.rc.ch2 = 1024;
		RC_Ctl.rc.ch3 = 1024;
		RC_Ctl.mouse.x = 0;
		RC_Ctl.mouse.y = 0;
		RC_Ctl.mouse.z = 0;
		RC_Ctl.mouse.press_l = 0;                                                
		RC_Ctl.mouse.press_r = 0;
	
		RC_Ctl.key.w = 0;
		RC_Ctl.key.s = 0;                            
		RC_Ctl.key.a = 0;
		RC_Ctl.key.d = 0;
		RC_Ctl.key.q = 0;
		RC_Ctl.key.e = 0;
		RC_Ctl.key.r = 0;
		RC_Ctl.key.f = 0;
		RC_Ctl.key.shift = 0;
		RC_Ctl.key.ctrl = 0;
	
	  RC_Ctl.rc.s1 = 2;
		RC_Ctl.rc.s2 = 2;
} 


/**********************************************************************************************************
*�� �� ��: F105_Rst
*����˵��: ���ʰ���߸�λ
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
unsigned int Heat_LimitTick;
void F105_Rst()
{
	  Heat_LimitTick++;
	  if(Heat_LimitTick%50 ==1 )
	    F105.IsShootAble = 1;
    else
			F105.IsShootAble =0;
		
		F105.BulletSpeedLevel=0;
}

/**********************************************************************************************************
*�� �� ��: RCReceive_task
*����˵��: ң�������ݴ�������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/

extern TaskHandle_t RCReceiveTask_Handler; //������
extern TaskHandle_t PCReceiveTask_Handler; //������

void RCReceive_task()
{
  //	static BaseType_t DataReceive_Exit = pdFALSE ;  //ʹ��֪ͨʵ�ּ����ź��������ڲ�ͬ�ж���Ϣ����
	

	while(1)
	{
		ulTaskNotifyTake( pdTRUE , portMAX_DELAY );  //����֪ͨ���£��򲻻���
		
/********************************* ң�������ݴ��� *****************************************************/		
		
		Remote_Receive_Flag = 0;
		RemoteReceive(sbus_rx_buffer);//���뺯��
			
	
	}
	
}

/**********************************************************************************************************
*�� �� ��: PCReceive_task
*����˵��: PC���ݴ�������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/

void PCReceive_task()
{
  //	static BaseType_t DataReceive_Exit = pdFALSE ;  //ʹ��֪ͨʵ�ּ����ź��������ڲ�ͬ�ж���Ϣ����
	
	static unsigned char temptemp[2*PC_RECVBUF_SIZE];  //����PC���ݴ���ľֲ�����
	short PackPoint,n;
  
	while(1)
	{
		ulTaskNotifyTake( pdTRUE , portMAX_DELAY );  //����֪ͨ���£��򲻻���,֪ͨʵ�ֶ�ֵ�ź���
		
/********************************* PC���ݴ��� *******************************************************/		
	
			PC_ReceiveFlag = 0;
			
			memcpy(temptemp+PC_RECVBUF_SIZE,PCbuffer,PC_RECVBUF_SIZE);
			for(PackPoint = 0; PackPoint < PC_RECVBUF_SIZE; PackPoint++)//��ֹ��λ����һ������Ԫ�صĵ�һ����Ϊ
			{
				if(temptemp[PackPoint] == '!')
				{
					for(n=0;n<PC_RECVBUF_SIZE;n++)
					{
						tempPC[n] = temptemp[(n+PackPoint)];
					}
					crcNopass++;
					if(Verify_CRC8_Check_Sum(tempPC,PC_RECVBUF_SIZE))
					{
						PCReceive(tempPC);
					}
					else
					{
						buffindex++;
						buffindex = buffindex%4;
					}
					break;
				}
			}
			memcpy(temptemp,temptemp+PC_RECVBUF_SIZE,PC_RECVBUF_SIZE);
		
/********************************* �������ݴ��� *******************************************************/		
	
	}
	
}