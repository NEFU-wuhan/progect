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

#define  Duoji_Output(x)     ftm_pwm_duty(S3010_FTM,S3010_CH,(int32)x);                 //舵机

int8 mid_line_last[5]={0};   //保存过去的五组中值     可以为负   不能定义为uint8
#define Normal_mid_line(x)   ABS(line[x].mid_line_new-64)<=5&&ABS(line[x+1].mid_line_new-64)<=5&&ABS(line[x+2].mid_line_new-64)<=5&&ABS(line[x+3].mid_line_new-64)<=5&&ABS(line[x+4].mid_line_new-64)<=5


uint16 sqrt_16_kp2;      //计算(line_num-used_length)的1.5次方

//******************舵机***********************//
float mid_line;
int32 SE_duty;                  //舵机的PWM占空比
int32 se_du;
float Ki;                       //舵机PID
float Kp;
float error,error_d,error_pre; //偏差
float index_A,index_B;    //index_A为斜率  index_B为截距   斜率一般更为常用



uint8 quan1[70]={
   1,  1,  1,  1,  1, 1,  2,  2,  2,  2,
   3,  3,  3, 3,  6,  8,  8,  8,  8,  8,
   9,  9,  9, 9,  9,  9,  9,  9,  9,  9,
   9,  9,  9, 9,  9,  8,  8,  8,  8,  8,
   5,  5,  5, 5,  5,  5,  3,  3,  3,  3,
   2,  2,  2, 2,  2,  1,  1,  1,  1,  1,
   0,  0,  0, 0,  0,  0,  0,  0,  0,  0,
};

/*! @brief 最小二乘法
 *  @note  Y=A+BX
 *  @data 2015/3/15
 */
//调用此函数传递参数的数量为有效行
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
/*Function: 开根号处理                  */
/*入口参数：被开方数，长整型            */
/*出口参数：开方结果，整型              */
/****************************************/
unsigned int sqrt_16(unsigned long M)
{
    unsigned int N, i;
    unsigned long tmp, ttp;   // 结果、循环计数
    if (M == 0)               // 被开方数，开方结果也为0
        return 0;
    N = 0;
    tmp = (M >> 30);          // 获取最高位：B[m-1]
    M <<= 2;
    if (tmp > 1)              // 最高位为1
    {
        N ++;                 // 结果当前位为1，否则为默认的0
        tmp -= N;
    }
    for (i=15; i>0; i--)      // 求剩余的15位
    {
        N <<= 1;              // 左移一位
        tmp <<= 2;
        tmp += (M >> 30);     // 假设
        ttp = N;
        ttp = (ttp<<1)+1;
        M <<= 2;
        if (tmp >= ttp)       // 假设成立
        {
            tmp -= ttp;
            N ++;
        }
    }
    return N;
}


