#ifndef __O_PICTURE_DEAL_H_
#define __O_PICTURE_DEAL_H_

#define line_num 70

#define nWidth  128
#define nHeight 70

extern void    median_line_extract(uint8 *src);
//extern

extern uint8  used_length;
extern float last_mid_line;
extern uint8 single_in;

extern uint8 shi_zi_count;
extern uint8 lost_count;
extern uint8 never_shi_zi_count;

extern uint8  X_point[70];
extern uint8  Y_point[70];
extern float last_mid_line;

extern uint8 fiag_huan_r,fiag_huan_l,fiag_huan;
extern uint8 follow_huan;    //1 ΪѰ���ߣ�0ΪѰ����

extern uint8 hehe,hehe1;
extern uint8 xia_no;
extern uint8 road_count_chao;
extern uint8 road_wide_chao[];
extern uint8 R_wrz_flag;
extern uint8 L_wrz_flag;


typedef enum
{
    normal,
    lose,
    left_normal_right_lose,
    left_lose_right_normal,
    all_normal,
    all_lose,
    all_back_line,
    starline
}line_case;

typedef struct
{
  int16 left_line;     //���ڱ궨���ұ߽������
  int16 right_line;
  int16 mid_line;

  int16 left_line_unrepiar;
  int16 right_line_unrepiar;
  int16 mid_line_new;

  line_case left_line_state;    //���ڱ궨���ұ߽�״̬
  line_case right_line_state;

  int16 left_single;
  int16 right_single;
  uint16 road_wide;
  uint8 single_road_wide;

  line_case line_case_mode;

} line_info;


extern uint8 zhi_dao_flag;
extern uint8 wrz_flag;
extern line_info  line[line_num];
extern uint8  all_left_line[line_num][20];
extern uint8  all_right_line[line_num][20];
extern uint8 origin_chao( uint8 cut );
//������ʶ��
extern uint8 left_lost_count;
extern uint8 right_lost_count;
extern uint8 yiting;
extern int ccout;
extern uint8 stop_count;


#endif //___GEMINI_PICTURE_DEAL_H