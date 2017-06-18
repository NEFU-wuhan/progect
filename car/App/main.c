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

//===============设置变量===============
//速度
int16 speedwantD_set=100;
int16 speedwantE_set=100;
//起跑线超车控制标志
uint8 origin_chao_cont=  0 ;
//直道超车控制
uint8 wrz_chao_cont=     0 ;
//环道超车控制标志
uint8 huan_chao_cont=    0 ;
//=======================================
uint8 CAR_MODEL;    //1-号车， 2-2号车， 3-3号车
uint8  var1, var2;
uint16 var3, var4;
uint32 var5, var6;

uint8 front_car=0,weizhi_turn=0;
int8 tongbu[10];
int8 diff=0;
int8 cut_1=0,cut_2=0;
int8 chao_huan=0,chao_zhi=0,Ramp_tong=0;  //此标志为了让前车在同一超车地点只触发一次超车

int8 sequence=0;  //车的顺序，为0的时候则前后车标志位与var1相同，1则相反
//解压后的图像，19200=160*120
uint8  img[160*120];                                     //解压后的图像

//-----------开车超声波检测前后车
void detection_front_back();
//-----------发车前检测NRF通讯
void test_communication();
void send_nrf();
void recive_nrf();
void SD_save_deal();

/*!
 *  @brief      main函数
 *  @since      v5.0
 *  @note       山外 无线调试 测试实验
 */
void  main(void)
{
    com_e     com;                               //无线通讯
    nrf_result_e nrf_result;

    init_sys();     //总初始化
//    img_sd_init();

    //------------开车检测前后车
//    detection_front_back();
    //---------------------------
    while(1)
    {
        /************************ 图像采集和显示  ***********************/
        camera_get_img();                           //摄像头获取图像
        //     printf("%d\n",*imgbuff);
        /************************ 图像解压        ***********************/
        img_extract1(img,imgbuff);          //摄像头图像解压

        /************************ 图像处理        ***********************/
        median_line_extract(img);	            //摄像头图像处理

        /*******************      起跑线      ***************************/
//        if(right_lost_count < 10 && left_lost_count < 10 && mid_line <70 && mid_line > 58)
//        start_line(img);

        quan_dir_duty_new();				    //算出每一帧图像的中线权重

//        hswy_sendimg(img);         //红书伟业上位机图像传输


#if nrf_debug
        /********************** 无线发送和接收数据  ***********************/
        do
        {
            nrf_result = nrf_msg_rx(&com, nrf_rx_buff);
        }while(nrf_result != NRF_RESULT_RX_NO);         //接收不到数据 才退出

        //--------发车前检测NRF通讯状况---------------------
        if(Motor_En==0)
        {
          test_communication();
        }
        //--------处理接收数据-----------
        recive_nrf();
        //--------开始发送数据-----------
        send_nrf();

#endif


        /*********************** 按键消息 处理  ***********************/
        deal_key_event();

        LCD_show();

//        SD_save_deal();

//        if(front_car==0 || var4!=0 || stop==1 ) measure_distance();
//        else  s_distance=550;

    }
}

//-----------开车检测前后车
void detection_front_back()
{
  uint16 ce_weizhi=0,ce_i=0;
  for(ce_i=0;ce_i<8;ce_i++)
    ce_weizhi +=(uint16)measure_distance();
  if( (ce_weizhi/8)>500 )   sequence=1;
  else sequence=0;
}

//发车前一直互相发送标志检测通讯状况
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

//通过NRF同步数据
void send_nrf()
{
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
    if (tongbu[3]>0)                                          //双车通信  （为了同步两车环道超车标志，为1 则标志正在进行环道超车）
    {
      var_syn(VAR3);
      tongbu[3]--;
    }
    if (tongbu[4]>0)                                          //双车通信  （为了同步两车直道超车标志，为1 则标志正在进行直道超车）
    {
      var_syn(VAR4);
      tongbu[4]--;
    }
  }
}

//处理NRF接受到的数据
void recive_nrf()
{
  //----------------------接收完数据更换前后车标志位  /////////////////////////////
  if( sequence==var1 && tongbu[1]==0 && tongbu[2]==0 && tongbu[3]==0) front_car=0;     //为了确定前后车，标志位始终与var1同步，不需要var1清0
  else  front_car=1;    //超车的时候两车都为前车     && var3==0

  //----------------------NRF接收方 发停车 的程序----------------------
  nrf_start_stop();

  //----------------------每次超车完之后的清零------------------------
  // chao_one //对应的是环道超车
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

//SD卡保存和处理
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

