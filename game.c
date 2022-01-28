#include "game.h"
#include "LPC17xx.h"                    // Device header
#include "timer/timer.h"								
#include "GLCD/GLCD.h" 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum GameStatus game_status;				// stato del gioco (NotStarted, Started, Paused, Lost)

int score_p0 = 0; 									// punteggo player 0	(bottom)
char score_p0_string[10]=""; 	
int score_p1 = 0; 									// punteggo player 1 (top)
char score_p1_string[10]=""; 
int winner = -1; 										// vincitore 0, 1 o -1 se non ha ancora vincitore
unsigned short paddle_last = 5;     // ultima posizione della barra player 0
unsigned short paddle_curr = 0;			// posizione corrente della barra  player 0
unsigned short paddle_p1_x = 5;			// posizione barra player 1
int paddle_p1_right = 1; 						// direzione barra player 1
int ball_x; 												// posizione della pallina, asse x
int ball_y; 												// posizione della pallina, asse y
int ball_x_incr = 0; 								// incremento della posizione della pallina, asse x
int ball_y_incr = 0; 								// incremento della posizione della pallina, asse y
int right; 													// flag 1 se la pallina va verso destra 
int down; 													// flag 1 se la pallina va verso giù
int i; 															// indice per cicli for 
int j; 															// indice per cicli for 

void startGame(){
	// il gioco è iniziato
	game_status = Started; 

	// disegno il campo di gioco
	LCD_Clear(Black);
	drawGameField(); 
		
	// inizializzo i punteggi
	score_p0=0;
	score_p1=0; 
	winner = -1; 
	
	// stampo i punteggi
	sprintf(score_p0_string,"%d", score_p0); 
	GUI_Text(7, 160, (uint8_t *) score_p0_string, White, Black, 1);
	sprintf(score_p1_string,"%d", score_p1); 
	GUI_Text(232, 173, (uint8_t *) score_p1_string, White, Black, -1);
	
	// inizializzo posizione palla
	initBall(); 
}

void initBall(){
		// inizializzo la posizione della pallina e di quanto viene 
	// incrementata la sua posizione muovendosi
	ball_x = 225; 
	ball_y = 141;
	ball_x_incr = 5; 
	ball_y_incr = 5; 
	
	// inizializzo la direzione della pallina 
	down = 1; 
	right = 0; 
	
	// disegno la pallina
	for ( i=0; i<5 ; i++ ) 
		for ( j = 0; j< 5; j++) 
			LCD_SetPoint(ball_x+i, ball_y+j, Green); 
	
	// disegno la barra nella nuova posizione 
	for ( i=0; i<10 ; i++ ) {
		LCD_DrawLine(paddle_last, 276+i, paddle_last+49, 276+i, Green);	
		LCD_DrawLine(paddle_p1_x, 31+i, paddle_p1_x+49, 31+i, Green);	
	}
			
	// faccio partire i timer
	enable_timer(0); 		// per la barra (ADC)
	enable_timer(1);	 	// per la pallina
}

void endGame(){
	// il gioco è terminato
	game_status=Lost; 		
	
	reset_timer(1); 			
	disable_timer(1); 		// disablilito timer pallina
	
	reset_timer(0); 
	disable_timer(0); 		// disabilito timer paddle 
	
	// cancello la pallina
	for ( i=0; i<5 ; i++ ) 
		for ( j = 0; j< 5; j++) 
			LCD_SetPoint(ball_x+i, ball_y+j, Black); 
	
	// cancello le barre
	for ( i=0; i<10 ; i++ ) {
		LCD_DrawLine(paddle_curr, 276+i, paddle_curr+49, 276+i, Black);	// orizzontale in alto
		LCD_DrawLine(paddle_p1_x, 31+i, paddle_p1_x+49, 31+i, Black);	// orizzontale in alto
	}
	
	// cancello il punteggio
	for ( i=0; i<17 ; i++ ) {
		LCD_DrawLine(6, 159+i, 35, 159+i, Black);
		LCD_DrawLine(225, 159+i, 234, 159+i, Black);	
	}
	
	// stampo messaggi di fine partita 
	if(winner==0){
		GUI_Text(90, 80, (uint8_t *) " ESOL OUY ", Red, Black, -1);
		GUI_Text(80, 240, (uint8_t *) " YOU WIN ", Green, Black, 1);
	}
	else if(winner==1){
		GUI_Text(90, 80, (uint8_t *) " NIW UOY ", Green, Black, -1);
		GUI_Text(80, 240, (uint8_t *) " YOU LOSE ", Red, Black, 1);
	}
	
	GUI_Text(20, 150, (uint8_t *) "Press INT0 and then KEY1", White, Black, 1);
	GUI_Text(40, 170, (uint8_t *) "to start a new game", White, Black, 1);
	
	return; 
}

