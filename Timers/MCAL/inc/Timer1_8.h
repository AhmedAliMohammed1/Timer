/*
 * Timer1_8.h
 *
 *  Created on: Oct 13, 2023
 *      Author: medoo
 */

#ifndef INC_TIMER1_8_H_
#define INC_TIMER1_8_H_
#include "STM32F103x6.h"
#include "GPIO_Driver.h"
typedef enum{
	Up, /*If the repetition counter is used, the update event (UEV) is generated after upcounting is
		repeated for the number of times programmed in the repetition counter register plus one
		(TIMx_RCR+1). Else the update event is generated at each counter overflow.
	 	 */
	DOWN,
	Up_Down
	,Compare_output

}Counter_Modes;
typedef enum{
	INT_EN,INT_DIS
}INTERPUT;
typedef enum{
	Frozen=0,
	active,
	inactive,
	Toggle,
	PWM_AH=6 ,//PWM ACTIVE HIGH
	PWM_AL
}Output_Compare_mode;
typedef enum{
	CTC_Preload_DIS,CTC_Preload_EN
}Output_Compare_preload_enable;
typedef enum{
	ACTIVE_H,ACTIVE_L
}Output_Compare_POL;
typedef struct{
	uint16_t TIME_PSC;
	uint16_t TIME_ARR; // = THE TOP VALUE THAT THE COUNTER REACH
	uint16_t TIME_RCR;
	uint16_t TIME_CCR;
	INTERPUT INT_EN_DIS;
	Counter_Modes mode;
	void (*f_callback)(void);


}TIMERx_type;


typedef struct{
	Output_Compare_mode TIM1_CTC_MD;
	Output_Compare_preload_enable TIM1_CTC_PE;
	Output_Compare_POL TIM1_POL;
}Compare_mode_sitting;



void TIMER_1_8_start(TIM1_8_REG* TIMx,TIMERx_type* Sitting);
void TIMER_1_CTC_start(TIM1_8_REG* TIMx,TIMERx_type* Sitting,Compare_mode_sitting* CTC_Sitting);

#endif /* INC_TIMER1_8_H_ */
