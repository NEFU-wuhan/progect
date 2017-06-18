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

#define add_count(number,x) number<x?(number++):(number=x)              //3目运算：如果式子为真，表达式的值等于冒号前的式子


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
 uint8 road_wide_chao[20];
 uint8 xia_no=0;
 uint8 budiu=0;
 uint8 huan_l_x=0,huan_r_x=0,huan_kuan=0,huan_kuan1_l=0,huan_kuan1_r=0,huan_loca=0;
 uint8 h_jiao=0,h_jiao_x=0,h_jiao6=0,h_jiao_x6=0,h_jiao_cnt=0;
 uint8 follow_huan;    //1 为寻左线，0为寻右线


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

uint8 distance2[70]=
{
   108,107,105,105,103,102,101,99,99,97,
   97,95,94,93,92,91,89,88,87,86,
   85,84,82,82,80,79,78,76,76,74,
   74,72,72,70,69,68,67,66,64,63,
   62,61,59,59,57,57,55,53,53,51,
   50,49,48,48,46,45,43,43,41,40,
   39,38,36,36,34,33,32,31,30,28,
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

/***************直道判断变量****************/
        uint8 zhi_dao_flag;
        uint8 zhidao_flag_1;
        uint8 zhidao_flag_2;
        uint8 zhidao_flag_3;
        uint8 zhidao_flag_4;
        uint8 zhidao_flag_5;
        uint8 all_normal_count_zhidao;
        uint8 all_normal_count_zhidao_1;
        uint8 all_normal_count_zhidao_2;
        uint8 all_normal_count_zhidao_3;
        uint8 all_normal_count_zhidao_4;
        uint8 all_normal_count_zhidao_5;

        uint8  R_wan_ru_zhi_cut_R=0;
        uint8 R_wan_ru_zhi_cut_R_1=0;
        uint8 R_wan_ru_zhi_cut_R_2=0;
        uint8  R_wan_ru_zhi_cut_L=0;
        uint8  L_wan_ru_zhi_cut_R=0;
        uint8  L_wan_ru_zhi_cut_L=0;
        uint8 L_wan_ru_zhi_cut_L_1=0;
        uint8 L_wan_ru_zhi_cut_L_2=0;

        uint8 R_wrz_flag;
        uint8 L_wrz_flag;

   /**********************************************/
   /////////podao
    uint8 po_road_wide_up[2][5],po_road_wide_down[2][5],po_road_wide_ss[5][10];
    ///////////////////
//起跑线识别
uint8 left_lost_count;
uint8 right_lost_count;
uint8 yiting = 0;
int ccout = 0;
uint8 stop_count=0;
/////////////////////
void Search_all_boundary( uint8 *src ,uint8 i);
void huan_mark_start();
void huan_mark_end();
void huan_judge( uint8 *src ,uint8 i );
void huan_clear( uint8 i );
void huan_along( uint8 i );
void zhidao( uint8 i );
void start_line(uint8 *src);
void start_line_stop() ;
void R_wan_ru_zhi_judge();
void L_wan_ru_zhi_judge();

//------------------------------------------------------------------------------
//功能    记录最下面几行和最上面几行的赛道宽度（用于坡道判断）
//------------------------------------------------------------------------------
void record_side_wide_part()
{
  uint8 j;
  uint16 po_road_wide_add_up0=0,po_road_wide_add_down0=0,po_road_wide_add_up1=0,po_road_wide_add_down1=0;
  uint8 num_po=5;
  uint8 s=0;
  for(j=4;j>0;j--)
  {
    po_road_wide_up[0][j]  = po_road_wide_up[0][j-1];
    po_road_wide_down[0][j]= po_road_wide_down[0][j-1];
    po_road_wide_up[1][j]  = po_road_wide_up[1][j-1];
    po_road_wide_down[1][j]= po_road_wide_down[1][j-1];
    for(s=0;s<num_po;s++)
    {
      po_road_wide_ss[j][s]=po_road_wide_ss[j-1][s];
    }
  }
  for(j=0;j<num_po;j++)
  {
    po_road_wide_add_down0 += line[j].road_wide;
    po_road_wide_add_up0   += line[69-j].road_wide;

    po_road_wide_ss[0][10-j]=line[69-j].road_wide ;
  }
  for(j=5;j<num_po+5;j++)
  {
    po_road_wide_add_down1 += line[j].road_wide;
    po_road_wide_add_up1   += line[69-j].road_wide;

    po_road_wide_ss[0][10-j]=line[69-j].road_wide ;
  }
  po_road_wide_up[0][0]=po_road_wide_add_up0/num_po;
  po_road_wide_down[0][0]=po_road_wide_add_down0/num_po;
  po_road_wide_up[1][0]=po_road_wide_add_up1/num_po;
  po_road_wide_down[1][0]=po_road_wide_add_down1/num_po;

}

//------------------------------------------------------------------------------
//功能    记录停车瞬间最下面几行的赛道宽度并与后面的赛道宽度比较是否变窄（用于直道超车）
//输出 road_count_chao 变窄的行数
//------------------------------------------------------------------------------
void recoed_xia_wide( uint8 i ,uint8 wider)
{
  uint8 j=0;
  static uint8 ci=0;
  if(chao_zhi==1 && ci==0 && i==20 )    //记录下停车时最下面几行的赛道宽度     && leftval<=5 && rightval<=5
  {
    ci++;
    for(j=0;j<20;j++)
    {
      road_wide_chao[j]= line[j].road_wide;
    }
  }
  if( line[i].road_wide < ( road_wide_chao[i]-wider) && i<20 && chao_zhi==1 )      //在这计算最下面几行的赛道宽度，直道超车的时候用
  {                                                            //  && leftval<=5 && rightval<=5
    road_count_chao++;
  }
}

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
    //环道定义
    road_count_chao=0;
    all_normal_count_huan=0;
    road_wide_count_huan=0;

    fiag_huan_r=0;
    fiag_huan_l=0;
    budiu=0;
    huan_l_x=0,huan_r_x=0,huan_kuan=0,huan_kuan1_l=0,huan_kuan1_r=0,huan_loca=0;
    h_jiao=0,h_jiao_x=0,h_jiao6=0,h_jiao_x6=0,h_jiao_cnt=0;
    float slope=0;   //计算斜率
    int16 last_dis_x=0,dis_x=0,dis_x_cnt=0,last_zuo_x=0,last_you_x=128;

    /***************直道判断变量****************/

         zhidao_flag_1=0;
         zhidao_flag_2=0;
         zhidao_flag_3=0;
         zhidao_flag_4=0;
         zhidao_flag_5=0;
         all_normal_count_zhidao=0;
         all_normal_count_zhidao_1=0;
         all_normal_count_zhidao_2=0;
         all_normal_count_zhidao_3=0;
         all_normal_count_zhidao_4=0;
         all_normal_count_zhidao_5=0;

          R_wan_ru_zhi_cut_R=0;
          R_wan_ru_zhi_cut_R_1=0;
          R_wan_ru_zhi_cut_R_2=0;
          R_wan_ru_zhi_cut_L=0;
          L_wan_ru_zhi_cut_R=0;
          L_wan_ru_zhi_cut_L=0;
          L_wan_ru_zhi_cut_L_1=0;
          L_wan_ru_zhi_cut_L_2=0;

   /**********************************************/

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

    //  记录每幅图片最下面十行的平均赛道宽度和最上面十行的平均赛道宽度用于判断坡道
    record_side_wide_part();
    //////////////////////////////////////////////////////////
    for(i=0;i<line_num;i++)             //大的行循环
     {
        pimg=src+(line_num-1-i)*128;    //倒着处理，从最底下一直扫到最上面

        Search_all_boundary( pimg,i );  //单纯的提取左右边线（不进行处理）
        recoed_xia_wide( i ,15);        //记录最下面几行的赛道宽度用于直道超车
// 识别环道
        if( fiag_huan==0 )  //&& front_car==1
        {
           huan_judge( src , i );
        }
//环岛识别结束
//刚入直道状态识别
        if( Ramp_flag!=0 )
        {
          if(Ramp_tong==1)
          {
            if((Total_distance-Ramp_Len_x)>200 )   //表示先识别坡道的车
            {
              if((Total_distance-Ramp_Len_x)<400)
              {
                Ramp_flag=2;
              }
              else
              {
                Ramp_Len_x=0;
                Ramp_flag=0;
              }
            }
          }
          else
          {
            if( Ramp_tong==0 )
            {
              if((Total_distance-Ramp_Len_x)>250 )   //表示通过无线传输识别坡道的车
              {
                if( (Total_distance-Ramp_Len_x)<450 )
                {
                  Ramp_flag=2;
                }
                else
                {
                  Ramp_Len_x=0;
                  Ramp_flag=0;
                }
              }
            }
          }
        }
        if( i==69 && fiag_huan==0 && Ramp_flag==0 )  //&& Total_distance>250
        {                      //&& front_car==1
//          if(i==55)
//          {
//            R_wan_ru_zhi_judge();
//            L_wan_ru_zhi_judge();
//          }
          zhidao( i );     //识别直道
        }

//刚入直道状态识别结束

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


        ////请环道标志
        huan_clear( i );

        //////////////////////////////  环岛巡线  ////////////////////////////
        huan_along( i );

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
            if(  i>30
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
            if(  i>30
               &&( (abs(line[i].right_line_unrepiar-line[i-1].right_line_unrepiar)>15) || line[i].right_line_unrepiar==0 || line[i].right_line_unrepiar==1 )
                 )
            {                                                                                                                             //|| line[i].left_line_unrepiar== line[i].right_line_unrepiar
              used_length=i;
              line[used_length].mid_line_new=255;
              return;
            }
          }
        }

        if(((ABS(line[i].right_line_unrepiar-line[i-1].right_line_unrepiar)>10)||(ABS(line[i].left_line_unrepiar-line[i-1].left_line_unrepiar)>10))&&i>40 && bujiezhi==0)
        {
          used_length=i;
          line[used_length].mid_line_new=255;
          return;
        }
        if(*(pimg+last_mid)==0 && bujiezhi==0)
        {
          used_length=i;
          line[used_length].mid_line_new=255;
          return;
        }
        if( Ramp_flag==1 && i>=30)
        {
          used_length=30;
          line[used_length].mid_line_new=255;
          return;
        }
        if(((line[i].right_line-line[i].left_line)<15)&&i>=20 && bujiezhi==0 )      //计算出有效行
         {                                               //加上这个判断，不然起跑线很有问题 数据会被有效行截断
				used_length=i;
				line[used_length].mid_line_new=255; 	 //记好了这里有效行的中值给的255，在其它地方会用到	255是不能在小液晶上显示的
				DD_mid_line_add=temp_dd_mid_line_add;
				temp_dd_mid_line_add=0;
            return;                                  //跳出本次循环
         }
         used_length=70;

 }
}

