#include "common.h"
#include "include.h"

int speedwantB;
int speedwantC;
int speedwantD;
int speedwantE;
extern int32 se_du;

long int speederror;
long int speederror2;

long int speedwant_pre;

long int speedlasterror;
long int speedlasterror2;
extern long int speedwant;
short int speedreal;
long int speedreal2;
long int speedreal3;
int32 leijia;


long int DJ_protect=0;

uint8 car_go;
uint8 Final_line_flag;

short int speedwant_R;
short int speedwant_L;


        float KP2;      //比例系数500
        float Ki2;      //积分系数220
        float Kd2;

float kvff=20;
float kvff2=20;
float kvff_param=100;   //前馈放大系数
float kvff_param2=100;  //前馈放大系数
float kaff=10;
float kaff2=10;
int16 error_R[3];
int16 error_L[3];
int16 error_Z[3];
void Fuzzy(uint16 P_L,uint16 P_R);
float fuzzy_kp(float e,float ec);
uint16 Ki_l=0,Ki_r=0;
int  x ;
int16 Distance_Current_Pule=0; //本次行驶脉冲
int32 Total_distance;       //总路程
uint8 flag_change_speed;    //变速使能
int shache;
int16 Car_Speed_ave;
uint8 wandao_number;
int set_value[8];
int set_value2[8];
int speed_exact[8];
int speed_exact2[8];

int16 Car_Speed_average[8] ;
uint8 wrz_distance=0;     //直道超车记步
//-------------------------------------------------
//                 电机初始化
//输入
//输出
//功能 电机初始化
//日期 2016-06-24
//作者
//--------------------------------------------------
void init_motor()
{
    ftm_pwm_init(MOTOR_FTM, MOTOR2_PWM,MOTOR_HZ,0);      //我们当初频率设置的10*1000
    ftm_pwm_init(MOTOR_FTM, MOTOR5_PWM,MOTOR_HZ,0);
    ftm_pwm_init(MOTOR_FTM, MOTOR3_PWM,MOTOR_HZ,0);
    ftm_pwm_init(MOTOR_FTM, MOTOR6_PWM,MOTOR_HZ,0);
}

//-------------------------------------------------
//                 速度控制
//输入
//输出
//功能 速度控制
//日期 2016-06-24
//作者
//--------------------------------------------------

void speedcontrol5()

{
      Spd_Dtc_Get();
/////////////////////   速度设置  /////////////////////////////////
      speed_input();
///////////////////// 差速计算   /////////////n ////////////////////
      nextError=Mid_duty-SE_duty;        //1552     1518
  //   y=(p1_3*ABS(nextError)*ABS(nextError)*ABS(nextError))+(p2_3*ABS(nextError)*ABS(nextError))+p3_3*ABS(nextError)+p4_3;                                  //二次的差速
      y=(p1*ABS(nextError))+p2;       //一次的差速比例
      unsigned char zhi=100;

//////////////////////////////////////////////////////////
///     新的差速想法；
///         弯道的时候先按照正常差速进行计算，
///         然后进行判断：如果内轮速度与计算速度偏差较小，就不

//////////////////////////////////////////////////////////
      if(nextError>=zhi)        //向左转          //        30  <=  nextE                rror
    {
     speedwant_L=(2*y*speedwant)/(1+y);   //
     speedwant_R=(2*speedwant)/(1+y);

   }
   else if((nextError+zhi<=0))      //向右转            //         nextError    <=  -30
    {
     speedwant_L=(2*speedwant)/(1+y);
     speedwant_R=(2*y*speedwant)/(1+y);     //+30*y
    }
  else
    {
     speedwant_L=speedwant;
     speedwant_R=speedwant;
     }





   if(Motor_En==1)      //电机使能
   {

         //右电机

          error_L[2]=error_L[1];
          error_L[1]=error_L[0];
          error_L[0]=speedwant_L-leftval;     //这个比是右轮
          error_R[2]=error_R[1];
          error_R[1]=error_R[0];
          error_R[0]=speedwant_R-rightval;     //这个比是左轮
          speedaboutangle2 += KP2 *(error_L[0]-error_L[1])+Ki2*error_L[0]+  Kd2 * (error_L[0]-2*error_L[1]+error_L[2]);
          speedaboutangle  += KP2 *(error_R[0]-error_R[1])+Ki2*error_R[0]+  Kd2 * (error_R[0]-2*error_R[1]+error_R[2]);






        ///////////////输出限幅///////////////////////
          if(speedaboutangle > 10000  &&  Motor_En==1)  speedaboutangle = 10000;
          if(speedaboutangle <-10000  &&  Motor_En==1)  speedaboutangle =-10000;
          if(speedaboutangle2> 10000  &&  Motor_En==1)  speedaboutangle2= 10000;
          if(speedaboutangle2<-10000  &&  Motor_En==1)  speedaboutangle2=-10000;

        ///////////////电机输出///////////////////////
          if(speedaboutangle>0  &&  Motor_En==1)                       //2、5是you电机
          {
            ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM,0);
            ftm_pwm_duty(MOTOR_FTM, MOTOR5_PWM,speedaboutangle);
          }
          else if(speedaboutangle<=0  &&  Motor_En==1)
          {
            ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM,-speedaboutangle);
            ftm_pwm_duty(MOTOR_FTM, MOTOR5_PWM,0);
          }
         if(speedaboutangle2>0  &&  Motor_En==1)                        //3、6是zuo电机
         {
          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM,0);
          ftm_pwm_duty(MOTOR_FTM, MOTOR6_PWM,speedaboutangle2);
         }
         else if(speedaboutangle2<=0  &&  Motor_En==1   )
         {
          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM,-speedaboutangle2);
          ftm_pwm_duty(MOTOR_FTM, MOTOR6_PWM,0);
         }
    }
}




