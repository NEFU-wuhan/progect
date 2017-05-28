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
int16 speedwantD_set=80;
int16 speedwantE_set=80;
uint8 CAR_MODEL;    //1-�ų��� 2-2�ų��� 3-3�ų�
uint8  var1, var2;
uint16 var3, var4;
uint32 var5, var6;

uint8 front_car=0,weizhi_turn=0;
int8 tongbu[10];
int8 diff=0;
int8 input=0,cut_1=0,cut_2=0;
int8 chao_one=0,chao_one2=0;  //�˱�־Ϊ����ǰ����ͬһ�����ص�ֻ����һ�γ���

int8 sequence=0;  //����˳��Ϊ0��ʱ����ǰ�󳵱�־λ��var1��ͬ��1���෴
//��ѹ���ͼ��19200=160*120
uint8  img[160*120];                                     //��ѹ���ͼ��
/*!

 *  @brief      main����
 *  @since      v5.0
 *  @note       ɽ�� ���ߵ��� ����ʵ��
 */
void  main(void)
{

    com_e     com;                                                             //����ͨѶ
    nrf_result_e nrf_result;

    init_sys();     //�ܳ�ʼ��
    //-----------�������ǰ��
    uint16 ce_weizhi=0,ce_i=0;
    for(ce_i=0;ce_i<8;ce_i++)
        ce_weizhi +=(uint16)measure_distance();
    if( (ce_weizhi/8)>500 )   sequence=1;
    else sequence=0;
    //---------------------------
    while(1)
    {
        /************************ ͼ��ɼ�����ʾ  ***********************/
        camera_get_img();                           //����ͷ��ȡͼ��

        /************************ ͼ���ѹ        ***********************/
        img_extract1(img,imgbuff);          //����ͷͼ���ѹ

        /************************ ͼ����        ***********************/
        median_line_extract(img);	            //����ͷͼ����

        quan_dir_duty_new();				    //���ÿһ֡ͼ�������Ȩ��

//        hswy_sendimg(img);         //����ΰҵ��λ��ͼ����


#if nrf_debug
        input=var2;
        /************************ ���߷��ͺͽ�������  ***********************/
        do
        {
            nrf_result = nrf_msg_rx(&com, nrf_rx_buff);
            if(nrf_result == NRF_RESULT_RX_VALID)
            {
                switch(com)
                {
                    case COM_IMG:

                        break;

                    case COM_VAR:
                        if(last_tab == CAR_CTRL)                    //����� ���� С�� ����
                        {
                            switch(car_ctrl_get())                  //��ȡС��������������д���
                            {
                            case CAR_START:                         //����
//                                     DJ_protect=0;
//                                     Motor_En=1;
//                                     speedwantB=70;
                                break;

                            case CAR_STOP:                          //ͣ��
//                                      speedwant=0;
//                                      ftm_pwm_init(FTM0, FTM_CH4,10000, 0);    //������ת
//                                      ftm_pwm_init(FTM0, FTM_CH5,10000, 0);    //������ת
//                                      ftm_pwm_init(FTM0, FTM_CH6,10000, 0);    //�ҵ����ת
//                                      ftm_pwm_init(FTM0, FTM_CH7,10000, 0);    // //�ҵ����ת
//                                      speedwantB=0;
                                break;
                             case  CAR_FORWARD:                               //ǰ��
//                                     Motor_En=1;
//                                     speedwantB=speedwantB+10;
//                                     DJ_protect=0;
                                break;
                             case  CAR_BACK:                               //ǰ��
//                                     speedwantB=speedwantB-10;
                                break;

                            default:
                                //��Ч���������κδ���
                                break;
                            }
                        }

                        break;
                    default:
                        break;
                }
            }
        }while(nrf_result != NRF_RESULT_RX_NO);         //���ղ������� ���˳�

        if(sequence==1)
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

        //////////////////     �����������־λ  /////////////////////////////
        if( sequence==var1 && var3==0 && tongbu[1]==0 && tongbu[2]==0 && tongbu[3]==0) front_car=0;     //Ϊ��ȷ��ǰ�󳵣���־λʼ����var1ͬ��������Ҫvar1��0
        else  front_car=1;    //������ʱ��������Ϊǰ��

        if(input!=var2)
        {
            key_flag_clear=1;
            if(var2==1)     //��ʾ����
            {
              DJ_protect=0;
              flag_key_select=5;
              flag_key_l_u_0=0;
              Motor_En=1;
              speedwantD=speedwantD_set;
              speedwantE=speedwantE_set;
            }
            else          //��ʾͣ��
            {
              flag_key_select=0;
              fache=0;
              speedwantD=0;
              speedwantE=0;
            }
        }

        if( front_car==1 && chao_one2==0)
        {
          if( var3==0 )
          {
            if( chao_one==1 && fiag_huan==0)  chao_one=0;
            speedwantD=speedwantD_set;
            speedwantE=speedwantE_set;
          }
        }

        ///////////////////   ��ʼ��������   /////////////////////////////////
        uint8 z=0;


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
          if (tongbu[3]>0)                                          //˫��ͨ��  ��Ϊ��ͬ������������־��Ϊ1 ���־���ڽ��л���������
          {
            var_syn(VAR3);
            tongbu[3]--;
          }
          if (tongbu[4]>0)                                          //˫��ͨ��  ��Ϊ��ͬ������������־��Ϊ1 ���־���ڽ��л���������
          {
            var_syn(VAR4);
            tongbu[4]--;
          }
        }

//        if(weizhi_turn==1)
//        {
//          if(var3==0  //�����������Ϊ����ǰ��ʶ�𳬳���־����ʼ�������Ҳ��ú��ٽ��룩
//             && chao_one==0 )  //Ϊ��ǰ��ֻ����һ���л���־����ֹͣ����ʱ��һֱ��������һֱ�л����������֮�������־
//          {
//            chao_one=1;
//            speedwantD=0;
//            speedwantE=0;
//
//            var3=1;         //���һ����л���־ͬ������ʾ�����л����л���ɺ����㣩
//            updata_var(VAR3);
//            tongbu[3]=10;
//
//            var1=1;         //�����л�ǰ��
//            updata_var(VAR1);
//            tongbu[1]=10;
//          }
//        }
//        else if(weizhi_turn==2)
//        {
//          if(front_car==1 && var3==1  && speedwantD && speedwantE)   //������ɺ��ǰ����ȥ��ʱ�򽫻���������־���㲢ͬ�����󳵣��ú󳵳���
//          {   //                       ��ֹǰ����ͣ�»�δ�����������������־�ˣ����԰��ٶȱ�Ϊ���������ı�־
//            var3=0;
//            updata_var(VAR3);
//            tongbu[3]=10;
//          }
//          if( chao_one==1 )  chao_one=0;
//        }
//        else
//        {
//          weizhi_turn=0;
//        }

#endif


        /*********************** ������Ϣ ����  ***********************/
        deal_key_event();    //����ͼ����ʾ

        LCD_show();

        measure_distance();

    }
}


