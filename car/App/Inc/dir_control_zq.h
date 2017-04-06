#ifndef _dir_control_zq_H
#define _dir_control_zq_H

/*设置引脚*/
#define MOTOR_FTM   FTM0
#define MOTOR2_PWM  FTM_CH5
#define MOTOR5_PWM  FTM_CH4
#define MOTOR3_PWM  FTM_CH7
#define MOTOR6_PWM  FTM_CH6


#define S3010_FTM   FTM3
#define S3010_CH    FTM_CH0
#define S3010_HZ    (50)

/*舵机参数*/
extern float mid_line;
extern int32 SE_duty;                  //舵机的PWM占空比


extern void DJ_PID(void);



#endif  
