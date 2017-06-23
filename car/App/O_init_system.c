#include "include.h"
#include "common.h"

uint8  nrf_rx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //Ԥ��
uint8  nrf_tx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //Ԥ��
uint8 *imgbuff = (uint8 *)(((uint8 *)&nrf_tx_buff) + COM_LEN);     //ͼ���ַ  //����洢����ͼ�������,2400=160*120/8
uint8 buff[CAMERA_SIZE];
Ind_msg Ind ={0};

void NRF_INIT(void);
void flash_onece();
void read_ext();
void record_ext();
//��ʼ������
void init_canshu()
{
  //�������
  if( CAR_MODEL==1 )
  {
    KP2=130;      //����ϵ��500
    Ki2=30;      //����ϵ��220
    Kd2=10;
  }
  else if(CAR_MODEL==2 )
  {
    KP2=100;      //����ϵ��500
    Ki2=30;      //����ϵ��220
    Kd2=10;
  }
  else if(CAR_MODEL==3 )
  {
    KP2=120;      //����ϵ��500
    Ki2=40;      //����ϵ��220
    Kd2=10;
  }
  else
  {
    KP2=120;      //����ϵ��500
    Ki2=85;      //����ϵ��220
    Kd2=10;
  }
  //�������
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
  //���ó��ڼ�������Ѱ����ʻ����
  follow_huan_set[1]=0;   // 1 ΪѰ���ߣ�0ΪѰ���� 2Ϊ������Ѱ���ߣ�3Ϊ������Ѱ����
  follow_huan_set[2]=0;
  follow_huan_set[3]=1;
  follow_huan_set[4]=1;
  follow_huan_set[5]=1;
}

//ͨ�Ŵ�����
void cmc(char mes)
{
  if(mes==Start)//����
  {
    uart_start_stop(1);
  }
  else if(mes==Stop) //ͣ��
  {
    uart_start_stop(0);
  }
  else if(mes==turn_car) //�л�ǰ��
  {
    if(front_car==1)  front_car=0;
    else front_car=1;

  }
  else if(mes==Ramp) //�µ�
  {
    Ramp_flag=1;
    Ramp_yushibie=1;
    add_count(cut_2,200);
  }
  else if(mes==huan_right) //������Ѳ��
  {
    fiag_huan_yu=1;
    if( follow_huan_set[hehe+1]==1 )
    {
      if(chao_cnt_total<=chao_car_cnt_set)
      {
        add_count(chao_cnt_total,200);  //��¼�ܳ�������
      }
      else
      {
        follow_huan=1;                  //����Ѱ����
      }
    }
    else if( follow_huan_set[hehe+1]==2 )
    {
      follow_huan=1;                  //������Ѱ����
    }
    else if( follow_huan_set[hehe+1]==3 )
    {
      follow_huan=0;                  //������Ѱ����
    }
    else
    {
      if(chao_cnt_total<=chao_car_cnt_set)
      {
        follow_huan=1;                  //����Ѱ����
        add_count(chao_cnt_total,200);  //��¼�ܳ�������
      }
      else
      {
         follow_huan=0;                  //����Ѱ����
      }
    }
  }
  else if(mes==huan_left)  //������Ѳ��
  {
    fiag_huan_yu=1;
    if( follow_huan_set[hehe+1]==1 )
    {
      if(chao_cnt_total<=chao_car_cnt_set)
      {
        follow_huan=0;                  //����Ѱ����
        add_count(chao_cnt_total,200);  //��¼�ܳ�������
      }
      else
      {
        follow_huan=1;                  //����Ѱ����
      }
    }
    else if( follow_huan_set[hehe+1]==2 )
    {
      follow_huan=1;                  //������Ѱ����
    }
    else if( follow_huan_set[hehe+1]==3 )
    {
      follow_huan=0;                  //������Ѱ����
    }
    else
    {
      if(chao_cnt_total<=chao_car_cnt_set)
      {
        follow_huan=1;                  //����Ѱ����
        add_count(chao_cnt_total,200);  //��¼�ܳ�������
      }
      else
      {
        follow_huan=0;                  //����Ѱ����
      }
    }
  }
  else if(mes==huan_finish) //�������
  {
    chao_huan=0;
//    fiag_huan_yu=0;
    car_start( 1 );
  }
  else if(mes==ZC_right) //ֱ���Ҳ೬��
  {
    R_wrz_flag=1;
  }
  else if(mes==ZC_left)  //ֱ����೬��
  {
    L_wrz_flag=1;
  }
  else if(mes==ZC_OK)   //ֱ���������
  {
    add_count(chao_cnt_total,200);  //��¼�ܳ�������
    L_wrz_flag=0;
    R_wrz_flag=0;
  }
  else if(mes==Block_right)  //�ϰ����Ҳ�
  {
  }
  else if(mes==Block_left)   //�ϰ������
  {
  }
}

