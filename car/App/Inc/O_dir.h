#ifndef _O_DIR_H
#define _O_DIR_H

/*��������*/
#define MOTOR_FTM   FTM0
#define MOTOR2_PWM  FTM_CH5
#define MOTOR5_PWM  FTM_CH4
#define MOTOR3_PWM  FTM_CH7
#define MOTOR6_PWM  FTM_CH6


#define S3010_FTM   FTM3
#define S3010_CH    FTM_CH0
#define S3010_HZ    (50)

/*�������*/
extern float mid_line;
extern int32 SE_duty;                  //�����PWMռ�ձ�
extern int Mid_duty ;

extern void DJ_PID(void);




#endif  //_GEMINI_DIRECTION_CONTROL_H