void pauseGame(void){
	game_status=Paused;			// il gioco è in pausa 
	disable_timer(0);				// disabilito timer per il paddle (ADC)
	disable_timer(1); 			// disabilito timer per la pallina 
	return; 
}

void resumeGame(void){
	game_status=Started; 		// il gioco ricomincia
	enable_timer(0); 				// riabilito timer per il paddle (ADC) 
	enable_timer(1); 				// riabilito timer per la pallina
	
	// cancello "GAME PAUSED" 
	for ( i=0; i<30 ; i++ ) {
		LCD_DrawLine(5, 290+i, 120, 290+i, Black);	
	}
}

void printWelcomeScreen(void){
	// disabilito timer nel caso in cui premo INT0
	// direttamente mentre sto giocando una partita
	reset_timer(1); 			
	disable_timer(1); 		// disablilito timer pallina
	
	reset_timer(0); 
	disable_timer(0); 		// disabilito timer paddle 
	
	// schermata iniziale con istruzioni e record attuale
	LCD_Clear(Black);

	GUI_Text(50, 70, (uint8_t *) "PONG: Multiplayer", White, Black, 1);
	GUI_Text(40, 140, (uint8_t *) "Press KEY1 to start", White, Black, 1);
	GUI_Text(40, 170, (uint8_t *) "Press KEY2 to pause", White, Black, 1);
	GUI_Text(40, 200, (uint8_t *) "Press INT0 to reset", White, Black, 1);
	
	game_status = NotStarted; 
}

void drawGameField(){
	// il campo è rappresentato da due linee verticali, a destra e a sinistra
	for ( i=0; i<5 ; i++ ) {
		LCD_DrawLine(0+i, 0, 0+i, 320, Red);			// verticale sinistra 
		LCD_DrawLine(235+i, 0, 235+i, 320, Red); 	// verticale destra 
	}
	return;
}

void mooveBall(){
	if(game_status!=Started)
		return; 
	
	// cancelliamo la pallina nella vecchia posizione 
	for ( i=0; i<5 ; i++ ) 
		for ( j = 0; j< 5; j++) {
			if(ball_x+i>6 && ball_x+i<35 && ball_y+j>159 && ball_y+j<175){
				// la pallina era sopra il punteggio 
					; 
			}else if(ball_x+i>215 && ball_x+i<234 && ball_y+j>159 && ball_y+j<175){
				// la pallina era sopra il punteggio 
					; 
			}
			else{
					LCD_SetPoint(ball_x+i, ball_y+j, Black); 
				}
			}

	// ha incontrato un muro ?
	if(ball_x<=230 && ball_x>=226 && right){				// muro a destra
		right = 0; 
		hitWall(); 		 
	}
	else if (ball_x>=5 && ball_x<=9 && !right){		// muro a sinistra
		right = 1; 
		hitWall(); 		
	}
	
	// ha incontrato la barra sopra ? 
	if(ball_y==41 && !down) {
		// si trova all'altezza della barra 
		if(ball_x> paddle_p1_x && ball_x< paddle_p1_x+49){
			// si trova nella lunghezza della barra
			if(ball_x> paddle_p1_x+16 && ball_x< paddle_p1_x+34)
				// si trova al centro della barra
				ball_x_incr = 1; 
			else if ((ball_x> paddle_p1_x && ball_x< paddle_p1_x+8)||
				(ball_x> paddle_p1_x+42 && ball_x< paddle_p1_x+49))
				// si trova all'estremità della barra 
				ball_x_incr = 5;
			else 
				ball_x_incr = 3; 
			down=1;
			hitPaddle(); 
			for ( i=0; i<5 ; i++ ) 
				for ( j = 0; j< 5; j++) {
					LCD_SetPoint(ball_x+i, ball_y+j, Green); 
				}
			return; 
		} 
	}else if (ball_y<41){
			increaseScore(0);
		return; 
	} 
	
	// ha incontrato la barra sotto? 
	if(ball_y==271 && down) {
		// si trova all'altezza della barra 
		if(ball_x>paddle_curr && ball_x<paddle_curr+49){
			// si trova nella lunghezza della barra
			if(ball_x>paddle_curr+16 && ball_x<paddle_curr+34)
				// si trova al centro della barra
				ball_x_incr = 1; 
			else if ((ball_x>paddle_curr && ball_x<paddle_curr+8)||
				(ball_x>paddle_curr+42 && ball_x<paddle_curr+49))
				// si trova all'estremità della barra 
				ball_x_incr = 5;
			else 
				ball_x_incr = 3; 
			down=0;
			hitPaddle(); 
			for ( i=0; i<5 ; i++ ) 
				for ( j = 0; j< 5; j++) {
					LCD_SetPoint(ball_x+i, ball_y+j, Green); 
				}
			return; 
		} 
	}else if (ball_y>271){
		increaseScore(1);
		return; 
	} 
	
	// aggiorno la posizione della pallina 
	if(down){
		ball_y = ball_y + ball_y_incr; 
	} else{
		ball_y = ball_y - ball_y_incr; 
	}
	if(right){
		ball_x = ball_x + ball_x_incr; 
	}else{
		ball_x = ball_x - ball_x_incr; 
	}
	
	for ( i=0; i<5 ; i++ ) 
			for ( j = 0; j< 5; j++) {
				if(ball_x+i>6 && ball_x+i<35 && ball_y+j>159 && ball_y+j<175){
					// la pallina passa sopra il punteggio 
					; 
				}
				else if(ball_x+i>215 && ball_x+i<234 && ball_y+j>159 && ball_y+j<175){
				// la pallina era sopra il punteggio 
					; 
				}
				else{
					LCD_SetPoint(ball_x+i, ball_y+j, Green); 
				}
			}	
	
	return; 
}


