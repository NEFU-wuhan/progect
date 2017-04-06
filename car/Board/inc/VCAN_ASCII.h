/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_ASCII.h
 * @brief      ����ASCII��
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-09-02
 */

#ifndef _VCAN_ASCII_H_
#define _VCAN_ASCII_H

extern unsigned char vcan_str[4*16*2];


//����ASCII��
//ƫ����32
//ASCII�ַ���
//ƫ����32
//��С:12*6
extern unsigned char const ascii_8x16[1536];

extern unsigned char menu[2*16*2] ;              // �˵�
extern unsigned char break_menu[4*16*2];         // ���ز˵�
extern unsigned char side[5*16*2];               // ������Ϣ
extern unsigned char sever[5*16*2];              // �������
extern unsigned char motor_display[5*16*2];      // �������
extern unsigned char page_up[3*16*2] ;           // ��һҳ
extern unsigned char page_down[3*16*2] ;         // ��һҳ
extern unsigned char exposure[2*16*2] ;          // �ع�
extern unsigned char didianya[4*16*2] ;          // �͵�ѹ����
extern unsigned char celue[3*16*2] ;             // ����
extern unsigned char datuxiang[4*16*2] ;         // ��ͼ��
extern unsigned char change_speed[2*16*2] ;      // ����
extern unsigned char yes[1*16*2] ;               // ��
extern unsigned char no[1*16*2] ;                // ��
extern unsigned char finish_line[3*16*2] ;       // �յ㣡
extern unsigned char speed_max[2*16*2] ;         // ����
extern unsigned char pattern[2*16*2] ;           // ģʽ
extern unsigned char preliminary[2*16*2] ;       // Ԥ��
extern unsigned char final[2*16*2] ;             // ����
extern unsigned char plan[2*16*2] ;              // ����
extern unsigned char obstacle_zi[2*16*2] ;       // �ϰ�
extern unsigned char ramp_zi[2*16*2] ;           // �µ�
extern unsigned char cross_zi[2*16*2] ;          // ʮ��
extern unsigned char speed_max_zi[4*16*2] ;      // ����ٶ�
extern unsigned char other_canshu[5*16*2] ;      // ��������
extern unsigned char start_go[3*16*2] ;             // ����
extern const unsigned short int nBitmapDot[] ;                  // ���ݱ�
extern const unsigned short int DuoLaAMeng[128*128];


#endif  //_VCAN_ASCII_H
