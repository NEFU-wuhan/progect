#ifndef __MAIN_H
#define __MAIN_H
         //定义存储接收图像的数组,2400=160*120/8
extern uint8 img[160*120];

#define nrf_debug 0    //0-关闭无线传输， 1-开启无线传输

extern uint8 CAR_MODEL;    //1-号车， 2-2号车， 3-3号车

extern uint8 SD_op_mode;
///////////******通信专用变量*******//////
extern char info;

/////////*******定义通信代号******//////////
#define Start         'A'   //发车
#define Respond       'B'   //jiasu
#define huan_right    'C'   //环道超车标志向右
#define huan_left     'M'   //环道超车标志向左   //不能用D，上电瞬间会自动发送一个'D'
#define huan_finish   'E'   //环道超车完成
#define Ramp          'F'   //坡道标志位
#define Stop          'G'   //停车标志
#define ZC_right      'H'   //直道超车区在右侧
#define ZC_left       'I'   //直道超车区在左侧
#define ZC_OK         'J'   //直道超车完成
#define Block_right   'K'   //右侧障碍
#define Block_left    'L'   //左侧障碍
#define turn_car      'N'   //切换前后车的标志


#define add_count(number,x) number<x?(number++):(number=x)              //3目运算：如果式子为真，表达式的值等于冒号前的式子

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
extern int8 chao_huan,chao_zhi,Ramp_tong;  //此标志为了让前车在同一超车地点只触发一次超车
extern int8 sequence;  //车的顺序，为0的时候则前后车标志位与var1相同，1则相反

#endif //__MAIN_H