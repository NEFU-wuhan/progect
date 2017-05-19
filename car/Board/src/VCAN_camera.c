#include "common.h"
#include "main.h"

uint16 camera_boundary=16;
//ѹ����ֵ��ͼ���ѹ���ռ� �� ʱ�� ��ѹ��
//srclen �Ƕ�ֵ��ͼ���ռ�ÿռ��С
//��ӥ�۽�ѹ��ӥ��ͼ���ѹ��תΪ ��ά���� - ���ܳ������� - ɽ����̳ http://vcan123.com/forum.php?mod=viewthread&tid=17&ctid=6
//��ѹ��ʱ�������и����飬���úڡ��׶�Ӧ��ֵ�Ƕ��١�
void img_extract(void *dst, void *src, uint32_t srclen)
{
    uint8_t colour[2] = {255, 0}; //0 �� 1 �ֱ��Ӧ����ɫ
    uint8_t * mdst = dst;
    uint8_t * msrc = src;
    //ע��ɽ�������ͷ 0 ��ʾ ��ɫ��1��ʾ ��ɫ
    uint8_t tmpsrc;
    while(srclen --)
    {
        tmpsrc = *msrc++;
        *mdst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}

 void img_extract1(uint8 *dst, uint8 *src)
{

   uint8 line_distance[120]=
{
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
  uint8 color[2] = {255, 0}; //0 �� 1 �ֱ��Ӧ����ɫ
  //ע��Ұ�������ͷ 0 ��ʾ ��ɫ��1��ʾ ��ɫ
  uint8 tmpsrc;
  uint8 all_img[160];
  uint8 *p_all;
  int i,j;
  for(i=0;i<120;i++)
  {
    if(line_distance[i]==1)             //ǰ80����1���ʮ����0
    {
        p_all=all_img;
        for(j=0;j<20;j++)
        {
            tmpsrc = *src++;                         //�����ѹǰһ���ֽ���������Ķ���
            *p_all++ = color[ (tmpsrc >> 7 ) & 0x01 ];
            *p_all++ = color[ (tmpsrc >> 6 ) & 0x01 ];
            *p_all++ = color[ (tmpsrc >> 5 ) & 0x01 ];
            *p_all++ = color[ (tmpsrc >> 4 ) & 0x01 ];
            *p_all++ = color[ (tmpsrc >> 3 ) & 0x01 ];
            *p_all++ = color[ (tmpsrc >> 2 ) & 0x01 ];
            *p_all++ = color[ (tmpsrc >> 1 ) & 0x01 ];
            *p_all++ = color[ (tmpsrc >> 0 ) & 0x01 ];
        }

#if (CAR_MODEL==1  )
        camera_boundary=10;
//        for(j=13;j<141;j++)
#elif (CAR_MODEL==2  )
          camera_boundary=25;
//        for(j=25;j<153;j++)   //27-155
#elif (CAR_MODEL==3 )
          camera_boundary=23;
//        for(j=17;j<145;j++)
#endif

        for(j=camera_boundary;j<camera_boundary+128;j++)
        {
            *dst++ = all_img[j];
        }
    }
    else
    {
      src+=20;
    }
  }
}

