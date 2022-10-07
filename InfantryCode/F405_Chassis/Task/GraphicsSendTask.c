/**********************************************************************************************************
 * @文件     Graphics_Send.c
 * @说明     裁判系统图形发送
 * @版本  	 V1.0
 * @作者     陈志鹏
 * @日期     2021.4
**********************************************************************************************************/
/**********************************************************************************************************
 * @文件     Graphics_Send.c
 * @说明     裁判系统图形发送
 * @版本  	 V2.0
 * @作者     戴军
 * @日期     2022.5
**********************************************************************************************************/
#include "main.h"
#define CAP_GRAPHIC_NUM 9			//超级电容电量图形显示细分格数
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
*函 数 名: JudgementCustomizeGraphics
*功能说明: 裁判系统自定义图形发送函数
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
ext_student_interactive_header_data_t custom_grapic_draw;			//自定义图像绘制
//装载图形
void JudgementCustomizeGraphics(int Op_type)
{
		custom_grapic_draw.data_cmd_id=0x0104;//绘制七个图形（内容ID，查询裁判系统手册）

		custom_grapic_draw.sender_ID=JudgeReceive.robot_id;//发送者ID，机器人对应ID
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

/*********************************自定义图像数据***********************************/
		referee_data_load_Graphic(Op_type);
}

///**********************************************************************************************************
//*函 数 名: referee_data_load_shootUI
//*功能说明: 图形数据包装载射击图形
//*形    参: 操作类型，机器人等级
//*返 回 值: 无
//**********************************************************************************************************/
//short bias_x[7] = {0,15,13,12,11,10,9};
//short bias_y[7] = {0,113,118,123,128,133,138};
//short bias_r = 100;
//short screen_bias = -20;
float XX = 0.58,YY = 0.15;
//void referee_data_load_shootUI(uint8_t operate_type,uint8_t robot_level)
//{
//	/*******************************画近似直线射击圆*********************************/
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[0] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[1] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[2] = 0;//图形名
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].operate_tpye=operate_type;//图形操作，0：空操作；1：增加；2：修改；3：删除；
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_tpye=2;//图形类型，0为直线，其他的查看用户手册
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].layer=0;//图层数
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].color=0;//颜色
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].width=1;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_x=SCREEN_LENGTH/2 + bias_x[0];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_y=SCREEN_WIDTH/2 + bias_y[0];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_x=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_y=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[0].radius= 2 + bias_r;

///*******************************图1*********************************/
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[0] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[1] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[2] = 1;//图形名
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].operate_tpye=operate_type;//图形操作，0：空操作；1：增加；2：修改；3：删除；
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_tpye=0;//图形类型，0为直线，其他的查看用户手册
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].layer=0;//图层数
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].color=1;//颜色
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].width=1;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_x=SCREEN_LENGTH/2 - bias_x[1] + screen_bias;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_y=SCREEN_WIDTH/2 - bias_y[1];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_x  =SCREEN_LENGTH/2 + bias_x[1] + screen_bias;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_y  =SCREEN_WIDTH/2 - bias_y[1];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[1].radius=0;

///*******************************图2*********************************/
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_name[0] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_name[1] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_name[2] = 2;//图形名
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].operate_tpye=operate_type;//图形操作，0：空操作；1：增加；2：修改；3：删除；
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_tpye=0;//图形类型，0为直线，其他的查看用户手册
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].layer=0;//图层数
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].color=2;//颜色
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].start_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].end_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].width=1;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].start_x=SCREEN_LENGTH/2 - bias_x[2] + screen_bias;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].start_y=SCREEN_WIDTH/2 - bias_y[2];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].end_x  =SCREEN_LENGTH/2 + bias_x[2] + screen_bias;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].end_y  =SCREEN_WIDTH/2 - bias_y[2];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[2].radius=0;

///*******************************图3*********************************/
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_name[0] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_name[1] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_name[2] = 3;//图形名
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].operate_tpye=operate_type;//图形操作，0：空操作；1：增加；2：修改；3：删除；
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_tpye=0;//图形类型，0为直线，其他的查看用户手册
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].layer=0;//图层数
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].color=3;//颜色
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].start_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].end_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].width=1;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].start_x=SCREEN_LENGTH/2 - bias_x[3] + screen_bias;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].start_y=SCREEN_WIDTH/2 - bias_y[3];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].end_x  =SCREEN_LENGTH/2 + bias_x[3] + screen_bias;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].end_y  =SCREEN_WIDTH/2 - bias_y[3];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[3].radius=0;

