#include "common.h"
#include "include.h"

/*
 ��1�� ���ʹ��ͼ�������������ÿ�η���֮ǰ�ȼ��ֱ����ͼ��Ȼ����ֱ�����������֪��
�������Ӧ��ͼ����������������Ϳ��Ա���ÿ������ͷ�ƶ���ɵĽ������������仯��
�����൱�ڽ�MATLAB�ļ��㹤��������Ƭ������ɣ�
 ��2�� ����ͷ���߸���������Ե���������ߣ�����ֻҪ��ÿ������֮ǰ����������׼��OK�ˣ�
 ��3�� ��ͨ����������֪�������ߣ�Ȼ����ݱ���б�ʵĹ��ɽ��в���
�����ǵ�����б��һ�㲻�ᷢ���ر����Եı仯��ֻ��ʮ�ֺ�ֱ���ǻ���Υ�����ɵ�������֣�
 ��4�� ʮ�ֲ���Ҳ����������������ֱ�Ӳ��ߣ�����ʶ��ʮ�֡�
����Ҫʶ��ʮ��Ҳ��������ʮ�ֵ�ֱ����������ʮ��ʶ��ֱ��ͬ��


*/

uint8 hehe=0,hehe1=0;
uint8 single_in;

int8 last_left_point[70];
int8 last_right_point[70];
int8 last_mid_point[70];
/*����б���е�*/
uint8 X_point[70];
uint8 Y_point[70];

uint8 left_line_first;
uint8 right_line_first;
uint8 left_line_first_flag;
uint8 right_line_first_flag;
#define l_judge_first_line(flag,y) flag==0?(left_line_first=y):(left_line_first=left_line_first)
#define r_judge_first_line(flag,y) flag==0?(right_line_first=y):(right_line_first=right_line_first)

#define Normal_mid_line(x)   ABS(line[x].mid_line_new-64)<=5&&ABS(line[x+1].mid_line_new-64)<=5&&ABS(line[x+2].mid_line_new-64)<=5&&ABS(line[x+3].mid_line_new-64)<=5&&ABS(line[x+4].mid_line_new-64)<=5

uint8  used_length=69;                   //��Ч��
int16 last_mid_first=64;    //ÿ��ͼƬ������һ�е�

line_info  line[line_num];            //��Ȼ�ɼ���120��,��ֻ��80�� ��ѹ֮���ͼ�����128*80��
uint8  all_left_line[line_num][20];
uint8  all_right_line[line_num][20];

//uint8 Tourner_used_length;//  Tourner��ת����
//uint16 Tourner_correct;   //correct��ת����

//extern int16 v_mid;//����
//extern float last_mid_line;//�ϴ�����
//��������
 uint8 lost_count;
 uint8 left_lost_count;
 uint8 right_lost_count;
 uint8 all_normal_count;
 uint8 all_normal_count_huan;
 uint8 road_wide_count_huan;
 uint8 road_count_chao;
 uint8 xia_no=0;
#if  ( CAR_MODEL==1 )
        uint8 follow_huan=1;    //1 ΪѰ���ߣ�0ΪѰ����
#elif (CAR_MODEL==2 )
        uint8 follow_huan=1;    //1 ΪѰ���ߣ�0ΪѰ����
#elif (CAR_MODEL==3 )
        uint8 follow_huan=1;    //1 ΪѰ���ߣ�0ΪѰ����
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


