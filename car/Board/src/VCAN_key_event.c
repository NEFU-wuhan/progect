/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_key_event.h
 * @brief      KEY �¼�����ʵ��
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2014-01-04
 */

#include "VCAN_key_event.h"
#include "VCAN_UI_VAR.h"
#include "O_display.h"
#include "O_menu_control.h"
#include "main.h"
#include "O_speed.h"

//#include "NRF24L0.h"
//#include "NRF24L0_MSG.h"

int8 flag_key_select=0;
int8 flag_key_l_u_0=0;
int8 flag_key_l_u_1=0;
int8 flag_key_l_u_2=0;
int8 flag_key_l_u_3=0;
int8 flag_key_l_u_4=0;


//-------------------------------------------------
//                 �������
//����
//��� void
//���� �������ļ�Ȩ�����ƶ�����
//����
//����
//--------------------------------------------------
void key_event_init()
{

    key_init(KEY_MAX);  //��ʼ�� ȫ�� ����
//    var_init();   //��ʼ����ʱ����
#if nrf_debug
    //ͬ��ȫ�����ݲ���ʾ
    var_syn(VAR_MAX);       //ͬ��ȫ�� ,������ͬ������ʾȫ������Ϊ�п���ͬ��ʧ�ܡ�
    var_display(VAR_MAX);   //��ʾȫ��
#endif

}


/*******************************************************************************
* Function Name: void deal_key_event()
* Input Param  : ��
* Output Param : ��
* Description  : ��������
* Author Data  : 2015/5/27  by momo
******************************************************************************/
void deal_key_event()
{
    KEY_MSG_t keymsg;

    while(get_key_msg(& keymsg))     //��ð����ͽ��д���
    {
        color_init();
        shua_one=0;
        if(keymsg.status == KEY_DOWN)
        {
            switch(keymsg.key)
            {
            case KEY_U:     //��
              //  printf("��");
                deal_key_up();
                break;

            case KEY_D:      //��
              //  printf("��");
                deal_key_down();
                break;

            case KEY_L:      //��
                key_flag_clear=1;
              //  printf("��");
                deal_key_left();
                break;

            case KEY_R:      //��
              //  printf("��");
                key_flag_clear=1;
                deal_key_right();
                break;

            case KEY_B:      //ѡ��
                {
                  key_flag_clear=1;
                  deal_key_select();
               //   printf("��");

                }
                break;

            case KEY_A:      //��������
              {
               //   printf("��������");
                  switch(flag_key_select)
                  {
                    case 0:
                      {

                      }
                      break;

                    case 1:
                      {

                      }
                      break;

                  case 2:
                      {
                      }
                      break;

                  case 3:
                      {
                      }
                      break;

                 }

              }
                break;

            case KEY_START:
              //  save_var(CAR_CTRL,CAR_START);
              //  var_syn(CAR_CTRL);
              //  var_display(CAR_CTRL);
                break;
            case KEY_STOP:
               // save_var(CAR_CTRL,CAR_STOP);
               // var_syn(CAR_CTRL);
               // var_display(CAR_CTRL);
                break;

            default:
                break;
            }
        }
        else if(keymsg.status == KEY_HOLD)
        {
            switch(keymsg.key)
            {
            case KEY_U:                //�����ϰ���
                {
                   //deal_key_up_1();
                 //  printf("�����°���");
                }
                break;

            case KEY_D:                //�����°���
                {
                   //deal_key_down_1();
                 //  printf("�����ϰ���");
                }
                break;

            case KEY_L:
              //  var_select(VAR_PREV_HOLD);
                break;

            case KEY_R:
              //  var_select(VAR_NEXT_HOLD);
                break;

            case KEY_B:                //���� OK �� ͬ��ȫ�����ݲ���ʾ
              //  var_syn(VAR_MAX);       //ͬ��ȫ�� ,������ͬ������ʾȫ������Ϊ�п���ͬ��ʧ�ܡ�
              //  var_display(VAR_MAX);   //��ʾȫ��
                break;

            case KEY_START:
              //  save_var(CAR_CTRL,CAR_START);
              //  while(var_syn(CAR_CTRL)==0);
                var_display(CAR_CTRL);
                break;

            case KEY_STOP:
              //  save_var(CAR_CTRL,CAR_STOP);
              //  while(var_syn(CAR_CTRL)==0);
              //  var_display(CAR_CTRL);

            default:            //����Ҫ���� cancel
                break;
            }
        }
        else
        {
            //KEY_UP ,�����д���
        }
    }
}

 /*******************************************************************************
* Function Name: void deal_key_select()
* Input Param  : ��
* Output Param : ��
* Description  : �м䰴������
* Author Data  : 2015/5/27  by momo
******************************************************************************/

