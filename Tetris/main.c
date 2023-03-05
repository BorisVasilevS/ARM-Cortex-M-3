#include <MDR32Fx.h>
#include "lcd.h"
#include "font.h"
#include "primitives.h"
#include "figures.h"
#include "buttons.h"
#include "cpu.h"
#include "timers.h"
#include "pictures.h"


#define F_CPU 8000000
#define PSG		1000

#define button_UP			(!(MDR_PORTC->RXTX&(1<<13))) 	//12
#define button_DOWN		(!(MDR_PORTC->RXTX&(1<<14)))//11
#define button_RIGHT	(!(MDR_PORTC->RXTX&(1<<11)))
#define button_LEFT		(!(MDR_PORTC->RXTX&(1<<12)))
#define button_CENTER (!(MDR_PORTC->RXTX&(1<<10)))

#define button_press_delay 3

#define StartPoint_x	6
#define StartPoint_y	24

#define ScreenHeight	128
#define ScreenWidth		64

#define primitivSize	5

#define mapWidth		primitivSize*10
#define mapHeight		primitivSize*20

#define	NumHeight	5
#define NumWidth	3

#define	ScorePoint_x	(ScreenWidth - 2 - 6 * (NumWidth + 1))
#define	ScorePoint_y	3
#define LinesPoint_x	(ScreenWidth - 2 - 4 * (NumWidth + 1))
#define	LinesPoint_y	13
#define LevelPoint_x	((ScreenWidth / 2) - 2 * (NumWidth + 1))
#define	LevelPoint_y	3

#define menu_sq_W 40
#define menu_sq_H 40

#define nextFigure_x 1
#define nextFigure_y 1


#define lvl_0 5
#define lvl_1 lvl_0 + 5
#define lvl_2 lvl_1 + 5
#define lvl_3 lvl_2 + 5
#define lvl_4 lvl_3 + 5
#define lvl_5 lvl_4 + 5
#define lvl_6 lvl_5 + 5
#define lvl_7 lvl_6 + 5
#define lvl_8 lvl_7 + 5
#define lvl_9 lvl_8 + 5
#define lvl_10 lvl_9 + 5

#define one_line 40
#define two_lines 100
#define three_lines 300
#define four_lines 1200




struct figure{
	char name; // J I O L Z T S
	int x, y;
	int width;
	int height;
	int rightpoint;
	int leftpoint;
	int bottompoint;
	int toppoint;
	char model[4][4];
};




int max(int a, int b);
int min(int a, int b);
void ScreenUpdate(char array[ScreenHeight][ScreenWidth], int theme);
void fillArray_primitiv(char array_m[ScreenHeight][ScreenWidth], char array_s[primitivSize][primitivSize], short xPos, short yPos);
void fillArray(char array_m[ScreenHeight][ScreenWidth], char array_s[NumHeight][NumWidth], short xPos, short yPos);
void StartScreen(char array[ScreenHeight][ScreenWidth]);
void showScore(char staticlevel[ScreenHeight][ScreenWidth], int score, int xPos, int yPos);
void drawGameField(char staticlevel[ScreenHeight][ScreenWidth], char gamingfield[20][10], char model[primitivSize][primitivSize]);
void drawNum(char staticlevel[ScreenHeight][ScreenWidth], char model[NumHeight][NumWidth], int xPos, int yPos);
void clearPrimitive(char staticlevel[ScreenHeight][ScreenWidth], char model[primitivSize][primitivSize], int xPos, int yPos);
void drawPrimitive(char staticlevel[ScreenHeight][ScreenWidth], char model[primitivSize][primitivSize], int xPos, int yPos);
int movDown(char staticlevel[20][10], struct figure *fig);
void movRight(char staticlevel[20][10], struct figure *fig);
void movLeft(char staticlevel[20][10], struct figure *fig);
void movRotate(char staticlevel[20][10], struct figure *fig);
char isCollision(char staticlevel[20][10], struct figure fig);
void initFigure_model(struct figure *fig, char model[4][4]);
void initFigure_coord(struct figure *fig, int xPos, int yPos);
void placeFigure(struct figure fig, char array[20][10]);
void mixLayer(char staticlevel[20][10], char dynamiclevel[20][10], char mixedlevel[20][10]);
int maxRight(char model[4][4]);
int minLeft(char model[4][4]);
int minBottom(char model[4][4]);
int maxTop(char model[4][4]);
void rotateArray_CW(char arr[4][4], int width, int height);
void rotateArray_ACW(char arr[4][4], int width, int height);
void drawOneFig(struct figure fig, char staticlayer[ScreenHeight][ScreenWidth], char primivite[primitivSize][primitivSize], int xPos, int yPos);
void generateFigure(struct figure *fig);
int checkTetris(char gamefield[20][10], int* score, int line_num[]);
void deleteLines(char gamefield[20][10], int line_amount, int line_num[]);
void saveFigure(char staticlevel[20][10], char dynamiclevel[20][10]);
int checkTSpin(char gamefield[20][10], struct figure fig);
void initTimer_1(int arr, int psg);
void initTimer_2(int arr, int psg);
void initTimer_3(int arr, int psg);
void clear_array(char arr[20][10]);
void showLevel(char staticlevel[ScreenHeight][ScreenWidth], int score, int xPos, int yPos);
void showLines(char staticlevel[ScreenHeight][ScreenWidth], int score, int xPos, int yPos);
void clearScuare(char staticlevel[ScreenHeight][ScreenWidth], int xPos, int yPos, int width, int height);
void drawScuare(char staticlevel[ScreenHeight][ScreenWidth], int xPos, int yPos, int width, int height);
int checkLevel(int lines);
int levelToFreq(int speed);
void picToArray_full(char dest[128][64], char arr[][8], int picWidth, int picHeight, int xPos, int yPos, int inv);
void picToArray_half(char dest[128][64], char arr[][6], int picWidth, int picHeight, int xPos, int yPos, int inv);
void test_button(void);

	char screen[ScreenHeight][ScreenWidth] = {0};
	char staticLayer[20][10] = {0};
	char dynamicLayer[20][10] = {0};
	char mixedLayer[20][10] = {0};
	
	
	
	struct figure ActiveFig;
	struct figure NextFig;
	struct figure TEST;
	
	char gameIsOn;
	//int startSpeed = 2;
	int theme_WB = 1;
	int score123 = 0;
	int lines123 = 0;
	int levell23 = 0;
	int startlevell23 = 0;
	
	int drop_flag = 0; //0 ne bottom, 1 soft drop, 2 hard drop, 3 - on bottom
	int speed123 = 8000;
	int gameStatus = 4; //0 - menu, 1 - game settings, 2 - game, 3 - gameover screen, 4 - startscreen?, 5 - movement
	
	int status_button_UP = 0;
	int status_button_DOWN = 0;
	int status_button_CENTER = 0;
	int status_button_LEFT = 0;
	int status_button_RIGHT = 0;
	
	int button_CENTER_pressed_cnt = 0;
	int button_DOWN_pressed_cnt = 0;
	int button_UP_pressed_cnt = 0;
	int button_RIGHT_pressed_cnt = 0;
	int button_LEFT_pressed_cnt = 0;
	
