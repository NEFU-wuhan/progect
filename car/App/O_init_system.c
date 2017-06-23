#include "include.h"
#include "common.h"

uint8  nrf_rx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //预多
uint8  nrf_tx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //预多
uint8 *imgbuff = (uint8 *)(((uint8 *)&nrf_tx_buff) + COM_LEN);     //图像地址  //定义存储接收图像的数组,2400=160*120/8
uint8 buff[CAMERA_SIZE];
Ind_msg Ind ={0};

void NRF_INIT(void);
void flash_onece();
void read_ext();
void record_ext();
//初始化参数
void init_canshu()
{
  //电机参数
  if( CAR_MODEL==1 )
  {
    KP2=130;      //比例系数500
    Ki2=30;      //积分系数220
    Kd2=10;
  }
  else if(CAR_MODEL==2 )
  {
    KP2=100;      //比例系数500
    Ki2=30;      //积分系数220
    Kd2=10;
  }
  else if(CAR_MODEL==3 )
  {
    KP2=120;      //比例系数500
    Ki2=40;      //积分系数220
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
    Kp1=14;  //16
    Kp2=0.65;
    Kd=5.7;
    MID_dir_duty=7400;  //7780
    MAX_dir_duty=8400;  //8740
    MIN_dir_duty=6400;  //6880
  }
  else if(CAR_MODEL==2 )
  {
    Mid_duty=7740;
    Kp1=14;  //15
    Kp2=0.6;        //0.36
    Kd=6.5;        //4.36
    MID_dir_duty=7740;  //7780
    MAX_dir_duty=8740;  //8740
    MIN_dir_duty=6740;  //6880
  }
  else if(CAR_MODEL==3 )
  {
    Mid_duty=7530;
    Kp1=14;  //16
    Kp2=0.36;
    Kd=4.55;
    MID_dir_duty=7530;
    MAX_dir_duty=8530;
    MIN_dir_duty=6530;
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
  //设置车第几个环道寻线行驶方向
  follow_huan_set[1]=0;   // 1 为寻左线，0为寻右线 2为两车都寻左线，3为两车都寻右线
  follow_huan_set[2]=0;
  follow_huan_set[3]=1;
  follow_huan_set[4]=1;
  follow_huan_set[5]=1;
}

//通信处理函数
void cmc(char mes)
{
  if(mes==Start)//发车
  {
    uart_start_stop(1);
  }
  else if(mes==Stop) //停车
  {
    uart_start_stop(0);
  }
  else if(mes==turn_car) //切换前后车
  {
    if(front_car==1)  front_car=0;
    else front_car=1;

  }
  else if(mes==Ramp) //坡道
  {
    Ramp_flag=1;
    Ramp_yushibie=1;
    add_count(cut_2,200);
  }
  else if(mes==huan_right) //环道右巡线
  {
    fiag_huan_yu=1;
    if( follow_huan_set[hehe+1]==1 )
    {
      if(chao_cnt_total<=chao_car_cnt_set)
      {
        add_count(chao_cnt_total,200);  //记录总超车次数
      }
      else
      {
        follow_huan=1;                  //本车寻左线
      }
    }
    else if( follow_huan_set[hehe+1]==2 )
    {
      follow_huan=1;                  //两车都寻左线
    }
    else if( follow_huan_set[hehe+1]==3 )
    {
      follow_huan=0;                  //两车都寻右线
    }
    else
    {
      if(chao_cnt_total<=chao_car_cnt_set)
      {
        follow_huan=1;                  //本车寻左线
        add_count(chao_cnt_total,200);  //记录总超车次数
      }
      else
      {
         follow_huan=0;                  //本车寻右线
      }
    }
  }
  else if(mes==huan_left)  //环道左巡线
  {
    fiag_huan_yu=1;
    if( follow_huan_set[hehe+1]==1 )
    {
      if(chao_cnt_total<=chao_car_cnt_set)
      {
        follow_huan=0;                  //本车寻右线
        add_count(chao_cnt_total,200);  //记录总超车次数
      }
      else
      {
        follow_huan=1;                  //本车寻左线
      }
    }
    else if( follow_huan_set[hehe+1]==2 )
    {
      follow_huan=1;                  //两车都寻左线
    }
    else if( follow_huan_set[hehe+1]==3 )
    {
      follow_huan=0;                  //两车都寻右线
    }
    else
    {
      if(chao_cnt_total<=chao_car_cnt_set)
      {
        follow_huan=1;                  //本车寻左线
        add_count(chao_cnt_total,200);  //记录总超车次数
      }
      else
      {
        follow_huan=0;                  //本车寻右线
      }
    }
  }
  else if(mes==huan_finish) //环道完成
  {
    chao_huan=0;
//    fiag_huan_yu=0;
    car_start( 1 );
  }
  else if(mes==ZC_right) //直道右侧超车
  {
    R_wrz_flag=1;
  }
  else if(mes==ZC_left)  //直道左侧超车
  {
    L_wrz_flag=1;
  }
  else if(mes==ZC_OK)   //直道超车完成
  {
    add_count(chao_cnt_total,200);  //记录总超车次数
    L_wrz_flag=0;
    R_wrz_flag=0;
  }
  else if(mes==Block_right)  //障碍在右侧
  {
  }
  else if(mes==Block_left)   //障碍在左侧
  {
  }
}

void UART_intur()//串口接收中断处理函数
{
  if(UART_S1_REG(UARTN[UART3]) & UART_S1_RDRF_MASK)   //接收数据寄存器满
  {
    uart_getchar(UART3,&info);
    cmc(info);
  }
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
    Site_t site;
    site.x = 13;
    site.y = 3;

    /************************ L CD 液晶屏 初始化  ***********************/
    LCD_init();

     /*液晶图片初始化*/
    bmp_init();
    bmp_display();

    /************************ 电机  舵机   初始化  ***********************/
    LCD_str(site,"duo_ji_init...", BLACK,WHITE);
    ftm_pwm_init(S3010_FTM, S3010_CH,S3010_HZ,Mid_duty);   // 舵机

    LCD_str(site,"dian_j_init...", BLACK,WHITE);
    ftm_pwm_init(FTM0, FTM_CH4,10000, 0);    //左电机正转
    ftm_pwm_init(FTM0, FTM_CH5,10000, 0);    //左电机反转
    ftm_pwm_init(FTM0, FTM_CH6,10000, 0);    //右电机反转
    ftm_pwm_init(FTM0, FTM_CH7,10000, 0);    //右电机正转

    /****************    编码器初始化     *******************/
    LCD_str(site,"bian_m_init...", BLACK,WHITE);
    ftm_quad_init(FTM1);
    ftm_quad_init(FTM2);

    /************************ IO引脚初始化  ***********************/
    LCD_str(site,"ce_ju_init....", BLACK,WHITE);
    gpio_init(receive_flag,GPI,0);    //脉冲接收标志位
    gpio_ddr (ECHG,GPI);//初始化接收模块两车都允许接收
    port_init (ECHG, IRQ_EITHER | PF | ALT1 | PULLUP ); //跳变沿中断触发，带上拉电阻（能否接下拉？）

    //*******拨码开关初始化******//
    LCD_str(site,"bo_ma_init....", BLACK,WHITE);
    gpio_init (PTE9,GPI,0); //最后                                             //初始化 拨码开关1
    boma[7]=gpio_get(PTE9);
    gpio_init (PTE7,GPI,0);                                                     //初始化 拨码开关2
    boma[6]=gpio_get(PTE7);
    gpio_init (PTE8,GPI,0);                                                     //初始化 拨码开关3
    boma[5]=gpio_get(PTE8);
    gpio_init (PTE6,GPI,0);  //最前                                            //初始化 拨码开关4
    boma[4]=gpio_get(PTE6);

    gpio_init (PTA9,GPI,0); //最左                                             //初始化 拨码开关1
    boma[3]=gpio_get(PTA9);
    gpio_init (PTA7,GPI,0);                                                     //初始化 拨码开关2
    boma[2]=gpio_get(PTA7);
    gpio_init (PTA5,GPI,0);                                                     //初始化 拨码开关3
    boma[1]=gpio_get(PTA5);
    gpio_init (PTE27,GPI,0);  //最右                                            //初始化 拨码开关4
    boma[0]=gpio_get(PTE27);

    if( boma[0]==0 ) CAR_MODEL=1;
    else if( boma[1]==0) CAR_MODEL=2;
    else if( boma[2]==0) CAR_MODEL=3;
    else CAR_MODEL=4;

//    LCD_str(site,"buzzer_init...", BLACK,WHITE);
////    gpio_init (PTC3, GPO,0);           //蜂鸣器管脚初始化
//
//    LCD_str(site,"dian_y_init...", BLACK,WHITE);
//    adc_init (ADC0_DM1);                                                      //电压测量的ADC引脚
    adc_init (ADC1_DM1);                                                        //坡道测量的ADC引脚
////    adc_init(ADC0_SE9);                                                         //ADC初始化(用于 陀螺仪或加速度计)
    /************************ 摄像头 初始化  ***********************/
    LCD_str(site,"camera_init...", BLACK,WHITE);
    camera_init(imgbuff);                                   //摄像头初始化，把图像采集到 imgbuff 地址
    LCD_str(site,"camera_end...", BLACK,WHITE);

    /************************** LED 初始化  ***************************/
//    led_init(LED_MAX);

    /*******************  无线模块初始化,预编译    *************************/
    #if nrf_debug
      LCD_str(site,"NRF_init....", BLACK,WHITE);
      NRF_INIT();
    #endif

    /***********************   按键消息 初始化    ***********************/
    LCD_str(site,"key_init....", BLACK,WHITE);
    key_event_init();

    /***********************   参数 初始化        ***********************/
    init_canshu();   //注意程序中的参数会被FLASH更新

    /************************ Flash 初始化  ***********************/
    LCD_str(site,"flash_init....", BLACK,WHITE);
    flash_init();
//      flash_onece();
    DELAY_MS(150);

    if( boma[4]==1 )    //拨码开关控制是否读取FLASH
    {
      read_ext();
    }

    /***********************   串口通信 初始化    ***********************/
    LCD_str(site,"UART_init...", BLACK,WHITE);
    uart_init (UART3, 57600);

    /***********************       PIT定时        ***********************/
    LCD_str(site,"vector_init...", BLACK,WHITE);
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
    NVIC_SetPriority(PORTB_IRQn,6);          //配置优先级     超声波中断
    set_irq_priority(UART3_RX_TX_IRQn,5);//串口接收中断

    /***********************   配置中断服务函数    ***********************/
    set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //设置 PORTA 的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //设置 DMA0 的中断服务函数为 DMA0_IRQHandler
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);                  //设置 PIT0 的中断服务函数为 PIT0_IRQHandler
    set_vector_handler(PIT2_VECTORn ,PIT2_IRQHandler);                 //设置 PIT2 的中断服务函数为 PIT2_IRQHandler
    set_vector_handler(PORTB_VECTORn , portb_handler);                  //超声波的外部中断服务函数为portb_handler
    set_vector_handler(UART3_RX_TX_VECTORn,UART_intur);               //设置串口接收中断函数

    enable_irq(PORTB_IRQn);
    enable_irq(PORTA_IRQn);
    enable_irq(PIT0_IRQn);
    enable_irq(PIT2_IRQn);
    uart_rx_irq_en(UART3);

    LCD_str(site,"init_end_all...", BLACK,WHITE);
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

/*
 *Description    :
 *
 *
 *DATA           :
 */
void record_ext()
{
 // flash_init();
  flash_erase_sector(ADDR);

  Ind.min_val[0]      =speedwantD_set;
  Ind.min_val[1]      =speedwantE_set;
  Ind.min_val[2]      =KP2;
  Ind.min_val[3]      =Ki2;
  Ind.min_val[4]      =Kd2;
  Ind.min_val[5]      =Kp1;
  Ind.min_val[6]      =Kp2;
  Ind.min_val[7]      =Kd;
  Ind.min_val[8]      =origin_chao_cont;
  Ind.min_val[9]      =wrz_chao_cont;
  Ind.min_val[10]     =huan_chao_cont;
  Ind.min_val[11]     =follow_huan_set[1];
  Ind.min_val[12]     =follow_huan_set[2];
  Ind.min_val[13]     =follow_huan_set[3];
  Ind.min_val[14]     =follow_huan_set[4];
  Ind.min_val[15]     =follow_huan_set[5];
//  Ind.min_val[16]     =follow_huan_set[1];
//  Ind.min_val[induc_num+17]     =;
//  Ind.min_val[induc_num+18]     =;
//  Ind.min_val[induc_num+19]     =;
//  Ind.min_val[induc_num+20]     =;

  for(uint8 i=0;i<PAR_NUM;i++)
  {
    flash_write(ADDR, 0+16*i,Ind.min_val[i]);
    flash_write(ADDR, 8+16*i,Ind.max_val[i]);
  }
}

/*
 *Description    :
 *
 *
 *DATA           :
 */
void read_ext()
{
  for(uint8 i=0;i<PAR_NUM;i++)
  {
    Ind.min_val[i]=flash_read(ADDR,0+16*i, uint16);
    Ind.max_val[i]=flash_read(ADDR,8+16*i, uint16);
  }

//Ind.min_val[0]   =80;
//Ind.min_val[1]   =80;
//Ind.min_val[2]          =114;
//Ind.min_val[3]          =85;
//Ind.min_val[4]          =10;
//Ind.min_val[5]          =14;
//Ind.min_val[6]          =0.36;
//Ind.min_val[7]           =4;

speedwantD_set   =Ind.min_val[0];
speedwantE_set   =Ind.min_val[1];
KP2          =Ind.min_val[2];
Ki2          =Ind.min_val[3];
Kd2          =Ind.min_val[4];
Kp1          =Ind.min_val[5];
Kp2          =Ind.min_val[6];
Kd           =Ind.min_val[7];
origin_chao_cont    =Ind.min_val[8];
wrz_chao_cont       =Ind.min_val[9];
huan_chao_cont      =Ind.min_val[10];
follow_huan_set[1]  =Ind.min_val[11];
follow_huan_set[2]  =Ind.min_val[12];
follow_huan_set[3]  =Ind.min_val[13];
follow_huan_set[4]  =Ind.min_val[14];
follow_huan_set[5]  =Ind.min_val[15];
//          =Ind.min_val[induc_num+16];
//          =Ind.min_val[induc_num+17];
//          =Ind.min_val[induc_num+18];
//          =Ind.min_val[induc_num+19];
//          =Ind.min_val[induc_num+20];
}



void flash_onece()
{
  flash_erase_sector(ADDR);

Ind.min_val[0]   =80;
Ind.min_val[1]   =80;
Ind.min_val[2]          =114;
Ind.min_val[3]          =85;
Ind.min_val[4]          =10;
Ind.min_val[5]          =14;
Ind.min_val[6]          =0.36;
Ind.min_val[7]           =4;

//  Ind.min_val[induc_num+8]      =;
//  Ind.min_val[induc_num+9]      =;
//  Ind.min_val[induc_num+10]     =;
//  Ind.min_val[induc_num+11]     =;
//  Ind.min_val[induc_num+12]     =;
//  Ind.min_val[induc_num+13]     =;
//  Ind.min_val[induc_num+14]     =;
//  Ind.min_val[induc_num+15]     =;
//  Ind.min_val[induc_num+16]     =;
//  Ind.min_val[induc_num+17]     =;
//  Ind.min_val[induc_num+18]     =;
//  Ind.min_val[induc_num+19]     =;
//  Ind.min_val[induc_num+20]     =;

  for(uint8 i=0;i<PAR_NUM;i++)
  {
    flash_write(ADDR, 0+16*i,Ind.min_val[i]);
    flash_write(ADDR, 8+16*i,Ind.max_val[i]);
  }
}


