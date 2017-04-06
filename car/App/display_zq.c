#include "common.h"
#include "include.h"


char color[10]={BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK};

int8 key_flag_clear=1;

const uint16    *bmp[3];
extern uint8 hehe;
/*******************************************************************************
 * Function Name: bmp_init();
 * Description  : bmp图片初始化
 * Author Data  : 2015/3/12 by Li
 ******************************************************************************/
void bmp_init()
{
    bmp[0]=nBitmapDot;    //nBitmapDot       DuoLaAMeng
}
/*******************************************************************************
 * Function Name: bmp_display();
 * Input Param  : 无
 * Output Param : 无
 * Description  : bpm图片显示，根据str_new_tab进行切换
 * Author Data  : 2014/3/22 星期四, by Li
 ******************************************************************************/
void bmp_display()
{
  Site_t bmp_site     = {0, 0};                     //显示图像左上角位置
  Size_t bmp_imgsize  = {128, 128};                 //图像大小
  Size_t bmp_size     = {128, 128};                   //显示区域图像大小

  LCD_Img_RGB565_Z(bmp_site,bmp_size,bmp[0],bmp_imgsize);
}

/*******************************************************************************
* Function Name:  void key_lcd_clear()
* Input Param  : 无
* Output Param : 无
* Description  : 按键清屏
* Author Data  : 2015/03/02 星期一, by momo
******************************************************************************/

void key_lcd_clear()
{
  if(key_flag_clear==1) //如果清屏标志位=1，清
  {
    key_flag_clear=0;
    LCD_clear(0X0000);
  }
}
/*******************************************************************************
 * Function Name: void lcd_main()
 * Input Param  : 无
 * Output Param : 无
 * Description  :
 * Author Data  : 2015/5/27  by momo
 ******************************************************************************/
void LCD_show()
{
        static uint8 fache=0;
        color_change();
        switch(flag_key_select)            //flag_key_select
        {
            case 0:
              {
                key_lcd_clear();
                lcd_show_0();    //显示菜单
                fache=0;
              }
              break;
            case 1:
              {
                key_lcd_clear();
                lcd_show_1();     //显示赛道信息
              }
              break;
            case 2:
              {
                key_lcd_clear();
                lcd_show_2();     //显示舵机参数
              }
              break;
            case 3:
              {
                key_lcd_clear();
                lcd_show_3();     //显示电机参数
              }
              break;
            case 4:
              {
                key_lcd_clear();
                lcd_show_4();     //显示其他
              }
              break;
            case 5:
              {

                if(fache<1)
                {
                    key_lcd_clear();
                    lcd_show_5();     //发车（停止刷屏）
                    fache++;
                }
              }
              break;
        }
}
/*******************************************************************************
 * Function Name: void lcd_show_1()
 * Input Param  : 无
 * Output Param : 无
 * Description  :
 * Author Data  : 2015/5/27  by momo
 ******************************************************************************/

void lcd_show_0()      //显示菜单
{
    Site_t site;
    site.x = 13;
    site.y = 3;
    LCD_FSTR_CH(site,menu ,WHITE, BLACK) ;    //菜单           //vcan_str       menu

    Site_t site_m1 = {5,23};
    LCD_str(site_m1,"1 ", WHITE,color[0]);           //
    site.x = 13;
    site.y = 23;
    LCD_FSTR_CH(site,side ,WHITE, color[0]) ;    //赛道信息           //datuxiang       menu

    Site_t site_m2 = {5,43};
    LCD_str(site_m2,"2 ", WHITE,color[1]);           //
    site.x = 13;
    site.y = 43;
    LCD_FSTR_CH(site,sever ,WHITE, color[1]) ;       //舵机参数

    Site_t site_m3 = {5,63};
    LCD_str(site_m3,"3 ", WHITE,color[2]);           //
    site.x = 13;
    site.y = 63;
    LCD_FSTR_CH(site,motor_display ,WHITE, color[2]) ;      //电机参数

    Site_t site_m4= {5,83};
    LCD_str(site_m4,"4 ", WHITE,color[3]);           //
    site.x = 13;
    site.y = 83;
    LCD_FSTR_CH(site,other_canshu ,WHITE, color[3]) ;      //其他参数

    Site_t site_m5 = {5,103};
    LCD_str(site_m5,"5 ", WHITE,color[4]);           //
    site.x = 13;
    site.y = 103;
    LCD_FSTR_CH(site,start_go,WHITE, color[4]) ;      //发车   finish_line

 //   if(flag_didianya)
//    {
//      site.x = 73;
//      site.y = 3;
//      LCD_FSTR_CH(site,didianya ,BLACK, YELLOW) ;      //显示低电压提示
//    }

//    if(Final_line_flag)
//    {
//      site.x = 78;
//      site.y = 103;
//      LCD_FSTR_CH(site,finish_line ,BLACK, YELLOW) ;      //显示终点提示
//    }

}