//-------------------------------------------------
//                 速度路程获取函数
//输入 void
//输出 g_CarSpd   g_Distance
//功能 获取速度与路程
//日期 2016-06-24
//作者
//--------------------------------------------------
void Spd_Dtc_Get()
{
    static int16 Speed_pre,Spees_pre_pre,Car_Speed;
    uint8 i=0;
    float number=0;

    Spees_pre_pre=Speed_pre;
    Speed_pre=Car_Speed;

    leftval   =-ftm_quad_get(FTM2);      //获取脉冲
    rightval  = ftm_quad_get(FTM1);

    dj_protect(rightval,leftval);        //电机保护函数

    Car_Speed=(rightval+leftval)/2;    //左右轮速度求平均
    Car_Speed_ave=(Car_Speed +Speed_pre+Spees_pre_pre)/3;   //速度滤波
    Car_Speed_average[0]=Car_Speed_ave;

    for(i=7;i>0;i--)     //存储编码器的值
    {
     Car_Speed_average[i]=Car_Speed_average[i-1];
    }

    Distance_Current_Pule+=Car_Speed;  //本次行驶脉冲累计

//    if(Distance_Current_Pule>Pule_to_Dtc_Proportion)    //脉冲转化为路程
//    {
//      Total_distance++;       //总路程累加
//      Distance_Current_Pule-=Pule_to_Dtc_Proportion;  //脉冲计数对应减少
//    }
    if(Distance_Current_Pule>(Pule_to_Dtc_Proportion))    //脉冲转化为路程
    {
      number= (Distance_Current_Pule/Pule_to_Dtc_Proportion);
      Total_distance+= number ;       //总路程累加

       if((L_wrz_flag==1||R_wrz_flag==1||L_wrz_flag==2||R_wrz_flag==2) && xia_no==0 && Ramp_flag==1 ) //完全出环道之后才开始记步停车
        wrz_distance+=number;

      Distance_Current_Pule-=(Pule_to_Dtc_Proportion*number);  //脉冲计数对应减少
    }

    ftm_quad_clean(FTM1);               //清空脉冲
    ftm_quad_clean(FTM2);

}

//-------------------------------------------------
//                 速度设置函数
//输入 void
//输出 speedwant_L   speedwant_R
//功能 分情况设置输入速度
//日期 2016-06-26
//作者
//--------------------------------------------------
void speed_input()
{
  uint16 control_dis=70;
   if((int)s_distance<150&&(int)s_distance>5)//距离小于3m,大于0.05m有效
    {
      speedwantB=speedwantD;

	if(s_distance<(control_dis-50))
          speedwant =(int) (speedwantB * 0.5);
        else if(s_distance<(control_dis-40))
          speedwant =(int) (speedwantB * 0.6);
        else if(s_distance<(control_dis-30))
          speedwant =(int) (speedwantB * 0.7);
        else if(s_distance<(control_dis-20))
          speedwant =(int) (speedwantB * 0.8);
        else if(s_distance<(control_dis-10))
          speedwant =(int) (speedwantB *0.90);
        else if(s_distance<(control_dis-5))
          speedwant =(int) (speedwantB * 0.95);

        else if(s_distance==(control_dis))          //速度系数最接近1的时候，就是所控制的距离。
          speedwant =(int) (speedwantB );

        else if(s_distance<(control_dis+5))
          speedwant =(int) (speedwantB * 1.1);
        else if(s_distance<(control_dis+15))
          speedwant =(int) (speedwantB * 1.15);
        else if(s_distance<(control_dis+30))
          speedwant =(int) (speedwantB * 1.2);
        else
          speedwant =(int) (speedwantB * 1.22);

        if(speedwant>(int)(speedwantB*1.30))//避免速度失控
          speedwant=(int)(speedwantB*1.30);
    }
    else
      speedwant =speedwantE;//(int)(speedwantB * 1.2) ;//超声波丢失的话保持设定速度100;//50;//(
//       printf("%d/n",speedwant);      speedwantC
//       printf("%d\n",s_distance);
//   if(fiag_huan) speedwant=0;
   if( Ramp_flag!=0 ) speedwant=80;

}

