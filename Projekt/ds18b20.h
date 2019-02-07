/*----------------------------------------------------------------------------
 *      Author: Bartlomiej Gac 2018
 *			Name:    ds18b20.c
 *      Purpose: SmP&DesignLaboratory Project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/

#ifndef ds18b20_h
#define ds18b20_h

#include "MKL46Z4.h"   /* Device header */
#include "onewire.h"
#include "delay.h"

/*ROM commands*/
#define ow_CMD_SEARCHROM			 0xF0		//Master wyszukuje wszystkie kody ROM slave'ow. Przy 1 urzadzniu mozna pominac i urzyc readrom
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

void ds_Initialize(void);																		//initialize ds18b20
/*SKIP FUNCTIONS*/
uint8_t ds_SetResSkip(uint8_t res);													//set resolution with skip command (only one device on bus)
uint8_t ds_RequestTempSkip(void);														//request temperature with skip command (only one device on bus)
uint16_t ds_ReadTempSkip(void);															//read temperature with skip command (only one device on bus)
/*MATCH FUNCTIONS*/
uint8_t ds_SetResMatch(uint8_t res, uint8_t index);					//set resolution with match command
uint8_t ds_RequestTempMatch(uint8_t index);									//request temperature with match command
uint16_t ds_ReadTempMatch(uint8_t index);										//read temperature with match command
/*OTHER FUNCTIONS*/
int ds_GetConvTime(uint8_t res);														//return conversion time (it depends by the resolution)
float ds_BytestoC(uint16_t value, uint8_t res);							//convert temperature from uint16_t to float
uint8_t ReturnCellAddress(uint8_t address_index, uint8_t cell_index);	//return value from one cell from one address
uint8_t ReturnDeviceNumber(void);														//return number of device on the bus

#endif
