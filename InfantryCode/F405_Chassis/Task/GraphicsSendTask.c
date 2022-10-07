/**********************************************************************************************************
 * @�ļ�     Graphics_Send.c
 * @˵��     ����ϵͳͼ�η���
 * @�汾  	 V1.0
 * @����     ��־��
 * @����     2021.4
**********************************************************************************************************/
/**********************************************************************************************************
 * @�ļ�     Graphics_Send.c
 * @˵��     ����ϵͳͼ�η���
 * @�汾  	 V2.0
 * @����     ����
 * @����     2022.5
**********************************************************************************************************/
#include "main.h"
#define CAP_GRAPHIC_NUM 9			//�������ݵ���ͼ����ʾϸ�ָ���
extern unsigned char JudgeSend[SEND_MAX_SIZE];
extern JudgeReceive_t JudgeReceive;
extern F405_typedef F405;
int pitch_change_flag;
int buff_change_flag;
int cap_change_flag;
int park_change_flag;
int Buff_flag;
char park_line,last_park_line;

float last_cap_vol;
/**********************************************************************************************************
*�� �� ��: JudgementCustomizeGraphics
*����˵��: ����ϵͳ�Զ���ͼ�η��ͺ���
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
ext_student_interactive_header_data_t custom_grapic_draw;			//�Զ���ͼ�����
//װ��ͼ��
void JudgementCustomizeGraphics(int Op_type)
{
		custom_grapic_draw.data_cmd_id=0x0104;//�����߸�ͼ�Σ�����ID����ѯ����ϵͳ�ֲᣩ

		custom_grapic_draw.sender_ID=JudgeReceive.robot_id;//������ID�������˶�ӦID
		if(JudgeReceive.robot_id == 103)
				custom_grapic_draw.receiver_ID = 0x0167;
		if(JudgeReceive.robot_id == 104)
				custom_grapic_draw.receiver_ID = 0x0168;
		if(JudgeReceive.robot_id == 105)
				custom_grapic_draw.receiver_ID = 0x0169;
		if(JudgeReceive.robot_id == 3)
				custom_grapic_draw.receiver_ID = 0x0103;	
		if(JudgeReceive.robot_id == 4)
				custom_grapic_draw.receiver_ID = 0x0104;
		if(JudgeReceive.robot_id == 5)
				custom_grapic_draw.receiver_ID = 0x0105;

/*********************************�Զ���ͼ������***********************************/
		referee_data_load_Graphic(Op_type);
}

///**********************************************************************************************************
//*�� �� ��: referee_data_load_shootUI
//*����˵��: ͼ�����ݰ�װ�����ͼ��
//*��    ��: �������ͣ������˵ȼ�
//*�� �� ֵ: ��
//**********************************************************************************************************/
//short bias_x[7] = {0,15,13,12,11,10,9};
//short bias_y[7] = {0,113,118,123,128,133,138};
//short bias_r = 100;
//short screen_bias = -20;
float XX = 0.58,YY = 0.15;
//void referee_data_load_shootUI(uint8_t operate_type,uint8_t robot_level)
//{
//	/*******************************������ֱ�����Բ*********************************/
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[0] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[1] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[2] = 0;//ͼ����
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].operate_tpye=operate_type;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_tpye=2;//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].layer=0;//ͼ����
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].color=0;//��ɫ
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].width=1;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_x=SCREEN_LENGTH/2 + bias_x[0];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_y=SCREEN_WIDTH/2 + bias_y[0];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_x=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_y=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].radius= 2 + bias_r;

///*******************************ͼ1*********************************/
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[0] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[1] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[2] = 1;//ͼ����
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].operate_tpye=operate_type;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_tpye=0;//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].layer=0;//ͼ����
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].color=1;//��ɫ
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].width=1;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_x=SCREEN_LENGTH/2 - bias_x[1] + screen_bias;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_y=SCREEN_WIDTH/2 - bias_y[1];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_x  =SCREEN_LENGTH/2 + bias_x[1] + screen_bias;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_y  =SCREEN_WIDTH/2 - bias_y[1];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].radius=0;

///*******************************ͼ2*********************************/
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_name[0] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_name[1] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_name[2] = 2;//ͼ����
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].operate_tpye=operate_type;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_tpye=0;//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].layer=0;//ͼ����
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].color=2;//��ɫ
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].start_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].end_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].width=1;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].start_x=SCREEN_LENGTH/2 - bias_x[2] + screen_bias;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].start_y=SCREEN_WIDTH/2 - bias_y[2];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].end_x  =SCREEN_LENGTH/2 + bias_x[2] + screen_bias;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].end_y  =SCREEN_WIDTH/2 - bias_y[2];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].radius=0;