//-------------------------------------------------
//                 边界提取
//输入    uint8 *src    经过转换的图像地址
//输出
//功能   单纯的寻找图像中的左边界和右边界（不进行任何处理）
//--------------------------------------------------
void Search_all_boundary( uint8 *src ,uint8 i)
{
  uint8 tmpsrc,j;
  uint8 left_case,right_case;
if(*(src+last_mid)==255)                    //中点为白色
 {
  for(j=last_mid;j>0;j--)                    //扫左线   //每一次都从中间开始
  {
    tmpsrc=*(src+j);
    if(tmpsrc==0)
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
    tmpsrc=*(src+j);
    if(tmpsrc==0)
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
 }
 else              //中点为黑色  从两边往中间扫线
 {
  for(j=0;j<127;j++)                    //扫左线
  {
    tmpsrc=*(src+j);
    if(tmpsrc==255)
    {
      line[i].left_line=j-1;
      line[i].left_line_unrepiar=j-1;
      line[i].left_line_state=normal;
      //到第一个黑点
      l_judge_first_line(left_line_first_flag,i);
      left_line_first_flag=1;
      if(ABS(line[i].left_line_unrepiar-line[i-1].left_line_unrepiar)>5)
      {
        line[i].left_line=0;
        line[i].left_line_unrepiar=0;
        used_length=i;
	line[used_length].mid_line_new=255;
      }
      break;
    }
  }

  for(j=127;j>0;j--)              //扫右线
  {
    tmpsrc=*(src+j);
    if(tmpsrc==255)
    {
      line[i].right_line=j+1;
      line[i].right_line_unrepiar=j+1;
      line[i].right_line_state=normal;
      //到第一个黑点
      r_judge_first_line(right_line_first_flag,i);
      right_line_first_flag=1;
      if(ABS(line[i].right_line_unrepiar-line[i-1].right_line_unrepiar)>5)
      {
        line[i].right_line=127;
        line[i].right_line_unrepiar=127;
        used_length=i;
	line[used_length].mid_line_new=255;

      }
      break;
    }
  }
 }

  //确定黑线情况
  switch(left_case-right_case)      //这里是对每一行都处理一次
  {
  case 0:             //左右边线都扫到
    line[i].line_case_mode=all_normal;
    add_count(all_normal_count,5);
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
}

//-------------------------------------------------
//                   环道标记变量处理
//输入
//输出
//功能 环道已经识别，用这个函数取代环道正好识别时的记录标志等处理
//--------------------------------------------------
void huan_mark_start()
{
  fiag_huan=1;
  add_count(hehe,200);
  //car_stop(1);
  //            tongbu[3]=1;
  if(var3==0  //加这个条件是为了让前车识别超车标志，开始超车（且不让后车再进入）
     && chao_huan==0    //为了前车只发送一次切换标志，防止停车的时候一直符合条件一直切换，超车完成之后清除标志
       )   //&& sequence==1
  {
    add_count(cut_1,200);
    chao_huan=1;

    var3=1;         //并且环道切换标志同步（表示正在切换，切换完成后清零）
    updata_var(VAR3);
    tongbu[3]=10;

    if(var1==1) var1=0;   //在这切换前后车
    else var1=1;
    updata_var(VAR1);
    tongbu[1]=10;
  }
}

//-------------------------------------------------
//                   环道清标记变量处理
//输入
//输出
//功能 环道结束之后，用这个函数取代出环道时的清楚标志等处理
//--------------------------------------------------
void huan_mark_end()
{
  xia_no = 0;
  all_normal_count_huan=0;
  fiag_huan=0;
  //&& sequence==0
  if(front_car==1 && var3==1  && chao_huan==0 )   //超车完成后的前车出去的时候将环道超车标志清零并同步给后车，让后车出来
  {   //                       防止前车刚停下还未被超车就清除超车标志了，可以把速度变为其他单独的标志
    var3=0;
    updata_var(VAR3);
    tongbu[3]=10;
  }
}

//-------------------------------------------------
//                   环道判断
//输入
//输出
//功能 判断环道
//--------------------------------------------------
void huan_judge( uint8 *src ,uint8 i )
{
           uint8 tmpsrc,j;
           uint8  *pimg1;

          uint8 zuo=0,you=0,zuo_x=0,you_x=0,zuo1=0,you1=0,you2_x=0,you2=0,zuo2=0,zuo2_x=0,zhong2=0,zhong2_x=0;
//--1--//   -----判断环道前的直道-----------------------------
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
//--2--//   -----直道之后必须得变宽-----------------------------
            //////////////////////判断赛道变宽的程序
            if( i<50 && line[i].road_wide > line[i-2].road_wide
               && line[i].right_line_unrepiar >= line[i-2].right_line_unrepiar
                 && line[i].left_line_unrepiar <= line[i-2].left_line_unrepiar
                   || line[i].line_case_mode== all_lose
               )
            {
              road_wide_count_huan++;
              if(road_wide_count_huan>10
                 && (line[i].left_line_unrepiar<=15)
                   && (line[i].right_line_unrepiar>=113) )
              {
                huan_kuan=1;
              }
            }
//--3--//   -----变宽之后能找到类似直角的尖-----------------------------
            if( huan_kuan==1)     //找拐点  就是类似于一个直角的尖
            {
              for(j=8;j<40;j++)
              {
                if(j<35 && line[j].left_line_unrepiar>3
                   &&  line[j].left_line_unrepiar<line[j-2].left_line_unrepiar
                     && (line[j-6].left_line_unrepiar>=line[j-8].left_line_unrepiar)
                       )
                {
                  fiag_huan_l=1;
                  huan_l_x=j;
                }
                if(j<35 && line[j].right_line_unrepiar<125
                   &&  line[j].right_line_unrepiar>line[j-2].right_line_unrepiar
                     && (line[j-6].right_line_unrepiar<=line[j-8].right_line_unrepiar) )
                {
                  fiag_huan_r=1;
                  huan_r_x=j;
//                  if((line[j-7].right_line_unrepiar==line[j-8].right_line_unrepiar))   //如果下面的部分都是相等的，检查是否是十字时的尖角
//                  {   //向上寻找最上面的一个黑点
//                    uint8 m,v;
//                    for(m=huan_r_x;m<(huan_r_x+5);m++)
//                    {
//                      pimg1=src+(line_num-1-m)*128;
//                      tmpsrc=*(pimg1+line[huan_loca].right_line_unrepiar - (j- huan_loca));
//                      for(v=(line[j-2].right_line_unrepiar-5);v<127;v++)
//                      {
//                        if(0==*(pimg1+v-1) && 255==*(pimg1+v) ) //黑白跳变
//                        {
//                           fiag_huan_r=1;
//                           huan_r_x=j;
//                        }
//                      }
//                    }
//                  }
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
                   &&  line[j].left_line_unrepiar<line[j-2].left_line_unrepiar  && line[j].line_case_mode==left_normal_right_lose
                     &&  line[j-2].left_line_unrepiar<line[j-4].left_line_unrepiar && line[j-4].line_case_mode==left_normal_right_lose
                       &&  line[j-4].left_line_unrepiar<line[j-6].left_line_unrepiar && line[j-6].line_case_mode==left_normal_right_lose
                         && (line[j-8].left_line_unrepiar>=line[j-10].left_line_unrepiar) )
                {
                  huan_kuan1_l=1;
                  huan_loca=j-6;
                }

              }
            }
            uint8 n=0,zuo_mini=0,you_mini=0;
            uint8 X_buff[69],Y_buff[69],f;
//--4--//   -----找到尖角之后根据正常图像的斜率向上寻找中心圆环---------------
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
                  //计算斜率
                  for(f=0;f<huan_loca;f++)
                  {
                    X_buff[f]=line[f].right_line_unrepiar;
                    Y_buff[f]=f;
                  }
                  Regression_cal(X_buff,Y_buff,huan_loca-4);
                  //找右
                  tmpsrc=*(pimg1+(uint8)(line[huan_loca-4].right_line_unrepiar + (float)(j- huan_loca)/(index_B+0.2)));  //这个点是右边直接加上去的点
                  if( tmpsrc ==0  )  // src+(line_num-1-i)*128;    && (line[huan_loca].left_line_unrepiar - (j- huan_loca))>10
                  {
                    you2=1;
                    you2_x=(uint8)(line[huan_loca-4].right_line_unrepiar + (float)(j- huan_loca)/(index_B+0.2)) ;
                  }
                  //找中
                  tmpsrc=*(pimg1+(uint8)(line[huan_loca-4].mid_line_new + (float)(j- huan_loca)/(index_B-0.2)));  //这个点是右边直接加上去的点
                  if( tmpsrc ==0  )  // src+(line_num-1-i)*128;    && (line[huan_loca].left_line_unrepiar - (j- huan_loca))>10
                  {
                    zhong2=1;
                    zhong2_x=(uint8)(line[huan_loca-4].mid_line_new + (float)(j- huan_loca)/(index_B-0.2)) ;
                  }
                  if( ( you2==1 && zhong2==1)  )
                  {
                    for(n=zhong2_x+2;n<you2_x-2;n++)
                    {
                      if(255==*(pimg1+n) ) //白点退出
                      {
                        zuo_mini=1;
                        break;
                      }
                      else
                      {
                        //                        you_mini= 1;   //
                      }
                    }
                    if( ( zuo_mini==0
                         //                       && you_mini==0
                         //                       && you_mini>zuo_mini
                         )   //zuo2==1 ||
                       )
                    {
                      huan_mark_start();
                      break;
                    }
                  }


                }
                else
                {
                  {
                    zuo2=0;
                    zuo2_x=0;
                    pimg1=src+(line_num-1-j)*128;
                    //计算斜率
                    for(f=0;f<huan_loca;f++)
                    {
                      X_buff[f]=line[f].left_line_unrepiar;
                      Y_buff[f]=f;
                    }
                    Regression_cal(X_buff,Y_buff,huan_loca-4);
                    //zhao左
                    tmpsrc=*(pimg1+(uint8)(line[huan_loca-4].left_line_unrepiar + (float)(j- huan_loca)/(index_B-0.2)));  //这个点是右边直接加上去的点

                    //                    tmpsrc=*(pimg1+line[huan_loca].left_line_unrepiar + (j- huan_loca));
                    if( tmpsrc ==0 )  // src+(line_num-1-i)*128;
                    {
                      zuo2=1;
                      zuo2_x=(uint8)(line[huan_loca-4].left_line_unrepiar + (float)(j- huan_loca)/(index_B-0.2)) ;
                    }
                    //找中
                    tmpsrc=*(pimg1+(uint8)(line[huan_loca-4].mid_line_new + (float)(j- huan_loca)/(index_B+0.2)));  //这个点是右边直接加上去的点
                    if( tmpsrc ==0  )  // src+(line_num-1-i)*128;    && (line[huan_loca].left_line_unrepiar - (j- huan_loca))>10
                    {
                      zhong2=1;
                      zhong2_x=(uint8)(line[huan_loca-4].mid_line_new + (float)(j- huan_loca)/(index_B+0.2)) ;
                    }
                    if( ( zuo2==1 && zhong2==1)  )
                    {
                      for(n=zuo2_x+2;n<zhong2_x-2;n++)
                      {
                        if(255==*(pimg1+n) ) //白点退出
                        {
                          you_mini=1;
                          break;
                        }
                        else
                        {
                          //                        you_mini= 1;   //
                        }
                      }

                      if( ( //zuo_mini!=0
                        you_mini==0
//                       && you_mini<zuo_mini
                         )   //zuo2==1 ||
                     )
                  {
                          huan_mark_start();
                    break;
                  }

                    }

                  }

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
                                    //计算斜率
                    for(f=0;f<huan_l_x;f++)
                    {
                      X_buff[f]=line[f].left_line_unrepiar;
                      Y_buff[f]=f;
                    }
                    Regression_cal(X_buff,Y_buff,huan_l_x-4);
                    //zhao左
                    tmpsrc=*(pimg1+(uint8)(line[huan_l_x-4].left_line_unrepiar + (float)(j- huan_l_x)/(index_B-0.2)));  //这个点是右边直接加上去的点

                    //                    tmpsrc=*(pimg1+line[huan_loca].left_line_unrepiar + (j- huan_loca));
                    if( tmpsrc ==0 && j>(huan_l_x+5))  // src+(line_num-1-i)*128;
                    {
                      zuo1=1;
                      zuo_x=(uint8)(line[huan_l_x-4].left_line_unrepiar + (float)(j- huan_l_x)/(index_B-0.2)) ;
                    }
                                  //计算斜率
                  for(f=0;f<huan_r_x;f++)
                  {
                    X_buff[f]=line[f].right_line_unrepiar;
                    Y_buff[f]=f;
                  }
                  Regression_cal(X_buff,Y_buff,huan_r_x-4);
                  //找右
                  tmpsrc=*(pimg1+(uint8)(line[huan_r_x-4].right_line_unrepiar + (float)(j- huan_r_x)/(index_B+0.2)));  //这个点是右边直接加上去的点
                  if( tmpsrc ==0 && j>(huan_r_x+5) )  // src+(line_num-1-i)*128;    && (line[huan_loca].left_line_unrepiar - (j- huan_loca))>10
                  {
                    you1=1;
                    you_x=(uint8)(line[huan_r_x-4].right_line_unrepiar + (float)(j- huan_r_x)/(index_B+0.2)) ;
                  }

                if(zuo1==1 && you1==1 )
                {
                  if(zuo_x>you_x)
                  {
                    for(n=you_x+2;n<zuo_x-2;n++)
                    {
                      if(255==*(pimg1+n) ) //白点退出
                      {
                        zhong2=1;
                        break;
                      }
                      else
                      {
                        //                        you_mini= 1;   //
                      }
                    }
                  }
                  else
                  {
                    for(n=zuo_x+2;n<you_x-2;n++)
                    {
                      if(255==*(pimg1+n) ) //白点退出
                      {
                        zhong2=1;
                        break;
                      }
                      else
                      {
                        //                        you_mini= 1;   //
                      }
                    }
                  }
                  if( ( zhong2==0 ) )
                    {
                      huan_mark_start();
                      break;
                    }
                }
              }
            }
          }
          ////////////////////////////////////////
}



