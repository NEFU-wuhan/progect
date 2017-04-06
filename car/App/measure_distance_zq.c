/******************************************
*                                         *
*            2017.1.20  ZQ�޸�            *
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


int measure_distance()                                   //��ຯ����ֱ�ӵ��ü��ɣ�����ֵ��s_distance���ⲿ��������ȫ������
{ 
    if(gpio_get(receive_flag)==1)                        //����źŲɼ���־λΪ1����ô��ʼ�ɼ�����
    {
    	flag = 0;
        while(gpio_get(ECHG) == 0)                      //�ȴ���ƽ��ߣ��͵�ƽһֱ�ȴ�
        {
            if(gpio_get(receive_flag)==0) break;
        }
        pit_time_start  (PIT3);                         //��ʼ��ʱ
        while(gpio_get(ECHG) == 1)                      //�ȴ���ƽ��ͣ��ߵ�ƽһֱ�ȴ�
        {
            flag++;
            if(flag >FLAGWAIT||gpio_get(receive_flag)==0)  break;  //�˴�Ӧ���ǵȴ����
        }             
        
        timevar = pit_time_get_us    (PIT3);            //ֹͣ��ʱ����ȡ��ʱʱ��
        
        if(flag <FLAGWAIT )   
        {       
            s_distance_lll=s_distance_ll;
            s_distance_ll = s_distance_l;
            s_distance_l = s_distance;
            s_distance = (timevar * 340 /2/1000)/5+3;     //ʱ��;���֮���ת��          
            s_distance = (int)((s_distance + s_distance_l + s_distance_ll + s_distance_lll)/4);//����ƽ��
	}
			
	} 
    else  s_distance = 550;                            //�źŲɼ�������ʱ�򣬷���550CM
   printf("%d\n",s_distance);    
    return s_distance;   

}



