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

//#include <reg52.h>    
#include <STC89C5xRC.H>
#include <intrins.h>
#include "LCD1602display.h"
#define uint    unsigned int                          //重定义无符号整数类型
#define uchar   unsigned char                         //重定义无符号字符类型

uchar code  LedShowData[]={0x03,0x9F,0x25,0x0D,0x99,  //定义数码管显示数据
                            0x49,0x41,0x1F,0x01,0x19};//0,1,2,3,4,5,6,7,8,9

uchar code  RecvData[]={0x19,0x46,0x15,0x44,0x43,0x40,0x0D,0x0E,0x00,0x0F};

uchar IRCOM[7];			 							 //缓存红外接收数据

static unsigned int LedFlash;                         //定义闪动频率计数变量

unsigned char RunFlag=0;                              //定义运行标志位
bit EnableLight=0;                                    //定义指示灯使能位

/*************完成状态指示灯定义*************/
sbit S1=P3^2;                                         //定义S1按键端口
                                         
/*************完成电机控制端口定义***********/
sbit M1A=P1^2;                                        //定义电机1正向端口
sbit M1B=P1^3;                                        //定义电机1反向端口
sbit M2A=P1^4;                                        //定义电机2正向端口
sbit M2B=P1^5;                                        //定义电机2反向端口

sbit SB1=P2^3;                                        //定义蜂鸣端口
/*********完成话筒,光敏电阻,蜂鸣器.端口定义**/
sbit IRIN=P3^3;                                       //定义红外接收端口
/*********完成红外接收端口的定义*************/
#define ShowPort P0                                   //定义数码管显示端口

extern void ControlCar(uchar CarType);                //声明小车控制子程序

void delayms(unsigned char x)                         //0.14mS延时程序
{
  unsigned char i;                                    //定义临时变量
  while(x--)                                          //延时时间循环
  {
    for (i = 0; i<13; i++) {}                         //14mS延时
  }
}


void Delay()                                          //定义延时子程序
{ uint DelayTime=30000;                               //定义延时时间变量
  while(DelayTime--);                                 //开始进行延时循环
  return;                                             //子程序返回
}

void tingzhi()
{
   M1A=0;                                   //将M1电机A端初始化为0
   M1B=0;                                   //将M1电机B端初始化为0
   M2A=0;                                   //将M2电机A端初始化为0
   M2B=0;

}

void qianjin()
{
   M1A=1;                                   
   M1B=0;                                   
   M2A=1;                                   
   M2B=0;
}

void houtui()
{
   M1A=0;                                   
   M1B=1;                                   
   M2A=0;                                   
   M2B=1;
}

void youzhuan()
{
   M1A=1;                                   
   M1B=0;                                   
   M2A=0;                                   
   M2B=1;

}

void zuozhuan()
{
   M1A=0;                                   
   M1B=1;                                   
   M2A=1;                                   
   M2B=0;

}

void ControlCar(unsigned char ConType)    //定义电机控制子程序
{
 
  tingzhi();
 switch(ConType)                          //判断用户设定电机形式
 {
  case 1:  //前进                         //判断用户是否选择形式1
  { 
    
	tingzhi();						      //进入前进之前 先停止一段时间  防止电机反向电压冲击主板 导致系统复位
	 Delay1ms(240);
	qianjin();
	ShowPort=LedShowData[1];
    break;
  }
  case 2: //后退                              //判断用户是否选择形式2
  { 
    tingzhi();							      //进入后退之前 先停止一段时间  防止电机反向电压冲击主板 导致系统复位
	  Delay1ms(240);
//	LeftLed = 1 ; 	 
	houtui();
	ShowPort=LedShowData[2];                                //M2电机反转
    break;
  }
  case 3: //左转                              //判断用户是否选择形式3
  { 
    tingzhi();								  //进入左转之前 先停止一段时间  防止电机反向电压冲击主板 导致系统复位
	 Delay1ms(240); 
	zuozhuan(); 
	ShowPort=LedShowData[3];                              //M2电机正转
	break;
  }
  case 4: //右转                              //判断用户是否选择形式4
  { 
    tingzhi();								  //进入右转之前 先停止一段时间  防止电机反向电压冲击主板 导致系统复位
	  Delay1ms(240);
	youzhuan();                                //M1电机正转
    ShowPort=LedShowData[4];                                      //M2电机反转
	break;
  }
  case 5: //停止                          //判断用户是否选择形式8
  {
    tingzhi();
	ShowPort=LedShowData[0]; 
	break; 
	                              //退出当前选择
  }
 }
}

void IR_IN() interrupt 2 using 0                      //定义INT2外部中断函数
{
  unsigned char j,k,N=0;                              //定义临时接收变量
  DisplayListChar(0, 0, IRCOM);   
  EX1 = 0;                                            //关闭外部中断,防止再有信号到达   
  delayms(15);                                        //延时时间，进行红外消抖
  if (IRIN==1)                                        //判断红外信号是否消失
  {  
     EX1 =1;                                          //外部中断开
	 return;                                          //返回
  } 
                           
  while (!IRIN)                                       //等IR变为高电平，跳过9ms的前导低电平信号。
  {
      delayms(1);                                     //延时等待
  }

  for (j=0;j<4;j++)                                   //采集红外遥控器数据
  { 
    for (k=0;k<8;k++)                                 //分次采集8位数据
    {
       while (IRIN)                                   //等 IR 变为低电平，跳过4.5ms的前导高电平信号。
       {
         delayms(1);                                  //延时等待
       }
       
       while (!IRIN)                                  //等 IR 变为高电平
       {
         delayms(1);                                  //延时等待
       }
   
       while (IRIN)                                   //计算IR高电平时长
       {
         delayms(1);                                  //延时等待
         N++;                                         //计数器加加
         if (N>=30)                                   //判断计数器累加值
	     { 
           EX1=1;                                     //打开外部中断功能
	       return;                                    //返回
         }                   
       }
                                       
      IRCOM[j]=IRCOM[j] >> 1;                         //进行数据位移操作并自动补零
     
      if (N>=8)                                       //判断数据长度 
      {
         IRCOM[j] = IRCOM[j] | 0x80;                  //数据最高位补1
      } 
      N=0;                                            //清零位数计录器
    }
  }
   
  if (IRCOM[2]!=~IRCOM[3])                            //判断地址码是否相同
  { 
     EX1=1;                                           //打开外部中断
     return;                                          //返回
  }
  DisplayListChar(0, 0, IRCOM);

  for(j=0;j<10;j++)                                   //循环进行键码解析
   {
   	  if(IRCOM[2]==RecvData[j])                       //进行键位对应
      {
       // ControlCar(j);                                          //数码管显示相应数码
      }
   }
   
   
   EX1 = 1;                                           //外部中断开 
} 

void main(void)                                       //主程序入口
{
 bit ExeFlag=0;                                       //定义可执行位变量
 
 LedFlash=3000;                                       //对闪灯数据进行初始化
 EX1=1;                                               //同意开启外部中断1
 IT1=1;                                               //设定外部中断1为低边缘触发类型
 EA=1;                                                //总中断开启
 ShowPort=LedShowData[0];                             //数码管显示数字0
while(1)                                              //程序主循环
 {
   
   Delay();                                           //延时
  // RunStopState=~RunStopState;                        //运行指示灯状态转变
  
 }
}