//-------------------------------------------------
//                   环道清除
//输入
//输出
//功能 清楚环道标志，并判断进环道出环道的位置
//--------------------------------------------------
void huan_clear( uint8 i )
{
  uint8 j;
        /////////////  清除环岛标志（出环道）  //////////////////////
     if(fiag_huan==1)
     {
       if( follow_huan )     // 1为寻左线,0为寻右线
       {
//--1--（寻左）先找到入环道尖角后的特征（右边丢线）
         if( i==10 && xia_no==0  && line[i].right_line_state== lose && line[i-8].right_line_state== lose
            &&(line[i].left_line_unrepiar < line[i-2].left_line_unrepiar ||  line[i-2].left_line_unrepiar <8 )
               &&(line[i-2].left_line_unrepiar < line[i-4].left_line_unrepiar ||  line[i-4].left_line_unrepiar <8 )
              )
         {
           xia_no=1;
         }
//--2--（寻左）在找到出环道处的尖角
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
           if(chao_huan==1 && var3==1 && huan_chao_cont==1)      //&& sequence==1
           {
             speedwantD=0;
             speedwantE=0;
           }
         }
//--3--（寻左）一直到看不到尖角时标志
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
//--1--（寻右）先找到入环道尖角后的特征（右边丢线）
         if( i==10 && xia_no==0  && line[i].left_line_state== lose && line[i-8].left_line_state== lose
            &&(line[i].right_line_unrepiar > line[i-2].right_line_unrepiar ||  line[i-2].right_line_unrepiar >120 )
               &&(line[i-2].right_line_unrepiar > line[i-4].right_line_unrepiar ||  line[i-4].right_line_unrepiar >120 )
              )
         {
           xia_no=1;
         }
//--2--（寻右）在找到出环道处的尖角
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
           if(chao_huan==1 && var3==1  && huan_chao_cont==1)      //&& sequence==1
           {
             speedwantD=0;
             speedwantE=0;
           }
         }
//--3--（寻右）一直到看不到尖角时标志
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

//--4--出环道的尖角之后如果有直道则清除环道标志
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
             if(all_normal_count_huan>10)
             {
               huan_mark_end();
               break;
             }

           }
         }
     }
     // 清环道结束
}


