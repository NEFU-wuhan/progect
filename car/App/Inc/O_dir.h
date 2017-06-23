#ifndef _O_DIR_H
#define _O_DIR_H

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
extern int Mid_duty ;

extern float Kp1;  //16
extern float Kp2;
extern float Kd;
extern int MID_dir_duty;
extern int MAX_dir_duty;
extern int MIN_dir_duty;
extern float index_A,index_B;    //index_A为斜率  index_B为截距   斜率一般更为常用

extern void DJ_PID(void);
//障碍记次数
extern uint8 left_obstancle_acc;
extern uint8 right_obstancle_acc;
extern uint8 obstancle_acc;



#endif  //_GEMINI_DIRECTION_CONTROL_H