void deal_key_select()
{
 switch(flag_key_select)
  {
   case 0:       //��ʾ����ͣ���ڲ˵�ҳ��
    {
         switch(flag_key_l_u_0)
            {
             case 0:       //��ʾ�˵�ҳ��
              {
                   flag_key_select=1;
                   flag_key_l_u_0=0;
              }
              break;

            case 1:        //��ʾͼ��ҳ��
              {
                   flag_key_select=2;
                   flag_key_l_u_0=0;
              }
              break;

            case 2:        //��ʾ�������ҳ��
              {
                   flag_key_select=3;
                   flag_key_l_u_0=0;
              }
              break;

            case 3:        //��ʾ�������ҳ��
              {
                   flag_key_select=4;
                   flag_key_l_u_0=0;
              }
              break;
            case 4:       //����
              {
                DELAY_MS(500);
                DJ_protect=0;
                Motor_En=1;
                speedwantD=100;
                speedwantE=110;
                Start_timing=System_time;    //������ʱ����Ҫ��¼ϵͳʱ��������ʱ����
                flag_key_select=5;
                flag_key_l_u_0=0;
                var2=1;
                updata_var(VAR2);                //���±�ű�����ֵ���޸ı�����ֵ����Ҫ���ô˺��������±�ű�����ֵ��
                tongbu[2]=10;

              }
              break;
            }

    }
    break;
  case 1:        //��ʾ����ͣ����������ʾҳ��
    {
       switch(flag_key_l_u_1)
        {
         case 0:       //��ʾ����ͣ����������ʾҳ�ġ���һҳ��
          {
               flag_key_select=0;
               flag_key_l_u_1=0;
          }
          break;
         case 1:       //��ʾ����ͣ����������ʾҳ�ġ���һҳ��
          {
               flag_key_select=2;
               flag_key_l_u_1=0;
          }
          break;
        }
    }
    break;
  case 2:        //��ʾ����ͣ���ڶ����Ϣ��ʾҳ��
    {
         switch(flag_key_l_u_2)
          {
           case 0:       //��ʾ����ͣ���ڶ����Ϣ��ʾҳ�ġ���һҳ��
            {
                 flag_key_select=1;
                 flag_key_l_u_2=0;
            }
            break;
           case 7:       //��ʾ����ͣ���ڶ����Ϣ��ʾҳ�ġ���һҳ��
            {
                 flag_key_select=3;
                 flag_key_l_u_2=0;
            }
            break;
           case 8:       //��ʾ����ͣ���ڶ����Ϣ��ʾҳ�ġ����ز˵���
            {
                 flag_key_select=0;
                 flag_key_l_u_2=0;
            }
            break;
          }
    }
    break;
  case 3:        //��ʾ����ͣ���ڵ����Ϣ��ʾҳ��
    {
         switch(flag_key_l_u_3)
          {
           case 0:       //��ʾ����ͣ���ڵ����Ϣ��ʾҳ�ġ���һҳ��
            {
                 flag_key_select=2;
                 flag_key_l_u_3=0;
            }
            break;
           case 7:       //��ʾ����ͣ���ڵ����Ϣ��ʾҳ�ġ���һҳ��
            {
                 flag_key_select=4;
                 flag_key_l_u_3=0;
            }
            break;
           case 8:       //��ʾ����ͣ���ڵ����Ϣ��ʾҳ�ġ����ز˵���
            {
                 flag_key_select=0;
                 flag_key_l_u_3=0;
            }
            break;
          }
    }
    break;
  case 4:        //��ʾ����ͣ����������Ϣ��ʾҳ��
    {
         switch(flag_key_l_u_4)
          {
           case 0:       //��ʾ����ͣ���ڵ����Ϣ��ʾҳ�ġ���һҳ��
            {
                 flag_key_select=3;
                 flag_key_l_u_4=0;
            }
            break;
           case 7:       //��ʾ����ͣ���ڵ����Ϣ��ʾҳ�ġ���һҳ��
            {
                 flag_key_select=0;
                 flag_key_l_u_4=0;
            }
            break;
           case 8:       //��ʾ����ͣ���ڵ����Ϣ��ʾҳ�ġ����ز˵���
            {
                 flag_key_select=0;
                 flag_key_l_u_4=0;
            }
            break;
          }
    }
    break;
  case 5:        //��ʾ����ͣ���ڿ�����ʾҳ��
    {
      flag_key_select=0;
      fache=0;
      speedwantD=0;
      speedwantE=0;
      var2=0;
      updata_var(VAR2);                //���±�ű�����ֵ���޸ı�����ֵ����Ҫ���ô˺��������±�ű�����ֵ��
      tongbu[2]=10;
    }
    break;
  default:
    {
        flag_key_select=0;
        flag_key_l_u_0=0;
        flag_key_l_u_1=0;
        flag_key_l_u_2=0;
        flag_key_l_u_3=0;
        flag_key_l_u_4=0;
    }
    break;
  }

}


 /*******************************************************************************
* Function Name: void deal_key_left()
* Input Param  : ��
* Output Param : ��
* Description  : �󰴼�����
* Author Data  : 2015/5/27  by momo
******************************************************************************/