///*******************************ͼ3*********************************/
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_name[0] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_name[1] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_name[2] = 3;//ͼ����
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].operate_tpye=operate_type;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_tpye=0;//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].layer=0;//ͼ����
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].color=3;//��ɫ
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].start_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].end_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].width=1;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].start_x=SCREEN_LENGTH/2 - bias_x[3] + screen_bias;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].start_y=SCREEN_WIDTH/2 - bias_y[3];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].end_x  =SCREEN_LENGTH/2 + bias_x[3] + screen_bias;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].end_y  =SCREEN_WIDTH/2 - bias_y[3];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].radius=0;

///*******************************ͼ4*********************************/
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[0] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[1] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[2] = 4;//ͼ����
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].operate_tpye=operate_type;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_tpye=0;//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].layer=0;//ͼ����
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].color=4;//��ɫ
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].width=1;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_x=SCREEN_LENGTH/2 - bias_x[4] + screen_bias;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_y=SCREEN_WIDTH/2 - bias_y[4];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_x  =SCREEN_LENGTH/2 + bias_x[4] + screen_bias;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_y  =SCREEN_WIDTH/2 - bias_y[4];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].radius=0;

///*******************************ͼ5*********************************/
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[0] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[1] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[2] = 5;//ͼ����
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].operate_tpye=operate_type;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_tpye=0;//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].layer=0;//ͼ����
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].color=5;//��ɫ
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].width=1;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_x=SCREEN_LENGTH/2 - bias_x[5] + screen_bias;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_y=SCREEN_WIDTH/2 - bias_y[5];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_x  =SCREEN_LENGTH/2 + bias_x[5] + screen_bias;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_y  =SCREEN_WIDTH/2 - bias_y[5];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].radius=0;

///*******************************ͼ6*********************************/
//		custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[0] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[1] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[2] = 6;//ͼ����
//		custom_grapic_draw.graphic_custom.grapic_data_struct[6].operate_tpye=operate_type;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
//		custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_tpye=0;//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
//		custom_grapic_draw.graphic_custom.grapic_data_struct[6].layer=0;//ͼ����
//		custom_grapic_draw.graphic_custom.grapic_data_struct[6].color=6;//��ɫ
//		custom_grapic_draw.graphic_custom.grapic_data_struct[6].start_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[6].end_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[6].width=1;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[6].start_x=SCREEN_LENGTH/2 - bias_x[6] + screen_bias;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[6].start_y=SCREEN_WIDTH/2 - bias_y[6];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[6].end_x  =SCREEN_LENGTH/2 + bias_x[6] + screen_bias;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[6].end_y  =SCREEN_WIDTH/2 - bias_y[6];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[6].radius=0;
//}

///**********************************************************************************************************
//*�� �� ��: referee_data_load_NumberUI
//*����˵��: ͼ�����ݰ�װ�����֣�����/�ַ�/���Σ�
//*��    ��: ��
//*�� �� ֵ: ��
//**********************************************************************************************************/
//void referee_data_load_NumberUI(void)
//{
//	static unsigned int pitch = 0;
//	pitch = F405.Gimbal_100 * 10;
//	/*******************************Pitch������*********************************/
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[0] = 0;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[1] = 3;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[2] = 1;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].operate_tpye=Op_Add;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_tpye=5;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].layer=3;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].color=Pink;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_angle=20;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_angle=3;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].width=4;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_x=0.7 * SCREEN_LENGTH;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_y=0.7 * SCREEN_WIDTH;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].radius = pitch & 0x03ff;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_x  = (pitch >> 10) & 0x07ff;
//	custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_y  = (pitch >> 21) & 0x07ff;
//}
/**********************************************************************************************************
*�� �� ��: referee_data_load_Graphic
*����˵��: ͼ�����ݰ�װ��ͼ��
*��    ��: ��������
*�� �� ֵ: ��
**********************************************************************************************************/
extern float AD_actual_value;		//����ʵ�ʵ�ѹ
char Color_array[CAP_GRAPHIC_NUM] = {Green,Green,Green,Green,Green,Green,Green,Green,Green};//������ɫ
float g_pos_x[CAP_GRAPHIC_NUM] = {0.57,0.34,0.4,0.52,0.34,0.42,0.62,0.5,0.42};
float g_pos_y[CAP_GRAPHIC_NUM] = {0.65,0.15,0.8,0.1,0.1,0.15,0.1,0.8,0.1};
/*��׼��ƫ����*/
int AIM_bias_x = 0;
int AIM_bias_y = 0;
int placece_x[14]={0  , 50, 30,  30, 30,  10,  7,  7,  7,  10,  7,  7,  7 ,10 };
int placece_y[15]={-80,-320,-80,-100,-120,-140,-160,-180,-200,-220,-240,-260,-280,10, 10 };