/*******************************************
*                                          *
*            2017.1.13  ZQ修改             *
*              电机保护函数                *
*       输入参数：左右编码器采集值         *
*                                         *
******************************************/
void dj_protect(long int rval,long int lval)
{
//    if((rightval<10||leftval<10)&&Motor_En==1)
//    {
//        DJ_protect++;
//        if(DJ_protect>200)     //大约200次  是1秒
//        {
//              speedwant=0;
//              speedwantB=0;
//              Motor_En=0;
//              ftm_pwm_init(FTM0, FTM_CH4,10000, 0);    //左电机正转
//              ftm_pwm_init(FTM0, FTM_CH5,10000, 0);    //左电机反转
//              ftm_pwm_init(FTM0, FTM_CH6,10000, 0);    //右电机反转
//              ftm_pwm_init(FTM0, FTM_CH7,10000, 0);    // //右电机正转
//        }
//    }
}

//-------------------------------------------------
//                 开车
//输入 void
//输出
//功能 发车时需要更改的变量及需要执行的函数
//日期
//作者
//说明 single为 0 表示双车都启动，为 1 表示单车启动
//--------------------------------------------------
void car_start(uint8 single)
{
  speedwantD=speedwantD_set;
  speedwantE=speedwantE_set;

  if(single==0)
  {
    uart_putchar(UART3,Start);
//    var2=1;
//    updata_var(VAR2);                //更新编号变量的值（修改变量的值后，需要调用此函数来更新编号变量的值）
//    tongbu[2]=10;
  }
}

//-------------------------------------------------
//                 停车
//输入 void
//输出
//功能 停车时需要更改的变量及需要执行的函数
//日期
//作者
//说明 single为 0 表示双车都停止，为 1 表示单车停止
//--------------------------------------------------
void car_stop(uint8 single)
{
  speedwantD=0;
  speedwantE=0;

  if(single==0)
  {
    uart_putchar(UART3,Stop);
//    var2=0;
//    updata_var(VAR2);                //更新编号变量的值（修改变量的值后，需要调用此函数来更新编号变量的值）
//    tongbu[2]=10;
  }
}

//-------------------------------------------------
//                 按键发车
//输入 void
//输出
//功能 按键发车时需要更改的变量及需要执行的函数
//日期
//作者
//--------------------------------------------------
void car_start_key()
{
  if(front_car == 1)
  {
    speedwantD=speedwantD_set;
    speedwantE=speedwantE_set;
  }
  else
  {
    speedwantD=speedwantD_set;
    speedwantE=speedwantE_set ; // + 3
  }
  uart_putchar(UART3,Start);
  Time_En=1;
  Motor_En=1;
//  var2=1;
//  updata_var(VAR2);                //更新编号变量的值（修改变量的值后，需要调用此函数来更新编号变量的值）
//  tongbu[2]=10;

}
//-------------------------------------------------
//                 串口（接受方）发停车
//输入 void
//输出
//功能 串口收到发车停车信号时需要更改的变量及需要执行的函数
//--------------------------------------------------
void uart_start_stop( uint8 go)
{
    key_flag_clear=1;
    shua_one=0;

    if(go==1)     //表示发车
    {
      key_flag_clear=1;
      shua_one=0;

      Motor_En=1;
      Time_En=1;
      flag_key_select=5;
      flag_key_l_u_0=0;

      car_start(1);
    }
    else          //表示停车
    {
      flag_key_select=0;
      flag_key_l_u_5=0;

      car_stop(1);
    }

}