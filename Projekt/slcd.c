/*----------------------------------------------------------------------------
 *      Author: Bartlomiej Gac 2018
 *			Name:    slcd.c
 *      Purpose: SmP&DesignLaboratory Project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/

#include "slcd.h"												//Declarations

void delay_ms(uint32_t time_ms);
/*----------------------------------------------------------------------------
  Function that initializes sLCD
 *----------------------------------------------------------------------------*/
void slcd_Initialize(void){
	
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | /* Enable Clock to Port B & C */ 
								SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK | SIM_SCGC5_SLCD_MASK; /* Enable Clock to Port D & E and sLCD module*/ 
	LCD->GCR |= LCD_GCR_PADSAFE_MASK; // Set PADSAFE during configuration
	LCD->GCR &= ~LCD_GCR_LCDEN_MASK; // Clear LCDEN during configuration
	//multiplexers configuration for ports to act as sLCD controller
	
	//TASK 2.3 - replace XXX with pin number, and YYY with proper alternative number
	PORTB->PCR[7] = PORT_PCR_MUX(0UL);
	PORTB->PCR[8] = PORT_PCR_MUX(0UL);
	PORTB->PCR[10] = PORT_PCR_MUX(0UL);
	PORTB->PCR[11] = PORT_PCR_MUX(0UL);
	PORTB->PCR[21] = PORT_PCR_MUX(0UL);
	PORTB->PCR[22] = PORT_PCR_MUX(0UL);
	PORTB->PCR[23] = PORT_PCR_MUX(0UL);
	PORTC->PCR[17] = PORT_PCR_MUX(0UL);
	PORTC->PCR[18] = PORT_PCR_MUX(0UL);
	PORTD->PCR[0] = PORT_PCR_MUX(0UL);
	PORTE->PCR[4] = PORT_PCR_MUX(0UL);
	PORTE->PCR[5] = PORT_PCR_MUX(0UL);
	
	//sLCD register configuration
	LCD->GCR = LCD_GCR_RVTRIM(0x00) | LCD_GCR_CPSEL_MASK | LCD_GCR_LADJ(0x03) |
						 LCD_GCR_VSUPPLY_MASK | LCD_GCR_ALTDIV(0x00) |
						 LCD_GCR_SOURCE_MASK | LCD_GCR_LCLK(0x01) | LCD_GCR_DUTY(0x03);
	//sLCD blinking configuration
	LCD->AR = LCD_AR_BRATE(0x03);	
	//FDCR register configuration
	LCD->FDCR = 0x00000000;
  //activation of 12 pins to control sLCD (2 registers, 32 bits each)
	LCD->PEN[0] = LCD_PEN_PEN(1u<<7) | // LCD_P7
								LCD_PEN_PEN(1u<<8) | // LCD_P8
								LCD_PEN_PEN(1u<<10) | // LCD_P10 
								LCD_PEN_PEN(1u<<11) | // LCD_P11
								LCD_PEN_PEN(1u<<17) | // LCD_P17
								LCD_PEN_PEN(1u<<18) | // LCD_P18
								LCD_PEN_PEN(1u<<19); // LCD_P19
	LCD->PEN[1] = LCD_PEN_PEN(1u<<5) | // LCD_P37
								LCD_PEN_PEN(1u<<6) | // LCD_P38
								LCD_PEN_PEN(1u<<8) | // LCD_P40
								LCD_PEN_PEN(1u<<20) | // LCD_P52
								LCD_PEN_PEN(1u<<21); // LCD_P53
	//configuration of 4 back plane pins (2 registers, 32 bits each)
	LCD->BPEN[0] = LCD_BPEN_BPEN(1u<<18) | //LCD_P18
								 LCD_BPEN_BPEN(1u<<19); //LCD_P19
	LCD->BPEN[1] = LCD_BPEN_BPEN(1u<<8) | //LCD_P40
								 LCD_BPEN_BPEN(1u<<20); //LCD_P52

	// waveform registers configuration – 4 active (because of 4 back planes)
	// (44.3.7 in KL46 Reference Manual)
	LCD->WF[4] =  LCD_WF_WF16(0x00) | LCD_WF_WF17(0x00) |
							  LCD_WF_WF18(0x88) | // COM3 (10001000)
							  LCD_WF_WF19(0x44);  // COM2 (01000100)
	LCD->WF[10] = LCD_WF_WF40(0x11) | // COM0 (00010001)
								LCD_WF_WF41(0x00) | LCD_WF_WF42(0x00) | LCD_WF_WF43(0x00);
	
	LCD->WF[13] = LCD_WF_WF52(0x22) | // COM1 (00100010)
								LCD_WF_WF53(0x00) | LCD_WF_WF54(0x00) | LCD_WF_WF55(0x00);
	
	LCD->GCR &= ~LCD_GCR_PADSAFE_MASK; // Clear PADSAFE at the end of configuration
	LCD->GCR |= LCD_GCR_LCDEN_MASK; // Set LCDEN at the end of configuration
}
/*----------------------------------------------------------------------------
  Function that outputs on sLCD (number) error message for debug purpose
 *----------------------------------------------------------------------------*/
