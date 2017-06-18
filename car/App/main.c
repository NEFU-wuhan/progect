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
//=======================================
uint8 CAR_MODEL;    //1-�ų��� 2-2�ų��� 3-3�ų�
uint8  var1, var2;
uint16 var3, var4;
uint32 var5, var6;

uint8 front_car=0,weizhi_turn=0;
int8 tongbu[10];
int8 diff=0;
int8 cut_1=0,cut_2=0;
int8 chao_huan=0,chao_zhi=0,Ramp_tong=0;  //�˱�־Ϊ����ǰ����ͬһ�����ص�ֻ����һ�γ���

int8 sequence=0;  //����˳��Ϊ0��ʱ����ǰ�󳵱�־λ��var1��ͬ��1���෴
//��ѹ���ͼ��19200=160*120
uint8  img[160*120];                                     //��ѹ���ͼ��

//-----------�������������ǰ��
void detection_front_back();
//-----------����ǰ���NRFͨѶ
void test_communication();
void send_nrf();
void recive_nrf();
void SD_save_deal();

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
//    img_sd_init();

    //------------�������ǰ��
//    detection_front_back();
    //---------------------------
    while(1)
    {
        /************************ ͼ��ɼ�����ʾ  ***********************/
        camera_get_img();                           //����ͷ��ȡͼ��
        //     printf("%d\n",*imgbuff);
        /************************ ͼ���ѹ        ***********************/
        img_extract1(img,imgbuff);          //����ͷͼ���ѹ

        /************************ ͼ����        ***********************/
        median_line_extract(img);	            //����ͷͼ����

        /*******************      ������      ***************************/
//        if(right_lost_count < 10 && left_lost_count < 10 && mid_line <70 && mid_line > 58)
//        start_line(img);

        quan_dir_duty_new();				    //���ÿһ֡ͼ�������Ȩ��

//        hswy_sendimg(img);         //����ΰҵ��λ��ͼ����


#if nrf_debug
        /********************** ���߷��ͺͽ�������  ***********************/
        do
        {
            nrf_result = nrf_msg_rx(&com, nrf_rx_buff);
        }while(nrf_result != NRF_RESULT_RX_NO);         //���ղ������� ���˳�

        //--------����ǰ���NRFͨѶ״��---------------------
        if(Motor_En==0)
        {
          test_communication();
        }
        //--------�����������-----------
        recive_nrf();
        //--------��ʼ��������-----------
        send_nrf();

#endif


        /*********************** ������Ϣ ����  ***********************/
        deal_key_event();

        LCD_show();

//        SD_save_deal();

//        if(front_car==0 || var4!=0 || stop==1 ) measure_distance();
//        else  s_distance=550;

    }
}

//-----------�������ǰ��
void detection_front_back()
{
  uint16 ce_weizhi=0,ce_i=0;
  for(ce_i=0;ce_i<8;ce_i++)
    ce_weizhi +=(uint16)measure_distance();
  if( (ce_weizhi/8)>500 )   sequence=1;
  else sequence=0;
}

//����ǰһֱ���෢�ͱ�־���ͨѶ״��
void  test_communication()
{
  if(sequence==1 )
  {
    var5++;
    updata_var(VAR5);
    var_syn(VAR5);
    if(var5>200) var5=0;
  }
  else
  {
    var6++;
    updata_var(VAR6);
    var_syn(VAR6);
    if(var6>200) var6=0;
  }

}

//ͨ��NRFͬ������
void send_nrf()
{
  if( front_car==1 )
  {
    if (tongbu[1]>0)                                          //˫��ͨ��  (Ϊ���л�˫��ǰ��)
    {
      var_syn(VAR1);
      tongbu[1]--;
    }

    if (tongbu[2]>0)                                          //˫��ͨ��  ��Ϊ��ͬʱ����ͣ����
    {
      var_syn(VAR2);
      tongbu[2]--;
    }
    if (tongbu[3]>0)                                          //˫��ͨ��  ��Ϊ��ͬ����������������־��Ϊ1 ���־���ڽ��л���������
    {
      var_syn(VAR3);
      tongbu[3]--;
    }
    if (tongbu[4]>0)                                          //˫��ͨ��  ��Ϊ��ͬ������ֱ��������־��Ϊ1 ���־���ڽ���ֱ��������
    {
      var_syn(VAR4);
      tongbu[4]--;
    }
  }
}

//����NRF���ܵ�������
void recive_nrf()
{
  //----------------------���������ݸ���ǰ�󳵱�־λ  /////////////////////////////
  if( sequence==var1 && tongbu[1]==0 && tongbu[2]==0 && tongbu[3]==0) front_car=0;     //Ϊ��ȷ��ǰ�󳵣���־λʼ����var1ͬ��������Ҫvar1��0
  else  front_car=1;    //������ʱ��������Ϊǰ��     && var3==0

  //----------------------NRF���շ� ��ͣ�� �ĳ���----------------------
  nrf_start_stop();

  //----------------------ÿ�γ�����֮�������------------------------
  // chao_one //��Ӧ���ǻ�������
  if( chao_huan==1 && var3==0)
  {
    chao_huan=0;
    car_start( 1 );
  }
  static uint8 a=0;
  if( Ramp_tong==0 && var3==2 && Ramp_flag==0 && a==0)
  {
    Ramp_flag=1;
    var4=0;
    Ramp_Len_x=Total_distance ;
    a++;

    chao_zhi=0;
    wrz_distance=0;
    L_wrz_flag=0;
    R_wrz_flag=0;
  }
  if(var3==0) a=0;
}

//SD������ʹ���
void SD_save_deal()
{
  if( SD_save_flag==0 )       //
  {
    img_sd_save(imgbuff,CAMERA_SIZE);
  }
  if( yiting==1 &&  abs(leftval)<10 && abs(rightval)<10 )    //
  {
    SD_save_flag=1;
    img_sd_exit();
  }
}