int main(void)
{
	
	MDR_RST_CLK->PER_CLOCK = 0xFFFFFFFF;	
	//cpu_freq();
	int menu_x = 8 - 1;
	int	menu_y = 32;
	__enable_irq();
	initPerefirial();
	initButtons();
	InitLcd();
	SetPage(0, 0);
	SetAddr(0, 0);
	clear_lcd();
	int lines = 0;
	int lines_num[4];
	initTimer_1(100, PSG);
	initTimer_2(6, 0);
	while(1){
		while(gameStatus == 0){
			initTimer_1(100, PSG);
			timer3Off();
			for(int i = 0; i < ScreenHeight; i++){
				for(int j = 0; j < ScreenWidth; j++){
					screen[i][j] = 0;
				}
			}
			menu_y = 32;
			picToArray_full(screen, startscreen_1, STARTSCREEN_1_WIDTH / 8, STARTSCREEN_1_HEIGHT, 0,  55, 1);
			picToArray_half(screen, start_logo, START_LOGO_WIDTH / 8 , START_LOGO_HEIGHT, 1, 32, 1);
			drawScuare(screen, menu_x - 1, menu_y - 1, START_LOGO_WIDTH + 2, START_LOGO_HEIGHT + 2);
			picToArray_half(screen, options_logo, OPTIONS_LOGO_WIDTH / 8 , OPTIONS_LOGO_HEIGHT, 1, 16, 1);
			//ScreenUpdate(screen, theme_WB);
			while(gameStatus == 0){
				if(button_DOWN){
					if(menu_y !=16){
						clearScuare(screen, menu_x - 1, menu_y - 1, START_LOGO_WIDTH + 2, START_LOGO_HEIGHT + 2);
						menu_y  = 16;
						drawScuare(screen, menu_x - 1, menu_y - 1, START_LOGO_WIDTH + 2, START_LOGO_HEIGHT + 2);
					}
					while(button_DOWN);
				}
				if(button_CENTER){
					if(menu_y == 32) gameStatus = 2;
					else if(menu_y == 16) gameStatus = 1;
					while(button_CENTER);
				}
				if(button_UP){
					if(menu_y !=32){
						clearScuare(screen, menu_x - 1, menu_y - 1, START_LOGO_WIDTH + 2, START_LOGO_HEIGHT + 2);
						menu_y  = 32;
						drawScuare(screen, menu_x - 1, menu_y - 1, START_LOGO_WIDTH + 2, START_LOGO_HEIGHT + 2);
					}
				}
			}
		}
		while(gameStatus == 1){
			initTimer_1(100, PSG);
			timer3Off();
			for(int i = 0; i < ScreenHeight; i++){
				for(int j = 0; j < ScreenWidth; j++){
					screen[i][j] = 0;
				}
			}
			picToArray_half(screen, settings_logo, SETTINGS_LOGO_WIDTH / 8, SETTINGS_LOGO_HEIGHT, 1, 100, 1);
			picToArray_half(screen, them_logo, SETTINGS_LOGO_WIDTH / 8, SETTINGS_LOGO_HEIGHT, 1, 56, 1);
			picToArray_half(screen, control_logo, SETTINGS_LOGO_WIDTH / 8, SETTINGS_LOGO_HEIGHT, 1, 72, 1);
			picToArray_half(screen, level_logo, SETTINGS_LOGO_WIDTH / 8, SETTINGS_LOGO_HEIGHT, 1, 40, 1);
			picToArray_half(screen, back_logo, SETTINGS_LOGO_WIDTH / 8, SETTINGS_LOGO_HEIGHT, 1, 5, 1);
			menu_y = 72;
			drawScuare(screen, menu_x - 1, menu_y - 1, START_LOGO_WIDTH + 2, START_LOGO_HEIGHT + 2);
			showLevel(screen, startlevell23 + 1, LevelPoint_x + 4, 30);
			while(gameStatus == 1){
				//showLevel(screen, levell23, LevelPoint_x + 4, 30);
				if(button_LEFT){
					if(menu_y == 40){
						if(startlevell23 != 0) {startlevell23--; showLevel(screen, startlevell23 + 1, LevelPoint_x + 4, 30);}
					}
				while(button_LEFT);
				}
				if(button_RIGHT){
					if(menu_y == 40){
						if(startlevell23 < 10) {startlevell23++; showLevel(screen, startlevell23 + 1, LevelPoint_x + 4, 30);}
					}
				while(button_RIGHT);
				}
				if(button_DOWN){
					if(menu_y == 72){
						clearScuare(screen, menu_x - 1, menu_y - 1, START_LOGO_WIDTH + 2, START_LOGO_HEIGHT + 2);
						menu_y  = 56;
						drawScuare(screen, menu_x - 1, menu_y - 1, START_LOGO_WIDTH + 2, START_LOGO_HEIGHT + 2);
					}
					else if(menu_y == 56){
						clearScuare(screen, menu_x - 1, menu_y - 1, START_LOGO_WIDTH + 2, START_LOGO_HEIGHT + 2);
						menu_y  = 40;
						drawScuare(screen, menu_x - 1, menu_y - 1, START_LOGO_WIDTH + 2, START_LOGO_HEIGHT + 2);
					}
					else if(menu_y == 40){
						clearScuare(screen, menu_x - 1, menu_y - 1, START_LOGO_WIDTH + 2, START_LOGO_HEIGHT + 2);
						menu_y  = 5;
						drawScuare(screen, menu_x - 1, menu_y - 1, START_LOGO_WIDTH + 2, START_LOGO_HEIGHT + 2);
					}
					while(button_DOWN);
				}
				if(button_UP){
					if(menu_y == 56){
						clearScuare(screen, menu_x - 1, menu_y - 1, START_LOGO_WIDTH + 2, START_LOGO_HEIGHT + 2);
						menu_y  = 72;
						drawScuare(screen, menu_x - 1, menu_y - 1, START_LOGO_WIDTH + 2, START_LOGO_HEIGHT + 2);
					}
					else if(menu_y == 40){
						clearScuare(screen, menu_x - 1, menu_y - 1, START_LOGO_WIDTH + 2, START_LOGO_HEIGHT + 2);
						menu_y  = 56;
						drawScuare(screen, menu_x - 1, menu_y - 1, START_LOGO_WIDTH + 2, START_LOGO_HEIGHT + 2);
					}
					else if(menu_y == 5){
						clearScuare(screen, menu_x - 1, menu_y - 1, START_LOGO_WIDTH + 2, START_LOGO_HEIGHT + 2);
						menu_y  = 40;
						drawScuare(screen, menu_x - 1, menu_y - 1, START_LOGO_WIDTH + 2, START_LOGO_HEIGHT + 2);
					}
					while(button_UP);
				}
				if(button_CENTER){
					if(menu_y == 72){
						gameStatus = 5;
					}
					if(menu_y == 56){
						if(theme_WB) theme_WB = 0;
						else theme_WB = 1;
					}
					else if(menu_y == 5){
						clearScuare(screen, menu_x - 1, menu_y - 1, START_LOGO_WIDTH + 2, START_LOGO_HEIGHT + 2);
						gameStatus = 0;
					}
				while(button_CENTER);
				}
			}
		}
		while(gameStatus == 2){
			
			for(int i = 0; i < ScreenHeight; i++){
				for(int j = 0; j < ScreenWidth; j++){
					screen[i][j] = 0;
				}
			}
			StartScreen(screen);
			generateFigure(&NextFig);
			generateFigure(&ActiveFig);
			drawOneFig(NextFig, screen, primitive, (nextFigure_x - NextFig.x), nextFigure_y - (NextFig.bottompoint - 1) * primitivSize);
			initFigure_coord(&ActiveFig, 4, 19 - ActiveFig.toppoint);
			clear_array(staticLayer);
			clear_array(dynamicLayer);
			clear_array(mixedLayer);
			levell23 = startlevell23;
			speed123 = levelToFreq(checkLevel(lines123));
			initTimer_3(speed123, PSG);
			
			while(gameStatus == 2){
				if(button_LEFT && status_button_LEFT != 1){
					status_button_LEFT = 1;
					movLeft(staticLayer, &ActiveFig);
					placeFigure(ActiveFig, dynamicLayer);
				}
				if(button_RIGHT && status_button_RIGHT != 1){
					status_button_RIGHT = 1;
					movRight(staticLayer, &ActiveFig);
					placeFigure(ActiveFig, dynamicLayer);
				}
				if(button_UP && drop_flag != 1 && status_button_UP != 1){ //soft drop
					status_button_UP = 1;
					MDR_TIMER3 -> IE &= ~(1 << 1);
					while(movDown(staticLayer, &ActiveFig)){
						score123 += 1;
					}
					placeFigure(ActiveFig, dynamicLayer);
					drop_flag = 1;
					MDR_TIMER3 -> IE |= (1 << 1);	
				}
				if(button_DOWN && drop_flag != 2 && status_button_DOWN != 1){ //hard drop
					status_button_DOWN = 1;
					drop_flag = 2;
					MDR_TIMER3 -> IE &= ~(1 << 1);
					while(movDown(staticLayer, &ActiveFig)){
						score123 += 2;
					}
					placeFigure(ActiveFig, dynamicLayer);
					MDR_TIMER3 -> IE |= (1 << 1);	
					drop_flag = 3;
				}
				if(button_CENTER && status_button_CENTER != 1){
					status_button_CENTER = 1;
					movRotate(staticLayer, &ActiveFig);
					placeFigure(ActiveFig, dynamicLayer);
				}
				if(drop_flag == 3){
					MDR_TIMER3 -> IE &= ~(1 << 1);
					//while(movDown(staticLayer, &ActiveFig)){score123 += 1;}
					saveFigure(staticLayer, dynamicLayer);
					initFigure_model(&ActiveFig, fig_CLR);
					lines = checkTetris(staticLayer, &score123, lines_num);
					if(lines != 0){
						lines123 += lines;
						deleteLines(staticLayer, lines, lines_num);
						speed123 = levelToFreq(checkLevel(lines123));
						MDR_TIMER3 -> ARR = speed123;
						lines = 0;
					}
					initFigure_model(&ActiveFig, NextFig.model);
					initFigure_coord(&ActiveFig, 4, 19 - NextFig.toppoint);
					if(isCollision(staticLayer, ActiveFig)) {gameStatus = 3;}
					placeFigure(ActiveFig, dynamicLayer);
					generateFigure(&NextFig);
					drawOneFig(NextFig, screen, primitive, nextFigure_x, nextFigure_y - (NextFig.bottompoint - 1) * primitivSize);
					drop_flag = 0;
					MDR_TIMER3 -> IE |= (1 << 1);	
				}
				if(!button_DOWN) status_button_DOWN = 0;
				if(!button_UP) status_button_UP = 0;
				if(!button_RIGHT) status_button_RIGHT = 0;
				if(!button_LEFT) status_button_LEFT = 0;
				if(!button_CENTER) status_button_CENTER = 0;
			}
			
		}
		while(gameStatus == 3){
			timer3Off();
			for(int i = 0; i < ScreenHeight; i++){
				for(int j = 0; j < ScreenWidth; j++){
					screen[i][j] = 0;
				}
			}
			clear_array(mixedLayer);
			clear_array(staticLayer);
			clear_array(dynamicLayer);
			picToArray_full(screen, gameover, GAMEOVER_WIDTH / 8, GAMEOVER_HEIGHT, 0,  60, 1);
			showScore(screen, score123, (ScreenWidth / 2) - 3 * (NumWidth + 1), 60);
			showLines(screen, lines123,	(ScreenWidth / 2) - 2 * (NumWidth + 1),	50);
			//ScreenUpdate(screen, theme_WB);
			while(gameStatus == 3){
				if(button_CENTER){
					score123 = 0;
					lines123 = 0;
					levell23 = 0;
					gameStatus = 0;
				while(button_CENTER);
				}
			}
		}
		while(gameStatus == 4){
			timer3Off();
			for(int i = 0; i < ScreenHeight; i++){
				for(int j = 0; j < ScreenWidth; j++){
					screen[i][j] = 0;
				}
			}
			picToArray_full(screen, tetris_logo, GAMEOVER_WIDTH / 8, TETRIS_LOGO_HEIGHT, 0,  30, 0);
			ScreenUpdate(screen, theme_WB);
			timer1Off();
			while(gameStatus == 4){
				if((button_CENTER) || (button_DOWN) || (button_LEFT) || (button_UP) || (button_RIGHT)) gameStatus = 0;
			}
		}
		while(gameStatus == 5){
			timer1Off();
			timer3Off();
			for(int i = 0; i < ScreenHeight; i++){
				for(int j = 0; j < ScreenWidth; j++){
					screen[i][j] = 0;
				}
			}
			picToArray_full(screen, movement, MOVEMENT_WIDTH / 8, MOVEMENT_HEIGHT, 0,  0, 1);
			menu_y = 1;
			picToArray_half(screen, back_logo, SETTINGS_LOGO_WIDTH / 8, SETTINGS_LOGO_HEIGHT, 1, 1, 1);
			drawScuare(screen, menu_x + 5, menu_y - 1, START_LOGO_WIDTH - 8, START_LOGO_HEIGHT);
			ScreenUpdate(screen, theme_WB);
			while(gameStatus == 5){
				if(button_CENTER){
					gameStatus = 1;
					while(button_CENTER);
				}
			}
		}
	}
}

