#ifndef pid_dsp_H
#define pid_dsp_H

#include <stdint.h>

typedef struct
{
    float Kp;
    float Ki;
    float Kd;
    float MaxOut;    // 输出限幅
    float IntegralLimit; // 积分抗饱和限幅
    
    float Error;
    float LastError;
    float Integral;
    float Output;
} PID_TypeDef;

void PID_Init(PID_TypeDef *pid, float kp, float ki, float kd, float max_out, float integral_limit);
float PID_Calc(PID_TypeDef *pid, float target, float measure);

#endif