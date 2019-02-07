/*----------------------------------------------------------------------------
 *      Author: Bartlomiej Gac 2018
 *			Name:    ds18b20.c
 *      Purpose: SmP&DesignLaboratory Project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/

#include "ds18b20.h"					//Declaration

uint8_t address[12][8];
uint8_t resolution[12];
uint8_t address_skip;
uint8_t resolution_skip;
uint8_t device_number;

void ow_MatchROM (uint8_t *address);

/*----------------------------------------------------------------------------
  Function for initalize ds library
 *----------------------------------------------------------------------------*/
void ds_Initialize(void){
	device_number = ow_AllSearch(address);
}
/*----------------------------------------------------------------------------
  Function for send resolution/quality of measure (9 bits, 10 bits, 11 bits or 12 bits) which skip adress
 *----------------------------------------------------------------------------*/
uint8_t ds_SetResSkip(uint8_t res){
	resolution_skip = res;
	
	res -=9;																		//0 for 9, 1 for 10, etc.
	res <<= 5;			
	res |= 0x1F;																//0b00011111
	
	if(!ow_Reset()){
		ow_WriteByte(ow_CMD_SKIPROM);							//send address: skip rom (only one device on bus)
		ow_WriteByte(ow_CMD_WRITESCRATCHPAD);			//send command: write scratchpad 
		ow_WriteByte(0);
		ow_WriteByte(0);
		ow_WriteByte(res);
		return 1;
	}
	else{
		return 0;
	}
}
/*----------------------------------------------------------------------------
  Function for get convertion time (is depend by the resolution)
 *----------------------------------------------------------------------------*/
int ds_GetConvTime(uint8_t res){
	switch(res){
		case 9: {
			return 94;
		}
		case 10: {
			return 188;
		}
		case 11: {
			return 375;
		}
		default: {
			return 750;
		}
	}
}
/*----------------------------------------------------------------------------
  Function for request for temparature (send "Convert" command) which skip addres (one device on bus)
 *----------------------------------------------------------------------------*/
uint8_t ds_RequestTempSkip(void){
	if(!ow_Reset()){
		ow_WriteByte(ow_CMD_SKIPROM);						//send address: skip rom (only one device on bus)
		ow_WriteByte(ow_CMD_CONVERT);						//send command: convert measure 
		delay_ms(ds_GetConvTime(resolution_skip)); 													//wait until conversion is complete
		return 1;
	}
	else{
		return 0;
	}
}
/*----------------------------------------------------------------------------
  Function for read temperature which skip addres (one device on bus)
 *----------------------------------------------------------------------------*/
uint16_t ds_ReadTempSkip(void){
	uint8_t temp_l, temp_h;
	
	if(!ow_Reset()){
		ow_WriteByte(ow_CMD_SKIPROM);						//send address: skip rom (only one device on bus)
		ow_WriteByte(ow_CMD_READSCRATCHPAD);		//send command: read from scratchpad (where are temperature measure) 
		temp_l = ow_ReadByte();
		temp_h = ow_ReadByte();
	}
	else{
		return 0xFFFF;
	}
	
	return (temp_h << 8) | temp_l;	
}

/*----------------------------------------------------------------------------
  Function for calculate value of temperature from bytes (uint16_t) to Celcius
 *----------------------------------------------------------------------------*/
float ds_BytestoC(uint16_t value, uint8_t res){
  float quality[] = {0.5, 0.25, 0.125, 0.0625};
  value >>= (12-res);

  return value * quality[res-9];
}
/*----------------------------------------------------------------------------
  Function for send resolution/quality of measure (9 bits, 10 bits, 11 bits or 12 bits) which match adress
 *----------------------------------------------------------------------------*/
uint8_t ds_SetResMatch(uint8_t res, uint8_t index){
	resolution[index] = res;
	
	res -=9;																		//0 for 9, 1 for 10, etc.
	res <<= 5;			
	res |= 0x1F;																//0b00011111
	
	if(!ow_Reset()){
		ow_MatchROM(address[index]);											//send address: skip rom (only one device on bus)
		ow_WriteByte(ow_CMD_WRITESCRATCHPAD);			//send command: write scratchpad 
		ow_WriteByte(0);
		ow_WriteByte(0);
		ow_WriteByte(res);
		return 1;
	}
	else{
		return 0;
	}
}

/*----------------------------------------------------------------------------
  Function for request for temparature (send "Convert" command) which match addres
 *----------------------------------------------------------------------------*/
uint8_t ds_RequestTempMatch(uint8_t index){
	if(!ow_Reset()){
		ow_MatchROM(address[index]);
		ow_WriteByte(ow_CMD_CONVERT);														//send command: convert measure 
		delay_ms(ds_GetConvTime(resolution[index])); 						//wait until conversion is complete
		return 1;
	}
	else{
		return 0;
	}
}
/*----------------------------------------------------------------------------
  Function for read temperature which match addres (one device on bus)
 *----------------------------------------------------------------------------*/
uint16_t ds_ReadTempMatch(uint8_t index){
	uint8_t temp_l, temp_h;
	
	if(!ow_Reset()){
		ow_MatchROM(address[index]);																//send address: match rom by device index
		ow_WriteByte(ow_CMD_READSCRATCHPAD);								//send command: read from scratchpad (where are temperature measure) 
		temp_l = ow_ReadByte();
		temp_h = ow_ReadByte();
	}
	else{
		return 0x0FFF;
	}
	
	return (temp_h << 8) | temp_l;
}
/*----------------------------------------------------------------------------
  Function that return one cell from one address
 *----------------------------------------------------------------------------*/
uint8_t ds_ReturnCellAddress(uint8_t address_index, uint8_t cell_index){
	return address[address_index][cell_index];
}
/*----------------------------------------------------------------------------
  Function that return device_number
 *----------------------------------------------------------------------------*/
uint8_t ds_ReturnDeviceNumber(void){
	return device_number;
}