void clear_array(char arr[20][10]){
	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 10; j++){
			arr[i][j] = 0;
		}
	}
}
void test_button(void){
	if(button_DOWN){
		if((MDR_PORTD -> RXTX & (1 << 10)) == (1 << 10)){
			MDR_PORTD -> RXTX &= ~(1 << 10);
		}
		else{
			MDR_PORTD -> RXTX |= (1 << 10);
		}
		while(button_DOWN);
	}
}
/***********************ENGINE***********************************/

void movLeft(char staticlevel[20][10], struct figure *fig){
	(*fig).x--;
	if(isCollision(staticlevel, (*fig)))
		(*fig).x++;
	return;
}

void movRight(char staticlevel[20][10], struct figure *fig){
	(*fig).x++;
	if(isCollision(staticlevel, (*fig)))
		(*fig).x--;
	return;
}

int movDown(char staticlevel[20][10], struct figure *fig){ //if 0 - at the bottom
	(*fig).y--;
	if(isCollision(staticlevel, (*fig))){
		(*fig).y++;
		return 0 ;
	}
	return 1;
}

void mixLayer(char staticlevel[20][10], char dynamiclevel[20][10], char mixedlevel[20][10]){
	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 10; j++){
			mixedlevel[i][j] = dynamiclevel[i][j] + staticlevel[i][j];
		}
	}
}