void moovePaddle_p0(unsigned short curr){
	if(game_status!=Started)
		return;
	
	paddle_curr = curr; 
	if(paddle_curr<=5)
		paddle_curr = paddle_curr + 5; 
	if(paddle_curr>=185)
		paddle_curr = paddle_curr-5; 
	
	// cancello la barra nell'ultima posizione 
	for ( i=0; i<10 ; i++ ) {
		LCD_DrawLine(paddle_last, 276+i, paddle_last+49, 276+i, Black);	
	}
	// disegno la barra nella nuova posizione 
	for ( i=0; i<10 ; i++ ) {
		LCD_DrawLine(paddle_curr, 276+i, paddle_curr+49, 276+i, Green);	
	}
	
	paddle_last = paddle_curr; 
	return;
}

void moovePaddle_p1(void){
	if(game_status!=Started)
		return;
	
	// cancelliamo il paddle nella vecchia posizione 
	for ( i=0; i<10 ; i++ ) {
		LCD_DrawLine(paddle_p1_x, 31+i, paddle_p1_x+49, 31+i, Black);	
	}

	//	ha incontrato un muro ?
	if(paddle_p1_x==185 && paddle_p1_right){				// muro a destra
		paddle_p1_right = 0; 
	}
	else if (paddle_p1_x==5 && !paddle_p1_right){		// muro a sinistra
		paddle_p1_right = 1; 
	}
	
	if(paddle_p1_right){
		paddle_p1_x = paddle_p1_x + 10; 
	} else {
		paddle_p1_x = paddle_p1_x - 10;
	}
	if(game_status==Started || game_status==Paused){
		// ridisegniamo il paddle nella nuova posizione 
		for ( i=0; i<10 ; i++ ) {
			LCD_DrawLine(paddle_p1_x, 31+i, paddle_p1_x+49, 31+i, Green);	
		}
	}

	return; 
}

void hitWall(void){
	init_timer(2, 2120);  		
	enable_timer(2); 
}

void hitPaddle(void){
	init_timer(2, 1062);  		
	enable_timer(2); 
}

void increaseScore(int player){
	// incremento punteggio
	if(player==0){
		score_p0 = score_p0 +1; 
		sprintf(score_p0_string,"%d", score_p0); 
		GUI_Text(7, 160, (uint8_t *) score_p0_string, White, Black, 1);
	}else if(player==1){
		score_p1 = score_p1+1; 
		sprintf(score_p1_string,"%d", score_p1); 
		GUI_Text(232, 173, (uint8_t *) score_p1_string, White, Black, -1);
	}
	
	// verifico se qualcuno è arrivato a 5
	if(score_p0 ==5){
		winner = 0; 
	}
	else if(score_p1 ==5){
		winner = 1; 
	}
	if(winner!=-1){
		endGame(); 
		return; 
	}
	
	reset_timer(1); 			
	disable_timer(1); 		// disablilito timer pallina
	
	reset_timer(0); 
	disable_timer(0); 		// disabilito timer paddle 
	
	// ricomincio partita 
	initBall(); 
}
