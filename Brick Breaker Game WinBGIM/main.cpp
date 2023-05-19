#include <graphics.h>
#include <stdlib.h>
#include <time.h>

#define BRICK_ROWS 5
#define BRICK_COLS 8
#define BRICK_WIDTH 60
#define BRICK_HEIGHT 20
#define BRICK_SPACING 10
#define PADDLE_WIDTH 80
#define PADDLE_HEIGHT 10
#define BALL_RADIUS 8
#define BALL_SPEED 5

#define VK_ESC 27

int bricks[BRICK_ROWS][BRICK_COLS];
int ballX, ballY, ballDX, ballDY;
int paddleX, paddleY;
int bricks_created= BRICK_ROWS * BRICK_COLS;
int bricks_left=BRICK_ROWS * BRICK_COLS;
int score=0;
int lives=3;






void initBricks();
void drawBricks();
void drawPaddle();
void drawBall();
void updateBall();
void updatePaddle();
int hit_paddle();
void game_loop();
void draw_score(int score);
void draw_lives(int lives);
void reset_game();




void initBricks() {
    for (int row = 0; row < BRICK_ROWS; row++) {
        for (int col = 0; col < BRICK_COLS; col++) {
            bricks[row][col] = 1;
        }
    }
    ballX = getmaxx() / 2;
    ballY = getmaxy() / 2;
    ballDX = rand() % 2 == 0 ? -BALL_SPEED : BALL_SPEED;
    ballDY = -BALL_SPEED;
    paddleX = getmaxx() / 2;
    paddleY = getmaxy() - PADDLE_HEIGHT / 2;
}




void drawBricks() {
    int totalWidth = BRICK_COLS * (BRICK_WIDTH + BRICK_SPACING) - BRICK_SPACING;
    int startX = (getmaxx() - totalWidth) / 2;
    int startY = 50;

    for (int row = 0; row < BRICK_ROWS; row++) {
        for (int col = 0; col < BRICK_COLS; col++) {
            if (bricks[row][col]) {
                int x = startX + col * (BRICK_WIDTH + BRICK_SPACING);
                int y = startY + row * (BRICK_HEIGHT + BRICK_SPACING);
                setfillstyle(SOLID_FILL, MAGENTA); // set the fill color to purple
            	setcolor(BLUE); // set the border color to blue
            	rectangle(x, y, x + BRICK_WIDTH, y + BRICK_HEIGHT);
            	floodfill(x + 1, y + 1, BLUE); // fill the rectangle with purple
            }
        }
    }
}




void drawPaddle() {
    int x = paddleX - PADDLE_WIDTH / 2;
    int y = paddleY - PADDLE_HEIGHT / 2;
    setcolor(CYAN);
	 setfillstyle(SOLID_FILL, CYAN);  // set fill color to cyan
    setcolor(CYAN);  // set border color to blue
    rectangle(x, y, x + PADDLE_WIDTH, y + PADDLE_HEIGHT);
    floodfill(x + 1, y + 1, CYAN);  // fill the paddle with cyan color
    
}





void drawBall() {
	setcolor(LIGHTRED);
    circle(ballX, ballY, BALL_RADIUS);
    setfillstyle(SOLID_FILL, LIGHTRED);
    floodfill(ballX, ballY, LIGHTRED);
}






void updateBall() {
    ballX += ballDX;
    ballY += ballDY;

    if (ballX <= BALL_RADIUS || ballX >= getmaxx() - BALL_RADIUS) {
        ballDX = -ballDX;
    }

    if (ballY <= BALL_RADIUS) {
        ballDY = -ballDY;
    }
	if(hit_paddle()){
        ballDY = -ballDY;
    }
	
    // check for collision with bricks
    int totalWidth = BRICK_COLS * (BRICK_WIDTH + BRICK_SPACING) - BRICK_SPACING;
    int startX = (getmaxx() - totalWidth) / 2;
    int startY = 50;
    int row = (ballY - startY) / (BRICK_HEIGHT + BRICK_SPACING);
    int col = (ballX - startX) / (BRICK_WIDTH + BRICK_SPACING);
    if (row >= 0 && row < BRICK_ROWS && col >= 0 && col < BRICK_COLS && bricks[row][col]) {
        bricks[row][col] = 0;
        bricks_left--;
        score+=10;
        ballDY = -ballDY;
    }
    
}


void updatePaddle() {
    if (kbhit()) {
        int key = getch();
        switch (key) {
            case 'a':
            case 'A':
                paddleX -= 20;
                break;
            case 'd':
            case 'D':
                paddleX += 20;
                break;
        }
    }

    if (paddleX < PADDLE_WIDTH / 2) {
        paddleX = PADDLE_WIDTH / 2;
    }

    if (paddleX > getmaxx() - PADDLE_WIDTH / 2) {
        paddleX = getmaxx() - PADDLE_WIDTH / 2;
    }
}


