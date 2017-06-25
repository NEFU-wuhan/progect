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

#define distan_value  40                // 判定坡道的值
extern int8 distan0;
extern int8 Ramp_flag1;       //坡道标志  -1：下坡  0：平路  1：上坡
extern int8 Ramp_flag_pre;       //坡道标志  -1：下坡  0：平路  1：上坡
extern int8 Ramp_jiansu_start,Ramp_after_jiasu;  //坡道减速标志
extern int16 Ramp_Len_Set;    //上坡长度     160
extern int8 Ramp_en;     //坡道使能
extern uint8 Ramp_yushibie;
extern uint8 Ramp_Len_yu;
extern int32 Ramp_Len;           //  上坡步数



#endif

