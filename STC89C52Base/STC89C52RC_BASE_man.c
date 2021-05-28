/**********************ZYWIFI0939C-WIFI机器人实验板例程************************
*  平台：ZYWIFI0939C-WIFI机器人 + Keil U4 + STC89C52
*  名称：ZY-1智能小车参考程序
*  公司：湖南智宇科教设备有限公司
*  淘宝：https://hnzyrobot1688.taobao.com/     
*  网站：www.hnzhiyu.cn	
*  编写：智宇公司研发一部
*  日期：2015-1-15
*  交流:智能车QQ:261339276
*  晶振:11.0592MHZ
*  说明：免费开源，不提供源代码分析
*  硬件设置：要有自己动手能力，才能完成实验
*  使用说明：根据下面IO口自己用杜邦线连接各种模块，可以自己修改各种模块IO口
*  视频教程：本小车配套学习C语言详细视频教程，资料统一网盘下载
   重点提示：本程序只做参考，不提供技术支持，请自己研究吸收。
  ZYWIFI0939WIFI控制智能机器人杜邦线接线方法，请一定照做，否则可能不工作，并烧毁小车。
   
******************************************************************/   

#include "MCU_STC89C52RC.h"
#include <intrins.h>
#include "LCD1602display.h"
#define uint    unsigned int                          //重定义无符号整数类型
#define uchar   unsigned char       

unsigned char code Range[] ="==Range Finder==";//LCD1602显示格式
unsigned char code ASCII[13] = "0123456789.-M";
unsigned char code table[]="Distance:000.0cm";
unsigned char code table1[]="!!! Out of range";

void cmg88()//关数码管，点阵函数
{
	P26=1;  
	P0=0X00;
	P26=0;
}
//=======================  1602 LED TEST
void testLed(); 
void testLed()
{


	cmg88();//关数码管
	Delay1ms(400); //启动等待，等LCM讲入工作状态
	LCMInit(); //LCM初始化
	Delay1ms(5);//延时片刻

	DisplayListChar(0, 0, Range);
	DisplayListChar(0, 1, table);

	while(1)                                              //程序主循环
	 {
	 	unsigned char i;
	    for(i=13;i>0;i--)
		{
	    	Delay1ms(1000);  
			DisplayOneChar(15, 1, ASCII[i]);      
		}                                   //延时
	  
	 }
}
//====end 1602 LED Test===========

void main(void)                                       //主程序入口
{
	  testLed();

}


