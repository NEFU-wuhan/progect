#ifndef __MAIN_H
#define __MAIN_H
         //����洢����ͼ�������,2400=160*120/8
extern uint8 img[160*120];

#define nrf_debug 0    //0-�ر����ߴ��䣬 1-�������ߴ���
#define CAR_MODEL 2    //1-�ų��� 2-2�ų��� 3-3�ų�

extern uint8  var1, var2;
extern uint16 var3, var4;
extern uint32 var5, var6;

extern int16 speedwantD_set;
extern int16 speedwantE_set;

extern uint8 front_car,weizhi_turn;
extern int8 tongbu[10];
extern int8 input,cut_1,cut_2;
extern int8 diff;
extern long int speedwant;
extern int8 chao_one,chao_one2;  //�˱�־Ϊ����ǰ����ͬһ�����ص�ֻ����һ�γ���
extern int8 sequence;  //����˳��Ϊ0��ʱ����ǰ�󳵱�־λ��var1��ͬ��1���෴

#endif //__MAIN_H