void saveFigure(char staticlevel[20][10], char dynamiclevel[20][10]){
	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 10; j++){
			staticlevel[i][j] += dynamiclevel[i][j];
			dynamiclevel[i][j] = 0;
		}
	}
}

char isCollision(char staticlevel[20][10], struct figure fig){
	if(((fig.y + fig.bottompoint) < 0) || (fig.y + fig.toppoint - 1 > 19) || ((fig.x + fig.leftpoint) < 0) || ((fig.x + fig.rightpoint) > 9))
				return 1;
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			if((fig.model)[i][j]){ // && (staticlevel[fig.y + i][fig.x + j]
				if(staticlevel[fig.y + i][fig.x + j])
					return 1;
			}
		}
	}
	return 0;
}

void movRotate(char staticlevel[20][10], struct figure *fig){
	int yPos = (*fig).y;
	int xPos = (*fig).x;
	if((*fig).height == 2){
		if((*fig).width == 2)
			return;
		}
	rotateArray_CW((*fig).model, (*fig).width, (*fig).height);
	(*fig).rightpoint = maxRight((*fig).model);
	(*fig).leftpoint = minLeft((*fig).model);
	(*fig).bottompoint = minBottom((*fig).model);
	(*fig).toppoint = maxTop((*fig).model);
	if(isCollision(staticlevel, *fig)){
		rotateArray_ACW((*fig).model, (*fig).width, (*fig).height);
		(*fig).x = xPos;
		(*fig).y = yPos;
		(*fig).rightpoint = maxRight((*fig).model);
		(*fig).leftpoint = minLeft((*fig).model);
		(*fig).bottompoint = minBottom((*fig).model);
		(*fig).toppoint = maxTop((*fig).model);
	}
}