//-------------------------------------------------
//                   环道中巡线
//输入
//输出
//功能 在环道中分别进行一边丢线两边丢线和不丢线情况下的找中线
//--------------------------------------------------
void huan_along( uint8 i )
{
  uint8 j ;
  if( fiag_huan )
  {
    if( follow_huan )    //寻左线
    {
      if(line[i].right_line_unrepiar>line[i].left_line_unrepiar  && line[i].right_line_state==normal      // 如果能找到正常的右线
         && (line[i].left_line_unrepiar+distance2[i]+10) > line[i].right_line_unrepiar
            )
      {
        if(line[i].left_line_state==normal)   //如果有左线就直接用左右边线
        {
          line[i].left_line  = line[i].left_line_unrepiar;
          line[i].right_line = line[i].right_line_unrepiar;
          line[i].mid_line_new= (line[i].left_line+ line[i].right_line)/2;
        }
        else   //否则就根据右线补左线
        {
          if(i<10)
          {
          line[i].left_line  = line[i].right_line_unrepiar-distance2[i];
          line[i].right_line = line[i].right_line_unrepiar;
          line[i].mid_line_new= (line[i].left_line+ line[i].right_line)/2;
          }
          else
          {
            line[i].right_line = line[i].right_line_unrepiar;
            line[i].mid_line_new=line[i-1].mid_line_new+(line[i].right_line-line[i-1].right_line);
          }
        }
      }
      else if(line[i].left_line_state==lose ) //如果没有正常的右线，也找不到左线就直接给定中线
      {
        line[i].left_line=0;
        line[i].right_line=64;
        line[i].mid_line_new= (line[i].left_line+ line[i].right_line)/2;
      }
      else   // 有正常左边线，没有正常右边线的情况，根据左线补右线
      {
        if(i<10)
        {
        line[i].left_line  = line[i].left_line_unrepiar;
        line[i].right_line = line[i].left_line_unrepiar+distance2[i];
        line[i].mid_line_new= (line[i].left_line+ line[i].right_line)/2;
        }
        else
        {
        line[i].left_line  = line[i].left_line_unrepiar;
        line[i].mid_line_new=line[i-1].mid_line_new+(line[i].left_line-line[i-1].left_line);
        }
       }
    }
    else    //寻右线
    {
      if(line[i].left_line_unrepiar<line[i].right_line_unrepiar && line[i].left_line_state==normal       // 如果能找到正常的左线
         && (line[i].left_line_unrepiar+distance2[i]+10) > line[i].right_line_unrepiar
             )
      {
        if(line[i].right_line_state==normal)   //如果有右线就直接用左右边线
        {
          line[i].left_line  = line[i].left_line_unrepiar;
          line[i].right_line = line[i].right_line_unrepiar;
          line[i].mid_line_new= (line[i].left_line+ line[i].right_line)/2;
        }
        else   //否则就根据左线补右线
        {
          if(i<10)
          {
          line[i].left_line  = line[i].left_line_unrepiar;
          line[i].right_line = line[i].left_line_unrepiar+distance2[i];
          line[i].mid_line_new= (line[i].left_line+ line[i].right_line)/2;
          }
          else
          {
            line[i].left_line  = line[i].left_line_unrepiar;
            line[i].mid_line_new=line[i-1].mid_line_new+(line[i].left_line-line[i-1].left_line);
          }
        }
      }
      else if(line[i].right_line_state==lose ) //如果没有正常的左线，也找不到右线就直接给定中线
      {
        line[i].left_line=64;
        line[i].right_line=128;
        line[i].mid_line_new= (line[i].left_line+ line[i].right_line)/2;
      }
      else   // 有正常右边线，没有正常左边线的情况，根据右线补左线 （最常见）
      {
        if(i<10)
        {
        line[i].left_line  = line[i].right_line_unrepiar-distance2[i];
        line[i].right_line = line[i].right_line_unrepiar;
        line[i].mid_line_new= (line[i].left_line+ line[i].right_line)/2;
        }
        else
        {
          line[i].right_line = line[i].right_line_unrepiar;
          line[i].mid_line_new=line[i-1].mid_line_new+(line[i].right_line-line[i-1].right_line);
        }
      }
    }
  }
}


