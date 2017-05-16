#include "include.h"
#include "O_data_send.h"


uint8 OutData[4] = { 0 };


/*
 * Function Name: CRC_CHECK
 * Input Param  : 需要校验的数组
 * Output Param : 返回校验后的值
 * Description  : 虚拟示波器校验函数
 * Author Data  : 2015/5/20
 */
static unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++)
        {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}

 void CRC(int *s)
{
unsigned int sum=0;                         // 和校验
unsigned char send[9]={0,0,0,0,0,0,0,0,0};// 实际发送数据
send[0]=s[0]&0x00ff;      sum+=send[0];     // 数据高低八位分离
send[1]=(s[0]&0xff00)>>8; sum+=send[1];
send[2]=s[1]&0x00ff;      sum+=send[2];
send[3]=(s[1]&0xff00)>>8; sum+=send[3];
send[4]=s[2]&0x00ff;      sum+=send[4];
send[5]=(s[2]&0xff00)>>8; sum+=send[5];
send[6]=s[3]&0x00ff;      sum+=send[6];
send[7]=(s[3]&0xff00)>>8; sum+=send[7];
send[8]=(unsigned char)(sum&0x00ff);        // 校验和取低八位
uart_putbuff (UART3,send, 9);                   // 发送数据
}


void hswy_sendimg(uint8 *imgaddr)
{
    uint8 cmd[1] = {0xff};            //红树伟业_摄像头串口调试 使用的命令
    uint32 i;
    uint8 temp[nWidth*nHeight];
    for(i=0;i<nWidth*nHeight;i++)
    {
        if(*imgaddr==255)
            temp[i]=0xfe;
        else
            temp[i]=0;
        imgaddr++;
    }
//    for(i=0;i<length;i++)
//    {
//        temp[left_point[i].y*nWidth+left_point[i].x]=100;
//        temp[right_point[i].y*nWidth+right_point[i].x]=200;
//        temp[mid_point[i].y*nWidth+mid_point[i].x]=30;
//    }
    uart_putbuff(VCAN_PORT, temp, nWidth*nHeight); //再发送图像
    uart_putbuff(VCAN_PORT, cmd, sizeof(cmd));    //先发送命令
}

void OutPut_Data(void)
{
  int temp[4] = {0};
  unsigned int temp1[4] = {0};
  unsigned char databuf[10] = {0};
  unsigned char i;
  unsigned short CRC16 = 0;
  for(i=0;i<4;i++)
   {

    temp[i]  = (int)OutData[i];
    temp1[i] = (unsigned int)temp[i];

   }

  for(i=0;i<4;i++)
  {
    databuf[i*2]   = (unsigned char)(temp1[i]%256);
    databuf[i*2+1] = (unsigned char)(temp1[i]/256);
  }

  CRC16 = CRC_CHECK(databuf,8);
  databuf[8] = CRC16%256;
  databuf[9] = CRC16/256;

  for(i=0;i<10;i++)
    uart_putchar(UART3,databuf[i]);
}

/*
 * Function Name: VisualScope_Output
 * Input Param  : 需要发送的数据，没用到的置零
 * Output Param : 经过校验后的数据
 * Description  : 虚拟示波器输出函数
 * Author Data  : 2015/5/20
 */
void VisualScope_Output(float data1 ,float data2 ,float data3 ,float data4)
{
  int temp[4] = {0};
  unsigned int temp1[4] = {0};
  unsigned char databuf[10] = {0};
  unsigned char i;
  unsigned short CRC16 = 0;

  temp[0] = (int)data1;
  temp[1] = (int)data2;
  temp[2] = (int)data3;
  temp[3] = (int)data4;

  temp1[0] = (unsigned int)temp[0];
  temp1[1] = (unsigned int)temp[1];
  temp1[2] = (unsigned int)temp[2];
  temp1[3] = (unsigned int)temp[3];

  for(i=0;i<4;i++)
  {
    databuf[i*2]   = (unsigned char)(temp1[i]%256);
    databuf[i*2+1] = (unsigned char)(temp1[i]/256);
  }

  CRC16 = CRC_CHECK(databuf,8);
  databuf[8] = CRC16%256;
  databuf[9] = CRC16/256;

  for(i=0;i<10;i++)
  uart_putchar(UART1,databuf[i]);
}