///*******************************图4*********************************/
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[0] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[1] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[2] = 4;//图形名
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].operate_tpye=operate_type;//图形操作，0：空操作；1：增加；2：修改；3：删除；
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_tpye=0;//图形类型，0为直线，其他的查看用户手册
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].layer=0;//图层数
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].color=4;//颜色
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].width=1;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_x=SCREEN_LENGTH/2 - bias_x[4] + screen_bias;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_y=SCREEN_WIDTH/2 - bias_y[4];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_x  =SCREEN_LENGTH/2 + bias_x[4] + screen_bias;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_y  =SCREEN_WIDTH/2 - bias_y[4];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[4].radius=0;

///*******************************图5*********************************/
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[0] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[1] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[2] = 5;//图形名
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].operate_tpye=operate_type;//图形操作，0：空操作；1：增加；2：修改；3：删除；
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_tpye=0;//图形类型，0为直线，其他的查看用户手册
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].layer=0;//图层数
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].color=5;//颜色
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_angle=0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].width=1;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_x=SCREEN_LENGTH/2 - bias_x[5] + screen_bias;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_y=SCREEN_WIDTH/2 - bias_y[5];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_x  =SCREEN_LENGTH/2 + bias_x[5] + screen_bias;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_y  =SCREEN_WIDTH/2 - bias_y[5];
//		custom_grapic_draw.graphic_custom.grapic_data_struct[5].radius=0;

///*******************************图6*********************************/
//		custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[0] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[1] = 0;
//		custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[2] = 6;//图形名
//		custom_grapic_draw.graphic_custom.grapic_data_struct[6].operate_tpye=operate_type;//图形操作，0：空操作；1：增加；2：修改；3：删除；
//		custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_tpye=0;//图形类型，0为直线，其他的查看用户手册
//		custom_grapic_draw.graphic_custom.grapic_data_struct[6].layer=0;//图层数
//		custom_grapic_draw.graphic_custom.grapic_data_struct[6].color=6;//颜色
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
//*函 数 名: referee_data_load_NumberUI
//*功能说明: 图形数据包装载数字（浮点/字符/整形）
//*形    参: 无
//*返 回 值: 无
//**********************************************************************************************************/
//void referee_data_load_NumberUI(void)
//{
//	static unsigned int pitch = 0;
//	pitch = F405.Gimbal_100 * 10;
//	/*******************************Pitch浮点数*********************************/
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
*函 数 名: referee_data_load_Graphic
*功能说明: 图形数据包装载图形
*形    参: 操作类型
*返 回 值: 无
**********************************************************************************************************/
extern float AD_actual_value;		//电容实际电压
char Color_array[CAP_GRAPHIC_NUM] = {Green,Green,Green,Green,Green,Green,Green,Green,Green};//储存颜色
float g_pos_x[CAP_GRAPHIC_NUM] = {0.57,0.34,0.4,0.52,0.34,0.42,0.62,0.5,0.42};
float g_pos_y[CAP_GRAPHIC_NUM] = {0.65,0.15,0.8,0.1,0.1,0.15,0.1,0.8,0.1};
/*瞄准线偏移量*/
int AIM_bias_x = 0;
int AIM_bias_y = 0;
int placece_x[14]={0  , 50, 30,  30, 30,  10,  7,  7,  7,  10,  7,  7,  7 ,10 };
int placece_y[15]={-80,-320,-80,-100,-120,-140,-160,-180,-200,-220,-240,-260,-280,10, 10 };