//-------------------------------------------------
//                   直道
//输入
//输出
//功能    判断直道
//--------------------------------------------------
void zhidao( uint8 i )
{
  uint8 j;
  uint8 zhidao_normal_cut=0,zhidao_normal_cut1=0;
  uint16 po_wide_num=0,po_wide_add=0,po_wide_av=0;
  if( used_length==70 )
    for(j=0;j<69;j++)
    {
      if( (line[j].left_line_unrepiar >= line[0].left_line_unrepiar)
       &&(line[j].left_line_unrepiar <= line[69].left_line_unrepiar)
         &&(line[j].right_line_unrepiar <= line[0].right_line_unrepiar)
           &&(line[j].right_line_unrepiar >= line[69].right_line_unrepiar) )
      {
          zhidao_normal_cut++;
      }
      if( (line[j].left_line_unrepiar >= line[j-1].left_line_unrepiar)
            &&(line[j].right_line_unrepiar <= line[j-1].right_line_unrepiar) )
      {
        if(j<5) zhidao_normal_cut1++;
        else if(line[j].line_case_mode==all_normal) zhidao_normal_cut1++;
      }
    }
  if( zhidao_normal_cut>66 && zhidao_normal_cut1>=63 )
  {
    //先计算第15行到50行的平均赛道宽度
    for(j=15;j<50;j++)
    {
      po_wide_num+=line[j].road_wide;
      po_wide_add++;
    }
    po_wide_av=po_wide_num/po_wide_add;
    if( po_wide_av>75) //平均宽度大于一定值时认为是坡道
    {
        Ramp_flag=1;
        Ramp_Len_x=Total_distance;

        var3=2;         //等于2表示坡道同步两车坡道标志
        updata_var(VAR3);
        tongbu[3]=10;
        Ramp_tong=1;

        add_count(cut_2,200);
//        chao_zhi=0;
//        wrz_distance=0;
//        L_wrz_flag=0;
//        R_wrz_flag=0;
//
        var4=0;         //并且环道切换标志同步（表示正在切换，切换完成后清零）
        updata_var(VAR4);
        tongbu[4]=10;

//        car_stop(1);
    }
  }
  if( zhidao_normal_cut>67 && zhidao_normal_cut1>=65 && Ramp_flag==0)
  {
    if( abs(po_road_wide_down[0][0]-po_road_wide_down[0][3])<2                 //在远处识别坡道的情况
       && (po_road_wide_up[0][0]-po_road_wide_up[0][3])>=3
         &&  abs(po_road_wide_down[1][0]-po_road_wide_down[1][3])<2
           && (po_road_wide_up[1][0]-po_road_wide_up[1][3])>=3
             && (abs(line[9].left_line_unrepiar - line[0].left_line_unrepiar)-abs(line[69].left_line_unrepiar - line[60].left_line_unrepiar))>=1
               && (abs(line[0].right_line_unrepiar - line[9].right_line_unrepiar)-abs(line[60].right_line_unrepiar - line[69].right_line_unrepiar))>=1
               )
    {
        Ramp_flag=1;
        Ramp_Len_x=Total_distance;

        var3=2;         //等于2表示坡道同步两车坡道标志
        updata_var(VAR3);
        tongbu[3]=10;
        Ramp_tong=1;

        add_count(cut_2,200);

//        chao_zhi=0;
//        wrz_distance=0;
//        L_wrz_flag=0;
//        R_wrz_flag=0;

        var4=0;         //并且环道切换标志同步（表示正在切换，切换完成后清零）
        updata_var(VAR4);
        tongbu[4]=10;

//        car_stop(1);
    }
    else if( R_wrz_flag==0 && L_wrz_flag==0 && wrz_chao_cont==1)
    {
      if( (line[69].left_line_unrepiar-line[0].left_line_unrepiar)>(line[0].right_line_unrepiar-line[69].right_line_unrepiar) )
      {
        chao_zhi=1;
        R_wrz_flag=1;
        zhidao_normal_cut=0;
//        car_stop(1);
      }
      else
      {
        chao_zhi=1;
        L_wrz_flag=1;
        zhidao_normal_cut=0;
//        car_stop(1);
      }
    }
  }
}
 /*
//--------------------------------------------------
//                   直道
//输入
//输出
//功能    判断直道
//--------------------------------------------------
void zhidao( uint8 i )
{
      int j;
//      uint8 L_x_point[70];
//      uint8 L_y_point[70];
//      uint8 R_x_point[70];
//      uint8 R_y_point[70];
//
//      L_x_point[i]=line[i].left_line_unrepiar;
//      L_y_point[i]=i;
//      R_x_point[i]=line[i].right_line_unrepiar;
//      R_y_point[i]=i;
//
//

//      if( line[i].road_wide-distance2[i]<=10 ) //&& line[i].right_line_unrepiar > 85 && line[i].left_line_unrepiar < 45 )
//      {
//        all_normal_count_zhidao++;
//        if(all_normal_count_zhidao>=45)
//        {
//        }
//      }

    if(i>=1 && i<=15)
    {
      if(line[i].line_case_mode==all_normal
         &&(line[i].left_line_unrepiar >= line[i-1].left_line_unrepiar)
         &&(line[i].left_line_unrepiar - line[i-1].left_line_unrepiar)<2
         &&(line[i].right_line_unrepiar <= line[i-1].right_line_unrepiar)
         &&(line[i-1].right_line_unrepiar-line[i].right_line_unrepiar)<2
         )
      {
        all_normal_count_zhidao_1++;
        if(all_normal_count_zhidao_1>=14)
        {
          zhidao_flag_1=1;
        }
      }
    }
    if(zhidao_flag_1==1&&i>15&&i<=30)
    {
       if(line[i].line_case_mode==all_normal
         &&(line[i].left_line_unrepiar >= line[i-1].left_line_unrepiar)
         &&(line[i].left_line_unrepiar - line[i-1].left_line_unrepiar)<2
         &&(line[i].right_line_unrepiar <= line[i-1].right_line_unrepiar)
         &&(line[i-1].right_line_unrepiar-line[i].right_line_unrepiar)<2
         )
      {
        all_normal_count_zhidao_2++;
        if(all_normal_count_zhidao_2>=15)
        {
          zhidao_flag_2=1;
        }
      }
    }
     if(zhidao_flag_2==1&&i>30&&i<=45)
    {
       if(line[i].line_case_mode==all_normal
         &&(line[i].left_line_unrepiar >= line[i-1].left_line_unrepiar)
         &&(line[i].left_line_unrepiar - line[i-1].left_line_unrepiar)<2
         &&(line[i].right_line_unrepiar <= line[i-1].right_line_unrepiar)
         &&(line[i-1].right_line_unrepiar-line[i].right_line_unrepiar)<2
         )
      {
        all_normal_count_zhidao_3++;
        if(all_normal_count_zhidao_3>=15)
        {
          zhidao_flag_3=1;
        }
      }
    }
    if(zhidao_flag_3==1&&i>45&&i<=60)
    {
       if(line[i].line_case_mode==all_normal
         &&(line[i].left_line_unrepiar >= line[i-1].left_line_unrepiar)
         &&(line[i].left_line_unrepiar - line[i-1].left_line_unrepiar)<2
         &&(line[i].right_line_unrepiar <= line[i-1].right_line_unrepiar)
         &&(line[i-1].right_line_unrepiar-line[i].right_line_unrepiar)<2
         )
      {
        all_normal_count_zhidao_4++;
        if(all_normal_count_zhidao_4>=12)
        {
          zhidao_flag_4=1;
          zhidao_flag_5=1;
        }
      }
    }
//    if(zhidao_flag_4==1&&i>60&&i<=69)
//    {
//       if(line[i].line_case_mode=all_normal
//         &&(line[i].left_line_unrepiar >= line[i-1].left_line_unrepiar)
//         &&(line[i].left_line_unrepiar - line[i-1].left_line_unrepiar)<2
//         &&(line[i].right_line_unrepiar <= line[i-1].right_line_unrepiar)
//         &&(line[i-1].right_line_unrepiar-line[i].right_line_unrepiar)<2
//         )
//      {
//        all_normal_count_zhidao_5++;
//        if(all_normal_count_zhidao_5>=6)
//        {
//          zhidao_flag_5=1;
//        }
//      }
//    }

  if(zhidao_flag_5==1)
  {
     zhi_dao_flag=1;
     zhidao_flag_1=0;
     zhidao_flag_2=0;
     zhidao_flag_3=0;
     zhidao_flag_4=0;
     zhidao_flag_5=0;

     all_normal_count_zhidao=0;
     all_normal_count_zhidao_1=0;
     all_normal_count_zhidao_2=0;
     all_normal_count_zhidao_3=0;
     all_normal_count_zhidao_4=0;
     all_normal_count_zhidao_5=0;
  }
}

/* s l
 * t i
 * a n
 * r e
 */
