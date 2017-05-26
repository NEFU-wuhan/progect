#include "common.h"
#include "include.h"

/*
 （1） 如果使用图像矫正，可以在每次发车之前先检测直道的图像。然后由直道赛道宽度已知，
计算出相应的图像矫正比例，这样就可以避免每次摄像头移动造成的矫正比例发生变化。
（就相当于将MATLAB的计算工作交给单片机来完成）
 （2） 摄像头搜线根据赛道边缘来进行搜线，这样只要在每次搜线之前将基础线找准就OK了，
 （3） 普通补线利用已知的完整线，然后根据边线斜率的规律进行补线
（考虑到边线斜率一般不会发生特别明显的变化，只在十字和直角是会有违反规律的情况出现）
 （4） 十字补线也可以利用上述方法直接补线，不用识别十字。
（想要识别十字也可以利用十字的直角特征进行十字识别，直角同理）


*/

uint8 hehe=0,hehe1=0;
uint8 single_in;

int8 last_left_point[70];
int8 last_right_point[70];
int8 last_mid_point[70];
/*计算斜率中点*/
uint8 X_point[70];
uint8 Y_point[70];

uint8 left_line_first;
uint8 right_line_first;
uint8 left_line_first_flag;
uint8 right_line_first_flag;
#define l_judge_first_line(flag,y) flag==0?(left_line_first=y):(left_line_first=left_line_first)
#define r_judge_first_line(flag,y) flag==0?(right_line_first=y):(right_line_first=right_line_first)

#define Normal_mid_line(x)   ABS(line[x].mid_line_new-64)<=5&&ABS(line[x+1].mid_line_new-64)<=5&&ABS(line[x+2].mid_line_new-64)<=5&&ABS(line[x+3].mid_line_new-64)<=5&&ABS(line[x+4].mid_line_new-64)<=5

uint8  used_length=69;                   //有效行
int16 last_mid_first=64;    //每幅图片都从上一行的

line_info  line[line_num];            //虽然采集了120行,但只用80行 解压之后的图像就是128*80的
uint8  all_left_line[line_num][20];
uint8  all_right_line[line_num][20];

//uint8 Tourner_used_length;//  Tourner（转动）
//uint16 Tourner_correct;   //correct（转动）

//extern int16 v_mid;//中线
//extern float last_mid_line;//上次中线
//环道变量
 uint8 lost_count;
 uint8 left_lost_count;
 uint8 right_lost_count;
 uint8 all_normal_count;
 uint8 all_normal_count_huan;
 uint8 road_wide_count_huan;
 uint8 road_count_chao;
 uint8 xia_no=0;
#if  ( CAR_MODEL==1 )
        uint8 follow_huan=1;    //1 为寻左线，0为寻右线
#elif (CAR_MODEL==2 )
        uint8 follow_huan=1;    //1 为寻左线，0为寻右线
#elif (CAR_MODEL==3 )
        uint8 follow_huan=1;    //1 为寻左线，0为寻右线
#endif


const uint8 distance[70]=
{
  126,126,126,126,126,126,124,121,118,115,
  113,111,109,107,105,103,101,99,97,95,
  93,91,89,87,84,83,82,81,80,79,
  78,76,75,74,73,72,70,69,68,67,
  66,64,63,62,60,60,58,56,56,54,
  52,52,50,48,48,46,44,43,42,40,
  39,37,36,35,34,32,30,29,28,27,
};


////回拐
//uint8 left_line_first;
//uint8 right_line_first;
//#define l_judge_first_line(flag,y) flag==0?(left_line_first=y):(left_line_first=left_line_first)       //如果条件成立，执行：前的语句，否则就执行：后的语句
//#define r_judge_first_line(flag,y) flag==0?(right_line_first=y):(right_line_first=right_line_first)
//#define L_Tourner(x)  line[x-1].line_case_mode==left_lose_right_momal  && line[x-2].line_case_mode==left_lose_right_momal && line[x-3].line_case_mode==left_lose_right_momal && line[x-4].line_case_mode==left_lose_right_momal && line[x-5].line_case_mode==left_lose_right_momal && left_line_first!=x && line[x].line_case_mode!=all_lose
//#define R_Tourner(x)  line[x-1].line_case_mode==left_normal_right_lose && line[x-2].line_case_mode==left_normal_right_lose && line[x-3].line_case_mode==left_normal_right_lose && line[x-4].line_case_mode==left_normal_right_lose && line[x-5].line_case_mode==left_normal_right_lose && right_line_first!=x && line[x].line_case_mode!=all_lose


//第一行检测
//int16 last_mid;
//float last_mid_line;//上次中线
//uint8 lost_count;
//uint8 left_lost_count;
//uint8 right_lost_count;
//uint8 all_normal_count;

//赛道宽度
uint8 distance1[70]=
{
  103   ,102	 ,100   , 100	 ,98  ,  98	, 95   , 95	, 94   ,  92,
  92   , 90	, 90   , 88	, 88   , 86	, 86   , 84	, 84   , 82,
  82  ,  80	, 80 ,   78	, 77  ,  76	, 74   , 74	, 72   , 72,
  70   , 69	, 68  ,  68	 ,66   , 66	, 64   , 64	, 62   , 62,
  60   , 59	, 58  , 57	 ,56   , 55	, 54   , 53	, 52   , 52,
  50   , 49	, 48  ,  47	 ,46   , 46	, 45   , 43     , 42   , 40,
  40   , 38	, 38  ,  37	 ,36   , 35	, 34   , 32	, 32   , 30,
};
//赛道宽度
uint8 distance2[70]=
{
  93   ,92	 ,91   , 90	 ,89  ,  88	, 87   , 86	, 85   ,  84,
  83   , 82	, 81   , 80	, 79   , 78	, 77   , 77	, 75   , 75,
  73  ,  72	, 71 ,   69	, 69  ,  67	, 67   , 65	, 65   , 63,
  63   , 62	, 61  ,  60	 ,58   , 57	, 56   , 55	, 54   , 53,

  60   , 59	, 58  , 57	 ,56   , 55	, 54   , 53	, 52   , 52,
  50   , 49	, 48  ,  47	 ,46   , 46	, 45   , 43     , 42   , 40,
  40   , 38	, 38  ,  37	 ,36   , 35	, 34   , 32	, 32   , 30,
};

 float X_change[80]=
{
2.579, 2.478, 2.478, 2.375, 2.279, 2.279, 2.111, 2.111, 2.035, 1.965,
1.899, 1.838, 1.781, 1.727, 1.676, 1.676, 1.583, 1.583, 1.500, 1.500,
1.424, 1.424, 1.390, 1.357, 1.325, 1.295, 1.266, 1.239, 1.212, 1.187,
1.163, 1.117, 1.117, 1.096, 1.075, 1.055, 1.036, 1.036, 1.000, 1.000,
0.966, 0.966, 0.934, 0.934, 0.904, 0.904, 0.876, 0.876, 0.850, 0.850,
0.826, 0.826, 0.814, 0.791, 0.791, 0.702, 0.702, 0.750, 0.740, 0.730,
0.730, 0.721, 0.712, 0.703, 0.695, 0.686, 0.678, 0.678, 0.662, 0.662,
0.647, 0.640, 0.633, 0.619, 0.619, 0.612, 0.606, 0.606, 0.593, 0.590,
};

