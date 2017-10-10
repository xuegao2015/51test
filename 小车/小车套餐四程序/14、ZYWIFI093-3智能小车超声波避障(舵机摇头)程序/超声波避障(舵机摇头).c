/*****************ZYDJ-CSB09A舵机控制*****************
*  平台：ZYDJ-CSB09A舵机控制 + Keil U4 + STC89C52
*  程序名称：超声波避障程序+舵机转向摇头
*  公司：湖南智宇科教设备有限公司
*  淘宝：http://tmmcu1688.taobao.com       
*  网站：www.china-zhiyu.com	
*  编写：智宇公司研发一部
*  日期：2015-1-15
*  交流:智能车QQ:1018973181 
*  晶振:11.0592MHZ
*  售后技术说明：免费开源，不支持客户自己修改程序分析与改正，可以提供我公司程序说明
*  视频教程：本小车配套学习C语言详细视频教程，资料统一网盘下载
******************************************************************/

/* 程序名：左右扫描,哪一边没有障碍往哪边走,两边有退后转大转断续走
	  angle=12  舵机 居中    angle=4  刚好右边   angle=20  刚好左边

	  调试舵机剧中的方法：插入电源，主板工作时，舵机刚好是居中的
	  ，此时，将舵机底板装入白色舵机齿轮上面，用螺丝搞紧就可以了。

/* 晶振：11.00592 MHz CPU型号：STC89C52RC */
/*****************************************************************/ 
#include<AT89x51.H>
#include <intrins.h>
#include "LCD1602display.h"
#include<HJ-4WD_PWM.H>		  //包含HL-1蓝牙智能小车驱动IO口定义等函数

#define uchar unsigned char
#define uint  unsigned int
unsigned char disbuff[6]={0};//用于分别放距离的值	
unsigned char code Range[] ="==Range Finder==";//LCD1602显示格式
unsigned char code ASCII[15] ="0123456789CM S";

unsigned char code ASCIIR[13] = "distance:";
unsigned char code ASCIIL[13] = "LMspeed:";
unsigned char code table[]="Distance:001.0cm";
unsigned char code table1[]="!!! Out of range";

sbit TRIG=P2^1;
sbit ECHO=P2^0;

sbit pwm=P2^6;//PWM信号输出口

uchar n,count,angle,left,right;//距离标志位，0.5ms次数，角度标识

uchar Left_Distance;
uchar Right_Distance;
uchar Front_Distance;

//float S;//距离变量
unsigned int S=0 ;

void DelayUs2x(uchar t)
{   
	while(--t);
}
void DelayMs(uchar t)
{
	while(t--)
	{
	    //大致延时1mS	
		DelayUs2x(245);
	    DelayUs2x(245);
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
	  DisplayOneChar(8, 1, ASCIIR[8]);	  //

	  DisplayOneChar(9, 1, ASCII[disbuff[0]]);	  //右电机速度值百位
	  DisplayOneChar(10, 1, ASCII[disbuff[1]]);	  //右电机速度值十位
	  DisplayOneChar(11, 1, ASCII[disbuff[2]]);	   //右电机速度值个位
	  DisplayOneChar(12, 1, ASCII[10]);	   //
	  DisplayOneChar(13, 1, ASCII[11]);	   //

}
/*------------------------------------------------
                    定时器01初始化
------------------------------------------------*/
void TIM0init(void)
{
	TMOD|=0x11;//定时器1工作方式1 (超声波)，定时器0舵机
	TH1=0x00;    
    TL1=0x00;
    TH0=(65536-100)/256;
	TL0=(65536-100)%256;
    ET1=1;
    ET0=1;
    EA=1;
    TR0=1;//定时器开始
    IE=0x82;
}
void chaoshengbo()
{
	TRIG=1;//触发信号是高电平脉冲，宽度大于10us
    DelayUs2x(10);
    TRIG=0;
    while(!ECHO);//等待高电平
    TR1=1;
    while(ECHO);//等待低电平
    TR1=0;
    S=TH1*256+TL1;//取出定时器值高8位和低8位合并
    S=S/58;       //为什么除以58等于厘米，  Y米=（X秒*344）/2
                  // X秒=（ 2*Y米）/344 -> X秒=0.0058*Y米 -> 厘米=微秒/58 

	disbuff[0]=S/100;		    //距离值百位
	disbuff[1]=S%100/10;		//距离值十位
    disbuff[2]=S%100%10;		//距离值个位

    if(S>=25)
    {n=1;}
    if(S<25)
    {n=0;}

    TH1=0;
    TL1=0;//清除定时器0寄存器中的值
}
/*------------------------------------------------
                    主函数
------------------------------------------------*/
main()
{
	Delay1ms(400); //启动等待，等LCM讲入工作状态
	LCMInit(); //LCM初始化
	Delay1ms(5);//延时片刻
	angle=12;//舵机居中
    count=0;
	//Display_LCD();
    TIM0init();//初始化定时器
    while(1)
    {          
		chaoshengbo();
		Display_LCD();
		Front_Distance = S;
        DelayMs(20);
		
        if(Front_Distance > 30)//如果大于一定距离
        {run();}//前进
        
		if(Front_Distance < 30)//如果小于一定距离
        {
			stop();//停车
            
            angle=4;
            count=0;
            DelayMs(400);
            chaoshengbo();
			Display_LCD();
			Right_Distance= S;
            DelayMs(20);

            angle=12;
            count=0;
            DelayMs(400);

            angle=20;
            count=0;
            DelayMs(400);
            chaoshengbo();
			Display_LCD();
			Left_Distance = S;
            DelayMs(20);
          
			angle=12;
            count=0;	
            DelayMs(400);
 			if(Left_Distance < Right_Distance)	  //左边距离小于右边的距离
            {
            	rightrun();		   //调用小车右转	函数
            	DelayMs(100);
        	}
            
			if(Left_Distance >= Right_Distance)
            {
           		leftrun();		  //调用小车左转  函数
            	DelayMs(100);
            }
			
			 if((Left_Distance < 30 ) &&( Right_Distance < 30 ))  //左右两边距离都小于30
   		    {
        		back();//后退
        		DelayMs(80);
        		leftrun;//左转
        		DelayMs(90);
			}
		}        
	}
}

/*------------------------------------------------
                 定时器中断子程序
------------------------------------------------*/
void Time0_0(void) interrupt 3
{
	ECHO=0;
}
void Time1_Int() interrupt 1//舵机
{
	 TH0=(65536-100)/256;
	 TL0=(65536-100)%256;
	 if(count<angle)//判断0.5ms次数是否小于角度标识
     pwm=1;//确实小于，pwm输出高电平
     else
     pwm=0;//大于则输出低电平
	 count=(count+1);//0.5ms次数加1
  //	count=count%40;//次数始终保持为40即保持周期为20ms
     if(count == 200)   //	保持周期为20ms
	 count = 0;

	 time3++;
	 pwm_val_left++;
	 pwm_val_right++;
	 pwm_out_left_moto();
	 pwm_out_right_moto();
}

 
 
 

