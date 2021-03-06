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

/******************************************************************/
	
	#include<AT89X52.H>		  //包含51单片机头文件，内部有各种寄存器定义
	#include<ZY-4WD_PWM.H>		  //包含HL-1蓝牙智能小车驱动IO口定义等函数
    
//主函数
	void main(void)
{	

	unsigned char i;
    P1=0X00;   //关电机	
		 	TMOD=0X01;
        	TH0= 0XFc;		  //1ms定时
         	TL0= 0X18;
           	TR0= 1;
        	ET0= 1;
	        EA = 1;			   //开总中断


	while(1)	//无限循环
	{ 
	 
			 //有信号为0  没有信号为1
			if( LeftIRBZ == 0 || RightIRBZ == 0 )  

			{
			
			  stop();		   //调用小车停止

			}
             
			 

			  else

			 {			  	   if(Left_1_led==0&&Right_1_led==0)

			   {
			     run();   //调用前进函数

			  
			   }
				              
							   if(Left_1_led==1&&Right_1_led==0)	    //左边检测到黑线
			 	 {
				 	  leftrun();		  //调用小车左转  函数
					 // delay(2);

			     }
			   
				 			    if(Right_1_led==1&&Left_1_led==0)		//右边检测到黑线
				  {	  
				      rightrun();		   //调用小车右转	函数
					 // delay(2);

				  }
						    if(Right_1_led==1&&Left_1_led==1)		//悬空状态  避悬崖
				  {	  
				      stop();		   //调用小车停止

				  }

			}	 
	 }
}