/*----------------------------------------------------------------------------
 *      Author: Bartlomiej Gac 2018
 *			Name:    onewire.h
 *      Purpose: SmP&DesignLaboratory Project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/

#ifndef onewire_h
#define onewire_h

#include "MKL46Z4.h"   /* Device header */

/*ROM commands*/
#define ow_CMD_SEARCHROM			 0xF0		//Master wyszukuje wszystkie kody ROM slave'ow. Przy 1 urzadzniu mozna pominac i uzyc readrom
#define ow_CMD_READROM				 0x33		//odczytanie kodu ROM z ukladu podrzednego. Dziala tylko przy 1 urzadzeniu
#define ow_CMD_MATCHROM				 0x55		//zaadresowanie do odpowiedniu ukladu podrzednego
#define ow_CMD_SKIPROM				 0xCC		//pominiecie ROMu, uzywane przy podlaczonym jednym urzadzeniu
#define ow_CMD_ALARMSEARCH		 0xEC		//Dziala tak jak searchRom, ale wyszukuje tylko te urzadzenia ktora maja flage alarmu
/*function commands*/
#define ow_CMD_CONVERT				 0x44		//Inicjalizuje pojedyncza konwersje temperatury. temp zapisywana jest w scrathpad				
#define ow_CMD_READSCRATCHPAD	 0xBE		//Odczyt ze scratchpada
#define ow_CMD_WRITESCRATCHPAD 0x4E		//Zapisywanie do scratchpada
#define ow_CMD_COPYSCRATCHPAD	 0x48		//kopiowanie ze scratchpada do EEPROM
#define ow_CMD_RECALLE2				 0xB8		//Sprawdzanie czy jakas operacja sie skonczyla?
#define ow_CMD_RPWRSUPPLY			 0xB4		//Odczytywanie, ktory z urzadzen uzywa zasilania pasozytniczego

#define DQ 9

void ow_Initialize(void);																//initialize onewire
/*GPIO FUNCTIONS*/
void ow_output(void);																		//set DQ as output
void ow_input(void);																		//set DQ as input
void ow_high(void);																			//set DQ as high (if DQ is output)																
void ow_low(void);																			//set DQ as low (if DQ is output)
uint8_t ow_readPIN(void);																//read value from DQ (if DQ is input)
/*MAIN FUNCTIONS*/
uint8_t ow_Reset(void);																	//reset onewire device
void ow_WriteBit(uint8_t bit);													//write one 'bit' to device
uint8_t ow_ReadBit(void);																//read one bit from device
void ow_WriteByte(uint8_t byte);												//write one 'byte' to device												
uint8_t ow_ReadByte(void);															//that read one byte from device
/*ADVANCED FUNCTIONS*/
void ow_ReadROM(uint8_t *address);											//read device address but only if is only one device on the bus
void ow_MatchROM (uint8_t *address);										//match master with device which address the same as in the argument
void ow_ResetSearch(void);															//parameters from search procedure
uint8_t ow_Search(uint8_t *address);										//read address from one device on the bus. If you use search procedure multiply time (without reset search) you should hava address all devices
uint8_t ow_AllSearch(uint8_t address[][8]);						  //read address from all devices on the bus

#endif
