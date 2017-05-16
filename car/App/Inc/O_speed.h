#ifndef __O_speed_h__
#define __O_speed_h__

#define MOTOR_FTM   FTM0
#define MOTOR2_PWM  FTM_CH5
#define MOTOR5_PWM  FTM_CH4
#define MOTOR3_PWM  FTM_CH7
#define MOTOR6_PWM  FTM_CH6

#define MOTOR_HZ    10000//(20*1000)   //

#define Pule_to_Dtc_Proportion  68           //脉冲到路程转化比例 路程单位为 厘米      //13622   /200 =68

extern long int speederror;
extern long int speederror2;
extern int speedwantC;
extern int speedwantD;
extern int speedwantE;

extern long int DJ_protect;

extern long int speedlasterror;
extern long int speedlasterror2;

extern short int speedreal;
extern long int speedreal2;
extern long int speedreal3;
extern int32 leijia;

extern short int speedwant_R;
extern short int speedwant_L;

extern float KP2;//比例系数500
extern float Ki2;//积分系数220
extern float Kd2;
extern float Kp22;//比例系数500
extern float Ki22;//积分系数220
extern float Kd22;

extern int32 Total_distance;       //总路程

extern void init_motor();   //电机初始化
extern void Spd_Dtc_Get();
extern void speed_input();
extern void Motor_protect();
extern uint8 flag_change_speed;
extern void dj_protect(long int rval,long int lval);
extern int16 Car_Speed_average[8] ;


#endif

