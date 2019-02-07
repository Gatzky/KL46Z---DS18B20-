/*----------------------------------------------------------------------------
 *      Author:  Bartlomiej Gac 2018
 *			Name:    button.c
 *      Purpose: SmP&DesignLaboratory Project
 *----------------------------------------------------------------------------*/

#include "buttons.h"

uint8_t active_index;

/*----------------------------------------------------------------------------
	Interrupt service routine
	Button will cause PORTC_PORTD interrupt 
 *----------------------------------------------------------------------------*/
void PORTC_PORTD_IRQHandler(void){		/* ToDo 5.2: Put a proper name of PORTC_PORTD Interrupt service routine ISR. See system_MKL46Z4.s file for function name */ 
	
	buttons_Service();
	
	while((!FPTC->PDIR & (1<<SW1_PIN)) | (!FPTC->PDIR & (1<<SW3_PIN)));				/* ToDo 7.1: Enable wait in the interrupt for SW1 button release */
	
	PORTC->PCR[SW3_PIN] |= PORT_PCR_ISF_MASK;
	PORTC->PCR[SW1_PIN] |= PORT_PCR_ISF_MASK; 		/* ToDo 5.3: Clear interrupt service flag in port control register otherwise int. remains active */
}

/*----------------------------------------------------------------------------
	Function initializes port C pin for switch 1 (SW1) handling
	and enables PORT_C_D interrupts
 *----------------------------------------------------------------------------*/
void buttons_Initialize(void){
	active_index = 0;
	
	SIM->SCGC5 |=  SIM_SCGC5_PORTC_MASK; 					/* Enable clock for port C */
	PORTC->PCR[SW1_PIN] |= PORT_PCR_MUX(1);      	/* Pin PTC3 is GPIO */
	
	/* Port control register for bit 3 of port C configuration. Activate pull up and interrupt */
	PORTC->PCR[SW1_PIN] |=  PORT_PCR_PE_MASK |		/* ToDo 5.4: Set bit in PCR register to enable pull resistor. See KL46 Sub-Family Reference Manual */
													PORT_PCR_PS_MASK;		/* ToDo 5.5: Set bit in PCR register to select pull up. See KL46 Sub-Family Reference Manual */
	PORTC->PCR[SW1_PIN] |= 	PORT_PCR_IRQC(0010);		/* ToDo 5.6: Set value for IRQC bit field in PCR register to select falling edge interrupts for PORTC[3]. See KL46 Sub-Family Reference Manual */
	
	PORTC->PCR[SW3_PIN] |= PORT_PCR_MUX(1);      	/* Pin PTC3 is GPIO */
	
	/* Port control register for bit 3 of port C configuration. Activate pull up and interrupt */
	PORTC->PCR[SW3_PIN] |=  PORT_PCR_PE_MASK |		/* ToDo 5.4: Set bit in PCR register to enable pull resistor. See KL46 Sub-Family Reference Manual */
													PORT_PCR_PS_MASK;		/* ToDo 5.5: Set bit in PCR register to select pull up. See KL46 Sub-Family Reference Manual */
	PORTC->PCR[SW3_PIN] |= 	PORT_PCR_IRQC(0010);
	
	/*ARM's Nested Vector Interrupt Controller configuration*/
	NVIC_ClearPendingIRQ(PORTC_D_IRQ_NBR);				/* Clear NVIC any pending interrupts on PORTC_D */
	NVIC_EnableIRQ(PORTC_D_IRQ_NBR);							/* Enable NVIC interrupts source for PORTC_D module */
	
	NVIC_SetPriority (PORTC_D_IRQ_NBR, 4);			/* ToDo 7.2a: Set PORTC_D interrupt priority level  */ 
}
/*----------------------------------------------------------------------------
	Function services PORT_C_D interrupt
*----------------------------------------------------------------------------*/
void buttons_Service(void){
	if (!(FPTC->PDIR&(1<<SW1_PIN))){
		active_index++;
		if (active_index > 2)
			active_index = 0;
	}
	//if (!(FPTC->PDIR&(1<<SW3_PIN)))
		//something
}
/*----------------------------------------------------------------------------
	Function for return active_index
*----------------------------------------------------------------------------*/
uint8_t buttons_ReturnActiveDevice(void){
	return active_index;
}
