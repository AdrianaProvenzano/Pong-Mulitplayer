#include "lpc17xx.h"
#include "timer.h"
#include "../led/led.h"
#include "../adc/adc.h"
#include "../GLCD/GLCD.h" 
#include "../game.h"


/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER0_IRQHandler (void)
{
	// per il movimento della barra in basso 
	ADC_start_conversion(); 
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
uint16_t SinTable[45] =                                       /*                      */
	{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
	};
	
void TIMER1_IRQHandler (void)
{
	// per il movimento della pallina e della barra sopra
	mooveBall(); 					
	moovePaddle_p1(); 		
	LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

void TIMER2_IRQHandler (void)
{
	// per la riproduzione dei suoni 
	static int ticks=0; 
	LPC_DAC->DACR = SinTable[ticks%45]<<6;
	ticks++; 
	if(ticks==90){
		disable_timer(2); 
		ticks=0; 
		LPC_DAC->DACR = 0; 
	}
  LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}
/******************************************************************************
**                            End Of File
******************************************************************************/
