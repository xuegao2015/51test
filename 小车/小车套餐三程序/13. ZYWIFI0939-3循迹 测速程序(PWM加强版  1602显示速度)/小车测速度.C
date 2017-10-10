 /**********************HL-1& ZY-2实验开发板例程************************
*  平台：HL-1& ZY-2 V8.0 + Keil U3 + STC89C52
*  名称：ZY-1智能小车参考程序
*  公司：湖南智宇科教设备有限公司
*  淘宝：https://hnzyrobot1688.taobao.com/     
*  网站：www.hnzhiyu.cn	
*  编写：智宇公司研发一部
*  日期：2015-1-15
*  交流:智能车QQ:261339276
*  晶振:11.0592MHZ
*  晶振:11.0592MHZ
*  说明：免费开源，不提供源代码分析
*  论坛：
*  硬件设置：要有自己动手能力，才能完成实验
*  使用说明：根据下面IO口自己用杜邦线连接各种模块，可以自己修改各种模块IO口

   重点提示：本程序只做参考，不提供技术支持，请自己研究吸收。

/******************************************************************/
	#include<AT89x51.H>
	#include <intrins.h>
    #include "LCD1602display.h"
	#include<HJ-4WD_PWM.H>		  //包含HL-1蓝牙智能小车驱动IO口定义等函数
/*   右电机测速线接 P3.2    左电机测速线接 P3.3*/
    #define Left_1_led        P3_7	 // 左循迹传感器       由于测速程序  必须使用外部中断  所以之前的循迹传感器IO由P3.3改为P3.7 
	#define Right_1_led       P3_6	 //右循迹传感器    		由于测速程序  必须使用外部中断  所以之前的循迹传感器IO由P3.2改为P3.6 
   	 

    unsigned char disbuff[6]={0};//用于分别存放速度的值的值	
	unsigned char  time=0;											  //显示缓存
   	unsigned char  i =0; 		 //定义扫描数码管字数
	unsigned int  count1=0;	    //计右电机码盘脉冲值
	unsigned int  count2=0;	    //计左电机码盘脉冲值
	unsigned int   V1=0;		   //定义右电机其速度
	unsigned int   V2=0;		   //定义左电机其速度

	void Delay400Ms(void);//延时400毫秒函数

unsigned char code Range[] ="==Range Finder==";//LCD1602显示格式
unsigned char code ASCII[15] ="0123456789CM S";

unsigned char code ASCIIR[13] = "RMspeed:";
unsigned char code ASCIIL[13] = "LMspeed:";
unsigned char code table[]="Distance:001.0cm";
unsigned char code table1[]="!!! Out of range";

void Count(void);//距离计算函数
void  Display_LCD();			  

unsigned long S=0;//用于存放距离的值
bit  flag =0; //量程溢出标志位
bit  turn_right_flag;
   
/************************************************************************/
///*TIMER0中断服务子函数产生2MS定时扫描数码管与产生0。5S*/
 	void timer0()interrupt 1   using 2
{
      TH0=(65536-2000)/256;		//2MS定时
      TL0=(65536-2000)%256;

	 time++;
//	 Display_SMG();
	 if(time>=250)	//250次即是，0。5S
	 {
	  time=0;
	  V1=count1*2;	 //计数公式：轮子直径*3.14/20格码盘=6.5Cm*3.14/20约=1cm  即一个脉冲走1CM距离
	                 //圆周长公式     C = 2πr	  所以将圆严格20等分  自然 一格就是约 1CM左右
	                 // 0.5S  到了   计算在0.5S时间内的	 脉冲数   因为1个脉冲基本是1CM  根据 S=VT公式  算出  V = S/T 
					 // 即  ((count1*1CM))/0.5S= (count1*2)CM/S
					 	  

	  count1=0;		 //清计数    0.5S  时间内  测一次速度     然后清0  重新 计数
						   														  																	
	  disbuff[0]=V1/100;		 //右电机速度值百位
	  disbuff[1]=V1%100/10;		//右电机速度值十位
      disbuff[2]=V1%100%10;		//右电机速度值个位

	  V2=count2*2;	 //计数公式：轮子直径*3.14/20格码盘=6.5Cm*3.14/20约=1cm  即一个脉冲走1CM距离
	                 //圆周长公式     C = 2πr	  所以将圆严格20等分  自然 一格就是约 1CM左右
	                 // 0.5S  到了   计算在0.5S时间内的	 脉冲数   因为1个脉冲基本是1CM  根据 S=VT公式  算出  V = S/T 
					 // 即  ((count1*1CM))/0.5S= (count1*2)CM/S
					 	  

	  count2=0;		 //清计数    0.5S  时间内  测一次速度     然后清0  重新 计数
						   														  																	
	  disbuff[3]=V2/100;		 //左电机速度值百位
	  disbuff[4]=V2%100/10;		 //左电机速度值十位
      disbuff[5]=V2%100%10;		//左电机速度值个位

	 }
 }
