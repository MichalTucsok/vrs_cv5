

#ifndef VRS_CV5_H_
#define VRS_CV5_H_

#include "stm32l1xx.h"

void nvicInit(void);
void gpioInit(void);
void delay(int pocet);
void adc_init(void);
void ADC1_IRQHandler(void);
void LED(uint16_t ADvalue);
void usart_init(void);
void USART2_IRQHandler(void);
void PutcUART2(char *ch);

#endif /* VRS_CV5_H_ */
