/*----------------------------------------------------------------------------
 *      Author: Bartlomiej Gac 2018
 *			Name:    leds.c
 *      Purpose: SmP&DesignLaboratory Project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/

#include "leds.h"												//Declarations	

/*----------------------------------------------------------------------------
  Function that initializes LEDs
 *----------------------------------------------------------------------------*/
void leds_Initialize(void){
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK; /* Enable Clock to Port D & E */ 
	PORTD->PCR[5] = PORT_PCR_MUX(1UL);  /* Pin PTD5 is GPIO */
	PORTE->PCR[29] = PORT_PCR_MUX(1UL);  /* Pin PTE29 is GPIO */
	PTD->PDDR |= led_mask[ledGreen];  /* enable PTD5 as Output */
	PTE->PDDR |= led_mask[ledRed];  /* enable PTE29 as Output */
	
	PTD->PSOR = led_mask[ledGreen];  /* switch Green LED off */
	PTE->PSOR = led_mask[ledRed];  /* switch Red LED off */
}
/*----------------------------------------------------------------------------
  Function for leds in ds18b20
 *----------------------------------------------------------------------------*/
void leds_ActiveDevice(uint8_t active_index){
	if(active_index == 0){
		PTD->PCOR = led_mask[ledGreen];      			/* switch Red LED on  */
		PTE->PSOR = led_mask[ledRed];      		  /* switch Green LED off  */
	}
	if(active_index == 1){
		PTE->PCOR = led_mask[ledRed];     				/* switch Green LED on */
		PTD->PSOR = led_mask[ledGreen];         /* switch Red LED off  */
	}
	if(active_index == 2){
		PTE->PCOR = led_mask[ledRed];     				/* switch Green LED on */
		PTD->PCOR = led_mask[ledGreen];         /* switch Red LED off  */
	}
}
