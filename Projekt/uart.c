 /*----------------------------------------------------------------------------
 *      Name:    uart.c
 *      Purpose: Microprocessors Laboratory
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/
 #include "uart.h"
 
uint8_t ds_ReturnCellAddress(uint8_t address_index, uint8_t cell_index);
uint8_t ds_ReturnDeviceNumber(void);
/*----------------------------------------------------------------------------
  Function for initalize uart
 *----------------------------------------------------------------------------*/
 void uart_Initialize(void){
	 SIM->SCGC4|= SIM_SCGC4_UART0_MASK;					//Clock for UART
	 SIM->SCGC5|= SIM_SCGC5_PORTA_MASK;
	 PORTA->PCR[1]|= PORT_PCR_MUX(2);						//Set MUX
	 PORTA->PCR[2]|= PORT_PCR_MUX(2);
	 
	 SIM->SOPT2 |= SIM_SOPT2_UART0SRC(2);				//Clock source
	 
	 UART0->C2 &= ~(0x4);											// Receiver enabled
	 UART0->C2 &= ~(0x8);											// Transmitter enabled
	 UART0->C4 |= 0x1F;	
	 
	 UART0->BDH = 0x00;
	 UART0->BDL = 0x19;
	 
	 UART0->BDH &= ~UART0_BDH_SBNS_MASK;
	 UART0->C1 &= ~(UART0_C1_M_MASK|UART0_C1_PE_MASK);//no parrity control 8 bit data
	 
	 UART0->C1 &= ~UART0_C1_PE_MASK;
	 UART0->C2 |= UART0_C2_RE_MASK | UART0_C2_TE_MASK; 
} 

/*----------------------------------------------------------------------------
  Function that send one value by the uart
 *----------------------------------------------------------------------------*/
void uart_Send(uint8_t value){
		if(UART0->S1&UART0_S1_TDRE_MASK)
			UART0->D = value;
		delay_ms(20);
}
 
/*----------------------------------------------------------------------------
  Function that send address all onewire devices on the bus
 *----------------------------------------------------------------------------*/
void uart_DS(void){
	uint8_t i,j;
	
	for (i=0; i<ds_ReturnDeviceNumber(); i++){
		for(j=0; j<8; j++){
			uart_Send(ds_ReturnCellAddress(i,j));
		}
		uart_Send(0xFF);
		uart_Send(0x00);		
	}
}
