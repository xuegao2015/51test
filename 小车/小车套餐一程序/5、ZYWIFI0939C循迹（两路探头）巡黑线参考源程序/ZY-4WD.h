/******************************************************************/
#ifndef _LED_H_
#define _LED_H_


    //定义小车驱动模块输入IO口 
	sbit IN1=P1^0;
	sbit IN2=P1^1;
	sbit IN3=P1^2;
	sbit IN4=P1^3;
	sbit IN5=P1^4;
	sbit IN6=P1^5;
	sbit IN7=P1^6;
	sbit IN8=P1^7;	

	//蜂鸣器接线定义
    sbit BUZZ=P3^2;

	//HJ-4WD小车底盘4路传感器接线定义
    #define Left_1_led        P3_7	 //P3_7接四路寻迹模块接口第一路输出信号即中控板上面标记为OUT1
	#define Left_2_led        P3_6	 //P3_6接四路寻迹模块接口第二路输出信号即中控板上面标记为OUT2	
    #define Right_1_led       P3_5	 //P3_5接四路寻迹模块接口第三路输出信号即中控板上面标记为OUT3
	#define Right_2_led       P3_4	 //P3_4接四路寻迹模块接口第四路输出信号即中控板上面标记为OUT4

	//HJ-4WD小车驱动接线定义
    #define Left_moto_go      {P1_4=1,P1_5=0,P1_6=1,P1_7=0;}    //左边两个电机向前走
	#define Left_moto_back    {P1_4=0,P1_5=1,P1_6=0,P1_7=1;}    //左边两个电机向后转
	#define Left_moto_Stop    {P1_4=0,P1_5=0,P1_6=0,P1_7=0;}    //左边两个电机停转                     
	#define Right_moto_go     {P1_0=1,P1_1=0,P1_2=1,P1_3=0;}	//右边两个电机向前走
	#define Right_moto_back   {P1_0=0,P1_1=1,P1_2=0,P1_3=1;}	//右边两个电机向后走
	#define Right_moto_Stop   {P1_0=0,P1_1=0,P1_2=0,P1_3=0;}	//右边两个电机停转  

//延时函数	
   void delay(unsigned int k)
{    
     unsigned int x,y;
	 for(x=0;x<k;x++) 
	   for(y=0;y<2000;y++);
}
/************************************************************************/
//全速前进函数 
     void  run(void)
{    
     IN1=1;	 	 
     IN2=0;	 
	 IN3=1;	 
	 IN4=0; 
     IN5=1;	 	 
     IN6=0;	 
	 IN7=1;	 
	 IN8=0;
	 //P1=0X55;
}
/*********************************************************************/		 
/************************************************************************/
//全速后退函数 
     void  backrun(void)
{
     IN1=0;	 	 
     IN2=1;	 
	 IN3=0;	 
	 IN4=1; 
     IN5=0;	 	 
     IN6=1;	 
	 IN7=0;	 
	 IN8=1;
}
/*********************************************************************/	
/************************************************************************/
//全速左转函数 
     void  leftrun(void)
{
     IN1=1;	 	 
     IN2=0;	 
	 IN3=1;	 
	 IN4=0; 
     IN5=0;	 	 
     IN6=1;	 
	 IN7=0;	 
	 IN8=1;
}
/*********************************************************************/	
/************************************************************************/
//全速右转函数 
     void  rightrun(void)
{
     IN1=0;	 	 
     IN2=1;	 
	 IN3=0;	 
	 IN4=1; 
     IN5=1;	 	 
     IN6=0;	 
	 IN7=1;	 
	 IN8=0;
}
/*********************************************************************/	
/************************************************************************/
//电机停止转动函数 
     void  stop(void)
{
     IN1=0;	 	 
     IN2=0;	 
	 IN3=0;	 
	 IN4=0; 
     IN5=0;	 	 
     IN6=0;	 
	 IN7=0;	 
	 IN8=0;
	 //P1=0X00;
}
/*********************************************************************/	

#endif