#include "common.h"
#include "include.h"
////////////////////////////////////////////


//////////////////////////////////////////////


uint8 Beep_En;  //蜂鸣器使能
int32 Menu_data_num[20];//数据存储区
uint32 System_time=0;          //单次开机系统时间
uint32 Start_timing=0;          //发车计时

uint8 Time_En=1;               //计时使能标识位
int8 Motor_En;      //电动机使能 0--disable  1--enable

uint8 Cross_flag;
uint8 Trick_flag;

uint8 Servos_En=0;


short int nextError;

int32 speedaboutangle;
int32 speedaboutangle2;
int32 speedaboutangle_zong;
uint8 boma[4];
uint32 zhenshu_count;
uint32 camera_zhenshu;
uint8 display_zs;
//电机差速系数
float p1=0.0013;//原值0.8043     0.004076    0.006576
float p2=0.9252;//原值1.0220

float y;
int stop=0;

long int rightval;
long int leftval;

int16 dianya;
int16 flag_didianya;


uint8 Cross_z_jibu=0,Cross_jici_ru=0;
int16 Init_flag_Len;
uint8 Cross_flag_pre=0,Cross_jici=0;
uint16 Cross_Len3,Cross_Len4;
uint32 Ramp_Len_x=0,Ramp_jici=0;
uint8 suoxian_kaishi=1;
int16 sensitivity_an=0;  //脉冲扫描灵敏度
uint8 yuzhi_genggai=0;
uint16 speed_ketiao=230;
uint8  flag_shuaping=1;


/*******************************************************************************
*Description    : 电池测压
*
*
*DATA           :2016-05-01 12:36:32  @Mjp
*******************************************************************************/
int16 bat_voltage_measure()
{
  static uint16 bat_cnt=0;
  float bat_temp=0;
  bat_cnt++;

  static uint8 baojing_cut;

  if(bat_cnt>400)//一秒检测一次
  {
    bat_cnt=0;
    for(uint8 syc=10;syc>0;syc--)
    {
      bat_temp+=adc_once(ADC0_DM1,ADC_12bit);
    }

    dianya=(0.1*bat_temp*14.3 *100/4096);        //14.3   39.78            19.15

    if(dianya<755&&dianya>200)//低压报警
    {
      // 报警清屏
      baojing_cut++;
    }
    else
    {
      baojing_cut=0;
    }
    if(baojing_cut>25)   baojing_cut=25;
    if(baojing_cut==20)
    {
      flag_didianya=1;
    }
    else
    {
      flag_didianya=0;
    }
  }
}



//-------------------------------------------------
//                 系统时间命令
//输入 系统时间
//输出 命令
//功能 按系统时间给出命令
//日期
//作者
//--------------------------------------------------

void Time_Commen()
{
   static int i=0;
   if(Time_En)
   {
     i++;
     if(i==1000)
     {

       System_time=System_time+1; //系统时间计时
       i=0;
     }
   }
    switch(System_time)//单位是 ms
    {
        case 1000 :
//          Motor_Pro_En=1;
        break; //开启电机保护
//        case 2000 : Motor_En=0;break; //停车
    }
}

   //
//蜂鸣器
//作者 杨倩晴
//时间 14-6-26
void Beep()
{

  static uint16 time=0;

  time++;
 //  if(((time%200)==0) && flag_stop==1)    gpio_turn (PTE2); //设定蜂鸣器按照不同的频率间接性的响     //&&(time<=(i*30))&&(time)
   gpio_set(PTE2,0);

   if( (time%1000)==0 )
   {
   gpio_set  (PTE2,1);
   Beep_En=0;
   }
   if(time>1000)
   {
      time=0;
   }
}

