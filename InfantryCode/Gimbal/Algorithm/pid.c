/**********************************************************************************************************
 * @文件     pid.c
 * @说明     pid各种优化算法+模糊PID+前馈算法
 * @版本  	 V3.0
 * @作者     黄志雄、江扬新、戴军
 * @日期     2020.1、2021.9、2022.7
**********************************************************************************************************/
#include "main.h"

/**********************************************************************************************************
*函 数 名: PID_Calc
*功能说明: PID+各种优化
*形    参: PID_Struct *P  PID参数结构体
  *        ActualValue    PID计算反馈量
*返 回 值: PID反馈计算输出值
**********************************************************************************************************/
float PID_Calc(Pid_Typedef *P)
{
		P->LastError = P->PreError;
		P->PreError = P->SetPoint - P->ActualValue;
	  if((ABS(P->PreError)< P->DeadZone ))   //死区控制
		{
			P->PreError = 0.0f;			
		}
		
		      //微分先行
		float DM = P->D*(P->Out - P->Out_last);   //微分先行
	
         //变速积分   (积分分离)
    if(ABS(P->PreError) < P->I_L )			
		{
	       
		P->SumError += (P->PreError+P->LastError)/2;    
		P->SumError = LIMIT_MAX_MIN(P->SumError,P->IMax,- P->IMax);
		}
		 else if( ABS(P->PreError) < P->I_U )
		{
	       //梯形积分
		P->SumError += (P->PreError+P->LastError)/2*(P->PreError - P->I_L)/(P->I_U - P->I_L);    
		P->SumError = LIMIT_MAX_MIN(P->SumError,P->IMax,- P->IMax);		
		}
			
		P->POut = P->P * P->PreError;
		
		P->IOut = P->I * P->SumError;
		    
		    //不完全微分
		P->DOut_last = P->DOut; 
		P->DOut = DM * P->RC_DF + P->DOut_last * ( 1 - P->RC_DF );    
		
		P->Out_last  = P->Out;
		P->Out = LIMIT_MAX_MIN(P->POut+P->IOut+P->DOut,P->OutMax,-P->OutMax);
		
		return P->Out; 
}

/**********************************************************************************************************
*函 数 名: FeedForward_Calc
*功能说明: 前馈算法
*形    参: PID_Struct *P  PID参数结构体
  *        ActualValue    PID计算反馈量（当前真实检测值）
*返 回 值: PID反馈计算输出值
**********************************************************************************************************/
float FeedForward_Calc(FeedForward_Typedef *FF)
{
	  FF->Out = FF->Now_DeltIn*FF->K1 + (FF->Now_DeltIn - FF->Last_DeltIn)*FF->K2;
	  FF->Last_DeltIn = FF->Now_DeltIn;
    return LIMIT_MAX_MIN(FF->Out,FF->OutMax,-FF->OutMax);
}




/*********模糊pid部分*/

 
#define NL   -3
#define NM	 -2
#define NS	 -1
#define ZE	 0
#define PS	 1
#define PM	 2
#define PL	 3
 
static const float fuzzyRuleKp[7][7]={
	PL,	PL,	PM,	PM,	PS,	ZE,	ZE,
	PL,	PL,	PM,	PS,	PS,	ZE,	NS,
	PM,	PM,	PM,	PS,	ZE,	NS,	NS,
	PM,	PM,	PS,	ZE,	NS,	NM,	NM,
	PS,	PS,	ZE,	NS,	NS,	NM,	NM,
	PS,	ZE,	NS,	NM,	NM,	NM,	NL,
	ZE,	ZE,	NM,	NM,	NM,	NL,	NL
};
 
static const float fuzzyRuleKi[7][7]={
	NL,	NL,	NM,	NM,	NS,	ZE,	ZE,
	NL,	NL,	NM,	NS,	NS,	ZE,	ZE,
	NL,	NM,	NS,	NS,	ZE,	PS,	PS,
	NM,	NM,	NS,	ZE,	PS,	PM,	PM,
	NS,	NS,	ZE,	PS,	PS,	PM,	PL,
	ZE,	ZE,	PS,	PS,	PM,	PL,	PL,
	ZE,	ZE,	PS,	PM,	PM,	PL,	PL
};
 
