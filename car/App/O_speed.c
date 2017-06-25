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


        float KP2;      //����ϵ��500
        float Ki2;      //����ϵ��220
        float Kd2;

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
uint8 wrz_distance=0;     //ֱ�������ǲ�
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
      nextError=Mid_duty-SE_duty;        //1552     1518
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

         //�ҵ��

          error_L[2]=error_L[1];
          error_L[1]=error_L[0];
          error_L[0]=speedwant_L-leftval;     //�����������
          error_R[2]=error_R[1];
          error_R[1]=error_R[0];
          error_R[0]=speedwant_R-rightval;     //�����������
          speedaboutangle2 += KP2 *(error_L[0]-error_L[1])+Ki2*error_L[0]+  Kd2 * (error_L[0]-2*error_L[1]+error_L[2]);
          speedaboutangle  += KP2 *(error_R[0]-error_R[1])+Ki2*error_R[0]+  Kd2 * (error_R[0]-2*error_R[1]+error_R[2]);






        ///////////////����޷�///////////////////////
          if(speedaboutangle > 10000  &&  Motor_En==1)  speedaboutangle = 10000;
          if(speedaboutangle <-10000  &&  Motor_En==1)  speedaboutangle =-10000;
          if(speedaboutangle2> 10000  &&  Motor_En==1)  speedaboutangle2= 10000;
          if(speedaboutangle2<-10000  &&  Motor_En==1)  speedaboutangle2=-10000;

        ///////////////������///////////////////////
          if(speedaboutangle>0  &&  Motor_En==1)                       //2��5��you���
          {
            ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM,0);
            ftm_pwm_duty(MOTOR_FTM, MOTOR5_PWM,speedaboutangle);
          }
          else if(speedaboutangle<=0  &&  Motor_En==1)
          {
            ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM,-speedaboutangle);
            ftm_pwm_duty(MOTOR_FTM, MOTOR5_PWM,0);
          }
         if(speedaboutangle2>0  &&  Motor_En==1)                        //3��6��zuo���
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

    leftval   =-ftm_quad_get(FTM2);      //��ȡ����
    rightval  = ftm_quad_get(FTM1);

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

       if((L_wrz_flag==1||R_wrz_flag==1||L_wrz_flag==2||R_wrz_flag==2) && xia_no==0 && Ramp_flag==1 ) //��ȫ������֮��ſ�ʼ�ǲ�ͣ��
        wrz_distance+=number;

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
  uint16 control_dis=70;
   if((int)s_distance<150&&(int)s_distance>5)//����С��3m,����0.05m��Ч
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

        else if(s_distance==(control_dis))          //�ٶ�ϵ����ӽ�1��ʱ�򣬾��������Ƶľ��롣
          speedwant =(int) (speedwantB );

        else if(s_distance<(control_dis+5))
          speedwant =(int) (speedwantB * 1.1);
        else if(s_distance<(control_dis+15))
          speedwant =(int) (speedwantB * 1.15);
        else if(s_distance<(control_dis+30))
          speedwant =(int) (speedwantB * 1.2);
        else
          speedwant =(int) (speedwantB * 1.22);

        if(speedwant>(int)(speedwantB*1.30))//�����ٶ�ʧ��
          speedwant=(int)(speedwantB*1.30);
    }
    else
      speedwant =speedwantE;//(int)(speedwantB * 1.2) ;//��������ʧ�Ļ������趨�ٶ�100;//50;//(
//       printf("%d/n",speedwant);      speedwantC
//       printf("%d\n",s_distance);
//   if(fiag_huan) speedwant=0;
   if( Ramp_flag!=0 ) speedwant=80;

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

//-------------------------------------------------
//                 ����
//���� void
//���
//���� ����ʱ��Ҫ���ĵı�������Ҫִ�еĺ���
//����
//����
//˵�� singleΪ 0 ��ʾ˫����������Ϊ 1 ��ʾ��������
//--------------------------------------------------
void car_start(uint8 single)
{
  speedwantD=speedwantD_set;
  speedwantE=speedwantE_set;

  if(single==0)
  {
    uart_putchar(UART3,Start);
//    var2=1;
//    updata_var(VAR2);                //���±�ű�����ֵ���޸ı�����ֵ����Ҫ���ô˺��������±�ű�����ֵ��
//    tongbu[2]=10;
  }
}

//-------------------------------------------------
//                 ͣ��
//���� void
//���
//���� ͣ��ʱ��Ҫ���ĵı�������Ҫִ�еĺ���
//����
//����
//˵�� singleΪ 0 ��ʾ˫����ֹͣ��Ϊ 1 ��ʾ����ֹͣ
//--------------------------------------------------
void car_stop(uint8 single)
{
  speedwantD=0;
  speedwantE=0;

  if(single==0)
  {
    uart_putchar(UART3,Stop);
//    var2=0;
//    updata_var(VAR2);                //���±�ű�����ֵ���޸ı�����ֵ����Ҫ���ô˺��������±�ű�����ֵ��
//    tongbu[2]=10;
  }
}

//-------------------------------------------------
//                 ��������
//���� void
//���
//���� ��������ʱ��Ҫ���ĵı�������Ҫִ�еĺ���
//����
//����
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
//  updata_var(VAR2);                //���±�ű�����ֵ���޸ı�����ֵ����Ҫ���ô˺��������±�ű�����ֵ��
//  tongbu[2]=10;

}
//-------------------------------------------------
//                 ���ڣ����ܷ�����ͣ��
//���� void
//���
//���� �����յ�����ͣ���ź�ʱ��Ҫ���ĵı�������Ҫִ�еĺ���
//--------------------------------------------------
void uart_start_stop( uint8 go)
{
    key_flag_clear=1;
    shua_one=0;

    if(go==1)     //��ʾ����
    {
      key_flag_clear=1;
      shua_one=0;

      Motor_En=1;
      Time_En=1;
      flag_key_select=5;
      flag_key_l_u_0=0;

      car_start(1);
    }
    else          //��ʾͣ��
    {
      flag_key_select=0;
      flag_key_l_u_5=0;

      car_stop(1);
    }

}