void deal_key_left()
{
    switch (flag_key_select)
    {
    case 0:
      {

      }
      break;
    case 1:
      {
          switch (flag_key_l_u_1)      //����������Ϣ�Ĳ���
            {
            case 0:    //��һҳ
              {

              }
              break;
            case 1:    //��һҳ
              {

              }
              break;
            }
      }
    case 2:
      {
          switch (flag_key_l_u_2)      //��������Ĳ���
            {
            case 0:     //��һҳ
              {

              }
              break;
            case 1:
              {
                //   DJ_Kp_d-=5;
              }
              break;
            case 2:
              {
                 //  DJ_Kd--;
              }
              break;
            case 3:
              {
                 //  angle_center--;
              }
              break;
            case 4:
              {
                //  F_p-=5;
              }
              break;
            case 5:
              {
               //   Servos_output-=10;
              }
              break;
            case 6:
              {
                 //  DJ_Kd--;
              }
              break;
            case 7:      //��һҳ
              {

              }
              break;
            case 8:      //���ڷ��ز˵�
              {

              }
              break;

            }
      }
    case 3:
      {
          switch (flag_key_l_u_3)      //��������Ĳ���
            {
            case 0:       //��һҳ
              {

              }
              break;
            case 1:
              {
                 //  Speed_set_tem-=5;
            //   p1-=0.0001;
              }
              break;
            case 2:

              {
              //  Menu_data_num[0]-=5;  //  flag_chasu--;
              }
              break;
            case 3:
              {
              //  Kp2-=10;
              //  Kp22-=10;
              }
              break;
            case 4:
              {
             //   Ki2-=10;
             //   Ki22-=10;
              }
              break;
            case 5:
              {
             //   Kd2-=1;
             //   Kd22-=1;
              }
              break;
            case 6:
              {
             //  Menu_data_num[1]-=5;
              }
              break;
            case 7:     //��һҳ
              {

              }
              break;
            case 8:    //���ز˵�
              {

              }
              break;
            }
      }
    case 4:
      {
          switch (flag_key_l_u_4)      //���������Ĳ���
            {
            case 0:       //��һҳ
              {

              }
              break;
            case 1:
              {
                   weizhi_turn--;
//                   tongbu[1]=1;
              }
              break;
            case 2:

              {
              //  game_plan--;
              }
              break;
            case 3:
              {

              }
              break;
            case 4:
              {
             //   Menu_data_num[2]-=5;  //  flag_chasu--;
              }
              break;
            case 5:
              {
            //    Kd2-=1;
             //   Kd22-=1;
              }
              break;
            case 6:
              {
            //   p2-=0.01;
              }
              break;
            case 7:     //��һҳ
              {

              }
              break;
            case 8:    //���ز˵�
              {

              }
              break;
            }
      }
    }
}


/*******************************************************************************
* Function Name: void deal_key_right()
* Input Param  : ��
* Output Param : ��
* Description  : �Ұ�������
* Author Data  : 2015/5/27  by momo
******************************************************************************/