int max(int a, int b){
	if(a > b) return a;
	else return b;
}

int min(int a, int b){
	if(a < b) return a;
	else return b;
}

void rotateArray_ACW(char arr[4][4], int width, int height){
	int n = max(height, width);
	for (int i = 0; i < n / 2; i++) 
  	{ 
	    for (int j = i; j < n - i - 1; j++) 
	    { 
            int temp=arr[i][j]; 
            arr[i][j]=arr[n-1-j][i]; 
            arr[n-1-j][i]=arr[n-1-i][n-1-j]; 
            arr[n-1-i][n-1-j]=arr[j][n-1-i]; 
            arr[j][n-1-i]=temp; 
	    } 
  }
}

void rotateArray_CW(char arr[4][4], int width, int height){
	int n = max(height, width);
	for (int i = 0; i < n / 2; i++){ 
	    for (int j = i; j < n - i - 1;j++) 
    	{ 
		// Swapping elements after each iteration in Anticlockwise direction
			int temp=arr[i][j];
			arr[i][j]=arr[j][n-i-1];
			arr[j][n-i-1]=arr[n-i-1][n-j-1];
			arr[n-i-1][n-j-1]=arr[n-j-1][i];
			arr[n-j-1][i]=temp;
		}
	}
}

void generateFigure(struct figure *fig){
	int n = (MDR_TIMER2->CNT) + 1;	//kakoyto timer po osnovaniu 7
	n = n % 7;
	switch (n) {
		case 0 : initFigure_model(fig, fig_I); break;
		case 1 : initFigure_model(fig, fig_J); break;
		case 2 : initFigure_model(fig, fig_L); break;
		case 3 : initFigure_model(fig, fig_O); break;
		case 4 : initFigure_model(fig, fig_S); break;
		case 5 : initFigure_model(fig, fig_T); break;
		case 6 : initFigure_model(fig, fig_Z); break;	
	}
}

void deleteLines(char gamefield[20][10], int line_amount, int line_num[]){
	for(int k = 0; k < 5; k++){ //for animation
		for(int i = 0; i < line_amount; i++){
			gamefield[line_num[i]][4 - k] = 0;
			gamefield[line_num[i]][5 + k] = 0;
		}
		for(long int i = 0; i < 15000; i++);
	}
	for(int i = 0; i < line_amount; i++){
		for(int k = (line_num[line_amount - 1 - i] + 1); k < 20; k++){
			for(int j = 0; j < 10; j++){
				gamefield[k - 1][j] = gamefield[k][j];
			}
		}
	}
	for(int i = 20 - line_amount; i < 20; i++){
		for(int j = 0; j < 10; j++){
			gamefield[i][j] = 0; 
		}
	}
}

int checkTSpin(char gamefield[20][10], struct figure fig){
	int cnt = 0;
	for(int i = 0; i < max(fig.height,fig.width); i++){
		for(int j = 0; j < max(fig.height,fig.width); j++){
			if(fig.model[i][j]){
				if(fig.y + i - 1 >= 0){
					if(gamefield[fig.y + i - 1][fig.x + j])
						cnt ++;
				}
				else cnt++;
				if(fig.x + j - 1 >= 0){
					if(gamefield[fig.y + i][fig.x + j - 1])
						cnt ++;
				}
				if(gamefield[fig.y + i + 1][fig.x + j])
					cnt ++;
				else cnt++;
				if(fig.y + j + 1 <= 9){
					if(gamefield[fig.y + i][fig.x + j + 1])
						cnt ++;
				}
				else cnt++;
			}
			if(cnt == 3) return 1;
			cnt = 0;
		}
	}
	return 0;
}

int levelToFreq(int level){
	int arr = 0;
	switch(level){
		case 0 : arr = (F_CPU/(PSG*1)); break; //1 sec
		case 1 : arr = (F_CPU/(PSG*1.2)); break;
		case 2 : arr = (F_CPU/(PSG*1.5)); break;
		case 3 : arr = (F_CPU/(PSG*2)); break;
		case 4 : arr = (F_CPU/(PSG*3)); break;
		case 5 : arr = (F_CPU/(PSG*4)); break;
		case 6 : arr = (F_CPU/(PSG*5)); break;
		case 7 : arr = (F_CPU/(PSG*6)); break;
		case 8 : arr = (F_CPU/(PSG*7)); break;
		case 9 : arr = (F_CPU/(PSG*8.5)); break;
		case 10 : arr = (F_CPU/(PSG*10)); break;
	}
}

int checkLevel(int lines){
	int speed = startlevell23;
	
	if(lines > lvl_0 && levell23 - startlevell23  == 0){
		speed += 1;
		levell23++;
	}
	else if(lines > lvl_1 && levell23 - startlevell23  == 1){
		speed += 1;
		levell23++;
	}
	else if(lines > lvl_2 && levell23 - startlevell23  == 2){
		speed += 1;
		levell23++;
	}
	else if(lines > lvl_3 && levell23 - startlevell23  == 3){
		speed += 1;
		levell23++;
	}
	else if(lines > lvl_4 && levell23 - startlevell23  == 4){
		speed += 1;
		levell23++;
	}
	else if(lines > lvl_5 && levell23 - startlevell23  == 5){
		speed += 1;
		levell23++;
	}
	else if(lines > lvl_6 && levell23 - startlevell23  == 6){
		speed += 1;
		levell23++;
	}
	else if(lines > lvl_7 && levell23 - startlevell23  == 7){
		speed += 1;
		levell23++;
	}
	else if(lines > lvl_8 && levell23 - startlevell23  == 8){
		speed += 1;
		levell23++;
	}
	else if(lines > lvl_9 && levell23 - startlevell23  == 9){
		speed += 1;
		levell23++;
	}
	else if(lines > lvl_10 && levell23 - startlevell23  == 10){
		speed += 1;
		levell23++;
	}
	if(levell23 > 10) levell23 = 10;
	if(speed > 10) speed = 10;
	return speed;
}