#define PACK_NUM 4
void referee_data_load_Graphic(int Op_type)
{
	static int pack_tick = 0;			//���ݰ�������
	static unsigned int pitch = 0;
	static float Start_Place_x[CAP_GRAPHIC_NUM] = {0.388,0.41,0.432,0.454,0.476,0.498,0.52,0.542,0.564};
  static float Start_Place_y = 0.02;
	static float End_Place_x[CAP_GRAPHIC_NUM] = {0.398,0.42,0.442,0.464,0.486,0.508,0.53,0.552,0.574};
	static float End_Place_y = 0.04;
	static int i;
	int packed_tick = 0;							//װ��������
	/*��ʼ����������������ͼ��*/
	if(Op_type == Op_Init)
	{
		switch(pack_tick % PACK_NUM)
		{
		case 0:

				/*******************************���ݵ�ѹ*********************************/
CAP_PERCENT:				
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[0] = 0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[1] = 3;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[2] = 22;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].operate_tpye=Op_type;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_tpye=5;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].layer=3;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].color=Pink;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_angle=20;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_angle=3;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].width=4;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_x=XX * SCREEN_LENGTH;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_y=YY * SCREEN_WIDTH;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].radius = (int)(AD_actual_value*1000) & 0x03ff;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_x  = ((int)(AD_actual_value*1000) >> 10) & 0x07ff;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_y  = ((int)(AD_actual_value*1000) >> 21) & 0x07ff;
				if(Op_type == Op_Change) goto CONT_1;
						/*******************************pitch�Ƕȸ���*********************************/
PITCH_:	pitch = F405.Gimbal_100 * 10;
				/*******************************Pitch ������*********************************/
				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?1:0].graphic_name[0] = 0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?1:0].graphic_name[1] = 3;
				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?1:0].graphic_name[2] = 1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?1:0].operate_tpye=Op_type;
				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?1:0].graphic_tpye=5;
				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?1:0].layer=3;
				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?1:0].color=Pink;
				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?1:0].start_angle=20;
				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?1:0].end_angle=2;
				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?1:0].width=4;
				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?1:0].start_x=0.65 * SCREEN_LENGTH;
				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?1:0].start_y=0.65 * SCREEN_WIDTH;
				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?1:0].radius = pitch & 0x03ff;
				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?1:0].end_x  = (pitch >> 10) & 0x07ff;
				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?1:0].end_y  = (pitch >> 21) & 0x07ff;
				if(Op_type == Op_Change) goto CONT_2;
//				/*******************************����*********************************/
//Park:		custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?2:0].graphic_name[0] = 0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?2:0].graphic_name[1] = 1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?2:0].graphic_name[2] = 0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?2:0].operate_tpye=Op_type;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?2:0].graphic_tpye=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?2:0].layer=1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?2:0].color=Orange;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?2:0].start_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?2:0].end_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?2:0].width=1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?2:0].start_x=SCREEN_LENGTH * 0.46;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?2:0].start_y=SCREEN_WIDTH/2 * 0.9;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?2:0].end_x=SCREEN_LENGTH * 0.298;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?2:0].end_y=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?2:0].radius = 0;

//				/*******************************�ҳ���*********************************/
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?3:1].graphic_name[0] = 0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?3:1].graphic_name[1] = 1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?3:1].graphic_name[2] = 1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?3:1].operate_tpye=Op_type;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?3:1].graphic_tpye=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?3:1].layer=1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?3:1].color=Orange;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?3:1].start_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?3:1].end_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?3:1].width=1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?3:1].start_x=SCREEN_LENGTH * 0.5;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?3:1].start_y=SCREEN_WIDTH/2 * 0.9;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?3:1].end_x  =SCREEN_LENGTH * 0.658;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?3:1].end_y  =0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[Op_type==Op_Init?3:1].radius=0;
//				if(Op_type == Op_Change) goto CONT_3;
				break;
				
		case 2:

