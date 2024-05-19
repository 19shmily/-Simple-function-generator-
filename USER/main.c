#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"  
#include "dma.h"
#include "dac.h"
#include "timer.h"


u16 any[50];
 u16 Sine6bit3[50];
u16 Sine12bit3[500];
const u16 Sine6bit2[50] = {		//���Ҳ�
	2048,2304,2557,2801,3034,3251,3449,3626,3777,3901,
	3995,4059,4091,4091,4059,3995,3901,3777,3626,3449,
	3251,3034,2801,2557,2304,2048,1791,1538,1294,1061,
	844,646,469,318,194,100,36,4,4,36,100,194,318,469,
	646,844,1061,1294,1538,1791};
const u16 Sine6bit4[50] = {		//���ǲ�
	4,164,328,492,656,820,984,1148,1312,1476,1640,1804,
	1968,2132,2296,2460,2624,2788,2952,3116,3280,3444,
	3608,3772,3936,4091,3932,3768,3604,3440,3276,3112,
	2948,2784,2620,2456,2292,2128,1964,1800,1636,1472,
	1308,1144,980,816,652,488,324,160};
const u16 Sine6bit5[50] = {		//����
	4091,4091,4091,4091,4091,4091,4091,4091,4091,4091,
	4091,4091,4091,4091,4091,4091,4091,4091,4091,4091,
	4091,4091,4091,4091,4091,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0};
const u16 Sine1bit5[2] = {		//����
	4091,0};
	
void Waveform_generated(void);   //��������
void Waveform_random(void);		 //�������Ⲩ��


u8 chansheng[]={"1 chansheng"};
u8 random[]={"2 random"};
u16 i,len=0,flag=0,flag1=0,pro=1.0;
u32 t=0,f=0;
u32 j,k=0,dacval=4;

u8 waveformdate[300]={1};//  ��1λΪ�洢���εĸ��� ��ʼΪ1//��������Ϊÿ�����εĲ��Σ���ֵ��Ƶ��
#define SIZE sizeof(waveformdate)
	
int main(){
	u8 num;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);     //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED  
	KEY_Init(); 				//������ʼ�� 
	Dac1_Init();		 		//DACͨ��1��ʼ��	
	MYDMA_Config(DMA1_Stream5,DMA_Channel_7,(u32)&DAC->DHR12R1,(u32)Sine6bit3,50);
	//DMA1,STEAM5,CH7,����ΪDAC->DHR12R1,�洢��ΪSine6bit3,����Ϊ:50.
	TIM2_Int_Init(4-1,2-1); 
	DAC_DMACmd(DAC_Channel_1, ENABLE);//ʹ��DMA	
	MYDMA_Enable(DMA1_Stream5,50);     //��ʼһ��DMA���䣡
	for(i=0;i<50;i++)//��ʼֵΪ���Ҳ�
	{
		Sine6bit3[i]=Sine6bit2[i];	   
    }	  
	i=0;
	
	printf("Ready!\n");
	delay_ms(500);
	
	while(1){
		t=KEY_Scan(0);
		if(t==WKUP_PRES)
		{
			t=5;
			printf(" �������ţ�k0.��������  k1.���Ⲩ��\r\n");
			while(k==0)
			{	
			t=KEY_Scan(0);				
			switch (t)
				{ 
				case KEY0_PRES :Waveform_generated();k=1;break;
				case KEY1_PRES :Waveform_random();k=1;break;
				default :;
				}
			}
		}
				if(t==KEY1_PRES)  //KEY1���£��ı�װ��ϵ�����ı�Ƶ��
		{
			dacval+=1;
			TIM2_Int_Init(dacval-1,2-1); 	
		}
//			DAC_DMACmd(DAC_Channel_1, ENABLE);//ʹ��DMA			
//			MYDMA_Enable(DMA1_Stream5,50);     //��ʼһ��DMA���䣡
			while(1)
		    {
				if(DMA_GetFlagStatus(DMA1_Stream5,DMA_FLAG_TCIF5)!=RESET)//�ȴ�DMA1_Steam5�������
				{ 
					DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_TCIF5);//���DMA1_Steam5������ɱ�־				
					break; 
		        }
			}
		i++;k=0;
		USART_RX_STA=0;
		delay_ms(10);
		if(i>=20)
		{
			LED0=!LED0;//��ʾϵͳ��������	
			i=0;
		}
	}
			
	}
			

