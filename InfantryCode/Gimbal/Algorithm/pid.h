#ifndef __PID_H
#define __PID_H

typedef struct{
		float K1;			
	  float K2;
	  float Last_DeltIn;
	  float Now_DeltIn;
	  float Out;
	  float OutMax;
}FeedForward_Typedef;

/*   PID����-----ZN������
		 Td/T = Kd  T/Ti = Ki   
     PID:   Ti >= 4Td    Kp = 0.6Ku   Td = 0.125Tu
     PI:    Kp = 0.45Ku  Ti = 0.83Tu
     P:     Kp = 0.5Ku
*/


typedef struct PID{
		float SetPoint;			//�趨Ŀ��ֵ
	
		float ActualValue;  //ʵ��ֵ

    float DeadZone;
		float P;						//��������
		float I;						//���ֳ���
		float D;						//΢�ֳ���
		
		float LastError;		//ǰ�����
		float PreError;			//��ǰ���
		float SumError;			//�������
	
		float IMax;					//��������
		
		float POut;					//�������
		float IOut;					//�������
		float DOut;					//΢�����
	  float DOut_last;    //��һ��΢�����
		float OutMax;       //�޷�
	  float Out;          //�����
		float Out_last;     //��һ�����
		
		float I_U;          //���ٻ�������
		float I_L;          //���ٻ�������
		
		float RC_DF;        //����ȫ΢���˲�ϵ��
		
}Pid_Typedef;

float PID_Calc(Pid_Typedef * P);

/*********ģ��pid����*/
typedef struct
{
		float SetPoint;			//�趨Ŀ��ֵ
	
		float ActualValue;  //ʵ��ֵ

    float DeadZone;
		
		float LastError;		//ǰ�����
		float PreError;			//��ǰ���
		float SumError;			//�������
	
		float IMax;					//��������
		
		float POut;					//�������
		float IOut;					//�������
		float DOut;					//΢�����
	  float DOut_last;    //��һ��΢�����
		float OutMax;       //�޷�
	  float Out;          //�����
		float Out_last;     //��һ�����
		
		float I_U;          //���ٻ�������
		float I_L;          //���ٻ�������
		
		float RC_DM;        //΢�������˲�ϵ��
		float RC_DF;        //����ȫ΢���˲�ϵ��
	
	  float Kp0;          //PID��ֵ
	  float Ki0;
  	float Kd0;
	
	  float dKp;          //PID�仯��
	  float dKi;
  	float dKd;
	
    float stair ;	      //��̬�����ݶ�   //0.25f
	  float Kp_stair;                      //0.015f
	  float Ki_stair;                      //0.0005f
	  float Kd_stair;                      //0.001f
	  
}FuzzyPID;
float FuzzyPID_Calc(FuzzyPID *pid);
float FeedForward_Calc(FeedForward_Typedef *FF);
#endif
