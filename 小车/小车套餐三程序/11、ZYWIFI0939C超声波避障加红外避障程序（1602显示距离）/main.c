/**********************HL-1& ZY-2ʵ�鿪��������************************
*  ƽ̨��HL-1& ZY-2 V8.0 + Keil U3 + STC89C52
*  ���ƣ�1602 LCD ��ʾ����ң��������
*  ��˾��ѩ��
*  �Ա���https://hnzyrobot1688.taobao.com/     
*  ��վ��www.hnzhiyu.cn	
*  ��д��ѩ��
*  ���ڣ�2017-10-10
*  ����:11.0592MHZ
*  ��̳��
*  Ӳ�����ã�Ҫ���Լ������������������ʵ��
*  ʹ��˵������������IO���Լ��öŰ������Ӹ���ģ�飬�����Լ��޸ĸ���ģ��IO��

   �ص���ʾ��������ֻ���ο������ṩ����֧�֣����Լ��о����ա�

******************************************************************/
//˵�������û��LCD1602Һ����ʾ����ͬѧ�����öŰ��P0.7���Žӵ�GND��Ч��һ����
#include <at89x51.h>	
#include <intrins.h>
#include "LCD1602display.h"

#define  TX  P2_1
#define  RX  P2_0
sbit DU = P2^6;
sbit WE = P2^7;
#define Forward_L_DATA  180//��ǰ��������ֱ�ߵ�ʱ������������������������ʱ����100,100�����256����С0��0��ʱ��������256��ʱ�����
#define Forward_R_DATA  180	//����С��ǰ����ʱ���е�����գ�˵���ұ����ת�ٹ��죬�ǿ���ȡһ��ֵ��һ�㣬����һ��ֵСһ�㣬���� 200  190
                            //ֱ�������Ϊ�����ϵ���ͬһ��������Ҳ��һ���ٶ�һ�µģ���Ҫ�Լ��ֶ�����

//sbit P4_0=0xc0;	//P4�ڵ�ַ

/*****����ԭͼ���߶���******/
sbit L293D_IN1=P1^2; 
sbit L293D_IN2=P1^3;
sbit L293D_IN3=P1^4;
sbit L293D_IN4=P1^5;
sbit L293D_EN1=P1^6;
sbit L293D_EN2=P1^7;

sbit BUZZ=P2^3;
 #define Left_1_led        P3_5	 //�󴫸���  
	
 #define Right_1_led       P3_4	 //�Ҵ�����  

void cmg88()//������ܣ�������
{
DU=1;  
P0=0X00;
DU=0;
}

void Delay400Ms(void);//��ʱ400���뺯��

unsigned char code Range[] ="==Range Finder==";//LCD1602��ʾ��ʽ
unsigned char code ASCII[13] = "0123456789.-M";
unsigned char code table[]="Distance:000.0cm";
unsigned char code table1[]="!!! Out of range";

unsigned char disbuff[4]={0,0,0,0};//���ڷֱ��ž����ֵ0.1mm��mm��cm��m��ֵ

void Count(void);//������㺯��
			  
unsigned int  time=0;//���ڴ�Ŷ�ʱ��ʱ��ֵ
unsigned long S=0;//���ڴ�ž����ֵ
bit  flag =0; //���������־λ
bit  turn_right_flag;

//=========================================================================================================================
void Forward(unsigned char Speed_Right,unsigned char Speed_Left)//	   ǰ��
{

	 L293D_IN1=1; 
	 L293D_IN2=0;
	 L293D_IN3=1;
	 L293D_IN4=0;
//     PWM_Set(255-Speed_Right,255-Speed_Left);
}
void Stop(void)	//ɲ��
{

     L293D_IN1=0; 
	 L293D_IN2=0;
	 L293D_IN3=0;
	 L293D_IN4=0;
//	 PWM_Set(0,0);
}
void Turn_Right(unsigned char Speed_Right,unsigned char Speed_Left)	 //��
{
    L293D_IN1=0; 
	L293D_IN2=1;
	L293D_IN3=0;
	L293D_IN4=0;
//	PWM_Set(255-Speed_Right,255-Speed_Left);
}

