/*----------------------------------------------------------------------------
 *      Author: Bartlomiej Gac 2018
 *			Name:    delay.h
 *      Purpose: SmP&DesignLaboratory Project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/

#ifndef delay_h
#define delay_h

#include "MKL46Z4.h"   /* Device header */

#define LPTMR_ALARM_INT_NBR (IRQn_Type) 28

void delay_initialize(void);							//initialize delay
void delay_us(uint32_t time_us);					//delay microseonds
void delay_ms(uint32_t time_ms);					//delay miliseconds

#endif