int checkTetris(char gamefield[20][10], int* score, int line_num[]){
	int cnt = 0;
	int lines = 0;
	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 10; j++){
			if(gamefield[i][j] == 1) cnt++;
		}
		if(cnt == 10){
			line_num[lines] = i;
			lines++;
		}
		cnt = 0;
	}
	if(lines){
		switch (lines){
			case 1 : score123 += one_line; break;
			case 2 : score123 += two_lines; break;
			case 3 : score123 += three_lines; break;
			case 4 : score123 += four_lines; break;
		}
	}
	return lines;
}



/***********************************LCD******************************/
void picToArray_full(char dest[128][64], char arr[][8], int picWidth, int picHeight, int xPos, int yPos, int inv){
	char msk = 0x80;
	for(int i = yPos; i < min(yPos + picHeight, 128); i++){
		for(int j = xPos; j < xPos + picWidth; j++){
			msk = 0x80;
			for(int k = 0; k < 8; k++){
				if(inv)
					dest[i][8 * j + k] = ((~arr[min(yPos + picHeight, 128) - 1 - i][j - xPos]) & (msk >> k));
				else
					dest[i][8 * j + k] = ((arr[min(yPos + picHeight, 128) - 1 - i][j - xPos]) & (msk >> k));
			}
		}
	}
}

void picToArray_half(char dest[128][64], char arr[][6], int picWidth, int picHeight, int xPos, int yPos, int inv){
	char msk = 0x80;
	for(int i = yPos; i < min(yPos + picHeight, 128); i++){
		for(int j = xPos; j < xPos + picWidth; j++){
			msk = 0x80;
			for(int k = 0; k < 8; k++){
				if(inv)
					dest[i][8 * j + k] = ((~arr[min(yPos + picHeight, 128) - 1 - i][j - xPos]) & (msk >> k));
				else
					dest[i][8 * j + k] = ((arr[min(yPos + picHeight, 128) - 1 - i][j - xPos]) & (msk >> k));
			}
		}
	}
}



void drawPrimitive(char staticlevel[ScreenHeight][ScreenWidth], char model[primitivSize][primitivSize], int xPos, int yPos){
	for(int i = 0; i < primitivSize; i++){
		for(int j = 0; j < primitivSize; j++){
			staticlevel[yPos + i][xPos + j] = model[primitivSize - 1 - i][j];
		}
	}
}

void clearPrimitive(char staticlevel[ScreenHeight][ScreenWidth], char model[primitivSize][primitivSize], int xPos, int yPos){
	for(int i = 0; i < primitivSize; i++){
		for(int j = 0; j < primitivSize; j++){
			staticlevel[yPos + i][xPos + j] = 0;
		}
	}
}

void drawGameField(char staticlevel[ScreenHeight][ScreenWidth], char gamingfield[20][10], char model[primitivSize][primitivSize]){
	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 10; j++){
			if(gamingfield[i][j])
				drawPrimitive(staticlevel, primitive, (StartPoint_x + 1 + j * primitivSize), (StartPoint_y + 1 + i * primitivSize));
			else
				clearPrimitive(staticlevel, primitive, (StartPoint_x + 1 + j * primitivSize), (StartPoint_y + 1 + i * primitivSize));
		}
	}
}



void drawNum(char staticlevel[ScreenHeight][ScreenWidth], char model[NumHeight][NumWidth], int xPos, int yPos){
	for(int i = 0; i < 5; i++){
		for(int j = 0; j < 3; j++){
			staticlevel[yPos + i][xPos + j] = model[NumHeight - 1 - i][j];
		}
	}
}

void showScore(char staticlevel[ScreenHeight][ScreenWidth], int score, int xPos, int yPos){
	int razryad[6] = {0};
	razryad[0] = score % 10;
	razryad[1] = (score / 10) % 10;
	razryad[2] = (score / 100) % 10;
	razryad[3] = (score / 1000) % 10;
	razryad[4] = (score / 10000) % 10;
	razryad[5] = (score / 100000) % 10;
	for(long int i = 0; i < 6; i++){
		switch (razryad[i]) {
			case 0 : drawNum(staticlevel, num_0, xPos + 4 * (5 - i), yPos); break;
			case 1 : drawNum(staticlevel, num_1, xPos + 4 * (5 - i), yPos); break;
			case 2 : drawNum(staticlevel, num_2, xPos + 4 * (5 - i), yPos); break;
			case 3 : drawNum(staticlevel, num_3, xPos + 4 * (5 - i), yPos); break;
			case 4 : drawNum(staticlevel, num_4, xPos + 4 * (5 - i), yPos); break;
			case 5 : drawNum(staticlevel, num_5, xPos + 4 * (5 - i), yPos); break;
			case 6 : drawNum(staticlevel, num_6, xPos + 4 * (5 - i), yPos); break;
			case 7 : drawNum(staticlevel, num_7, xPos + 4 * (5 - i), yPos); break;
			case 8 : drawNum(staticlevel, num_8, xPos + 4 * (5 - i), yPos); break;
			case 9 : drawNum(staticlevel, num_9, xPos + 4 * (5 - i), yPos); break;
		}
	}
	drawScuare(staticlevel, xPos - 2, yPos - 2, 6 * (NumWidth + 1) + 3, 1 * (NumHeight + 1) + 3);
}

