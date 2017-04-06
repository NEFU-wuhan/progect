/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_ASCII.h
 * @brief      常用ASCII表
 * @author     山外科技
 * @version    v5.0
 * @date       2013-09-02
 */

#ifndef _VCAN_ASCII_H_
#define _VCAN_ASCII_H

extern unsigned char vcan_str[4*16*2];


//常用ASCII表
//偏移量32
//ASCII字符集
//偏移量32
//大小:12*6
extern unsigned char const ascii_8x16[1536];

extern unsigned char menu[2*16*2] ;              // 菜单
extern unsigned char break_menu[4*16*2];         // 返回菜单
extern unsigned char side[5*16*2];               // 赛道信息
extern unsigned char sever[5*16*2];              // 舵机参数
extern unsigned char motor_display[5*16*2];      // 电机参数
extern unsigned char page_up[3*16*2] ;           // 上一页
extern unsigned char page_down[3*16*2] ;         // 下一页
extern unsigned char exposure[2*16*2] ;          // 曝光
extern unsigned char didianya[4*16*2] ;          // 低电压报警
extern unsigned char celue[3*16*2] ;             // 策略
extern unsigned char datuxiang[4*16*2] ;         // 大图像
extern unsigned char change_speed[2*16*2] ;      // 变速
extern unsigned char yes[1*16*2] ;               // 是
extern unsigned char no[1*16*2] ;                // 否
extern unsigned char finish_line[3*16*2] ;       // 终点！
extern unsigned char speed_max[2*16*2] ;         // 高速
extern unsigned char pattern[2*16*2] ;           // 模式
extern unsigned char preliminary[2*16*2] ;       // 预赛
extern unsigned char final[2*16*2] ;             // 决赛
extern unsigned char plan[2*16*2] ;              // 方案
extern unsigned char obstacle_zi[2*16*2] ;       // 障碍
extern unsigned char ramp_zi[2*16*2] ;           // 坡道
extern unsigned char cross_zi[2*16*2] ;          // 十字
extern unsigned char speed_max_zi[4*16*2] ;      // 最大速度
extern unsigned char other_canshu[5*16*2] ;      // 其他参数
extern unsigned char start_go[3*16*2] ;             // 发车
extern const unsigned short int nBitmapDot[] ;                  // 数据表
extern const unsigned short int DuoLaAMeng[128*128];


#endif  //_VCAN_ASCII_H