void start_line(uint8 *src)
{
  uint8 i,j,k;
  uint8 *pimg;                    //水平扫线

  static uint8 white_to_black_num;
  static uint8 black_to_white_num;
  static uint8 qpxian,a;

  //清零
  qpxian = 0;
  bujiezhi = 0;
 // stop = 0;

  for(i=0;i<30;i++)            //从第25行 开始扫线
  {
    black_to_white_num = 0;
    white_to_black_num = 0;

    pimg=src+(line_num-1-i)*128;
    for(j=10;j<=117;j++)               // 扫1行
    {
      //如果黑变白
      if( (*(pimg+j)-*(pimg+j-1))!=0 && *(pimg+j)==255)
       black_to_white_num++;
       //如果白变黑
      if( (*(pimg+j)-*(pimg+j-1))!=0 && *(pimg+j)==0)
      white_to_black_num++;
    }
    if(black_to_white_num>=5&&white_to_black_num>=5)
    {
      qpxian ++;
      line[i].line_case_mode = starline;
    }
    if(qpxian > 4 && System_time>=8)                   //大于3行 则判为起跑线 且有效行不节制
    {
      stop = 1;
      bujiezhi = 1;
    }
   if(stop == 1 && System_time>=8 && a==0)
   {
     a++;
     start_line_stop();
   }

  }
}