/*******************************************************************************
 * Function Name: void lcd_show_1()
 * Input Param  : 无
 * Output Param : 无
 * Description  :
 * Author Data  : 2015/5/27  by momo
 ******************************************************************************/

void lcd_show_1()      //主要用于采集图像显示赛道信息
{
         Site_t   camera_site={0,0};                           //显示图像左上角位置
         Size_t   camera_imgsize={128,80};
         LCD_Img_Binary_My(camera_site,camera_imgsize,img);  //显示图像
         LCD_line(line,RED,GREEN);                           //画出边线和中线

         Site_t site = {0,73};   //x = 10 ,y = 20
         Size_t size = {128,4};  // W = 50 ,H = 60
         LCD_rectangle(site, size, RED);

         Site_t site_m0 = {0,80};
         LCD_FSTR_CH(site_m0,page_up ,WHITE, color[0]) ;     //上一页

         Site_t site_m1 = {0,80+16};
         LCD_str(site_m1,"mid", WHITE,color[3]);
         Site_t site_m2 = {0,80+16*2};
//       LCD_float(site_m2,mid_line,WHITE,color[1]);                //显示中线值
         LCD_num_BC(site_m2,mid_line,3,WHITE, color[3]);          //显示有用行

         Site_t site_m3 = {32,80+16};
         LCD_str(site_m3,"use", WHITE,color[3]);
         Site_t site_m4 = {32,80+16*2};
         LCD_num_BC(site_m4,used_length,3,WHITE, color[3]);          //显示有用行

         Site_t site_m5 = {32*2,80+16};
         LCD_str(site_m5,"duoj", WHITE,color[3]);
         Site_t site_m6 = {32*2,80+16*2};
         LCD_num(site_m6,SE_duty,WHITE, color[3]);                   //显示舵机打角

         Site_t site_m7 = {32*3 +8 ,80+16};
         LCD_str(site_m7,"zs", WHITE,color[4]);
         Site_t site_m8 = {32*3 +8 ,80+16*2};
         LCD_num(site_m8,hehe,WHITE,color[4]);                   //显示帧数   fiag_huan
      //   LCD_num_BC(site_m8,frame,3,WHITE, color[4]);          //显示有用行

         Site_t site_m18 = {64,80};
         LCD_FSTR_CH(site_m18,page_down ,WHITE, color[1]) ;     //下一页



}

/*******************************************************************************
 * Function Name: void lcd_show_1()
 * Input Param  : 无
 * Output Param : 无
 * Description  :
 * Author Data  : 2015/5/27  by momo
 ******************************************************************************/