//				/*********************************��׼ʮ�ֺ���*********************************/
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_name[0] = 0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_name[1] = 1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_name[2] = 7;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].operate_tpye=Op_type;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_tpye=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].layer=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].color=Orange;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].start_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].end_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].width=1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].start_x=SCREEN_LENGTH/2 - placece_x[13] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].start_y=SCREEN_WIDTH/2 + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].end_x  =SCREEN_LENGTH/2 + placece_x[13] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].end_y  =SCREEN_WIDTH/2 + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].radius=0;
				/*�������������������*/
				/*********************************����*********************************/
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[0] = 0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[1] = 1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[2] = 8;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].operate_tpye=Op_type;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_tpye=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].layer=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].color=Green;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].width=1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_x=SCREEN_LENGTH/2 - placece_x[0] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_y=SCREEN_WIDTH/2 + placece_y[0] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_x  =SCREEN_LENGTH/2 - placece_x[0] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_y  =SCREEN_WIDTH/2 + placece_y[1] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].radius=0;
				
				/*********************************����0���м����*********************************/
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[0] = 0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[1] = 1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[2] = 9;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].operate_tpye=Op_type;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_tpye=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].layer=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].color=Green;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].width=1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_x=SCREEN_LENGTH/2 - placece_x[1] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_y=SCREEN_WIDTH/2 + placece_y[2] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_x  =SCREEN_LENGTH/2 + placece_x[1] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_y  =SCREEN_WIDTH/2 + placece_y[2] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].radius=0;
				
				/*********************************����1*********************************/
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[0] = 0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[1] = 1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[2] = 10;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].operate_tpye=Op_type;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_tpye=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].layer=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].color=Green;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].start_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].end_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].width=1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].start_x=SCREEN_LENGTH/2 - placece_x[2] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].start_y=SCREEN_WIDTH/2 + placece_y[3] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].end_x  =SCREEN_LENGTH/2 + placece_x[2] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].end_y  =SCREEN_WIDTH/2 + placece_y[3] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].radius=0;
				
			
				/*********************************����2*********************************/
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[0] = 0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[1] = 1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[2] = 11;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].operate_tpye=Op_type;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_tpye=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].layer=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].color=Green;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].width=1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_x=SCREEN_LENGTH/2 - placece_x[3] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_y=SCREEN_WIDTH/2 + placece_y[4] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_x  =SCREEN_LENGTH/2 + placece_x[3] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_y  =SCREEN_WIDTH/2 + placece_y[4] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].radius=0;
			
				/*********************************����3*********************************/
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[0] = 0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[1] = 1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[2] = 12;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].operate_tpye=Op_type;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_tpye=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].layer=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].color=Green;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].width=1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_x=SCREEN_LENGTH/2 - placece_x[4] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_y=SCREEN_WIDTH/2 + placece_y[5] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_x  =SCREEN_LENGTH/2 + placece_x[4] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_y  =SCREEN_WIDTH/2 + placece_y[5] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].radius=0;
				
				/*********************************����4*********************************/
				custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_name[0] = 0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_name[1] = 1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_name[2] = 13;
				custom_grapic_draw.graphic_custom.grapic_data_struct[2].operate_tpye=Op_type;
				custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_tpye=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[2].layer=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[2].color=Green;
				custom_grapic_draw.graphic_custom.grapic_data_struct[2].start_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[2].end_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[2].width=1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[2].start_x=SCREEN_LENGTH/2 - placece_x[5] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[2].start_y=SCREEN_WIDTH/2 + placece_y[6] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[2].end_x  =SCREEN_LENGTH/2 + placece_x[5] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[2].end_y  =SCREEN_WIDTH/2 + placece_y[6] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[2].radius=0;
				
				break;	
			case 3:

				/*********************************����5*********************************/
				custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_name[0] = 0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_name[1] = 1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_name[2] = 14;
				custom_grapic_draw.graphic_custom.grapic_data_struct[3].operate_tpye=Op_type;
				custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_tpye=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[3].layer=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[3].color=Green;
				custom_grapic_draw.graphic_custom.grapic_data_struct[3].start_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[3].end_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[3].width=1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[3].start_x=SCREEN_LENGTH/2 - placece_x[6] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[3].start_y=SCREEN_WIDTH/2 + placece_y[7] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[3].end_x  =SCREEN_LENGTH/2 + placece_x[6] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[3].end_y  =SCREEN_WIDTH/2 + placece_y[7] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[3].radius=0;

				/*********************************����6*********************************/
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[0] = 0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[1] = 1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[2] = 15;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].operate_tpye=Op_type;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_tpye=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].layer=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].color=Green;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].width=1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_x=SCREEN_LENGTH/2 - placece_x[7] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_y=SCREEN_WIDTH/2 + placece_y[8] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_x  =SCREEN_LENGTH/2 + placece_x[7] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_y  =SCREEN_WIDTH/2 + placece_y[8] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[4].radius=0;

				/*********************************����7*********************************/
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[0] = 0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[1] = 1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[2] = 16;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].operate_tpye=Op_type;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_tpye=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].layer=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].color=Green;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].width=1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_x=SCREEN_LENGTH/2 - placece_x[8] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_y=SCREEN_WIDTH/2 + placece_y[9] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_x  =SCREEN_LENGTH/2 + placece_x[8] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_y  =SCREEN_WIDTH/2 + placece_y[9] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[5].radius=0;

				/*********************************����8*********************************/
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[0] = 0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[1] = 1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[2] = 17;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].operate_tpye=Op_type;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_tpye=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].layer=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].color=Green;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].start_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].end_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].width=1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].start_x=SCREEN_LENGTH/2 - placece_x[9] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].start_y=SCREEN_WIDTH/2 + placece_y[10] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].end_x  =SCREEN_LENGTH/2 + placece_x[9] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].end_y  =SCREEN_WIDTH/2 + placece_y[10] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[6].radius=0;
				
					/*********************************����9*********************************/
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[0] = 0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[1] = 1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[2] = 18;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].operate_tpye=Op_type;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_tpye=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].layer=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].color=Green;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].width=1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_x=SCREEN_LENGTH/2 - placece_x[10] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_y=SCREEN_WIDTH/2 + placece_y[11] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_x  =SCREEN_LENGTH/2 + placece_x[10] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_y  =SCREEN_WIDTH/2 + placece_y[11] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[0].radius=0;
			
				/*********************************����10*********************************/
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[0] = 0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[1] = 1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[2] = 19;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].operate_tpye=Op_type;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_tpye=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].layer=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].color=Green;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_angle=0;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].width=1;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_x=SCREEN_LENGTH/2 - placece_x[11] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_y=SCREEN_WIDTH/2 + placece_y[12] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_x  =SCREEN_LENGTH/2 + placece_x[11] + AIM_bias_x;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_y  =SCREEN_WIDTH/2 + placece_y[12] + AIM_bias_y;
				custom_grapic_draw.graphic_custom.grapic_data_struct[1].radius=0;