//////////////////////////十字
uint8 last_lost_count;
uint8 shi_zi_flag;
uint8 jia_single;
uint16 left_add_point_count,right_add_point_count;
//uint8 last_lost_count;
//uint8 shi_zi_flag;
//uint8 jia_single;
uint8 shi_zi_count;
uint8 never_shi_zi_count=0;


int16 wide[70]={0};
float real_wide[70]={0};
float D_real_wide[70]={0};
int16 D_left[70]={0};
int16 D_right[70]={0};
int16 D_wide[70]={0};
int16 DD_wide[70]={0};                  //微分的微分
int16 DD_left[70]={0};
int16 DD_right[70]={0};
float D_wide_add;
int16 DD_mid_line_add;
float last_mid_line;//上次中线

uint8 *pimg;
uint8 left_first_lose_line_flag;
uint8 left_first_lose;
uint8 right_first_lose_line_flag;
uint8 right_first_lose;


uint16 road_wide_add;
uint8  road_wide_use;
uint16 road_wide_normal;
int16 last_mid;
uint32 shizi_new;     //十字新的算法，统计白的列数均值作为本次图像中值

uint8 fiag_huan_r,fiag_huan_l,fiag_huan;
/*******************************************************************************
* Function Name: median_line_extract();
* Input Param  : src       img数组，160*120
* Output Param : 无
* Description  : 图像处理、中线提取、补线
* Author Data  :  by Li
******************************************************************************/
void median_line_extract(uint8 *src)
 {
    static int16 last_mid_first;
    static int16 temp_dd_mid_line_add=0;
    uint8  *pimg1;
    uint8 i,j;
    uint8 tmpsrc;
    uint8 left_case,right_case;
    int16 left_Sabc[3]={0,0,0};
    int16 right_Sabc[3]={0,0,0};
    last_mid=64;
    last_lost_count=lost_count;
    lost_count=0;//清零丢失次数
    left_lost_count=0;
    right_lost_count=0;
    all_normal_count=0;
    left_add_point_count=0;
    right_add_point_count=0;
    left_line_first_flag=0;
    right_line_first_flag=0;
    single_in=0;              //不会改变为1了
    //环道定义
    road_count_chao=0;
    all_normal_count_huan=0;
    road_wide_count_huan=0;

    uint8 fiag_huan_r=0;
    uint8 fiag_huan_l=0;
    uint8 budiu=0;
    uint8 huan_l_x=0,huan_r_x=0,huan_kuan=0,huan_kuan1_l=0,huan_kuan1_r=0,huan_loca=0;
    uint8 h_jiao=0,h_jiao_x=0,h_jiao6=0,h_jiao_x6=0,h_jiao_cnt=0;
    float slope=0;   //计算斜率
    int16 last_dis_x=0,dis_x=0,dis_x_cnt=0,last_zuo_x=0,last_you_x=128;
//    Search_all_boundary( src );

	/*利用第一行的平均值作为开始找线的起点*/
    static int16 first_line_num,first_line_sum;
    pimg=src+(69*128);
    for(j=0;j<128;j++)
    {
        tmpsrc=*(pimg+j);
        if(tmpsrc==255)
         {
            first_line_num++;
            first_line_sum+=j;
         }
    }
	last_mid_first=first_line_sum/first_line_num;
	first_line_num=0;
	first_line_sum=0;
    last_mid=last_mid_first;
	if(last_mid==0)               //如果第一行全为黑就将中值设为64
		last_mid=64;

    for(i=0;i<line_num;i++)             //大的行循环
     {

        pimg=src+(line_num-1-i)*128;    //倒着处理，从最底下一直扫到最上面
//        static uint16 hang_white_num,hang_white_sum;
//		/*利用每一行白的列数得到的每一行中点*/            //怎么去用?
//		for(j=0;j<128;j++)
//		{
//		    tmpsrc=*(pimg+j);
//            if(tmpsrc==255)
//            {
//				hang_white_sum+=j;
//				hang_white_num++;
//			  }
//			else
//				continue;
//			hang_mid_point[i]=hang_white_sum/hang_white_num;
//			hang_white_sum=0;
//			hang_white_num=0;
//		}

        for(j=last_mid;j>0;j--)                    //扫左线   //每一次都从中间开始
         {
            tmpsrc=*(pimg+j);
            if(tmpsrc==0)
//              if(0==*(pimg+j-1) && 255==*(pimg+j)) //白黑跳变
             {
                line[i].left_line=j;
                line[i].left_line_unrepiar=j;
                line[i].left_line_state=normal;
                left_case=1;
                //到第一个黑点
                l_judge_first_line(left_line_first_flag,i);
                left_line_first_flag=1;
                break;
             }
         }
        //左边黑线没找到
        if(j==0)
         {
            left_case=3;
            line[i].left_line_state=lose;
            if(left_first_lose_line_flag==0)
             {
                left_first_lose=i;
                left_first_lose_line_flag=1;
             }
         }

        for(j=last_mid;j<127;j++)              //扫右线
         {
            tmpsrc=*(pimg+j);
            if(tmpsrc==0)
//            if(255==*(pimg+j) && 0==*(pimg+j+1))//白黑跳变
             {
                line[i].right_line=j;
                line[i].right_line_unrepiar=j;
                line[i].right_line_state=normal;
                right_case=1;
                //到第一个黑点
                r_judge_first_line(right_line_first_flag,i);
                right_line_first_flag=1;
                break;
             }
         }
        //右边黑线没找到
        if(j==127)                                  //一行是从0计数到127
         {
            right_case=0;
            line[i].right_line_state=lose;
            if( right_first_lose_line_flag==0)
             {
                right_first_lose=i;
                right_first_lose_line_flag=1;
             }
         }
        //确定黑线情况
        switch(left_case-right_case)      //这里是对每一行都处理一次
         {
          case 0:             //左右边线都扫到
            line[i].line_case_mode=all_normal;
            all_normal_count++;
            if(i>5) all_normal_count_huan++;
            break;
          case 1:            //扫到左边线，丢失右边线
            line[i].line_case_mode=left_normal_right_lose;
            line[i].right_line=128;
            line[i].right_line_unrepiar=128;
            right_lost_count++;
            break;
          case 2:           //丢失左边线，扫到右边线
            line[i].line_case_mode=left_lose_right_normal;
            line[i].left_line=0;
            line[i].left_line_unrepiar=0;
            left_lost_count++;
            break;
          case 3:          //左右边线均丢失
            line[i].line_case_mode=all_lose;
            line[i].left_line=1;
            line[i].right_line=127;
            line[i].left_line_unrepiar=1;
            line[i].right_line_unrepiar=127;
            lost_count++;
            break;
         }


        /*  赛道宽度赋值  */
        line[i].road_wide=line[i].right_line-line[i].left_line;  //右边线坐标减左边线坐标
        /////////////////////////////////////////////////////////
        for(j=0;j<20;j++)      //在这计算最下面几行的赛道宽度，超车的时候用
            {
              if( line[j].road_wide<50  )
              {
                road_count_chao++;
              }
            }





///////////////////////////   判断环道     /////////////
        uint8 zuo=0,you=0,zuo_x=0,you_x=0,zuo1=0,you1=0,you2_x=0,you2=0,zuo2=0,zuo2_x=0;

// 识别环道
        if( fiag_huan==0 )
        {
          //////////////////// 环道前直道的程序 ////////////////////
          if( i<10 && budiu==0)  //进入环道之前是直道
          {
            if( line[i].road_wide-distance2[i]<=10  && line[i].right_line_unrepiar > 85 && line[i].left_line_unrepiar < 45 )
            {
              all_normal_count_huan++;
              if(all_normal_count_huan>8)
              {
                budiu=1;
              }
            }
          }
          if( budiu && i>6 && i<52 )   //
          {
            //////////////////////判断赛道变宽的程序
            if( i<35 && line[i].road_wide > line[i-2].road_wide
               && line[i].right_line_unrepiar >= line[i-2].right_line_unrepiar
                 && line[i].left_line_unrepiar <= line[i-2].left_line_unrepiar
               )
            {
              road_wide_count_huan++;
              if(road_wide_count_huan>5)
              {
                huan_kuan=1;
              }
            }

            if( huan_kuan==1)     //找拐点  就是类似于一个直角的尖
            {
              for(j=7;j<40;j++)
              {
                if(j<23 && line[j].left_line_unrepiar>3
                   &&  line[j].left_line_unrepiar<line[j-2].left_line_unrepiar
                     && (line[j-4].left_line_unrepiar>=line[j-6].left_line_unrepiar))
                {
                  fiag_huan_l=1;
                  huan_l_x=j;
                }
                if(j<23 && line[j].right_line_unrepiar<125
                   &&  line[j].right_line_unrepiar>line[j-2].right_line_unrepiar
                     && (line[j-4].right_line_unrepiar<=line[j-6].right_line_unrepiar) )
                {
                  fiag_huan_r=1;
                  huan_r_x=j;
                }
                if( fiag_huan_r &&  fiag_huan_l && abs( huan_r_x - huan_l_x )<15 )
                {
                  huan_kuan=2;  //左右两边变宽的尖角离的很近
                  break;
                }
                /////////////环道前只能看到一个右尖角的情况
                if( j>10 && huan_kuan1_r==0 && huan_kuan1_l==0 && line[j].right_line_unrepiar<125
                   &&  line[j].right_line_unrepiar>line[j-2].right_line_unrepiar  && line[j].line_case_mode==left_lose_right_normal
                     &&  line[j-2].right_line_unrepiar>line[j-4].right_line_unrepiar && line[j-4].line_case_mode==left_lose_right_normal
                       &&  line[j-4].right_line_unrepiar>line[j-6].right_line_unrepiar && line[j-6].line_case_mode==left_lose_right_normal
                         && (line[j-8].right_line_unrepiar<=line[j-10].right_line_unrepiar) )
                {
                  huan_kuan1_r=1;
                  huan_loca=j-6;
                }
                /////////////环道前只能看到一个左尖角的情况
                if( j>10 && huan_kuan1_l==0 && huan_kuan1_r==0 && line[j].left_line_unrepiar<125
                   &&  line[j].left_line_unrepiar>line[j-2].left_line_unrepiar  && line[j].line_case_mode==left_normal_right_lose
                     &&  line[j-2].left_line_unrepiar>line[j-4].left_line_unrepiar && line[j-4].line_case_mode==left_normal_right_lose
                       &&  line[j-4].left_line_unrepiar>line[j-6].left_line_unrepiar && line[j-6].line_case_mode==left_normal_right_lose
                         && (line[j-8].left_line_unrepiar<=line[j-10].left_line_unrepiar) )
                {
                  huan_kuan1_l=1;
                  huan_loca=j-6;
                }

              }
            }
            uint8 n=0,zuo_mini=0,you_mini=0;
            //////////////////////  只有一个尖角时环道中心圆环的识别
            if(huan_kuan==1 && (huan_kuan1_r==1 || huan_kuan1_l==1) )
            {
              for(j=huan_loca+5;j<55;j++)
              {

                if( huan_kuan1_r==1 )
                {

                  you2=0;
                  you2_x=0;
                  pimg1=src+(line_num-1-j)*128;

                  tmpsrc=*(pimg1+line[huan_loca].right_line_unrepiar - (j- huan_loca));
                  if( tmpsrc ==0  )  // src+(line_num-1-i)*128;    && (line[huan_loca].left_line_unrepiar - (j- huan_loca))>10
                  {
                    you2=1;
                    you2_x=line[huan_loca].right_line_unrepiar - (j- huan_loca) ;
                  }
                  if( ( you2==1 && you2_x>10)  )
                  {
                    for(n=you2_x-5;n<127;n++)
                    {
                      if(0==*(pimg1+n-1) && 255==*(pimg1+n)  && zuo_mini==0) //黑白跳变
                      {
                        zuo_mini=n;
                      }
                      if(255==*(pimg1+n) && 0==*(pimg1+n+1) && you_mini==0)//白黑跳变
                      {
                        you_mini=n;
                      }
                    }
                  }
                }
                else
                {
                  {

                    zuo2=0;
                    zuo2_x=0;
                    pimg1=src+(line_num-1-j)*128;

                    tmpsrc=*(pimg1+line[huan_loca].left_line_unrepiar + (j- huan_loca));
                    if( tmpsrc ==0 )  // src+(line_num-1-i)*128;
                    {
                      zuo2=1;
                      zuo2_x=line[huan_l_x].left_line_unrepiar + (j- huan_l_x) ;
                    }

                    if( ( zuo2==1 && zuo2_x<118)  )
                    {
                      for(n=zuo2_x+5;n>0;n--)
                      {
                        if(0==*(pimg1+n-1) && 255==*(pimg1+n)  && zuo_mini==0) //黑白跳变
                        {
                          zuo_mini=n;
                        }
                        if(255==*(pimg1+n) && 0==*(pimg1+n+1) && you_mini==0)//白黑跳变
                        {
                          you_mini=n;
                        }
                      }
                    }
                  }                }
                if( ( zuo_mini!=0 && you_mini!=0
                     && you_mini>zuo_mini
                       //                       && you_mini-zuo_mini<20
                       )   //zuo2==1 ||
                   //                   && *(pimg1+(uint8)(zuo_x+ you_x)/2 )==0
                   )
                {
                  fiag_huan=1;
                  hehe++;

//                  speedwantD=0;

                  if(hehe>200) hehe=200;
                  //            tongbu[3]=1;
                  if(var3==0  //加这个条件是为了让前车识别超车标志，开始超车（且不让后车再进入）
                     && chao_one==0    //为了前车只发送一次切换标志，防止停车的时候一直符合条件一直切换，超车完成之后清除标志
                       )   //&& sequence==1
                  {
                    cut_1++;
                    if(cut_1>200) cut_1=200;
                    chao_one=1;


                    var3=1;         //并且环道切换标志同步（表示正在切换，切换完成后清零）
                    updata_var(VAR3);
                    tongbu[3]=10;

                    if(var1==1) var1=0;   //在这切换前后车
                    else var1=1;
                    updata_var(VAR1);
                    tongbu[1]=10;


                  }
                  break;
                }
              }


            }
            //////////////////////  环道中心圆环的识别
            if(huan_kuan==2)
            {
              for(j=20;j<55;j++)
              {
                zuo1=0,zuo_x=0,you1=0,you_x=0;
                pimg1=src+(line_num-1-j)*128;

                tmpsrc=*(pimg1+line[huan_l_x].left_line_unrepiar + (j- huan_l_x));

                if( tmpsrc ==0 && j>(huan_l_x+5))  // src+(line_num-1-i)*128;
                {
                  zuo1=1;
                  zuo_x=line[huan_l_x].left_line_unrepiar + (j- huan_l_x) ;
                }
                tmpsrc=*(pimg1+line[huan_r_x].right_line_unrepiar - (j- huan_r_x));

                if( tmpsrc ==0  && j>(huan_r_x+5))  // src+(line_num-1-i)*128;
                {
                  you1=1;
                  you_x=line[huan_r_x].right_line_unrepiar - (j- huan_r_x) ;
                }
                if(zuo1==1 && you1==1 && *(pimg1+(uint8)(zuo_x+ you_x)/2 )==0 )
                {
                  fiag_huan=1;
                  hehe++;
//                                      speedwantD=0;

                  if(hehe>200) hehe=200;
                  //            tongbu[3]=1;
                  if(var3==0  //加这个条件是为了让前车识别超车标志，开始超车（且不让后车再进入）
                     && chao_one==0    //为了前车只发送一次切换标志，防止停车的时候一直符合条件一直切换，超车完成之后清除标志
                       )   //&& sequence==1
                  {
                    cut_1++;
                    if(cut_1>200) cut_1=200;
                    chao_one=1;

                    var3=1;         //并且环道切换标志同步（表示正在切换，切换完成后清零）
                    updata_var(VAR3);
                    tongbu[3]=10;

                    if(var1==1) var1=0;   //在这切换前后车
                    else var1=1;
                    updata_var(VAR1);
                    tongbu[1]=10;


                  }
                  break;
                }
              }

            }

          }
          ////////////////////////////////////////
        }
      //环岛识别结束






		/***************************十字边线补线*************************/
		static uint8 shi_zi_baochi=0;
		if(last_lost_count>25)
		 {
			if(line[0].line_case_mode==left_lose_right_normal&&line[i].line_case_mode==all_lose)		  //十字从第一行开始丢
			 {
				line[i].right_line=line[0].right_line-1;
			 }
			if(line[0].line_case_mode==left_normal_right_lose&&line[i].line_case_mode==all_lose)		  //十字从第一行开始丢
			 {
				line[i].left_line=line[0].left_line+1;
			 }
			if(line[0].line_case_mode==all_lose&&line[i].road_wide-distance[i]>20)
			 {
				line[i].left_line=64-distance[i]/2;
				line[i].right_line=64+distance[i]/2;
				shi_zi_flag=1;
				if(never_shi_zi_count==0)
				{
                        shi_zi_count++;
					never_shi_zi_count=1;      //不让其继续检测
				}

			 }
		 }
		if(shi_zi_count==1)
		    shi_zi_baochi=0;    //入十字将保持标志位清零
		if(shi_zi_flag==1&&line[0].line_case_mode==all_lose&&line[i].line_case_mode==all_lose)
		 {
			line[i].left_line=64-distance[i]/2;
			line[i].right_line=64+distance[i]/2;
		 }
		if(last_lost_count<5||line[0].line_case_mode==all_normal)
		 {
			shi_zi_flag=0;
			never_shi_zi_count=0;
		 }
		//5.赛道宽度处理
        wide[i]=line[i].right_line-line[i].left_line;     //如果补线了 宽度不是最新的 Dwide 有误
        real_wide[i]=wide[i]*X_change[69-i];              //从最底下处理，矫正表是从上面到下面排列的
        if(i>0)                                           //计算各种变化率
        {
            D_left[i]=line[i].left_line-line[i-1].left_line;      //微分从1开始   那么微分的微分就应该从2开始
            D_right[i]=line[i].right_line-line[i-1].right_line;
            D_wide[i]=wide[i]-wide[i-1];
            D_real_wide[i]=real_wide[i]-real_wide[i-1];
			if(i>1)
			{
				DD_wide[i]=D_wide[i]-D_wide[i-1];
				DD_left[i]=D_left[i]-D_left[i-1];
				DD_right[i]=D_right[i]-D_right[i-1];
				DD_mid_line_add+=DD_left[i];               //暂时先累加左线的微分的微分
				temp_dd_mid_line_add+=DD_left[i];
			}
        }
        if(i>5)
        {
            left_Sabc[0] =(line[i-1].left_line - line[i-2].left_line)*2-(line[i].left_line - line[i-2].left_line);
            left_Sabc[1] =(line[i-2].left_line - line[i-3].left_line)*3-(line[i].left_line - line[i-3].left_line);
            left_Sabc[2] =(line[i-2].left_line - line[i-3].left_line)*2-(line[i-1].left_line - line[i-3].left_line);

            right_Sabc[0] =(line[i-1].right_line- line[i-2].right_line)*2-(line[i].right_line- line[i-2].right_line);
            right_Sabc[1] =(line[i-2].right_line- line[i-3].right_line)*3-(line[i].right_line- line[i-3].right_line);
            right_Sabc[2] =(line[i-2].right_line- line[i-3].right_line)*2-(line[i-1].right_line- line[i-3].right_line);
		    //7.补线 拐点 赛道宽度变化比较大 十字
		    D_wide_add=D_real_wide[i]+D_real_wide[i-1];
//			if(D_wide[i]>=0 &&D_wide_add>2.2/*&& left_point[i-1].state!=LOSE && right_point[i-1].state!=LOSE*/)
//			{
//				if(/*right_Sabc[0]>-3&& right_Sabc[0]<=0 */(left_Sabc[0]>=1||right_Sabc[0]==0)&&(D_left[i-1]+D_left[i-2]+D_left[i-3])>=0)
//				{
//					if(i<65)
//						line[i].left_line=line[i-3].left_line *(float)(X_change[69-i-3]/X_change[69-i]);
//					else
//						line[i].left_line=line[i-1].left_line;
//				}
//				if(/*left_Sabc[0]<3 &&left_Sabc[0]>=0 */(right_Sabc[0]<=-1||left_Sabc[0]==0)&& (D_right[i-1]+D_right[i-2]+D_right[i-3])<=0)
//				{
//					if(i<65)
//						line[i].right_line=127-(127-line[i-3].right_line) *(float)(X_change[69-i-3]/X_change[69-i]);
//					else
//						line[i].right_line=line[i-1].right_line;
//				}
//				wide[i]=line[i].right_line-line[i].left_line;
//				real_wide[i]=wide[i]*X_change[69-i];                     //十字左右丢线会影响坡道计算 需重新计算real、wide
//				D_wide[i]=wide[i]-wide[i-1];
//				D_real_wide[i]=real_wide[i]-real_wide[i-1];              //将所有的微分项全部重新计算，左右线都改变了
//				D_left[i]=line[i].left_line-line[i-1].left_line;
//				D_right[i]=line[i].right_line-line[i-1].right_line;
//			}
        }
         if(i>3&&single_in==0)     //找拐点  就是类似于一个直角的尖
         {
            if(line[i-1].left_line>5
               &&  line[i-2].left_line-line[i-3].left_line>0 && (line[i-1].left_line-line[i-2].left_line==0)
               || (line[i-2].left_line-line[i-3].left_line>0)&&  line[i  ].left_line-line[i-1].left_line<0
                    && (line[i].road_wide-line[i-1].road_wide>0 || line[i].road_wide-distance[i]>12)
               || (line[i-1].left_line-line[i-2].left_line>0)&&  line[i  ].left_line-line[i-1].left_line<0
                    && (line[i].road_wide-line[i-1].road_wide>=0|| line[i].road_wide-distance[i]>12))      //line[i].line_case_mode==left_normal_right_lose
             {
                line[i].left_line=line[i-1].left_line+1;
				left_add_point_count++;
             }
            if(line[i-1].right_line<122
               &&line[i-2].right_line-line[i-3].right_line<0&&(line[i-1].right_line-line[i-2].right_line==0)
               ||(line[i-2].right_line-line[i-3].right_line<0)&&line[i].right_line-line[i-1].right_line>0
                    &&(line[i].road_wide-line[i-1].road_wide>0||line[i].road_wide-distance[i]>12)
               ||(line[i-1].right_line-line[i-2].right_line<0)&&line[i].right_line-line[i-1].right_line>0
                    &&(line[i].road_wide-line[i-1].road_wide>=0||line[i].road_wide-distance[i]>12))//||line[i].line_case_mode==all_lose
             {
                line[i].right_line=line[i-1].right_line-1;
				right_add_point_count++;
             }
         }
		 /*从新计算赛道宽度*/
//         line[i].road_wide=line[i].right_line-line[i].left_line;  //右边线坐标减左边线坐标

        //防止一幅图片出现多条赛道。。
        if(line[i].left_line>line[i].right_line)
         {
            line[i].left_line=line[i].right_line;
         }
        if(line[i-1].left_line>line[i-1].right_line)
         {
            line[i-1].left_line=line[i-1].right_line;
         }
        if(line[i-2].left_line>line[i-2].right_line)
         {
            line[i-2].left_line=line[i-2].right_line;
         }

        //中线补线情况            //找中线算法有待改进，目前是十字拐点检测不好，经常误判，要把条件改变，同时中线算法从头改起
        if(i==0)
         {
            line[i].mid_line_new= (line[i].left_line+ line[i].right_line)/2;
         }
        else
        {
          if(line[i].left_line < 1)              //丢左线 右正常   现在是根据正常的一边的变化趋势在补线  效果不好 可能跟焦距有关
          {
            line[i].mid_line_new=line[i-1].mid_line_new+(line[i].right_line-line[i-1].right_line);
            //			   line[i].mid_line_new=line[i-1].mid_line_new+(line[i].right_line-line[i-1].right_line);   //因为左转转太过，左边界丢线加1
            //			   line[i].mid_line_new=line[i].left_line+line[i].road_wide/2;
          }
          else if(line[i].right_line >127)         //丢右线，左正常    //这个盲目的用赛道宽度补线在十字还是有一些问题，找时间一定解决
              line[i].mid_line_new=line[i-1].mid_line_new+(line[i].left_line-line[i-1].left_line);
          //			      line[i].mid_line_new=line[i].right_line-line[i].road_wide/2;
          else
              line[i].mid_line_new= (line[i].left_line+ line[i].right_line)/2;
        }

        /*利用最底下图像校正上方大片白丢失图像*/
        if(Normal_mid_line(0)&&shi_zi_count==0&&last_lost_count>=10)    //最底下十行图像都在中值附近
        {
            if((line[i].line_case_mode==all_lose))
            {
				line[i].mid_line_new=64;
				shi_zi_baochi=1;
            }
		}
		if(shi_zi_baochi==1&&shi_zi_count==0&&last_lost_count>=10&&(ABS(line[0].mid_line_new-64)<=5||ABS(line[1].mid_line_new-64)<=5))       //shi_zi_count==0 代表入十字  没有其他地方会用到
		{
            if((line[i].line_case_mode==all_lose))
            {
				line[i].mid_line_new=64;
            }
		}
		/*end利用最底下图像校正上方大片白丢失图像*/

        /////////////  清除环岛标志（出环道）  //////////////////////
     if(fiag_huan==1)
     { //先识别出环道出的尖角
       if( follow_huan )     // 1为寻左线,0为寻右线
       {
         if( i==10 && xia_no==0  && line[i].right_line_state== lose && line[i-8].right_line_state== lose
            &&(line[i].left_line_unrepiar < line[i-2].left_line_unrepiar ||  line[i-2].left_line_unrepiar <8 )
               &&(line[i-2].left_line_unrepiar < line[i-4].left_line_unrepiar ||  line[i-4].left_line_unrepiar <8 )
              )
           xia_no=1;
         if( xia_no==1  && i>20 && i<40
            &&  line[i].left_line_state== normal &&(line[i].left_line_unrepiar > line[i-2].left_line_unrepiar  )
              &&  line[i-3].left_line_state== normal &&(line[i-2].left_line_unrepiar > line[i-4].left_line_unrepiar  )
                )   // 先找尖角前半部分
         {
           h_jiao=1;
           h_jiao_x=i;
         }
         if( xia_no==1 &&  h_jiao==1 && (i-h_jiao_x) < 20  && i<50
            && line[i].left_line_state== normal &&(line[i].left_line_unrepiar < line[i-2].left_line_unrepiar  )
              && line[i-3].left_line_state== normal &&(line[i-2].left_line_unrepiar < line[i-4].left_line_unrepiar  )
                && line[i-6].left_line_state== normal &&(line[i-4].left_line_unrepiar < line[i-6].left_line_unrepiar  )
                  )      //再找尖角后半部分
         {
           xia_no=2;       // 都找到则识别为尖角 ，前车停车
           h_jiao_cnt=1;   //每次都只识别一次尖角
           if(chao_one==1  )      //&& sequence==1
           {
//             speedwantD=0;
//             speedwantE=0;
           }
         }

         if( xia_no==2  && h_jiao_cnt==0)
         {
           if(  xia_no==2 && i>5 && i<40
              &&  line[i].left_line_state== normal &&(line[i].left_line_unrepiar > line[i-2].left_line_unrepiar  )
                &&  line[i-3].left_line_state== normal &&(line[i-2].left_line_unrepiar > line[i-4].left_line_unrepiar  )
                  )   // 先找尖角前半部分
           {
             h_jiao=1;
           }
           if( h_jiao==1 && i<50
              && line[i].left_line_state== normal &&(line[i].left_line_unrepiar < line[i-2].left_line_unrepiar  )
                && line[i-3].left_line_state== normal &&(line[i-2].left_line_unrepiar < line[i-4].left_line_unrepiar  )
                  )   // 重新找一遍尖角，找不到则证明已经出尖角，出尖角之后赛道正常则出环道
           {
             h_jiao_cnt=1;
           }
           else
           {
             if(i>47)
               xia_no=4;   //一直到最后都找不到则找不到尖角了
           }
         }
       }
       else
       {
         if( i==10 && xia_no==0  && line[i].left_line_state== lose && line[i-8].left_line_state== lose
            &&(line[i].right_line_unrepiar > line[i-2].right_line_unrepiar ||  line[i-2].right_line_unrepiar >120 )
               &&(line[i-2].right_line_unrepiar > line[i-4].right_line_unrepiar ||  line[i-4].right_line_unrepiar >120 )
              )
           xia_no=1;
         if( xia_no==1  && i>20 && i<40
            &&  line[i].right_line_state== normal &&(line[i].right_line_unrepiar < line[i-2].right_line_unrepiar  )
              &&  line[i-3].right_line_state== normal &&(line[i-2].right_line_unrepiar < line[i-4].right_line_unrepiar  )
                )   // 先找尖角前半部分
         {
           h_jiao=1;
           h_jiao_x=i;
         }
         if( xia_no==1 &&  h_jiao==1 && (i-h_jiao_x) < 20  && i<50
            && line[i].right_line_state== normal &&(line[i].right_line_unrepiar > line[i-2].right_line_unrepiar  )
              && line[i-3].right_line_state== normal &&(line[i-2].right_line_unrepiar > line[i-4].right_line_unrepiar  )
                && line[i-6].right_line_state== normal &&(line[i-4].right_line_unrepiar > line[i-6].right_line_unrepiar  )
                  )      //再找尖角后半部分
         {
           xia_no=2;       // 都找到则识别为尖角 ，前车停车
           h_jiao_cnt=1;   //每次都只识别一次尖角
           if(chao_one==1  )      //&& sequence==1
           {
//             speedwantD=0;
//             speedwantE=0;
           }
         }

         if( xia_no==2  && h_jiao_cnt==0)
         {
           if(  xia_no==2 && i>5 && i<40
              &&  line[i].right_line_state== normal &&(line[i].right_line_unrepiar < line[i-2].right_line_unrepiar  )
                &&  line[i-3].right_line_state== normal &&(line[i-2].right_line_unrepiar < line[i-4].right_line_unrepiar  )
                  )   // 先找尖角前半部分
           {
             h_jiao=1;
           }
           if( h_jiao==1 && i<50
              && line[i].right_line_state== normal &&(line[i].right_line_unrepiar > line[i-2].right_line_unrepiar  )
                && line[i-3].right_line_state== normal &&(line[i-2].right_line_unrepiar > line[i-4].right_line_unrepiar  )
                  )   // 重新找一遍尖角，找不到则证明已经出尖角，出尖角之后赛道正常则出环道
           {
             h_jiao_cnt=1;
           }
           else
           {
             if(i>47)
               xia_no=4;   //一直到最后都找不到则找不到尖角了
           }
         }
       }
       //出环道的尖角之后如果有直道则清除环道标志
       if( i==25 && fiag_huan==1  && xia_no==4)  //
         for(j=7;j<25;j++)
         {
           if( line[j].line_case_mode==all_normal
              &&(line[j].left_line_unrepiar >= line[j-1].left_line_unrepiar  )
                //            &&(line[j].left_line_unrepiar - line[j-1].left_line_unrepiar  )<8
                &&(line[j].right_line_unrepiar <= line[j-1].right_line_unrepiar  )
                  //            &&(line[j-1].right_line_unrepiar - line[j].right_line_unrepiar  )<8
                  )
           {
             all_normal_count_huan++;
             if(all_normal_count_huan>14)
             {
               xia_no = 0;
               all_normal_count_huan=0;
               fiag_huan=0;                                  //&& sequence==0
               if(front_car==1 && var3==1  && chao_one==0 )   //超车完成后的前车出去的时候将环道超车标志清零并同步给后车，让后车出来
               {   //                       防止前车刚停下还未被超车就清除超车标志了，可以把速度变为其他单独的标志
                 var3=0;
                 updata_var(VAR3);
                 tongbu[3]=10;
                 cut_2++;
                 if(cut_2>200) cut_2=200;
               }

               break;
             }

           }
         }
     }
     // 清环道结束
       //////////////////////////////  寻左线行驶  ////////////////////////////
       if( fiag_huan )
       {
         if( follow_huan )    //寻左线
         {
           if(line[i].right_line_unrepiar>line[i].left_line_unrepiar        // 如果能找到正常的右线
              && (line[i].left_line_unrepiar+distance[i]+10) > line[i].right_line_unrepiar )
           {
             if(line[i].left_line_state==normal)   //如果有左线就直接用左右边线
             {
                line[i].left_line  = line[i].left_line_unrepiar;
                line[i].right_line = line[i].right_line_unrepiar;
             }
             else   //否则就根据右线补左线
             {
                line[i].left_line  = line[i].right_line_unrepiar-distance[i];
                line[i].right_line = line[i].right_line_unrepiar;
             }
           }
           else if(line[i].left_line_state==lose) //如果没有正常的右线，也找不到左线就直接给定中线
           {
                line[i].left_line=0;
                line[i].right_line=64;
           }
           else   // 有正常左边线，没有正常右边线的情况，根据左线补右线
           {
                line[i].left_line  = line[i].left_line_unrepiar;
                line[i].right_line = line[i].left_line_unrepiar+distance[i];
           }
         }
         else    //寻右线
         {
           if(line[i].left_line_unrepiar<line[i].right_line_unrepiar        // 如果能找到正常的左线
              && (line[i].left_line_unrepiar+distance[i]+10) > line[i].right_line_unrepiar )
           {
             if(line[i].right_line_state==normal)   //如果有右线就直接用左右边线
             {
                line[i].left_line  = line[i].left_line_unrepiar;
                line[i].right_line = line[i].right_line_unrepiar;
             }
             else   //否则就根据左线补左线
             {
                line[i].left_line  = line[i].left_line_unrepiar;
                line[i].right_line = line[i].left_line_unrepiar+distance[i];
             }
           }
           else if(line[i].right_line_state==lose) //如果没有正常的左线，也找不到右线就直接给定中线
           {
                line[i].left_line=64;
                line[i].right_line=128;
           }
           else   // 有正常右边线，没有正常左边线的情况，根据右线补左线 （最常见）
           {
            line[i].left_line  = line[i].right_line_unrepiar-distance[i];
            line[i].right_line = line[i].right_line_unrepiar;
           }
         }
         line[i].mid_line_new= (line[i].left_line+ line[i].right_line)/2;
       }
       /////////////////////////////////////////////////////////////////////////

                last_mid=line[i].mid_line_new;                      //将每一行的中值都当作下一次左右扫线的中值

		/*保存上一次的左右边线*/                      //每一帧的左右边线值都保存到下一帧 作为一定的参考
        last_left_point[i]=line[i].left_line;
		last_right_point[i]=line[i].right_line;
		last_mid_point[i]=line[i].mid_line_new;

		if(line[i].mid_line_new<0)                //这个限幅值得深究
         {
            line[i].mid_line_new=0;
         }
        if(line[i].mid_line_new>129)
         {
            line[i].mid_line_new=129;
         }

        /*十字走椭圆是因为有部分中线会在右边界的右边*/
//		if(i>50&&(line[i].mid_line_new-line[i-1].right_line>1||line[i].mid_line_new-line[i-2].right_line>1))
//			line[i].mid_line_new=255;
		/*保存中线点，计算斜率*/
		X_point[i]=line[i].mid_line_new;    //这里的i是从零计算起的
		Y_point[i]=i;

        if(last_mid<6)
         {
            last_mid=6;
         }
        if(last_mid>120)
         {
            last_mid=120;
         }
        if(last_mid_first<3)
         {
            last_mid_first=3;
         }
        if(last_mid_first>125)
         {
            last_mid_first=125;
         }
        //对宽度进行累加

        //环道截至有效行
        if( follow_huan )  //1为寻左线，0为寻右线
        {
          if( fiag_huan )
          {
//            if(i>20
//               && line[i].left_line_unrepiar < line[i-1].left_line_unrepiar
//                 &&  line[i-1].left_line_unrepiar < line[i-2].left_line_unrepiar
//                   &&  line[i-7].left_line_unrepiar > line[i-8].left_line_unrepiar
//                     &&  line[i-8].left_line_unrepiar > line[i-9].left_line_unrepiar )
//            {
//              used_length=i;
//              line[used_length].mid_line_new=255;
//              return;
//            }
            if(  i>10
               &&( (abs(line[i].left_line_unrepiar-line[i-1].left_line_unrepiar)>15) || line[i].left_line_unrepiar==128 || line[i].left_line_unrepiar==127 )
                 )
            {                                                                                                                                //|| line[i].left_line_unrepiar== line[i].right_line_unrepiar
              used_length=i;
              line[used_length].mid_line_new=255;
              return;
            }
          }
        }
        else
        {
          if( fiag_huan )
          {
            //环道的时候先判断有没有右边线逐渐减小
            //符合上述条件则继续寻找右边线逐渐增大，
            //此过程中保证右边线正常
//            if( i>20 && i<50
//               && line[i].right_line_state== normal &&(line[i].right_line_unrepiar < line[i-2].right_line_unrepiar  )
//                 && line[i-3].right_line_state== normal &&(line[i-2].right_line_unrepiar < line[i-4].right_line_unrepiar  )
//                   && line[i-6].right_line_state== normal &&(line[i-4].right_line_unrepiar < line[i-6].right_line_unrepiar  )
//                     )
//            {
//              h_jiao6=1;
//              h_jiao_x6=i;
//            }
//            if(  h_jiao6==1  && i<50    //&& (i-h_jiao_x) < 20
//               && line[i].right_line_state== normal &&(line[i].right_line_unrepiar > line[i-2].right_line_unrepiar  )
//                 && line[i-3].right_line_state== normal &&(line[i-2].right_line_unrepiar > line[i-4].right_line_unrepiar  )
//                   && line[i-6].right_line_state== normal &&(line[i-4].right_line_unrepiar > line[i-6].right_line_unrepiar  )
//                     )
//            {
//              used_length=i;
//              line[used_length].mid_line_new=255;
//              return;
//            }
//            if(   line[i].right_line_unrepiar >126
//               && line[i-1].right_line_unrepiar > 126
//                 &&  line[i-2].right_line_unrepiar > 126 )
//            {
//              h_jiao6=0;
//            }

            if(  i>10
               &&( (abs(line[i].right_line_unrepiar-line[i-1].right_line_unrepiar)>15) || line[i].right_line_unrepiar==0 || line[i].right_line_unrepiar==1 )
                 )
            {                                                                                                                             //|| line[i].left_line_unrepiar== line[i].right_line_unrepiar
              used_length=i;
              line[used_length].mid_line_new=255;
              return;
            }
          }
        }

        if(((line[i].right_line-line[i].left_line)<15)&&i>=20)      //计算出有效行
         {                                               //加上这个判断，不然起跑线很有问题 数据会被有效行截断
 //           if(ABS(error)<=25&&i<=50)
 //           {
 //              if(line[i].mid_line_new<30)
//					line[i].mid_line_new=line[0].mid_line_new+15;
//                if(line[i].mid_line_new>98)
//					line[i].mid_line_new=line[0].mid_line_new+-15;
//			}
//			else
//			{
				used_length=i;
				line[used_length].mid_line_new=255; 	 //记好了这里有效行的中值给的255，在其它地方会用到	255是不能在小液晶上显示的
				DD_mid_line_add=temp_dd_mid_line_add;
				temp_dd_mid_line_add=0;
//			}

		/*几乎十字部分每一帧都计算按列处理的图像*/
		if(lost_count>=20)
		{
		    uint32 lie_num=0;
	        uint32 lie_sum_add=0;
			for(j=0;j<127;j++)		   //外层列循环  里层行循环   遇到黑就跳出
			{
				pimg=src+(line_num-1)*128+j;	//最后一行所有数据
				for(i=0;i<line_num;i++)
				{
					if(*pimg==255)
					{
						lie_sum_add+=j;
						lie_num++;
						pimg-=128;
					}
					else
					{
					   break;
					}
				}
			}
			shizi_new=lie_sum_add/lie_num;      //计算了但不一定使用
			lie_sum_add=0;			 //清0  一定清零
			lie_num=0;
		}

		/*十字被有效行截断导致突变的情况*/
        if(used_length<60&&lost_count>=15&&last_lost_count>=15)         //这里是处理的十字的一种特殊情况  只是一部分
        {                                                          //达不到十字的情况，但是快符合了，一般为出十字的一部分情况
			if(shizi_new-64>0)          //人为使其加大一点
				shizi_new+=10;
			if(shizi_new-64<0)
				shizi_new-=10;
			for(i=0;i<line_num-1;i++)
			{
				line[i].mid_line_new=shizi_new; 		   //将得到的中线改变
			}
//				used_length=70;
			line[used_length].mid_line_new=shizi_new;
			line[70].mid_line_new=255;
		}
            return;                                  //跳出本次循环
         }
         used_length=70;
 }
}

