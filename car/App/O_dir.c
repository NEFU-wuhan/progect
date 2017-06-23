#include "common.h"
#include "include.h"

        int Mid_duty;
        float Kp1;
        float Kp2;
        float Kd;
        int MID_dir_duty;
        int MAX_dir_duty;
        int MIN_dir_duty;


extern int16 speed_current;

#define  Duoji_Output(x)     ftm_pwm_duty(S3010_FTM,S3010_CH,(int32)x);                 //���

int8 mid_line_last[5]={0};   //�����ȥ��������ֵ     ����Ϊ��   ���ܶ���Ϊuint8
#define Normal_mid_line(x)   ABS(line[x].mid_line_new-64)<=5&&ABS(line[x+1].mid_line_new-64)<=5&&ABS(line[x+2].mid_line_new-64)<=5&&ABS(line[x+3].mid_line_new-64)<=5&&ABS(line[x+4].mid_line_new-64)<=5


uint16 sqrt_16_kp2;      //����(line_num-used_length)��1.5�η�

uint32 obstancle_time_pluse=0;
uint8 obstancle_acc=0;      //�ϰ��ǲ�
//******************���***********************//
float mid_line;
int32 SE_duty;                  //�����PWMռ�ձ�
int32 se_du;
float Ki;                       //���PID
float Kp;
float error,error_d,error_pre; //ƫ��
float index_A,index_B;    //index_AΪб��  index_BΪ�ؾ�   б��һ���Ϊ����

uint8 set_midline=64;

uint8 quan1[70]={
   1,  1,  1,  1,  1, 1,  2,  2,  2,  2,
   3,  3,  3, 3,  6,  8,  8,  8,  8,  8,
   9,  9,  9, 9,  9,  9,  9,  9,  9,  9,
   9,  9,  9, 9,  9,  8,  8,  8,  8,  8,
   5,  5,  5, 5,  5,  5,  3,  3,  3,  3,
   2,  2,  2, 2,  2,  1,  1,  1,  1,  1,
   0,  0,  0, 0,  0,  0,  0,  0,  0,  0,
};

/*! @brief ��С���˷�
 *  @note  Y=A+BX
 *  @data 2015/3/15
 */
//���ô˺������ݲ���������Ϊ��Ч��
void Regression_cal(uint8 *X_buff,uint8 *Y_buff,uint8 AvaliableLines)
{
    int16 cnt_R;
    int16 SumX,SumY;
    float AverageX,AverageY,SumUp,SumDown;
    SumX=0;
    SumY=0;
    for(cnt_R=0;cnt_R<AvaliableLines;cnt_R++)
    {
        SumX+=X_buff[cnt_R];
        SumY+=Y_buff[cnt_R];
    }
    AverageX=(float)SumX/(float)AvaliableLines;
    AverageY=(float)SumY/(float)AvaliableLines;
    SumUp=0;
    SumDown=0;
    for(cnt_R=0;cnt_R<AvaliableLines;cnt_R++)
    {
        SumUp+=(Y_buff[cnt_R]-AverageY)*(X_buff[cnt_R]-AverageX);
        SumDown+=(X_buff[cnt_R]-AverageX)*(X_buff[cnt_R]-AverageX);
    }
    if(SumDown==0) index_B=0;
    else index_B=(SumUp/SumDown);
    index_A=(SumY-index_B*SumX)/AvaliableLines;
}

/****************************************/
/*Function: �����Ŵ���                  */
/*��ڲ���������������������            */
/*���ڲ������������������              */
/****************************************/
unsigned int sqrt_16(unsigned long M)
{
    unsigned int N, i;
    unsigned long tmp, ttp;   // �����ѭ������
    if (M == 0)               // �����������������ҲΪ0
        return 0;
    N = 0;
    tmp = (M >> 30);          // ��ȡ���λ��B[m-1]
    M <<= 2;
    if (tmp > 1)              // ���λΪ1
    {
        N ++;                 // �����ǰλΪ1������ΪĬ�ϵ�0
        tmp -= N;
    }
    for (i=15; i>0; i--)      // ��ʣ���15λ
    {
        N <<= 1;              // ����һλ
        tmp <<= 2;
        tmp += (M >> 30);     // ����
        ttp = N;
        ttp = (ttp<<1)+1;
        M <<= 2;
        if (tmp >= ttp)       // �������
        {
            tmp -= ttp;
            N ++;
        }
    }
    return N;
}


