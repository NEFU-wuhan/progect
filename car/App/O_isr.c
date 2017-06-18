#include "include.h"

/*!
 *  @brief      PIT2中断服务函数
 *  @since      v5.0
 */
void PIT2_IRQHandler()        //主要中断循环
{
    static uint8  part=0;
    zhenshu_count++;
    if(zhenshu_count==1000)    //1秒钟显示一次，即为当前帧数
    {
      display_zs=camera_zhenshu;
      camera_zhenshu=0;
      zhenshu_count=0;
      if(display_zs<display_zs_min ) display_zs_min=display_zs;
    }
    part++;

    //循环计数控制，（只用于CCD采集和舵机速度控制），总中断还是一秒进入一次
    switch(part)
    {
    case 1:
      DJ_PID();                //先进行舵机控制
      break;
    case 2:
      speedcontrol5();
      break;
    case 3:
      break;
    case 4:

      break;
    case 5:
//      bat_voltage_measure() ;      //电压测量

      part=0;
      break;

    default:
      part=0;
      break;
    }
     Time_Commen();   //记录系统时间
//   if(Beep_En)                                                //蜂鸣器使能及蜂鸣器程序
//      Beep();

   PIT_Flag_Clear(PIT2);
}

/*!
 *  @brief      PORTA中断服务函数
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n;    //引脚号
    uint32 flag;

    //while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //清中断标志位

    n = 29; //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //使用行中断
    n = 28;
    if(flag & (1 << n))                                 //PTA28触发中断
    {
        camera_href();
    }
#endif

}



/*!
 *  @brief      DMA0中断服务函数
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}

/*!
 *  @brief      PIT0中断服务函数
 *  @since      v5.0
 */
void PIT0_IRQHandler()
{
 /**/
   if(stop == 1)
  {
    stop_count++;
    if(stop_count >= 800)
    {
      stop_count=800;
    }
  }
  /**/

    key_IRQHandler();

    PIT_Flag_Clear(PIT0);
}

/******************nrf中断*******************/
void PORTE_IRQHandler()
{
    uint8  n;    //引脚号
    uint32 flag;

    flag = PORTE_ISFR;
    PORTE_ISFR  = ~0;                                        //清中断标志位

    n = 27;
    if(flag & (1 << n))                                      //PTE27触发中断
    {
        nrf_handler();
    }
}

///////////******超声波外部中断服务函数*******//////////
void portb_handler()
{
  PORT_FUNC(B,22,measure_distance);        //A  24
}

