 #include "include.h"


void hswy_sendimg(uint8 *imgaddr) ;

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














