#include "include.h"
//#include "PORT_CFG.h"

#define Variable_ADDR   2
#define Para_ADDR   102
#define CCD_ADDR   212
#define  SD_CS PTE4_OUT


#define SD_CLK PTE2_OUT
#define SD_DI  PTE3_IN    //输入
#define SD_DO  PTE1_OUT    //输出

uint8 SD_save_flag=0;

uint8   SDHC_Buff[512];
uint8   SD_OK=0;
uint8   SaveData=0;
uint16  Block_Index=0;
uint8   Zone_Index=0;
uint8   Zone_Read_Index=0;
uint8   Block_Index_Receiving;
uint8   SD_Type = 0;

void   SD_DisSelect();  //取消片选信号
uint8  SD_GetResponse(uint8 Response);     //获取应答信号
uint8  SD_RecvData(uint8*buf,uint16 len);   //从sd卡读取一个数据包的内容

//uint8  SD_Select();      //
void   SD_SPI_HighSpeed();   //设置高速模式
void   SD_SPI_LowSpeed();    //设置低速模式
uint8  SD_WaitReady();       //等待写入
uint8  SD_WriteDisk(uint8*buf,uint32 sector);  //写SD卡
uint8  SPI_SendReceiveData(uint8 TxData);      //写数据
uint8  SD_SendCmd(uint8 cmd, uint32 arg, uint8 crc);     //向SD卡发送一个命令

//将PID变量的地址加到指针数组里
//float *Control_Para[16]={&PID_ANGLE.P,&PID_ANGLE.D,&PID_SPEED.P,&PID_SPEED.I,&PID_SPEED.D,&PID_TURN.P,&PID_TURN.D,&SetSpeed,
//                         &Fuzzy_Kp,&Fuzzy_Kd,&Delta_P,&Delta_D};
//float *Variable[20]={&Angle,&Angle_Speed,&Car_Angle,&Middle_Err,&Delt_error,&Turn_Speed,&CarSpeed,&Voltage,&PID_ANGLE.OUT,&PID_SPEED.OUT,&PID_TURN.OUT,
//                        &RightMotorOut,&LeftMotorOut,&ControlSpeed,&Distance,&RunTime,&Character_Distance,&reserve,&reserve,&reserve};

