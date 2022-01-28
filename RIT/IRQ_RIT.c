#include "lpc17xx.h"
#include "RIT.h"
#include "../timer/timer.h"
#include "../GLCD/GLCD.h" 
#include "../game.h"
#include <string.h>

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

int key1=0;	// flag che dice se key1 è stato premuto
int key2=0;	// flag che dice se key1 è stato premuto
int int0=0; // flag che dice se int0 è stato premuto

extern enum GameStatus game_status; 

void RIT_IRQHandler (void)
{					
	/* gestione bottone INT0 */
	if(int0 > 1){ 
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	
			/* INT0 premuto */
			reset_RIT(); 
			switch(int0){
				case 2:
					// azione: RESET gioco
					if(game_status!=NotStarted){
						GUI_Text(6, 300, (uint8_t *) " Resetting Game... ", White, Black, 1);
						printWelcomeScreen(); 
					}
					break;
				default:
					break;
			}
			int0++;
		}
		else {	
			/* bottone rilasciato */
			int0=0;
			disable_RIT(); 
			reset_RIT(); 			
			NVIC_EnableIRQ(EINT0_IRQn);							 /* riabilitiamo interupt di EINT0			*/
			LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		}
	}
	else {
		if (int0 == 1)
			int0++;
	}
	
	/* gestione bottone KEY1 */
	if(key1 > 1){ 
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){	
			/* KEY1 premuto */				
			reset_RIT(); 
			switch(key1){
				case 2:
					// azione: START gioco
					if(game_status==NotStarted){
						GUI_Text(6, 300, (uint8_t *) " Starting Game...", White, Black, 1);
						startGame(); 
					}else 
					break;
				default:
					// se continuo a tenere il bottone premuto non succede niente
					break;
			}
			key1++;
		}
		else {	
			/* bottone KEY1 rilasciato */
			key1=0;		
			disable_RIT(); 
			reset_RIT();			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* riabilitiamo interrupt EINT1			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
	else {
		if(key1 == 1)
			key1++;
	}
	
	/* gestione bottone KEY2 */
	if(key2 > 1){ 
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){	
			/* KEY2 premuto */				
			reset_RIT(); 
			switch(key2){
				case 2:
					// azione: PAUSE gioco
					if(game_status==Started){
						pauseGame(); 
						GUI_Text(30, 300, (uint8_t *) "GAME PAUSED", White, Black, 1);
						GUI_Text(6, 300, (uint8_t *) "||", Red, Black, 1); 
					} else if(game_status== Paused){
							resumeGame(); 
					}
					break;
				default:
					// se continuo a tenere il bottone premuto non succede niente
					break;
			}
			key2++;
		}
		else {	
			/* bottone KEY1 rilasciato */
			key2=0;
			disable_RIT(); 
			reset_RIT(); 			
			NVIC_EnableIRQ(EINT2_IRQn);							 /* riabilitiamo interrupt EINT2			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}
	}
	else {
		if(key2 == 1)
			key2++;
	}
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