void lcd_show_2()           //主要用于查看舵机及其相关参数
{
        //下面显示不可调整的参数

         Site_t site_m1 = {64,0};
         LCD_str(site_m1,"1_", BLUE,BLACK);

         Site_t site_m2 = {96,0};   //x = 10 ,y = 20
         LCD_num_BC(site_m2,1,3, BLUE,BLACK);

         Site_t site_m3 = {64,16};
         LCD_str(site_m3,"2_", BLUE,BLACK);

         Site_t site_m4 = {96,16};   //x = 10 ,y = 20
         LCD_num_BC(site_m4,2,4, BLUE,BLACK);

         Site_t site_m5 = {64,32};
         LCD_str(site_m5,"3_", BLUE,BLACK);

         Site_t site_m6 = {93,32};   //x = 10 ,y = 20
         LCD_num_BC(site_m6,3,3, BLUE,BLACK);

         Site_t site_m7 = {64,48};
         LCD_str(site_m7,"4_", BLUE,BLACK);

         Site_t site_m8 = {93,48};   //x = 10 ,y = 20
         LCD_num_BC(site_m8,4,3, BLUE,BLACK);

         Site_t site_m9 = {64,64};
         LCD_str(site_m9,"5_", BLUE,BLACK);

         Site_t site_m10 = {96,64};   //x = 10 ,y = 20
         LCD_num_BC(site_m10,5,3, BLUE,BLACK);

         Site_t site_m11 = {64,80};
         LCD_str(site_m11,"6_", BLUE,BLACK);

         Site_t site_m12 = {96,80};   //x = 10 ,y = 20
         LCD_num_BC(site_m12,6,3, BLUE,BLACK);

         Site_t site_m13 = {64,96};
         LCD_str(site_m13,"7_", BLUE,BLACK);

         Site_t site_m14 = {96,96};   //x = 10 ,y = 20
         LCD_num_BC(site_m14,7,3, BLUE,BLACK);
      //下面显示可以调整的参数

         Site_t site_m21 = {0,0};
         LCD_FSTR_CH(site_m21,page_up ,WHITE, color[0]) ;     //上一页

         Site_t site_m22 = {0,16};
         LCD_str(site_m22,"P", WHITE,color[1]);
         Site_t site_m23 = {32,16};   //x = 10 ,y = 20
         LCD_num_BC(site_m23,1,3, WHITE,color[1]);

         Site_t site_m24 = {0,32};
         LCD_str(site_m24,"D", WHITE,color[2]);
         Site_t site_m25 = {32,32};   //x = 10 ,y = 20
         LCD_num_BC(site_m25,2,3, WHITE,color[2]);

         Site_t site_m28 = {0,48};
         LCD_str(site_m28,"zz", WHITE,color[3]);
         Site_t site_m29 = {32,48};   //x = 10 ,y = 20
         LCD_num_BC(site_m29,3,3, WHITE,color[3]);

         Site_t site_m30 = {0,64};
         LCD_str(site_m30,"4_", WHITE,color[4]);
         Site_t site_m31 = {32,64};   //x = 10 ,y = 20
         LCD_num_BC(site_m31,4,3, WHITE,color[4]);

         Site_t site_m32 = {0,80};
         LCD_str(site_m32,"5_", WHITE,color[5]);
         Site_t site_m33 = {32,80};   //x = 10 ,y = 20
         LCD_num_BC(site_m33,5,3, WHITE,color[5]);

         Site_t site_m34 = {0,96};
         LCD_str(site_m34,"6_", WHITE,color[6]);
         Site_t site_m35 = {32,96};   //x = 10 ,y = 20
         LCD_num_BC(site_m35,6,3, WHITE,color[6]);

         Site_t site_m26 = {0,112};
         LCD_FSTR_CH(site_m26,page_down ,WHITE, color[7]) ;     //下一页
         Site_t site_m27 = {64,112};
         LCD_FSTR_CH(site_m27,break_menu ,WHITE, color[8]) ;     //返回主菜单

}

/*******************************************************************************
 * Function Name: void lcd_show_1()
 * Input Param  : 无
 * Output Param : 无
 * Description  :
 * Author Data  : 2015/5/27  by momo
 ******************************************************************************/