////�ع�
//uint8 left_line_first;
//uint8 right_line_first;
//#define l_judge_first_line(flag,y) flag==0?(left_line_first=y):(left_line_first=left_line_first)       //�������������ִ�У�ǰ����䣬�����ִ�У�������
//#define r_judge_first_line(flag,y) flag==0?(right_line_first=y):(right_line_first=right_line_first)
//#define L_Tourner(x)  line[x-1].line_case_mode==left_lose_right_momal  && line[x-2].line_case_mode==left_lose_right_momal && line[x-3].line_case_mode==left_lose_right_momal && line[x-4].line_case_mode==left_lose_right_momal && line[x-5].line_case_mode==left_lose_right_momal && left_line_first!=x && line[x].line_case_mode!=all_lose
//#define R_Tourner(x)  line[x-1].line_case_mode==left_normal_right_lose && line[x-2].line_case_mode==left_normal_right_lose && line[x-3].line_case_mode==left_normal_right_lose && line[x-4].line_case_mode==left_normal_right_lose && line[x-5].line_case_mode==left_normal_right_lose && right_line_first!=x && line[x].line_case_mode!=all_lose


//��һ�м��
//int16 last_mid;
//float last_mid_line;//�ϴ�����
//uint8 lost_count;
//uint8 left_lost_count;
//uint8 right_lost_count;
//uint8 all_normal_count;

//�������
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
//�������
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

//////////////////////////ʮ��
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
int16 DD_wide[70]={0};                  //΢�ֵ�΢��
int16 DD_left[70]={0};
int16 DD_right[70]={0};
float D_wide_add;
int16 DD_mid_line_add;
float last_mid_line;//�ϴ�����

uint8 *pimg;
uint8 left_first_lose_line_flag;
uint8 left_first_lose;
uint8 right_first_lose_line_flag;
uint8 right_first_lose;


uint16 road_wide_add;
uint8  road_wide_use;
uint16 road_wide_normal;
int16 last_mid;
uint32 shizi_new;     //ʮ���µ��㷨��ͳ�ư׵�������ֵ��Ϊ����ͼ����ֵ

