/*
 * Timer1_8.c
 *
 *  Created on: Oct 13, 2023
 *      Author: medoo
 */


#include "Timer1_8.h"
TIMERx_type g_Sitting;
TIM1_8_REG* g_TIMX;
void ERROR_HANDLER(){
	while(1);
}

void MCAL_SET_TIMx_GPIO(TIM1_8_REG* TIMx){
	PIN_config pin={PIN_8,OUTPUT_AF_PP,SPEED_10};
	MCAL_GPIO_init(GPIOA, &pin);
}
void TIMER_1_8_start(TIM1_8_REG* TIMx,TIMERx_type* Sitting){
	g_Sitting=(* Sitting);
	g_TIMX=TIMx;
	if(TIMx== TIM1) RCC->APB2ENR|=(1<<11);
	else if(TIMx== TIM8) RCC->APB2ENR|=(1<<13);



	// 2.select the pre vale
	TIMx->PSC=Sitting->TIME_PSC;

	TIMx->ARR=Sitting->TIME_ARR;
	//	TIMx->CR1.BIT_NAME.ARPE=1;

		TIMx->RCR=Sitting->TIME_RCR;
	//1.Select the mode CMS[1:0]
	switch(Sitting->mode){
	case Up:
		TIMx->CR1.BIT_NAME.CMS=0b00; //00: Edge-aligned mode. The counter counts up or down depending on the direction bit (DIR).
		TIMx->CR1.BIT_NAME.DIR=0; //0: Counter used as upcounter
		break;
	case DOWN:
		TIMx->CR1.BIT_NAME.CMS=0b00; //00: Edge-aligned mode. The counter counts up or down depending on the direction bit (DIR).
		TIMx->CR1.BIT_NAME.DIR=1; //1: Counter used as downcounter
		break;
	case Up_Down:
		TIMx->CR1.BIT_NAME.CMS=0b11;
		/*11: Center-aligned mode 3. The counter counts up and down alternatively. Output compare
		interrupt flags of channels configured in output (CCxS=00 in TIMx_CCMRx register) are set
		both when the counter is counting up or down*/
		break;
	default:
		ERROR_HANDLER();
		break;
	}
	MCAL_SET_TIMx_GPIO(TIMx);
	TIMx->CR1.BIT_NAME.CEN=1;

	if(Sitting->INT_EN_DIS==INT_EN){

		TIMx->DIER.BIT_NAME.UIE=1;
		NVIC_ISER0|=(1<<25);


	}else{
		while(!TIMx->SR.BIT_NAME.UIF);
		TIMx->SR.BIT_NAME.UIF=0;
	}

}
void TIMER_1_CTC_start(TIM1_8_REG* TIMx,TIMERx_type* Sitting,Compare_mode_sitting* CTC_Sitting){
	g_Sitting=(* Sitting);
	g_TIMX=TIMx;
	if(TIMx== TIM1) RCC->APB2ENR|=(1<<11);
	else if(TIMx== TIM8) RCC->APB2ENR|=(1<<13);



	// 2.select the pre vale
	TIMx->PSC=Sitting->TIME_PSC;

	TIMx->ARR=Sitting->TIME_ARR;
	//	TIMx->CR1.BIT_NAME.ARPE=1;

		TIMx->RCR=Sitting->TIME_RCR;
	//1.Select the mode CMS[1:0]
	switch(Sitting->mode){
	case Compare_output:
		/*
		 * Procedure:
1. Select the counter clock (internal, external, prescaler). Done
2. Write the desired data in the TIMx_ARR and TIMx_CCRx registers.
3. Set the CCxIE bit if an interrupt request is to be generated.
4. Select the output mode. For example:
– Write OCxM = 011 to toggle OCx output pin when CNT matches CCRx
– Write OCxPE = 0 to disable preload register
– Write CCxP = 0 to select active high polarity
– Write CCxE = 1 to enable the output
5. Enable the counter by setting the CEN bit in the TIMx_CR1 register.*/
		TIMx->CCR1=Sitting->TIME_CCR;
		TIMx->CCMR1.Compare_BIT_NAME.OC1M=CTC_Sitting->TIM1_CTC_MD;
		TIMx->CCMR1.Compare_BIT_NAME.OC1PE=CTC_Sitting->TIM1_CTC_PE;
		TIMx->CCER.BIT_NAME.CC1P=CTC_Sitting->TIM1_POL;
		TIMx->BDTR.BIT_NAME.MOE=1;
//		TIMx->BDTR.BIT_NAME.OSSR=1;
//		TIMx->BDTR.BIT_NAME.OSSI=1;

		TIMx->CCER.BIT_NAME.CC1E=1;

		break;

	default:
		ERROR_HANDLER();
		break;
	}
	MCAL_SET_TIMx_GPIO(TIMx);
	TIMx->CR1.BIT_NAME.CEN=1;

	if(Sitting->INT_EN_DIS==INT_EN){
		TIMx->DIER.BIT_NAME.CC1IE=1;
		NVIC_ISER0|=(1<<27);
	}else{
		while(!TIMx->SR.BIT_NAME.CC1IF);
		TIMx->SR.BIT_NAME.CC1IF=0;
	}

}



void TIM1_UP_IRQHandler(){
	g_Sitting.f_callback();
	g_TIMX->SR.BIT_NAME.UIF=0;

}


void TIM1_CC_IRQHandler(){
	g_Sitting.f_callback();
	g_TIMX->SR.BIT_NAME.CC1IF=0;

}
void TIM1_BRK_IRQHandler(){
	g_Sitting.f_callback();
	g_TIMX->SR.BIT_NAME.UIF=0;

}
void TIM1_TRG_COM_IRQHandler(){
	g_Sitting.f_callback();
	g_TIMX->SR.BIT_NAME.UIF=0;

}