//用来在第一个页面存储一共有多少页有用数据，车子自记录以来跑了多少米
void  Write_Information()
{
//  SDHC_Buff[0]=BYTE0(Block_Index);
//  SDHC_Buff[1]=BYTE1(Block_Index);
//  SDHC_Buff[2]=BYTE0(Distance);
//  SDHC_Buff[3]=BYTE1(Distance);
//  SDHC_Buff[4]=BYTE2(Distance);
//  SDHC_Buff[5]=BYTE3(Distance);
  SD_WriteDisk(SDHC_Buff,Zone_Size);//第一个扇区用来记录重要的信息
  Block_Index=1;                       //其他数据只能从第二个扇区写
}
//用来存储运行数据
void Record()
{
//用来更新缓存，每10ms执行一次
  float temp;
  uint8 i;
  SDHC_Buff[0]=BYTE0(Block_Index);
  SDHC_Buff[1]=BYTE1(Block_Index);
   for(i=0;i<20;i++)
  {
//    temp=*Variable[i]+0.0000005;
    SDHC_Buff[Variable_ADDR+i*4]=BYTE0(temp);
    SDHC_Buff[Variable_ADDR+i*4+1]=BYTE1(temp);
    SDHC_Buff[Variable_ADDR+i*4+2]=BYTE2(temp);
    SDHC_Buff[Variable_ADDR+i*4+3]=BYTE3(temp);
  }

  for(i=0;i<12;i++)
  {
//    temp=*Control_Para[i]+0.0000005;
    SDHC_Buff[Para_ADDR+i*4]=BYTE0(temp);
    SDHC_Buff[Para_ADDR+i*4+1]=BYTE1(temp);
    SDHC_Buff[Para_ADDR+i*4+2]=BYTE2(temp);
    SDHC_Buff[Para_ADDR+i*4+3]=BYTE3(temp);
  }
   for(i=0;i<150;i++)
   {
//     SDHC_Buff[CCD_ADDR+i]=CCD_Draw_Buff[i];
   }
   SD_WriteDisk(SDHC_Buff,Zone_Size+Block_Index);
//   LED_BLUE_TURN;                          //LED灯指示
   Block_Index++;
}
//从sd卡读取一个数据包的内容
//buf:数据缓存区
//len:要读取的数据长度.
//返回值:0,成功;其他,失败;
uint8 SD_RecvData(uint8*buf,uint16 len)
{
   if(SD_GetResponse(0xFE)) return 1;//等待SD卡发回数据起始令牌0xFE
    while(len--)//开始接收数据
    {
      *buf=SPI_SendReceiveData(0xFF);
       buf++;
    }
    //下面是2个伪CRC（dummy CRC）
    SPI_SendReceiveData(0xFF);
    SPI_SendReceiveData(0xFF);
    return 0;//读取成功
}
//-------------------------------------------------
//功能 初始化判断SD卡状态
//--------------------------------------------------
uint8 SD_Initialize(void)
{
    uint8 r1;           //存放SD卡的返回值
    uint16 retry;       //用来进行超时计数
    uint16 i;
    uint8 buff[4];
    uint8 success=0;
    SD_SPI_LowSpeed();	//设置到低速模式
    while(1)
    SD_DisSelect();
    for(i=0;i<10;i++)SPI_SendReceiveData(0XFF);//发送最少74个脉冲
    retry=20;
    do
    {
       r1=SD_SendCmd(CMD0,0,0x95);//进入IDLE状态  （CMD0是卡复位操作）
       SD_DisSelect();            //取消片选
    }
    while((r1!=0X01)&&retry--);
    SD_Type=0;//默认无卡
    if(r1==0X01)
    {
      if(SD_SendCmd(CMD8,0x1AA,0x87)==1)//SD V2.0
      {
        retry=0XFFFE;
        do
        {
            SD_SendCmd(CMD55,0,1);	       //发送CMD55
            r1=SD_SendCmd(CMD41,0x40000000,1);//发送CMD41
        }while(r1&&retry--);
        if(r1==0)success=1;  //初始化成功了
        r1 = SD_SendCmd(CMD58, 0, 1);
        if(r1==0)
        {
          buff[0] =SPI_SendReceiveData(0xFF);
          buff[1] =SPI_SendReceiveData(0xFF);
          buff[2] =SPI_SendReceiveData(0xFF);
          buff[3] =SPI_SendReceiveData(0xFF);
          SD_DisSelect();//取消片选
          if(buff[0]&0x40)SD_Type = SD_TYPE_V2HC; //检查CCS
          else SD_Type = SD_TYPE_V2;
        }
      }
    }
    SD_DisSelect();
    SD_SPI_HighSpeed();//高速
    return success;//其他错误
}
//读SD卡
//buf:数据缓存区
//sector:扇区
//cnt:扇区数
//返回值:0,ok;其他,失败.
uint8 SD_ReadDisk(uint8*buf,uint32 sector)
{
  uint8 r1;
  if(SD_Type!=SD_TYPE_V2HC)sector<<= 9;//转换为字节地址
  r1=SD_SendCmd(CMD17,sector,0X01);//读命令
  if(r1==0)//指令发送成功
  {
    r1=SD_RecvData(buf,512);//接收512个字节
  }
  SD_DisSelect();//取消片选
  return r1;
}
//写SD卡
//buf:数据缓存区
//sector:起始扇区
//cnt:扇区数
//返回值:0,ok;其他,失败.
uint8 SD_WriteDisk(uint8*buf,uint32 sector)
{
  uint8 r1;
  uint16 t;
  uint16 retry;
  if(SD_Type!=SD_TYPE_V2HC)sector <<= 9;//转换为字节地址
  if(SD_Type!=SD_TYPE_MMC)
  {
    SD_SendCmd(CMD55,0,0X01);
    SD_SendCmd(CMD23,1,0X01);//发送指令
  }
  r1=SD_SendCmd(CMD24,sector,0X01);//单块读命令
  if(r1==0)
  {

    SD_CS=0;
    SPI_SendReceiveData(0XFF);
    SPI_SendReceiveData(0XFF);
    SPI_SendReceiveData(0XFF);

    SPI_SendReceiveData(0XFE);//开始写

    for(t=0;t<512;t++)SPI_SendReceiveData(buf[t]);//提高速度,减少函数传参时间

    SPI_SendReceiveData(0xFF);//忽略crc
    SPI_SendReceiveData(0xFF);
    t=SPI_SendReceiveData(0xFF);//接收响应
    if((t&0x1F)!=0x05)return 2;//响应错误

   }
    retry = 0;
    while(!SPI_SendReceiveData(0xff))
    {
      retry++;
      if(retry>0xfffe) //如果长时间写入没有完成，报错退出
      {
          SD_CS=1;
          return 1; //写入超时返回1
      }
    }
  SD_DisSelect();//取消片选
  return 1;//
}


