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

long int speedwant=0;

//===============���ñ���===============
//�ٶ�
int16 speedwantD_set=100;
int16 speedwantE_set=100;
//�����߳������Ʊ�־
uint8 origin_chao_cont=  0 ;
//ֱ����������
uint8 wrz_chao_cont=     0 ;
//�����������Ʊ�־
uint8 huan_chao_cont=    0 ;
//SD������ģʽ
uint8 SD_op_mode=        0 ;     //  0-����ͼ��  1-������ͼ��  2-������ͼ��(������ʱδд������ֱ�ӵ�������)
//������������
uint8 chao_car_cnt_set=  5 ;
//=======================================
uint8 CAR_MODEL;    //1-�ų��� 2-2�ų��� 3-3�ų�
uint8  var1, var2;
uint16 var3, var4;
uint32 var5, var6;
int16 var_wire[6]={0};
uint8 chao_cnt_total=0;
uint8 front_car=0,weizhi_turn=0;
int8 tongbu[10];
int8 diff=0;
int8 cut_1=0,cut_2=0;
int8 chao_huan=0,chao_zhi=0,Ramp_tong=0;  //�˱�־Ϊ����ǰ����ͬһ�����ص�ֻ����һ�γ���

char info=0;

int8 sequence=0;  //����˳��Ϊ0��ʱ����ǰ�󳵱�־λ��var1��ͬ��1���෴
//��ѹ���ͼ��19200=160*120
uint8  img[160*120];                                     //��ѹ���ͼ��

//-----------�������������ǰ��
void detection_front_back()
{
  if( gpio_get(receive_flag)==0 )   front_car=1;
  else front_car=0;
}
//����NRF���ܵ�������
void recive_nrf()
{
  var_wire[0]=var1;
  var_wire[1]=var2;
//  var_wire[2]=var3;
//  var_wire[3]=var4;
//  var_wire[4]=var5;
//  var_wire[5]=var6;
  if(var3==1) var_wire[3]=-var4;
  else var_wire[3]=var4;
  if(var5==1) var_wire[5]=-var6;
  else var_wire[5]=var6;
}
//NRF��������
void send_nrf()
{
  //var1��uint8�� ������ʾͼ��ÿ�λ�һ��ͼ�����һ�����壬
  //������� fiag_huan   ���� 2-3 ת��
  if( fiag_huan )
  {
    if(var3==2) var3=3;
    else var3=2;
  }
  else
  {
    if(var3==0) var3=1;
    else var3=0;
  }
  //var2(uint8)  ������ʾ ��ǰ�ٶ� speedwant
  var5=speedwant;
  //var3(uint16) ������ʾ ����PWM������
  //var4(uint16) ������ʾ ����PWM��
  if(speedaboutangle2<0)
  {
    var1=1;
    var4=abs(speedaboutangle2);
  }
  else
  {
    var1=0;
    var4=speedaboutangle2;
  }
  //var5(uint32) ������ʾ ����PWM������
  //var6(uint32) ������ʾ ����PWM��
  if(speedaboutangle<0)
  {
    var2=1;
    var6=abs(speedaboutangle);
  }
  else
  {
    var2=0;
    var6=speedaboutangle;
  }
  updata_var(VAR1);
  updata_var(VAR2);
  updata_var(VAR3);
  updata_var(VAR4);
  updata_var(VAR5);
  updata_var(VAR6);
  var_syn(VAR_MAX);       //ͬ��ȫ�� ,������ͬ������ʾȫ������Ϊ�п���ͬ��ʧ�ܡ�
}
//SD������ʹ���
void SD_save_deal()
{
  if(( SD_save_flag==0 || SD_save_flag==1) && Motor_En==1 )       //
  {
    img_sd_save(imgbuff,CAMERA_SIZE);
  }
  if( yiting==1 &&  abs(leftval)<10 && abs(rightval)<10 )    //
  {
    SD_save_flag=1;
  }
}
//���������ղ�������ʱ��������
void ceju_zhi0()
{
  static uint8 n_cut=0;
  if(gpio_get(receive_flag)==0)
  {
    n_cut++ ;
    if( n_cut>10)
    {
      n_cut=0;
      pit_close (PIT3);  //�رռ���������ʱ����ֵ���㣩
      timevar=0;
      s_distance=0;
    }
  }
  else
  {
    n_cut=0;
    //�ܽ��յ�����ʱ�Զ�У׼ǰ�󳵣�����������⣩
    if( front_car==1 && L_wrz_flag==0 && R_wrz_flag==0 && fiag_huan==0 ) uart_putchar(UART3,turn_car);
  }
}

/*!
 *  @brief      main����
 *  @since      v5.0
 *  @note       ɽ�� ���ߵ��� ����ʵ��
 */
void  main(void)
{
    com_e     com;                               //����ͨѶ
    nrf_result_e nrf_result;

    init_sys();     //�ܳ�ʼ��
    if(boma[5]==1)  img_sd_init( SD_op_mode );

    //------------�������ǰ��
    detection_front_back();
    //---------------------------
    while(1)
    {
//        uart_putchar(UART3,Start);
        /************************ ͼ��ɼ�����ʾ  ***********************/
        if(SD_op_mode==0) camera_get_img();                           //����ͷ�ɼ���ȡͼ��
        else img_sd_read( buff, CAMERA_SIZE , zhizhen );              //SD����ȡͼ��
        if( zhizhen<((picture_sum-1)*CAMERA_SIZE+4) && flag_key_select==1 && flag_key_l_u_1==1 && boma[5]==1) zhizhen+=CAMERA_SIZE;
//            printf("666");

        /************************ ͼ���ѹ        ***********************/
        if(SD_op_mode==0) img_extract1(img,imgbuff);          //����ͷͼ���ѹ
        else img_extract1(img,buff);                          //SD��  ͼ���ѹ

        /************************ ͼ����        ***********************/
        median_line_extract(img);

        /*******************      ������      ***************************/
        if(Ramp_flag==0) start_line(img);

        /*******************      �ϰ�      ***************************/
        if(never_obstacle_flag==0&&Ramp_flag==0&&fiag_huan==0&&obstancle_acc==0&&shi_zi_count==0)
        {
//          avoid_obstacle(img);                    //����ϰ�
        }

        quan_dir_duty_new();				    //���ÿһ֡ͼ�������Ȩ��

//        hswy_sendimg(img);         //����ΰҵ��λ��ͼ����

#if nrf_debug
        /********************** ���߷��ͺͽ�������  ***********************/
        if( 0 )     //Ϊ 0 ��ʾ���Ͷ�  1 ��ʾ���ն�
        {
          do
          {
            nrf_result = nrf_msg_rx(&com, nrf_rx_buff);
          }while(nrf_result != NRF_RESULT_RX_NO);         //���ղ������� ���˳�

          //--------�����������-----------
        recive_nrf();
//        vcan_sendware(var_wire, sizeof(var_wire));
        }
        else
        //--------��ʼ��������-----------
         send_nrf();
#endif


        /*********************** ������Ϣ ����  ***********************/
        deal_key_event();

        LCD_show();

        if(SD_op_mode==0 && boma[5]==1)  SD_save_deal();

        Ramp_mesure();    //�µ�ʶ��

        ceju_zhi0();      //�������

    }
}