/* Function Name: dir_duty_new();
* Description  : 中线加权函数和舵机方向控制
* Author Data  : 2015/4/4 , by Li*/
void quan_dir_duty_new()
 {
    uint8 i;                                        //利用赛道宽度检测坡道
    static int32 num,add;
//	if(left_lost_count>=40&&right_add_point_count>=10)         //防止出十字的时候偏差突然增大  因为图像补线而反向跳变
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

    for(i=0;i<line_num;i++)                  //正常赛道加权有用行以内的
     {
        if((line[i].mid_line_new)==255)      //line[used_length].mid_line_new  就等于255
         {
            break;
         }
//            if(i<used_length)
//             {
            num=(line[i].mid_line_new)*quan1[i]+num;       //加权每一行的mid_line_new坐标
            add=add+quan1[i];
//             }
     }
    /*出十字算法  实验阶段*/
	if((shi_zi_count==1&&never_shi_zi_count==0||shi_zi_count==2)&&lost_count>=8)
	{
	     for(i=0;i<line_num;i++)
	     {
	         if((line[i].mid_line_new)==255)      //line[used_length].mid_line_new  就等于255
	         {
	            break;
	         }
			 if(line[i].line_case_mode!=all_lose)
			 {
		         num=(line[i].mid_line_new)*quan1[i]+num;       //加权每一行的mid_line_new坐标
		         add=add+quan1[i];
			 }
	     }
	}

    if(add>0)
    {
        mid_line=(float)num/(float)add;
    }
	mid_line_last[4]=mid_line_last[3];          //保存五组图像中值
	mid_line_last[3]=mid_line_last[2];
	mid_line_last[2]=mid_line_last[1];
	mid_line_last[1]=mid_line_last[0];
	mid_line_last[0]=mid_line;
    num=0;                         //清零
    add=0;
    if(mid_line>128)               //对加权后中线值进行限幅
     {
        mid_line=128;
     }
    else if(mid_line<0)
     {
        mid_line=0;
     }

	if(shi_zi_count>=2)        //0 代表入十字的前一刻   1代表进十字  2代表出十字             //十字清零
	    shi_zi_count=0;
//	if(lost_count>=10&&add_point_count>=30)     //两边丢线且根据拐点补线到达一定的数量
//	{
//        mid_line=(mid_line_last[0]+mid_line_last[1]+mid_line_last[2]+mid_line_last[3]+mid_line_last[4])/5;
//		mid_line_last[0]=mid_line;
//	}

//	if(lost_count>=20)
//	{
//        if((mid_line_last[0]-mid_line_last[1])*(mid_line_last[1]-mid_line_last[2])<0)     //发生了不符合变化规律的突变
//        {
//            if(ABS(mid_line_last[1]-shizi_new)>10)    //如果突变较大
//				mid_line=shizi_new;
//			else                                      //如果突变较小
//	            mid_line=mid_line_last[1]+(mid_line_last[1]-mid_line_last[2]);
//		  }
//	}
       uint8 set_midline=64;
       static uint8 z=0,h=0;
       if(Total_distance<170   //发车前两米内，一个车靠左一个车靠右
          && var4==0)    // 只有在超车未完成的时候在进入
       {
          used_length=30;
          if(sequence==1)   //前车靠左
          {
            set_midline=85;  //77
            if(Total_distance>30 && Total_distance<35 && z==0 && chao_one2==0)    //前车在指定位置停车
            {
              z++;
              chao_one2=1;
              speedwantD=0;
              speedwantE=0;
            }
          }
          else              //后车靠右
          {
            set_midline=51;
          }
       }
       if(sequence==1)   //停下的车检测到超声波之后恢复正常
       {
         if(road_count_chao>18 && chao_one2==1 && h==0)    //前车停车等待超车完成时若超声波能检测到数据，证明超车完成
         {
           chao_one2=0;
           h++;

           var4=1;         //并且环道切换标志同步（表示正在切换，切换完成后清零）
           updata_var(VAR4);
           tongbu[4]=10;

           speedwantD=speedwantD_set;
           speedwantE=speedwantE_set;
           set_midline=64;
         }
       }
       else              //超过去的车接收到NRF信号之后恢复正常
       {
         if(var4==1)
         {
           set_midline=64;
         }
       }

	error_pre=error;
	error=set_midline-mid_line;    //左转 error>0   右转 error<0
        error_d=error-error_pre;       //这里最终得到了实际是    last_mid_line-mid_line !(之前写反了，怪不得舵机D不起作用!!!!!!!!!!!!!)
//	dealt_error=error_d/(0.035);
 }



/*舵机PD控制函数*/    //可以尝试别的算法 这个算法有它的局限性
#define nWidth  128
#define nHeight 70
void DJ_PID(void)
{
    //可以加前馈补偿PID
//    if(ABS(error)<=30)        //小弯大D值，小打角就好        大弯小D值  打角别太猛
//		Kd=800;
	sqrt_16_kp2=sqrt_16((nHeight-used_length)*(nHeight-used_length)*(nHeight-used_length));
//    Kp=Kp1+(nHeight-used_length)*(nHeight-used_length)*Kp2/100;
    Kp=Kp1+sqrt_16_kp2*Kp2/50.0;

    se_du=(int32)(Kp*error+Kd*error_d);   //如果车在左边，即error小于零，se_du<0 车子该右转
                                          //舵机占空比应该大于中值
//    if(ABS(error)<=10&&used_length>=70)
//		se_du=0;
    last_mid_line=mid_line;               //保存上一次的图像中值

    SE_duty=(uint32)(MID_dir_duty+se_du);   //显示类型转换
    if(SE_duty>MAX_dir_duty)
        SE_duty=MAX_dir_duty;
    else if(SE_duty<MIN_dir_duty)
        SE_duty=MIN_dir_duty;

    Duoji_Output(SE_duty);
}


