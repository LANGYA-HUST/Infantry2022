#ifndef __PID_H
#define __PID_H
typedef struct PID{
		float SetPoint;			//设定目标值
	  float SetPointLast;
		
		float P;						//比例常数
		float I;						//积分常数
		float D;						//微分常数
		
		float LastError;		//前次误差
		float PreError;			//当前误差
		float SumError;			//积分误差
		float dError;
		
		float ErrorMax;			//偏差上限 超出偏差则不计算积分作用
		float IMax;					//积分限制
		
		float POut;					//比例输出
		float IOut;					//积分输出
		float DOut;					//微分输出
	  float OutMax;       //限幅
}Pid_Typedef;

float PID_Calc(Pid_Typedef * P, float ActualValue);
#endif