static const float fuzzyRuleKd[7][7]={
	PS,	NS,	NL,	NL,	NL,	NM,	PS,
	PS,	NS,	NL,	NM,	NM,	NS,	ZE,
	ZE,	NS,	NM,	NM,	NS,	NS,	ZE,
	ZE,	NS,	NS,	NS,	NS,	NS,	ZE,
	ZE,	ZE,	ZE,	ZE,	ZE,	ZE,	ZE,
	PL,	NS,	PS,	PS,	PS,	PS,	PL,
	PL,	PM,	PM,	PM,	PS,	PS,	PL
};
 

 //关键算法
void fuzzy( FuzzyPID*  fuzzy_PID)
{
     float e = fuzzy_PID ->PreError/ fuzzy_PID->stair;
	   float ec = (fuzzy_PID ->Out - fuzzy_PID ->Out_last) / fuzzy_PID->stair;
     short etemp,ectemp;
     float eLefttemp,ecLefttemp;    //隶属度
     float eRighttemp ,ecRighttemp; 
 
     short eLeftIndex,ecLeftIndex;  //标签
     short eRightIndex,ecRightIndex;

	  //模糊化
     if(e>=PL)
			 etemp=PL;//超出范围
		 else if(e>=PM)
			 etemp=PM;
		 else if(e>=PS)
			 etemp=PS;
		 else if(e>=ZE)
			 etemp=ZE;
		 else if(e>=NS)
			 etemp=NS;
		 else if(e>=NM)
			 etemp=NM;
		 else if(e>=NL)
			 etemp=NL;
		 else 
			 etemp=2*NL;
 
		 if( etemp == PL)
		{
		 //计算E隶属度
				eRighttemp= 0 ;    //右溢出
				eLefttemp= 1 ;
			
     //计算标签
	   eLeftIndex = 6 ;      
	   eRightIndex= 6 ;
			
		}else if( etemp == 2*NL )
    {

			//计算E隶属度
				eRighttemp = 1;    //左溢出
				eLefttemp = 0;
	
     //计算标签
	   eLeftIndex = 0 ;       
	   eRightIndex = 0 ;
			
		}	else 
    {

			//计算E隶属度
				eRighttemp=(e-etemp);  //线性函数作为隶属函数
				eLefttemp=(1- eRighttemp);
			
     //计算标签
	   eLeftIndex =(short) (etemp-NL);       //例如 etemp=2.5，NL=-3，那么得到的序列号为5  【0 1 2 3 4 5 6】
	   eRightIndex=(short) (eLeftIndex+1);
			
		}		
	   
		
		 if(ec>=PL)
			 ectemp=PL;
		 else if(ec>=PM)
			 ectemp=PM;
		 else if(ec>=PS)
			 ectemp=PS;
		 else if(ec>=ZE)
			 ectemp=ZE;
		 else if(ec>=NS)
			 ectemp=NS;
		 else if(ec>=NM)
			 ectemp=NM;
		 else if(ec>=NL)
			 ectemp=NL;
		 else 
			 ectemp=2*NL;
		 
	  
   if( ectemp == PL )
	 {
    //计算EC隶属度		 
		 ecRighttemp= 0 ;      //右溢出
		 ecLefttemp= 1 ;
			
		 ecLeftIndex = 6 ;  
	   ecRightIndex = 6 ;	 
	 
	 } else if( ectemp == 2*NL)
	 {
    //计算EC隶属度		 
		 ecRighttemp= 1 ;
		 ecLefttemp= 0 ;
			
		 ecLeftIndex = 0 ;  
	   ecRightIndex = 0 ;	 	 
	 }else
	 {
    //计算EC隶属度		 
		 ecRighttemp=(ec-ectemp);
		 ecLefttemp=(1- ecRighttemp);
			
		 ecLeftIndex =(short) (ectemp-NL);  
	   ecRightIndex= (short)(eLeftIndex+1);
	 }	

 
/*************************************反模糊*************************************/
 
 
 
 
	fuzzy_PID->dKp = fuzzy_PID->Kp_stair * (eLefttemp * ecLefttemp * fuzzyRuleKp[eLeftIndex][ecLeftIndex]                   
   + eLefttemp * ecRighttemp * fuzzyRuleKp[eLeftIndex][ecRightIndex]
   + eRighttemp * ecLefttemp * fuzzyRuleKp[eRightIndex][ecLeftIndex]
   + eRighttemp * ecRighttemp * fuzzyRuleKp[eRightIndex][ecRightIndex]);
 
	fuzzy_PID->dKi = fuzzy_PID->Ki_stair * (eLefttemp * ecLefttemp * fuzzyRuleKi[eLeftIndex][ecLeftIndex]
   + eLefttemp * ecRighttemp * fuzzyRuleKi[eLeftIndex][ecRightIndex]
   + eRighttemp * ecLefttemp * fuzzyRuleKi[eRightIndex][ecLeftIndex]
   + eRighttemp * ecRighttemp * fuzzyRuleKi[eRightIndex][ecRightIndex]);

 
	fuzzy_PID->dKd = fuzzy_PID->Kd_stair * (eLefttemp * ecLefttemp * fuzzyRuleKd[eLeftIndex][ecLeftIndex]
   + eLefttemp * ecRighttemp * fuzzyRuleKd[eLeftIndex][ecRightIndex]
   + eRighttemp * ecLefttemp * fuzzyRuleKd[eRightIndex][ecLeftIndex]
   + eRighttemp * ecRighttemp * fuzzyRuleKd[eRightIndex][ecRightIndex]);
 
}


