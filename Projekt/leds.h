/*----------------------------------------------------------------------------
 *      Author: Bartlomiej Gac 2018
 *			Name:    leds.h
 *      Purpose: SmP&DesignLaboratory Project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/

#ifndef leds_h
#define leds_h

#include "MKL46Z4.h"   /* Device header */

#define ledGreen 0  //Green led is first in led_mask
#define ledRed 1  //Red led is second in led_mask

const static uint32_t led_mask[] = {1UL << 5, 1UL << 29};  //Green led is Port D bit 5, Red led is Port E bit 29

void leds_Initialize(void);											//function that initialize leds 
void leds_ActiveDevice(uint8_t active_index);		//function that light led for active device
#endif