void lcd_show_3()         //主要用于查看电机及其相关参数
{
        //下面显示不可调整的参数

         Site_t site_m1 = {64,0};
         LCD_str(site_m1,"1_", BLUE,BLACK);
         Site_t site_m2 = {96,0};   //x = 10 ,y = 20
         LCD_num_BC(site_m2,1,3, BLUE,BLACK);

         Site_t site_m3 = {64,16};
         LCD_str(site_m3,"2_", BLUE,BLACK);
         Site_t site_m4 = {96,16};   //x = 10 ,y = 20
         LCD_num_BC(site_m4,2,3, BLUE,BLACK);

         Site_t site_m5 = {64,16*2};
         LCD_str(site_m5,"3_", BLUE,BLACK);
         Site_t site_m6 = {96,16*2};   //x = 10 ,y = 20
         LCD_num_BC(site_m6,3,3, BLUE,BLACK);

         Site_t site_m7 = {64,16*3};
         LCD_str(site_m7,"lj", BLUE,BLACK);
         Site_t site_m8 = {96,16*3};   //x = 10 ,y = 20
         LCD_num_BC(site_m8,4,5, BLUE,BLACK);

         Site_t site_m9 = {64,16*4};
         LCD_str(site_m9,"speed", BLUE,BLACK);
         Site_t site_m10 = {96,16*4};   //x = 10 ,y = 20
         LCD_num_BC(site_m10,5,3, BLUE,BLACK);

         Site_t site_m11 = {64,16*5};
         LCD_str(site_m11,"6_", BLUE,BLACK);
         Site_t site_m12 = {96,16*5};   //x = 10 ,y = 20
         LCD_num_BC(site_m12,6,3, BLUE,BLACK);

         Site_t site_m13 = {64,16*6};
         LCD_str(site_m13,"7_", BLUE,BLACK);
         Site_t site_m14 = {96,16*6};   //x = 10 ,y = 20
         LCD_num_BC(site_m14,7,3, BLUE,BLACK);

      //下面显示可以调整的参数
         Site_t site_m21 = {0,0};
         LCD_FSTR_CH(site_m21,page_up ,WHITE, color[0]) ;     //上一页

         Site_t site_m22 = {0,16};                       // 差速的斜率 p1
         LCD_str(site_m22,"p1", WHITE,color[1]);
         Site_t site_m23 = {28,16};   //x = 10 ,y = 20
//         LCD_num_BC(site_m23,p1*10000,5, WHITE,color[1]);

         Site_t site_m24 = {0,16*2};
         LCD_str(site_m24,"sudu", WHITE,color[2]);
         Site_t site_m25 = {32,16*2};   //x = 10 ,y = 20
//         LCD_num_BC(site_m25,Menu_data_num[0],3, WHITE,color[2]);

         Site_t site_m28 = {0,16*3};
         LCD_str(site_m28,"Kp", WHITE,color[3]);
         Site_t site_m29 = {28,16*3};   //x = 10 ,y = 20
//         LCD_num_BC(site_m29,Kp2,4, WHITE,color[3]);

         Site_t site_m30 = {0,16*4};
         LCD_str(site_m30,"Ki", WHITE,color[4]);
         Site_t site_m31 = {28,16*4};   //x = 10 ,y = 20
//         LCD_num_BC(site_m31,Ki2,4, WHITE,color[4]);

         Site_t site_m32 = {0,16*5};
         LCD_str(site_m32,"Kd", WHITE,color[5]);
         Site_t site_m33 = {28,16*5};   //x = 10 ,y = 20
//         LCD_num_BC(site_m33,Kd2,4, WHITE,color[5]);

         Site_t site_m34 = {0,16*6};                          //差速的截距p2
         LCD_str(site_m34,"posu", WHITE,color[6]);
         Site_t site_m35 = {32,16*6};   //x = 10 ,y = 20
//         LCD_num_BC(site_m35,Menu_data_num[1],3, WHITE,color[6]);

         Site_t site_m26 = {0,16*7};
         LCD_FSTR_CH(site_m26,page_down ,WHITE, color[7]) ;     //下一页

         Site_t site_m27 = {64,16*7};
         LCD_FSTR_CH(site_m27,break_menu ,WHITE, color[8]) ;     //返回主菜单

}



/*******************************************************************************
 * Function Name: void lcd_show_6()
 * Input Param  : 无
 * Output Param : 无
 * Description  :
 * Author Data  : 2015/5/27  by momo
 ******************************************************************************/

