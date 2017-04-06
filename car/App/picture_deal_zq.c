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
uint8 hehe=0;
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

//uint8 Tourner_used_length;//  Tourner（转动）
//uint16 Tourner_correct;   //correct（转动）

//extern int16 v_mid;//中线
//extern float last_mid_line;//上次中线
 uint8 lost_count;
 uint8 left_lost_count;
 uint8 right_lost_count;
 uint8 all_normal_count;
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
    uint8  *pimg;
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

    uint8 fiag_huan_r=0;
    uint8 fiag_huan_l=0;
    uint8 budiu=0;

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
//            if( *(pimg+j)==0  &&  *(pimg+j+1)==255 )    //左边界（跳变）//这两个通过找跳变边界的函数替换为原来的找黑点方式
             {
                line[i].left_line=j;
                line[i].left_line_unrepiar=j;
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
//            if( *(pimg+j-1)==255 && *(pimg+j)==0)       //右边界（跳变）
             {
                line[i].right_line=j;
                line[i].right_line_unrepiar=j;
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

///////////////////////////   判断环道     /////////////
        uint8 zuo=0,you=0;

// 判断环道
     if( i==40 && all_normal_count>=33)  budiu=1;
     if( i>=40 && fiag_huan==0 && budiu)
     {
          if ( line[i].road_wide -line[i-2].road_wide>3
//              && line[i].line_case_mode==all_normal
              && fiag_huan_r==0 )
            fiag_huan_l++ ;
          else  fiag_huan_l=0;
///////   if 赛道变宽连续超过 8 次 ，做标志
          if( fiag_huan_l>8 )
          {
            fiag_huan_l=0;
            fiag_huan_r=1;
          }
//////   if标志为1   则判断中点是否为黑色
          if( fiag_huan_r
             && *(pimg+64 )==0
               )
          {
//////   若为黑色则从黑点向左右找跳变
             for(j=64;j<118;j++)
             {
               if( *(pimg+j-1)==0 && *(pimg+j)==255 )    //有边界（跳变）
               {
                 zuo=1;
                 break;
               }
             }
             for(j=64;j>10;j--)
             {
               if( *(pimg+j+1)==0 && *(pimg+j)==255 )    //有边界（跳变）
               {
                 you=1;
                 break;
               }
             }
          }

     }

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
         if(i>=3&&single_in==0)     //找拐点  就是类似于一个直角的尖
         {
            if(line[i-1].left_line>5&&line[i-2].left_line-line[i-3].left_line>0&&(line[i-1].left_line-line[i-2].left_line==0)||(line[i-2].left_line-line[i-3].left_line>0)&&line[i].left_line-line[i-1].left_line<0&&(line[i].road_wide-line[i-1].road_wide>0||line[i].road_wide-distance[i]>12)||(line[i-1].left_line-line[i-2].left_line>0)&&line[i].left_line-line[i-1].left_line<0&&(line[i].road_wide-line[i-1].road_wide>=0||line[i].road_wide-distance[i]>12))      //line[i].line_case_mode==left_normal_right_lose
             {
                line[i].left_line=line[i-1].left_line+1;
				left_add_point_count++;
             }
            if(line[i-1].right_line<122&&line[i-2].right_line-line[i-3].right_line<0&&(line[i-1].right_line-line[i-2].right_line==0)||(line[i-2].right_line-line[i-3].right_line<0)&&line[i].right_line-line[i-1].right_line>0&&(line[i].road_wide-line[i-1].road_wide>0||line[i].road_wide-distance[i]>12)||(line[i-1].right_line-line[i-2].right_line<0)&&line[i].right_line-line[i-1].right_line>0&&(line[i].road_wide-line[i-1].road_wide>=0||line[i].road_wide-distance[i]>12))//||line[i].line_case_mode==all_lose
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
			else
               if(line[i].right_line >127)         //丢右线，左正常    //这个盲目的用赛道宽度补线在十字还是有一些问题，找时间一定解决
                  line[i].mid_line_new=line[i-1].mid_line_new+(line[i].left_line-line[i-1].left_line);
//			      line[i].mid_line_new=line[i].right_line-line[i].road_wide/2;
               else
                  line[i].mid_line_new= (line[i].left_line+ line[i].right_line)/2;
         }
        last_mid=line[i].mid_line_new;                      //将每一行的中值都当作下一次左右扫线的中值

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

     uint8 xia_nom=0;
       if( zuo && you)
       {
         fiag_huan=1;
//         hehe++;
//         if(hehe>200) hehe=200;
       }
       if(  i==15  && all_normal_count > 13 )
         xia_nom=1;
       if(   xia_nom  &&  i <40
          && line[i].line_case_mode==all_normal
          && (line[i].left_line_unrepiar==line[i-5].left_line_unrepiar  ) && line[i-5].line_case_mode==all_normal
          && (line[i-5].left_line_unrepiar==line[i-10].left_line_unrepiar  )  && line[i-10].line_case_mode==all_normal
          && (line[i-10].left_line_unrepiar==line[i-15].left_line_unrepiar  )  && line[i-15].line_case_mode==all_normal
//            &&  line[i].left_line_unrepiar - line[i-5].left_line_unrepiar <8
            )
       {
          fiag_huan=0;
       }

       //////////////////////////////  寻左线行驶  ////////////////////////////
       if( fiag_huan )
       {
         line[i].left_line  = line[i].left_line_unrepiar;
         line[i].right_line = line[i].left_line_unrepiar+distance[i];
       }
       /////////////////////////////////////////////////////////////////////////


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

        if(i>10 && fiag_huan && (abs(line[i].left_line_unrepiar-line[i-1].left_line_unrepiar)>15) )
         {
           used_length=i;
           line[used_length].mid_line_new=255;
           return;
         }

        if(((line[i].right_line-line[i].left_line)<8)&&i>=30)      //计算出有效行
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
//检测赛道类型



/*******************************************************************************
* Function Name: median_line_extract();
* Input Param  : src       img数组，160*120
* Output Param : 无
* Description  : 图像处理、中线提取、补线
* Author Data  : 2015/6/2 星期四, by Li
******************************************************************************/
//void median_line_extract(uint8 *src)
//{
//
//  uint8 i,j;
//  //    uint8 tmpsrc;
//  //    uint8 left_case,right_case;     //用于判断左线和右线的情况
//  uint8  length_flag=0;
//  //    last_lost_count=lost_count;
//  //    lost_count=0;//清零都丢失次数
//  //   printf("\n 开始---------------------");
//
//  for(i=0;i<nHeight;i++)                          //nHeight=80高度
//  {
//    pimg=src+(nHeight-1-i)*nWidth;                //nWidth=128宽度
//    //--1--先确定第一行的左右边界
//    if(i==0)
//    {
//      //(1)扫左线，查找黑白跳变
//      for(j=last_mid_first ; j>=0 ; j--)
//      {
//        if( (*(pimg+j)-*(pimg+j-1))!=0 && *(pimg+j-1)==0  && j!=0)    //0是黑色，255是白色
//        {  //如果当前点与前一个点颜色不一样，且当前点是黑色，且有边界   则证明是左边界，保存
//          line[i].left_line=j;             //记录左边界坐标     line_case_mode
//          line[i].left_line_state=normal;
//          break;
//        }
//        else if(j==0)                       //丢线
//        {
//          line[i].left_line=0;
//          line[i].left_line_state=lose;
//          break;
//        }
//      }
//      //(2).扫右线，查找白黑跳变
//      for(j=last_mid_first; j<=nWidth ; j++)         //记录right_point    first_right_point   first_right_lose
//      {
//        if( (*(pimg+j)-*(pimg+j-1))!=0 && *(pimg+j)==0  && j!=(nWidth-1) )    //0是黑色，255是白色
//        {
//          line[i].right_line=j;             //记录左边界坐标     line_case_mode
//          line[i].right_line_state=normal;
//          break;
//        }
//        else if(j==(nWidth))                  //表示first_right_lose还没有改变是第一次丢失
//        {
//          line[i].right_line=(nWidth);
//          line[i].right_line_state=lose;
//        }
//      }
//    }
//    //--2--再根据第一行的左右边界进行后面所有行的搜线
//    else
//    {
//      //(1)扫左线，查找黑白跳变
//      for(j=(line[i-1].left_line+3 ) ; j>=0 ; j--)
//      {         //x+3表示每隔3个点查询一次
//        if( (*(pimg+j)-*(pimg+j-1))!=0 && *(pimg+j-1)==0  && j!=0)    //0是黑色，255是白色
//        {
//          line[i].left_line=j;             //记录左边界坐标     line_case_mode
//          line[i].left_line_state=normal;
//          break;
//        }
//        else if(j==0)                       //丢线
//        {
//          line[i].left_line=0;
//          line[i].left_line_state=lose;
//          break;
//        }
//      }
//      //(2).扫右线，查找白黑跳变
//      for(j=line[i-1].right_line-3; j<=nWidth ;j++)         //记录right_point    first_right_point   first_right_lose
//      {
//        if( (*(pimg+j)-*(pimg+j-1))!=0 && *(pimg+j)==0  && j!=(nWidth-1) )
//        {
//          line[i].right_line=j;             //记录左边界坐标     line_case_mode
//          line[i].right_line_state=normal;
//          break;
//        }
//        else if(j==(nWidth))                  //表示first_right_lose还没有改变是第一次丢失
//        {
//          line[i].right_line=(nWidth);
//          line[i].right_line_state=lose;
//        }
//      }
//    }
//
//    //--3--常规补线和十字补线 (需要先进行十字判定，十字情况比较复杂，需要将十字补线和普通补线区分开)
//    if(i>3)
//    {
//      //(1)只要丢线就先进行十字判定 (十字打算用尖叫判定，但是还不知道该怎么写，先不写了，只写普通补线)
//      if( line[i].right_line_state==lose  &&  line[i].left_line_state==lose )
//      {
//
//      }
//      else if( line[i].right_line_state==lose )
//      {
//
//      }
//      else
//      {
//
//      }
//      //(2)普通情况的补线
//      if( line[i].right_line_state==lose  &&  line[i].left_line_state==lose )
//      {
//
//      }
//      else if( line[i].right_line_state==lose )
//      {
//        //////////////////////   补线start   //////////////////////////////
//        if(i<75)
//          // line[i].right_line=(nWidth-1)-((nWidth-1)-line[i-3].right_line) *(float)(X_change[79-i-3]/X_change[79-i]);    // 用右边线减去前一条线的宽度
//          line[i].right_line=line[i].left_line+distance[i];
//        else
//          line[i].right_line=line[i-1].right_line;               //太远了直接延长
//        //////////////////////   补线end    //////////////////////////////
//      }
//      else if( line[i].left_line_state==lose )
//      {
//        //////////////////////   补线start   //////////////////////////////
//        if(i<75)
//          // line[i].left_line=line[i-3].left_line *(float)(X_change[79-i-3]/X_change[79-i]);                       //用左边线加上前一条线的宽度
//          line[i].left_line=line[i].right_line-distance[i];
//        else
//          line[i].left_line=line[i-1].left_line;
//        //////////////////////   补线end     //////////////////////////////
//      }
//      else
//      {
//
//      }
//    }
//    //--4--左右点限幅，防止一幅图像出现多个赛道的情况
//    if(i>0)
//    {
//      if(line[i].left_line<-20)
//      {
//        line[i].left_line=-20;
//      }
//      if(line[i].right_line>147)
//      {
//        line[i].right_line=147;
//      }
//      if(line[i].left_line>line[i].right_line)
//      {
//        line[i].left_line=line[i].right_line ;
//      }
//      if(line[i-1].left_line>line[i-1].right_line)
//      {
//        line[i-1].left_line=line[i-1].right_line;
//      }
//    }
//
//    //--5--中线提取
//    line[i].mid_line=(line[i].left_line+line[i].right_line)/2;
//
//    if( line[i].right_line_state==lose  &&  line[i].left_line_state==lose )
//    {   //左右都丢线的情况就不更新初始中线，直接沿用上次的
//    }
//    else if( line[i].right_line_state==lose )
//    {   //右边丢线就从左边往右找右边界
//      last_mid_first=line[i].left_line+4;
//    }
//    else if( line[i].left_line_state==lose )
//    {   //左边丢线就从右往左找左边界
//      last_mid_first=line[i].right_line-4;
//    }
//    else
//    {
//      last_mid_first=line[i].mid_line;
//    }
//
//    //    printf("\n 左=%d   中=%d   右=%d   last_mid_first=%d",line[i].left_line,line[i].mid_line,line[i].right_line,last_mid_first);
//    //--6--有效行截止 在白色赛道下需要测试
//    if((line[i].right_line-line[i].left_line<8)&&length_flag==0 && i>30)
//    {
//      length_flag=1;
//      used_length=i;
//      break;     //截止有效行之后，后面的图像就都不找线了
//    }
//  }
//  if( length_flag==0)   //整幅图片扫描结束之后如果没问题进行有效行重置
//  {
//    used_length=nHeight;
//  }
//  //   printf("\n 结束---------------------");
//  ////////////////////////////////////////////////////
//
//
//  //扫线到这结束，先写到这，剩下的再说，
//
//  //接下来需要处理的是丢线情况的补线问题
//
//
//  /////////////////////////////////////////////////////
//
//  /*
//  //    pimg=src+(69*128);    //指针的用法，表示的是SCR所指向图像中的第（69*128）的点，也就是图像左下角的点的位置
//
//  //    if(*(pimg+64)==0)     //注意，这里的0表示黑色，正常情况下会直接跳到else去
//  //    {
//  //      pimg=src+(nHeight-1-i)*nWidth; //此时指向图片的左下角， （79，128）
//  //      for(i=1;i<128;i++)
//  //      {
//  //        //黑白跳变
//  //        if( (*(pimg+i)-*(pimg+i-1))!=0 && *(pimg+i)==255)
//  //        {
//  //
//  //          last_mid_first=(127-i)/2+i;
//  //          break;
//  //
//  //        }
//  //        //白黑跳变
//  //        else if( (*(pimg+i)-*(pimg+i-1))!=0 && *(pimg+i)==0)
//  //        {
//  //
//  //          last_mid_first=i/2;
//  //          break;
//  //
//  //        }
//  //        else
//  //        {
//  //          last_mid_first=64;
//  //        }
//  //      }
//  //    }
//  //    else
//  //    {
//  //      last_mid_first=64;
//  //    }
//  //
//  //    last_mid=last_mid_first;
//
//  ///////////////////////////////////////////////////////////
//
//
//
//  //主要查看两个程序的区别
//
//  //然后再进行整合
//
//
//
//  //////////////////////////////////////////
//
//
//  //注:这里的图像扫描程序还非常之原始，还有许多都没有写，等到舵机PD控制写完后，就马上
//  //来将之完善一些，但这个是长期的事情，慢慢写，一定能完善的，相信自己。但是凡事要尽量
//  //往简单方面想，不用想复杂了，没那么复杂的。现在存在一个bug->在图像有一边边线丢失的
//  //时候，显示中线及边线都不好，图像中值也不变，这应该是图像没处理好的原因。之后再来完善
//
//  //从这里开始逐行扫描
//  for(i=0;i<line_num;i++)
//  {
//
//  pimg=src+(line_num-1-i)*128;      //Y=128，X从69开始递减（表示的是最左侧的那一竖行）
//
//  for(j=last_mid;j>0;j--)                   //扫左线   //每一次都从中间开始
//  {
//  tmpsrc=*(pimg+j);
//  if(tmpsrc==0)                         //中间的白色是255     =0的时候证明发生白黑跳变
//  {
//  line[i].left_line=j;
//  //         line[i].left_line_unrepiar=j;
//  left_case=1;
//  //到第一个黑点
//  //         l_judge_first_line(left_line_first_flag,i);
//  //         left_line_first_flag=1;
//  break;
//}
//}
//  //左边黑线没找到
//  if(j==0)
//  {
//  left_case=3; //如何处理?  没解决
//
//  break;
//}
//  for(j=last_mid;j<128;j++)              //扫右线
//  {
//  tmpsrc=*(pimg+j);
//  if(tmpsrc==0)
//  {
//  line[i].right_line=j;
//  right_case=1;
//  //到第一个黑点
//  break;
//}
//}
//  //右边黑线没找到
//  if(j==127)
//  {
//  right_case=0;					//如何处理?  没解决
//
//  break;
//}
//
//
//
//  //     //处理黑线
//  //     switch(left_case-right_case)      //这里是对每一行都处理一次
//  //     {
//  //     case 0:             //左右边线都扫到
//  //       line[i].line_case_mode=all_normal;
//  //       all_normal_count++;
//  //       break;
//  //     case 1:            //扫到左边线，丢失右边线
//  //       line[i].line_case_mode=left_normal_right_lose;
//  //       line[i].right_line=128;
//  //       line[i].right_line_unrepiar=128;
//  //       right_lost_count++;
//  //       break;
//  //     case 2:           //丢失左边线，扫到右边线
//  //       line[i].line_case_mode=left_lose_right_normal;
//  //       line[i].left_line=0;
//  //       line[i].left_line_unrepiar=0;
//  //       left_lost_count++;
//  //
//  //       break;
//  //     case 3:          //左右边线均丢失
//  //       line[i].line_case_mode=all_lose;
//  //       line[i].left_line=1;
//  //       line[i].right_line=127;
//  //       line[i].left_line_unrepiar=1;
//  //       line[i].right_line_unrepiar=127;
//  //       lost_count++;
//  //
//  //       break;
//  //     }
//
//
//
//
//  //*  赛道宽度赋值
//  line[i].road_wide=line[i].right_line-line[i].left_line;  //右边线坐标减左边线坐标
//
//  //***************************十字边线补线*************************
//  if(last_lost_count>25)
//  {
//  if(line[0].line_case_mode==left_lose_right_normal&&line[i].line_case_mode==all_lose)		  //十字从第一行开始丢
//  {
//  line[i].right_line=line[0].right_line-1;
//}
//  if(line[0].line_case_mode==left_normal_right_lose&&line[i].line_case_mode==all_lose)		  //十字从第一行开始丢
//  {
//  line[i].left_line=line[0].left_line+1;
//}
//  if(line[0].line_case_mode==all_lose&&line[i].road_wide-distance[i]>20)
//  {
//  line[i].left_line=64-distance[i]/2;
//  line[i].right_line=64+distance[i]/2;
//  shi_zi_flag=1;
//  if(never_shi_zi_count==0)
//  {
//  shi_zi_count++;
//  never_shi_zi_count=1;      //不让其继续检测
//}
//
//}
//}
//  if(shi_zi_flag==1&&line[0].line_case_mode==all_lose&&line[i].line_case_mode==all_lose)
//  {
//  line[i].left_line=64-distance[i]/2;
//  line[i].right_line=64+distance[i]/2;
//}
//  if(last_lost_count<5||line[0].line_case_mode==all_normal)
//  {
//  shi_zi_flag=0;
//}
//  //*******十字边线补线********
//  if(last_lost_count>25)    //左右两边同时丢线大于25次时进入十字判定
//  {
//  if(line[0].line_case_mode==left_lose_right_normal&&line[i].line_case_mode==all_lose)          //十字从第一行开始丢
//  {  //上一行是左丢线右正常  且 本次全部丢线    的时候，补右线   (正常右线-1 就是补出来的右线)
//  line[i].right_line=line[0].right_line-1;
//}
//  if(line[0].line_case_mode==left_normal_right_lose&&line[i].line_case_mode==all_lose)          //十字从第一行开始丢
//  {  //上一行是右丢线左正常  且 本次全部丢线    的时候，补左线   (正常左线+1 就是补出来的左线)
//  line[i].left_line=line[0].left_line+1;
//}
//  if(line[0].line_case_mode==all_lose&&line[i].road_wide-distance[i]>20)
//  { //如果两边同时丢线，  且赛道宽度突然增大20 ，就说明是十字丢线
//  line[i].left_line=64-distance[i]/2;
//  line[i].right_line=64+distance[i]/2;
//  shi_zi_flag=1;
//}
//}
//  if(shi_zi_flag==1&&line[0].line_case_mode==all_lose&&line[i].line_case_mode==all_lose)
//  {  //十字      第一行全丢线           当前行也全丢线               (补线:中线加减赛道宽度)
//  line[i].left_line=64-distance[i]/2;
//  line[i].right_line=64+distance[i]/2;
//}
//  if(last_lost_count<5||line[0].line_case_mode==all_normal)
//  { //连续丢线小于5次  或者  不丢线   则清除十字标志位
//  shi_zi_flag=0;
//}
//
//  if(i>3)  //图像扫描超过三行之后，如果不是单线就进行以下补线
//  {
//  if(line[i-1].left_line>5&&line[i-2].left_line-line[i-3].left_line>0&&(line[i-1].left_line-line[i-2].left_line==0)||(line[i-1].left_line-line[i-2].left_line>0)&&line[i].left_line-line[i-1].left_line<0&&(line[i].road_wide-line[i-1].road_wide>0||line[i].road_wide-distance[i]>12))      //line[i].line_case_mode==left_normal_right_lose
//  {    //首行左线大于5    且       第2行左线大于第三行左线           且        第1行左线等于第2行左线             或       第1行左线大于第2行左线              且      第0行左线小于第1行左线         且            第0行宽度大于第一行宽度         或      第0行宽度比正常宽度大12
//
//  line[i].left_line=line[i-1].left_line+1;
//}
//  if(line[i-1].right_line<122&&line[i-2].right_line-line[i-3].right_line<0&&(line[i-1].right_line-line[i-2].right_line==0)||(line[i-1].right_line-line[i-2].right_line<0)&&line[i].right_line-line[i-1].right_line>0&&(line[i].road_wide-line[i-1].road_wide>0||line[i].road_wide-distance[i]>12))//||line[i].line_case_mode==all_lose
//  {
//  line[i].right_line=line[i-1].right_line-1;
//}
//}
//  //防止一幅图片出现多条赛道。。
//  if(line[i].left_line>line[i].right_line)  //若 左线大于右线  就令  左线等于右线
//  {
//  line[i].left_line=line[i].right_line;
//}
//  if(line[i-1].left_line>line[i-1].right_line) //若 左线大于右线  就令  左线等于右线  （上行）
//  {
//  line[i-1].left_line=line[i-1].right_line;
//}
//  if(line[i-2].left_line>line[i-2].right_line)  //若 左线大于右线  就令  左线等于右线  (上上行)
//  {
//  line[i-2].left_line=line[i-2].right_line;
//}
//  //中线补线情况
//  if(i==0)
//  {              //第一行中线为  左加右除2
//  line[i].mid_line_new= (line[i].left_line+ line[i].right_line)/2;
//}
//  else
//  {
//  if(line[i].left_line < 1)          //左边线超出屏幕范围的时候，中线靠右边界的变化确定
//  line[i].mid_line_new=line[i-1].mid_line_new+(line[i].right_line-line[i-1].right_line);
//  else
//  if(line[i].right_line >127)      //右边线超出屏幕范围的时候，中线靠左边界的变化确定
//  line[i].mid_line_new=line[i-1].mid_line_new+(line[i].left_line-line[i-1].left_line);
//  else             //左右边线都正常就是左加右除2
//  line[i].mid_line_new= (line[i].left_line+ line[i].right_line)/2;
//}
//  last_mid=line[i].mid_line_new;   //中线储存
//  //对中线进行限幅 ，防止超出范围太多
//
//  //用于确定有效行  (左右线距离在8以内说明太远了，图像无效)
//  if((line[i].right_line-line[i].left_line)<8)
//  {
//  used_length=i;
//  line[used_length].mid_line_new=255;
//  return;
//}
//  used_length=70;
//  */
//}



