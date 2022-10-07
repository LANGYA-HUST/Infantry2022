#include "KalmanFilter.h"
/*
const float delta_t = 0.001;
const short init_sigma_vx = 1000;
const short sigma_x = 20;
const short sigma_vx = 10;
*/

short KalMan_doneflag=0;//卡尔曼初始化完成标志位

KalmanFilter_Init_t K=
{
  .P_data = {1000*1000*0.001*0.001, 1000*1000*0.001,
						 1000*1000*0.001, 1000*1000},
  .A_data = {1, 0.001,
						 0, 1},
  .H_data = {1, 0,
						 0, 0},
  .Q_data = {10*10*0.001*0.001, 10*10*0.001,
						 10*10*0.001, 10*10},
  .R_data = {20*20, 0,
						 0, 20*20}
};


void KalmanFilter_Init(KalmanFilter_t *Kalman, KalmanFilter_Init_t *I)
{
  mat_init(&Kalman->data,2,1,(float *)I->xhat_data);
  mat_init(&Kalman->predict_data,2,1,(float *)I->xhatminus_data);
  mat_init(&Kalman->z,2,1,(float *)I->z_data);
  mat_init(&Kalman->A,2,2,(float *)I->A_data);
  mat_init(&Kalman->H,2,2,(float *)I->H_data);
  mat_init(&Kalman->Q,2,2,(float *)I->Q_data);
  mat_init(&Kalman->R,2,2,(float *)I->R_data);
  mat_init(&Kalman->P,2,2,(float *)I->P_data);
  mat_init(&Kalman->predict_P,2,2,(float *)I->Pminus_data);
  mat_init(&Kalman->K,2,2,(float *)I->K_data);
  mat_init(&Kalman->AT,2,2,(float *)I->AT_data);
  mat_trans(&Kalman->A, &Kalman->AT);
  mat_init(&Kalman->HT,2,2,(float *)I->HT_data);
  mat_trans(&Kalman->H, &Kalman->HT);
}

float KalmanFilter_Calc(KalmanFilter_t *Kalman, float signal)
{
  float TEMP_data[4] = {0, 0, 0, 0};
  float TEMP_data21[2] = {0, 0};
  mat TEMP,TEMP21;

  mat_init(&TEMP,2,2,(float *)TEMP_data);
  mat_init(&TEMP21,2,1,(float *)TEMP_data21);

  Kalman->z.pData[0] = signal;
  Kalman->z.pData[1] = 0;

  //1. data'(k)= A data(k-1)
  mat_mult(&Kalman->A, &Kalman->data, &Kalman->predict_data);

  //2. P'(k) = A P(k-1) AT + Q
  mat_mult(&Kalman->A, &Kalman->P, &Kalman->predict_P);
  mat_mult(&Kalman->predict_P, &Kalman->AT, &TEMP);
  mat_add(&TEMP, &Kalman->Q, &Kalman->predict_P);

  //3. K(k) = P'(k) HT / (H P'(k) HT + R)
  mat_mult(&Kalman->H, &Kalman->predict_P, &Kalman->K);
  mat_mult(&Kalman->K, &Kalman->HT, &TEMP);
  mat_add(&TEMP, &Kalman->R, &Kalman->K);

  mat_inv(&Kalman->K, &Kalman->P);
  mat_mult(&Kalman->predict_P, &Kalman->HT, &TEMP);
  mat_mult(&TEMP, &Kalman->K, &Kalman->P);

  //4. data(k) = data'(k) + K(k) (z(k) - H data'(k))
  mat_mult(&Kalman->H, &Kalman->predict_data, &TEMP21);
  mat_sub(&Kalman->z, &TEMP21, &Kalman->data);
  mat_mult(&Kalman->K, &Kalman->data, &TEMP21);
  mat_add(&Kalman->predict_data, &TEMP21, &Kalman->data);

  //5. P(k) = (1-K(k)H)P'(k)
//  mat_mult(&Kalman->K, &Kalman->H, &Kalman->P);
//  mat_sub(&Kalman->Q, &Kalman->P, &TEMP);
//  mat_mult(&TEMP, &Kalman->predict_P, &Kalman->P);
  mat_mult(&Kalman->K, &Kalman->H, &Kalman->P);
	mat_mult(&Kalman->P, &Kalman->predict_P, &TEMP);
  mat_sub(&Kalman->predict_P, &TEMP, &Kalman->P);

  Kalman->filtered_value[0] = Kalman->data.pData[0];
  Kalman->filtered_value[1] = Kalman->data.pData[1];

  return Kalman->filtered_value[0];
}

