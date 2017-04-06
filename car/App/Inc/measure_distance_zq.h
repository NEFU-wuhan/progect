#ifndef _measure_distance_zq_H
#define _measure_distance_zq_H



#define ECHG    PTA19
#define receive_flag   PTA24


#define FLAGWAIT    0xFFFFF


extern unsigned int s_distance;
extern int s_distance_l;
extern int s_distance_ll;
extern int s_distance_lll;


extern int measure_distance();

#endif 
