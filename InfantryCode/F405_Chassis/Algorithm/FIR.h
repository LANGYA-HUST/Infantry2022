#ifndef __FIR_H__
#define __FIR_H__

void Fir(float *Input,float *Output);
void LowPass_SetChassis(float* output,float In);
float LowPass_SetWheel(float In,float past);

#define ORDER        5                       //½×Êý
#endif