void showLevel(char staticlevel[ScreenHeight][ScreenWidth], int score, int xPos, int yPos){
	int razryad[2] = {0};
	razryad[0] = score % 10;
	razryad[1] = (score / 10) % 10;
	for(long int i = 0; i < 2; i++){
		switch (razryad[i]) {
			case 0 : drawNum(staticlevel, num_0, xPos + 4 * (1 - i), yPos); break;
			case 1 : drawNum(staticlevel, num_1, xPos + 4 * (1 - i), yPos); break;
			case 2 : drawNum(staticlevel, num_2, xPos + 4 * (1 - i), yPos); break;
			case 3 : drawNum(staticlevel, num_3, xPos + 4 * (1 - i), yPos); break;
			case 4 : drawNum(staticlevel, num_4, xPos + 4 * (1 - i), yPos); break;
			case 5 : drawNum(staticlevel, num_5, xPos + 4 * (1 - i), yPos); break;
			case 6 : drawNum(staticlevel, num_6, xPos + 4 * (1 - i), yPos); break;
			case 7 : drawNum(staticlevel, num_7, xPos + 4 * (1 - i), yPos); break;
			case 8 : drawNum(staticlevel, num_8, xPos + 4 * (1 - i), yPos); break;
			case 9 : drawNum(staticlevel, num_9, xPos + 4 * (1 - i), yPos); break;
		}
	}
	drawScuare(staticlevel, xPos - 2, yPos - 2, 2 * (NumWidth + 1) + 3, 1 * (NumHeight + 1) + 3);
}

void showLines(char staticlevel[ScreenHeight][ScreenWidth], int score, int xPos, int yPos){
	int razryad[4] = {0};
	razryad[0] = score % 10;
	razryad[1] = (score / 10) % 10;
	razryad[2] = (score / 100) % 10;
	razryad[3] = (score / 1000) % 10;
	for(long int i = 0; i < 4; i++){
		switch (razryad[i]) {
			case 0 : drawNum(staticlevel, num_0, xPos + 4 * (3 - i), yPos); break;
			case 1 : drawNum(staticlevel, num_1, xPos + 4 * (3 - i), yPos); break;
			case 2 : drawNum(staticlevel, num_2, xPos + 4 * (3 - i), yPos); break;
			case 3 : drawNum(staticlevel, num_3, xPos + 4 * (3 - i), yPos); break;
			case 4 : drawNum(staticlevel, num_4, xPos + 4 * (3 - i), yPos); break;
			case 5 : drawNum(staticlevel, num_5, xPos + 4 * (3 - i), yPos); break;
			case 6 : drawNum(staticlevel, num_6, xPos + 4 * (3 - i), yPos); break;
			case 7 : drawNum(staticlevel, num_7, xPos + 4 * (3 - i), yPos); break;
			case 8 : drawNum(staticlevel, num_8, xPos + 4 * (3 - i), yPos); break;
			case 9 : drawNum(staticlevel, num_9, xPos + 4 * (3 - i), yPos); break;
		}
	}
	drawScuare(staticlevel, xPos - 2, yPos - 2, 4 * (NumWidth + 1) + 3, 1 * (NumHeight + 1) + 3);
}

void drawScuare(char staticlevel[ScreenHeight][ScreenWidth], int xPos, int yPos, int width, int height){
	for(int i = yPos; i < (yPos + height); i++){
		for(int j = xPos; j < (xPos + width); j++){
			if((i == yPos) || (i == (yPos + height - 1))){
				if((j >= xPos) && (j <= (xPos + width - 1))){
					staticlevel[i][j] = 1;
				}
			}
			if((j == xPos) || (j == (xPos + width - 1))){
				if((i >= yPos) && (i <= (yPos + height - 1))){
					staticlevel[i][j] = 1;
				}
			}
		}
	}
}

void clearScuare(char staticlevel[ScreenHeight][ScreenWidth], int xPos, int yPos, int width, int height){
	for(int i = yPos; i < (yPos + height); i++){
		for(int j = xPos; j < (xPos + width); j++){
			if((i == yPos) || (i == yPos + height - 1)){
				if((j >= xPos) && (j <= xPos + width - 1)){
					staticlevel[i][j] = 0;
				}
			}
			if((j == xPos) || (j == xPos + width - 1)){
				if((i >= yPos) && (i <= yPos + height - 1)){
					staticlevel[i][j] = 0;
				}
			}
		}
	}
}

void ScreenUpdate(char array[ScreenHeight][ScreenWidth], int theme){
	int byte = 0;
	int x = (ScreenWidth - 1);
	int y = (ScreenHeight - 1);
	for(int crystal = 0; crystal < 2; crystal++){
		for(int page = 0; page < 8; page++){
			SetAddr(0, crystal);
			SetPage(page, crystal);
			for(int height = 0; height < 64; height++){
				for(int bit = 0; bit < 8; bit++) {
					if(array[y - (crystal * 64) - height][x - (page * 8) - bit]){
						if(theme)
							byte |= (1 << bit);
						else
							byte &= ~(1 << bit);
					}
					else{
						if(theme)
							byte &= ~(1 << bit);
						else
							byte |= (1 << bit);
					}
				}
				Send_Data(byte, crystal);
			}
			byte = 0;
		}
	}				
}

void StartScreen(char array[ScreenHeight][ScreenWidth]){
	drawScuare(array, StartPoint_x, StartPoint_y, mapWidth + 2, mapHeight + 2);
}

void fillArray(char array_m[ScreenHeight][ScreenWidth], char array_s[NumHeight][NumWidth], short xPos, short yPos){
	int i = 0;
	int j = 0;
	for(int y = yPos; y < (yPos + NumHeight); y++){
		for(int x = xPos; x < (xPos + NumWidth); x++){
			array_m[y][x] = array_s[(NumHeight - 1 - j)][i];
			i++;
		}
		i = 0;
		j++;
	}
}

void fillArray_primitiv(char array_m[ScreenHeight][ScreenWidth], char array_s[primitivSize][primitivSize], short xPos, short yPos){
	int i = 0;
	int j = 0;
	for(int y = yPos; y < (yPos + primitivSize); y++){
		for(int x = xPos; x < (xPos + primitivSize); x++){
			array_m[y][x] = array_s[(primitivSize - 1 - j)][i];
			i++;
		}
		i = 0;
		j++;
	}
}

