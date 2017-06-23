 /*!
  *     COPYRIGHT NOTICE
  *     Copyright (c) 2013,ɽ��Ƽ�
  *     All rights reserved.
  *     �������ۣ�ɽ����̳ http://www.vcan123.com
  *
  *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
  *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
  *
  * @file       main.c
  * @brief      ɽ��K60 ƽ̨������
  * @author     ɽ��Ƽ�
  * @version    v5.0
  * @date       2013-08-28
  */

#include "common.h"
#include "include.h"

uint8  nrf_rx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //Ԥ��
//uint8  nrf_tx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //Ԥ��
uint8 *imgbuff = (uint8 *)(((uint8 *)&nrf_rx_buff) + COM_LEN);     //ͼ���ַ


//��������
void PIT0_IRQHandler();
void PORTE_IRQHandler();


uint8  var1 , var2;
uint16 var3, var4;
uint32 var5, var6;

int16 var_wire[6]={0};

//����NRF���ܵ�������
void recive_nrf()
{
  var_wire[0]=var3;               ///  uint16
  var_wire[1]=var5;               //uint32
//  var_wire[2]=var3;
//  var_wire[3]=var4;
//  var_wire[4]=var5;
//  var_wire[5]=var6;
  if(var1==1) var_wire[3]=-var4;
  else var_wire[3]=var4;
  if(var2==1) var_wire[5]=-var6;
  else var_wire[5]=var6;
}

/*!
 *  @brief      main����
 *  @since      v5.0
 *  @note       ɽ�� ���ߵ��� ����ʵ��
 */
void  main(void)
{
    Site_t site     = {0, 0};                           //��ʾͼ�����Ͻ�λ��
    Size_t imgsize  = {CAMERA_W, CAMERA_H};             //ͼ���С
    Size_t size     = {80, 60}; //{LCD_W, LCD_H};       //��ʾ����ͼ���С
    uint32 i;
    com_e     com;
    nrf_result_e nrf_result;

    /************************ ���� K60 �����ȼ�  ***********************/
    //K60 ��Ĭ�����ȼ� ��Ϊ 0
    //�ο����ӣ������ж�Ƕ�׵����� - ���ܳ������� - ɽ����̳
    //          http://www.vcan123.com/forum.php?mod=viewthread&tid=499&page=1#pid3270
    NVIC_SetPriorityGrouping(4);            //�������ȼ�����,4bit ��ռ���ȼ�,û�������ȼ�

    NVIC_SetPriority(PORTE_IRQn,0);         //�������ȼ�
    NVIC_SetPriority(PIT0_IRQn,1);          //�������ȼ�


    /************************ LCD Һ���� ��ʼ��  ***********************/
    LCD_init();

    /************************ ����ģ�� NRF ��ʼ��  ***********************/
    while(!nrf_init());
    //�����жϷ�����
    set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);    			//���� PORTE ���жϷ�����Ϊ PORTE_VECTORn
    enable_irq(PORTE_IRQn);
    nrf_msg_init();                                                     //����ģ����Ϣ��ʼ��


    /*********************** ������Ϣ ��ʼ��  ***********************/
    key_event_init();                   //������Ϣ��ʼ��
    pit_init_ms(PIT0,10);               //pit ��ʱ�ж�(���ڰ�����ʱɨ��)
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);    //���� PIT0 ���жϷ�����Ϊ PIT0_IRQHandler
    enable_irq(PIT0_IRQn);

    while(1)
    {
//      printf("666");
        /************************ ���߷��ͺͽ�������  ***********************/
        do
        {
            nrf_result = nrf_msg_rx(&com, nrf_rx_buff);

        }while(nrf_result != NRF_RESULT_RX_NO);
        //--------�����������-----------
        recive_nrf();
        vcan_sendware(var_wire, sizeof(var_wire));

        /*********************** ������Ϣ ����  ***********************/
        deal_key_event();

    }
}

/*

֪ʶ�� ����;

1.ӥ�� ����ͷ �ɼ�������ͼ���� ѹ��ͼ����Ҫ���н�ѹ���ο����� ����:
        ӥ��ͼ���ѹ��תΪ ��ά���� - ���ܳ������� - ɽ����̳
        http://www.vcan123.com/forum.php?mod=viewthread&tid=17


2.ͼ�� ��ַ ת��Ϊ ��ά���顣
        ����� ӥ�ۣ��ο� ��1��֪ʶ��˵��

        ��������� �Ҷ�ͼ������ͷ���޸����´���:
        uint8 *imgbuff = (uint8 *)(((uint8 *)&nrf_buff) + COM_LEN);     //ͼ���ַ
        ��Ϊ
        uint8 (*imgbuff)[CAMERA_H][CAMERA_W] =   (uint8 (*)[CAMERA_H][CAMERA_W])(((uint8 *)&nrf_buff) + COM_LEN);     //ͼ���ַ
        ʵ���� ����ǿ�� ����ת����

3.����ͷ�Ĳɼ���ַ ���� ����ͷ��ʼ���Ǵ��ݽ�ȥ�ģ��˺��ͼ�񶼲ɼ����õ�ַ�ϡ�
        camera_init(imgbuff);                                   //����ͷ��ʼ������ͼ��ɼ��� imgbuff ��ַ

4.����ģ�飬������ݽ��պ󣬲Ž��� ���ͣ��������Ծ������� ����ģ��ͬʱ���� ����״̬��

5.Ƕ���жϵ����⣬�ο���������:
        �����ж�Ƕ�׵����� - ���ܳ������� - ɽ����̳
        http://www.vcan123.com/forum.php?mod=viewthread&tid=499&page=1#pid3270

6.����ɨ�裬�ǲ��ö�ʱɨ�跽ʽ����ɨ���


*/

/*!
 *  @brief      PORTE�жϷ�����
 *  @since      v5.0
 */
void PORTE_IRQHandler()
{
    uint8  n;    //���ź�
    uint32 flag;

    flag = PORTE_ISFR;
    PORTE_ISFR  = ~0;                                   //���жϱ�־λ

    n = 27;
    if(flag & (1 << n))                                 //PTE27�����ж�
    {
        nrf_handler();
    }
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

