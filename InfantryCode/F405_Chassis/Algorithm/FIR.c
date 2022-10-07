#include "main.h"

float Input[4] = {0};
float Output[4] = {0};
#define K_Low_setchassis  0.05   //0.05   截止频率f=1000a/2pi=160a
#define K_Low_setwheel    0.6

extern float Flow[4];
const float Gains[6] = {
 0.01016679592,   0.1177062541,   0.3721269369,   0.3721269369,   0.1177062541,
    0.01016679592
};

float Buffer[4][ORDER+1];        //采样历史数据
/**
  * @brief  读入当前浮点型，加入FIR滤波队列
  * @param  Input: 滤波对象的当前值     
  * @retval 滤波后最新值
  */
void Fir(float Input[],float Output[4])
{
		unsigned int Index;                //下标索引
		for(int i = 0;i < 4;i ++)
		{
			for(Index=ORDER;Index>0;Index--) Buffer[i][Index]=Buffer[i][Index-1];
			Buffer[i][0]=Input[i];
			//计算输出
			for(Index=0;Index<ORDER+1;Index++)
			{
				Output[i]+=Gains[Index]*Buffer[i][Index];
			}
			Flow[i] = Output[i];
			Output[i] = 0;
		}
        
}

/**
  * @brief  车子速度设定的低通滤波
  * @param  
  * @retval 滤波后最新值
  */
void LowPass_SetChassis(float* output,float In)
{
    *output=(1-K_Low_setchassis)*(*output)+K_Low_setchassis*In;   
}

/**
* @brief  车子轮子电流发送的低通滤波
  * @param  
  * @retval 滤波后最新值
  */
float LowPass_SetWheel(float In,float past)
{
  past=(1-K_Low_setwheel)*past+K_Low_setwheel*In;
  return past;	
}