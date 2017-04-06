/******************************************
*                                         *
*            2017.1.20  ZQ修改            *
*                                         *
******************************************/
#include "measure_distance_zq.h"
#include "include.h"

uint32 timevar;
uint32 flag;

unsigned int s_distance=0;  


int s_distance_l=0;
int s_distance_ll=0;
int s_distance_lll=0;  


int measure_distance()                                   //测距函数，直接调用即可，返回值是s_distance，外部变量，可全局引用
{ 
    if(gpio_get(receive_flag)==1)                        //如果信号采集标志位为1，那么开始采集距离
    {
    	flag = 0;
        while(gpio_get(ECHG) == 0)                      //等待电平变高，低电平一直等待
        {
            if(gpio_get(receive_flag)==0) break;
        }
        pit_time_start  (PIT3);                         //开始计时
        while(gpio_get(ECHG) == 1)                      //等待电平变低，高电平一直等待
        {
            flag++;
            if(flag >FLAGWAIT||gpio_get(receive_flag)==0)  break;  //此处应该是等待溢出
        }             
        
        timevar = pit_time_get_us    (PIT3);            //停止计时，获取计时时间
        
        if(flag <FLAGWAIT )   
        {       
            s_distance_lll=s_distance_ll;
            s_distance_ll = s_distance_l;
            s_distance_l = s_distance;
            s_distance = (timevar * 340 /2/1000)/5+3;     //时间和距离之间的转换          
            s_distance = (int)((s_distance + s_distance_l + s_distance_ll + s_distance_lll)/4);//数据平滑
	}
			
	} 
    else  s_distance = 550;                            //信号采集不到的时候，返回550CM
   printf("%d\n",s_distance);    
    return s_distance;   

}



