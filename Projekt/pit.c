/*----------------------------------------------------------------------------
 *      Author: Bartlomiej Gac 2018
 *			Name:    pit.c
 *      Purpose: SmP&DesignLaboratory Project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/
#include "pit.h"

void ledgreenBlink(uint32_t x, uint32_t y);

/*----------------------------------------------------------------------------
  initialize timer for periodic interrupt
 *----------------------------------------------------------------------------*/
void PitInitialize(void){
	
  SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; //clock to PIT
  PIT->MCR = 0x00;   //enable PIT module

	//uruchomienie timerów
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;
	PIT->MCR |= PIT_MCR_FRZ_MASK;	
	
	//zaladowanie wartosci
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(0x00005000);
	
	//uruchomienie przerwania
	PIT->CHANNEL[0].TCTRL |=PIT_TCTRL_TIE_MASK;	
	
	//ustawienie priorytetu i wyczyszcenie przerwan  1.9
	NVIC_SetPriority(PIT_IRQn,2);
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_EnableIRQ(PIT_IRQn);

	//rozpoczecie odliczania
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
}

/*----------------------------------------------------------------------------
  timer's handler
 *----------------------------------------------------------------------------*/
void PIT_IRQHandler(){
	//wyczyszczenie innych przerwan
	NVIC_ClearPendingIRQ(PIT_IRQn);
	//sprawdzenie, na którym kanale jest przerwanie:
	
	//clearing status flag for this channel:
	PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
	//SOMETHING	
}
