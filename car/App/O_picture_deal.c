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

#define add_count(number,x) number<x?(number++):(number=x)              //3Ŀ���㣺���ʽ��Ϊ�棬���ʽ��ֵ����ð��ǰ��ʽ��


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
 uint8 road_wide_chao[20];
 uint8 xia_no=0;
 uint8 budiu=0;
 uint8 huan_l_x=0,huan_r_x=0,huan_kuan=0,huan_kuan1_l=0,huan_kuan1_r=0,huan_loca=0;
 uint8 h_jiao=0,h_jiao_x=0,h_jiao6=0,h_jiao_x6=0,h_jiao_cnt=0;
 uint8 follow_huan;    //1 ΪѰ���ߣ�0ΪѰ����


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

/***************ֱ���жϱ���****************/
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
//������ʶ��
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
//����    ��¼�����漸�к������漸�е�������ȣ������µ��жϣ�
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
//����    ��¼ͣ��˲�������漸�е�������Ȳ�������������ȱȽ��Ƿ��խ������ֱ��������
//��� road_count_chao ��խ������
//------------------------------------------------------------------------------
void recoed_xia_wide( uint8 i ,uint8 wider)
{
  uint8 j=0;
  static uint8 ci=0;
  if(chao_zhi==1 && ci==0 && i==20 )    //��¼��ͣ��ʱ�����漸�е��������     && leftval<=5 && rightval<=5
  {
    ci++;
    for(j=0;j<20;j++)
    {
      road_wide_chao[j]= line[j].road_wide;
    }
  }
  if( line[i].road_wide < ( road_wide_chao[i]-wider) && i<20 && chao_zhi==1 )      //������������漸�е�������ȣ�ֱ��������ʱ����
  {                                                            //  && leftval<=5 && rightval<=5
    road_count_chao++;
  }
}

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
    //��������
    road_count_chao=0;
    all_normal_count_huan=0;
    road_wide_count_huan=0;

    fiag_huan_r=0;
    fiag_huan_l=0;
    budiu=0;
    huan_l_x=0,huan_r_x=0,huan_kuan=0,huan_kuan1_l=0,huan_kuan1_r=0,huan_loca=0;
    h_jiao=0,h_jiao_x=0,h_jiao6=0,h_jiao_x6=0,h_jiao_cnt=0;
    float slope=0;   //����б��
    int16 last_dis_x=0,dis_x=0,dis_x_cnt=0,last_zuo_x=0,last_you_x=128;

    /***************ֱ���жϱ���****************/

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

    //  ��¼ÿ��ͼƬ������ʮ�е�ƽ��������Ⱥ�������ʮ�е�ƽ��������������ж��µ�
    record_side_wide_part();
    //////////////////////////////////////////////////////////
    for(i=0;i<line_num;i++)             //�����ѭ��
     {
        pimg=src+(line_num-1-i)*128;    //���Ŵ����������һֱɨ��������

        Search_all_boundary( pimg,i );  //��������ȡ���ұ��ߣ������д���
        recoed_xia_wide( i ,15);        //��¼�����漸�е������������ֱ������
// ʶ�𻷵�
        if( fiag_huan==0 )  //&& front_car==1
        {
           huan_judge( src , i );
        }
//����ʶ�����
//����ֱ��״̬ʶ��
        if( Ramp_flag!=0 )
        {
          if(Ramp_tong==1)
          {
            if((Total_distance-Ramp_Len_x)>200 )   //��ʾ��ʶ���µ��ĳ�
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
              if((Total_distance-Ramp_Len_x)>250 )   //��ʾͨ�����ߴ���ʶ���µ��ĳ�
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
          zhidao( i );     //ʶ��ֱ��
        }

//����ֱ��״̬ʶ�����

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


        ////�뻷����־
        huan_clear( i );

        //////////////////////////////  ����Ѳ��  ////////////////////////////
        huan_along( i );

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
        if(((line[i].right_line-line[i].left_line)<15)&&i>=20 && bujiezhi==0 )      //�������Ч��
         {                                               //��������жϣ���Ȼ�����ߺ������� ���ݻᱻ��Ч�нض�
				used_length=i;
				line[used_length].mid_line_new=255; 	 //�Ǻ���������Ч�е���ֵ����255���������ط����õ�	255�ǲ�����СҺ������ʾ��
				DD_mid_line_add=temp_dd_mid_line_add;
				temp_dd_mid_line_add=0;
            return;                                  //��������ѭ��
         }
         used_length=70;

 }
}