//				/*********************************��׼ʮ������*********************************/
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_name[0] = 0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_name[1] = 1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_name[2] = 20;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].operate_tpye=Op_type;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_tpye=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].layer=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].color=Orange;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].start_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].end_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].width=2;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].start_x=SCREEN_LENGTH/2 + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].start_y=SCREEN_WIDTH/2 + placece_y[13] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].end_x  =SCREEN_LENGTH/2 + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].end_y  =SCREEN_WIDTH/2 - placece_y[13] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].radius=0;	
				break;
			
			default:
				break;
		}
		pack_tick++;
	}else if(Op_type == Op_Change)
	{
		if(cap_change_flag == Op_Change )
		{
			goto CAP_PERCENT;
CONT_1:cap_change_flag = Op_None;
		}
		if(pitch_change_flag == Op_Change )
		{
			 goto PITCH_;
CONT_2:pitch_change_flag = Op_None;
		}
//		if(park_change_flag == Op_Change )
//		{
//			 goto Park;
//CONT_3:park_change_flag = Op_None;
//		}
	}
}
/**********************************************************************************************************
*�� �� ��: Graphic_Change_Check
*����˵��: �����û��ͼ�εı䶯�����߽���ͼ��ĳ�ʼ��
*��    ��: ��
*�� �� ֵ: 
**********************************************************************************************************/
int Graphic_Change_Check(void)
{
	int int_AD_actual_value; 	//ȡ��			
	char vol_num; 
	int pitch_100;
	static char last_vol_num;
	static int last_pitch_100;
	static char last_color_array[CAP_GRAPHIC_NUM];
	static int last_Buff_Flag;
	/*�������ְٷֱ�*/
	float cap_vol;
	/*���ڳ�ʼ������ͼ�Σ��糵���ߣ����ߣ��͸�����׼�ߵ�*/
	if(F405.Graphic_Init_Flag == 0)		
	{
		return Op_Init;	//����Init,��һֱ����Add���������ͼ��
	}
	
	/***********************Pitch�Ƕȱ仯��ⲿ��************************/
	pitch_100 = F405.Gimbal_100;
	if(pitch_100 != last_pitch_100) pitch_change_flag = Op_Change;
	last_pitch_100 = pitch_100;
	
//		/***********************ͣ���ߵ�λ�仯��ⲿ��************************/
//	if(pitch_100<-500) park_line = 0;
//	else if(pitch_100 >-500 && pitch_100 < 0) park_line =1;
//	else if(pitch_100 > 0   && pitch_100 < 500) park_line =2;
//	else if(pitch_100 >500  && pitch_100 <1000) park_line =3;
//	else if(pitch_100 >1000 && pitch_100 <1500) park_line =4;
//	else if(pitch_100 >1500 && pitch_100 <2000) park_line =5;
//	else if(pitch_100 >2000 && pitch_100 <3000) park_line =6;
//	
//	if(park_line != last_park_line) park_change_flag = Op_Change;
//	last_park_line = park_line;
	
//	/***********************Buff״̬�仯��ⲿ��************************/
//	if(F405.Gimbal_Flag == Gimbal_Buff_Mode)
//		Buff_flag = 1;
//	else
//		Buff_flag = 0;
//	if(Buff_flag != last_Buff_Flag)
//	{
//			buff_change_flag = Op_Change;
//	}
//	last_Buff_Flag = Buff_flag;
	
	/*���ݵ�ѹ��ⲿ��*/
	if(last_cap_vol != AD_actual_value) cap_change_flag = Op_Change;
	last_cap_vol = AD_actual_value;
	
	/*������û�з����仯������б仯�򷵻��޸�ͼ��*/
	if(pitch_change_flag == Op_Change || park_change_flag == Op_Change || cap_change_flag == Op_Change) return Op_Change;


	/*��û�б仯*/
	return Op_None;	//���ؿղ���
}
/**********************************************************************************************************
*�� �� ��: referee_data_pack_handle
*����˵��: ����ϵͳͼ�����ݴ������
*��    ��: uint8_t sof,uint16_t cmd_id, uint8_t *p_data, uint16_t len
*�� �� ֵ: ��
**********************************************************************************************************/
uint8_t seq = 0;	
void referee_data_pack_handle(uint8_t sof,uint16_t cmd_id, uint8_t *p_data, uint16_t len)
{
	uint16_t frame_length = frameheader_len + cmd_len + len + crc_len;   //����֡����	

	memset(JudgeSend,0,frame_length);  //�洢���ݵ���������
	
	/*****֡ͷ���*****/
	JudgeSend[0] = sof;//����֡��ʼ�ֽ�
	memcpy(&JudgeSend[1],(uint8_t*)&len, sizeof(len));//����֡��data�ĳ���
	JudgeSend[3] = seq;//�����
	Append_CRC8_Check_Sum(JudgeSend,frameheader_len);  //֡ͷУ��CRC8

	/*****��������*****/
	memcpy(&JudgeSend[frameheader_len],(uint8_t*)&cmd_id, cmd_len);
	
	/*****���ݴ��*****/
	memcpy(&JudgeSend[frameheader_len+cmd_len], p_data, len);
	Append_CRC16_Check_Sum(JudgeSend,frame_length);  //һ֡����У��CRC16

	if (seq == 0xff) seq=0;
  else seq++;
	
	/*****�����ϴ�*****/
  while(DMA_GetCmdStatus(DMA1_Stream4)==ENABLE);
	DMA1_Stream4->NDTR = frame_length;
	DMA_Cmd(DMA1_Stream4, ENABLE);
	
}
/**********************************************************************************************************
*�� �� ��: GraphicSendtask
*����˵��: ͼ�η�������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
extern ext_student_interactive_char_header_data_t custom_char_draw;  //�Զ����ַ�����
int char_change_state,graphic_change_state;
int GraficTaskcnt = 0;
void GraphicSendtask(void *pvParameters)
{
	
   while (1) {
    
			/*��������ͼ��*/
			graphic_change_state = Graphic_Change_Check();
			if(graphic_change_state)
			{
				JudgementCustomizeGraphics(graphic_change_state);
				if(graphic_change_state != Op_None)
				referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&custom_grapic_draw,sizeof(custom_grapic_draw));
			}
	
      IWDG_Feed();//ι��
			GraficTaskcnt++;
			vTaskDelay(50); 	 
#if INCLUDE_uxTaskGetStackHighWaterMark
		
#endif
    }
}
