/*----------------------------------------------------------------------------
 *      Author: Bartlomiej Gac 2018
 *      Name:    main.c
 *      Purpose: Microprocessors Laboratory
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/

#include "MKL46Z4.h"                    	/* Device header */
#include "leds.h"													/* Leds functions for FRDM46-KL46 */
#include "buttons.h"											/* Buttons of FRDM46-KL46 */
#include "slcd.h"											    /* Segment LCD of FRDM46-KL46 */
#include "onewire.h"
#include "ds18b20.h"
#include "delay.h"
#include "uart.h"

void leds_ActiveDevice(uint8_t active_index);
void slcdDS(void);
uint16_t ds_ReadTempMatch(uint8_t index);
uint8_t ds_SetResMatch(uint8_t res, uint8_t index);
void ds_Initialize(void);
uint8_t ds_RequestTempMatch(uint8_t index);
void uart_Write(uint8_t value);
void uart_DS(void);

void initialize(void){
	slcd_Initialize();
	leds_Initialize();
	ow_Initialize();
	delay_initialize();
	buttons_Initialize();
	ds_Initialize();
	uart_Initialize();
}

int main(void){
	initialize();
 	uart_DS();
	ds_SetResMatch(9,1);
	ds_SetResMatch(12,2);
	
	while(1){
		leds_ActiveDevice(buttons_ReturnActiveDevice());
		ds_RequestTempMatch(buttons_ReturnActiveDevice());
		slcd_DS18B20(ds_ReadTempMatch(buttons_ReturnActiveDevice()));
	}
}