/* s c
 * t a
 * o r
 * p
 */
void start_line_stop()
{
  if(speedwant >= 120)            //速度比较大，延时20ms
  {
    flag_key_select=0;
    flag_key_l_u_5=0;

    car_stop( 1 );
    yiting = 1;
  }
  else                             //速度比较小，延时50ms
  {
    if( s_distance<550 && s_distance>10 )
    {
      if(stop_count > 15)
      {
        flag_key_select=0;
        flag_key_l_u_5=0;

        car_stop( 1 );
        yiting = 1;
      }
    }
    else
      if(stop_count > 40)
      {
        flag_key_select=0;
        flag_key_l_u_5=0;
        car_stop( 1 );
        yiting = 1;
      }
  }
}


//-------------------------------------------------
//功能    判断右弯道进入直道
//--------------------------------------------------

void R_wan_ru_zhi_judge()
{
  uint8 j;

  if(abs(line[50].right_line_unrepiar-line[3].right_line_unrepiar)<=15)         //右边50行与3行差小于10
  {
    for(j=5;j<50;j++)
    {
      if( line[j+3].line_case_mode==all_normal                                    //5到50行所有点正常
         && (line[j].right_line_unrepiar>=(line[3].right_line_unrepiar)        //
          || line[j].right_line_unrepiar<=(line[3].right_line_unrepiar))
          )
      {
        R_wan_ru_zhi_cut_R++ ;
      }
      if(line[j+3].line_case_mode==all_normal                                     //第一种情况 右点减小
         &&(line[j].right_line_unrepiar<=line[j-3].right_line_unrepiar)         //5到50行所有点正常
         &&(line[j-3].right_line_unrepiar-line[j].right_line_unrepiar)<=3        //右边第j行小于j-3行且差值小于3
         &&(line[j].right_line_state<=(line[3].right_line_unrepiar))          //j行的点在50和3行之间
         &&(line[j].right_line_unrepiar>=(line[50].right_line_unrepiar))
         )
      {
        R_wan_ru_zhi_cut_R_1++;
      }
      if(line[j+3].line_case_mode==all_normal                                     //第二种情况 右点增大
         &&(line[j].right_line_unrepiar>=line[j-3].right_line_unrepiar)         //5到50行所有点正常
         &&(line[j].right_line_unrepiar-line[j-3].right_line_unrepiar)<=3        //右边第j行大于于j-3行且差值小于3
         &&(line[j].right_line_unrepiar>=(line[3].right_line_unrepiar))       //j行的点在50和3行之间
         &&(line[j].right_line_unrepiar<=(line[50].right_line_unrepiar))
         )
      {
        R_wan_ru_zhi_cut_R_2++;
      }

      if(line[j+3].line_case_mode==all_normal                                     //左边点一直增大
         &&(line[j].left_line_unrepiar>=line[j-3].left_line_unrepiar)           //5到50行所有点正常
         &&(line[j].left_line_unrepiar-line[j-3].left_line_unrepiar)>=2          //左边点j行与j-3行差值小于6
         )
      {
        R_wan_ru_zhi_cut_L++;
      }
      if( R_wan_ru_zhi_cut_R>=43&&R_wan_ru_zhi_cut_L>=45&&((R_wan_ru_zhi_cut_R_1>=43)||(R_wan_ru_zhi_cut_R_2>=43)))
      {
        chao_zhi=1;

        R_wrz_flag=1;
        R_wan_ru_zhi_cut_R=0;
        R_wan_ru_zhi_cut_L=0;
        R_wan_ru_zhi_cut_R_1=0;
        R_wan_ru_zhi_cut_R_2=0;

      }
//      else
//      {
//        R_wrz_flag=0;
//      }

    }
  }
}

