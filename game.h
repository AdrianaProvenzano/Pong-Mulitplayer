// methods 

void drawGameField(void); 

void startGame (void);

void endGame (void); 

void resumeGame(void); 

void pauseGame(void); 

void mooveBall (void);

void moovePaddle_p0(unsigned short );

void moovePaddle_p1(void);

void increaseScore(int); 

void hitWall(void); 

void hitPaddle(void); 

void printWelcomeScreen(void); 

void initBall(void); 

enum GameStatus{NotStarted, Started, Paused, Lost};
