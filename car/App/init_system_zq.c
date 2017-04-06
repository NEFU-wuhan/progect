#include "include.h"
#include "common.h"

uint8  nrf_rx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //Ԥ��
uint8  nrf_tx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //Ԥ��
uint8 *imgbuff = (uint8 *)(((uint8 *)&nrf_tx_buff) + COM_LEN);     //ͼ���ַ  //����洢����ͼ�������,2400=160*120/8

/*******************************************************************************
* Function Name: init();
* Input Param  : ��
* Output Param : ��
* Description  : ϵͳ��ʼ��
* Author Data  : 2015/1/12
******************************************************************************/
void init_sys(void)
 {
    /************************ ���  ���   ��ʼ��  ***********************/
    ftm_pwm_init(S3010_FTM, S3010_CH,S3010_HZ,7880);   // ���
    ftm_pwm_init(FTM0, FTM_CH4,10000, 0);    //������ת
    ftm_pwm_init(FTM0, FTM_CH5,10000, 0);    //������ת
    ftm_pwm_init(FTM0, FTM_CH6,10000, 0);    //�ҵ����ת
    ftm_pwm_init(FTM0, FTM_CH7,10000, 0);    //�ҵ����ת

    /****************    ��������ʼ��     *******************/
    ftm_quad_init(FTM1);
    ftm_quad_init(FTM2);
    /************************ ���� K60 �����ȼ�  ***********************/
    NVIC_SetPriorityGrouping(5);            //�������ȼ�����,4bit ��ռ���ȼ�,û�������ȼ�
    //����ͷ���ж���ߣ�pit��֮��dma��֮
    NVIC_SetPriority(PORTA_IRQn,0);         //�������ȼ�     ����ͷ���жϺͳ��ж�
    NVIC_SetPriority(DMA0_IRQn,1);          //�������ȼ�     ����ͷ���ݴ���
    NVIC_SetPriority(PIT2_IRQn,2);          //�������ȼ�     ���߶�ʱ
    NVIC_SetPriority(PORTE_IRQn,4);         //�������ȼ�     ���ߴ����ж�
    NVIC_SetPriority(PIT0_IRQn,3);          //�������ȼ�     �����ж�

    /************************ IO���ų�ʼ��  ***********************/
    gpio_init(ECHG,GPI,0);            //�������   �ܽų�ʼ��
    gpio_init(receive_flag,GPI,0);    //������ձ�־λ

//    gpio_init (PTD3, GPI,1);                                                    //��ʼ�� ���뿪��1
//    gpio_init (PTC6, GPI,1);                                                    //��ʼ�� ���뿪��2
//    gpio_init (PTC7, GPI,1);                                                    //��ʼ�� ���뿪��3
//    gpio_init (PTD2, GPI,1);                                                    //��ʼ�� ���뿪��4

//    gpio_init (PTC3, GPO,0);           //�������ܽų�ʼ��

    adc_init (ADC0_DM1);                                                        //��ѹ������ADC����
//    adc_init(ADC0_SE9);                                                         //ADC��ʼ��(���� �����ǻ���ٶȼ�)
    /************************ ����ͷ ��ʼ��  ***********************/
    camera_init(imgbuff);                                   //����ͷ��ʼ������ͼ��ɼ��� imgbuff ��ַ

    /************************** LED ��ʼ��  ***************************/
    led_init(LED_MAX);

    /*******************  ����ģ���ʼ��,Ԥ����    *************************/
     #if nrf_debug
      NRF_INIT();
     #endif

    /************************ LCD Һ���� ��ʼ��  ***********************/
    LCD_init();

     /*Һ��ͼƬ��ʼ��*/
    bmp_init();
    bmp_display();
    DELAY_MS(500);
    /***********************   ������Ϣ ��ʼ��    ***********************/
    key_event_init();

    /***********************       PIT��ʱ        ***********************/
    pit_init_ms(PIT0,10);      //pit0 ���ڰ�����ʱɨ��
    pit_init_ms(PIT2,1);       //pit2 ��ʱ�жϣ����߶�ʱ

    /***********************   �����жϷ�����    ***********************/
    set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //���� PORTA ���жϷ�����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //���� DMA0 ���жϷ�����Ϊ DMA0_IRQHandler
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);                  //���� PIT0 ���жϷ�����Ϊ PIT0_IRQHandler
    set_vector_handler(PIT2_VECTORn ,PIT2_IRQHandler);                 //���� PIT2 ���жϷ�����Ϊ PIT2_IRQHandler

    enable_irq(PORTA_IRQn);
    enable_irq(PIT0_IRQn);
    enable_irq(PIT2_IRQn);

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