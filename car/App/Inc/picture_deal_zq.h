#ifndef __GEMINI_PICTURE_DEAL_H_
#define ___GEMINI_PICTURE_DEAL_H_

#define line_num 70

#define nWidth  128
#define nHeight 70

extern void    median_line_extract(uint8 *src);
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


typedef enum
{
    normal,
    lose,
    left_normal_right_lose,
    left_lose_right_normal,
    all_normal,
    all_lose,
    all_back_line
}line_case;

typedef struct
{
  int16 left_line;     //用于标定左右边界和中线
  int16 right_line;
  int16 mid_line;

  int16 left_line_unrepiar;
  int16 right_line_unrepiar;
  int16 mid_line_new;

  line_case left_line_state;    //用于标定左右边界状态
  line_case right_line_state;

  int16 left_single;
  int16 right_single;
  uint16 road_wide;
  uint8 single_road_wide;

  line_case line_case_mode;

} line_info;

extern line_info  line[line_num];

#endif //___GEMINI_PICTURE_DEAL_H