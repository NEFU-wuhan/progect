#ifndef _O_measure_distance_H
#define _O_measure_distance_H


#define ECHG   PTB22// PTA19
#define receive_flag  PTB23// PTA24


#define FLAGWAIT    0xFFFFF

extern uint32 timevar;

extern int s_distance;
extern int s_distance_l;
extern int s_distance_ll;
extern int s_distance_lll;


extern int measure_distance();

extern void Ramp_mesure();

#define distan_value  40                // �ж��µ���ֵ
extern int8 distan0;
extern int8 Ramp_flag1;       //�µ���־  -1������  0��ƽ·  1������
extern int8 Ramp_flag_pre;       //�µ���־  -1������  0��ƽ·  1������
extern int8 Ramp_jiansu_start,Ramp_after_jiasu;  //�µ����ٱ�־
extern int16 Ramp_Len_Set;    //���³���     160
extern int8 Ramp_en;     //�µ�ʹ��
extern uint8 Ramp_yushibie;
extern uint8 Ramp_Len_yu;
extern int32 Ramp_Len;           //  ���²���



#endif

