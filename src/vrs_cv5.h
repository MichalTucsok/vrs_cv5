

#ifndef VRS_CV5_H_
#define VRS_CV5_H_

#include "stm32l1xx.h"

void nvicInit(void);
void gpioInit(void);
void delay(uint32_t pocet);
void adc_init(void);

void LED(uint16_t ADvalue);
void usart_init(void);

void PutcUART2(char *ch);

#endif /* VRS_CV5_H_ */
