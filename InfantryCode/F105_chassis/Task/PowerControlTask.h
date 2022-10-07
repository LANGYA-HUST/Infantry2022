#ifndef __POWERCONTROLTASK_H
#define __POWERCONTROLTASK_H

#define Bat_on	GPIO_SetBits(GPIOC,GPIO_Pin_5)
#define Bat_off	GPIO_ResetBits(GPIOC,GPIO_Pin_5)
#define CAP_on	GPIO_SetBits(GPIOC,GPIO_Pin_4)
#define CAP_off	GPIO_ResetBits(GPIOC,GPIO_Pin_4)
#define Charge_On	GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define Charge_Off	GPIO_ResetBits(GPIOA,GPIO_Pin_6)

//功率限制
enum POWERSTATE_Typedef
{
	BAT = 0,
	CAP,
	HalfCAP
};
enum CHARGESTATE_Typedef
{
	ChargeOn = 0,
	ChargeOff
};

void ChargeIO_Configuration(void);
void ADC_Filter(void);
void Pid_ChargeCal_Init(void);
float ChargeCal(void);
void Charge_Set(float I_Set);
void ChargeControl(void);
void SuperPowerControl(void);
void Rc_k_Normalized_processing(void);

//热量控制
void HeatControl(void);
void HeatUpdate(void);

//发送结构体配置
void BuildF105(void);

//任务函数
void PowerControl_task(void *pvParameters);

#endif

