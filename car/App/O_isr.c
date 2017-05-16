#include "include.h"

/*!
 *  @brief      PIT2�жϷ�����
 *  @since      v5.0
 */
void PIT2_IRQHandler()        //��Ҫ�ж�ѭ��
{
    static uint8  part=0;

    part++;
    //ѭ���������ƣ���ֻ����CCD�ɼ��Ͷ���ٶȿ��ƣ������жϻ���һ�����һ��
    switch(part)
    {
      case 1:
           DJ_PID();                //�Ƚ��ж������
        break;
      case 2:
   //        GetMotoPulse();
    //     Spd_Dtc_Get();
        break;
      case 3:
         speedcontrol5();
        break;
      case 4:
        break;
      case 5:

        part=0;
        break;

      default:
        part=0;
        break;
    }
     Time_Commen();   //��¼ϵͳʱ��
//   if(Beep_En)                                                //������ʹ�ܼ�����������
//      Beep();
//   bat_voltage_measure() ;      //��ѹ����

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

    n = 29;                                             //���ж�
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
