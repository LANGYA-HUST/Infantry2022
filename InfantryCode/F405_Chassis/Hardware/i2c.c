#include "main.h"
void i2c_init(void)
{
	GPIO_InitTypeDef GPIO_Init_Structure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_Init_Structure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
	GPIO_Init_Structure.GPIO_Mode=GPIO_Mode_OUT;
  GPIO_Init_Structure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init_Structure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init_Structure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_Init_Structure);
	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);
}

void SDA_OUT()
{
	GPIO_InitTypeDef GPIO_Init_Structure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC->AHB1ENR|=1<<1;    //ʹ��PORTBʱ��	
	GPIO_Init_Structure.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init_Structure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_Init_Structure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_Structure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Init_Structure);
}


void SDA_IN()
{
	GPIO_InitTypeDef GPIO_Init_Structure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_Init_Structure.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init_Structure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_Init_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Init_Structure);
}

//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_ms(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_ms(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}

//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_ms(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_ms(4);							   	
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
unsigned char IIC_Wait_Ack(void)
{
	unsigned char ucErrTime=0; 
	IIC_SDA=1;delay_ms(1);
	SDA_IN();      //SDA����Ϊ����	   
	IIC_SCL=1;delay_ms(1); 	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			//IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
}

void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_ms(2);
	IIC_SCL=1;
	delay_ms(2);
	IIC_SCL=0;
}

void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_ms(2);
	IIC_SCL=1;
	delay_ms(2);
	IIC_SCL=0;
}

void IIC_Send_Byte(u8 txd)
{                        
    unsigned char t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1; 	  
		delay_ms(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_ms(2); 
		IIC_SCL=0;	
		delay_ms(2);
    }	 
} 

unsigned char IIC_Read_Byte(void)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
  for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_ms(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_ms(2); 
    }
	return receive;
}


