/******************************************
*                                         *
*            2017.1.6  ZQ�޸�             *
*                                         *
******************************************/
#include "common.h"
#include "include.h"

long int speedwant=0;

#define CAR_MODEL 1    //1-�ų��� 2-2�ų��� 3-3�ų�
#define nrf_debug 0    //0-�ر����ߴ��䣬 1-�������ߴ���

uint8  var1, var2;
uint16 var3, var4;
uint32 var5, var6;

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

        save_var(VAR1, speedwant);       //�����ű�����ֵ
        updata_var(VAR1);                //���±�ű�����ֵ���޸ı�����ֵ����Ҫ���ô˺��������±�ű�����ֵ��
        var_syn(VAR1);

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
                                     DJ_protect=0;
                                     Motor_En=1;
                                     speedwantB=70;
                                break;

                            case CAR_STOP:                          //ͣ��
                                      speedwant=0;
                                      ftm_pwm_init(FTM0, FTM_CH4,10000, 0);    //������ת
                                      ftm_pwm_init(FTM0, FTM_CH5,10000, 0);    //������ת
                                      ftm_pwm_init(FTM0, FTM_CH6,10000, 0);    //�ҵ����ת
                                      ftm_pwm_init(FTM0, FTM_CH7,10000, 0);    // //�ҵ����ת
                                      speedwantB=0;
                                break;
                             case  CAR_FORWARD:                               //ǰ��
                                     Motor_En=1;
                                     speedwantB=speedwantB+10;
                                     DJ_protect=0;
                                break;
                             case  CAR_BACK:                               //ǰ��
                                     speedwantB=speedwantB-10;
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


        /*********************** ������Ϣ ����  ***********************/
        deal_key_event();    //����ͼ����ʾ

        LCD_show();
  //      measure_distance();
    }
}



