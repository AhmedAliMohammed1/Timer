/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif
#include "Timer1_8.h"
#include "GPIO_Driver.h"
#include "LCD.h"
	uint32_t sec_s;
void Toggel(){
	//MCAL_TOGGLE_PIN(GPIOA, PIN_0);
	sec_s++;
}
int main(void)
{
	RCC->APB2ENR |=(1<<2);
	PIN_config PIN_Config={PIN_9,OUTPUT_PP,SPEED_2};
	MCAL_GPIO_init(GPIOA, &PIN_Config);
	PIN_Config=(PIN_config){PIN_10,OUTPUT_PP,SPEED_2};
	MCAL_GPIO_init(GPIOA, &PIN_Config);
	MCAL_write_PIN(GPIOA, PIN_9, 0);
	MCAL_write_PIN(GPIOA, PIN_10, 1);



	Timer2_init();
	LCD_init();
	TIMERx_type SIT={799,83,0,83,INT_EN,Compare_output,Toggel};
	Compare_mode_sitting CTC_Sitting={PWM_AL,CTC_Preload_DIS,ACTIVE_H};
//	PIN_config PIN={PIN_0,OUTPUT_PP,SPEED_10};
//	MCAL_GPIO_init(GPIOA, &PIN);
	TIMER_1_CTC_start(TIM1, &SIT, &CTC_Sitting);
	/* Loop forever */
	for(;;){

//		for(unsigned int  i=1000;i<=2000;i+=6){
//		TIMERx_type SIT={7,20000,0,i,INT_EN,Compare_output,Toggel};
//		TIMER_1_8_start(TIM1,&SIT);
//		dms(100);
//		}

		//		MCAL_TOGGLE_PIN(GPIOA, PIN_0);
		LCD_clearScreen();
		LCD_intgerToString(sec_s);

	}
}

void Servo_deg(uint32_t deg){
	deg=((deg*555)+100000)/100;
	TIMERx_type SIT={7,20000,0,(deg),INT_DIS,Compare_output,0};

}

