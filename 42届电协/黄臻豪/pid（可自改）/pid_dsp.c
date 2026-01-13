#include "pid_dsp.h"

void PID_Init(PID_TypeDef *pid, float kp, float ki, float kd, float max_out, float integral_limit)
{
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    pid->MaxOut = max_out;
    pid->IntegralLimit = integral_limit;
    
    pid->Error = 0;
    pid->LastError = 0;
    pid->Integral = 0;
    pid->Output = 0;
}

float PID_Calc(PID_TypeDef *pid, float target, float measure)
{
    pid->Error = target - measure;
    
    pid->Integral += pid->Error;
    
    // 积分抗饱和
    if (pid->Integral > pid->IntegralLimit)
        pid->Integral = pid->IntegralLimit;
    else if (pid->Integral < -pid->IntegralLimit)
        pid->Integral = -pid->IntegralLimit;
        
    pid->Output = pid->Kp * pid->Error + pid->Ki * pid->Integral + pid->Kd * (pid->Error - pid->LastError);
    
    pid->LastError = pid->Error;
    
    // 输出限幅
    if (pid->Output > pid->MaxOut)
        pid->Output = pid->MaxOut;
    else if (pid->Output < -pid->MaxOut)
        pid->Output = -pid->MaxOut;
        
    return pid->Output;
}