void slcd_Err(uint8_t number){
	LCD->WF8B[LCD_Front_Pin[0]] = (LCD_S_G | LCD_S_E | LCD_S_D | LCD_S_F); //1st digit
	LCD->WF8B[LCD_Front_Pin[1]] = (LCD_S_A);
	
	LCD->WF8B[LCD_Front_Pin[2]] = (LCD_S_E | LCD_S_G); //2nd digit
	LCD->WF8B[LCD_Front_Pin[3]] = (LCD_C);
	
	LCD->WF8B[LCD_Front_Pin[4]] = (LCD_S_E | LCD_S_G); //3rd digit
	LCD->WF8B[LCD_Front_Pin[5]] = (LCD_C);
	
	switch(number){  //4 digit depends on provided number
		case 0x00:
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_S_D | LCD_S_E | LCD_S_F); /* To display '0' we must active segments: d, e & f on first front plane */
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_S_A | LCD_S_B | LCD_S_C); /* To display '0' we must active segments: a, b & c on second front plane */
			break;
		case 0x01:
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_C);  //TASK 2.5 - replace ZZZ with segment definition
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_S_B | LCD_S_C);  //TASK 2.5 - replace ZZZ with segment definition
			break;
		case 0x02:
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_S_G | LCD_S_E | LCD_S_D );  //TASK 2.5 - replace ZZZ with segment definition
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_S_A | LCD_S_B);  //TASK 2.5 - replace ZZZ with segment definition
			break;
		case 0x03:
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_S_G | LCD_S_D);  //TASK 2.5 - replace ZZZ with segment definition
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_S_A | LCD_S_B | LCD_S_C);  //TASK 2.5 - replace ZZZ with segment definition
			break;
		case 0x04:
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_S_G | LCD_S_F);
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_S_B | LCD_S_C);
			break;
		case 0x05:
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_S_G | LCD_S_F | LCD_S_D);
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_S_A | LCD_S_C);
			break;
		case 0x06:
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_S_G | LCD_S_E | LCD_S_D  | LCD_S_F);
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_S_A | LCD_S_C);
			break;
		case 0x07:
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_C);
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_S_A | LCD_S_B | LCD_S_C);
			break;
		case 0x08:
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_S_G | LCD_S_E | LCD_S_D  | LCD_S_F);
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_S_A | LCD_S_B | LCD_S_C);
			break;
		case 0x09:
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_S_F | LCD_S_G | LCD_S_D);
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_S_A | LCD_S_B | LCD_S_C);
			break;
		case 0x0A:
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_S_F | LCD_S_E | LCD_S_G);
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_S_A | LCD_S_B | LCD_S_C);
			break;
		case 0x0B:
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_S_C);
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_S_G | LCD_S_E | LCD_S_D | LCD_S_F);
			break;
		case 0x0C:
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_S_F | LCD_S_E | LCD_S_D);
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_S_A);
			break;
		case 0x0D:
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_S_G | LCD_S_E | LCD_S_D);
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_S_B | LCD_S_C);
			break;
		case 0x0E:
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_S_G | LCD_S_E | LCD_S_D | LCD_S_F);
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_S_A);
			break;
		case 0x0F:
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_S_F | LCD_S_E | LCD_S_G);
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_S_A);
			break;
		case 0xFF:	//minus
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_S_G);
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_C);
			break;
		case 0xF0:	//nothing
			LCD->WF8B[LCD_Front_Pin[6]] = (LCD_C);
			LCD->WF8B[LCD_Front_Pin[7]] = (LCD_C);
			break;
	}
}
/*----------------------------------------------------------------------------
  Function for dots purpose
 *----------------------------------------------------------------------------*/
void slcd_Dots(uint8_t first, uint8_t second, uint8_t third){
	if (first)
		LCD->WF8B[LCD_Front_Pin[1]] |= (LCD_S_DEC);
	if (second)
		LCD->WF8B[LCD_Front_Pin[3]] |= (LCD_S_DEC);
	if (third)	
		LCD->WF8B[LCD_Front_Pin[5]] |= (LCD_S_DEC);
}
/*----------------------------------------------------------------------------
  Function for colon purpose
 *----------------------------------------------------------------------------*/
