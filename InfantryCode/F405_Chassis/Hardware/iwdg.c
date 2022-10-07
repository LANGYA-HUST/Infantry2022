#include "iwdg.h"
/*
 *        �������Ź�ʹ��LSI��Ϊʱ�ӡ�
 *        LSI ��Ƶ��һ���� 30~60KHZ ֮�䣬�����¶Ⱥ͹������ϻ���һ����Ư�ƣ���
 *        ��һ��ȡ 40KHZ�����Զ������Ź��Ķ�ʱʱ�䲢һ���ǳ���ȷ��ֻ�����ڶ�ʱ�侫��
 *        Ҫ��Ƚϵ͵ĳ��ϡ�
 *
 * rlv:��װ�ؼĴ�����ֵ��ȡֵ��ΧΪ��0-0XFFF
 * �������þ�����
 * IWDG_Config(IWDG_Prescaler_64 ,625);  // IWDG 1s ��ʱ��� 
 *                        ��64/40��*625 = 1s
 */
void IWDG_Config(uint8_t prv ,uint16_t rlv)
{    
    // ʹ�� Ԥ��Ƶ�Ĵ���PR����װ�ؼĴ���RLR��д
    IWDG_WriteAccessCmd( IWDG_WriteAccess_Enable );
    
    // ����Ԥ��Ƶ��ֵ
    IWDG_SetPrescaler( prv );
    
    // ������װ�ؼĴ���ֵ
    IWDG_SetReload( rlv );
    
    // ����װ�ؼĴ�����ֵ�ŵ���������
    IWDG_ReloadCounter();
    
    // ʹ�� IWDG
    IWDG_Enable();    
}

// ι��
void IWDG_Feed(void)
{
    // ����װ�ؼĴ�����ֵ�ŵ��������У�ι������ֹIWDG��λ
    // ����������ֵ����0��ʱ������ϵͳ��λ
    IWDG_ReloadCounter();
}