/* Function Name: dir_duty_new();
* Description  : ���߼�Ȩ�����Ͷ���������
* Author Data  : 2015/4/4 , by Li*/
void quan_dir_duty_new()
 {
    uint8 i;                                        //����������ȼ���µ�
    static int32 num,add;

/////////////////////////////////////////////////////////////////////
         /*�ϰ�����*/        //һ��ʱ��ļǲ�  ͼ��ƫ��
    if(left_obstancle_flag==1&&never_obstacle_flag==1&&Ramp_flag==0)
    {

      if(obstancle_time_pluse<=2200)
      {

        for(i=0;i<used_length;i++)
        {
          line[i].mid_line_new=line[i].right_line-17;
        }
        obstancle_time_pluse+=Car_Speed_ave;
      }
       else      //�ǲ�����һ������
       {
        obstancle_time_pluse=0;
        left_obstancle_flag=0;
        never_obstacle_flag=0;
        obstancle_acc=0;
       }
    }
    else if(right_obstancle_flag==1&&never_obstacle_flag==1&&Ramp_flag==0)
    {


       if(obstancle_time_pluse<=2200)
       {
        for(i=0;i<used_length;i++)
        {
          line[i].mid_line_new=line[i].left_line+17;
        }
        obstancle_time_pluse+=Car_Speed_ave;
       }
       else
       {
          obstancle_time_pluse=0;
          right_obstancle_flag=0;
          never_obstacle_flag=0;
          obstancle_acc=0;
       }
    }
    else
    {

    }
  ////////////////////////////////////////////////////////////

    if(origin_chao_cont==1)
    {
      if(Motor_En==0)
      {
        if(front_car==1)   //ǰ������
          L_wrz_flag=2;
        else R_wrz_flag=2;
      }
      if( L_wrz_flag==2 || R_wrz_flag==2 )
        set_midline=origin_chao( mid_line );
    }
    else
    {
      if(Motor_En==0)
      {
        L_wrz_flag=0;
        R_wrz_flag=0;
      }
    }
    //�����ֱ������
    if(wrz_chao_cont==1 && ( L_wrz_flag==1 || R_wrz_flag==1 ))     //&& Total_distance>250
    {
      set_midline=wrz_chao( 1 );
    }

    for(i=0;i<line_num;i++)                  //����������Ȩ���������ڵ�
     {
        if((line[i].mid_line_new)==255)      //line[used_length].mid_line_new  �͵���255
         {
            break;                                                //&& Ramp_flag==0
         }
        if(line[i].line_case_mode != starline)
        {
          if( ( (L_wrz_flag==2&&origin_chao_cont==1) || (L_wrz_flag==1&& wrz_chao_cont==1) ) && xia_no==0  )   //��ȫ������֮��ſ�ʼѰ���߻�Ѱ����
            num=(line[i].left_line+16)*quan1[i]+num;       //��Ȩÿһ�е�zuo����
          else if( ( (R_wrz_flag==2&&origin_chao_cont==1) || (R_wrz_flag==1&&wrz_chao_cont==1) ) && xia_no==0 )   //&& Ramp_flag==0
            num=(line[i].right_line-16)*quan1[i]+num;       //��Ȩÿһ�е�you����
          else
            num=(line[i].mid_line_new)*quan1[i]+num;       //��Ȩÿһ�е�mid_line_new����
          add=add+quan1[i];
        }
     }
    /*��ʮ���㷨  ʵ��׶�*/
    if((shi_zi_count==1&&never_shi_zi_count==0||shi_zi_count==2)&&lost_count>=8)
    {
      for(i=0;i<line_num;i++)
      {
        if((line[i].mid_line_new)==255)      //line[used_length].mid_line_new  �͵���255
        {
          break;
        }
        if(line[i].line_case_mode!=all_lose)
        {
          num=(line[i].mid_line_new)*quan1[i]+num;       //��Ȩÿһ�е�mid_line_new����
          add=add+quan1[i];
        }
      }
    }

    if(add>0)
    {
        mid_line=(float)num/(float)add;
    }
	mid_line_last[4]=mid_line_last[3];          //��������ͼ����ֵ
	mid_line_last[3]=mid_line_last[2];
	mid_line_last[2]=mid_line_last[1];
	mid_line_last[1]=mid_line_last[0];
	mid_line_last[0]=mid_line;
    num=0;                         //����
    add=0;
    if(mid_line>128)               //�Լ�Ȩ������ֵ�����޷�
     {
        mid_line=128;
     }
    else if(mid_line<0)
     {
        mid_line=0;
     }

    if(shi_zi_count>=2)        //0 ������ʮ�ֵ�ǰһ��   1�����ʮ��  2�����ʮ��             //ʮ������
        shi_zi_count=0;

	error_pre=error;
	error=set_midline-mid_line;    //��ת error>0   ��ת error<0
        error_d=error-error_pre;       //�������յõ���ʵ����    last_mid_line-mid_line !(֮ǰд���ˣ��ֲ��ö��D��������!!!!!!!!!!!!!)
//	dealt_error=error_d/(0.035);
 }

/*���PD���ƺ���*/    //���Գ��Ա���㷨 ����㷨�����ľ�����
#define nWidth  128
#define nHeight 70
void DJ_PID(void)
{
    //���Լ�ǰ������PID
//    if(ABS(error)<=30)        //С���Dֵ��С��Ǿͺ�        ����СDֵ  ��Ǳ�̫��
//		Kd=800;
    sqrt_16_kp2=sqrt_16((nHeight-used_length)*(nHeight-used_length)*(nHeight-used_length));
//    Kp=Kp1+(nHeight-used_length)*(nHeight-used_length)*Kp2/100;
    Kp=Kp1+sqrt_16_kp2*Kp2/50.0;

    se_du=(int32)(Kp*error+Kd*error_d);   //���������ߣ���errorС���㣬se_du<0 ���Ӹ���ת
                                          //���ռ�ձ�Ӧ�ô�����ֵ
//    if(ABS(error)<=10&&used_length>=70)
//		se_du=0;
    last_mid_line=mid_line;               //������һ�ε�ͼ����ֵ

    SE_duty=(uint32)(MID_dir_duty+se_du);   //��ʾ����ת��
    if(SE_duty>MAX_dir_duty)
        SE_duty=MAX_dir_duty;
    else if(SE_duty<MIN_dir_duty)
        SE_duty=MIN_dir_duty;

    Duoji_Output(SE_duty);
}


