#ifndef __MAIN_H
#define __MAIN_H
         //定义存储接收图像的数组,2400=160*120/8
extern uint8 img[160*120];

#define nrf_debug 0    //0-关闭无线传输， 1-开启无线传输
extern uint8 CAR_MODEL;    //1-号车， 2-2号车， 3-3号车

/////////*******定义通信代号******//////////
#define Start         'A'    //发车
#define Respond       'B'   //应答
#define Turn_right    'C'  //十字超车标志向右
#define Turn_left     'D'  //十字超车标志向左
#define Turn_finish   'E'   //十字超车完成
#define Stop_pre      'F'   //停车预备标志位
#define Stop          'G'   //停车标志
#define ZC_right      'H'   //直道超车区在右侧
#define ZC_left       'I'   //直道超车区在左侧
#define ZC_OK         'J'   //检测出离开直道超车区
#define Block_right   'K'   //右侧障碍
#define Block_left    'L'   //左侧障碍


extern uint8  var1, var2;
extern uint16 var3, var4;
extern uint32 var5, var6;

extern int16 speedwantD_set;
extern int16 speedwantE_set;
extern uint8 origin_chao_cont;
extern uint8 huan_chao_cont;
extern uint8 wrz_chao_cont;


extern uint8 front_car,weizhi_turn;
extern int8 tongbu[10];
extern int8 cut_1,cut_2;
extern int8 diff;
extern long int speedwant;
extern int8 chao_huan,chao_zhi,Ramp_tong;  //此标志为了让前车在同一超车地点只触发一次超车
extern int8 sequence;  //车的顺序，为0的时候则前后车标志位与var1相同，1则相反

#endif //__MAIN_H