uint8 fiag_huan_r,fiag_huan_l,fiag_huan;
/*******************************************************************************
* Function Name: median_line_extract();
* Input Param  : src       img���飬160*120
* Output Param : ��
* Description  : ͼ����������ȡ������
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
    lost_count=0;//���㶪ʧ����
    left_lost_count=0;
    right_lost_count=0;
    all_normal_count=0;
    left_add_point_count=0;
    right_add_point_count=0;
    left_line_first_flag=0;
    right_line_first_flag=0;
    single_in=0;              //����ı�Ϊ1��
    //��������
    road_count_chao=0;
    all_normal_count_huan=0;
    road_wide_count_huan=0;

    uint8 fiag_huan_r=0;
    uint8 fiag_huan_l=0;
    uint8 budiu=0;
    uint8 huan_l_x=0,huan_r_x=0,huan_kuan=0,huan_kuan1_l=0,huan_kuan1_r=0,huan_loca=0;
    uint8 h_jiao=0,h_jiao_x=0,h_jiao6=0,h_jiao_x6=0,h_jiao_cnt=0;
    float slope=0;   //����б��
    int16 last_dis_x=0,dis_x=0,dis_x_cnt=0,last_zuo_x=0,last_you_x=128;
//    Search_all_boundary( src );

	/*���õ�һ�е�ƽ��ֵ��Ϊ��ʼ���ߵ����*/
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
	if(last_mid==0)               //�����һ��ȫΪ�ھͽ���ֵ��Ϊ64
		last_mid=64;

    for(i=0;i<line_num;i++)             //�����ѭ��
     {

        pimg=src+(line_num-1-i)*128;    //���Ŵ����������һֱɨ��������
//        static uint16 hang_white_num,hang_white_sum;
//		/*����ÿһ�а׵������õ���ÿһ���е�*/            //��ôȥ��?
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

        for(j=last_mid;j>0;j--)                    //ɨ����   //ÿһ�ζ����м俪ʼ
         {
            tmpsrc=*(pimg+j);
            if(tmpsrc==0)
//              if(0==*(pimg+j-1) && 255==*(pimg+j)) //�׺�����
             {
                line[i].left_line=j;
                line[i].left_line_unrepiar=j;
                line[i].left_line_state=normal;
                left_case=1;
                //����һ���ڵ�
                l_judge_first_line(left_line_first_flag,i);
                left_line_first_flag=1;
                break;
             }
         }
        //��ߺ���û�ҵ�
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

        for(j=last_mid;j<127;j++)              //ɨ����
         {
            tmpsrc=*(pimg+j);
            if(tmpsrc==0)
//            if(255==*(pimg+j) && 0==*(pimg+j+1))//�׺�����
             {
                line[i].right_line=j;
                line[i].right_line_unrepiar=j;
                line[i].right_line_state=normal;
                right_case=1;
                //����һ���ڵ�
                r_judge_first_line(right_line_first_flag,i);
                right_line_first_flag=1;
                break;
             }
         }
        //�ұߺ���û�ҵ�
        if(j==127)                                  //һ���Ǵ�0������127
         {
            right_case=0;
            line[i].right_line_state=lose;
            if( right_first_lose_line_flag==0)
             {
                right_first_lose=i;
                right_first_lose_line_flag=1;
             }
         }
        //ȷ���������
        switch(left_case-right_case)      //�����Ƕ�ÿһ�ж�����һ��
         {
          case 0:             //���ұ��߶�ɨ��
            line[i].line_case_mode=all_normal;
            all_normal_count++;
            if(i>5) all_normal_count_huan++;
            break;
          case 1:            //ɨ������ߣ���ʧ�ұ���
            line[i].line_case_mode=left_normal_right_lose;
            line[i].right_line=128;
            line[i].right_line_unrepiar=128;
            right_lost_count++;
            break;
          case 2:           //��ʧ����ߣ�ɨ���ұ���
            line[i].line_case_mode=left_lose_right_normal;
            line[i].left_line=0;
            line[i].left_line_unrepiar=0;
            left_lost_count++;
            break;
          case 3:          //���ұ��߾���ʧ
            line[i].line_case_mode=all_lose;
            line[i].left_line=1;
            line[i].right_line=127;
            line[i].left_line_unrepiar=1;
            line[i].right_line_unrepiar=127;
            lost_count++;
            break;
         }


        /*  ������ȸ�ֵ  */
        line[i].road_wide=line[i].right_line-line[i].left_line;  //�ұ�����������������
        /////////////////////////////////////////////////////////
        for(j=0;j<20;j++)      //������������漸�е�������ȣ�������ʱ����
            {
              if( line[j].road_wide<50  )
              {
                road_count_chao++;
              }
            }





///////////////////////////   �жϻ���     /////////////
        uint8 zuo=0,you=0,zuo_x=0,you_x=0,zuo1=0,you1=0,you2_x=0,you2=0,zuo2=0,zuo2_x=0;

// ʶ�𻷵�
        if( fiag_huan==0 )
        {
          //////////////////// ����ǰֱ���ĳ��� ////////////////////
          if( i<10 && budiu==0)  //���뻷��֮ǰ��ֱ��
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
            //////////////////////�ж��������ĳ���
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

            if( huan_kuan==1)     //�ҹյ�  ����������һ��ֱ�ǵļ�
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
                  huan_kuan=2;  //�������߱��ļ����ĺܽ�
                  break;
                }
                /////////////����ǰֻ�ܿ���һ���Ҽ�ǵ����
                if( j>10 && huan_kuan1_r==0 && huan_kuan1_l==0 && line[j].right_line_unrepiar<125
                   &&  line[j].right_line_unrepiar>line[j-2].right_line_unrepiar  && line[j].line_case_mode==left_lose_right_normal
                     &&  line[j-2].right_line_unrepiar>line[j-4].right_line_unrepiar && line[j-4].line_case_mode==left_lose_right_normal
                       &&  line[j-4].right_line_unrepiar>line[j-6].right_line_unrepiar && line[j-6].line_case_mode==left_lose_right_normal
                         && (line[j-8].right_line_unrepiar<=line[j-10].right_line_unrepiar) )
                {
                  huan_kuan1_r=1;
                  huan_loca=j-6;
                }
                /////////////����ǰֻ�ܿ���һ�����ǵ����
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
            //////////////////////  ֻ��һ�����ʱ��������Բ����ʶ��
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
                      if(0==*(pimg1+n-1) && 255==*(pimg1+n)  && zuo_mini==0) //�ڰ�����
                      {
                        zuo_mini=n;
                      }
                      if(255==*(pimg1+n) && 0==*(pimg1+n+1) && you_mini==0)//�׺�����
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
                        if(0==*(pimg1+n-1) && 255==*(pimg1+n)  && zuo_mini==0) //�ڰ�����
                        {
                          zuo_mini=n;
                        }
                        if(255==*(pimg1+n) && 0==*(pimg1+n+1) && you_mini==0)//�׺�����
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
                  if(var3==0  //�����������Ϊ����ǰ��ʶ�𳬳���־����ʼ�������Ҳ��ú��ٽ��룩
                     && chao_one==0    //Ϊ��ǰ��ֻ����һ���л���־����ֹͣ����ʱ��һֱ��������һֱ�л����������֮�������־
                       )   //&& sequence==1
                  {
                    cut_1++;
                    if(cut_1>200) cut_1=200;
                    chao_one=1;


                    var3=1;         //���һ����л���־ͬ������ʾ�����л����л���ɺ����㣩
                    updata_var(VAR3);
                    tongbu[3]=10;

                    if(var1==1) var1=0;   //�����л�ǰ��
                    else var1=1;
                    updata_var(VAR1);
                    tongbu[1]=10;


                  }
                  break;
                }
              }


            }
            //////////////////////  ��������Բ����ʶ��
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
                  if(var3==0  //�����������Ϊ����ǰ��ʶ�𳬳���־����ʼ�������Ҳ��ú��ٽ��룩
                     && chao_one==0    //Ϊ��ǰ��ֻ����һ���л���־����ֹͣ����ʱ��һֱ��������һֱ�л����������֮�������־
                       )   //&& sequence==1
                  {
                    cut_1++;
                    if(cut_1>200) cut_1=200;
                    chao_one=1;

                    var3=1;         //���һ����л���־ͬ������ʾ�����л����л���ɺ����㣩
                    updata_var(VAR3);
                    tongbu[3]=10;

                    if(var1==1) var1=0;   //�����л�ǰ��
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
      //����ʶ�����






		/***************************ʮ�ֱ��߲���*************************/
		static uint8 shi_zi_baochi=0;
		if(last_lost_count>25)
		 {
			if(line[0].line_case_mode==left_lose_right_normal&&line[i].line_case_mode==all_lose)		  //ʮ�ִӵ�һ�п�ʼ��
			 {
				line[i].right_line=line[0].right_line-1;
			 }
			if(line[0].line_case_mode==left_normal_right_lose&&line[i].line_case_mode==all_lose)		  //ʮ�ִӵ�һ�п�ʼ��
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
					never_shi_zi_count=1;      //������������
				}

			 }
		 }
		if(shi_zi_count==1)
		    shi_zi_baochi=0;    //��ʮ�ֽ����ֱ�־λ����
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
		//5.������ȴ���
        wide[i]=line[i].right_line-line[i].left_line;     //��������� ��Ȳ������µ� Dwide ����
        real_wide[i]=wide[i]*X_change[69-i];              //������´����������Ǵ����浽�������е�
        if(i>0)                                           //������ֱ仯��
        {
            D_left[i]=line[i].left_line-line[i-1].left_line;      //΢�ִ�1��ʼ   ��ô΢�ֵ�΢�־�Ӧ�ô�2��ʼ
            D_right[i]=line[i].right_line-line[i-1].right_line;
            D_wide[i]=wide[i]-wide[i-1];
            D_real_wide[i]=real_wide[i]-real_wide[i-1];
			if(i>1)
			{
				DD_wide[i]=D_wide[i]-D_wide[i-1];
				DD_left[i]=D_left[i]-D_left[i-1];
				DD_right[i]=D_right[i]-D_right[i-1];
				DD_mid_line_add+=DD_left[i];               //��ʱ���ۼ����ߵ�΢�ֵ�΢��
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
		    //7.���� �յ� ������ȱ仯�Ƚϴ� ʮ��
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
//				real_wide[i]=wide[i]*X_change[69-i];                     //ʮ�����Ҷ��߻�Ӱ���µ����� �����¼���real��wide
//				D_wide[i]=wide[i]-wide[i-1];
//				D_real_wide[i]=real_wide[i]-real_wide[i-1];              //�����е�΢����ȫ�����¼��㣬�����߶��ı���
//				D_left[i]=line[i].left_line-line[i-1].left_line;
//				D_right[i]=line[i].right_line-line[i-1].right_line;
//			}
        }
         if(i>3&&single_in==0)     //�ҹյ�  ����������һ��ֱ�ǵļ�
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
		 /*���¼����������*/
//         line[i].road_wide=line[i].right_line-line[i].left_line;  //�ұ�����������������

        //��ֹһ��ͼƬ���ֶ�����������
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

        //���߲������            //�������㷨�д��Ľ���Ŀǰ��ʮ�ֹյ��ⲻ�ã��������У�Ҫ�������ı䣬ͬʱ�����㷨��ͷ����
        if(i==0)
         {
            line[i].mid_line_new= (line[i].left_line+ line[i].right_line)/2;
         }
        else
        {
          if(line[i].left_line < 1)              //������ ������   �����Ǹ���������һ�ߵı仯�����ڲ���  Ч������ ���ܸ������й�
          {
            line[i].mid_line_new=line[i-1].mid_line_new+(line[i].right_line-line[i-1].right_line);
            //			   line[i].mid_line_new=line[i-1].mid_line_new+(line[i].right_line-line[i-1].right_line);   //��Ϊ��תת̫������߽綪�߼�1
            //			   line[i].mid_line_new=line[i].left_line+line[i].road_wide/2;
          }
          else if(line[i].right_line >127)         //�����ߣ�������    //���äĿ����������Ȳ�����ʮ�ֻ�����һЩ���⣬��ʱ��һ�����
              line[i].mid_line_new=line[i-1].mid_line_new+(line[i].left_line-line[i-1].left_line);
          //			      line[i].mid_line_new=line[i].right_line-line[i].road_wide/2;
          else
              line[i].mid_line_new= (line[i].left_line+ line[i].right_line)/2;
        }

        /*���������ͼ��У���Ϸ���Ƭ�׶�ʧͼ��*/
        if(Normal_mid_line(0)&&shi_zi_count==0&&last_lost_count>=10)    //�����ʮ��ͼ������ֵ����
        {
            if((line[i].line_case_mode==all_lose))
            {
				line[i].mid_line_new=64;
				shi_zi_baochi=1;
            }
		}
		if(shi_zi_baochi==1&&shi_zi_count==0&&last_lost_count>=10&&(ABS(line[0].mid_line_new-64)<=5||ABS(line[1].mid_line_new-64)<=5))       //shi_zi_count==0 ������ʮ��  û�������ط����õ�
		{
            if((line[i].line_case_mode==all_lose))
            {
				line[i].mid_line_new=64;
            }
		}
		/*end���������ͼ��У���Ϸ���Ƭ�׶�ʧͼ��*/

        /////////////  ���������־����������  //////////////////////
     if(fiag_huan==1)
     { //��ʶ����������ļ��
       if( follow_huan )     // 1ΪѰ����,0ΪѰ����
       {
         if( i==10 && xia_no==0  && line[i].right_line_state== lose && line[i-8].right_line_state== lose
            &&(line[i].left_line_unrepiar < line[i-2].left_line_unrepiar ||  line[i-2].left_line_unrepiar <8 )
               &&(line[i-2].left_line_unrepiar < line[i-4].left_line_unrepiar ||  line[i-4].left_line_unrepiar <8 )
              )
           xia_no=1;
         if( xia_no==1  && i>20 && i<40
            &&  line[i].left_line_state== normal &&(line[i].left_line_unrepiar > line[i-2].left_line_unrepiar  )
              &&  line[i-3].left_line_state== normal &&(line[i-2].left_line_unrepiar > line[i-4].left_line_unrepiar  )
                )   // ���Ҽ��ǰ�벿��
         {
           h_jiao=1;
           h_jiao_x=i;
         }
         if( xia_no==1 &&  h_jiao==1 && (i-h_jiao_x) < 20  && i<50
            && line[i].left_line_state== normal &&(line[i].left_line_unrepiar < line[i-2].left_line_unrepiar  )
              && line[i-3].left_line_state== normal &&(line[i-2].left_line_unrepiar < line[i-4].left_line_unrepiar  )
                && line[i-6].left_line_state== normal &&(line[i-4].left_line_unrepiar < line[i-6].left_line_unrepiar  )
                  )      //���Ҽ�Ǻ�벿��
         {
           xia_no=2;       // ���ҵ���ʶ��Ϊ��� ��ǰ��ͣ��
           h_jiao_cnt=1;   //ÿ�ζ�ֻʶ��һ�μ��
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
                  )   // ���Ҽ��ǰ�벿��
           {
             h_jiao=1;
           }
           if( h_jiao==1 && i<50
              && line[i].left_line_state== normal &&(line[i].left_line_unrepiar < line[i-2].left_line_unrepiar  )
                && line[i-3].left_line_state== normal &&(line[i-2].left_line_unrepiar < line[i-4].left_line_unrepiar  )
                  )   // ������һ���ǣ��Ҳ�����֤���Ѿ�����ǣ������֮�����������������
           {
             h_jiao_cnt=1;
           }
           else
           {
             if(i>47)
               xia_no=4;   //һֱ������Ҳ������Ҳ��������
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
                )   // ���Ҽ��ǰ�벿��
         {
           h_jiao=1;
           h_jiao_x=i;
         }
         if( xia_no==1 &&  h_jiao==1 && (i-h_jiao_x) < 20  && i<50
            && line[i].right_line_state== normal &&(line[i].right_line_unrepiar > line[i-2].right_line_unrepiar  )
              && line[i-3].right_line_state== normal &&(line[i-2].right_line_unrepiar > line[i-4].right_line_unrepiar  )
                && line[i-6].right_line_state== normal &&(line[i-4].right_line_unrepiar > line[i-6].right_line_unrepiar  )
                  )      //���Ҽ�Ǻ�벿��
         {
           xia_no=2;       // ���ҵ���ʶ��Ϊ��� ��ǰ��ͣ��
           h_jiao_cnt=1;   //ÿ�ζ�ֻʶ��һ�μ��
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
                  )   // ���Ҽ��ǰ�벿��
           {
             h_jiao=1;
           }
           if( h_jiao==1 && i<50
              && line[i].right_line_state== normal &&(line[i].right_line_unrepiar > line[i-2].right_line_unrepiar  )
                && line[i-3].right_line_state== normal &&(line[i-2].right_line_unrepiar > line[i-4].right_line_unrepiar  )
                  )   // ������һ���ǣ��Ҳ�����֤���Ѿ�����ǣ������֮�����������������
           {
             h_jiao_cnt=1;
           }
           else
           {
             if(i>47)
               xia_no=4;   //һֱ������Ҳ������Ҳ��������
           }
         }
       }
       //�������ļ��֮�������ֱ�������������־
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
               if(front_car==1 && var3==1  && chao_one==0 )   //������ɺ��ǰ����ȥ��ʱ�򽫻���������־���㲢ͬ�����󳵣��ú󳵳���
               {   //                       ��ֹǰ����ͣ�»�δ�����������������־�ˣ����԰��ٶȱ�Ϊ���������ı�־
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
     // �廷������
       //////////////////////////////  Ѱ������ʻ  ////////////////////////////
       if( fiag_huan )
       {
         if( follow_huan )    //Ѱ����
         {
           if(line[i].right_line_unrepiar>line[i].left_line_unrepiar        // ������ҵ�����������
              && (line[i].left_line_unrepiar+distance[i]+10) > line[i].right_line_unrepiar )
           {
             if(line[i].left_line_state==normal)   //��������߾�ֱ�������ұ���
             {
                line[i].left_line  = line[i].left_line_unrepiar;
                line[i].right_line = line[i].right_line_unrepiar;
             }
             else   //����͸������߲�����
             {
                line[i].left_line  = line[i].right_line_unrepiar-distance[i];
                line[i].right_line = line[i].right_line_unrepiar;
             }
           }
           else if(line[i].left_line_state==lose) //���û�����������ߣ�Ҳ�Ҳ������߾�ֱ�Ӹ�������
           {
                line[i].left_line=0;
                line[i].right_line=64;
           }
           else   // ����������ߣ�û�������ұ��ߵ�������������߲�����
           {
                line[i].left_line  = line[i].left_line_unrepiar;
                line[i].right_line = line[i].left_line_unrepiar+distance[i];
           }
         }
         else    //Ѱ����
         {
           if(line[i].left_line_unrepiar<line[i].right_line_unrepiar        // ������ҵ�����������
              && (line[i].left_line_unrepiar+distance[i]+10) > line[i].right_line_unrepiar )
           {
             if(line[i].right_line_state==normal)   //��������߾�ֱ�������ұ���
             {
                line[i].left_line  = line[i].left_line_unrepiar;
                line[i].right_line = line[i].right_line_unrepiar;
             }
             else   //����͸������߲�����
             {
                line[i].left_line  = line[i].left_line_unrepiar;
                line[i].right_line = line[i].left_line_unrepiar+distance[i];
             }
           }
           else if(line[i].right_line_state==lose) //���û�����������ߣ�Ҳ�Ҳ������߾�ֱ�Ӹ�������
           {
                line[i].left_line=64;
                line[i].right_line=128;
           }
           else   // �������ұ��ߣ�û����������ߵ�������������߲����� �������
           {
            line[i].left_line  = line[i].right_line_unrepiar-distance[i];
            line[i].right_line = line[i].right_line_unrepiar;
           }
         }
         line[i].mid_line_new= (line[i].left_line+ line[i].right_line)/2;
       }
       /////////////////////////////////////////////////////////////////////////

                last_mid=line[i].mid_line_new;                      //��ÿһ�е���ֵ��������һ������ɨ�ߵ���ֵ

		/*������һ�ε����ұ���*/                      //ÿһ֡�����ұ���ֵ�����浽��һ֡ ��Ϊһ���Ĳο�
        last_left_point[i]=line[i].left_line;
		last_right_point[i]=line[i].right_line;
		last_mid_point[i]=line[i].mid_line_new;

		if(line[i].mid_line_new<0)                //����޷�ֵ���
         {
            line[i].mid_line_new=0;
         }
        if(line[i].mid_line_new>129)
         {
            line[i].mid_line_new=129;
         }

        /*ʮ������Բ����Ϊ�в������߻����ұ߽���ұ�*/
//		if(i>50&&(line[i].mid_line_new-line[i-1].right_line>1||line[i].mid_line_new-line[i-2].right_line>1))
//			line[i].mid_line_new=255;
		/*�������ߵ㣬����б��*/
		X_point[i]=line[i].mid_line_new;    //�����i�Ǵ���������
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
        //�Կ�Ƚ����ۼ�

        //����������Ч��
        if( follow_huan )  //1ΪѰ���ߣ�0ΪѰ����
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
            //������ʱ�����ж���û���ұ����𽥼�С
            //�����������������Ѱ���ұ���������
            //�˹����б�֤�ұ�������
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

        if(((line[i].right_line-line[i].left_line)<15)&&i>=20)      //�������Ч��
         {                                               //��������жϣ���Ȼ�����ߺ������� ���ݻᱻ��Ч�нض�
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
				line[used_length].mid_line_new=255; 	 //�Ǻ���������Ч�е���ֵ����255���������ط����õ�	255�ǲ�����СҺ������ʾ��
				DD_mid_line_add=temp_dd_mid_line_add;
				temp_dd_mid_line_add=0;
//			}

		/*����ʮ�ֲ���ÿһ֡�����㰴�д����ͼ��*/
		if(lost_count>=20)
		{
		    uint32 lie_num=0;
	        uint32 lie_sum_add=0;
			for(j=0;j<127;j++)		   //�����ѭ��  �����ѭ��   �����ھ�����
			{
				pimg=src+(line_num-1)*128+j;	//���һ����������
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
			shizi_new=lie_sum_add/lie_num;      //�����˵���һ��ʹ��
			lie_sum_add=0;			 //��0  һ������
			lie_num=0;
		}

		/*ʮ�ֱ���Ч�нضϵ���ͻ������*/
        if(used_length<60&&lost_count>=15&&last_lost_count>=15)         //�����Ǵ����ʮ�ֵ�һ���������  ֻ��һ����
        {                                                          //�ﲻ��ʮ�ֵ���������ǿ�����ˣ�һ��Ϊ��ʮ�ֵ�һ�������
			if(shizi_new-64>0)          //��Ϊʹ��Ӵ�һ��
				shizi_new+=10;
			if(shizi_new-64<0)
				shizi_new-=10;
			for(i=0;i<line_num-1;i++)
			{
				line[i].mid_line_new=shizi_new; 		   //���õ������߸ı�
			}
//				used_length=70;
			line[used_length].mid_line_new=shizi_new;
			line[70].mid_line_new=255;
		}
            return;                                  //��������ѭ��
         }
         used_length=70;
 }
}

//-------------------------------------------------
//                 �߽���ȡ
//����    uint8 *src    ����ͼƬ
//���    uint8 all_left_line[][] ������߽�
//        uint8 all_left_line[][] �����ұ߽�
//���� ��ȡͼ�������еĺڰ�����Ͱ׺�����
//����
//����
//--------------------------------------------------
void Search_all_boundary( uint8 *src )
{
    uint8 i,j=0,k=0;
    for(i=0;i<line_num;i++)             //�����ѭ��
    {
      pimg=src+(line_num-1-i)*128;    //���Ŵ����������һֱɨ��������
      for(j=1;j<127;j++)
      {
        if(0==*(pimg+j-1) && 255==*(pimg+j)) //�ڰ����� �����ߣ�
        {
          all_left_line[i][k]=j;
          k++;
        }
        if(255==*(pimg+j) && 0==*(pimg+j+1)) //�׺����� �����ߣ�
        {
          all_right_line[i][k]=j;
          k++;
        }
      }
    }
}

//-------------------------------------------------
//                 �����߽�ʶ��
//���� uint8 all_left_line[][],all_right_line[][]  ����
//���
//���� �����е��������ҵ���������������ĵ���Ϊ�߽�
//����
//����
//--------------------------------------------------
void SideDdistinguish()
{
//    for(j=0;j<20;j++)
//    {
//        if(all_right_line[i][j]==0)  break;      //û�б߽�ֱ�ӷ���
//        if( (all_right_line[i][j]>last_mid) )     //�ұ߽�϶��ô�����һ�ε�����
//        {
//          line[i].right_line=all_right_line[i][j];
//          line[i].right_line_unrepiar=all_right_line[i][j];
//          line[i].right_line_state=normal;
//          right_case=1;
//          break;   //�ҵ��ĵ�һ���߽������ӽ����ߵı߽磨����ֱ�ӷ��ؼ��ɣ�
//        }
//    }

}

