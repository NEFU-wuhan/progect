#ifndef __O_MENU_CONTROL_h__
#define __O_MENU_CONTROL_h__

/*     ���ⶨ�����еı����Ͳ��ֺ���
 *
 *
 *
 *
 *
 */

/////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////

extern uint8 Beep_En;  //������ʹ��
extern int32 Menu_data_num[20];//���ݴ洢��
extern uint32 System_time;          //���ο���ϵͳʱ��
extern uint32 Start_timing;          //������ʱ

extern uint8 Time_En;               //��ʱʹ�ܱ�ʶλ
extern int8 Motor_En;      //�綯��ʹ�� 0--disable  1--enable

extern uint8 Servos_En;


extern short int nextError;

extern int32 speedaboutangle;
extern int32 speedaboutangle2;
extern int32 speedaboutangle_zong;

extern float p1;//ԭֵ0.8043     0.004076
extern float p2;//ԭֵ1.0220

extern float y;
extern int stop;

extern long int rightval;
extern long int leftval;
extern int16 dianya;
extern int16 flag_didianya;
extern uint8 boma[4];

extern uint32 zhenshu_count;
extern uint32 camera_zhenshu;
extern uint8 display_zs;
////////////////////////////  ��������  ////////////////////////
extern int16 bat_voltage_measure();
extern void Time_Commen();

extern uint8 Cross_z_jibu,Cross_jici_ru;
extern int16 Init_flag_Len;
extern uint8 Cross_flag_pre,Cross_jici;
extern uint16 Cross_Len3,Cross_Len4;
extern uint32 Ramp_Len_x,Ramp_jici;
extern uint8 suoxian_kaishi;
extern int16 sensitivity_an;  //����ɨ��������
extern uint8 yuzhi_genggai;
extern uint16 speed_ketiao;
extern uint8  flag_shuaping;

#endif
