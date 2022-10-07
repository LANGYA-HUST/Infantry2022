#include "main.h"

float Input[4] = {0};
float Output[4] = {0};
#define K_Low_setchassis  0.05   //0.05   ��ֹƵ��f=1000a/2pi=160a
#define K_Low_setwheel    0.6

extern float Flow[4];
const float Gains[6] = {
 0.01016679592,   0.1177062541,   0.3721269369,   0.3721269369,   0.1177062541,
    0.01016679592
};

float Buffer[4][ORDER+1];        //������ʷ����
/**
  * @brief  ���뵱ǰ�����ͣ�����FIR�˲�����
  * @param  Input: �˲�����ĵ�ǰֵ     
  * @retval �˲�������ֵ
  */
void Fir(float Input[],float Output[4])
{
		unsigned int Index;                //�±�����
		for(int i = 0;i < 4;i ++)
		{
			for(Index=ORDER;Index>0;Index--) Buffer[i][Index]=Buffer[i][Index-1];
			Buffer[i][0]=Input[i];
			//�������
			for(Index=0;Index<ORDER+1;Index++)
			{
				Output[i]+=Gains[Index]*Buffer[i][Index];
			}
			Flow[i] = Output[i];
			Output[i] = 0;
		}
        
}

/**
  * @brief  �����ٶ��趨�ĵ�ͨ�˲�
  * @param  
  * @retval �˲�������ֵ
  */
void LowPass_SetChassis(float* output,float In)
{
    *output=(1-K_Low_setchassis)*(*output)+K_Low_setchassis*In;   
}

/**
* @brief  �������ӵ������͵ĵ�ͨ�˲�
  * @param  
  * @retval �˲�������ֵ
  */
float LowPass_SetWheel(float In,float past)
{
  past=(1-K_Low_setwheel)*past+K_Low_setwheel*In;
  return past;	
}