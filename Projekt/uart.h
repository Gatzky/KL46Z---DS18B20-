 /*      
 *      Author: Mateusz Gawron i Tomasz Cygan AGH University of Science and Technology
 *      Name:    uart.h
 *      Purpose: Microprocessors Laboratory
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/
 
#ifndef uart_h
#define uart_h
 
#include "MKL46Z4.h"
#include "delay.h"
 
void uart_Initialize(void);								//initialize uart
void uart_send(uint8_t value);						//send value by the uart
void uart_DS(void);												//send address of ds18b20 by the uart

#endif
