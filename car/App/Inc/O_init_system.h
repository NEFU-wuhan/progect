#ifndef _O_INIT_SYSTEM_H
#define _O_INIT_SYSTEM_H

extern void init_sys(void);

extern uint8  nrf_tx_buff[];         //Ԥ��
extern uint8  nrf_rx_buff[];         //Ԥ��
extern uint8 *imgbuff;

/*-----------------  flash  ------------------------*/
#define     ADDR                    FLASH_SECTOR_NUM - 1    //��ַ
#define induc_num       6
#define PAR_NUM         8        //flash�洢�����ĸ���

typedef struct
{
  uint16 max_val[PAR_NUM];       //���� PAR_NUM λ�������
  uint16 min_val[PAR_NUM];

}Ind_msg;//��Ϣ

extern Ind_msg Ind;
extern void read_ext();
/*--------------------------------------------------*/


#endif  //_GEMINI_INIT_SYS_H