//-------------------------------------------------
//功能    判断左弯道进入直道
//--------------------------------------------------
void L_wan_ru_zhi_judge()
{
  uint8 j;

  if(abs(line[50].left_line_unrepiar-line[3].left_line_unrepiar)<=15)           //左边边50行与3行差小于10
  {
    for(j=5;j<50;j++)
    {
      if( line[j+3].line_case_mode==all_normal
         && (line[j].left_line_unrepiar>=(line[3].left_line_unrepiar)
          || line[j].left_line_unrepiar<=(line[3].left_line_unrepiar))
          )
      {
        L_wan_ru_zhi_cut_L++ ;
      }
      if(line[j+3].line_case_mode==all_normal                                     //第一种情况 左点增大
         &&line[j].left_line_unrepiar>=line[j-3].left_line_unrepiar             //5到50行所有点正常
         &&(line[j].left_line_unrepiar-line[j-3].left_line_unrepiar)<=3          //左边第j行大于j-3行且差值小于3
         &&(line[j].left_line_unrepiar>=line[3].left_line_unrepiar)           //j行的点在50和3行之间
         &&(line[j].left_line_unrepiar<=(line[50].left_line_unrepiar))
         )
      {
        L_wan_ru_zhi_cut_L_1++;
      }
      if(line[j+3].line_case_mode==all_normal                                         //第二种情况 左点减小
         &&line[j].left_line_unrepiar<=line[j-3].left_line_unrepiar             //5到50行所有点正常
         &&(line[j-3].left_line_unrepiar-line[j].left_line_unrepiar)<=3          //左边第j行小于j-3行且差值小于3
         &&(line[j].left_line_unrepiar<=(line[3].left_line_unrepiar))
         &&(line[j].left_line_unrepiar>=(line[50].left_line_unrepiar))
         )
      {
        L_wan_ru_zhi_cut_L_2++;
      }

      if(line[j+3].line_case_mode==all_normal                                     //右边点一直减小
         &&(line[j].right_line_unrepiar<=line[j-3].right_line_unrepiar)         //右边第j行小于j-3行且差值小于6
         &&(line[j-3].right_line_unrepiar-line[j].right_line_unrepiar)>=2        //右边点j行与j-3行差值小于6
         )
      {
        L_wan_ru_zhi_cut_R++;
      }
      if( L_wan_ru_zhi_cut_L>=43&&L_wan_ru_zhi_cut_R>=45&&((L_wan_ru_zhi_cut_L_1>=43)||(L_wan_ru_zhi_cut_L_2>=43)))
      {
        chao_zhi=1;

        L_wrz_flag=1;
        L_wan_ru_zhi_cut_R=0;
        L_wan_ru_zhi_cut_L=0;
        L_wan_ru_zhi_cut_L_1=0;
        L_wan_ru_zhi_cut_L_2=0;

      }

//      else
//        L_wrz_flag=0;

    }
  }
}


//-------------------------------------------------
//                   刚发车时的直道超车
//输入
//输出
//功能 刚冲出起跑线之后的超车，原则上应该可以选择刚出起跑线时的超车次数
//--------------------------------------------------
uint8 origin_chao( uint8 cut )
{
  static uint8 z=0,h=0;
  uint8 mid=64;
  if(Total_distance<170   //发车前两米内，一个车靠左一个车靠右
     && var4==1 )    // 只有在超车未完成的时候在进入
  {
    used_length=30;
    if(sequence==1)   //前车靠左
    {
      L_wrz_flag=3;
      if(Total_distance>30 && z==0 && chao_zhi==0)    //前车在指定位置停车    && Total_distance<35
      {
        z++;
        chao_zhi=1;
        car_stop(1);
      }
    }
    else              //后车靠右
    {
      R_wrz_flag=3;
    }
  }
  if(sequence==1)   //停下的车检测到超声波之后恢复正常
  {
    if( (road_count_chao>10 || ( leftval==0 && rightval==0 && s_distance<550 && s_distance>10))&& chao_zhi==1 && h==0)    //前车停车等待超车完成时若超声波能检测到数据，证明超车完成
    {
      chao_zhi=0;
      h++;
      L_wrz_flag=0;

      var4=0;         //并且环道切换标志同步（表示正在切换，切换完成后清零）
      updata_var(VAR4);
      tongbu[4]=10;

      if(var1==1) var1=0;   //在这切换前后车
      else var1=1;
      updata_var(VAR1);
      tongbu[1]=10;

      car_start(1);

      mid=64;
    }
  }
  else              //超过去的车接收到NRF信号之后恢复正常
  {
    if(var4==0)
    {
      mid=64;
      R_wrz_flag=0;
    }
  }
  return mid;
}

void zhi_chao_deal()
{

  speedwantD=40;
  speedwantE=40;
//  car_stop(1);
//  add_count(cut_2,200);

  updata_var(VAR4);
  tongbu[4]=10;
}
//-------------------------------------------------
//                   弯道进入直道时超车
//输入
//输出
//功能 弯道刚进入直道时超车
//--------------------------------------------------
uint8 wrz_chao( uint8 cut )
{
  static uint8 z=0,h=0,a=0;
  uint8 mid=64;

  if(var4==0 )     //只有先识别的前车才停下
  {
    h=0;
    a=0;
//    used_length=30;
    if((L_wrz_flag==1 || L_wrz_flag==2) && chao_zhi==1 )   //前车左弯进直道靠左停
    {
      if(wrz_distance>15 && z==0 )    //前车在指定位置停车
      {
        z++;
        var4=1;         //并且直道超车标志同步（表示正在切换，切换完成后清零）
        zhi_chao_deal();
      }
    }
    else if((R_wrz_flag==1 || R_wrz_flag==2) && chao_zhi==1 )
    {
      if(wrz_distance>15 && z==0 )
      {
        z++;
        var4=2;         //并且直道超车标志同步（表示正在超车，超车完成后清零）
        zhi_chao_deal();
      }
    }

    if((L_wrz_flag==1 || L_wrz_flag==2) && chao_zhi==0 )   //后车超过去之后清直道标志
    {
      L_wrz_flag=0;
    }
    else if((R_wrz_flag==1 || R_wrz_flag==2) && chao_zhi==0 )
    {
      R_wrz_flag=0;
    }
  }
  else
  {
    if(chao_zhi==1)   //停下的车
    {
      z=0;
      if(wrz_distance>45)
      {
        car_stop(1);
      }

      if( ( ( leftval==0 && rightval==0 && s_distance<550 && s_distance>10)) && h==0)    //前车停车等待超车完成时若超声波能检测到数据，证明超车完成,然后向前车发数据清标志
      {    //road_count_chao>10 ||
        chao_zhi=0;
        h++;
        wrz_distance=0;
        L_wrz_flag=0;
        R_wrz_flag=0;

        var4=0;         //并且环道切换标志同步（表示正在切换，切换完成后清零）
        updata_var(VAR4);
        tongbu[4]=10;

        if(var1==1) var1=0;   //在这切换前后车
        else var1=1;
        updata_var(VAR1);
        tongbu[1]=10;

        car_start(1);
      }
    }
    else           //超过去的车
    {
      if( R_wrz_flag==0 && L_wrz_flag==0 && var4==1 && a==0 )   //后车超过去之后清直道标志
      {
        R_wrz_flag=1;
        a++;
      }
      else if( L_wrz_flag==0 && R_wrz_flag==0 && var4==2 && a==0 )
      {
        L_wrz_flag=1;
        a++;
      }
    }

  }

  return mid;
}

