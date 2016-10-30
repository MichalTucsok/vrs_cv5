#include <stddef.h>
#include "stm32l1xx.h"
#include "vrs_cv5.h"

GPIO_InitTypeDef GPIO_InitStructure;
ADC_InitTypeDef ADC_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
USART_InitTypeDef USART_InitStructure;

extern uint16_t value;
extern uint8_t stav;
extern char txt[10];


void gpioInit(){


	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
}

void delay(uint32_t pocet){
	uint32_t i=0;
	for(i;i<pocet;i++){}
}

void LED(uint16_t ADvalue){
	if ((ADvalue>=1800)&&(ADvalue<=2500)){
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
			delay(50000);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
			delay(50000);
	}
	else if((ADvalue>=2501)&&(ADvalue<=3200)){
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
			delay(150000);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
			delay(150000);
	}
	else if((ADvalue>=3201)&&(ADvalue<=3500)){
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
			delay(300000);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
			delay(300000);
	}
	else if((ADvalue>=3501)&&(ADvalue<=3700)){
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
			delay(500000);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
			delay(500000);
		}
	else{
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
	}
}

void adc_init(void)
{



	/* Enable ADC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* Enable the HSI oscillator */
	RCC_HSICmd(ENABLE);

	/* Check that HSI oscillator is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);

	/* ADC1 configuration */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADCx regular channel8 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_384Cycles);
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	ADC_ITConfig(ADC1,ADC_IT_OVR,ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn; //zoznam prerušení nájdete v
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the ADC */
	ADC_Cmd(ADC1, ENABLE);

	/* Wait until the ADC1 is ready */
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET);
	ADC_SoftwareStartConv(ADC1);


}

void usart_init(void)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2,&USART_InitStructure);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);


	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART2,ENABLE);
}

void ADC1_IRQHandler(){		//nazov z startup_stm32l1xx_hd.s
	if(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC))
	{
		value = ADC_GetConversionValue(ADC1);
	}

	if(ADC_GetFlagStatus(ADC1,ADC_FLAG_OVR))
	{
		ADC_ClearFlag(ADC1,ADC_FLAG_OVR);
	}
}

void USART2_IRQHandler()
{
	if(USART_GetFlagStatus(USART2,USART_FLAG_RXNE))
	{
		if(USART_ReceiveData(USART2)=='m')
		{
			stav=!stav; //po stlaceni tlacidla 'm' sa zmeni hodnota tlacidla 'stav'
		}
		if (USART_GetFlagStatus(USART2, USART_FLAG_TC)) {
				USART_ClearFlag(USART2, USART_FLAG_RXNE);
				USART_ClearFlag(USART2, USART_FLAG_TC);
		}
	}
	if(USART_GetFlagStatus(USART2,USART_FLAG_TXE))
	{
		int i=0;
		if (txt[i] !=0 ){
			USART_SendData(USART2, txt[i]);
			i++;
		}
		else{
		i=0;
		USART_ClearFlag(USART2, USART_FLAG_TXE);
		}
		}
	}


/* void PutcUART2(char *ch)
{
	uint8_t i=0;
	while(ch[i])
	{
	USART_SendData(USART2,ch[i]);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	i++;
	}
	USART_SendData(USART2,'\n');
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
	USART_SendData(USART2,'\r');
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
} */
/*
void print_response(void)
{
	uint8_t i, resp[5];
	for (i=0;i<5;i++)
	{
		while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE)== RESET);
					resp[i] = USART_ReceiveData(USART2);
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}
	sprintf((char*)buffer, "0x%x 0x%x 0x%x 0x%x 0x%x", resp[0], resp[1], resp[2], resp[3], resp[4]);
	// LCD_DisplayStringLine(LINE(13), (uint8_t*)buffer);
	Delay(200);
}
*/
