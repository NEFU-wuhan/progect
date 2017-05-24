#include "common.h"
#include "include.h"

int speedwantB=0;
int speedwantC=100;
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

int8 Ramp_flag=0;            //�µ���־  -1������  0��ƽ·  1������   Ramp_flag_pre
uint8 Final_line_flag;

short int speedwant_R;
short int speedwant_L;


#if  ( CAR_MODEL==1 )
        float KP2=120;      //����ϵ��500
        float Ki2=80;      //����ϵ��220
        float Kd2=10;
#elif (CAR_MODEL==2 )
        float KP2=120;      //����ϵ��500
        float Ki2=85;      //����ϵ��220
        float Kd2=10;
#elif (CAR_MODEL==3 )
        float KP2=120;      //����ϵ��500
        float Ki2=85;      //����ϵ��220
        float Kd2=10;
#endif

float kvff=20;
float kvff2=20;
float kvff_param=100;   //ǰ���Ŵ�ϵ��
float kvff_param2=100;  //ǰ���Ŵ�ϵ��
float kaff=10;
float kaff2=10;
int16 error_R[3];
int16 error_L[3];
int16 error_Z[3];
void Fuzzy(uint16 P_L,uint16 P_R);
float fuzzy_kp(float e,float ec);
uint16 Ki_l=0,Ki_r=0;
int  x ;
int16 Distance_Current_Pule=0; //������ʻ����
int32 Total_distance;       //��·��
uint8 flag_change_speed;    //����ʹ��
int shache;
int16 Car_Speed_ave;
uint8 wandao_number;
int set_value[8];
int set_value2[8];
int speed_exact[8];
int speed_exact2[8];

int16 Car_Speed_average[8] ;
//-------------------------------------------------
//                 �����ʼ��
//����
//���
//���� �����ʼ��
//���� 2016-06-24
//����
//--------------------------------------------------
void init_motor()
{
    ftm_pwm_init(MOTOR_FTM, MOTOR2_PWM,MOTOR_HZ,0);      //���ǵ���Ƶ�����õ�10*1000
    ftm_pwm_init(MOTOR_FTM, MOTOR5_PWM,MOTOR_HZ,0);
    ftm_pwm_init(MOTOR_FTM, MOTOR3_PWM,MOTOR_HZ,0);
    ftm_pwm_init(MOTOR_FTM, MOTOR6_PWM,MOTOR_HZ,0);
}

//-------------------------------------------------
//                 �ٶȿ���
//����
//���
//���� �ٶȿ���
//���� 2016-06-24
//����
//--------------------------------------------------

void speedcontrol5()

{
      Spd_Dtc_Get();
/////////////////////   �ٶ�����  /////////////////////////////////
      speed_input();
///////////////////// ���ټ���   /////////////n ////////////////////
      nextError=SE_duty-Mid_duty;        //1552     1518
  //   y=(p1_3*ABS(nextError)*ABS(nextError)*ABS(nextError))+(p2_3*ABS(nextError)*ABS(nextError))+p3_3*ABS(nextError)+p4_3;                                  //���εĲ���
      y=(p1*ABS(nextError))+p2;       //һ�εĲ��ٱ���
      unsigned char zhi=100;

//////////////////////////////////////////////////////////
///     �µĲ����뷨��
///         �����ʱ���Ȱ����������ٽ��м��㣬
///         Ȼ������жϣ���������ٶ�������ٶ�ƫ���С���Ͳ�

//////////////////////////////////////////////////////////
      if(nextError>=zhi)        //����ת          //        30  <=  nextE                rror
    {
     speedwant_L=(2*y*speedwant)/(1+y);   //
     speedwant_R=(2*speedwant)/(1+y);

   }
   else if((nextError+zhi<=0))      //����ת            //         nextError    <=  -30
    {
     speedwant_L=(2*speedwant)/(1+y);
     speedwant_R=(2*y*speedwant)/(1+y);     //+30*y
    }
  else
    {
     speedwant_L=speedwant;
     speedwant_R=speedwant;
     }





   if(Motor_En==1)      //���ʹ��
   {


          if(speedwant_pre==160&&speedwant==Menu_data_num[0])
          {
           kvff_param=5;
           kvff_param2=5;
          }
          else
          {
            kvff_param=1;
           kvff_param2=1;
          }
         //�ҵ��

          error_L[2]=error_L[1];
          error_L[1]=error_L[0];
          error_L[0]=speedwant_L-leftval;     //�����������
          error_R[2]=error_R[1];
          error_R[1]=error_R[0];
          error_R[0]=speedwant_R-rightval;     //�����������
          speedaboutangle += KP2 *(error_L[0]-error_L[1])+Ki2*error_L[0]+  Kd2 * (error_L[0]-2*error_L[1]+error_L[2]);
          speedaboutangle2 += KP2 *(error_R[0]-error_R[1])+Ki2*error_R[0]+  Kd2 * (error_R[0]-2*error_R[1]+error_R[2]);






        ///////////////����޷�///////////////////////
          if(speedaboutangle > 10000  &&  Motor_En==1)  speedaboutangle = 10000;
          if(speedaboutangle <-10000  &&  Motor_En==1)  speedaboutangle =-10000;
          if(speedaboutangle2> 10000  &&  Motor_En==1)  speedaboutangle2= 10000;
          if(speedaboutangle2<-10000  &&  Motor_En==1)  speedaboutangle2=-10000;
        ///////////////������///////////////////////
          if(speedaboutangle>0  &&  Motor_En==1)                       //2��5������
          {
            ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM,0);
            ftm_pwm_duty(MOTOR_FTM, MOTOR5_PWM,speedaboutangle);
          }
          else if(speedaboutangle<=0  &&  Motor_En==1)
          {
            ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM,-speedaboutangle);
            ftm_pwm_duty(MOTOR_FTM, MOTOR5_PWM,0);
          }
         if(speedaboutangle2>0  &&  Motor_En==1)                        //3��6���ҵ��
         {
          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM,0);
          ftm_pwm_duty(MOTOR_FTM, MOTOR6_PWM,speedaboutangle2);
         }
         else if(speedaboutangle2<=0  &&  Motor_En==1)
         {
          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM,-speedaboutangle2);
          ftm_pwm_duty(MOTOR_FTM, MOTOR6_PWM,0);
         }
    }
}