void  Display_LCD()
{
      DisplayOneChar(0, 1, ASCIIR[0]);	  //
	  DisplayOneChar(1, 1, ASCIIR[1]);	  //
	  DisplayOneChar(2, 1, ASCIIR[2]);	  //
	  DisplayOneChar(3, 1, ASCIIR[3]);	  //
	  DisplayOneChar(4, 1, ASCIIR[4]);	  //
	  DisplayOneChar(5, 1, ASCIIR[5]);	  //
	  DisplayOneChar(6, 1, ASCIIR[6]);	  //
	  DisplayOneChar(7, 1, ASCIIR[7]);	  //

	  DisplayOneChar(8, 1, ASCII[disbuff[0]]);	  //右电机速度值百位
	  DisplayOneChar(9, 1, ASCII[disbuff[1]]);	  //右电机速度值十位
	  DisplayOneChar(10, 1, ASCII[disbuff[2]]);	   //右电机速度值个位
	  DisplayOneChar(11, 1, ASCII[10]);	   //
	  DisplayOneChar(12, 1, ASCII[11]);	   //
	  DisplayOneChar(13, 1, 0x2f);	  //根据1602的字符表找到 /
	  DisplayOneChar(14, 1, ASCII[13]);	   //


	  DisplayOneChar(0, 0, ASCIIL[0]);	  //
	  DisplayOneChar(1, 0, ASCIIL[1]);	  //
	  DisplayOneChar(2, 0, ASCIIL[2]);	  //
	  DisplayOneChar(3, 0, ASCIIL[3]);	  //
	  DisplayOneChar(4, 0, ASCIIL[4]);	  //
	  DisplayOneChar(5, 0, ASCIIL[5]);	  //
	  DisplayOneChar(6, 0, ASCIIL[6]);	  //
	  DisplayOneChar(7, 0, ASCIIL[7]);	  //

	  DisplayOneChar(8, 0, ASCII[disbuff[3]]);	   //左电机速度值百位
	  DisplayOneChar(9, 0, ASCII[disbuff[4]]);	   //左电机速度值十位
	  DisplayOneChar(10, 0, ASCII[disbuff[5]]);	   //左电机速度值个位
      DisplayOneChar(11, 0, ASCII[10]);	   //
	  DisplayOneChar(12, 0, ASCII[11]);	   //
	  DisplayOneChar(13, 0, 0x2f);	  //根据1602的字符表找到 /
	  DisplayOneChar(14, 0, ASCII[13]);	
	   

}
/***************************************************/
 //外部0中断用于计算左轮的脉冲
   void intersvr1(void) interrupt 0 using 1

{
    count1++;	
}
   void intersvr2(void) interrupt 2 using 2

{
    count2++;	
}
/************************************************************************/


/*********************************************************************/		 
/*--主函数--*/
	void main(void)
{
   	//cmg88();//关数码管
	Delay1ms(400); //启动等待，等LCM讲入工作状态
	LCMInit(); //LCM初始化
	Delay1ms(5);//延时片刻

    TMOD=0X11;
    TH0=(65536-2000)/256;		//2MS定时
    TL0=(65536-2000)%256;
	TR0= 1;
	ET0= 1;

	TH1= 0XFc;		  //1ms定时
    TL1= 0X18;
    TR1= 1;
    ET1= 1;

	EX0=1;	   //开启外部中断0
    IT0=1;	   //下降沿有效
	IE0=0;

	EX1=1;	   //开启外部中断1
    IT1=1;	   //下降沿有效
	IE1=0;
	EA = 1;	 
 //   run();
	
	while(1)
	{
	   Display_LCD();	 	
			 //有信号为0  没有信号为1
	 
              if(Left_1_led==0&&Right_1_led==0)

			  run();   //调用前进函数

			  else
			 {			  
				               if(Left_1_led==1&&Right_1_led==0)	    //左边检测到黑线
			 	 {
				 	  leftrun();		  //调用小车左转  函数

			     }
			   
				 			    if(Right_1_led==1&&Left_1_led==0)		//右边检测到黑线
				  {	  
				      rightrun();		   //调用小车右转	函数

				  }
				  		    if(Right_1_led==1&&Left_1_led==1)		//悬空
				  {	  
				      stop();		   //调用小车停止	函数

				  }

			} 

					 
	 }





}
	