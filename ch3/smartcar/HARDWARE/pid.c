
 /**
 * 
 *   Website	: http://www.mrobotit.cn/
 * 	 Author 	: Xiao Dong
 * 	 Description: PID控制算法实现
 * 	 Date		: 2020/11
 * 	 
 */
#include "pid.h"
#include "usart.h"

#define KP   150.0f 
#define KI   100.0f 
#define KD   100.0f		


 Moto_Info Left_moto;
 Moto_Info Right_moto;
 
//调试代码 
//float send_data[2];
//PID控制速度
void Pid_Control_speed(float current_speed,float target_speed, unsigned char Moto_ID)
{
	float Error   = 0;
	float P_Error = 0;
	float I_Error = 0;
	float D_Error = 0;
	float add     = 0;
	
	if(Moto_ID == MOTO_LEFT)
	{
		Error = target_speed - current_speed;
		//计算比例微分积分项
		P_Error = Error;
		I_Error = Error - Left_moto.L_Error;
		D_Error = Error - 2*Left_moto.L_Error + Left_moto.LL_Error;
		
		//计算比例微分积分
		add = KP * P_Error + KI * I_Error + KD * D_Error;
		Left_moto.ESC_Output_PWM += add;
		//记录速度差
		Left_moto.LL_Error = Left_moto.L_Error;
		Left_moto.L_Error = Error;
		//判断是否超出PWM限制
		if(Left_moto.ESC_Output_PWM > ESC_OUTPUT_PWM_LIMT)	Left_moto.ESC_Output_PWM = ESC_OUTPUT_PWM_LIMT;		
		else if(Left_moto.ESC_Output_PWM < -ESC_OUTPUT_PWM_LIMT)	Left_moto.ESC_Output_PWM = -ESC_OUTPUT_PWM_LIMT;
	
	//调试代码 	
	//	send_data[0] = current_speed*1000;
	//	send_data[1] = target_speed*1000;
	//	Vcan_sendware((uint8_t*)&send_data,sizeof(send_data));
		
	}else if(Moto_ID == MOTO_RIGHT)
	{
		Error = target_speed - current_speed;
		//计算比例微分积分项
		P_Error = Error;
		I_Error = Error - Right_moto.L_Error;
		D_Error = Error - 2*Right_moto.L_Error + Right_moto.LL_Error;

		//计算比例微分积分
		add = KP * P_Error + KI * I_Error + KD * D_Error;
		Right_moto.ESC_Output_PWM += add;
		
		//记录速度差
		Right_moto.LL_Error = Right_moto.L_Error;
		Right_moto.L_Error = Error;

		//判断是否超出PWM限制
		if(Right_moto.ESC_Output_PWM > ESC_OUTPUT_PWM_LIMT)	Right_moto.ESC_Output_PWM = ESC_OUTPUT_PWM_LIMT;		
		else if(Right_moto.ESC_Output_PWM < -ESC_OUTPUT_PWM_LIMT) Right_moto.ESC_Output_PWM = -ESC_OUTPUT_PWM_LIMT;
		
	}
	//PWM控制左右电机
	 PWM_OUTPUT(Left_moto.ESC_Output_PWM, MOTO_LEFT);
	 PWM_OUTPUT(Right_moto.ESC_Output_PWM, MOTO_RIGHT);
}