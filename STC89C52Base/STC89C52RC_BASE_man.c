/**********************ZYWIFI0939C-WIFI������ʵ�������************************
*  ƽ̨��ZYWIFI0939C-WIFI������ + Keil U4 + STC89C52
*  ���ƣ�ZY-1����С���ο�����
*  ��˾����������ƽ��豸���޹�˾
*  �Ա���https://hnzyrobot1688.taobao.com/     
*  ��վ��www.hnzhiyu.cn	
*  ��д�����˾�з�һ��
*  ���ڣ�2015-1-15
*  ����:���ܳ�QQ:261339276
*  ����:11.0592MHZ
*  ˵������ѿ�Դ�����ṩԴ�������
*  Ӳ�����ã�Ҫ���Լ������������������ʵ��
*  ʹ��˵������������IO���Լ��öŰ������Ӹ���ģ�飬�����Լ��޸ĸ���ģ��IO��
*  ��Ƶ�̳̣���С������ѧϰC������ϸ��Ƶ�̳̣�����ͳһ��������
   �ص���ʾ��������ֻ���ο������ṩ����֧�֣����Լ��о����ա�
  ZYWIFI0939WIFI�������ܻ����˶Ű��߽��߷�������һ��������������ܲ����������ջ�С����
   
******************************************************************/   

#include "MCU_STC89C52RC.h"
#include <intrins.h>
#include "LCD1602display.h"
#define uint    unsigned int                          //�ض����޷�����������
#define uchar   unsigned char       

unsigned char code Range[] ="==Range Finder==";//LCD1602��ʾ��ʽ
unsigned char code ASCII[13] = "0123456789.-M";
unsigned char code table[]="Distance:000.0cm";
unsigned char code table1[]="!!! Out of range";

void cmg88()//������ܣ�������
{
	P26=1;  
	P0=0X00;
	P26=0;
}
//=======================  1602 LED TEST
void testLed(); 
void testLed()
{


	cmg88();//�������
	Delay1ms(400); //�����ȴ�����LCM���빤��״̬
	LCMInit(); //LCM��ʼ��
	Delay1ms(5);//��ʱƬ��

	DisplayListChar(0, 0, Range);
	DisplayListChar(0, 1, table);

	while(1)                                              //������ѭ��
	 {
	 	unsigned char i;
	    for(i=13;i>0;i--)
		{
	    	Delay1ms(1000);  
			DisplayOneChar(15, 1, ASCII[i]);      
		}                                   //��ʱ
	  
	 }
}
//====end 1602 LED Test===========

void main(void)                                       //���������
{
	  testLed();

}