float FuzzyPID_Calc(FuzzyPID *P)
{
	
	  P->LastError = P->PreError;
	  
	  if((ABS(P->PreError)< P->DeadZone ))   //死区控制
		{
			P->PreError = 0.0f;			
		}
		else
		{
			P->PreError = P->SetPoint - P->ActualValue;
		}
		
		fuzzy(P);      //模糊调整  kp,ki,kd   形参1当前误差，形参2前后误差的差值
	
    float Kp = P->Kp0 + P->dKp , Ki = P->Ki0 + P->dKi , Kd = P->Kd0 + P->dKd ;   //PID均模糊
//	float Kp = P->Kp0 + P->dKp , Ki = P->Ki0  , Kd = P->Kd0 + P->dKd ;           //仅PD均模糊
//	float Kp = P->Kp0 + P->dKp , Ki = P->Ki0  , Kd = P->Kd0 ;                    //仅P均模糊

		
		
		      //微分先行
		float DM = Kd*(P->Out - P->Out_last);   //微分先行	
         //变速积分
    if(ABS(P->PreError) < P->I_L )			
		{
	       //梯形积分
		P->SumError += (P->PreError+P->LastError)/2;    
		P->SumError = LIMIT_MAX_MIN(P->SumError,P->IMax,- P->IMax);
		}
		 else if( ABS(P->PreError) < P->I_U )
		{
	       //梯形积分
		P->SumError += (P->PreError+P->LastError)/2*(P->PreError - P->I_L)/(P->I_U - P->I_L);    
		P->SumError = LIMIT_MAX_MIN(P->SumError,P->IMax,- P->IMax);		
		}
			
		P->POut = Kp * P->PreError;
		
		P->IOut = Ki * P->SumError;
		    
		    //不完全微分
		P->DOut_last = P->DOut; 
		P->DOut = DM * P->RC_DF + P->DOut_last * ( 1 - P->RC_DF );    
		
		P->Out_last  = P->Out;
		P->Out = LIMIT_MAX_MIN(P->POut+P->IOut+P->DOut,P->OutMax,-P->OutMax);
		
    return P->Out;                             

}