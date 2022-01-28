				
				
				AREA asm_functions, CODE, READONLY				
                EXPORT  ricerca_massimo_negativo
					
ricerca_massimo_negativo
				; save current SP for a faster access 
				; to parameters in the stack
				
				; registri usati per passare parametri r0, r1, r2, r3
				; tutti gli altri registri non devono essere modificati 
				; PRIMA COSA DA FARE: pusharli nello stack
				; ULTIMA COSA DA FARE: fare la pop dallo stack 
				; R0 vettore 
				; R1 cont elementi nel vettore 
				
				MOV   r12, sp
				; save volatile registers
				STMFD sp!,{r4-r8,r10-r11,lr}		
				
				MOV r5, #0	; risultato temporaneo, massimo negativo
				SUB r1, #1	; indice di scansione
first_negative				
				; scandisco il vettore dall'ultimo al primo elemento <-
				; carico in r3 il primo valore del vettore, 
				; guardo se r3 < 0 
				; se si vado a loop 
				; se no decremento r1 
				; guardo se r1 < 0
				; se si vado a fine (era l'ultimo elemento) 
				; se no vado a first_negative
				LDR	r3, [r0,r1, LSL #2]				
				CMP r3, #0
				BLT loop				
				SUBS r1, #1				
				BLT fine
				B	first_negative	; carico un altro valore in r2
				
loop			SUBS r1, #1
				MOVMI R5, R3
				BMI	fine
				LDR r2, [r0,r1, LSL #2]
				CMP r2, #0
				SUBLTS r4, r3, r2
				MOVLT r3, r2
				B	loop
								
				; setup a value for R0 to return
fine				
				; muovo in r0 r5 che avevo inizializzato a 0 
				; faccio la pop 
				; r0 è il registro attraverso cui restituisco valore quando la funzione termina
				MOV	  r0, r5
				; restore volatile registers
				LDMFD sp!,{r4-r8,r10-r11,pc}
				
                END