void deal_key_right()
{
      switch (flag_key_select)
    {
    case 0:
      {

      }
      break;
    case 1:
      {
          switch (flag_key_l_u_1)      //����������Ϣ�Ĳ���
            {
            case 0:     //��һҳ
              {

              }
              break;
            case 1:     //��һҳ
              {

              }
              break;
            }
      }
    case 2:
      {
          switch (flag_key_l_u_2)      //��������Ĳ���
            {
            case 0:      //��һҳ
              {

              }
              break;
            case 1:
              {
                //  DJ_Kp_d+=5;
              }
              break;
            case 2:
              {
                //  DJ_Kd++;
              }
              break;
            case 3:
              {
                //  angle_center++;
              }
              break;
            case 4:
              {
                //   F_p+=5;
              }
              break;
            case 5:
              {
                 //  Servos_output+=10;
              }
              break;
            case 6:
              {
                 //  DJ_Kd++;
              }
              break;
            case 7:     //��һҳ
              {

              }
              break;
            case 8:      //���ز˵�
              {

              }
              break;

            }
      }
    case 3:
      {
          switch (flag_key_l_u_3)      //��������Ĳ���
            {
            case 0:       //��һҳ
              {

              }
              break;
            case 1:
              {
              //  p1+=0.0001;
                  //  Speed_set_tem+=5;
              }
              break;
            case 2:
              {
              //   Menu_data_num[0]+=5;//  flag_chasu++;
              }
              break;
            case 3:
              {
             //  Kp2+=10;
             //  Kp22+=10;
              }
              break;
            case 4:
              {
             //  Ki2+=10;
             //  Ki22+=10;
              }
              break;
            case 5:
              {
             //  Kd2+=1;
             //  Kd22+=1;
              }
              break;
            case 6:
              {
             //  Menu_data_num[1]+=5;
              }
              break;
            case 7:     //��һҳ
              {

              }
              break;
            case 8:    //���ز˵�
              {

              }
              break;
            }
      }
    case 4:
      {
          switch (flag_key_l_u_4)      //����qita�Ĳ���
            {
            case 0:       //��һҳ
              {
              }
              break;
            case 1:
              {
                    weizhi_turn++;
//                    tongbu[1]=1;
              }
              break;
            case 2:
              {
              //  game_plan++;
              }
              break;
            case 3:
              {
             //   if(flag_change_speed)   flag_change_speed=0;
             //   else   flag_change_speed=1;
              }
              break;
            case 4:
              {
             //    Menu_data_num[2]+=5;//  flag_chasu++;
              }
              break;
            case 5:
              {
             //  Kd2+=1;
             //  Kd22+=1;
              }
              break;
            case 6:
              {
             //  p2+=0.01;
              }
              break;
            case 7:     //��һҳ
              {

              }
              break;
            case 8:    //���ز˵�
              {

              }
              break;
            }
      }
    }
}


/*******************************************************************************
* Function Name: void deal_key_up()
* Input Param  : ��
* Output Param : ��
* Description  : �ϰ�������
* Author Data  : 2015/5/27  by momo
******************************************************************************/

void deal_key_up()
{
  switch (flag_key_select)
    {
    case 0:
      {
        if(flag_key_l_u_0==0)
          flag_key_l_u_0=4;
        else
          flag_key_l_u_0 --;

      }
      break;
    case 1:
      {
        if(flag_key_l_u_1==0)
          flag_key_l_u_1=1;
        else
          flag_key_l_u_1 --;

      }
      break;
    case 2:
      {
        if(flag_key_l_u_2==0)
          flag_key_l_u_2=8;
        else
          flag_key_l_u_2 --;
      }
      break;
    case 3:
      {
        if(flag_key_l_u_3==0)
          flag_key_l_u_3=8;
        else
          flag_key_l_u_3 --;
      }
      break;
    case 4:
      {

        if(flag_key_l_u_4==0)
          flag_key_l_u_4=8;
        else
          flag_key_l_u_4 --;
      }
      break;
    }

}

/*******************************************************************************
* Function Name: void deal_key_down()
* Input Param  : ��
* Output Param : ��
* Description  : �°�������
* Author Data  : 2015/5/27  by momo
******************************************************************************/

