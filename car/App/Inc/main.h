#ifndef __MAIN_H
#define __MAIN_H
         //����洢����ͼ�������,2400=160*120/8
extern uint8 img[160*120];

#define nrf_debug 0    //0-�ر����ߴ��䣬 1-�������ߴ���

extern uint8 CAR_MODEL;    //1-�ų��� 2-2�ų��� 3-3�ų�

extern uint8 SD_op_mode;
///////////******ͨ��ר�ñ���*******//////
extern char info;

/////////*******����ͨ�Ŵ���******//////////
#define Start         'A'   //����
#define Respond       'B'   //jiasu
#define huan_right    'C'   //����������־����
#define huan_left     'M'   //����������־����   //������D���ϵ�˲����Զ�����һ��'D'
#define huan_finish   'E'   //�����������
#define Ramp          'F'   //�µ���־λ
#define Stop          'G'   //ͣ����־
#define ZC_right      'H'   //ֱ�����������Ҳ�
#define ZC_left       'I'   //ֱ�������������
#define ZC_OK         'J'   //ֱ���������
#define Block_right   'K'   //�Ҳ��ϰ�
#define Block_left    'L'   //����ϰ�
#define turn_car      'N'   //�л�ǰ�󳵵ı�־


#define add_count(number,x) number<x?(number++):(number=x)              //3Ŀ���㣺���ʽ��Ϊ�棬���ʽ��ֵ����ð��ǰ��ʽ��

extern uint8  var1, var2;
extern uint16 var3, var4;
extern uint32 var5, var6;

extern uint8 chao_cnt_total;
extern int16 speedwantD_set;
extern int16 speedwantE_set;
extern uint8 origin_chao_cont;
extern uint8 huan_chao_cont;
extern uint8 wrz_chao_cont;
extern uint8 chao_car_cnt_set ;


extern uint8 front_car,weizhi_turn;
extern int8 tongbu[10];
extern int8 cut_1,cut_2;
extern int8 diff;
extern long int speedwant;
extern int8 chao_huan,chao_zhi,Ramp_tong;  //�˱�־Ϊ����ǰ����ͬһ�����ص�ֻ����һ�γ���
extern int8 sequence;  //����˳��Ϊ0��ʱ����ǰ�󳵱�־λ��var1��ͬ��1���෴

#endif //__MAIN_H