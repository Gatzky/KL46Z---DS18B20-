/*----------------------------------------------------------------------------
 *      Author:  Bartlomiej Gac 2018
 *			Name:    onewire.c
 *      Purpose: SmP&DesignLaboratory Project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/

#include "onewire.h"						//Declaration

uint8_t LastDiscrepancy;       //Czyli gdy jedne urzadzenia wysylaja 1 a inne 0
uint8_t LastFamilyDiscrepancy; /*!< Search private */
uint8_t LastDeviceFlag;        // flaga, 1 jesli poprzedni sprawdzany device byl ostatni
uint8_t ROM_NO[8];             /*!< 8-bytes address of last search device */ 

void delay_us(uint32_t time_us);
//
//**********************************GPIO FUNCTIONS**********************************
//
/*----------------------------------------------------------------------------
  Function that initializes ow
 *----------------------------------------------------------------------------*/
void ow_Initialize(void){
	SIM->SCGC5 |=  SIM_SCGC5_PORTC_MASK;		//Enable clock for PORTC
	PORTC->PCR[9] |= PORT_PCR_MUX(1);				//Set PTC9 (D7) as GPIO
}
/*----------------------------------------------------------------------------
  Function that changes DQ to input
 *----------------------------------------------------------------------------*/
void ow_input(void){
	PTC->PDDR &= ~(1 << DQ);
	PORTC->PCR[DQ] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; /* TASK 2.9 - enable on PTC3 pull resistor, and choose correct pull option */
}
/*----------------------------------------------------------------------------
  Function that changes DQ to output
 *----------------------------------------------------------------------------*/
void ow_output(void){
	PTC->PDDR |= (1 << DQ);
}
/*----------------------------------------------------------------------------
  Function that changes DQ(output) to high
 *----------------------------------------------------------------------------*/
void ow_high(){
	PTC->PSOR = (1UL << DQ);
}
/*----------------------------------------------------------------------------
  Function that changes DQ(output) to low
 *----------------------------------------------------------------------------*/
void ow_low(){
	FPTC->PCOR = (1UL << DQ);
}
/*----------------------------------------------------------------------------
  Function that reads from DQ PIN (input)
 *----------------------------------------------------------------------------*/
uint8_t ow_readPIN(void){
	if (PTC->PDIR & (1UL << DQ))
		return 1;
	else
		return 0;
}
//
//**********************************MAIN FUNCTIONS**********************************
//

/*----------------------------------------------------------------------------
  Function that resets device and check that any device is in data buffer
 *----------------------------------------------------------------------------*/
uint8_t ow_Reset(void) {
	uint8_t i;
	
	/* Line low, and wait 480us */
	ow_low();
	ow_output();
	delay_us(480);
	
	/* Release line and wait for 70us */
	ow_input();
	delay_us(70);
	
	/* Check bit value */
	i = ow_readPIN();
	
	/* Delay for 410 us */
	delay_us(410);
	
	/* Return value of presence pulse, 0 = OK, 1 = ERROR */
	return i;
}

/*----------------------------------------------------------------------------
  Function that writes one bit 
 *----------------------------------------------------------------------------*/
void ow_WriteBit(uint8_t bit) {
	if (bit & 1UL) {
		/*Bit low*/
		ow_low();
		ow_output();
			delay_us(6);			//10
		
		/* Bit high */
		ow_high();
		
		delay_us(53);				//60
	} 
	else {
		/*Bit low*/
		ow_low();
		ow_output();
		delay_us(60);				//64
		
		/* Bit high */
		ow_high();
		
		delay_us(3);				//6
	}
}
/*----------------------------------------------------------------------------
  Function that reads one bit 
 *----------------------------------------------------------------------------*/
uint8_t ow_ReadBit(void) {
	uint8_t bit = 0;
	
	/* Line low */
	ow_output();
	ow_low();
	delay_us(2);								//6
	
	/* Release line */
	ow_input();
	delay_us(6);								//10
	
	/* Read line value */
	bit = ow_readPIN();
	
	/* Wait to complete period */
	delay_us(51);								//55
	
	/* Return bit value */
	return bit;
}
/*----------------------------------------------------------------------------
  Function that writes one byte, 8 bits
 *----------------------------------------------------------------------------*/
void ow_WriteByte(uint8_t byte) {
	uint8_t i = 8;
	/* Write 8 bits */
	while (i--) {
		/* LSB bit is first */
		ow_WriteBit(byte & 1);
		byte >>= 1;
	}
}
/*----------------------------------------------------------------------------
  Function that reads one byte, 8 bits
 *----------------------------------------------------------------------------*/
