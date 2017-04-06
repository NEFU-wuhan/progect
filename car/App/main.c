/******************************************
*                                         *
*            2017.1.6  ZQ修改             *
*                                         *
******************************************/
#include "common.h"
#include "include.h"

long int speedwant=0;

#define CAR_MODEL 1    //1-号车， 2-2号车， 3-3号车
#define nrf_debug 0    //0-关闭无线传输， 1-开启无线传输

uint8  var1, var2;
uint16 var3, var4;
uint32 var5, var6;

//解压后的图像，19200=160*120
uint8  img[160*120];                                     //解压后的图像
/*!

 *  @brief      main函数
 *  @since      v5.0
 *  @note       山外 无线调试 测试实验
 */
void  main(void)
{
    com_e     com;                                                             //无线通讯
    nrf_result_e nrf_result;

    init_sys();     //总初始化

    while(1)
    {
        /************************ 图像采集和显示  ***********************/
        camera_get_img();                           //摄像头获取图像

        /************************ 图像解压        ***********************/
        img_extract1(img,imgbuff);          //摄像头图像解压

        /************************ 图像处理        ***********************/
        median_line_extract(img);	            //摄像头图像处理

        quan_dir_duty_new();				    //算出每一帧图像的中线权重
//        hswy_sendimg(img);         //红书伟业上位机图像传输

        save_var(VAR1, speedwant);       //保存编号变量的值
        updata_var(VAR1);                //更新编号变量的值（修改变量的值后，需要调用此函数来更新编号变量的值）
        var_syn(VAR1);

        /************************ 无线发送和接收数据  ***********************/
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
                        if(last_tab == CAR_CTRL)                    //如果是 控制 小车 命令
                        {
                            switch(car_ctrl_get())                  //获取小车控制命令，并进行处理
                            {
                            case CAR_START:                         //开车
                                     DJ_protect=0;
                                     Motor_En=1;
                                     speedwantB=70;
                                break;

                            case CAR_STOP:                          //停车
                                      speedwant=0;
                                      ftm_pwm_init(FTM0, FTM_CH4,10000, 0);    //左电机正转
                                      ftm_pwm_init(FTM0, FTM_CH5,10000, 0);    //左电机反转
                                      ftm_pwm_init(FTM0, FTM_CH6,10000, 0);    //右电机反转
                                      ftm_pwm_init(FTM0, FTM_CH7,10000, 0);    // //右电机正转
                                      speedwantB=0;
                                break;
                             case  CAR_FORWARD:                               //前进
                                     Motor_En=1;
                                     speedwantB=speedwantB+10;
                                     DJ_protect=0;
                                break;
                             case  CAR_BACK:                               //前进
                                     speedwantB=speedwantB-10;
                                break;

                            default:
                                //无效，不进行任何处理
                                break;
                            }
                        }

                        break;
                    default:
                        break;
                }
            }
        }while(nrf_result != NRF_RESULT_RX_NO);         //接收不到数据 才退出


        /*********************** 按键消息 处理  ***********************/
        deal_key_event();    //包括图像显示

        LCD_show();
  //      measure_distance();
    }
}



