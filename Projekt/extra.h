/*----------------------------------------------------------------------------
 *      Author: Bartlomiej Gac 2018
 *			Name:    extra.h
 *      Purpose: SmP&DesignLaboratory Project
 *----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/

#ifndef extra_h
#define extra_h

#include "MKL46Z4.h"   /* Device header */	

void delay_mc(uint32_t value);
void delay_uc(uint32_t value);

typedef unsigned char bool;
enum { false, true };

#endif
