#ifndef __O_DATA_SEND_H__
#define __O_DATA_SEND_H__

//取一个数据的各个位
#define BYTE0(Temp)       (*(char *)(&Temp))
#define BYTE1(Temp)       (*((char *)(&Temp) + 1))
#define BYTE2(Temp)       (*((char *)(&Temp) + 2))
#define BYTE3(Temp)       (*((char *)(&Temp) + 3))

extern void OutPut_Data(void);

//extern void send_data(void);
//extern void CRC(int *s) ;
extern void VisualScope_Output(float data1 ,float data2 ,float data3 ,float data4);

extern uint8 OutData[4];

extern void hswy_sendimg(uint8 *imgaddr) ;


#endif  //__GEMINI_DATA_H__