//-------------------------------------------------
//                 �ٶ�·�̻�ȡ����
//���� void
//��� g_CarSpd   g_Distance
//���� ��ȡ�ٶ���·��
//���� 2016-06-24
//����
//--------------------------------------------------

void Spd_Dtc_Get()
{
    static int16 Speed_pre,Spees_pre_pre,Car_Speed;
    uint8 i=0;
    float number=0;

    Spees_pre_pre=Speed_pre;
    Speed_pre=Car_Speed;

    rightval =-ftm_quad_get(FTM2);      //��ȡ����
    leftval  =ftm_quad_get(FTM1);

    dj_protect(rightval,leftval);        //�����������

    Car_Speed=(rightval+leftval)/2;    //�������ٶ���ƽ��
    Car_Speed_ave=(Car_Speed +Speed_pre+Spees_pre_pre)/3;   //�ٶ��˲�
    Car_Speed_average[0]=Car_Speed_ave;

    for(i=7;i>0;i--)     //�洢��������ֵ
    {
     Car_Speed_average[i]=Car_Speed_average[i-1];
    }

    Distance_Current_Pule+=Car_Speed;  //������ʻ�����ۼ�

//    if(Distance_Current_Pule>Pule_to_Dtc_Proportion)    //����ת��Ϊ·��
//    {
//      Total_distance++;       //��·���ۼ�
//      Distance_Current_Pule-=Pule_to_Dtc_Proportion;  //���������Ӧ����
//    }
    if(Distance_Current_Pule>(Pule_to_Dtc_Proportion))    //����ת��Ϊ·��
    {
      number= (Distance_Current_Pule/Pule_to_Dtc_Proportion);
      Total_distance+= number ;       //��·���ۼ�
      Distance_Current_Pule-=(Pule_to_Dtc_Proportion*number);  //���������Ӧ����
    }

    ftm_quad_clean(FTM1);               //�������
    ftm_quad_clean(FTM2);

}

//-------------------------------------------------
//                 �ٶ����ú���
//���� void
//��� speedwant_L   speedwant_R
//���� ��������������ٶ�
//���� 2016-06-26
//����
//--------------------------------------------------
void speed_input()
{


//   if((int)s_distance<100&&(int)s_distance>5)//����С��3m,����0.05m��Ч
//    {
//      speedwantB=speedwantD;    //speedwantC
      speedwant=speedwantD;
//	if(s_distance<20)
//          speedwant =(int) (speedwantB * 0.5);
//        else if(s_distance<25)
//          speedwant =(int) (speedwantB * 0.6);
//        else if(s_distance<30)
//          speedwant =(int) (speedwantB * 0.7);
//        else if(s_distance<40)
//          speedwant =(int) (speedwantB * 0.8);
//        else if(s_distance<50)
//          speedwant =(int) (speedwantB *0.90);
//        else if(s_distance<55)
//          speedwant =(int) (speedwantB * 0.95);
//        else if(s_distance<65)                           //�ٶ�ϵ����ӽ�1��ʱ�򣬾��������Ƶľ��롣
//          speedwant =(int) (speedwantB * 1.1);
//        else if(s_distance<70)
//          speedwant =(int) (speedwantB * 1.15);
//        else if(s_distance<95)
//          speedwant =(int) (speedwantB * 1.2);
//        else
//          speedwant =(int) (speedwantB * 1.22);
//
//        if(speedwant>(int)(speedwantB*1.10))//�����ٶ�ʧ��
//          speedwant=(int)(speedwantB*1.10);
//    }
//    else
//      speedwant =speedwantE;//(int)(speedwantB * 1.2) ;//��������ʧ�Ļ������趨�ٶ�100;//50;//(
//       printf("%d/n",speedwant);      speedwantC
//       printf("%d\n",s_distance);
//   if(fiag_huan) speedwant=0;
}

/*******************************************
*                                          *
*            2017.1.13  ZQ�޸�             *
*              �����������                *
*       ������������ұ������ɼ�ֵ         *
*                                         *
******************************************/
void dj_protect(long int rval,long int lval)
{
//    if((rightval<10||leftval<10)&&Motor_En==1)
//    {
//        DJ_protect++;
//        if(DJ_protect>200)     //��Լ200��  ��1��
//        {
//              speedwant=0;
//              speedwantB=0;
//              Motor_En=0;
//              ftm_pwm_init(FTM0, FTM_CH4,10000, 0);    //������ת
//              ftm_pwm_init(FTM0, FTM_CH5,10000, 0);    //������ת
//              ftm_pwm_init(FTM0, FTM_CH6,10000, 0);    //�ҵ����ת
//              ftm_pwm_init(FTM0, FTM_CH7,10000, 0);    // //�ҵ����ת
//        }
//    }
}