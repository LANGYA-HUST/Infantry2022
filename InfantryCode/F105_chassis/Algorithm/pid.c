#include "main.h"
/**
  * @brief  PID�����㷨
  * @param  PID_Struct *P  PID�����ṹ��
  *         ActualValue    PID���㷴��������ǰ��ʵ���ֵ��
  * @retval PID�����������ֵ
  */
float PID_Calc(Pid_Typedef *P, float ActualValue)
{
	  P->ActValue=(ABS(ActualValue)> P->DeadZone)?ActualValue:0;  
	
		P->PreError = P->SetPoint - P->ActValue;
		P->dError = P->PreError - P->LastError;
	
	  P->SetPointLast = P->SetPoint;
	
		if(P->PreError > -P->ErrorMax && P->PreError < P->ErrorMax)
		{
			P->SumError += P->PreError;
		}
		
		P->LastError = P->PreError;
		
		if(P->SumError >= P->IMax)
			P->SumError = P->IMax;
		else if(P->SumError <= -P->IMax)
			P->SumError = -P->IMax;
		
		P->POut = P->P * P->PreError;
		P->IOut = P->I * P->SumError;
		P->DOut = P->D * P->dError;
		
		return LIMIT_MAX_MIN(P->POut+P->IOut+P->DOut,P->OutMax,-P->OutMax); 
}

