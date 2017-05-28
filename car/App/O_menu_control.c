#include "common.h"
#include "include.h"
////////////////////////////////////////////


//////////////////////////////////////////////


uint8 Beep_En;  //������ʹ��
int32 Menu_data_num[20];//���ݴ洢��
uint32 System_time=0;          //���ο���ϵͳʱ��
uint32 Start_timing=0;          //������ʱ

uint8 Time_En=1;               //��ʱʹ�ܱ�ʶλ
int8 Motor_En;      //�綯��ʹ�� 0--disable  1--enable

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
//�������ϵ��
float p1=0.0013;//ԭֵ0.8043     0.004076    0.006576
float p2=0.9252;//ԭֵ1.0220

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
int16 sensitivity_an=0;  //����ɨ��������
uint8 yuzhi_genggai=0;
uint16 speed_ketiao=230;
uint8  flag_shuaping=1;


/*******************************************************************************
*Description    : ��ز�ѹ
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

  if(bat_cnt>400)//һ����һ��
  {
    bat_cnt=0;
    for(uint8 syc=10;syc>0;syc--)
    {
      bat_temp+=adc_once(ADC0_DM1,ADC_12bit);
    }

    dianya=(0.1*bat_temp*14.3 *100/4096);        //14.3   39.78            19.15

    if(dianya<755&&dianya>200)//��ѹ����
    {
      // ��������
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
//                 ϵͳʱ������
//���� ϵͳʱ��
//��� ����
//���� ��ϵͳʱ���������
//����
//����
//--------------------------------------------------

void Time_Commen()
{
   static int i=0;
   if(Time_En)
   {
     i++;
     if(i==1000)
     {

       System_time=System_time+1; //ϵͳʱ���ʱ
       i=0;
     }
   }
    switch(System_time)//��λ�� ms
    {
        case 1000 :
//          Motor_Pro_En=1;
        break; //�����������
//        case 2000 : Motor_En=0;break; //ͣ��
    }
}

   //
//������
//���� ��ٻ��
//ʱ�� 14-6-26
void Beep()
{

  static uint16 time=0;

  time++;
 //  if(((time%200)==0) && flag_stop==1)    gpio_turn (PTE2); //�趨���������ղ�ͬ��Ƶ�ʼ���Ե���     //&&(time<=(i*30))&&(time)
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

