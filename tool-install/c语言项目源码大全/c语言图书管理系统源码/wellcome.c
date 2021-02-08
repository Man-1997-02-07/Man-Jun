#include"graphics.h"
void drawmat(char *mat,int matsize,int x,int y,int color)
/*依次：字模指针、点阵大小、起始坐标(x,y)、颜色*/
{
int i, j, k, n;
n = (matsize - 1) / 8 + 1;
for(j = 0; j < matsize; j++)
for(i = 0; i < n; i++)
for(k = 0;k < 8; k++)
if(mat[j * n + i] & (0x80 >> k)) /*测试为1的位则显示*/
putpixel(x + i * 8 + k, y + j, color);
}
char bookm[]={
/* the following data and text is protected by Copyright law and international copyright treaty provisions! please don't copy or sale without Qinwenhao's authorization */
/* @0 图(32x32,H)@ [suki software]*/
0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x04,0x00,0x00,0x40,  0x07,0xFF,0xFF,0xE0,  
0x06,0x0E,0x00,0x40,  0x06,0x08,0x10,0x40,  0x06,0x1F,0xF8,0x40,  0x06,0x18,0x38,0x40,  
0x06,0x28,0x20,0x40,  0x06,0x24,0x60,0x40,  0x06,0x44,0xC0,0x40,  0x06,0x83,0x80,0x40,  
0x06,0x03,0x00,0x40,  0x06,0x07,0xC0,0x40,  0x06,0x08,0x70,0x40,  0x06,0x30,0x3F,0x40,  
0x06,0xC3,0x0E,0x40,  0x07,0x01,0xC0,0x40,  0x06,0x00,0xC0,0x40,  0x06,0x00,0x00,0x40,  
0x06,0x1F,0x00,0x40,  0x06,0x03,0xE0,0x40,  0x06,0x00,0x60,0x40,  0x06,0x00,0x20,0x40,  
0x06,0x00,0x00,0x40,  0x07,0xFF,0xFF,0xC0,  0x06,0x00,0x00,0x40,  0x04,0x00,0x00,0x00,  
0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  
/* @1 书(32x32,H)@ [suki software]*/
0x00,0x00,0x00,0x00,  0x00,0x08,0x00,0x00,  0x00,0x06,0x06,0x00,  0x00,0x04,0x03,0x80,  
0x00,0x04,0x00,0xC0,  0x00,0x04,0x00,0xE0,  0x00,0x04,0x02,0x60,  0x03,0xFF,0xFF,0x20,  
0x00,0x04,0x06,0x00,  0x00,0x04,0x06,0x00,  0x00,0x04,0x06,0x00,  0x00,0x04,0x06,0x00,  
0x00,0x04,0x06,0x00,  0x00,0x04,0x04,0x00,  0x00,0x04,0x04,0x20,  0x0F,0xFF,0xFB,0xF0,  
0x00,0x04,0x00,0x20,  0x00,0x04,0x00,0x60,  0x00,0x04,0x00,0x60,  0x00,0x04,0x00,0x60,  
0x00,0x04,0x00,0x60,  0x00,0x04,0x10,0xC0,  0x00,0x04,0x0F,0xC0,  0x00,0x04,0x03,0x80,  
0x00,0x04,0x01,0x00,  0x00,0x04,0x00,0x00,  0x00,0x04,0x00,0x00,  0x00,0x0C,0x00,0x00,  
0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  
/* @2 管(32x32,H)@ [suki software]*/
0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x01,0xC0,0x30,0x00,  0x01,0x80,0x30,0x00,  
0x03,0x06,0x60,0x60,  0x02,0xF8,0x5F,0x80,  0x04,0x60,0x84,0x00,  0x08,0x23,0x06,0x00,  
0x10,0x23,0x02,0x00,  0x04,0x01,0x00,0x20,  0x04,0x01,0x00,0x70,  0x0F,0xFE,0xFF,0xF0,  
0x08,0x00,0x00,0x40,  0x18,0xC0,0x0C,0x80,  0x00,0xFF,0xFC,0x00,  0x00,0xC0,0x0C,0x00,  
0x00,0xC0,0x0C,0x00,  0x00,0xFF,0xFC,0x00,  0x00,0xC0,0x0C,0x00,  0x00,0xC0,0x00,0x00,  
0x00,0xC0,0x00,0x00,  0x00,0xFF,0xFE,0x00,  0x00,0xC0,0x06,0x00,  0x00,0xC0,0x06,0x00,  
0x00,0xC0,0x06,0x00,  0x00,0xFF,0xFE,0x00,  0x00,0xC0,0x06,0x00,  0x00,0xC0,0x00,0x00,  
0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  
/* @3 理(32x32,H)@ [suki software]*/
0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x02,0x00,0x40,  
0x00,0x33,0xFF,0xE0,  0x1F,0xFA,0x18,0x40,  0x01,0x82,0x18,0x40,  0x01,0x82,0x18,0x40,  
0x01,0x82,0x18,0x40,  0x01,0x83,0xFF,0xC0,  0x01,0x82,0x18,0x40,  0x01,0x82,0x18,0x40,  
0x01,0xB2,0x18,0x40,  0x0F,0xC2,0x18,0x40,  0x01,0x82,0x18,0x40,  0x01,0x83,0xFF,0xC0,  
0x01,0x82,0x18,0x00,  0x01,0x80,0x18,0x00,  0x01,0x80,0x18,0x00,  0x01,0x88,0x18,0x40,  
0x01,0xF7,0xFF,0xE0,  0x03,0x80,0x18,0x00,  0x3E,0x00,0x18,0x00,  0x18,0x00,0x18,0x00,  
0x00,0x00,0x18,0x20,  0x00,0x3F,0xFF,0xF0,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  
0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  
/* @4 系(32x32,H)@ [suki software]*/
0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x80,  0x00,0x00,0x07,0xC0,  0x00,0x01,0xFC,0x00,  
0x01,0xFE,0x00,0x00,  0x06,0x03,0xC0,0x00,  0x00,0x06,0x00,0x00,  0x00,0x0C,0x06,0x00,  
0x00,0x10,0x0E,0x00,  0x00,0x60,0x18,0x00,  0x01,0xFF,0xE0,0x00,  0x00,0xC0,0xC0,0x00,  
0x00,0x03,0x00,0x00,  0x00,0x0C,0x04,0x00,  0x00,0x10,0x03,0x00,  0x00,0xE0,0x1F,0xC0,  
0x01,0xFF,0xE0,0xC0,  0x01,0x80,0x80,0xC0,  0x00,0x00,0x80,0x00,  0x00,0x38,0x88,0x00,  
0x00,0x30,0x86,0x00,  0x00,0x60,0x83,0x80,  0x01,0x80,0x81,0xE0,  0x03,0x00,0x80,0xE0,  
0x04,0x00,0x80,0x60,  0x08,0x1D,0x80,0x20,  0x00,0x07,0x80,0x00,  0x00,0x03,0x00,0x00,  
0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  
/* @5 统(32x32,H)@ [suki software]*/
0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0xC0,0x20,0x00,  0x00,0xC0,0x30,0x00,  
0x01,0x80,0x10,0x00,  0x01,0x00,0x10,0x60,  0x03,0x07,0xFF,0xF0,  0x02,0x18,0x30,0x00,  
0x04,0x18,0x30,0x00,  0x0C,0x30,0x64,0x00,  0x1F,0xE0,0xC3,0x00,  0x1C,0x41,0x81,0xC0,  
0x00,0x83,0x00,0xE0,  0x01,0x07,0xFF,0x60,  0x03,0x02,0x88,0x20,  0x06,0x00,0xE6,0x00,  
0x0C,0xF0,0xC4,0x00,  0x0F,0x00,0xC4,0x00,  0x08,0x00,0xC4,0x00,  0x00,0x00,0xC4,0x00,  
0x00,0x00,0xC4,0x00,  0x00,0x70,0x84,0x00,  0x07,0x81,0x84,0x20,  0x1C,0x01,0x04,0x10,  
0x10,0x02,0x04,0x10,  0x00,0x0C,0x06,0x38,  0x00,0x10,0x07,0xF0,  0x00,0x60,0x00,0x00,  
0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  

};
char cyy[]={/* the following data and text is protected by Copyright law and international copyright treaty provisions! please don't copy or sale without Qinwenhao's authorization */
/* @0 C(64x64,H)@ [suki software]*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x01,0xFC,0x08,0x00,0x00,0x00,0x00,  0x00,0x07,0xFF,0xF8,0x00,0x00,0x00,0x00,  
0x00,0x1F,0x03,0xF8,0x00,0x00,0x00,0x00,  0x00,0x3C,0x00,0xF8,0x00,0x00,0x00,0x00,  0x00,0x78,0x00,0x78,0x00,0x00,0x00,0x00,  0x00,0xF0,0x00,0x3C,0x00,0x00,0x00,0x00,  
0x01,0xF0,0x00,0x1C,0x00,0x00,0x00,0x00,  0x01,0xE0,0x00,0x0C,0x00,0x00,0x00,0x00,  0x03,0xE0,0x00,0x0C,0x00,0x00,0x00,0x00,  0x03,0xC0,0x00,0x06,0x00,0x00,0x00,0x00,  
0x07,0xC0,0x00,0x06,0x00,0x00,0x00,0x00,  0x07,0x80,0x00,0x06,0x00,0x00,0x00,0x00,  0x0F,0x80,0x00,0x00,0x00,0x00,0x00,0x00,  0x0F,0x80,0x00,0x00,0x00,0x00,0x00,0x00,  
0x0F,0x80,0x00,0x00,0x00,0x00,0x00,0x00,  0x0F,0x80,0x00,0x00,0x00,0x00,0x00,0x00,  0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
0x0F,0x80,0x00,0x00,0x00,0x00,0x00,0x00,  0x0F,0x80,0x00,0x00,0x00,0x00,0x00,0x00,  0x0F,0x80,0x00,0x04,0x00,0x00,0x00,0x00,  0x0F,0x80,0x00,0x06,0x00,0x00,0x00,0x00,  
0x07,0x80,0x00,0x06,0x00,0x00,0x00,0x00,  0x07,0xC0,0x00,0x0C,0x00,0x00,0x00,0x00,  0x07,0xC0,0x00,0x0C,0x00,0x00,0x00,0x00,  0x03,0xC0,0x00,0x18,0x00,0x00,0x00,0x00,  
0x03,0xE0,0x00,0x18,0x00,0x00,0x00,0x00,  0x01,0xF0,0x00,0x30,0x00,0x00,0x00,0x00,  0x00,0xF0,0x00,0x60,0x00,0x00,0x00,0x00,  0x00,0x7C,0x00,0xC0,0x00,0x00,0x00,0x00,  
0x00,0x3F,0x03,0x80,0x00,0x00,0x00,0x00,  0x00,0x0F,0xFF,0x00,0x00,0x00,0x00,0x00,  0x00,0x03,0xFC,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
/* @1 语(64x64,H)@ [suki software]*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,  0x00,0xE0,0x00,0x00,0x00,0x00,0x0E,0x00,  0x00,0x78,0x00,0x00,0x00,0x00,0x1F,0x00,  
0x00,0x3E,0x07,0xFF,0xFF,0xFF,0xFF,0x80,  0x00,0x1F,0x03,0xF0,0x1C,0x00,0x00,0x00,  0x00,0x0F,0x80,0x00,0x1C,0x00,0x00,0x00,  0x00,0x0F,0x80,0x00,0x3C,0x00,0x00,0x00,  
0x00,0x07,0x80,0x00,0x3C,0x00,0x00,0x00,  0x00,0x07,0x80,0x00,0x3C,0x00,0x00,0x00,  0x00,0x03,0x00,0x00,0x38,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x38,0x00,0x00,0x00,  
0x00,0x00,0x00,0x00,0x78,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x78,0x00,0xC0,0x00,  0x00,0x00,0x00,0x00,0x78,0x01,0xE0,0x00,  0x00,0x00,0x01,0xFF,0xFF,0xFF,0xF0,0x00,  
0x00,0x00,0x00,0xE0,0x70,0x01,0xF0,0x00,  0x00,0x00,0x00,0x00,0x70,0x01,0xE0,0x00,  0x00,0x04,0x00,0x00,0xF0,0x01,0xC0,0x00,  0x00,0x06,0x00,0x00,0xF0,0x01,0xC0,0x00,  
0x00,0x0F,0x00,0x00,0xF0,0x01,0xC0,0x00,  0x00,0x1F,0x80,0x00,0xE0,0x01,0xC0,0x00,  0x0F,0xFF,0x80,0x00,0xE0,0x01,0xC0,0x00,  0x00,0x0E,0x00,0x00,0xE0,0x01,0xC0,0x00,  
0x00,0x0E,0x00,0x01,0xE0,0x01,0xC0,0x20,  0x00,0x0E,0x00,0x01,0xE0,0x01,0xC0,0x20,  0x00,0x0E,0x00,0x01,0xE0,0x01,0xC0,0x70,  0x00,0x0E,0x00,0x01,0xC0,0x01,0xC0,0xF8,  
0x00,0x0E,0x3F,0xFF,0xFF,0xFF,0xFF,0xFC,  0x00,0x0E,0x18,0x00,0x00,0x00,0x00,0x00,  0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,  
0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x0E,0x00,0x08,0x00,0x00,0x18,0x00,  0x00,0x0E,0x00,0x0E,0x00,0x00,0x3C,0x00,  
0x00,0x0E,0x00,0x0F,0xFF,0xFF,0xFE,0x00,  0x00,0x0E,0x00,0x0E,0x00,0x00,0x3E,0x00,  0x00,0x0E,0x00,0x8E,0x00,0x00,0x3C,0x00,  0x00,0x0E,0x01,0x8E,0x00,0x00,0x3C,0x00,  
0x00,0x0E,0x03,0x0E,0x00,0x00,0x3C,0x00,  0x00,0x0E,0x06,0x0E,0x00,0x00,0x3C,0x00,  0x00,0x0E,0x0C,0x0E,0x00,0x00,0x3C,0x00,  0x00,0x0E,0x18,0x0E,0x00,0x00,0x3C,0x00,  
0x00,0x0E,0x78,0x0E,0x00,0x00,0x3C,0x00,  0x00,0x0E,0xF0,0x0E,0x00,0x00,0x3C,0x00,  0x00,0x0F,0xE0,0x0E,0x00,0x00,0x3C,0x00,  0x00,0x0F,0xC0,0x0E,0x00,0x00,0x3C,0x00,  
0x00,0x1F,0x80,0x0E,0x00,0x00,0x3C,0x00,  0x00,0x0F,0x00,0x0E,0x00,0x00,0x3C,0x00,  0x00,0x0F,0x00,0x0E,0x00,0x00,0x3C,0x00,  0x00,0x06,0x00,0x0F,0xFF,0xFF,0xFC,0x00,  
0x00,0x00,0x00,0x0E,0x00,0x00,0x3C,0x00,  0x00,0x00,0x00,0x0E,0x00,0x00,0x3C,0x00,  0x00,0x00,0x00,0x0E,0x00,0x00,0x3C,0x00,  0x00,0x00,0x00,0x0C,0x00,0x00,0x30,0x00,  
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
/* @2 言(64x64,H)@ [suki software]*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x0E,0x00,0x00,0x00,0x00,  
0x00,0x00,0x00,0x07,0x80,0x00,0x00,0x00,  0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,  0x00,0x00,0x00,0x03,0xE0,0x00,0x00,0x00,  0x00,0x00,0x00,0x01,0xF0,0x00,0x00,0x00,  
0x00,0x00,0x00,0x01,0xF0,0x00,0x00,0x00,  0x00,0x00,0x00,0x01,0xF0,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0xE0,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0xE0,0x00,0x00,0xC0,  
0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xE0,  0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xF0,  0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,  0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,  
0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,  0x00,0x00,0x00,0x00,0x00,0x07,0xC0,0x00,  0x00,0x00,0x00,0x00,0x00,0x0F,0xE0,0x00,  0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,  
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,  0x00,0x00,0x00,0x00,0x00,0x01,0xC0,0x00,  0x00,0x00,0x00,0x00,0x00,0x03,0xE0,0x00,  
0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,  0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,  
0x00,0x00,0x60,0x00,0x00,0x03,0xC0,0x00,  0x00,0x00,0x78,0x00,0x00,0x03,0xE0,0x00,  0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xF0,0x00,  0x00,0x00,0x78,0x00,0x00,0x03,0xE0,0x00,  
0x00,0x00,0x78,0x00,0x00,0x03,0xC0,0x00,  0x00,0x00,0x78,0x00,0x00,0x03,0xC0,0x00,  0x00,0x00,0x78,0x00,0x00,0x03,0xC0,0x00,  0x00,0x00,0x78,0x00,0x00,0x03,0xC0,0x00,  
0x00,0x00,0x78,0x00,0x00,0x03,0xC0,0x00,  0x00,0x00,0x78,0x00,0x00,0x03,0xC0,0x00,  0x00,0x00,0x78,0x00,0x00,0x03,0xC0,0x00,  0x00,0x00,0x78,0x00,0x00,0x03,0xC0,0x00,  
0x00,0x00,0x78,0x00,0x00,0x03,0xC0,0x00,  0x00,0x00,0x78,0x00,0x00,0x03,0xC0,0x00,  0x00,0x00,0x78,0x00,0x00,0x03,0xC0,0x00,  0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xC0,0x00,  
0x00,0x00,0x78,0x00,0x00,0x03,0xC0,0x00,  0x00,0x00,0x78,0x00,0x00,0x03,0xC0,0x00,  0x00,0x00,0x78,0x00,0x00,0x03,0xC0,0x00,  0x00,0x00,0x78,0x00,0x00,0x03,0xC0,0x00,  
0x00,0x00,0x78,0x00,0x00,0x03,0x80,0x00,  0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 



};
char bz[]={/* @0 工(24x24,H)@ [suki software]*/
0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x40,  0x1F,0xFF,0xE0,  
0x00,0x30,0x00,  0x00,0x30,0x00,  0x00,0x30,0x00,  0x00,0x30,0x00,  
0x00,0x30,0x00,  0x00,0x30,0x00,  0x00,0x30,0x00,  0x00,0x30,0x00,  
0x00,0x30,0x00,  0x00,0x30,0x00,  0x00,0x30,0x00,  0x00,0x30,0x00,  
0x1F,0xFF,0xE0,  0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  
0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  
/* @1 程(24x24,H)@ [suki software]*/
0x00,0x00,0x00,  0x00,0x80,0x00,  0x07,0x1F,0xE0,  0x1A,0x10,0x60,  
0x02,0x10,0x60,  0x02,0x10,0x60,  0x02,0x90,0x60,  0x1F,0x1F,0xE0,  
0x06,0x00,0x00,  0x07,0x00,0x20,  0x0A,0x9F,0xC0,  0x0A,0x82,0x00,  
0x12,0x02,0x00,  0x12,0x3F,0xE0,  0x22,0x02,0x00,  0x02,0x02,0x00,  
0x02,0x02,0x00,  0x02,0xFF,0xF0,  0x02,0x00,0x00,  0x00,0x00,0x00,  
0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  
/* @2 学(24x24,H)@ [suki software]*/
0x00,0x00,0x00,  0x04,0x41,0x00,  0x02,0x21,0x00,  0x03,0x32,0x00,  
0x01,0x32,0x00,  0x02,0x04,0x20,  0x0D,0xFB,0xF0,  0x18,0x00,0x40,  
0x30,0x02,0x80,  0x03,0xFF,0x00,  0x00,0x08,0x00,  0x00,0x30,0x00,  
0x00,0x20,0x60,  0x1F,0xFF,0x80,  0x00,0x20,0x00,  0x00,0x20,0x00,  
0x00,0x20,0x00,  0x00,0xE0,0x00,  0x00,0x60,0x00,  0x00,0x00,0x00,  
0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  
/* @3 院(24x24,H)@ [suki software]*/
0x00,0x00,0x00,  0x00,0x04,0x00,  0x1F,0x86,0x00,  0x11,0x04,0x00,  
0x12,0x7B,0xE0,  0x12,0x40,0x40,  0x12,0x80,0x00,  0x14,0x3F,0x80,  
0x12,0x00,0x00,  0x11,0x00,0x00,  0x11,0xFF,0xE0,  0x10,0x8A,0x00,  
0x11,0x8A,0x00,  0x13,0x0A,0x00,  0x10,0x12,0x00,  0x10,0x12,0x20,  
0x10,0x22,0x20,  0x10,0x41,0xF0,  0x11,0x80,0x00,  0x00,0x00,0x00,  
0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  
/* @4 软(24x24,H)@ [suki software]*/
0x00,0x00,0x00,  0x02,0x04,0x00,  0x02,0x04,0x00,  0x04,0x04,0x00,  
0x3F,0xE8,0x20,  0x04,0x0F,0xF0,  0x0D,0x14,0x60,  0x09,0x16,0x40,  
0x09,0x26,0x00,  0x19,0x26,0x00,  0x07,0xC6,0x00,  0x01,0x06,0x00,  
0x01,0x45,0x00,  0x07,0x8D,0x00,  0x19,0x08,0x80,  0x01,0x10,0x80,  
0x01,0x20,0x60,  0x01,0x40,0x70,  0x01,0x80,0x00,  0x00,0x00,0x00,  
0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  
/* @5 件(24x24,H)@ [suki software]*/
0x00,0x00,0x00,  0x01,0x06,0x00,  0x03,0x06,0x00,  0x02,0x26,0x00,  
0x06,0x26,0x00,  0x04,0x46,0x20,  0x0E,0x7F,0xC0,  0x0C,0x46,0x00,  
0x14,0x86,0x00,  0x24,0x06,0x00,  0x24,0x06,0x20,  0x04,0xFF,0xC0,  
0x04,0x06,0x00,  0x04,0x06,0x00,  0x04,0x06,0x00,  0x04,0x06,0x00,  
0x04,0x06,0x00,  0x04,0x06,0x00,  0x04,0x06,0x00,  0x00,0x00,0x00,  
0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  
/* @6 科(24x24,H)@ [suki software]*/
0x00,0x00,0x00,  0x00,0x80,0xC0,  0x07,0x80,0x80,  0x1A,0x00,0x80,  
0x02,0x0C,0x80,  0x02,0x04,0x80,  0x02,0x40,0x80,  0x1F,0x80,0x80,  
0x02,0x10,0x80,  0x07,0x08,0x80,  0x06,0xC8,0x80,  0x0A,0x40,0xB0,  
0x0A,0x07,0xC0,  0x12,0x78,0x80,  0x22,0x00,0x80,  0x02,0x00,0x80,  
0x02,0x00,0x80,  0x02,0x00,0x80,  0x02,0x00,0x80,  0x00,0x00,0x00,  
0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  
/* @7 技(24x24,H)@ [suki software]*/
0x00,0x00,0x00,  0x06,0x02,0x00,  0x02,0x02,0x00,  0x02,0x02,0x00,  
0x02,0x02,0x20,  0x3F,0xFF,0xF0,  0x02,0x02,0x00,  0x02,0x02,0x00,  
0x02,0x42,0x40,  0x03,0xBD,0xC0,  0x1E,0x10,0xC0,  0x32,0x10,0x80,  
0x02,0x09,0x80,  0x02,0x09,0x00,  0x02,0x06,0x00,  0x02,0x06,0x00,  
0x02,0x09,0x80,  0x0E,0x30,0xF0,  0x04,0xC0,0x20,  0x00,0x00,0x00,  
0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  
/* @8 节(24x24,H)@ [suki software]*/
0x00,0x00,0x00,  0x00,0x86,0x00,  0x00,0x86,0x00,  0x00,0x86,0x20,  
0x3F,0xFF,0xC0,  0x00,0x86,0x00,  0x00,0x84,0x00,  0x00,0x00,0x80,  
0x0F,0xFF,0x80,  0x00,0x60,0x80,  0x00,0x60,0x80,  0x00,0x60,0x80,  
0x00,0x60,0x80,  0x00,0x60,0x80,  0x00,0x67,0x80,  0x00,0x61,0x00,  
0x00,0x60,0x00,  0x00,0x60,0x00,  0x00,0x40,0x00,  0x00,0x00,0x00,  
0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  
/* @9 指(24x24,H)@ [suki software]*/
0x00,0x00,0x00,  0x06,0x10,0x00,  0x02,0x10,0x40,  0x02,0x11,0xC0,  
0x02,0x16,0x00,  0x3F,0xD8,0x00,  0x02,0x10,0x20,  0x02,0x10,0x30,  
0x02,0x9F,0xE0,  0x03,0x00,0x00,  0x0E,0x30,0x40,  0x3A,0x1F,0xC0,  
0x02,0x10,0x40,  0x02,0x10,0x40,  0x02,0x1F,0xC0,  0x02,0x10,0x40,  
0x02,0x10,0x40,  0x1C,0x3F,0xC0,  0x04,0x30,0x00,  0x00,0x00,0x00,  
0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  
/* @10 导(24x24,H)@ [suki software]*/
0x00,0x00,0x00,  0x04,0x01,0x00,  0x07,0xFF,0x00,  0x04,0x01,0x00,  
0x04,0x01,0x00,  0x07,0xFF,0x00,  0x04,0x00,0x40,  0x04,0x00,0x40,  
0x03,0xFF,0xE0,  0x00,0x04,0x00,  0x00,0x06,0x00,  0x00,0x06,0x20,  
0x1F,0xFB,0xC0,  0x01,0x02,0x00,  0x01,0x82,0x00,  0x00,0x82,0x00,  
0x00,0x06,0x00,  0x00,0x1C,0x00,  0x00,0x0C,0x00,  0x00,0x00,0x00,  
0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  
/* @11 老(24x24,H)@ [suki software]*/
0x00,0x00,0x00,  0x00,0x30,0x00,  0x00,0x20,0x80,  0x00,0x22,0xC0,  
0x07,0xFF,0x80,  0x00,0x21,0x00,  0x00,0x22,0x00,  0x00,0x26,0x30,  
0x1F,0xDD,0xC0,  0x00,0x18,0x00,  0x00,0x30,0x00,  0x00,0xE3,0x00,  
0x00,0x86,0x00,  0x03,0x98,0x40,  0x04,0xE0,0x40,  0x18,0x80,0x40,  
0x00,0x80,0x40,  0x00,0xFF,0xE0,  0x00,0x00,0x00,  0x00,0x00,0x00,  
0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  
/* @12 师(24x24,H)@ [suki software]*/
0x00,0x00,0x00,  0x03,0x00,0x20,  0x02,0xFF,0xF0,  0x02,0x04,0x00,  
0x1A,0x04,0x00,  0x12,0x04,0x00,  0x12,0x7F,0xE0,  0x12,0x44,0x20,  
0x12,0x44,0x20,  0x12,0x44,0x20,  0x12,0x44,0x20,  0x12,0x44,0x20,  
0x12,0x44,0x20,  0x12,0x44,0x20,  0x04,0x44,0xE0,  0x04,0x44,0x40,  
0x08,0x04,0x00,  0x10,0x04,0x00,  0x20,0x06,0x00,  0x00,0x00,0x00,  
0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  
/* @13 ：(24x24,H)@ [suki software]*/
0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  
0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  
0x00,0x00,0x00,  0x00,0x00,0x00,  0x0C,0x00,0x00,  0x0C,0x00,0x00,  
0x0C,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  0x0C,0x00,0x00,  
0x0C,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  
0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  
/* @14 廖(24x24,H)@ [suki software]*/
0x00,0x00,0x00,  0x00,0x10,0x00,  0x00,0x10,0x20,  0x0F,0xEF,0xC0,  
0x08,0x00,0x40,  0x09,0xFF,0xC0,  0x09,0xA3,0x40,  0x08,0xA2,0x40,  
0x08,0xE3,0xC0,  0x0B,0x1C,0x40,  0x08,0x28,0x00,  0x08,0x46,0x00,  
0x08,0x99,0x80,  0x0B,0x20,0x60,  0x14,0xCE,0x00,  0x10,0x31,0x80,  
0x20,0xC7,0x00,  0x20,0x38,0x00,  0x03,0xC0,0x00,  0x00,0x00,0x00,  
0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  
/* @15 华(24x24,H)@ [suki software]*/
0x00,0x00,0x00,  0x01,0x18,0x00,  0x03,0x10,0x40,  0x02,0x10,0xE0,  
0x06,0x11,0x80,  0x0A,0x16,0x00,  0x12,0x18,0x00,  0x22,0x30,0x20,  
0x02,0xD0,0x20,  0x03,0x18,0x30,  0x02,0x2F,0xE0,  0x00,0x30,0x00,  
0x00,0x20,0x00,  0x3F,0xFF,0xF0,  0x00,0x20,0x00,  0x00,0x20,0x00,  
0x00,0x20,0x00,  0x00,0x20,0x00,  0x00,0x20,0x00,  0x00,0x00,0x00,  
0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  
/* @16 雄(24x24,H)@ [suki software]*/
0x00,0x00,0x00,  0x06,0x0A,0x00,  0x04,0x0D,0x00,  0x04,0x19,0x00,  
0x04,0x10,0x20,  0x3F,0xDF,0xC0,  0x04,0x31,0x00,  0x09,0x31,0x00,  
0x09,0x5F,0xE0,  0x0A,0x51,0x00,  0x0A,0x91,0x00,  0x12,0x11,0x00,  
0x14,0x1F,0xE0,  0x14,0x91,0x00,  0x28,0x51,0x00,  0x2F,0xD1,0x00,  
0x00,0x11,0x30,  0x00,0x1E,0xC0,  0x00,0x10,0x00,  0x00,0x00,0x00,  
0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  0x00,0x00,0x00,  
};
char namec[]={
/* @0 小(16x16,H)@ [suki software]*/
0x00,0x00,  0x01,0x00,  0x01,0x00,  0x01,0x00,  
0x01,0x00,  0x11,0x20,  0x11,0x18,  0x21,0x08,  
0x21,0x00,  0x01,0x00,  0x05,0x00,  0x02,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @1 组(16x16,H)@ [suki software]*/
0x00,0x00,  0x08,0x00,  0x08,0x70,  0x10,0x90,  
0x24,0x90,  0x38,0xD0,  0x10,0x90,  0x24,0x90,  
0x38,0xD0,  0x04,0x90,  0x18,0xFC,  0x67,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @2 成(16x16,H)@ [suki software]*/
0x02,0x00,  0x02,0x20,  0x02,0x10,  0x01,0xC0,  
0x1F,0x00,  0x11,0x20,  0x10,0xA0,  0x1E,0xA0,  
0x12,0x40,  0x12,0xC0,  0x2D,0x24,  0x40,0x14,  
0x00,0x0C,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @3 员(16x16,H)@ [suki software]*/
0x03,0xC0,  0x0C,0x40,  0x09,0xC0,  0x06,0x00,  
0x01,0xC0,  0x0E,0x40,  0x09,0x40,  0x09,0x40,  
0x09,0x40,  0x0A,0x40,  0x02,0x80,  0x04,0x60,  
0x18,0x20,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @4 :(16x16,H)@ [suki software]*/
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x30,0x00,  0x20,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x30,0x00,  0x20,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @5 邱(16x16,H)@ [suki software]*/
0x01,0x00,  0x06,0x18,  0x18,0x68,  0x13,0x48,  
0x1E,0x50,  0x12,0x50,  0x13,0x48,  0x1C,0x44,  
0x70,0x5C,  0x00,0x40,  0x00,0x40,  0x00,0x40,  
0x00,0x40,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @6 健(16x16,H)@ [suki software]*/
0x00,0x40,  0x08,0x40,  0x08,0x70,  0x11,0xD0,  
0x3E,0x78,  0x53,0xD0,  0x14,0x60,  0x16,0xC0,  
0x12,0x70,  0x1E,0xC0,  0x15,0x40,  0x18,0xFC,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @7 .(16x16,H)@ [suki software]*/
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x30,0x00,  0x20,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @8 姜(16x16,H)@ [suki software]*/
0x04,0x20,  0x02,0x40,  0x01,0xE0,  0x07,0x00,  
0x01,0xC0,  0x07,0x00,  0x01,0xE0,  0x0E,0x00,  
0x03,0xFC,  0x7E,0x40,  0x02,0x80,  0x03,0xC0,  
0x0C,0x30,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @9 杰(16x16,H)@ [suki software]*/
0x01,0x00,  0x01,0x00,  0x01,0x00,  0x01,0xE0,  
0x1F,0x00,  0x03,0x80,  0x05,0x60,  0x09,0x1C,  
0x11,0x00,  0x00,0x00,  0x24,0x90,  0x22,0x48,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @10 .(16x16,H)@ [suki software]*/
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x30,0x00,  0x20,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @11 郑(16x16,H)@ [suki software]*/
0x00,0x00,  0x11,0x00,  0x0A,0x18,  0x07,0x68,  
0x1C,0x50,  0x07,0x50,  0x3C,0x50,  0x04,0x48,  
0x0A,0x58,  0x11,0x48,  0x20,0x40,  0x00,0x40,  
0x00,0x40,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @12 紫(16x16,H)@ [suki software]*/
0x04,0x40,  0x04,0x50,  0x17,0x60,  0x14,0x48,  
0x1F,0x38,  0x32,0x80,  0x07,0x80,  0x02,0x40,  
0x04,0xE0,  0x07,0x00,  0x09,0x40,  0x09,0x20,  
0x13,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @13 渊(16x16,H)@ [suki software]*/
0x00,0x00,  0x20,0x90,  0x14,0xB0,  0x06,0xD0,  
0x25,0x90,  0x14,0xF0,  0x07,0x90,  0x15,0xD0,  
0x16,0xB0,  0x24,0x90,  0x28,0x90,  0x10,0x10,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @14 .(16x16,H)@ [suki software]*/
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x30,0x00,  0x20,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @15 吴(16x16,H)@ [suki software]*/
0x00,0x00,  0x03,0xC0,  0x0C,0x40,  0x09,0xC0,  
0x06,0x00,  0x01,0xC0,  0x0E,0x00,  0x03,0xF0,  
0x3E,0x00,  0x05,0x00,  0x08,0x80,  0x10,0x60,  
0x20,0x38,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @16 铭(16x16,H)@ [suki software]*/
0x00,0x20,  0x08,0x20,  0x08,0x40,  0x0E,0x78,  
0x11,0x90,  0x2E,0xA0,  0x78,0x60,  0x0E,0x98,  
0x39,0xE8,  0x0A,0x88,  0x0C,0xB8,  0x08,0xC0,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @17 杰(16x16,H)@ [suki software]*/
0x01,0x00,  0x01,0x00,  0x01,0x00,  0x01,0xE0,  
0x1F,0x00,  0x03,0x80,  0x05,0x60,  0x09,0x1C,  
0x11,0x00,  0x00,0x00,  0x24,0x90,  0x22,0x48,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
};
char classbj[]={
/* @0 制(16x16,H)@ [suki software]*/
0x04,0x08,  0x04,0x08,  0x14,0x08,  0x17,0x28,  
0x1C,0x28,  0x27,0xA8,  0x7C,0x28,  0x07,0x28,  
0x1D,0x28,  0x15,0x08,  0x17,0x08,  0x04,0x18,  
0x04,0x08,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @1 作(16x16,H)@ [suki software]*/
0x00,0x40,  0x04,0x40,  0x04,0x80,  0x08,0xBC,  
0x19,0xC0,  0x2A,0x40,  0x4C,0x78,  0x08,0x40,  
0x08,0x78,  0x08,0x40,  0x08,0x40,  0x08,0x40,  
0x00,0x40,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @2 班(16x16,H)@ [suki software]*/
0x01,0x00,  0x01,0x00,  0x0D,0x38,  0x31,0x60,  
0x11,0x20,  0x15,0x38,  0x3D,0x60,  0x15,0x20,  
0x11,0x20,  0x19,0x3C,  0x62,0xC0,  0x04,0x00,  
0x08,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @3 级(16x16,H)@ [suki software]*/
0x00,0x00,  0x08,0x70,  0x09,0x90,  0x10,0xA0,  
0x24,0xA0,  0x38,0xB0,  0x14,0x90,  0x39,0x50,  
0x05,0x20,  0x1A,0x50,  0x65,0x8C,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @4 ：(16x16,H)@ [suki software]*/
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x30,0x00,  
0x30,0x00,  0x00,0x00,  0x30,0x00,  0x30,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @5 0(16x16,H)@ [suki software]*/
0x00,0x00,  0x00,0x00,  0x1C,0x00,  0x12,0x00,  
0x12,0x00,  0x12,0x00,  0x12,0x00,  0x12,0x00,  
0x12,0x00,  0x12,0x00,  0x14,0x00,  0x08,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @6 9(16x16,H)@ [suki software]*/
0x00,0x00,  0x00,0x00,  0x1C,0x00,  0x24,0x00,  
0x22,0x00,  0x22,0x00,  0x22,0x00,  0x1E,0x00,  
0x04,0x00,  0x04,0x00,  0x08,0x00,  0x30,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @7 可(16x16,H)@ [suki software]*/
0x00,0x00,  0x00,0x00,  0x01,0xFC,  0x7E,0x40,  
0x00,0x40,  0x0E,0x40,  0x0A,0x40,  0x0E,0x40,  
0x00,0x40,  0x00,0x40,  0x00,0x40,  0x00,0xC0,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @8 视(16x16,H)@ [suki software]*/
0x10,0x00,  0x08,0x70,  0x01,0x90,  0x01,0x50,  
0x19,0x50,  0x69,0x50,  0x19,0x50,  0x34,0x90,  
0x50,0xC0,  0x11,0x44,  0x16,0x44,  0x10,0x3C,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @9 化(16x16,H)@ [suki software]*/
0x00,0x00,  0x09,0x00,  0x09,0x00,  0x09,0x10,  
0x11,0x20,  0x11,0x40,  0x31,0x80,  0x57,0x00,  
0x11,0x04,  0x11,0x04,  0x10,0x84,  0x10,0x78,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @10 （(16x16,H)@ [suki software]*/
0x00,0x00,  0x00,0x10,  0x00,0x20,  0x00,0x20,  
0x00,0x40,  0x00,0x40,  0x00,0x40,  0x00,0x40,  
0x00,0x40,  0x00,0x20,  0x00,0x20,  0x00,0x10,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @11 3(16x16,H)@ [suki software]*/
0x00,0x00,  0x00,0x00,  0x0C,0x00,  0x12,0x00,  
0x02,0x00,  0x04,0x00,  0x0C,0x00,  0x02,0x00,  
0x02,0x00,  0x02,0x00,  0x14,0x00,  0x18,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @12 ）(16x16,H)@ [suki software]*/
0x00,0x00,  0x10,0x00,  0x08,0x00,  0x08,0x00,  
0x04,0x00,  0x04,0x00,  0x04,0x00,  0x04,0x00,  
0x04,0x00,  0x08,0x00,  0x08,0x00,  0x10,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @13 班(16x16,H)@ [suki software]*/
0x01,0x00,  0x01,0x00,  0x0D,0x38,  0x31,0x60,  
0x11,0x20,  0x15,0x38,  0x3D,0x60,  0x15,0x20,  
0x11,0x20,  0x19,0x3C,  0x62,0xC0,  0x04,0x00,  
0x08,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
};
char tuxing[]={
/* @0 /(16x16,H)@ [suki software]*/
0x00,0x00,  0x00,0x00,  0x01,0x80,  0x03,0x00,  
0x03,0x00,  0x06,0x00,  0x06,0x00,  0x0C,0x00,  
0x0C,0x00,  0x18,0x00,  0x18,0x00,  0x30,0x00,  
0x30,0x00,  0x60,0x00,  0x60,0x00,  0x00,0x00,  
/* @2  (16x16,H)@ [suki software]*/
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @3  (16x16,H)@ [suki software]*/
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @4 ╱(16x16,H)@ [suki software]*/
0x00,0x01,  0x00,0x03,  0x00,0x06,  0x00,0x0C,  
0x00,0x18,  0x00,0x30,  0x00,0x60,  0x00,0xC0,  
0x01,0x80,  0x03,0x00,  0x06,0x00,  0x0C,0x00,  
0x18,0x00,  0x30,0x00,  0x60,0x00,  0xC0,0x00,  
/* @0 ┎(16x16,H)@ [suki software]*/
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x01,0xFF,  
0x01,0xC0,  0x01,0xC0,  0x01,0xC0,  0x01,0xC0,  
0x01,0xC0,  0x01,0xC0,  0x01,0xC0,  0x01,0xC0,  
/* @1 ╮(16x16,H)@ [suki software]*/
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0xF8,0x00,  
0x0E,0x00,  0x03,0x00,  0x03,0x00,  0x01,0x80,  
0x01,0x80,  0x01,0x80,  0x01,0x80,  0x01,0x80,  
/* @2 /(16x16,H)@ [suki software]*/
0x00,0x00,  0x00,0x00,  0x01,0x80,  0x03,0x00,  
0x03,0x00,  0x06,0x00,  0x06,0x00,  0x0C,0x00,  
0x0C,0x00,  0x18,0x00,  0x18,0x00,  0x30,0x00,  
0x30,0x00,  0x60,0x00,  0x60,0x00,  0x00,0x00,  
/* @3  (16x16,H)@ [suki software]*/
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @4 ╱(16x16,H)@ [suki software]*/
0x00,0x01,  0x00,0x03,  0x00,0x06,  0x00,0x0C,  
0x00,0x18,  0x00,0x30,  0x00,0x60,  0x00,0xC0,  
0x01,0x80,  0x03,0x00,  0x06,0x00,  0x0C,0x00,  
0x18,0x00,  0x30,0x00,  0x60,0x00,  0xC0,0x00, 
/* @0 ╰(16x16,H)@ [suki software]*/
0x01,0x80,  0x01,0x80,  0x01,0x80,  0x01,0x80,  
0x00,0xC0,  0x00,0xC0,  0x00,0x70,  0x00,0x1F,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @1 ★(16x16,H)@ [suki software]*/
0x01,0x80,  0x01,0x80,  0x03,0xC0,  0x03,0xC0,  
0x03,0xC0,  0x07,0xE0,  0xFF,0xFF,  0x3F,0xFC,  
0x1F,0xF8,  0x0F,0xF0,  0x0F,0xF0,  0x1F,0xF8,  
0x1F,0xF8,  0x1C,0x38,  0x30,0x0C,  0x00,0x00,  
/* @2 ╮(16x16,H)@ [suki software]*/
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0xF8,0x00,  
0x0E,0x00,  0x03,0x00,  0x03,0x00,  0x01,0x80,  
0x01,0x80,  0x01,0x80,  0x01,0x80,  0x01,0x80,  
/* @0 ╰(16x16,H)@ [suki software]*/
0x01,0x80,  0x01,0x80,  0x01,0x80,  0x01,0x80,  
0x00,0xC0,  0x00,0xC0,  0x00,0x70,  0x00,0x1F,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @1 ┚(16x16,H)@ [suki software]*/
0x01,0xC0,  0x01,0xC0,  0x01,0xC0,  0x01,0xC0,  
0x01,0xC0,  0x01,0xC0,  0x01,0xC0,  0xFF,0xC0,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @0 ╱(16x16,H)@ [suki software]*/
0x00,0x01,  0x00,0x03,  0x00,0x06,  0x00,0x0C,  
0x00,0x18,  0x00,0x30,  0x00,0x60,  0x00,0xC0,  
0x01,0x80,  0x03,0x00,  0x06,0x00,  0x0C,0x00,  
0x18,0x00,  0x30,0x00,  0x60,0x00,  0xC0,0x00,  
/* @1  (16x16,H)@ [suki software]*/
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @2 /(16x16,H)@ [suki software]*/
0x00,0x00,  0x00,0x00,  0x01,0x80,  0x03,0x00,  
0x03,0x00,  0x06,0x00,  0x06,0x00,  0x0C,0x00,  
0x0C,0x00,  0x18,0x00,  0x18,0x00,  0x30,0x00,  
0x30,0x00,  0x60,0x00,  0x60,0x00,  0x00,0x00,  
/* @0 ╱(16x16,H)@ [suki software]*/
0x00,0x01,  0x00,0x03,  0x00,0x06,  0x00,0x0C,  
0x00,0x18,  0x00,0x30,  0x00,0x60,  0x00,0xC0,  
0x01,0x80,  0x03,0x00,  0x06,0x00,  0x0C,0x00,  
0x18,0x00,  0x30,0x00,  0x60,0x00,  0xC0,0x00,  
/* @1  (16x16,H)@ [suki software]*/
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @2  (16x16,H)@ [suki software]*/
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
0x00,0x00,  0x00,0x00,  0x00,0x00,  0x00,0x00,  
/* @4 /(16x16,H)@ [suki software]*/
0x00,0x00,  0x00,0x00,  0x01,0x80,  0x03,0x00,  
0x03,0x00,  0x06,0x00,  0x06,0x00,  0x0C,0x00,  
0x0C,0x00,  0x18,0x00,  0x18,0x00,  0x30,0x00,  
0x30,0x00,  0x60,0x00,  0x60,0x00,  0x00,0x00,  
};
/*****************欢迎窗口***********************/
void main()
{
	 int drive,mode,i;
	  drive=DETECT;
registerbgidriver(EGAVGA_driver);
registerbgifont(triplex_font);
registerbgifont(small_font);
registerbgifont(sansserif_font);
registerbgifont(gothic_font);
 initgraph(&drive,&mode,"");
 setbkcolor(3);
 cleardevice();
 setfillstyle(10,5);
 bar(1,1,637,474);
 settextstyle(4,0,8);
 moveto(10,60);
 outtext("book");
 for(i = 0; i < 2; i++)
    drawmat(bookm + i * 128, 32, 55 + i * 32, 148, WHITE);
 for(i = 2; i < 6; i++)
    drawmat(bookm + i * 128, 32, 70 + i * 32, 260, WHITE);
 moveto(60,170);
 outtext("manager");
 setfillstyle(1,9);
 bar(350,50,550,450);
 rectangle(350,50,550,450);
 line(350,158,550,158);
for(i = 1; i < 3; i++)
	drawmat(cyy + i * 512, 64, 350 + i * 60, 70, WHITE);
	drawmat(cyy , 64, 370, 70, WHITE);
for(i = 0; i < 6; i++)
	drawmat(bz + i * 72, 24, 350 + i * 20, 170, WHITE);
for(i = 2; i < 4; i++)
	drawmat(bz + i * 72, 24, 470 + (i-2) * 20, 170, WHITE);
for(i = 9; i < 17; i++)
	drawmat(bz + i * 72, 24, 370 + (i-9) * 20, 194, WHITE);
for(i = 0; i < 18; i++)
	drawmat(namec + i * 32, 16,10+i *13, 435, WHITE);
for(i = 0; i < 14; i++)
	drawmat(classbj + i * 32, 16,10+i *13, 415, WHITE);
for(i = 0; i < 4; i++)
	drawmat(tuxing + i * 32, 16,460+i *12, 300, 10);
for(i = 4; i < 9; i++)
	drawmat(tuxing + i * 32, 16,425+(i-4) *14, 314, 10);
for(i = 9; i < 12; i++)
	drawmat(tuxing + i * 32, 16,425+(i-9) *14, 328, 10);
for(i = 12; i < 14; i++)
	drawmat(tuxing + i * 32, 16,425+(i-11) *14, 342,10);
for(i = 14; i < 17; i++)
	drawmat(tuxing + i * 32, 16,310+(i-7) *14, 359, 10);
for(i = 17; i < 21; i++)
	drawmat(tuxing + i * 32, 16,395+(i-17) *11, 373, 10);
getch();
 closegraph();
 clrscr();
}