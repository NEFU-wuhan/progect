#ifndef _O_INIT_SYSTEM_H
#define _O_INIT_SYSTEM_H

extern void init_sys(void);

extern uint8  nrf_tx_buff[];         //预多
extern uint8  nrf_rx_buff[];         //预多
extern uint8 *imgbuff;
extern uint8 buff[CAMERA_SIZE];
extern uint8 date_save_buff[CAMERA_SIZE];  // CAMERA_SIZE =160*120/8=2400
extern uint8 date_read_buff[CAMERA_SIZE];

/*-----------------  flash  ------------------------*/
#define     ADDR                    FLASH_SECTOR_NUM - 1    //地址
#define induc_num       6
#define PAR_NUM         16        //flash存储参数的个数

typedef struct
{
  uint16 max_val[PAR_NUM];       //最后多 PAR_NUM 位保存参数
  uint16 min_val[PAR_NUM];

}Ind_msg;//信息

extern Ind_msg Ind;
extern void read_ext();
/*--------------------------------------------------*/


#endif  //_GEMINI_INIT_SYS_H