void lcd_show_4()           //主要用于查看其他信息（待完善）
{
        //下面显示不可调整的参数

         Site_t site_m1 = {64,0};
         LCD_str(site_m1,"1_", BLUE,BLACK);

         Site_t site_m2 = {96,0};   //x = 10 ,y = 20
         LCD_num_BC(site_m2,1,3, BLUE,BLACK);

         Site_t site_m3 = {64,16};
         LCD_str(site_m3,"2_", BLUE,BLACK);

         Site_t site_m4 = {96,16};   //x = 10 ,y = 20
         LCD_num_BC(site_m4,2,4, BLUE,BLACK);

         Site_t site_m5 = {64,32};
         LCD_str(site_m5,"3_", BLUE,BLACK);

         Site_t site_m6 = {93,32};   //x = 10 ,y = 20
         LCD_num_BC(site_m6,3,3, BLUE,BLACK);

         Site_t site_m7 = {64,48};
         LCD_str(site_m7,"4_", BLUE,BLACK);

         Site_t site_m8 = {93,48};   //x = 10 ,y = 20
         LCD_num_BC(site_m8,4,3, BLUE,BLACK);

         Site_t site_m9 = {64,64};
         LCD_str(site_m9,"5_", BLUE,BLACK);

         Site_t site_m10 = {96,64};   //x = 10 ,y = 20
         LCD_num_BC(site_m10,5,3, BLUE,BLACK);

         Site_t site_m11 = {64,80};
         LCD_str(site_m11,"6_", BLUE,BLACK);

         Site_t site_m12 = {96,80};   //x = 10 ,y = 20
         LCD_num_BC(site_m12,6,3, BLUE,BLACK);

         Site_t site_m13 = {64,96};
         LCD_str(site_m13,"7_", BLUE,BLACK);

         Site_t site_m14 = {96,96};   //x = 10 ,y = 20
         LCD_num_BC(site_m14,7,3, BLUE,BLACK);
      //下面显示可以调整的参数

         Site_t site_m21 = {0,0};
         LCD_FSTR_CH(site_m21,page_up ,WHITE, color[0]) ;     //上一页

         Site_t site_m22 = {0,16};
         LCD_str(site_m22,"P", WHITE,color[1]);
         Site_t site_m23 = {32,16};   //x = 10 ,y = 20
         LCD_num_BC(site_m23,1,3, WHITE,color[1]);

         Site_t site_m24 = {0,32};
         LCD_str(site_m24,"D", WHITE,color[2]);
         Site_t site_m25 = {32,32};   //x = 10 ,y = 20
         LCD_num_BC(site_m25,2,3, WHITE,color[2]);

         Site_t site_m28 = {0,48};
         LCD_str(site_m28,"zz", WHITE,color[3]);
         Site_t site_m29 = {32,48};   //x = 10 ,y = 20
         LCD_num_BC(site_m29,3,3, WHITE,color[3]);

         Site_t site_m30 = {0,64};
         LCD_str(site_m30,"4_", WHITE,color[4]);
         Site_t site_m31 = {32,64};   //x = 10 ,y = 20
         LCD_num_BC(site_m31,4,3, WHITE,color[4]);

         Site_t site_m32 = {0,80};
         LCD_str(site_m32,"5_", WHITE,color[5]);
         Site_t site_m33 = {32,80};   //x = 10 ,y = 20
         LCD_num_BC(site_m33,5,3, WHITE,color[5]);

         Site_t site_m34 = {0,96};
         LCD_str(site_m34,"6_", WHITE,color[6]);
         Site_t site_m35 = {32,96};   //x = 10 ,y = 20
         LCD_num_BC(site_m35,6,3, WHITE,color[6]);

         Site_t site_m26 = {0,112};
         LCD_FSTR_CH(site_m26,page_down ,WHITE, color[7]) ;     //下一页
         Site_t site_m27 = {64,112};
         LCD_FSTR_CH(site_m27,break_menu ,WHITE, color[8]) ;     //返回主菜单

}
/*******************************************************************************
 * Function Name: void lcd_show_6()
 * Input Param  : 无
 * Output Param : 无
 * Description  :
 * Author Data  : 2015/5/27  by momo
 ******************************************************************************/

void lcd_show_5()           //主要用于显示发车图像
{
         Site_t site_m27 = {64,112};
         LCD_FSTR_CH(site_m27,break_menu ,WHITE, BLUE) ;     //返回主菜单
}

 /*******************************************************************************
  * Function Name: void deal_key_down_1()
  * Input Param  : 无
  * Output Param : 无
  * Description  : 颜色改变的程序
  * Author Data  : 2015/5/27  by momo
  ******************************************************************************/

void color_change()
{
    int8 i;
     i=flag_key_l_u_0;

   switch (flag_key_select)
    {
    case 0:
      {
        i=flag_key_l_u_0;
        color[i]=BLUE;
      }
      break;
    case 1:
      {
        i=flag_key_l_u_1;
        color[i]=BLUE;
      }
      break;
    case 2:
      {
        i=flag_key_l_u_2;
        color[i]=BLUE;
      }
      break;
    case 3:
      {
        i=flag_key_l_u_3;
        color[i]=BLUE;
      }
      break;
    case 4:
      {
        i=flag_key_l_u_4;
        color[i]=BLUE;
      }
      break;
    }

}

/*******************************************************************************
  * Function Name: void deal_key_down_1()
  * Input Param  : 无
  * Output Param : 无
  * Description  : 颜色数组重新初始化的程序
  * Author Data  : 2015/5/27  by momo
  ******************************************************************************/

void color_init()
{
     color[0]=BLACK;
     color[1]=BLACK;
     color[2]=BLACK;
     color[3]=BLACK;
     color[4]=BLACK;
     color[5]=BLACK;
     color[6]=BLACK;
     color[7]=BLACK;
     color[8]=BLACK;
     color[9]=BLACK;
}