void game_loop(){
	int game_over = 0;
	int start_game=0;
	
	setcolor(LIGHTRED);
	settextstyle(10, 0, 4);
	outtextxy(100, 150, "BRICK BREAKER GAME");
	settextstyle(10, 0, 4);
	setcolor(CYAN); 
	outtextxy(120, 190, "Press any key to start");
	settextstyle(10, 0, 2);
	outtextxy(180, 240, "Credits Duran Duman");
	
    while (!start_game) {
        if (getch()) {
            start_game = 1;
        }
    }

	while (!game_over) {
        cleardevice();
        drawBricks();
        drawPaddle();
        drawBall();
        updateBall();
        updatePaddle();
        draw_score(score);
        draw_lives(lives);
        
        //BÜTÜN BLOKLAR KIRILDIYSA OYUN LOOP'UNU DURDURUR.
        if(bricks_left==0){
        	game_over=1;
		}
			// Oyunun bitip bitmediðini kontrol eder. (Top en alt row'a deðerse 3 haktan sonra oyun biter)
        if (ballY > getmaxy()) {
            lives--;
        	if (lives==0) {
            	game_over=1;
        	}
        	else {
            	settextstyle(10, 0, 2);
            	setcolor(LIGHTRED);
				outtextxy(90, 200, "You lost a life. Press Enter to continue.\n");
                while (1){
                    if (getch()) {
                        if (GetAsyncKeyState(VK_RETURN)!= 0) {
                            break;
                        }
                    }
                }
                ballX = getmaxx() / 2;
    			ballY = getmaxy() / 2;
    			ballDX = rand() % 2 == 0 ? -BALL_SPEED : BALL_SPEED;
    			ballDY = -BALL_SPEED;
    			paddleX = getmaxx() / 2;
    			paddleY = getmaxy() - PADDLE_HEIGHT / 2;
            }
        }
        delay(40);
        
    }
    if(bricks_left == 0){ // oyunu kazandýðýnýzý yazdýrýr.
        	cleardevice();	
			settextstyle(8, 0, 4);
			outtextxy(140, 200, "You Win!");
			draw_score(score);
			draw_lives(lives);	
	}
	else{ // oyunun bittiðini yazdýrýr.
			cleardevice();	
			settextstyle(10, 0, 3);
			outtextxy(240, 140, "Game Over!");
			outtextxy(100, 190,"Press enter to restart the game!");
			outtextxy(200, 240,"Press esc to exit!");
			draw_score(score);
			draw_lives(lives);	
			//Eðer yeniden oynamak ya da çýkmak istiyorsan:
			 while (1){
                    if (getch()) {
                    	//OYUNU RESETLER
                        if (GetAsyncKeyState(VK_RETURN)!= 0) {
                        	 reset_game();          
                        }if (GetAsyncKeyState(VK_ESC)!= 0) {
                            	closegraph(); // ekraný kapatýr              
                        }
                    }
            }
	}
}

int hit_paddle(){
 	if (ballY >= getmaxy() - BALL_RADIUS - PADDLE_HEIGHT && ballX >= paddleX - PADDLE_WIDTH / 2 - BALL_RADIUS &&
        ballX <= paddleX + PADDLE_WIDTH / 2 + BALL_RADIUS) {
        return 1;
 	}
 	return 0;
}
    





void draw_score(int score) {
  // Ýlk olarak, score deðiþkeninin deðerini karakter dizisine dönüþtürüyoruz.
  char score_str[20];
  sprintf(score_str, "Your score : %d", score);
  // Metni ekrana yazdýrýyoruz.
  setcolor(LIGHTMAGENTA);
  settextstyle(10, 0, 1);
  outtextxy(10, 5, score_str);
}

void draw_lives(int lives){
	// Ýlk olarak, lives deðiþkeninin deðerini karakter dizisine dönüþtürüyoruz.
	char lives_str[20];
  	sprintf(lives_str, "Your life left: %d", lives);
  	setcolor(LIGHTMAGENTA);
  	settextstyle(10, 0, 1);
  	outtextxy(450, 5,lives_str);
}

void reset_game(){
	cleardevice();
	ballX = getmaxx() / 2;
    ballY = getmaxy() / 2;
    ballDX = rand() % 2 == 0 ? -BALL_SPEED : BALL_SPEED;
    ballDY = -BALL_SPEED;
    paddleX = getmaxx() / 2;
    paddleY = getmaxy() - PADDLE_HEIGHT / 2;
	score=0;
	lives=3;
	initBricks();
	game_loop();    
	
}

int main() {
    initwindow(640, 480, "Brick Breaker");
    srand(time(NULL));
    initBricks();
   	game_loop();
    getch();
    closegraph();
    return 0;
}
        
        
        
        
