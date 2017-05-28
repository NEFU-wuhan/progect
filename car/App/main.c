/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       main.c
 * @brief      山外K60 平台主程序
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-28
 */
#include "common.h"
#include "include.h"

long int speedwant=0;
int16 speedwantD_set=80;
int16 speedwantE_set=80;
uint8 CAR_MODEL;    //1-号车， 2-2号车， 3-3号车
uint8  var1, var2;
uint16 var3, var4;
uint32 var5, var6;

uint8 front_car=0,weizhi_turn=0;
int8 tongbu[10];
int8 diff=0;
int8 input=0,cut_1=0,cut_2=0;
int8 chao_one=0,chao_one2=0;  //此标志为了让前车在同一超车地点只触发一次超车

int8 sequence=0;  //车的顺序，为0的时候则前后车标志位与var1相同，1则相反
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
    //-----------开车检测前后车
    uint16 ce_weizhi=0,ce_i=0;
    for(ce_i=0;ce_i<8;ce_i++)
        ce_weizhi +=(uint16)measure_distance();
    if( (ce_weizhi/8)>500 )   sequence=1;
    else sequence=0;
    //---------------------------
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


#if nrf_debug
        input=var2;
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
//                                     DJ_protect=0;
//                                     Motor_En=1;
//                                     speedwantB=70;
                                break;

                            case CAR_STOP:                          //停车
//                                      speedwant=0;
//                                      ftm_pwm_init(FTM0, FTM_CH4,10000, 0);    //左电机正转
//                                      ftm_pwm_init(FTM0, FTM_CH5,10000, 0);    //左电机反转
//                                      ftm_pwm_init(FTM0, FTM_CH6,10000, 0);    //右电机反转
//                                      ftm_pwm_init(FTM0, FTM_CH7,10000, 0);    // //右电机正转
//                                      speedwantB=0;
                                break;
                             case  CAR_FORWARD:                               //前进
//                                     Motor_En=1;
//                                     speedwantB=speedwantB+10;
//                                     DJ_protect=0;
                                break;
                             case  CAR_BACK:                               //前进
//                                     speedwantB=speedwantB-10;
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

        //////////////////     接收完更换标志位  /////////////////////////////
        if( sequence==var1 && var3==0 && tongbu[1]==0 && tongbu[2]==0 && tongbu[3]==0) front_car=0;     //为了确定前后车，标志位始终与var1同步，不需要var1清0
        else  front_car=1;    //超车的时候两车都为前车

        if(input!=var2)
        {
            key_flag_clear=1;
            if(var2==1)     //表示发车
            {
              DJ_protect=0;
              flag_key_select=5;
              flag_key_l_u_0=0;
              Motor_En=1;
              speedwantD=speedwantD_set;
              speedwantE=speedwantE_set;
            }
            else          //表示停车
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

        ///////////////////   开始发送数据   /////////////////////////////////
        uint8 z=0;


        if( front_car==1 )
        {
          if (tongbu[1]>0)                                          //双车通信  (为了切换双车前后)
          {
            var_syn(VAR1);
            tongbu[1]--;
          }

          if (tongbu[2]>0)                                          //双车通信  （为了同时发车停车）
          {
            var_syn(VAR2);
            tongbu[2]--;
          }
          if (tongbu[3]>0)                                          //双车通信  （为了同步两车环道标志，为1 则标志正在进行环道超车）
          {
            var_syn(VAR3);
            tongbu[3]--;
          }
          if (tongbu[4]>0)                                          //双车通信  （为了同步两车环道标志，为1 则标志正在进行环道超车）
          {
            var_syn(VAR4);
            tongbu[4]--;
          }
        }

//        if(weizhi_turn==1)
//        {
//          if(var3==0  //加这个条件是为了让前车识别超车标志，开始超车（且不让后车再进入）
//             && chao_one==0 )  //为了前车只发送一次切换标志，防止停车的时候一直符合条件一直切换，超车完成之后清除标志
//          {
//            chao_one=1;
//            speedwantD=0;
//            speedwantE=0;
//
//            var3=1;         //并且环道切换标志同步（表示正在切换，切换完成后清零）
//            updata_var(VAR3);
//            tongbu[3]=10;
//
//            var1=1;         //在这切换前后车
//            updata_var(VAR1);
//            tongbu[1]=10;
//          }
//        }
//        else if(weizhi_turn==2)
//        {
//          if(front_car==1 && var3==1  && speedwantD && speedwantE)   //超车完成后的前车出去的时候将环道超车标志清零并同步给后车，让后车出来
//          {   //                       防止前车刚停下还未被超车就清除超车标志了，可以把速度变为其他单独的标志
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


        /*********************** 按键消息 处理  ***********************/
        deal_key_event();    //包括图像显示

        LCD_show();

        measure_distance();

    }
}