void slcd_Colon(void){
		LCD->WF8B[LCD_Front_Pin[7]] |= (LCD_S_DEC);
}
/*----------------------------------------------------------------------------
  Writing on sLCD (value) on specified position (digit)
*
* FP0 - Front Plane 0
*  f |_g
*  e |_
*    d
*
* FP1 - Front Plane 1
*    a_
*      | b
*    c |.
*        dot
 *----------------------------------------------------------------------------*/
void slcd_Set(uint8_t value, uint8_t digit){	
	switch(value){
		case 0x00:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_D | LCD_S_E | LCD_S_F); /* To display '0' we must active segments: d, e & f on first front plane */
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_A | LCD_S_B | LCD_S_C); /* To display '0' we must active segments: a, b & c on second front plane */
			break;
		case 0x01:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_C);  //TASK 2.5 - replace ZZZ with segment definition
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_B | LCD_S_C);  //TASK 2.5 - replace ZZZ with segment definition
			break;
		case 0x02:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_G | LCD_S_E | LCD_S_D );  //TASK 2.5 - replace ZZZ with segment definition
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_A | LCD_S_B);  //TASK 2.5 - replace ZZZ with segment definition
			break;
		case 0x03:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_G | LCD_S_D);  //TASK 2.5 - replace ZZZ with segment definition
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_A | LCD_S_B | LCD_S_C);  //TASK 2.5 - replace ZZZ with segment definition
			break;
		case 0x04:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_G | LCD_S_F);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_B | LCD_S_C);
			break;
		case 0x05:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_G | LCD_S_F | LCD_S_D);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_A | LCD_S_C);
			break;
		case 0x06:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_G | LCD_S_E | LCD_S_D  | LCD_S_F);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_A | LCD_S_C);
			break;
		case 0x07:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_C);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_A | LCD_S_B | LCD_S_C);
			break;
		case 0x08:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_G | LCD_S_E | LCD_S_D  | LCD_S_F);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_A | LCD_S_B | LCD_S_C);
			break;
		case 0x09:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_F | LCD_S_G | LCD_S_D);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_A | LCD_S_B | LCD_S_C);
			break;
		case 0x0A:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_F | LCD_S_E | LCD_S_G);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_A | LCD_S_B | LCD_S_C);
			break;
		case 0x0B:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_C);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_G | LCD_S_E | LCD_S_D | LCD_S_F);
			break;
		case 0x0C:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_F | LCD_S_E | LCD_S_D);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_A);
			break;
		case 0x0D:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_G | LCD_S_E | LCD_S_D);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_B | LCD_S_C);
			break;
		case 0x0E:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_G | LCD_S_E | LCD_S_D | LCD_S_F);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_A);
			break;
		case 0x0F:
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_F | LCD_S_E | LCD_S_G);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_S_A);
			break;
		case 0xFF:	//minus
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_G);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_C);
			break;
		case 0xF0:	//nothing
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_C);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_C);
			break;
		default:   //as default display 'r'
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = (LCD_S_E | LCD_S_G);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = (LCD_C);
			break;
	}
	if(digit>4){ 
		slcd_Err(1);
	}
	//slcd_Dots();
	//slcd_Colon();
}
/*----------------------------------------------------------------------------
  Function for conversion purpose, displaying (value) in specified (format)
 *----------------------------------------------------------------------------*/
void slcd_Display(uint16_t value ,uint16_t format){	
	uint16_t  tVariable = 0;
	uint16_t x[4];
	x[3] = value % format;
	tVariable = value - x[3]; //full number of system
	x[2] = tVariable / format % format;
	x[1] = tVariable / (format*format) %format;
	x[0] = tVariable / (format*format*format) % format;
	
	slcd_Set(x[0], 1);
	slcd_Set(x[1], 2);
	slcd_Set(x[2], 3);
	slcd_Set(x[3], 4);
	
}
/*----------------------------------------------------------------------------
  Clear sLCD
 *----------------------------------------------------------------------------*/
void slcd_Clear(void){
	uint8_t i = 0;
	
	for(i=0; i < 8; i++){
		LCD->WF8B[LCD_Front_Pin[i]] = LCD_C;
	}
}
/*----------------------------------------------------------------------------
  slcDisplay for ds18b20
 *----------------------------------------------------------------------------*/
void slcd_DS18B20(uint16_t value){
	uint16_t  tVariable = 0;
	uint16_t x[3];
	uint8_t format = 10;
	uint8_t afterdigit;
	
	if (value & 0xF800){
		slcd_Set(0xFF, 1);
		value = ~value;
	}
	else
		slcd_Set(0xF0, 1);
	
	afterdigit = value & 0x000F;
	value = value>>4;

	x[2] = value % format;
	tVariable = value - x[2];
	x[1] = tVariable / format % format;
	x[0] = afterdigit % 10;

	slcd_Set(x[1], 2);
	slcd_Set(x[2], 3);
	slcd_Set(x[0], 4);
	slcd_Dots(0,0,1);
}