//-------------------------------------------------
//                 边界提取
//输入    uint8 *src    整幅图片
//输出    uint8 all_left_line[][] 所有左边界
//        uint8 all_left_line[][] 所有右边界
//功能 提取图像中所有的黑白跳变和白黑跳变
//日期
//作者
//--------------------------------------------------
void Search_all_boundary( uint8 *src )
{
    uint8 i,j=0,k=0;
    for(i=0;i<line_num;i++)             //大的行循环
    {
      pimg=src+(line_num-1-i)*128;    //倒着处理，从最底下一直扫到最上面
      for(j=1;j<127;j++)
      {
        if(0==*(pimg+j-1) && 255==*(pimg+j)) //黑白跳变 （左线）
        {
          all_left_line[i][k]=j;
          k++;
        }
        if(255==*(pimg+j) && 0==*(pimg+j+1)) //白黑跳变 （右线）
        {
          all_right_line[i][k]=j;
          k++;
        }
      }
    }
}

//-------------------------------------------------
//                 赛道边界识别
//输入 uint8 all_left_line[][],all_right_line[][]  跳变
//输出
//功能 在所有的跳变中找到最符合正常赛道的点作为边界
//日期
//作者
//--------------------------------------------------
void SideDdistinguish()
{
//    for(j=0;j<20;j++)
//    {
//        if(all_right_line[i][j]==0)  break;      //没有边界直接返回
//        if( (all_right_line[i][j]>last_mid) )     //右边界肯定得大于上一次的中线
//        {
//          line[i].right_line=all_right_line[i][j];
//          line[i].right_line_unrepiar=all_right_line[i][j];
//          line[i].right_line_state=normal;
//          right_case=1;
//          break;   //找到的第一个边界就是最接近中线的边界（所以直接返回即可）
//        }
//    }

}