uint8_t ow_ReadByte(void) {
	uint8_t i = 8, byte = 0;
	while (i--) {
		byte >>= 1;												//right shift byte for 1 bit
		byte |= (ow_ReadBit() << 7);
	}
	return byte;
}
//
//**********************************ADVANCED FUNCTION**********************************
//
/*----------------------------------------------------------------------------
  Function for write matchrom command and write device address
 *----------------------------------------------------------------------------*/
void ow_MatchROM (uint8_t *address){
	uint8_t i;
	ow_WriteByte(ow_CMD_MATCHROM);
	for(i=0; i<8; i++){
		ow_WriteByte(address[i]);
	}
}
/*----------------------------------------------------------------------------
  Function that get device rom, but only when only one device is on the bus
 *----------------------------------------------------------------------------*/
void ow_ReadROM(uint8_t *address){
	uint8_t i;
	if(!ow_Reset()){
		ow_WriteByte(ow_CMD_READROM);
		for (i = 0; i<8; i++){
			address[i] = ow_ReadByte();
		}
	}
}
/*----------------------------------------------------------------------------
  Function that resets search state
 *----------------------------------------------------------------------------*/
void ow_ResetSearch(void)  {
	/* Reset the search state */
	LastDiscrepancy = 0;
	LastDeviceFlag = 0;
	LastFamilyDiscrepancy = 0;
}
/*----------------------------------------------------------------------------
  Function that search ROM in 1wire bus
 *----------------------------------------------------------------------------*/
uint8_t ow_Search(uint8_t *address){
   uint8_t id_bit_number;									//numer ROMu ktory aktualnie szukamy
   uint8_t last_zero, rom_byte_number;
   uint8_t id_bit, cmp_id_bit;						//bity odbierane z devices. pierwszy normalny i drugi complement
   unsigned char rom_byte_mask, search_direction;

   // initialize for search
   id_bit_number = 1;							
   last_zero = 0;
   rom_byte_number = 0;						
   rom_byte_mask = 1;
   //crc8 = 0;

   // if the last call was not the last one
   if (!LastDeviceFlag){
			//jezeli reset nie wyjdzie zrób ow_reset
		  if (ow_Reset()){
         ow_ResetSearch();
         return 0;
      }
			
      ow_WriteByte(ow_CMD_SEARCHROM);  										// issue the search command 

      do{   
         id_bit = ow_ReadBit();														// read a bit and its complement
         cmp_id_bit = ow_ReadBit();
				
				 // 1 & 1 mean: no device on the bus
         if ((id_bit == 1) && (cmp_id_bit == 1))					
            break;

         else{
            if (id_bit != cmp_id_bit)											//normal read from bit
               search_direction = id_bit;  								// bit write value for search
            else{
               if (id_bit_number < LastDiscrepancy)													// if this discrepancy if before the Last Discrepancy
                  search_direction = ((address[rom_byte_number] & rom_byte_mask) > 0);	// on a previous next then pick the same as last time
               else
                  
                  search_direction = (id_bit_number == LastDiscrepancy);		// if equal to last pick 1, if not then pick 0

               if (search_direction == 0){																	// if 0 was picked then record its position in LastZero
                  last_zero = id_bit_number;
								 
                  if (last_zero < 9)											// check for Last discrepancy in family
                     LastFamilyDiscrepancy = last_zero;
               }
            }

            // set or clear the bit in the ROM byte rom_byte_number
            // with mask rom_byte_mask
            if (search_direction == 1)
              address[rom_byte_number] |= rom_byte_mask;
            else
              address[rom_byte_number] &= ~rom_byte_mask;

            // serial number search direction write bit
            ow_WriteBit(search_direction);

            // increment the byte counter id_bit_number
            // and shift the mask rom_byte_mask
            id_bit_number++;
            rom_byte_mask <<= 1;

            // if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
            if (rom_byte_mask == 0){
                //docrc8(ROM_NO[rom_byte_number]);  // accumulate the CRC
                rom_byte_number++;
                rom_byte_mask = 1;
            }
         }
			 }while(rom_byte_number < 8);  							// loop until through all ROM bytes 0-7

      // if the search was successful then (all 64 bit_number and crc)
      //if (!((id_bit_number < 65) /*|| (crc8 != 0)*/)){
			if(id_bit_number > 64){
				 LastDiscrepancy = last_zero;
         if (LastDiscrepancy == 0){
            LastDeviceFlag = 1;
				 }
         return 1;
      }
   }
	 return 0;
}
/*----------------------------------------------------------------------------
  Function that search ROM all devices on bus
 *----------------------------------------------------------------------------*/
uint8_t ow_AllSearch(uint8_t address[][8]){
	uint8_t i=0;
	ow_ResetSearch();
	while(ow_Search(address[i])){
		i++;
	}
	return i;
}
