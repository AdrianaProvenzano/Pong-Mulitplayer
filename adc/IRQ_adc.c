#include "lpc17xx.h"
#include "adc.h"
#include "../GLCD/GLCD.h" 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../game.h"

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

unsigned short ADC_curr;   
unsigned short ADC_last=0;     /* Last converted value*/ 

void ADC_IRQHandler(void) {
  ADC_curr = ((LPC_ADC->ADGDR>>4) & 0xFFF)*190/0xFFF;/* Read Conversion Result */
  
	if(abs(ADC_curr - ADC_last)>25){	
		// lo confronto con il valore precendente e se è 
		// abbastanza diverso lo passo alla funzione che muove il paddle
		moovePaddle_p0(ADC_curr); 	
		ADC_last = ADC_curr;
  }	
}
