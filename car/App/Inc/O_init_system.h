#ifndef _O_INIT_SYSTEM_H
#define _O_INIT_SYSTEM_H

extern void init_sys(void);

extern uint8  nrf_tx_buff[];         //预多
extern uint8  nrf_rx_buff[];         //预多
extern uint8 *imgbuff;

/*-----------------  flash  ------------------------*/
#define     ADDR                    FLASH_SECTOR_NUM - 1    //地址
#define induc_num       6
#define PAR_NUM         8        //flash存储参数的个数

typedef struct
{
  uint16 max_val[PAR_NUM];       //最后多 PAR_NUM 位保存参数
  uint16 min_val[PAR_NUM];

}Ind_msg;//信息

extern Ind_msg Ind;
extern void read_ext();
/*--------------------------------------------------*/


#endif  //_GEMINI_INIT_SYS_H