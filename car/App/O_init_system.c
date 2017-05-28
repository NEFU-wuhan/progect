#include "include.h"
#include "common.h"

uint8  nrf_rx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //预多
uint8  nrf_tx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //预多
uint8 *imgbuff = (uint8 *)(((uint8 *)&nrf_tx_buff) + COM_LEN);     //图像地址  //定义存储接收图像的数组,2400=160*120/8

void NRF_INIT(void);
//初始化参数
void init_canshu()
{
  //电机参数
  if( CAR_MODEL==1 )
  {
    KP2=120;      //比例系数500
    Ki2=80;      //积分系数220
    Kd2=10;
  }
  else if(CAR_MODEL==2 )
  {
    KP2=114;      //比例系数500
    Ki2=84;      //积分系数220
    Kd2=20;
  }
  else if(CAR_MODEL==3 )
  {
    KP2=120;      //比例系数500
    Ki2=85;      //积分系数220
    Kd2=10;
  }
  else
  {
    KP2=120;      //比例系数500
    Ki2=85;      //积分系数220
    Kd2=10;
  }
  //舵机参数
  if( CAR_MODEL==1 )
  {
    Mid_duty=7400;
    Kp1=13;  //16
    Kp2=0.36;
    Kd=4.36;
    MID_dir_duty=7400;  //7780
    MAX_dir_duty=8400;  //8740
    MIN_dir_duty=6400;  //6880
  }
  else if(CAR_MODEL==2 )
  {
    Mid_duty=7500;
    Kp1=13;  //15
    Kp2=0.41;        //0.36
    Kd=4.40;        //4.36
    MID_dir_duty=7500;  //7780
    MAX_dir_duty=8500;  //8740
    MIN_dir_duty=6500;  //6880
  }
  else if(CAR_MODEL==3 )
  {
    Mid_duty=7500;
    Kp1=14;  //16
    Kp2=0.36;
    Kd=4.55;
    MID_dir_duty=7500;
    MAX_dir_duty=8500;
    MIN_dir_duty=6550;
  }
  else
  {
    Mid_duty=7500;
    Kp1=14;  //16
    Kp2=0.36;
    Kd=4.55;
    MID_dir_duty=7500;
    MAX_dir_duty=8500;
    MIN_dir_duty=6550;
  }
  //设置每个车环道寻线行驶方向
  if( CAR_MODEL==1 ) follow_huan=1;    //1 为寻左线，0为寻右线
  else if(CAR_MODEL==2 ) follow_huan=1;    //1 为寻左线，0为寻右线
  else if(CAR_MODEL==3 ) follow_huan=1;    //1 为寻左线，0为寻右线
  else follow_huan=1;
}
/*******************************************************************************
* Function Name: init();
* Input Param  : 无
* Output Param : 无
* Description  : 系统初始化
* Author Data  : 2015/1/12
******************************************************************************/
void init_sys(void)
 {
    /************************ 电机  舵机   初始化  ***********************/
    ftm_pwm_init(S3010_FTM, S3010_CH,S3010_HZ,Mid_duty);   // 舵机
    ftm_pwm_init(FTM0, FTM_CH4,10000, 0);    //左电机正转
    ftm_pwm_init(FTM0, FTM_CH5,10000, 0);    //左电机反转
    ftm_pwm_init(FTM0, FTM_CH6,10000, 0);    //右电机反转
    ftm_pwm_init(FTM0, FTM_CH7,10000, 0);    //右电机正转
//
//    /****************    编码器初始化     *******************/
    ftm_quad_init(FTM1);
    ftm_quad_init(FTM2);

    /************************ IO引脚初始化  ***********************/
    gpio_init(ECHG,GPI,0);            //脉冲输出   管脚初始化
    gpio_init(receive_flag,GPI,0);    //脉冲接收标志位
    /////////*****鸳鸯超声波测距初始化*********///////
    gpio_init(PTA24,GPO,1);//初始化发射模块
    gpio_ddr (PTA24,GPI);//初始化接收模块两车都允许接收
//    port_init (PTA24, IRQ_EITHER | PF | ALT1 | PULLUP ); //跳变沿中断触发，带上拉电阻（能否接下拉？）
//    gpio_ddr (PTA19,GPI);//初始化接收模块两车都允许接收
//    port_init (PTA19, IRQ_EITHER | PF | ALT1 | PULLUP ); //跳变沿中断触发，带上拉电阻（能否接下拉？）

//    gpio_init (PTD3, GPI,1);                                                    //初始化 拨码开关1
//    gpio_init (PTC6, GPI,1);                                                    //初始化 拨码开关2
//    gpio_init (PTE7, GPI,1);                                                    //初始化 拨码开关3
//    gpio_init (PTE6, GPI,1);                                                    //初始化 拨码开关4
      //*******拨码开关初始化******//
      gpio_init (PTE9,GPI,0); //最前                                            //初始化 拨码开关1
      boma[0]=gpio_get(PTE9);
      gpio_init (PTE8,GPI,0);                                                     //初始化 拨码开关2
      boma[1]=gpio_get(PTE8);
      gpio_init (PTE7,GPI,0);                                                     //初始化 拨码开关3
      boma[2]=gpio_get(PTE7);
      gpio_init (PTE6,GPI,0);  //最后                                            //初始化 拨码开关4
      boma[3]=gpio_get(PTE6);

      if(boma[0]==0 && boma[1]==0) CAR_MODEL=1;
      else if(boma[0]==1 && boma[1]==0) CAR_MODEL=2;
      else if(boma[0]==0 && boma[1]==1) CAR_MODEL=3;
      else CAR_MODEL=4;
//    gpio_init (PTC3, GPO,0);           //蜂鸣器管脚初始化

    adc_init (ADC0_DM1);                                                        //电压测量的ADC引脚
//    adc_init(ADC0_SE9);                                                         //ADC初始化(用于 陀螺仪或加速度计)
    /************************ 摄像头 初始化  ***********************/
    camera_init(imgbuff);                                   //摄像头初始化，把图像采集到 imgbuff 地址

    /************************** LED 初始化  ***************************/
    led_init(LED_MAX);

    /*******************  无线模块初始化,预编译    *************************/
     #if nrf_debug
      NRF_INIT();
     #endif

    /************************ LCD 液晶屏 初始化  ***********************/
    LCD_init();

     /*液晶图片初始化*/
    bmp_init();
    bmp_display();
    DELAY_MS(500);
    /***********************   按键消息 初始化    ***********************/
    key_event_init();

    /***********************   参数 初始化    ***********************/
    init_canshu();

    /***********************       PIT定时        ***********************/
    pit_init_ms(PIT0,10);      //pit0 用于按键定时扫描
    pit_init_ms(PIT2,1);       //pit2 定时中断，总线定时

    /************************ 配置 K60 的优先级  ***********************/
    NVIC_SetPriorityGrouping(5);            //设置优先级分组,4bit 抢占优先级,没有亚优先级
    //摄像头场中断最高，pit次之，dma次之
    NVIC_SetPriority(PORTA_IRQn,0);         //配置优先级     摄像头行中断和场中断
    NVIC_SetPriority(DMA0_IRQn,1);          //配置优先级     摄像头数据传输
    NVIC_SetPriority(PIT2_IRQn,2);          //配置优先级     总线定时
    NVIC_SetPriority(PORTE_IRQn,4);         //配置优先级     无线传输中断
    NVIC_SetPriority(PIT0_IRQn,3);          //配置优先级     按键中断
    NVIC_SetPriority(PORTB_IRQn,5);          //配置优先级     超声波中断

    /***********************   配置中断服务函数    ***********************/
    set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //设置 PORTA 的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //设置 DMA0 的中断服务函数为 DMA0_IRQHandler
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);                  //设置 PIT0 的中断服务函数为 PIT0_IRQHandler
    set_vector_handler(PIT2_VECTORn ,PIT2_IRQHandler);                 //设置 PIT2 的中断服务函数为 PIT2_IRQHandler
//    set_vector_handler(PORTB_VECTORn , portb_handler);                  //超声波的外部中断服务函数为portb_handler

    enable_irq(PORTB_IRQn);
    enable_irq(PORTA_IRQn);
    enable_irq(PIT0_IRQn);
    enable_irq(PIT2_IRQn);

 }


/************************ 无线模块 NRF 初始化  ********************/
void NRF_INIT(void)
 {
    uint32 i;

   /************************ 无线模块 NRF 初始化  ***********************/
    while(!nrf_init());
    //配置中断服务函数
    set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);                //设置 PORTE 的中断服务函数为 PORTE_VECTORn
    enable_irq(PORTE_IRQn);
    nrf_msg_init();                                                     //无线模块消息初始化

    //摄像头模块，需要 发送 空命令给 调试板模块，这样可以清掉 接收图像失败而产生多余数据
    i = 20;
    while(i--)
    {
        nrf_msg_tx(COM_RETRAN,nrf_tx_buff);                              //发送多个 空 命令过去，用于清空 接收端的缓存数据
    }

 }