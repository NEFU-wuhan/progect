#ifndef __O_DISPLAY_H__
#define __O_DISPLAY_H__

///////////////////  ���������ⲿ /////////////////////////////
//extern
extern void LCD_show();
extern void lcd_show_1();
extern void lcd_show_3();
extern void lcd_show_2();
extern void lcd_show_4();      //��Ҫ���ڲɼ�ͼ����ʾ������Ϣ
extern void lcd_show_0();
extern void lcd_show_5();           //��Ҫ������ʾ����ͼ��
extern void lcd_show_6();

extern void  bmp_init();
extern void bmp_display();

extern void color_change();

///////////////////  �����ⲿ����   ///////////////////////////
extern char color[10];
extern int8 key_flag_clear;
extern uint8 fache;
extern uint8 shua_one;
extern uint8 bujiezhi ;

#endif