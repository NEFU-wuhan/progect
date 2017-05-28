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

//******************���***********************//
float mid_line;
int32 SE_duty;                  //�����PWMռ�ձ�
int32 se_du;
float Ki;                       //���PID
float Kp;
float error,error_d,error_pre; //ƫ��
float index_A,index_B;    //index_AΪб��  index_BΪ�ؾ�   б��һ���Ϊ����



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
//	if(left_lost_count>=40&&right_add_point_count>=10)         //��ֹ��ʮ�ֵ�ʱ��ƫ��ͻȻ����  ��Ϊͼ���߶���������
//    {
//        for(i=40;i<line_num;i++)
//        {
//            if(line[i].mid_line_new==0)
//            {
//				line[i].mid_line_new=255;
//				used_length=i;
//				break;
//            }
//		}
//	}
//	if(right_lost_count>=40&&left_add_point_count>=10)
//    {
//        for(i=40;i<line_num;i++)
//        {
//            if(line[i].mid_line_new==129)
//            {
//				line[i].mid_line_new=255;
//				used_length=i;
//				break;
//            }
//		}
//	}

    for(i=0;i<line_num;i++)                  //����������Ȩ���������ڵ�
     {
        if((line[i].mid_line_new)==255)      //line[used_length].mid_line_new  �͵���255
         {
            break;
         }
//            if(i<used_length)
//             {
            num=(line[i].mid_line_new)*quan1[i]+num;       //��Ȩÿһ�е�mid_line_new����
            add=add+quan1[i];
//             }
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
//	if(lost_count>=10&&add_point_count>=30)     //���߶����Ҹ��ݹյ㲹�ߵ���һ��������
//	{
//        mid_line=(mid_line_last[0]+mid_line_last[1]+mid_line_last[2]+mid_line_last[3]+mid_line_last[4])/5;
//		mid_line_last[0]=mid_line;
//	}

//	if(lost_count>=20)
//	{
//        if((mid_line_last[0]-mid_line_last[1])*(mid_line_last[1]-mid_line_last[2])<0)     //�����˲����ϱ仯���ɵ�ͻ��
//        {
//            if(ABS(mid_line_last[1]-shizi_new)>10)    //���ͻ��ϴ�
//				mid_line=shizi_new;
//			else                                      //���ͻ���С
//	            mid_line=mid_line_last[1]+(mid_line_last[1]-mid_line_last[2]);
//		  }
//	}
       uint8 set_midline=64;
       static uint8 z=0,h=0;
       if(Total_distance<170   //����ǰ�����ڣ�һ��������һ��������
          && var4==0)    // ֻ���ڳ���δ��ɵ�ʱ���ڽ���
       {
          used_length=30;
          if(sequence==1)   //ǰ������
          {
            set_midline=85;  //77
            if(Total_distance>30 && Total_distance<35 && z==0 && chao_one2==0)    //ǰ����ָ��λ��ͣ��
            {
              z++;
              chao_one2=1;
              speedwantD=0;
              speedwantE=0;
            }
          }
          else              //�󳵿���
          {
            set_midline=51;
          }
       }
       if(sequence==1)   //ͣ�µĳ���⵽������֮��ָ�����
       {
         if(road_count_chao>18 && chao_one2==1 && h==0)    //ǰ��ͣ���ȴ��������ʱ���������ܼ�⵽���ݣ�֤���������
         {
           chao_one2=0;
           h++;

           var4=1;         //���һ����л���־ͬ������ʾ�����л����л���ɺ����㣩
           updata_var(VAR4);
           tongbu[4]=10;

           speedwantD=speedwantD_set;
           speedwantE=speedwantE_set;
           set_midline=64;
         }
       }
       else              //����ȥ�ĳ����յ�NRF�ź�֮��ָ�����
       {
         if(var4==1)
         {
           set_midline=64;
         }
       }

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