void UART_intur()//���ڽ����жϴ�����
{
  if(UART_S1_REG(UARTN[UART3]) & UART_S1_RDRF_MASK)   //�������ݼĴ�����
  {
    uart_getchar(UART3,&info);
    cmc(info);
  }
}

/*******************************************************************************
* Function Name: init();
* Input Param  : ��
* Output Param : ��
* Description  : ϵͳ��ʼ��
* Author Data  : 2015/1/12
******************************************************************************/
void init_sys(void)
 {
    Site_t site;
    site.x = 13;
    site.y = 3;

    /************************ L CD Һ���� ��ʼ��  ***********************/
    LCD_init();

     /*Һ��ͼƬ��ʼ��*/
    bmp_init();
    bmp_display();

    /************************ ���  ���   ��ʼ��  ***********************/
    LCD_str(site,"duo_ji_init...", BLACK,WHITE);
    ftm_pwm_init(S3010_FTM, S3010_CH,S3010_HZ,Mid_duty);   // ���

    LCD_str(site,"dian_j_init...", BLACK,WHITE);
    ftm_pwm_init(FTM0, FTM_CH4,10000, 0);    //������ת
    ftm_pwm_init(FTM0, FTM_CH5,10000, 0);    //������ת
    ftm_pwm_init(FTM0, FTM_CH6,10000, 0);    //�ҵ����ת
    ftm_pwm_init(FTM0, FTM_CH7,10000, 0);    //�ҵ����ת

    /****************    ��������ʼ��     *******************/
    LCD_str(site,"bian_m_init...", BLACK,WHITE);
    ftm_quad_init(FTM1);
    ftm_quad_init(FTM2);

    /************************ IO���ų�ʼ��  ***********************/
    LCD_str(site,"ce_ju_init....", BLACK,WHITE);
    gpio_init(receive_flag,GPI,0);    //������ձ�־λ
    gpio_ddr (ECHG,GPI);//��ʼ������ģ���������������
    port_init (ECHG, IRQ_EITHER | PF | ALT1 | PULLUP ); //�������жϴ��������������裨�ܷ����������

    //*******���뿪�س�ʼ��******//
    LCD_str(site,"bo_ma_init....", BLACK,WHITE);
    gpio_init (PTE9,GPI,0); //���                                             //��ʼ�� ���뿪��1
    boma[7]=gpio_get(PTE9);
    gpio_init (PTE7,GPI,0);                                                     //��ʼ�� ���뿪��2
    boma[6]=gpio_get(PTE7);
    gpio_init (PTE8,GPI,0);                                                     //��ʼ�� ���뿪��3
    boma[5]=gpio_get(PTE8);
    gpio_init (PTE6,GPI,0);  //��ǰ                                            //��ʼ�� ���뿪��4
    boma[4]=gpio_get(PTE6);

    gpio_init (PTA9,GPI,0); //����                                             //��ʼ�� ���뿪��1
    boma[3]=gpio_get(PTA9);
    gpio_init (PTA7,GPI,0);                                                     //��ʼ�� ���뿪��2
    boma[2]=gpio_get(PTA7);
    gpio_init (PTA5,GPI,0);                                                     //��ʼ�� ���뿪��3
    boma[1]=gpio_get(PTA5);
    gpio_init (PTE27,GPI,0);  //����                                            //��ʼ�� ���뿪��4
    boma[0]=gpio_get(PTE27);

    if( boma[0]==0 ) CAR_MODEL=1;
    else if( boma[1]==0) CAR_MODEL=2;
    else if( boma[2]==0) CAR_MODEL=3;
    else CAR_MODEL=4;

//    LCD_str(site,"buzzer_init...", BLACK,WHITE);
////    gpio_init (PTC3, GPO,0);           //�������ܽų�ʼ��
//
//    LCD_str(site,"dian_y_init...", BLACK,WHITE);
//    adc_init (ADC0_DM1);                                                      //��ѹ������ADC����
    adc_init (ADC1_DM1);                                                        //�µ�������ADC����
////    adc_init(ADC0_SE9);                                                         //ADC��ʼ��(���� �����ǻ���ٶȼ�)
    /************************ ����ͷ ��ʼ��  ***********************/
    LCD_str(site,"camera_init...", BLACK,WHITE);
    camera_init(imgbuff);                                   //����ͷ��ʼ������ͼ��ɼ��� imgbuff ��ַ
    LCD_str(site,"camera_end...", BLACK,WHITE);

    /************************** LED ��ʼ��  ***************************/
//    led_init(LED_MAX);

    /*******************  ����ģ���ʼ��,Ԥ����    *************************/
    #if nrf_debug
      LCD_str(site,"NRF_init....", BLACK,WHITE);
      NRF_INIT();
    #endif

    /***********************   ������Ϣ ��ʼ��    ***********************/
    LCD_str(site,"key_init....", BLACK,WHITE);
    key_event_init();

    /***********************   ���� ��ʼ��        ***********************/
    init_canshu();   //ע������еĲ����ᱻFLASH����

    /************************ Flash ��ʼ��  ***********************/
    LCD_str(site,"flash_init....", BLACK,WHITE);
    flash_init();
//      flash_onece();
    DELAY_MS(150);

    if( boma[4]==1 )    //���뿪�ؿ����Ƿ��ȡFLASH
    {
      read_ext();
    }

    /***********************   ����ͨ�� ��ʼ��    ***********************/
    LCD_str(site,"UART_init...", BLACK,WHITE);
    uart_init (UART3, 57600);

    /***********************       PIT��ʱ        ***********************/
    LCD_str(site,"vector_init...", BLACK,WHITE);
    pit_init_ms(PIT0,10);      //pit0 ���ڰ�����ʱɨ��
    pit_init_ms(PIT2,1);       //pit2 ��ʱ�жϣ����߶�ʱ

    /************************ ���� K60 �����ȼ�  ***********************/
    NVIC_SetPriorityGrouping(5);            //�������ȼ�����,4bit ��ռ���ȼ�,û�������ȼ�
    //����ͷ���ж���ߣ�pit��֮��dma��֮
    NVIC_SetPriority(PORTA_IRQn,0);         //�������ȼ�     ����ͷ���жϺͳ��ж�
    NVIC_SetPriority(DMA0_IRQn,1);          //�������ȼ�     ����ͷ���ݴ���
    NVIC_SetPriority(PIT2_IRQn,2);          //�������ȼ�     ���߶�ʱ
    NVIC_SetPriority(PORTE_IRQn,4);         //�������ȼ�     ���ߴ����ж�
    NVIC_SetPriority(PIT0_IRQn,3);          //�������ȼ�     �����ж�
    NVIC_SetPriority(PORTB_IRQn,6);          //�������ȼ�     �������ж�
    set_irq_priority(UART3_RX_TX_IRQn,5);//���ڽ����ж�

    /***********************   �����жϷ�����    ***********************/
    set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //���� PORTA ���жϷ�����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //���� DMA0 ���жϷ�����Ϊ DMA0_IRQHandler
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);                  //���� PIT0 ���жϷ�����Ϊ PIT0_IRQHandler
    set_vector_handler(PIT2_VECTORn ,PIT2_IRQHandler);                 //���� PIT2 ���жϷ�����Ϊ PIT2_IRQHandler
    set_vector_handler(PORTB_VECTORn , portb_handler);                  //���������ⲿ�жϷ�����Ϊportb_handler
    set_vector_handler(UART3_RX_TX_VECTORn,UART_intur);               //���ô��ڽ����жϺ���

    enable_irq(PORTB_IRQn);
    enable_irq(PORTA_IRQn);
    enable_irq(PIT0_IRQn);
    enable_irq(PIT2_IRQn);
    uart_rx_irq_en(UART3);

    LCD_str(site,"init_end_all...", BLACK,WHITE);
 }


/************************ ����ģ�� NRF ��ʼ��  ********************/
void NRF_INIT(void)
 {
    uint32 i;

   /************************ ����ģ�� NRF ��ʼ��  ***********************/
    while(!nrf_init());
    //�����жϷ�����
    set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);                //���� PORTE ���жϷ�����Ϊ PORTE_VECTORn
    enable_irq(PORTE_IRQn);
    nrf_msg_init();                                                     //����ģ����Ϣ��ʼ��

    //����ͷģ�飬��Ҫ ���� ������� ���԰�ģ�飬����������� ����ͼ��ʧ�ܶ�������������
    i = 20;
    while(i--)
    {
        nrf_msg_tx(COM_RETRAN,nrf_tx_buff);                              //���Ͷ�� �� �����ȥ��������� ���ն˵Ļ�������
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