void rightrun(unsigned char Speed_Right,unsigned char Speed_Left)	 //��
{
    L293D_IN1=0; 
	L293D_IN2=0;
	L293D_IN3=0;
	L293D_IN4=1;
//	PWM_Set(255-Speed_Right,255-Speed_Left);
}
//=========================================================================================================================
/********����������***************/
    void Conut(void)
	{
	 time=TH1*256+TL1;
	 TH1=0;
	 TL1=0;
	
	 //��ʱtime��ʱ�䵥λ�����ھ�����ٶȣ���Ӿ���Ϊ11.0592MHZʱ��
	            //time��ֵΪ0.54us*time����λΪ΢��
				//��ô1us�������߶�Զ�ľ����أ�1s=1000ms=1000000us 
				// 340/1000000=0.00034��
				//0.00034��/1000=0.34����  Ҳ����1us����0.34����
				//���ǣ��������ڼ�����Ǵӳ��������䵽������յ�˫·�̣�
				//�������ǽ�����Ľ������2����ʵ�ʵ�·��

	S=time*2;//�����һ����ʱ���Ƕ���΢�롣
   	S=S*0.17;//��ʱ���㵽�Ľ��Ϊ���ף������Ǿ�ȷ�����׵ĺ���λ�ˣ�������С���� 
	 if(S<=400)	 //
	 {	
	    if(turn_right_flag!=1)
		{
		    Stop();
		    Delay1ms(5);//����С���Զ���λ��ʱ�򣬿�����΢�ӳ�һ�������ʱ�����ٵ�������ѹ�Ե�·��ĳ����
		}
		turn_right_flag=1;
		P2_3=0;
		Delay1ms(50);
		P2_3=1;
	    Turn_Right(120,120); 				//С���趨����ʱ�������ת��
	 }
	 else
	 {
	    turn_right_flag=0;
	    Forward(Forward_R_DATA,Forward_L_DATA);			  //ǰ��������20-30CMǰ����
	 }
	 	 if(Left_1_led==1&&Right_1_led==0)	    //�ұ߼�⵽�����ź�
			 	 
	{
	  rightrun(120,120);	 //����С����ת����

	 }
			   
	if(Right_1_led==1&&Left_1_led==0)		//��߼�⵽�����ź�
	{	  
				      
	    Turn_Right(120,120); 				//С���趨����ʱ�������ת��

	}  
	 //=======================================
	 if((S>=5000)||flag==1) //����������Χ
	 {	
	  flag=0;
      DisplayListChar(0, 1, table1);
	 }
	 else
	 {
      disbuff[0]=S%10;
	  disbuff[1]=S/10%10;
	  disbuff[2]=S/100%10;
	  disbuff[3]=S/1000;
	  DisplayListChar(0, 1, table);
	  DisplayOneChar(9, 1, ASCII[disbuff[3]]);
	  DisplayOneChar(10, 1, ASCII[disbuff[2]]);	
	  DisplayOneChar(11, 1, ASCII[disbuff[1]]);
      DisplayOneChar(12, 1, ASCII[10]);
	  DisplayOneChar(13, 1, ASCII[disbuff[0]]);
	 }
	}

/********************************************************/
     void zd0() interrupt 3 		 //T0�ж��������������,������෶Χ
  {
    flag=1;			 //�ж������־
	RX=0;
  }

/********�������ߵ�ƽ�����ȼ������***************/
void Timer_Count(void)
{
		 TR1=1;			    //��������
	     while(RX);			//��RXΪ1�������ȴ�
	     TR1=0;				//�رռ���
         Conut();			//����

}
/********************************************************/
   void  StartModule() 		         //����ģ��
  {
	  TX=1;			                     //����һ��ģ��
      Delay10us(2);
	  TX=0;
  }
/********************************************************/ 

/*************������********************/
void main(void)
{
    unsigned char i;
	unsigned int a;
	cmg88();//�������
	Delay1ms(400); //�����ȴ�����LCM���빤��״̬
	LCMInit(); //LCM��ʼ��
	Delay1ms(5);//��ʱƬ��

	DisplayListChar(0, 0, Range);
	DisplayListChar(0, 1, table);
    TMOD=TMOD|0x10;//��T0Ϊ��ʽ1��GATE=1��
    EA=1;					   //�������ж�
    TH1=0;
    TL1=0;          
    ET1=1;             //����T0�ж�

	//===============================
	//PWM_ini();
	//===============================
	turn_right_flag=0;
	//=================================
B:		for(i=0;i<50;i++) //�ж�K3�Ƿ���
		{
		   Delay1ms(1);	//1ms���ж�50�Σ����������һ�α��жϵ�K3û���£������¼��
		   if(P3_2!=0 )//��K3����ʱ������С��
		   goto B; //��ת�����B�����¼��
		}
	//��������һ��
	BUZZ=0;	//50�μ��K3ȷ���ǰ���֮�󣬷������������Ρ����죬Ȼ������С����
	Delay1ms(50);
	BUZZ=1;//��50ms��رշ�����
	//=======================================================================================================================			
 	while(1)
	  {
		RX=1;
	    StartModule();				 //����ģ��
        for(a=951;a>0;a--)
	    {
		   
	       if(RX==1)
		   {
           Timer_Count();		 //�������ߵ�ƽ�����ȼ��㺯��
		   }
    	 }
   	}
} 



                