#define PACK_NUM 4
void referee_data_load_Graphic(int Op_type)
{
	static int pack_tick = 0;			//数据包计数器
	static unsigned int pitch = 0;
	static float Start_Place_x[CAP_GRAPHIC_NUM] = {0.388,0.41,0.432,0.454,0.476,0.498,0.52,0.542,0.564};
  static float Start_Place_y = 0.02;
	static float End_Place_x[CAP_GRAPHIC_NUM] = {0.398,0.42,0.442,0.464,0.486,0.508,0.53,0.552,0.574};
	static float End_Place_y = 0.04;
	static int i;
	int packed_tick = 0;							//装包计数器
	/*初始化操作，轮流生成图层*/
	if(Op_type == Op_Init)
	{
		switch(pack_tick % PACK_NUM)
		{
		case 0:

				/*******************************电容电压*********************************/
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
						/*******************************pitch角度更新*********************************/
PITCH_:	pitch = F405.Gimbal_100 * 10;
				/*******************************Pitch 浮点数*********************************/
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
//				/*******************************左车线*********************************/
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

//				/*******************************右车线*********************************/
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

//				/*********************************瞄准十字横线*********************************/
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
				/*从这以下是射击辅助线*/
				/*********************************中线*********************************/
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
				
				/*********************************横线0，中间最长线*********************************/
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
				
				/*********************************横线1*********************************/
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
				
			
				/*********************************横线2*********************************/
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
			
				/*********************************横线3*********************************/
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
				
				/*********************************横线4*********************************/
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

				/*********************************横线5*********************************/
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

				/*********************************横线6*********************************/
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

				/*********************************横线7*********************************/
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

				/*********************************横线8*********************************/
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
				
					/*********************************横线9*********************************/
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
			
				/*********************************横线10*********************************/
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

//				/*********************************瞄准十字竖线*********************************/
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
*函 数 名: Graphic_Change_Check
*功能说明: 检测有没有图形的变动，或者进行图层的初始化
*形    参: 无
*返 回 值: 
**********************************************************************************************************/
int Graphic_Change_Check(void)
{
	int int_AD_actual_value; 	//取整			
	char vol_num; 
	int pitch_100;
	static char last_vol_num;
	static int last_pitch_100;
	static char last_color_array[CAP_GRAPHIC_NUM];
	static int last_Buff_Flag;
	/*电容数字百分比*/
	float cap_vol;
	/*用于初始化基本图形，如车道线，框线，和辅助瞄准线等*/
	if(F405.Graphic_Init_Flag == 0)		
	{
		return Op_Init;	//返回Init,会一直发送Add，添加所有图层
	}
	
	/***********************Pitch角度变化检测部分************************/
	pitch_100 = F405.Gimbal_100;
	if(pitch_100 != last_pitch_100) pitch_change_flag = Op_Change;
	last_pitch_100 = pitch_100;
	
//		/***********************停车线档位变化检测部分************************/
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
	
//	/***********************Buff状态变化检测部分************************/
//	if(F405.Gimbal_Flag == Gimbal_Buff_Mode)
//		Buff_flag = 1;
//	else
//		Buff_flag = 0;
//	if(Buff_flag != last_Buff_Flag)
//	{
//			buff_change_flag = Op_Change;
//	}
//	last_Buff_Flag = Buff_flag;
	
	/*电容电压检测部分*/
	if(last_cap_vol != AD_actual_value) cap_change_flag = Op_Change;
	last_cap_vol = AD_actual_value;
	
	/*检索有没有发生变化，如果有变化则返回修改图层*/
	if(pitch_change_flag == Op_Change || park_change_flag == Op_Change || cap_change_flag == Op_Change) return Op_Change;


	/*都没有变化*/
	return Op_None;	//返回空操作
}
/**********************************************************************************************************
*函 数 名: referee_data_pack_handle
*功能说明: 裁判系统图形数据打包发送
*形    参: uint8_t sof,uint16_t cmd_id, uint8_t *p_data, uint16_t len
*返 回 值: 无
**********************************************************************************************************/
uint8_t seq = 0;	
void referee_data_pack_handle(uint8_t sof,uint16_t cmd_id, uint8_t *p_data, uint16_t len)
{
	uint16_t frame_length = frameheader_len + cmd_len + len + crc_len;   //数据帧长度	

	memset(JudgeSend,0,frame_length);  //存储数据的数组清零
	
	/*****帧头打包*****/
	JudgeSend[0] = sof;//数据帧起始字节
	memcpy(&JudgeSend[1],(uint8_t*)&len, sizeof(len));//数据帧中data的长度
	JudgeSend[3] = seq;//包序号
	Append_CRC8_Check_Sum(JudgeSend,frameheader_len);  //帧头校验CRC8

	/*****命令码打包*****/
	memcpy(&JudgeSend[frameheader_len],(uint8_t*)&cmd_id, cmd_len);
	
	/*****数据打包*****/
	memcpy(&JudgeSend[frameheader_len+cmd_len], p_data, len);
	Append_CRC16_Check_Sum(JudgeSend,frame_length);  //一帧数据校验CRC16

	if (seq == 0xff) seq=0;
  else seq++;
	
	/*****数据上传*****/
  while(DMA_GetCmdStatus(DMA1_Stream4)==ENABLE);
	DMA1_Stream4->NDTR = frame_length;
	DMA_Cmd(DMA1_Stream4, ENABLE);
	
}
/**********************************************************************************************************
*函 数 名: GraphicSendtask
*功能说明: 图形发送任务
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
extern ext_student_interactive_char_header_data_t custom_char_draw;  //自定义字符绘制
int char_change_state,graphic_change_state;
int GraficTaskcnt = 0;
void GraphicSendtask(void *pvParameters)
{
	
   while (1) {
    
			/*浮点数，图形*/
			graphic_change_state = Graphic_Change_Check();
			if(graphic_change_state)
			{
				JudgementCustomizeGraphics(graphic_change_state);
				if(graphic_change_state != Op_None)
				referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&custom_grapic_draw,sizeof(custom_grapic_draw));
			}
	
      IWDG_Feed();//喂狗
			GraficTaskcnt++;
			vTaskDelay(50); 	 
#if INCLUDE_uxTaskGetStackHighWaterMark
		
#endif
    }
}