//------------------------------------------------------------------------------
// SPI1 读写一个字节
// TxData:要写入的字节
// 返回值:读取到的字节
// 在读数据的时候，由于设置SPI为全双工，所以当主机往从机发送0xff时，数据无效，只是相当于给了从机八个sck，
// 而在发送数据的同时，从机按照sck就将数据输出至主机缓存，所以在读的过程中，上述代码的上半段是为了产生脉冲而已，下半段是从缓存中读取数据。
//------------------------------------------------------------------------------
uint8 SPI_SendReceiveData(uint8 TxData)
{
  uint16 retry=0;
//--1--   发送寄存器非空等待
  while(  ( SPI_SR_REG(SPI1_BASE_PTR) & SPI_SR_RFDF_MASK )  )  //等待发送区空
  {
   // retry++;
   // if(retry>200)return 0;                                     //超时退出
  }
//--2--   发送数据
  SPI_PUSHR_REG(SPI1_BASE_PTR)=TxData;                         //  发送一个byte
//--3--   检测接收缓冲区非空
  retry=0;
  while(!(SPI_SR_REG(SPI1_BASE_PTR) & SPI_SR_RFDF_MASK))       //RFDF为1，Rx FIFO is not empty.
  {
//    retry++;
//    if(retry>2000)return 0;
  }
//--4--   接受数据
 return   SPI_POPR_REG(SPI1_BASE_PTR);                         // 返回接收到的数据
}
//uint8 SPI_SendReceiveData(uint8 TxData)
//{
//   uint8 i;
//   for(i=8;i>0;i--)    //写
//   {
//    SD_CLK=0;
//    if(TxData&0x80) SD_DI=1;
//    else SD_DO=0;
//    TxData<<=1;
//    SD_CLK=1;
//   }
//
//   for (i=8;i>0;i--)    //读
//    {
//      SD_CLK=0;
//      SD_CLK=1;
//      TxData<<=1;
//      if (SD_DI) TxData|=1;
//    }
//    return TxData;
//
//
//}
//向SD卡发送一个命令
//输入: uint8 cmd   命令
//      u32 arg  命令参数
//      uint8 crc   crc校验值
//返回值:SD卡返回的响应
uint8 SD_SendCmd(uint8 cmd, uint32 arg, uint8 crc)
{
    uint8 r1;
    uint8 Retry=0;
    SD_DisSelect();//取消片选
    SD_CS=0; //片选
    SPI_SendReceiveData(cmd | 0x40);//分别写入命令
    SPI_SendReceiveData(arg >> 24);
    SPI_SendReceiveData(arg >> 16);
    SPI_SendReceiveData(arg >> 8);
    SPI_SendReceiveData(arg);
    SPI_SendReceiveData(crc);
    Retry=0X1F;
    do
    {
     r1=SPI_SendReceiveData(0xFF);
    }
    while((r1&0X80)&&Retry--);
    //返回状态值;
    return r1;
}

//////////////////////////
//设置SD卡低速模式
/////////////////////////
void SD_SPI_LowSpeed()
{
  spi_set_baud (SPI1,200*1000);
}
//////////////////////////
//设置SD卡高速模式
////////////////////////
void SD_SPI_HighSpeed()
{
  spi_set_baud (SPI1,10000*1000);
}
//////////////////////////
//取消片选
////////////////////////
void SD_DisSelect()
{
  SD_CS=1; //取消片选
  SPI_SendReceiveData(0xff);   //提供额外的8个时钟
}
//////////////////////////
//等待写入
////////////////////////
uint8 SD_WaitReady()
{
  uint32 t = 0;
  do
  {
    if(SPI_SendReceiveData(0xff) == 0xff) return 0;
    t++;
  }while(t < 0xffffff);
  return 1;
}
//////////////////////////
//得到应答
////////////////////////
uint8 SD_GetResponse(uint8 Response)
{
  uint16 Count=0xFFFF;//等待次数
  while((SPI_SendReceiveData(0XFF)!=Response)&&Count) Count--;//等待得到准确的回应
  if(Count==0)return MSD_RESPONSE_FAILURE;//得到回应失败
  else return MSD_RESPONSE_NO_ERROR;//正确回应
}