void deal_key_down()
{

    switch (flag_key_select)
    {
    case 0:
      {
        flag_key_l_u_0 ++;
        if(flag_key_l_u_0>4)
          flag_key_l_u_0=0;
      }
      break;
    case 1:
      {
        flag_key_l_u_1 ++;
        if(flag_key_l_u_1>1)
          flag_key_l_u_1=0;
      }
      break;
    case 2:
      {
        flag_key_l_u_2 ++;
        if(flag_key_l_u_2>8)
          flag_key_l_u_2=0;
      }
      break;
    case 3:
      {
        flag_key_l_u_3 ++;
        if(flag_key_l_u_3>8)
          flag_key_l_u_3=0;
      }
      break;
    case 4:
      {
        flag_key_l_u_4 ++;
        if(flag_key_l_u_4>8)
          flag_key_l_u_4=0;
      }
      break;
    }

}


  /*******************************************************************************
  * Function Name: void deal_key_up_1()
  * Input Param  : ��
  * Output Param : ��
  * Description  : �����ϰ�������
  * Author Data  : 2015/5/27  by momo
  ******************************************************************************/

  void deal_key_up_1()
  {
    /*
    if(flag_key_select==2)
    {
      switch (flag_key_l_r)
      {
      case 0:
        {
          //speed_add_start+=10;
        }
        break;
      case 1:
        {
          //speed_add_max+=10;
        }
        break;
      case 2:
        {
          //speed_add_radio+=10;
        }
        break;
      case 3:
        {
          //speed_want_init+=10;
        }
        break;
      case 4:
        {
          //ZL_value+=10;

        }
        break;
      case 5:
        {
         // ZR_value+=10;

        }
        break;
      case 6:
        {
         // speed_P+=10;

        }
        break;
      case 7:
        {
         // speed_I+=10;

        }
        break;
      case 8:
        {
          //direction_P+=10;
        }
        break;

      case 9:
        {
         // direction_D+=10;
        }
        break;
      case 10:
        {
         // angle_P+=10;
        }
        break;
      }
    }
    else if(flag_key_select==1)
    {
      switch(flag_key_l_r)
      {
      case 0:
        {

          //sensitivity+=10;
        }
        break;
      case 1:
        {

         // ccd_error_value+=10;
        }
        break;
      case 2:
        {
         // flag_zhijiao_double+=10;
        }
        break;
      }
    }
        else if(flag_key_select==0)
    {
      switch(flag_key_l_r)
      {
      case 0:
        {
        //acc_init+=10;
        }
        break;
      }
    }   */
  }

/*******************************************************************************
  * Function Name: void deal_key_down_1()
  * Input Param  : ��
  * Output Param : ��
  * Description  : �����°�������
  * Author Data  : 2015/5/27  by momo
  ******************************************************************************/

  void deal_key_down_1()
  {
    /*
    if(flag_key_select==2)
    {
      switch (flag_key_l_r)
      {
      case 0:
        {
         // speed_add_start-=10;
        }
        break;
      case 1:
        {
          //speed_add_max-=10;
        }
        break;
      case 2:
        {
         // speed_add_radio-=10;
        }
        break;
      case 3:
        {
         // speed_want_init-=10;
        }
        break;
      case 4:
        {

         // ZL_value-=10;

        }
        break;
      case 5:
        {
         // ZR_value-=10;

        }
        break;
      case 6:
        {
         // speed_P-=10;

        }
        break;
      case 7:
        {
         // speed_I-=10;

        }
        break;
      case 8:
        {
         // direction_P-=10;
        }
        break;


      case 9:
        {
         // direction_D-=10;
        }
        break;
      case 10:
        {
         // angle_P-=10;
        }
        break;
      case 11:
        {
         // angle_D-=10;

        }
        break;
      }
    }

    else if(flag_key_select==1)
    {
      switch(flag_key_l_r)
      {
      case 0:
        {

         // sensitivity-=10;
        }
        break;
      case 1:
        {

         // ccd_error_value-=10;
        }
        break;
      case 2:
        {
         // flag_zhijiao_double-=10;
        }
        break;
      }
    }
        else if(flag_key_select==0)
    {
      switch(flag_key_l_r)
      {
      case 0:
        {
        // acc_init-=10;
        }
        break;
      }
    }    */
  }




