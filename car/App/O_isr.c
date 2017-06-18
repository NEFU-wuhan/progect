#include "include.h"

/*!
 *  @brief      PIT2�жϷ�����
 *  @since      v5.0
 */
void PIT2_IRQHandler()        //��Ҫ�ж�ѭ��
{
    static uint8  part=0;
    zhenshu_count++;
    if(zhenshu_count==1000)    //1������ʾһ�Σ���Ϊ��ǰ֡��
    {
      display_zs=camera_zhenshu;
      camera_zhenshu=0;
      zhenshu_count=0;
      if(display_zs<display_zs_min ) display_zs_min=display_zs;
    }
    part++;

    //ѭ���������ƣ���ֻ����CCD�ɼ��Ͷ���ٶȿ��ƣ������жϻ���һ�����һ��
    switch(part)
    {
    case 1:
      DJ_PID();                //�Ƚ��ж������
      break;
    case 2:
      speedcontrol5();
      break;
    case 3:
      break;
    case 4:

      break;
    case 5:
//      bat_voltage_measure() ;      //��ѹ����

      part=0;
      break;

    default:
      part=0;
      break;
    }
     Time_Commen();   //��¼ϵͳʱ��
//   if(Beep_En)                                                //������ʹ�ܼ�����������
//      Beep();

   PIT_Flag_Clear(PIT2);
}

/*!
 *  @brief      PORTA�жϷ�����
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n;    //���ź�
    uint32 flag;

    //while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //���жϱ�־λ

    n = 29; //���ж�
    if(flag & (1 << n))                                 //PTA29�����ж�
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //ʹ�����ж�
    n = 28;
    if(flag & (1 << n))                                 //PTA28�����ж�
    {
        camera_href();
    }
#endif

}



/*!
 *  @brief      DMA0�жϷ�����
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}

/*!
 *  @brief      PIT0�жϷ�����
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

/******************nrf�ж�*******************/
void PORTE_IRQHandler()
{
    uint8  n;    //���ź�
    uint32 flag;

    flag = PORTE_ISFR;
    PORTE_ISFR  = ~0;                                        //���жϱ�־λ

    n = 27;
    if(flag & (1 << n))                                      //PTE27�����ж�
    {
        nrf_handler();
    }
}

///////////******�������ⲿ�жϷ�����*******//////////
void portb_handler()
{
  PORT_FUNC(B,22,measure_distance);        //A  24
}

