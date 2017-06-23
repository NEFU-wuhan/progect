#include "common.h"
#include "vcan_img2sd.h"
#include "ff.h"
#include "VCAN_camera.h"       //摄像头总头文件
#include "O_SD_Card.h"
#include "O_init_system.h"
static FATFS vcanfs;    //文件系统
static FIL   vcansrc;   //文件


void img_sd_init( uint8 mode )
{
    //保存图像模式相关定义
    int     vcanres;
    char    myfilename[20];
    uint16  imgsize[] = {
                            CAMERA_H,
                            CAMERA_W,
                            //CAMERA_COLOR,
                         };      //高、宽、摄像头颜色类型
    uint32  mybw;
    uint32  Imag_num = 0;
    //读取图像模式相关定义
    uint32 size;

    f_mount(0, &vcanfs);
    if( mode==0 ) //两种模式，1-保存图像模式和 0-读取图像模式
    {
      do
      {
        Imag_num ++;
        sprintf(myfilename, "0:/vcan%d.sd", Imag_num);
        vcanres = f_open( &vcansrc , myfilename, FA_CREATE_NEW | FA_WRITE);

        if(vcanfs.fs_type == 0)
        {
          vcansrc.fs = 0;
          return;
        }

      }while(vcanres == FR_EXIST);        //如果文件存在，则命名继续加1

      if ( vcanres == FR_OK )
      {
        vcanres = f_write(&vcansrc, imgsize, sizeof(imgsize),&mybw);  //先写入高和宽，方便上位机处理
      }
      else
      {
        f_close(&vcansrc);
        vcansrc.fs = 0;
      }
    }
    else
    {
      vcanres = f_open(&vcansrc, "0:/FIRE1.SD", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);  //打开文件，如果没有就创建，带读写打开
      if ( vcanres == FR_OK )
      {
        size = f_size(&vcansrc);                   //获取文件的总大小
        picture_sum=(size-4)/CAMERA_SIZE;
      }
    }
}

void img_sd_save(uint8 * imgaddr,uint32 size)
{
#define F_SYNC_TIME   30      //保存 n 次后 才 同步

    int   vcanres;
    uint32 mybw;
    static uint8 time = 0;
    //uint32 size = CAMERA_SIZE;

    if(vcansrc.fs != 0)
    {
        time ++;

        vcanres = f_write(&vcansrc, imgaddr, size ,&mybw);

        if(vcanres != FR_OK)
        {
            f_close(&vcansrc);
            vcansrc.fs = 0;
        }

        if(time > F_SYNC_TIME)
        {
            time = 0 ;
//            f_sync(&vcansrc);
        }
        if(SD_save_flag==1)
        {
          f_sync(&vcansrc);
          SD_save_flag=2;
          img_sd_exit();
        }
    }
}

//读取sd卡中的图像，dst-图像保存地址 size-图像占用空间大小  read_imgaddr-读取指针位置
//例：  img_sd_read( buff, CAMERA_SIZE , zhizhen )
void img_sd_read(uint8 * dst, uint32 size, uint32 read_imgaddr)
{
    uint32 sizetmp;
    f_lseek(&vcansrc, read_imgaddr);
    f_read (&vcansrc, dst, size, &sizetmp);   //读取
}

void img_sd_exit(void)
{
    f_close(&vcansrc);
    vcansrc.fs = 0;
}
