#ifndef _VCAN_IMG2SD_H_
#define _VCAN_IMG2SD_H_

extern void img_sd_init( uint8 mode );
extern void img_sd_save(uint8 * imgaddr,uint32 size);
extern void img_sd_exit(void);
extern void img_sd_read(uint8 * dst, uint32 size, uint32 read_imgaddr);






#endif //_VCAN_IMG2SD_H_
