/*****************ZYDJ-CSB09A�������*****************
*  ƽ̨��ZYDJ-CSB09A������� + Keil U4 + STC89C52
*  �������ƣ����������ϳ���+���ת��ҡͷ
*  ��˾����������ƽ��豸���޹�˾
*  �Ա���http://tmmcu1688.taobao.com       
*  ��վ��www.china-zhiyu.com	
*  ��д�����˾�з�һ��
*  ���ڣ�2015-1-15
*  ����:���ܳ�QQ:1018973181 
*  ����:11.0592MHZ
*  �ۺ���˵������ѿ�Դ����֧�ֿͻ��Լ��޸ĳ������������������ṩ�ҹ�˾����˵��
*  ��Ƶ�̳̣���С������ѧϰC������ϸ��Ƶ�̳̣�����ͳһ��������
******************************************************************/

/* ������������ɨ��,��һ��û���ϰ����ı���,�������˺�ת��ת������
	  angle=12  ��� ����    angle=4  �պ��ұ�   angle=20  �պ����

	  ���Զ�����еķ����������Դ�����幤��ʱ������պ��Ǿ��е�
	  ����ʱ��������װ�װ���ɫ����������棬����˿����Ϳ����ˡ�

/* ����11.00592 MHz CPU�ͺţ�STC89C52RC */
/*****************************************************************/ 
#include<AT89x51.H>
#include <intrins.h>
#include "LCD1602display.h"
#include<HJ-4WD_PWM.H>		  //����HL-1��������С������IO�ڶ���Ⱥ���

#define uchar unsigned char
#define uint  unsigned int
unsigned char disbuff[6]={0};//���ڷֱ�ž����ֵ	
unsigned char code Range[] ="==Range Finder==";//LCD1602��ʾ��ʽ
unsigned char code ASCII[15] ="0123456789CM S";

unsigned char code ASCIIR[13] = "distance:";
unsigned char code ASCIIL[13] = "LMspeed:";
unsigned char code table[]="Distance:001.0cm";
unsigned char code table1[]="!!! Out of range";

sbit TRIG=P2^1;
sbit ECHO=P2^0;

sbit pwm=P2^6;//PWM�ź������

uchar n,count,angle,left,right;//�����־λ��0.5ms�������Ƕȱ�ʶ

uchar Left_Distance;
uchar Right_Distance;
uchar Front_Distance;

//float S;//�������
unsigned int S=0 ;

void DelayUs2x(uchar t)
{   
	while(--t);
}
void DelayMs(uchar t)
{
	while(t--)
	{
	    //������ʱ1mS	
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

	  DisplayOneChar(9, 1, ASCII[disbuff[0]]);	  //�ҵ���ٶ�ֵ��λ
	  DisplayOneChar(10, 1, ASCII[disbuff[1]]);	  //�ҵ���ٶ�ֵʮλ
	  DisplayOneChar(11, 1, ASCII[disbuff[2]]);	   //�ҵ���ٶ�ֵ��λ
	  DisplayOneChar(12, 1, ASCII[10]);	   //
	  DisplayOneChar(13, 1, ASCII[11]);	   //

}
/*------------------------------------------------
                    ��ʱ��01��ʼ��
------------------------------------------------*/
void TIM0init(void)
{
	TMOD|=0x11;//��ʱ��1������ʽ1 (������)����ʱ��0���
	TH1=0x00;    
    TL1=0x00;
    TH0=(65536-100)/256;
	TL0=(65536-100)%256;
    ET1=1;
    ET0=1;
    EA=1;
    TR0=1;//��ʱ����ʼ
    IE=0x82;
}
void chaoshengbo()
{
	TRIG=1;//�����ź��Ǹߵ�ƽ���壬��ȴ���10us
    DelayUs2x(10);
    TRIG=0;
    while(!ECHO);//�ȴ��ߵ�ƽ
    TR1=1;
    while(ECHO);//�ȴ��͵�ƽ
    TR1=0;
    S=TH1*256+TL1;//ȡ����ʱ��ֵ��8λ�͵�8λ�ϲ�
    S=S/58;       //Ϊʲô����58�������ף�  Y��=��X��*344��/2
                  // X��=�� 2*Y�ף�/344 -> X��=0.0058*Y�� -> ����=΢��/58 

	disbuff[0]=S/100;		    //����ֵ��λ
	disbuff[1]=S%100/10;		//����ֵʮλ
    disbuff[2]=S%100%10;		//����ֵ��λ

    if(S>=25)
    {n=1;}
    if(S<25)
    {n=0;}

    TH1=0;
    TL1=0;//�����ʱ��0�Ĵ����е�ֵ
}
/*------------------------------------------------
                    ������
------------------------------------------------*/
main()
{
	Delay1ms(400); //�����ȴ�����LCM���빤��״̬
	LCMInit(); //LCM��ʼ��
	Delay1ms(5);//��ʱƬ��
	angle=12;//�������
    count=0;
	//Display_LCD();
    TIM0init();//��ʼ����ʱ��
    while(1)
    {          
		chaoshengbo();
		Display_LCD();
		Front_Distance = S;
        DelayMs(20);
		
        if(Front_Distance > 30)//�������һ������
        {run();}//ǰ��
        
		if(Front_Distance < 30)//���С��һ������
        {
			stop();//ͣ��
            
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
 			if(Left_Distance < Right_Distance)	  //��߾���С���ұߵľ���
            {
            	rightrun();		   //����С����ת	����
            	DelayMs(100);
        	}
            
			if(Left_Distance >= Right_Distance)
            {
           		leftrun();		  //����С����ת  ����
            	DelayMs(100);
            }
			
			 if((Left_Distance < 30 ) &&( Right_Distance < 30 ))  //�������߾��붼С��30
   		    {
        		back();//����
        		DelayMs(80);
        		leftrun;//��ת
        		DelayMs(90);
			}
		}        
	}
}

/*------------------------------------------------
                 ��ʱ���ж��ӳ���
------------------------------------------------*/
void Time0_0(void) interrupt 3
{
	ECHO=0;
}
void Time1_Int() interrupt 1//���
{
	 TH0=(65536-100)/256;
	 TL0=(65536-100)%256;
	 if(count<angle)//�ж�0.5ms�����Ƿ�С�ڽǶȱ�ʶ
     pwm=1;//ȷʵС�ڣ�pwm����ߵ�ƽ
     else
     pwm=0;//����������͵�ƽ
	 count=(count+1);//0.5ms������1
  //	count=count%40;//����ʼ�ձ���Ϊ40����������Ϊ20ms
     if(count == 200)   //	��������Ϊ20ms
	 count = 0;

	 time3++;
	 pwm_val_left++;
	 pwm_val_right++;
	 pwm_out_left_moto();
	 pwm_out_right_moto();
}

 
 
 

