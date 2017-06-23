/******************************************
*                                         *
*            2017.1.20  ZQ修改            *
*                                         *
******************************************/
#include "include.h"
#include "O_measure_distance.h"

uint32 timevar;
uint32 flag;

int s_distance=0;


int s_distance_l=0;
int s_distance_ll=0;
int s_distance_lll=0;

/****************************************************/
int measure_distance()                                   //测距函数，直接调用即可，返回值是s_distance，外部变量，可全局引用
{
  if(gpio_get(ECHG)==1)//高电平，则说明是上升沿触发，开启PIT计数器       A24
  {
    pit_time_start(PIT3);//开启PIT1计数
  }
  else if(gpio_get(ECHG)==0)//低电平，表明下降沿触发，关闭计数器，读取计数值
  {
    timevar = pit_time_get_us(PIT3);//获取时间，单位us
    pit_close (PIT3);  //关闭计数器（此时计数值清零）
  }

  if(((int)(timevar * 340 /2/1000)/5+3 )<500 && ((int)(timevar * 340 /2/1000)/5+3 )>5)//距离小于5m,大于0.05m有效
  {
    s_distance_lll=s_distance_ll;
    s_distance_ll = s_distance_l;
    s_distance_l = s_distance;
    s_distance = (int)(timevar * 340 /2/1000)/5+3;
    s_distance = (int)((s_distance + s_distance_l + s_distance_ll + s_distance_lll)/4);//数据平滑
  }
  else s_distance = 550;                            //信号采集不到的时候，返回550CM

  return s_distance;
}

//-------------------------------------------------
//                 坡道测量
//输入
//输出
//功能 坡道测量
//日期 2016-06-26
//作者
//--------------------------------------------------
int16 distan[10];
int16 distan_acc[10];
int8  distan0=0;         //平均值
int16 distan1=0;      //公式换算值
int32 distan2=0;

int8 Ramp_flag=0;            //坡道标志  ：坡上--1  坡后--2  平路--0
int8 Ramp_flag_pre=0;
int8 Ramp_en=1;     //坡道使能
int8 Ramp_jiansu_start=0,Ramp_after_jiasu=0;  //坡道减速标志
int32 Ramp_Len=0;           //  上坡步数
uint8 Ramp_yushibie=0;
uint8 Ramp_Len_yu=0;

void Ramp_mesure()
{
  int8 i=0,j=0;
  int16 Votage_Gyroscope=0;
  float aa=2.146e+05;
  float bb=-1.266;

  Votage_Gyroscope=0;
  Votage_Gyroscope   = (adc_once(ADC1_DM1, ADC_12bit)+adc_once(ADC1_DM1, ADC_12bit)+adc_once(ADC1_DM1, ADC_12bit))/3;
  //公式换算成距离
  distan1=aa*pow(Votage_Gyroscope,bb);
  if(distan1<5)distan1=5;
  if(distan1>80)distan1=80;
  distan[0]=distan1;
  //存储ADC值
  for(i=10;i>0;i--)
  {
    distan[i]=distan[i-1];
  }
  //累加求平均
  for(i=0;i<10;i++)
  {
    distan2+=distan[i];
  }
  distan0=distan2/10;        //平均值

  //累加平均值
  for(i=10;i>0;i--)
  {
    distan_acc[i]=distan_acc[i-1];
  }
  distan_acc[0]=distan0;

 //增加滤波，防止突然的跳变，j==6表示一直在上升，j==-6表示一直在下降
  for(i=0;i<10;i++)
  {
    if(distan_acc[i]<=50) j++;
    else j--;
  }
//  if( (distan0<distan_value) && (Ramp_flag==0) && Ramp_en && gpio_get(receive_flag)==0 && Ramp_yushibie==0 )      //  &&(j==10)
//  {
//    Ramp_flag=1;
//  }
  if((Ramp_flag!=0)&&(Ramp_flag_pre==0))
  {    //坡道开始
    Ramp_Len=Total_distance;
  }
  if(Ramp_flag!=0)
  {
    if(Ramp_yushibie==1)  Ramp_Len_yu=155;
      else Ramp_Len_yu=115;
    if(((Total_distance-Ramp_Len)>=20)&&((Total_distance-Ramp_Len)<=Ramp_Len_yu) )     // *72.358
    {    //识别上坡阶段，减速
      Ramp_jiansu_start =1;
    }
    else if(((Total_distance-Ramp_Len)>Ramp_Len_yu)&&((Total_distance-Ramp_Len)<=(Ramp_Len_yu+50)))     // *72.358
    {    //识别下坡阶段，加速
      Ramp_jiansu_start =0;
      Ramp_after_jiasu =1;
    }
    else if(((Total_distance-Ramp_Len)>(Ramp_Len_yu+50)) && ((Total_distance-Ramp_Len)<((Ramp_Len_yu+50)+300)))     // *72.358
    {    //坡道后一定距离不再识别坡道
      Ramp_flag=2;
    }
    else if( (Total_distance-Ramp_Len)>((Ramp_Len_yu+30)+300) )
    {
      Ramp_flag=0;
      Ramp_after_jiasu =0;
      Ramp_Len=0;
      Ramp_yushibie=0;
      Ramp_Len_yu=0;
    }
  }
  Ramp_flag_pre=Ramp_flag;
}

