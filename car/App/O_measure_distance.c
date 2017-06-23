/******************************************
*                                         *
*            2017.1.20  ZQ�޸�            *
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
int measure_distance()                                   //��ຯ����ֱ�ӵ��ü��ɣ�����ֵ��s_distance���ⲿ��������ȫ������
{
  if(gpio_get(ECHG)==1)//�ߵ�ƽ����˵���������ش���������PIT������       A24
  {
    pit_time_start(PIT3);//����PIT1����
  }
  else if(gpio_get(ECHG)==0)//�͵�ƽ�������½��ش������رռ���������ȡ����ֵ
  {
    timevar = pit_time_get_us(PIT3);//��ȡʱ�䣬��λus
    pit_close (PIT3);  //�رռ���������ʱ����ֵ���㣩
  }

  if(((int)(timevar * 340 /2/1000)/5+3 )<500 && ((int)(timevar * 340 /2/1000)/5+3 )>5)//����С��5m,����0.05m��Ч
  {
    s_distance_lll=s_distance_ll;
    s_distance_ll = s_distance_l;
    s_distance_l = s_distance;
    s_distance = (int)(timevar * 340 /2/1000)/5+3;
    s_distance = (int)((s_distance + s_distance_l + s_distance_ll + s_distance_lll)/4);//����ƽ��
  }
  else s_distance = 550;                            //�źŲɼ�������ʱ�򣬷���550CM

  return s_distance;
}

//-------------------------------------------------
//                 �µ�����
//����
//���
//���� �µ�����
//���� 2016-06-26
//����
//--------------------------------------------------
int16 distan[10];
int16 distan_acc[10];
int8  distan0=0;         //ƽ��ֵ
int16 distan1=0;      //��ʽ����ֵ
int32 distan2=0;

int8 Ramp_flag=0;            //�µ���־  ������--1  �º�--2  ƽ·--0
int8 Ramp_flag_pre=0;
int8 Ramp_en=1;     //�µ�ʹ��
int8 Ramp_jiansu_start=0,Ramp_after_jiasu=0;  //�µ����ٱ�־
int32 Ramp_Len=0;           //  ���²���
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
  //��ʽ����ɾ���
  distan1=aa*pow(Votage_Gyroscope,bb);
  if(distan1<5)distan1=5;
  if(distan1>80)distan1=80;
  distan[0]=distan1;
  //�洢ADCֵ
  for(i=10;i>0;i--)
  {
    distan[i]=distan[i-1];
  }
  //�ۼ���ƽ��
  for(i=0;i<10;i++)
  {
    distan2+=distan[i];
  }
  distan0=distan2/10;        //ƽ��ֵ

  //�ۼ�ƽ��ֵ
  for(i=10;i>0;i--)
  {
    distan_acc[i]=distan_acc[i-1];
  }
  distan_acc[0]=distan0;

 //�����˲�����ֹͻȻ�����䣬j==6��ʾһֱ��������j==-6��ʾһֱ���½�
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
  {    //�µ���ʼ
    Ramp_Len=Total_distance;
  }
  if(Ramp_flag!=0)
  {
    if(Ramp_yushibie==1)  Ramp_Len_yu=155;
      else Ramp_Len_yu=115;
    if(((Total_distance-Ramp_Len)>=20)&&((Total_distance-Ramp_Len)<=Ramp_Len_yu) )     // *72.358
    {    //ʶ�����½׶Σ�����
      Ramp_jiansu_start =1;
    }
    else if(((Total_distance-Ramp_Len)>Ramp_Len_yu)&&((Total_distance-Ramp_Len)<=(Ramp_Len_yu+50)))     // *72.358
    {    //ʶ�����½׶Σ�����
      Ramp_jiansu_start =0;
      Ramp_after_jiasu =1;
    }
    else if(((Total_distance-Ramp_Len)>(Ramp_Len_yu+50)) && ((Total_distance-Ramp_Len)<((Ramp_Len_yu+50)+300)))     // *72.358
    {    //�µ���һ�����벻��ʶ���µ�
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

