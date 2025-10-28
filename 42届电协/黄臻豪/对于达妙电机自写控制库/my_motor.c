#include "stm32f4xx_hal.h"
#include "my_motor.h"
#include "drv_can.h"
#include "string.h"
#include "stdbool.h"

extern void Error_Handler(void);




//初始化达妙电机函数，清零电机状态并且写入电机所需参数
void dm_motor_init(motor_t *m)
{
	memset(m, 0, sizeof(*m));
	   // 电机基本参数
    m->id = 0x01U;
    m->tmp.read_flag = 1;
    
    // 控制参数设定
    m->ctrl.vel_set = 1.0f;       // 速度设定值
    m->ctrl.pos_set = 0.0f;       // 位置设定值
    m->ctrl.cur_set = 0.03f;      // 电流设定值
    m->ctrl.kp_set = 20.0f;       // KP设定值
    m->ctrl.kd_set = 0.5f;        // KD设定值
    
    // 电机限制参数
    m->tmp.PMAX = 12.5f;          // 最大位置
    m->tmp.VMAX = 30.0f;          // 最大速度
    m->tmp.TMAX = 10.0f;          // 最大扭矩
    
    // 电机控制参数
    m->tmp.KP_APR = 20.0f;        // 位置环比例增益
    m->tmp.KI_APR = 0.0f;         // 位置环积分增益
    m->tmp.V_BW = 500.0f;         // 速度环带宽
    m->tmp.I_BW = 2000.0f;        // 电流环带宽
    m->tmp.KI_ASR = 0.1f;         // 电流环积分增益
    
    // 电机保护参数
    m->tmp.UV_Value = 10.0f;      // 欠压保护值
    m->tmp.OV_Value = 36.0f;      // 过压保护值
    m->tmp.OC_Value = 20.0f;      // 过流保护值
    m->tmp.OT_Value = 85.0f;      // 过温保护值
    
    // 电机特性参数
    m->tmp.KT_Value = 0.1f;       // 扭矩常数
    m->tmp.Rs = 0.1f;             // 相电阻
    m->tmp.Ls = 0.001f;           // 相电感
    m->tmp.Flux = 0.01f;          // 磁通量
    m->tmp.Gr = 100.0f;           // 减速比
    m->tmp.p_m = 7.0f;            // 磁极对数
    
    // 运动控制参数
    m->tmp.ACC = 100.0f;          // 加速度
    m->tmp.DEC = 100.0f;          // 减速度
    m->tmp.MAX_SPD = 30.0f;       // 最大速度
    
    // CAN通信参数
    m->tmp.MST_ID = 0x201U;       // 主ID
    m->tmp.ESC_ID = 0x201U;       // 从ID
    m->tmp.can_br = 1000000U;     // CAN波特率
    m->tmp.TIMEOUT = 100U;        // 超时时间
    
    // 其他参数
    m->tmp.Damp = 0.1f;           // 阻尼系数
    m->tmp.Inertia = 0.01f;       // 转动惯量
    m->tmp.Deta = 0.01f;          // 死区补偿
    m->tmp.VL_cl = 32.0f;         // 电压限制
    m->tmp.GREF = 1.0f;           // 参考增益
    m->tmp.dir = 1.0f;            // 方向
    m->tmp.u_off = 0.0f;          // U相偏移
    m->tmp.v_off = 0.0f;          // V相偏移
    m->tmp.k1 = 1.0f;             // 电流系数1
    m->tmp.k2 = 0.0f;             // 电流系数2
    m->tmp.m_off = 0.0f;          // 测量偏移
    m->tmp.x_out = 1.0f;          // 输出限幅
     
}




//唤醒电机函数
void enable_motor_mode(CAN_HandleTypeDef *hcan, uint16_t motor_id)
{
	uint8_t data[8];
	data[0] = 0xFF;
	data[1] = 0xFF;
	data[2] = 0xFF;
	data[3] = 0xFF;
	data[4] = 0xFF;
	data[5] = 0xFF;
	data[6] = 0xFF;
	data[7] = 0xFC;
	CAN_Send_Data(hcan,motor_id,data,8);
}


//浮点数转换成无符号整数的函数

int float_to_uint(float x_float, float x_min, float x_max, int bits)
{
	/* Converts a float to an unsigned int, given range and number of bits */
	float span = x_max - x_min;
	float offset = x_min;
	return (int) ((x_float-offset)*((float)((1<<bits)-1))/span);
}


//整数转换成浮点数函数，可用来获取小数位的状态数据
float uint_to_float(int x_int, float x_min, float x_max, int bits)
{
	/* converts unsigned int to float, given range and number of bits */
	float span = x_max - x_min;
	float offset = x_min;
	return ((float)x_int)*span/((float)((1<<bits)-1)) + offset;
}




void mit_ctrl(CAN_HandleTypeDef *hcan, motor_t *motor, uint16_t motor_id, float pos, float vel,float kp, float kd, float tor)
{
	uint8_t data[8];
	uint16_t pos_tmp,vel_tmp,kp_tmp,kd_tmp,tor_tmp;

	pos_tmp = float_to_uint(pos, -motor->tmp.PMAX, motor->tmp.PMAX, 16);
	vel_tmp = float_to_uint(vel, -motor->tmp.VMAX, motor->tmp.VMAX, 12);
	tor_tmp = float_to_uint(tor, -motor->tmp.TMAX, motor->tmp.TMAX, 12);
	kp_tmp  = float_to_uint(kp,  KP_MIN, KP_MAX, 12);
	kd_tmp  = float_to_uint(kd,  KD_MIN, KD_MAX, 12);

	data[0] = (pos_tmp >> 8);
	data[1] = pos_tmp;
	data[2] = (vel_tmp >> 4);
	data[3] = ((vel_tmp&0xF)<<4)|(kp_tmp>>8);
	data[4] = kp_tmp;
	data[5] = (kd_tmp >> 4);
	data[6] = ((kd_tmp&0xF)<<4)|(tor_tmp>>8);
	data[7] = tor_tmp;
	
	if (CAN_Send_Data(hcan, motor_id, data, 8) != HAL_OK)
		{
        Error_Handler();  // 或自定义错误处理
    }

}








