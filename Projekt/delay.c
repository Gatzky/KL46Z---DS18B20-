/*----------------------------------------------------------------------------
 *      Author: Bartlomiej Gac 2018
 *			Name:    delay.c
 *      Purpose: SmP&DesignLaboratory Project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/

#include "delay.h"						//Declaration

uint32_t TimingDelay;

/*----------------------------------------------------------------------------
  Function for initialize systick
 *----------------------------------------------------------------------------*/
void delay_initialize(){
  SysTick_Config(48);
}

/*----------------------------------------------------------------------------
  Function for start microseconds delay, in argument how many us delay will have
 *----------------------------------------------------------------------------*/
void delay_us(uint32_t time_us){ 
	TimingDelay = time_us;
  while(TimingDelay != 0);
}

/*----------------------------------------------------------------------------
  Function for start miliseconds delay, in argument how many ms delay will have
 *----------------------------------------------------------------------------*/
void delay_ms(uint32_t time_ms){ 
	TimingDelay = 1000 * time_ms;
  while(TimingDelay != 0);
}
 
/*----------------------------------------------------------------------------
  Systick handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void){
	if (TimingDelay != 0)
    TimingDelay--;
}
