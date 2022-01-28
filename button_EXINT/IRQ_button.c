#include "button.h"
#include "lpc17xx.h"

// FLAG definiti in IRQ_RIT.c
extern int key1;	// flag che dice se key1 è stato premuto
extern int key2;	// flag che dice se key2 è stato premuto
extern int int0;	// flag che dice se int0 è stato premuto 

void EINT0_IRQHandler (void)	  	/* INT0														 */
{	
	// EVITO DEBOUNCING
	enable_RIT(); 
	NVIC_DisableIRQ(EINT0_IRQn);		/* disable Button interrupts, per de-bouncing */
	LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection */
	int0=1;	
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	// EVITO DEBOUNCING
	enable_RIT(); 
	NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	key1=1;
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	// EVITO DEBOUNCING
	enable_RIT(); 
	NVIC_DisableIRQ(EINT2_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection */
	key2=1;
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}