void drawOneFig(struct figure fig, char staticlayer[ScreenHeight][ScreenWidth], char primivite[primitivSize][primitivSize], int xPos, int yPos){
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 4; j++){
			clearPrimitive(staticlayer, primitive, xPos + j * primitivSize, yPos + i * primitivSize);	
		}
	}
	int n = max(fig.height, fig.width);
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(fig.model[i][j])
				drawPrimitive(staticlayer, primitive, xPos + j * primitivSize, yPos + i * primitivSize);
			else
				clearPrimitive(staticlayer, primitive, xPos + j * primitivSize, yPos + i * primitivSize);	
		}
	}
}

/****************************************FIGURE*******************************************/

void initFigure_model(struct figure *fig, char model[4][4]){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			(*fig).model[i][j] = model[i][j];
		}
	}
	(*fig).rightpoint = maxRight((*fig).model);
	(*fig).leftpoint = minLeft((*fig).model);
	(*fig).bottompoint = minBottom((*fig).model);
	(*fig).toppoint = maxTop((*fig).model);
	(*fig).width = (*fig).rightpoint - (*fig).leftpoint + 1;
	(*fig).height = ((*fig).toppoint - (*fig).bottompoint + 1);
	
}
void initFigure_coord(struct figure *fig, int xPos, int yPos){
	(*fig).x = xPos;
	(*fig).y = yPos;
}

void clearArray(char array[20][10]){
	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 10; j++){
			array[i][j] = 0;
		}
	}
}

void placeFigure(struct figure fig, char array[20][10]){
	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 10; j++){
			array[i][j] = 0;
		}
	}
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			if(fig.model[i][j])
				array[i + fig.y][j + fig.x] = fig.model[i][j];
		}
	}
}

int maxRight(char model[4][4]){
	int max = 0;
	int buf = 0;
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			if(model[i][j]){
				buf = j;
			}
		}
		if(buf > max) max = buf;
	}
	return max;
}

int minLeft(char model[4][4]){
	int min = 3;
	int buf = 3;
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			if(model[i][j]){
				buf = j;
				if(buf < min) min = buf;
			}
		}
	}
	return min;
}

int minBottom(char model[4][4]){
	int min = 3;
	int cnt = 0;
	int buf = 3;
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			if(model[i][j]){
				buf = cnt;
			}
			
		}
		cnt++;
		if(buf < min) min = buf;
	}
	return min;
}

int maxTop(char model[4][4]){
	int cnt = 3;
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			if(model[3 - i][j]){
				return cnt;
			}
		}
		cnt--;
	}
	return cnt;
}




void Timer1_IRQHandler (void)
	{
		if(button_CENTER)
			button_CENTER_pressed_cnt++;
		else{
			button_CENTER_pressed_cnt = 0;
			button_CENTER_pressed_cnt = 0;
		}
		if(button_CENTER_pressed_cnt == button_press_delay){
			status_button_CENTER = 0;
			button_CENTER_pressed_cnt = 0;
		}
		
		if(button_DOWN)
			button_DOWN_pressed_cnt++;
		else{
			button_DOWN_pressed_cnt = 0;
			status_button_DOWN = 0;
		}
		if(button_DOWN_pressed_cnt == 10 * button_press_delay){
			status_button_DOWN = 0;
			button_DOWN_pressed_cnt = 0;
		}
		
		if(button_UP)
			button_UP_pressed_cnt++;
		else
			button_UP_pressed_cnt = 0;
		if(button_UP_pressed_cnt == 10 * button_press_delay){
			status_button_UP = 0;
			button_UP_pressed_cnt = 0;
		}
		
		if(button_LEFT)
			button_LEFT_pressed_cnt++;
		else
			button_LEFT_pressed_cnt = 0;
		if(button_LEFT_pressed_cnt == button_press_delay){
			status_button_LEFT = 0;
			button_LEFT_pressed_cnt = 0;
		}
		
		if(button_RIGHT)
			button_RIGHT_pressed_cnt++;
		else
			button_RIGHT_pressed_cnt = 0;
		if(button_RIGHT_pressed_cnt == button_press_delay){
			status_button_RIGHT = 0;
			button_RIGHT_pressed_cnt = 0;
	}
		if(gameStatus == 2){
			//placeFigure(ActiveFig, dynamicLayer);
			mixLayer(staticLayer, dynamicLayer, mixedLayer);
			drawGameField(screen, mixedLayer, primitive);
			showScore(screen, score123, ScorePoint_x, ScorePoint_y);
			int level_0 = 0;
			if(levell23> 10) level_0 = 10;
			else level_0 = levell23 + 1;
			showLevel(screen, level_0, LevelPoint_x, LevelPoint_y);
			showLines(screen, lines123, LinesPoint_x, LinesPoint_y);
			ScreenUpdate(screen, theme_WB);
		}
		else 
			ScreenUpdate(screen, theme_WB);
		
		MDR_TIMER1 -> STATUS = 0;//&= ~(1 << 0); //CNT_ZERO_EVENT
		NVIC->ICPR[0] &= ~(1 << 14);
	}

void Timer2_IRQHandler (void){
//	if((MDR_PORTD -> RXTX & (1 << 11)) == (1 << 11)){
//				MDR_PORTD -> RXTX &= ~(1 << 11);
//			}
//	else{
//		MDR_PORTD -> RXTX |= (1 << 11);
//	}
	NVIC->ISER[0] &= ~(1<<15);
	MDR_TIMER2 -> STATUS = 0;//&= ~(1 << 0); //CNT_ZERO_EVENT
	NVIC->ICPR[0] &= ~(1 << 15);
}

void Timer3_IRQHandler (void)
	{
		
		int var = 0;
		if(drop_flag != 2){
			var = movDown(staticLayer, &ActiveFig);
			score123 += var;
			if(var == 0) drop_flag = 3;
		}
		else
			drop_flag = 3;
		placeFigure(ActiveFig, dynamicLayer);
		MDR_TIMER3 -> STATUS = 0;//&= ~(1 << 0); //CNT_ZERO_EVENT
		NVIC->ICPR[0] &= ~(1 << 16);
	}
