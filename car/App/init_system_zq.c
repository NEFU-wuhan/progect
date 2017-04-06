#include "include.h"
#include "common.h"

uint8  nrf_rx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //预多
uint8  nrf_tx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //预多
uint8 *imgbuff = (uint8 *)(((uint8 *)&nrf_tx_buff) + COM_LEN);     //图像地址  //定义存储接收图像的数组,2400=160*120/8

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
    ftm_pwm_init(S3010_FTM, S3010_CH,S3010_HZ,7880);   // 舵机
    ftm_pwm_init(FTM0, FTM_CH4,10000, 0);    //左电机正转
    ftm_pwm_init(FTM0, FTM_CH5,10000, 0);    //左电机反转
    ftm_pwm_init(FTM0, FTM_CH6,10000, 0);    //右电机反转
    ftm_pwm_init(FTM0, FTM_CH7,10000, 0);    //右电机正转

    /****************    编码器初始化     *******************/
    ftm_quad_init(FTM1);
    ftm_quad_init(FTM2);
    /************************ 配置 K60 的优先级  ***********************/
    NVIC_SetPriorityGrouping(5);            //设置优先级分组,4bit 抢占优先级,没有亚优先级
    //摄像头场中断最高，pit次之，dma次之
    NVIC_SetPriority(PORTA_IRQn,0);         //配置优先级     摄像头行中断和场中断
    NVIC_SetPriority(DMA0_IRQn,1);          //配置优先级     摄像头数据传输
    NVIC_SetPriority(PIT2_IRQn,2);          //配置优先级     总线定时
    NVIC_SetPriority(PORTE_IRQn,4);         //配置优先级     无线传输中断
    NVIC_SetPriority(PIT0_IRQn,3);          //配置优先级     按键中断

    /************************ IO引脚初始化  ***********************/
    gpio_init(ECHG,GPI,0);            //脉冲输出   管脚初始化
    gpio_init(receive_flag,GPI,0);    //脉冲接收标志位

//    gpio_init (PTD3, GPI,1);                                                    //初始化 拨码开关1
//    gpio_init (PTC6, GPI,1);                                                    //初始化 拨码开关2
//    gpio_init (PTC7, GPI,1);                                                    //初始化 拨码开关3
//    gpio_init (PTD2, GPI,1);                                                    //初始化 拨码开关4

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

    /***********************       PIT定时        ***********************/
    pit_init_ms(PIT0,10);      //pit0 用于按键定时扫描
    pit_init_ms(PIT2,1);       //pit2 定时中断，总线定时

    /***********************   配置中断服务函数    ***********************/
    set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //设置 PORTA 的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //设置 DMA0 的中断服务函数为 DMA0_IRQHandler
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);                  //设置 PIT0 的中断服务函数为 PIT0_IRQHandler
    set_vector_handler(PIT2_VECTORn ,PIT2_IRQHandler);                 //设置 PIT2 的中断服务函数为 PIT2_IRQHandler

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