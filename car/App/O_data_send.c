#include "include.h"
#include "O_data_send.h"


uint8 OutData[4] = { 0 };


/*
 * Function Name: CRC_CHECK
 * Input Param  : ��ҪУ�������
 * Output Param : ����У����ֵ
 * Description  : ����ʾ����У�麯��
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
unsigned int sum=0;                         // ��У��
unsigned char send[9]={0,0,0,0,0,0,0,0,0};// ʵ�ʷ�������
send[0]=s[0]&0x00ff;      sum+=send[0];     // ���ݸߵͰ�λ����
send[1]=(s[0]&0xff00)>>8; sum+=send[1];
send[2]=s[1]&0x00ff;      sum+=send[2];
send[3]=(s[1]&0xff00)>>8; sum+=send[3];
send[4]=s[2]&0x00ff;      sum+=send[4];
send[5]=(s[2]&0xff00)>>8; sum+=send[5];
send[6]=s[3]&0x00ff;      sum+=send[6];
send[7]=(s[3]&0xff00)>>8; sum+=send[7];
send[8]=(unsigned char)(sum&0x00ff);        // У���ȡ�Ͱ�λ
uart_putbuff (UART3,send, 9);                   // ��������
}


void hswy_sendimg(uint8 *imgaddr)
{
    uint8 cmd[1] = {0xff};            //����ΰҵ_����ͷ���ڵ��� ʹ�õ�����
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
    uart_putbuff(VCAN_PORT, temp, nWidth*nHeight); //�ٷ���ͼ��
    uart_putbuff(VCAN_PORT, cmd, sizeof(cmd));    //�ȷ�������
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
 * Input Param  : ��Ҫ���͵����ݣ�û�õ�������
 * Output Param : ����У��������
 * Description  : ����ʾ�����������
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
