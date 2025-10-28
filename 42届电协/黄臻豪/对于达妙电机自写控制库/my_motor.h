#ifndef MY_MOTOR_H
#define MY_MOTOR_H
#include "stm32f4xx_hal.h"
#define KP_MIN  0.0f
#define KP_MAX  500.0f
#define KD_MIN  0.0f
#define KD_MAX  5.0f
#define POS_MIN -12.5f  
#define POS_MAX  12.5f
#define VEL_MIN -30.0f  
#define VEL_MAX  30.0f
#define TOR_MIN -10.0f  
#define TOR_MAX  10.0f

typedef enum
{
    Motor1,
    Motor2,
    Motor3,
    Motor4,
    Motor5,
    Motor6,
	Motor7,
	Motor8,
	Motor9,
	Motor10,
    num
} motor_num;


typedef struct
{
	uint8_t read_flag;
	uint8_t write_flag;
	uint8_t save_flag;
	
    float UV_Value;		
    float KT_Value;		
    float OT_Value;		
    float OC_Value;		
    float ACC;			
    float DEC;			
    float MAX_SPD;		
    uint32_t MST_ID;	
    uint32_t ESC_ID;	
    uint32_t TIMEOUT;	   uint32_t cmode;		
    float  	 Damp;		
    float    Inertia;
    uint32_t hw_ver;	
    uint32_t SN;		
    float    Rs;		
    float    Ls;		
    float    Flux;		
    float    Gr;		
    float    PMAX;		
    float    VMAX;		
    float    TMAX;		
    float    I_BW;		
    float    KI_ASR;	
    float    KP_APR;	
    float    KI_APR;	
    float    OV_Value;	
    float    GREF;		
    float    Deta;		
    float 	 V_BW;		
    float    VL_cl;		
    uint32_t can_br;	
    uint32_t sub_ver;	
	float 	 u_off;		
	float	 v_off;		
	float	 k1;		
	float 	 k2;		
	float 	 m_off;		
	float  	 dir;		
	float	 p_m;		
	float	 x_out;		
} esc_inf_t;


typedef struct
{
    int id;
    int state;
    int p_int;
    int v_int;
    int t_int;
    int kp_int;
    int kd_int;
    float pos;
    float vel;
    float tor;
    float Kp;
    float Kd;
    float Tmos;
    float Tcoil;
} motor_fbpara_t;


typedef struct
{
    float pos_set;
    float vel_set;
    float tor_set;
	float cur_set;
    float kp_set;
    float kd_set;
} motor_ctrl_t;

typedef struct
{
    uint16_t id;
    motor_fbpara_t para;
    motor_ctrl_t ctrl;
	esc_inf_t tmp;
} motor_t;










void dm_motor_init(motor_t *m);
void enable_motor_mode(CAN_HandleTypeDef *hcan, uint16_t motor_id);
int float_to_uint(float x_float, float x_min, float x_max, int bits);
float uint_to_float(int x_int, float x_min, float x_max, int bits);
void mit_ctrl(CAN_HandleTypeDef *hcan, motor_t *motor, uint16_t motor_id, float pos, float vel,float kp, float kd, float tor);
#endif

