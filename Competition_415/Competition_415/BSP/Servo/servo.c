#include "servo.h"
#include "delay.h"

//映射
static uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
  return (out_max - out_min) / (in_max - in_min) *(x - in_min)   + out_min;
}

uint32_t period = 10000;



unsigned int Servo_Angle = 0;//舵机角度

unsigned int Servo_Angle270 = 0;//舵机角度


float target_phi;
float target_theta = 135;

float last_phi;
float last_theta = 135;

float real_phi;
float real_theta = 135;

float step_phi;
float step_theta;

float Y_DIS = 91;

float now_x, now_z;

/******************************************************************
 * 函 数 名 称：Set_Servo_Angle
 * 函 数 说 明：设置270角度
 * 函 数 形 参：angle=要设置的角度，范围0-270
 * 函 数 返 回：无
******************************************************************/
void Set_Servo270_Angle(float angle)
{
      if(angle > 270) angle = 270; // 限制角度在0到270度之间
			
      Servo_Angle270 = angle;
			
      // 计算PWM占空比
      // 0.5ms对应的计数 = 50
      // 2.5ms对应的计数 = 250
      float min_count = 0.025 * period;
      float max_count = 0.125 * period;
      float range = max_count - min_count;
      float ServoAngle = min_count + (angle / 270.0f * range);

      //DL_TimerG_setCaptureCompareValue(PWM_Servo_INST, (unsigned int)(ServoAngle + 0.5f), GPIO_PWM_Servo_C1_IDX);
}


/******************************************************************
 * 函 数 名 称：Set_Servo_Angle
 * 函 数 说 明：设置180角度
 * 函 数 形 参：angle=要设置的角度，范围0-180
 * 函 数 返 回：无
******************************************************************/
void Set_Servo_Angle(float angle)
{

      if(angle > 180) angle = 180; // 限制角度在0到180度之间

      Servo_Angle = angle;

      // 计算PWM占空比
      // 0.5ms对应的计数 = 50
      // 2.5ms对应的计数 = 250
      float min_count = 0.025 * period;
      float max_count = 0.125 * period;
      float range = max_count - min_count;
      float ServoAngle = min_count + (angle / 180.0f * range);

      //DL_TimerG_setCaptureCompareValue(PWM_Servo_INST, (unsigned int)(ServoAngle + 0.5f), GPIO_PWM_Servo_C0_IDX);
}


unsigned int Get_Servo_Angle(void){return Servo_Angle;}
unsigned int Get_Servo_Angle270(void){return Servo_Angle270;}


void Set_TH(float angle){Set_Servo270_Angle(angle+45);}
void Set_PH(float angle){Set_Servo_Angle(angle);}
unsigned int Get_TH(void){return Get_Servo_Angle270()-45;}
unsigned int Get_PH(void){return Get_Servo_Angle();}




// 设置舵机 PWM
void Set_PWM(int pwma,int pwmb)
{
	DL_Timer_setCaptureCompareValue(PWM_PHI_INST,pwma,GPIO_PWM_PHI_C1_IDX);
	DL_Timer_setCaptureCompareValue(PWM_THETA_INST,pwmb,GPIO_PWM_THETA_C0_IDX);
}

// 将角度转换为PWM值（适用于270度舵机）
int angle_to_pwm_270(float angle)
{
    // 限制角度在0到270度之间
    if (angle < 0.0f) {
        angle = 0.0f;
    } else if (angle > 270.0f) {
        //angle = 270.0f;
    }
    
    // 线性转换公式：PWM = (angle * 1000 / 270) + 250
    float pwm = (angle * 1000.0f / 270.0f) + 250.0f;
    return (int)roundf(pwm); // 四舍五入到整数
}

int angle_to_pwm_180(float angle) 
{

    if (angle < 0.0f) {
        angle = 0.0f;
    } else if (angle > 180.0f) {
        angle = 180.0f;
    }
    
    float pwm = (angle * 1000.0f / 180.0f) + 250.0f;
    return (int)roundf(pwm); // 四舍五入到整数
}

void Set_Angle(float theta, float phi)// 注意是先 theta 后 phi
{
      Set_PWM(angle_to_pwm_180(phi), angle_to_pwm_270(theta));
}

float base_x(float y,float x)
{
	float angle = 0;
	angle = 135.0f - (atan(x/y))*57.3f;
	return angle;
}

float base_z(float y,float z)
{
	float angle = 0;
	angle = (atan(z/y))*57.3f;
	return angle;
}

void move_Point(float x, float y, float z)
{
    int steps = 100;

    for(int i=1;i<=steps;i++)
    {
        float tx = (now_x * (steps - i) + x * i) / steps;
        float tz = (now_z * (steps - i) + z * i) / steps;

        target_theta = base_x(y, tx);
        target_phi = base_z(y, tz);

        // 计算每步的增量
        step_theta = (target_theta - last_theta) / 100.0f;
        step_phi = (target_phi - last_phi) / 100.0f;

        // 更新上一次的角度
        last_theta = target_theta;
        last_phi = target_phi;

        // 计算实际角度（插值）
        real_theta = last_theta + step_theta;
        real_phi = last_phi + step_phi;

        // 转换为 PWM 信号
        int pwmA = angle_to_pwm_180(real_phi);
        int pwmB = angle_to_pwm_270(real_theta);
        // 输出到舵机
        Set_PWM(pwmA,pwmB);
        Set_Angle(real_theta, real_phi);
        delay_ms(1);
    }

    now_x = x;
    now_z = z;
}