void Waveform_generated(void){
	u8 i,s;
	u32 sum;
	
	printf(" ��������:k0.���Ҳ�  k1.���ǲ�  UP.������\r\n");
	while(1){
		s=KEY_Scan(0);
		if(s!=0)
		{
			waveformdate[1]=s;
			break;
		}
	}
	printf(" ������Ƶ��\r\n");
	 while(1){
		 sum=0;
		if(USART_RX_STA&0xC000){//�Ƿ��յ�
			if((USART_RX_STA&0x3FFF)==0) //����һ���س� ����ʾһ��
			{
				printf(" ������Ƶ��\r\n");
				USART_RX_STA=0;					
			}
			else {
				for(i=0;i<(USART_RX_STA&0x3FFF);i++){
					sum=sum*10+(USART_RX_BUF[i]-0x30);   //���������ֵ
				}
				printf("%d,",sum);
				sum=sum/10;
				printf("%d",sum);
				waveformdate[2]=sum/100;  //����������
				waveformdate[3]=sum%100;
				USART_RX_STA=0;
				break;	
			}
			
		}
	}
	printf(" �������ֵ\r\n");
	 while(1){
		 sum=0;
		 pro=1;
		if(USART_RX_STA&0xC000){
			if((USART_RX_STA&0x3FFF)==0)
			{
				printf(" �������ֵ\r\n");
				USART_RX_STA=0;
				for(i=0;i<50;i++)
						{
							USART_RX_BUF[i]=0;
						}
			
			}
			else {
				for(i=0;i<(USART_RX_STA&0x3FFF);i++){
					sum=sum*10+(USART_RX_BUF[i]-0x30);
				}
				waveformdate[4]=sum;
				USART_RX_STA=0;
				break;	
			}
			
		}
	}
	switch (waveformdate[1]){ 
			case 1 :for(i=0;i<50;i++)
					{
						Sine6bit3[i]=Sine6bit2[i];
					}break;
			case 2 :for(i=0;i<50;i++)
					{
						Sine6bit3[i]=Sine6bit4[i];
					}break;
			case 3 :for(i=0;i<50;i++)
					{
						Sine6bit3[i]=Sine6bit5[i];
					}break;
			default :;
			}
	dacval=836250/(waveformdate[2]*1000+waveformdate[3]*10);
			printf("%d,%d",waveformdate[2],waveformdate[3]);
	TIM2_Int_Init(dacval-1,2-1);
	for(i=0;i<50;i++)
	{
		Sine6bit3[i]=Sine6bit3[i]/33*(waveformdate[4]);//��ֵ
	}
	printf("���β����ɹ���������Ĳ��Σ��밴��UP��\n");
}


void Waveform_random(void){
	//������������
	u8 s;
	flag=0;
	t=0;
	LED1=0;
	printf("���Ⲩ��ģʽ��������50������\r\n");
	while(flag==0)
	{
	
		if(USART_RX_STA&0x8000)
		{					   
		len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
		printf("%d\r\n",(len));
			for(i=0;i<50;i++)
			{
				Sine6bit3[i]=1000*(USART_RX_BUF[t]-48)+100*(USART_RX_BUF[t+1]-48)+10*(USART_RX_BUF[t+2]-48)+(USART_RX_BUF[t+3]-48);
				t+=4;
			}
			for(i=0;i<50;i++)
					{
						printf("%d\r\n",Sine6bit3[i]);
					}
		USART_RX_STA=0;//����������´ν���
		flag=1;
		LED1=1;
		i=0;
		break;
		}
	}
		printf("���β����ɹ���������Ĳ��Σ��밴��UP��\n");
}


void wave_chansheng(u8 s){	
	if(s!=0){
switch (waveformdate[s*4+1]){ 
			case 1 :for(i=0;i<50;i++)
					{
						Sine6bit3[i]=Sine6bit2[i];
					}break;
			case 2 :for(i=0;i<50;i++)
					{
						Sine6bit3[i]=Sine6bit4[i];
					}break;
			case 3 :for(i=0;i<50;i++)
					{
						Sine6bit3[i]=Sine6bit5[i];
					}break;
			default :;
			}
	dacval=836250/(waveformdate[s*4+2]*1000+waveformdate[s*4+3]*10);		
	TIM2_Int_Init(dacval-1,2-1);
	for(i=0;i<50;i++)
	{
		Sine6bit3[i]=Sine6bit3[i]/33*(waveformdate[s*4+4]);//��ֵ
	}
}
}