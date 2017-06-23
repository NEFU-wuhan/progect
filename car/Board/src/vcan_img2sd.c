#include "common.h"
#include "vcan_img2sd.h"
#include "ff.h"
#include "VCAN_camera.h"       //����ͷ��ͷ�ļ�
#include "O_SD_Card.h"
#include "O_init_system.h"
static FATFS vcanfs;    //�ļ�ϵͳ
static FIL   vcansrc;   //�ļ�


void img_sd_init( uint8 mode )
{
    //����ͼ��ģʽ��ض���
    int     vcanres;
    char    myfilename[20];
    uint16  imgsize[] = {
                            CAMERA_H,
                            CAMERA_W,
                            //CAMERA_COLOR,
                         };      //�ߡ�������ͷ��ɫ����
    uint32  mybw;
    uint32  Imag_num = 0;
    //��ȡͼ��ģʽ��ض���
    uint32 size;

    f_mount(0, &vcanfs);
    if( mode==0 ) //����ģʽ��1-����ͼ��ģʽ�� 0-��ȡͼ��ģʽ
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

      }while(vcanres == FR_EXIST);        //����ļ����ڣ�������������1

      if ( vcanres == FR_OK )
      {
        vcanres = f_write(&vcansrc, imgsize, sizeof(imgsize),&mybw);  //��д��ߺͿ�������λ������
      }
      else
      {
        f_close(&vcansrc);
        vcansrc.fs = 0;
      }
    }
    else
    {
      vcanres = f_open(&vcansrc, "0:/FIRE1.SD", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);  //���ļ������û�оʹ���������д��
      if ( vcanres == FR_OK )
      {
        size = f_size(&vcansrc);                   //��ȡ�ļ����ܴ�С
        picture_sum=(size-4)/CAMERA_SIZE;
      }
    }
}

void img_sd_save(uint8 * imgaddr,uint32 size)
{
#define F_SYNC_TIME   30      //���� n �κ� �� ͬ��

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

//��ȡsd���е�ͼ��dst-ͼ�񱣴��ַ size-ͼ��ռ�ÿռ��С  read_imgaddr-��ȡָ��λ��
//����  img_sd_read( buff, CAMERA_SIZE , zhizhen )
void img_sd_read(uint8 * dst, uint32 size, uint32 read_imgaddr)
{
    uint32 sizetmp;
    f_lseek(&vcansrc, read_imgaddr);
    f_read (&vcansrc, dst, size, &sizetmp);   //��ȡ
}

void img_sd_exit(void)
{
    f_close(&vcansrc);
    vcansrc.fs = 0;
}