//-------------------------------------------------
//                 �߽���ȡ
//����    uint8 *src    ����ת����ͼ���ַ
//���
//����   ������Ѱ��ͼ���е���߽���ұ߽磨�������κδ���
//--------------------------------------------------
void Search_all_boundary( uint8 *src ,uint8 i)
{
  uint8 tmpsrc,j;
  uint8 left_case,right_case;
if(*(src+last_mid)==255)                    //�е�Ϊ��ɫ
 {
  for(j=last_mid;j>0;j--)                    //ɨ����   //ÿһ�ζ����м俪ʼ
  {
    tmpsrc=*(src+j);
    if(tmpsrc==0)
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
    tmpsrc=*(src+j);
    if(tmpsrc==0)
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
 }
 else              //�е�Ϊ��ɫ  ���������м�ɨ��
 {
  for(j=0;j<127;j++)                    //ɨ����
  {
    tmpsrc=*(src+j);
    if(tmpsrc==255)
    {
      line[i].left_line=j-1;
      line[i].left_line_unrepiar=j-1;
      line[i].left_line_state=normal;
      //����һ���ڵ�
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

  for(j=127;j>0;j--)              //ɨ����
  {
    tmpsrc=*(src+j);
    if(tmpsrc==255)
    {
      line[i].right_line=j+1;
      line[i].right_line_unrepiar=j+1;
      line[i].right_line_state=normal;
      //����һ���ڵ�
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

  //ȷ���������
  switch(left_case-right_case)      //�����Ƕ�ÿһ�ж�����һ��
  {
  case 0:             //���ұ��߶�ɨ��
    line[i].line_case_mode=all_normal;
    add_count(all_normal_count,5);
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
}

//-------------------------------------------------
//                   ������Ǳ�������
//����
//���
//���� �����Ѿ�ʶ�����������ȡ����������ʶ��ʱ�ļ�¼��־�ȴ���
//--------------------------------------------------
void huan_mark_start()
{
  fiag_huan=1;
  add_count(hehe,200);
  //car_stop(1);
  //            tongbu[3]=1;
  if(var3==0  //�����������Ϊ����ǰ��ʶ�𳬳���־����ʼ�������Ҳ��ú��ٽ��룩
     && chao_huan==0    //Ϊ��ǰ��ֻ����һ���л���־����ֹͣ����ʱ��һֱ��������һֱ�л����������֮�������־
       )   //&& sequence==1
  {
    add_count(cut_1,200);
    chao_huan=1;

    var3=1;         //���һ����л���־ͬ������ʾ�����л����л���ɺ����㣩
    updata_var(VAR3);
    tongbu[3]=10;

    if(var1==1) var1=0;   //�����л�ǰ��
    else var1=1;
    updata_var(VAR1);
    tongbu[1]=10;
  }
}

//-------------------------------------------------
//                   �������Ǳ�������
//����
//���
//���� ��������֮�����������ȡ��������ʱ�������־�ȴ���
//--------------------------------------------------
void huan_mark_end()
{
  xia_no = 0;
  all_normal_count_huan=0;
  fiag_huan=0;
  //&& sequence==0
  if(front_car==1 && var3==1  && chao_huan==0 )   //������ɺ��ǰ����ȥ��ʱ�򽫻���������־���㲢ͬ�����󳵣��ú󳵳���
  {   //                       ��ֹǰ����ͣ�»�δ�����������������־�ˣ����԰��ٶȱ�Ϊ���������ı�־
    var3=0;
    updata_var(VAR3);
    tongbu[3]=10;
  }
}

//-------------------------------------------------
//                   �����ж�
//����
//���
//���� �жϻ���
//--------------------------------------------------
void huan_judge( uint8 *src ,uint8 i )
{
           uint8 tmpsrc,j;
           uint8  *pimg1;

          uint8 zuo=0,you=0,zuo_x=0,you_x=0,zuo1=0,you1=0,you2_x=0,you2=0,zuo2=0,zuo2_x=0,zhong2=0,zhong2_x=0;
//--1--//   -----�жϻ���ǰ��ֱ��-----------------------------
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
//--2--//   -----ֱ��֮�����ñ��-----------------------------
            //////////////////////�ж��������ĳ���
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
//--3--//   -----���֮�����ҵ�����ֱ�ǵļ�-----------------------------
            if( huan_kuan==1)     //�ҹյ�  ����������һ��ֱ�ǵļ�
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
//                  if((line[j-7].right_line_unrepiar==line[j-8].right_line_unrepiar))   //�������Ĳ��ֶ�����ȵģ�����Ƿ���ʮ��ʱ�ļ��
//                  {   //����Ѱ���������һ���ڵ�
//                    uint8 m,v;
//                    for(m=huan_r_x;m<(huan_r_x+5);m++)
//                    {
//                      pimg1=src+(line_num-1-m)*128;
//                      tmpsrc=*(pimg1+line[huan_loca].right_line_unrepiar - (j- huan_loca));
//                      for(v=(line[j-2].right_line_unrepiar-5);v<127;v++)
//                      {
//                        if(0==*(pimg1+v-1) && 255==*(pimg1+v) ) //�ڰ�����
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
//--4--//   -----�ҵ����֮���������ͼ���б������Ѱ������Բ��---------------
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
                  //����б��
                  for(f=0;f<huan_loca;f++)
                  {
                    X_buff[f]=line[f].right_line_unrepiar;
                    Y_buff[f]=f;
                  }
                  Regression_cal(X_buff,Y_buff,huan_loca-4);
                  //����
                  tmpsrc=*(pimg1+(uint8)(line[huan_loca-4].right_line_unrepiar + (float)(j- huan_loca)/(index_B+0.2)));  //��������ұ�ֱ�Ӽ���ȥ�ĵ�
                  if( tmpsrc ==0  )  // src+(line_num-1-i)*128;    && (line[huan_loca].left_line_unrepiar - (j- huan_loca))>10
                  {
                    you2=1;
                    you2_x=(uint8)(line[huan_loca-4].right_line_unrepiar + (float)(j- huan_loca)/(index_B+0.2)) ;
                  }
                  //����
                  tmpsrc=*(pimg1+(uint8)(line[huan_loca-4].mid_line_new + (float)(j- huan_loca)/(index_B-0.2)));  //��������ұ�ֱ�Ӽ���ȥ�ĵ�
                  if( tmpsrc ==0  )  // src+(line_num-1-i)*128;    && (line[huan_loca].left_line_unrepiar - (j- huan_loca))>10
                  {
                    zhong2=1;
                    zhong2_x=(uint8)(line[huan_loca-4].mid_line_new + (float)(j- huan_loca)/(index_B-0.2)) ;
                  }
                  if( ( you2==1 && zhong2==1)  )
                  {
                    for(n=zhong2_x+2;n<you2_x-2;n++)
                    {
                      if(255==*(pimg1+n) ) //�׵��˳�
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
                    //����б��
                    for(f=0;f<huan_loca;f++)
                    {
                      X_buff[f]=line[f].left_line_unrepiar;
                      Y_buff[f]=f;
                    }
                    Regression_cal(X_buff,Y_buff,huan_loca-4);
                    //zhao��
                    tmpsrc=*(pimg1+(uint8)(line[huan_loca-4].left_line_unrepiar + (float)(j- huan_loca)/(index_B-0.2)));  //��������ұ�ֱ�Ӽ���ȥ�ĵ�

                    //                    tmpsrc=*(pimg1+line[huan_loca].left_line_unrepiar + (j- huan_loca));
                    if( tmpsrc ==0 )  // src+(line_num-1-i)*128;
                    {
                      zuo2=1;
                      zuo2_x=(uint8)(line[huan_loca-4].left_line_unrepiar + (float)(j- huan_loca)/(index_B-0.2)) ;
                    }
                    //����
                    tmpsrc=*(pimg1+(uint8)(line[huan_loca-4].mid_line_new + (float)(j- huan_loca)/(index_B+0.2)));  //��������ұ�ֱ�Ӽ���ȥ�ĵ�
                    if( tmpsrc ==0  )  // src+(line_num-1-i)*128;    && (line[huan_loca].left_line_unrepiar - (j- huan_loca))>10
                    {
                      zhong2=1;
                      zhong2_x=(uint8)(line[huan_loca-4].mid_line_new + (float)(j- huan_loca)/(index_B+0.2)) ;
                    }
                    if( ( zuo2==1 && zhong2==1)  )
                    {
                      for(n=zuo2_x+2;n<zhong2_x-2;n++)
                      {
                        if(255==*(pimg1+n) ) //�׵��˳�
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
            //////////////////////  ��������Բ����ʶ��
            if(huan_kuan==2)
            {
              for(j=20;j<55;j++)
              {
                zuo1=0,zuo_x=0,you1=0,you_x=0;
                pimg1=src+(line_num-1-j)*128;
                                    //����б��
                    for(f=0;f<huan_l_x;f++)
                    {
                      X_buff[f]=line[f].left_line_unrepiar;
                      Y_buff[f]=f;
                    }
                    Regression_cal(X_buff,Y_buff,huan_l_x-4);
                    //zhao��
                    tmpsrc=*(pimg1+(uint8)(line[huan_l_x-4].left_line_unrepiar + (float)(j- huan_l_x)/(index_B-0.2)));  //��������ұ�ֱ�Ӽ���ȥ�ĵ�

                    //                    tmpsrc=*(pimg1+line[huan_loca].left_line_unrepiar + (j- huan_loca));
                    if( tmpsrc ==0 && j>(huan_l_x+5))  // src+(line_num-1-i)*128;
                    {
                      zuo1=1;
                      zuo_x=(uint8)(line[huan_l_x-4].left_line_unrepiar + (float)(j- huan_l_x)/(index_B-0.2)) ;
                    }
                                  //����б��
                  for(f=0;f<huan_r_x;f++)
                  {
                    X_buff[f]=line[f].right_line_unrepiar;
                    Y_buff[f]=f;
                  }
                  Regression_cal(X_buff,Y_buff,huan_r_x-4);
                  //����
                  tmpsrc=*(pimg1+(uint8)(line[huan_r_x-4].right_line_unrepiar + (float)(j- huan_r_x)/(index_B+0.2)));  //��������ұ�ֱ�Ӽ���ȥ�ĵ�
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
                      if(255==*(pimg1+n) ) //�׵��˳�
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
                      if(255==*(pimg1+n) ) //�׵��˳�
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
//                   �������
//����
//���
//���� ���������־�����жϽ�������������λ��
//--------------------------------------------------
void huan_clear( uint8 i )
{
  uint8 j;
        /////////////  ���������־����������  //////////////////////
     if(fiag_huan==1)
     {
       if( follow_huan )     // 1ΪѰ����,0ΪѰ����
       {
//--1--��Ѱ�����ҵ��뻷����Ǻ���������ұ߶��ߣ�
         if( i==10 && xia_no==0  && line[i].right_line_state== lose && line[i-8].right_line_state== lose
            &&(line[i].left_line_unrepiar < line[i-2].left_line_unrepiar ||  line[i-2].left_line_unrepiar <8 )
               &&(line[i-2].left_line_unrepiar < line[i-4].left_line_unrepiar ||  line[i-4].left_line_unrepiar <8 )
              )
         {
           xia_no=1;
         }
//--2--��Ѱ�����ҵ����������ļ��
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
           if(chao_huan==1 && var3==1 && huan_chao_cont==1)      //&& sequence==1
           {
             speedwantD=0;
             speedwantE=0;
           }
         }
//--3--��Ѱ��һֱ�����������ʱ��־
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
//--1--��Ѱ�ң����ҵ��뻷����Ǻ���������ұ߶��ߣ�
         if( i==10 && xia_no==0  && line[i].left_line_state== lose && line[i-8].left_line_state== lose
            &&(line[i].right_line_unrepiar > line[i-2].right_line_unrepiar ||  line[i-2].right_line_unrepiar >120 )
               &&(line[i-2].right_line_unrepiar > line[i-4].right_line_unrepiar ||  line[i-4].right_line_unrepiar >120 )
              )
         {
           xia_no=1;
         }
//--2--��Ѱ�ң����ҵ����������ļ��
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
           if(chao_huan==1 && var3==1  && huan_chao_cont==1)      //&& sequence==1
           {
             speedwantD=0;
             speedwantE=0;
           }
         }
//--3--��Ѱ�ң�һֱ�����������ʱ��־
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

//--4--�������ļ��֮�������ֱ�������������־
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
     // �廷������
}


//-------------------------------------------------
//                   ������Ѳ��
//����
//���
//���� �ڻ����зֱ����һ�߶������߶��ߺͲ���������µ�������
//--------------------------------------------------
void huan_along( uint8 i )
{
  uint8 j ;
  if( fiag_huan )
  {
    if( follow_huan )    //Ѱ����
    {
      if(line[i].right_line_unrepiar>line[i].left_line_unrepiar  && line[i].right_line_state==normal      // ������ҵ�����������
         && (line[i].left_line_unrepiar+distance2[i]+10) > line[i].right_line_unrepiar
            )
      {
        if(line[i].left_line_state==normal)   //��������߾�ֱ�������ұ���
        {
          line[i].left_line  = line[i].left_line_unrepiar;
          line[i].right_line = line[i].right_line_unrepiar;
          line[i].mid_line_new= (line[i].left_line+ line[i].right_line)/2;
        }
        else   //����͸������߲�����
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
      else if(line[i].left_line_state==lose ) //���û�����������ߣ�Ҳ�Ҳ������߾�ֱ�Ӹ�������
      {
        line[i].left_line=0;
        line[i].right_line=64;
        line[i].mid_line_new= (line[i].left_line+ line[i].right_line)/2;
      }
      else   // ����������ߣ�û�������ұ��ߵ�������������߲�����
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
    else    //Ѱ����
    {
      if(line[i].left_line_unrepiar<line[i].right_line_unrepiar && line[i].left_line_state==normal       // ������ҵ�����������
         && (line[i].left_line_unrepiar+distance2[i]+10) > line[i].right_line_unrepiar
             )
      {
        if(line[i].right_line_state==normal)   //��������߾�ֱ�������ұ���
        {
          line[i].left_line  = line[i].left_line_unrepiar;
          line[i].right_line = line[i].right_line_unrepiar;
          line[i].mid_line_new= (line[i].left_line+ line[i].right_line)/2;
        }
        else   //����͸������߲�����
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
      else if(line[i].right_line_state==lose ) //���û�����������ߣ�Ҳ�Ҳ������߾�ֱ�Ӹ�������
      {
        line[i].left_line=64;
        line[i].right_line=128;
        line[i].mid_line_new= (line[i].left_line+ line[i].right_line)/2;
      }
      else   // �������ұ��ߣ�û����������ߵ�������������߲����� �������
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
//                   ֱ��
//����
//���
//����    �ж�ֱ��
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
    //�ȼ����15�е�50�е�ƽ���������
    for(j=15;j<50;j++)
    {
      po_wide_num+=line[j].road_wide;
      po_wide_add++;
    }
    po_wide_av=po_wide_num/po_wide_add;
    if( po_wide_av>75) //ƽ����ȴ���һ��ֵʱ��Ϊ���µ�
    {
        Ramp_flag=1;
        Ramp_Len_x=Total_distance;

        var3=2;         //����2��ʾ�µ�ͬ�������µ���־
        updata_var(VAR3);
        tongbu[3]=10;
        Ramp_tong=1;

        add_count(cut_2,200);
//        chao_zhi=0;
//        wrz_distance=0;
//        L_wrz_flag=0;
//        R_wrz_flag=0;
//
        var4=0;         //���һ����л���־ͬ������ʾ�����л����л���ɺ����㣩
        updata_var(VAR4);
        tongbu[4]=10;

//        car_stop(1);
    }
  }
  if( zhidao_normal_cut>67 && zhidao_normal_cut1>=65 && Ramp_flag==0)
  {
    if( abs(po_road_wide_down[0][0]-po_road_wide_down[0][3])<2                 //��Զ��ʶ���µ������
       && (po_road_wide_up[0][0]-po_road_wide_up[0][3])>=3
         &&  abs(po_road_wide_down[1][0]-po_road_wide_down[1][3])<2
           && (po_road_wide_up[1][0]-po_road_wide_up[1][3])>=3
             && (abs(line[9].left_line_unrepiar - line[0].left_line_unrepiar)-abs(line[69].left_line_unrepiar - line[60].left_line_unrepiar))>=1
               && (abs(line[0].right_line_unrepiar - line[9].right_line_unrepiar)-abs(line[60].right_line_unrepiar - line[69].right_line_unrepiar))>=1
               )
    {
        Ramp_flag=1;
        Ramp_Len_x=Total_distance;

        var3=2;         //����2��ʾ�µ�ͬ�������µ���־
        updata_var(VAR3);
        tongbu[3]=10;
        Ramp_tong=1;

        add_count(cut_2,200);

//        chao_zhi=0;
//        wrz_distance=0;
//        L_wrz_flag=0;
//        R_wrz_flag=0;

        var4=0;         //���һ����л���־ͬ������ʾ�����л����л���ɺ����㣩
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
//                   ֱ��
//����
//���
//����    �ж�ֱ��
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
  uint8 *pimg;                    //ˮƽɨ��

  static uint8 white_to_black_num;
  static uint8 black_to_white_num;
  static uint8 qpxian,a;

  //����
  qpxian = 0;
  bujiezhi = 0;
 // stop = 0;

  for(i=0;i<30;i++)            //�ӵ�25�� ��ʼɨ��
  {
    black_to_white_num = 0;
    white_to_black_num = 0;

    pimg=src+(line_num-1-i)*128;
    for(j=10;j<=117;j++)               // ɨ1��
    {
      //����ڱ��
      if( (*(pimg+j)-*(pimg+j-1))!=0 && *(pimg+j)==255)
       black_to_white_num++;
       //����ױ��
      if( (*(pimg+j)-*(pimg+j-1))!=0 && *(pimg+j)==0)
      white_to_black_num++;
    }
    if(black_to_white_num>=5&&white_to_black_num>=5)
    {
      qpxian ++;
      line[i].line_case_mode = starline;
    }
    if(qpxian > 4 && System_time>=8)                   //����3�� ����Ϊ������ ����Ч�в�����
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
  if(speedwant >= 120)            //�ٶȱȽϴ���ʱ20ms
  {
    flag_key_select=0;
    flag_key_l_u_5=0;

    car_stop( 1 );
    yiting = 1;
  }
  else                             //�ٶȱȽ�С����ʱ50ms
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
//����    �ж����������ֱ��
//--------------------------------------------------

void R_wan_ru_zhi_judge()
{
  uint8 j;

  if(abs(line[50].right_line_unrepiar-line[3].right_line_unrepiar)<=15)         //�ұ�50����3�в�С��10
  {
    for(j=5;j<50;j++)
    {
      if( line[j+3].line_case_mode==all_normal                                    //5��50�����е�����
         && (line[j].right_line_unrepiar>=(line[3].right_line_unrepiar)        //
          || line[j].right_line_unrepiar<=(line[3].right_line_unrepiar))
          )
      {
        R_wan_ru_zhi_cut_R++ ;
      }
      if(line[j+3].line_case_mode==all_normal                                     //��һ����� �ҵ��С
         &&(line[j].right_line_unrepiar<=line[j-3].right_line_unrepiar)         //5��50�����е�����
         &&(line[j-3].right_line_unrepiar-line[j].right_line_unrepiar)<=3        //�ұߵ�j��С��j-3���Ҳ�ֵС��3
         &&(line[j].right_line_state<=(line[3].right_line_unrepiar))          //j�еĵ���50��3��֮��
         &&(line[j].right_line_unrepiar>=(line[50].right_line_unrepiar))
         )
      {
        R_wan_ru_zhi_cut_R_1++;
      }
      if(line[j+3].line_case_mode==all_normal                                     //�ڶ������ �ҵ�����
         &&(line[j].right_line_unrepiar>=line[j-3].right_line_unrepiar)         //5��50�����е�����
         &&(line[j].right_line_unrepiar-line[j-3].right_line_unrepiar)<=3        //�ұߵ�j�д�����j-3���Ҳ�ֵС��3
         &&(line[j].right_line_unrepiar>=(line[3].right_line_unrepiar))       //j�еĵ���50��3��֮��
         &&(line[j].right_line_unrepiar<=(line[50].right_line_unrepiar))
         )
      {
        R_wan_ru_zhi_cut_R_2++;
      }

      if(line[j+3].line_case_mode==all_normal                                     //��ߵ�һֱ����
         &&(line[j].left_line_unrepiar>=line[j-3].left_line_unrepiar)           //5��50�����е�����
         &&(line[j].left_line_unrepiar-line[j-3].left_line_unrepiar)>=2          //��ߵ�j����j-3�в�ֵС��6
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
//����    �ж����������ֱ��
//--------------------------------------------------
void L_wan_ru_zhi_judge()
{
  uint8 j;

  if(abs(line[50].left_line_unrepiar-line[3].left_line_unrepiar)<=15)           //��߱�50����3�в�С��10
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
      if(line[j+3].line_case_mode==all_normal                                     //��һ����� �������
         &&line[j].left_line_unrepiar>=line[j-3].left_line_unrepiar             //5��50�����е�����
         &&(line[j].left_line_unrepiar-line[j-3].left_line_unrepiar)<=3          //��ߵ�j�д���j-3���Ҳ�ֵС��3
         &&(line[j].left_line_unrepiar>=line[3].left_line_unrepiar)           //j�еĵ���50��3��֮��
         &&(line[j].left_line_unrepiar<=(line[50].left_line_unrepiar))
         )
      {
        L_wan_ru_zhi_cut_L_1++;
      }
      if(line[j+3].line_case_mode==all_normal                                         //�ڶ������ ����С
         &&line[j].left_line_unrepiar<=line[j-3].left_line_unrepiar             //5��50�����е�����
         &&(line[j-3].left_line_unrepiar-line[j].left_line_unrepiar)<=3          //��ߵ�j��С��j-3���Ҳ�ֵС��3
         &&(line[j].left_line_unrepiar<=(line[3].left_line_unrepiar))
         &&(line[j].left_line_unrepiar>=(line[50].left_line_unrepiar))
         )
      {
        L_wan_ru_zhi_cut_L_2++;
      }

      if(line[j+3].line_case_mode==all_normal                                     //�ұߵ�һֱ��С
         &&(line[j].right_line_unrepiar<=line[j-3].right_line_unrepiar)         //�ұߵ�j��С��j-3���Ҳ�ֵС��6
         &&(line[j-3].right_line_unrepiar-line[j].right_line_unrepiar)>=2        //�ұߵ�j����j-3�в�ֵС��6
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
//                   �շ���ʱ��ֱ������
//����
//���
//���� �ճ��������֮��ĳ�����ԭ����Ӧ�ÿ���ѡ��ճ�������ʱ�ĳ�������
//--------------------------------------------------
uint8 origin_chao( uint8 cut )
{
  static uint8 z=0,h=0;
  uint8 mid=64;
  if(Total_distance<170   //����ǰ�����ڣ�һ��������һ��������
     && var4==1 )    // ֻ���ڳ���δ��ɵ�ʱ���ڽ���
  {
    used_length=30;
    if(sequence==1)   //ǰ������
    {
      L_wrz_flag=3;
      if(Total_distance>30 && z==0 && chao_zhi==0)    //ǰ����ָ��λ��ͣ��    && Total_distance<35
      {
        z++;
        chao_zhi=1;
        car_stop(1);
      }
    }
    else              //�󳵿���
    {
      R_wrz_flag=3;
    }
  }
  if(sequence==1)   //ͣ�µĳ���⵽������֮��ָ�����
  {
    if( (road_count_chao>10 || ( leftval==0 && rightval==0 && s_distance<550 && s_distance>10))&& chao_zhi==1 && h==0)    //ǰ��ͣ���ȴ��������ʱ���������ܼ�⵽���ݣ�֤���������
    {
      chao_zhi=0;
      h++;
      L_wrz_flag=0;

      var4=0;         //���һ����л���־ͬ������ʾ�����л����л���ɺ����㣩
      updata_var(VAR4);
      tongbu[4]=10;

      if(var1==1) var1=0;   //�����л�ǰ��
      else var1=1;
      updata_var(VAR1);
      tongbu[1]=10;

      car_start(1);

      mid=64;
    }
  }
  else              //����ȥ�ĳ����յ�NRF�ź�֮��ָ�����
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
//                   �������ֱ��ʱ����
//����
//���
//���� ����ս���ֱ��ʱ����
//--------------------------------------------------
uint8 wrz_chao( uint8 cut )
{
  static uint8 z=0,h=0,a=0;
  uint8 mid=64;

  if(var4==0 )     //ֻ����ʶ���ǰ����ͣ��
  {
    h=0;
    a=0;
//    used_length=30;
    if((L_wrz_flag==1 || L_wrz_flag==2) && chao_zhi==1 )   //ǰ�������ֱ������ͣ
    {
      if(wrz_distance>15 && z==0 )    //ǰ����ָ��λ��ͣ��
      {
        z++;
        var4=1;         //����ֱ��������־ͬ������ʾ�����л����л���ɺ����㣩
        zhi_chao_deal();
      }
    }
    else if((R_wrz_flag==1 || R_wrz_flag==2) && chao_zhi==1 )
    {
      if(wrz_distance>15 && z==0 )
      {
        z++;
        var4=2;         //����ֱ��������־ͬ������ʾ���ڳ�����������ɺ����㣩
        zhi_chao_deal();
      }
    }

    if((L_wrz_flag==1 || L_wrz_flag==2) && chao_zhi==0 )   //�󳵳���ȥ֮����ֱ����־
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
    if(chao_zhi==1)   //ͣ�µĳ�
    {
      z=0;
      if(wrz_distance>45)
      {
        car_stop(1);
      }

      if( ( ( leftval==0 && rightval==0 && s_distance<550 && s_distance>10)) && h==0)    //ǰ��ͣ���ȴ��������ʱ���������ܼ�⵽���ݣ�֤���������,Ȼ����ǰ�����������־
      {    //road_count_chao>10 ||
        chao_zhi=0;
        h++;
        wrz_distance=0;
        L_wrz_flag=0;
        R_wrz_flag=0;

        var4=0;         //���һ����л���־ͬ������ʾ�����л����л���ɺ����㣩
        updata_var(VAR4);
        tongbu[4]=10;

        if(var1==1) var1=0;   //�����л�ǰ��
        else var1=1;
        updata_var(VAR1);
        tongbu[1]=10;

        car_start(1);
      }
    }
    else           //����ȥ�ĳ�
    {
      if( R_wrz_flag==0 && L_wrz_flag==0 && var4==1 && a==0 )   //�󳵳���ȥ֮����ֱ����־
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

