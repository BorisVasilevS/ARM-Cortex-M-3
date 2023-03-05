#include <MDR32Fx.h>
#include "lcd.h"
#include <math.h>
#include "timers.h"
#include "cpu.h"
#include "buttons.h"

#define ScreenHeight	64
#define ScreenWidth		128

#define radius_123 5

#define PI 3.141592

#define friction 0.92
#define gravity 0.5
#define stop_speed 0.025
#define basket_x 0
#define basket_y 40
#define basket_width 20
#define basket_height 16
#define points 2
#define speed_step 0.05
#define angle_step 0.02
#define speed_start 7.0
#define angle_start 2.0


#define ball_startpos_x 100
#define ball_startpos_y 8

#define score_pos_x 110
#define score_pos_y 55

#define time_pos_x 60
#define time_pos_y 55

#define	NumHeight	5
#define NumWidth	3

#define button_RIGHT	(!(MDR_PORTC->RXTX&(1<<13))) 	//12
#define button_LEFT		(!(MDR_PORTC->RXTX&(1<<14)))//11
#define button_UP			(!(MDR_PORTC->RXTX&(1<<11)))
#define button_DOWN		(!(MDR_PORTC->RXTX&(1<<12)))
#define button_CENTER (!(MDR_PORTC->RXTX&(1<<10)))
#define button_press_delay 20

char num_0[NumHeight][NumWidth] = {
	1, 1, 1,
	1, 0, 1,
	1, 0, 1,
	1, 0, 1,
	1, 1, 1};

char num_1[NumHeight][NumWidth] = {
	0, 1, 0,
	1, 1, 0,
	0, 1, 0,
	0, 1, 0,
	1, 1, 1};
	
char num_2[NumHeight][NumWidth] = {
	0, 1, 0,
	1, 0, 1,
	0, 0, 1,
	0, 1, 0,
	1, 1, 1};

char num_3[NumHeight][NumWidth] = {
	1, 1, 0,
	0, 0, 1,
	0, 1, 0,
	0, 0, 1,
	1, 1, 0};

char num_4[NumHeight][NumWidth] = {
	0, 0, 1,
	0, 1, 1,
	1, 0, 1,
	1, 1, 1,
	0, 0, 1};

char num_5[NumHeight][NumWidth] = {
	1, 1, 1,
	1, 0, 0,
	1, 1, 1,
	0, 0, 1,
	1, 1, 1};

char num_6[NumHeight][NumWidth] = {
	1, 1, 1,
	1, 0, 0,
	1, 1, 1,
	1, 0, 1,
	1, 1, 1};

char num_7[NumHeight][NumWidth] = {
	1, 1, 1,
	0, 0, 1,
	0, 1, 0,
	1, 0, 0,
	1, 0, 0};

char num_8[NumHeight][NumWidth] = {
	1, 1, 1,
	1, 0, 1,
	1, 1, 1,
	1, 0, 1,
	1, 1, 1};

char num_9[NumHeight][NumWidth] = {
	1, 1, 1,
	1, 0, 1,
	1, 1, 1,
	0, 0, 1,
	1, 1, 1};



typedef struct {
	float x, y;
	float speed;
} velocity;

typedef struct {
	float x, y;
	int radius; //radius = radius + 0.5; always
	char model[2 * radius_123 + 1][2 * radius_123 + 1];
} Ball;

typedef struct {
	Ball ball;
	velocity vel;
} Object;

void PutObjOnMap(Object obj, char layer[ScreenHeight][ScreenWidth]);
void InitObj(Object *obj, float xPos, float yPos, int rad, float speed, float angle, char model[2 * radius_123 + 1][2 * radius_123 + 1]);
//void InitObj(Object *obj, float xPos, float yPos, int rad, float speed, float angle, char *model);
void ScreenUpdate(char array1[ScreenHeight][ScreenWidth], char array2[ScreenHeight][ScreenWidth], int theme);
char isPosInMap(Object obj);
void drawScuare(char staticlevel[ScreenHeight][ScreenWidth], int xPos, int yPos, int width, int height);
void clearObject(Object obj, char layer[ScreenHeight][ScreenWidth]);
void mixLayers(char sumlayer[ScreenHeight][ScreenWidth], char layer1[ScreenHeight][ScreenWidth], char layer2[ScreenHeight][ScreenWidth]);
void drawBasket(char layer[ScreenHeight][ScreenWidth], int xPos, int yPos, int width);
void drawBasketNet(char hex[][4], char layer[ScreenHeight][ScreenWidth], int xPos, int yPos, int inv);
void drawPlayer(char hex[][2], char layer[ScreenHeight][ScreenWidth], int xPos, int yPos, int inv);
//void drawLine(char layer[ScreenHeight][ScreenWidth], int xPos, int yPos, float length, float angle);
void drawTrajectory(Object obj, char layer[ScreenHeight][ScreenWidth]);
void clearTrajectory(Object obj, char layer[ScreenHeight][ScreenWidth]);
void showTime(char staticlevel[ScreenHeight][ScreenWidth], int time, int xPos, int yPos);
void showScore(char staticlevel[ScreenHeight][ScreenWidth], int score, int xPos, int yPos);


char model[2 * radius_123 + 1][2 * radius_123 + 1] = {
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
	0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0,
	0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0,
	0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1,
	1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1,
	0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0,
	0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0,
	0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0
};

char basket[][4]  = {
  0xc0, 0x00, 0x01, 0xff, 
  0xfb, 0xbb, 0xdd, 0xff, 
  0xc0, 0x00, 0x01, 0xff, 
  0xf7, 0x77, 0xb7, 0xff, 
  0xc0, 0x00, 0x07, 0xff, 
  0xed, 0xee, 0xdf, 0xff, 
  0xdd, 0xcd, 0x9f, 0xff, 
  0xc0, 0x00, 0x3f, 0xff, 
  0xf7, 0xba, 0xff, 0xff, 
  0xef, 0x74, 0xff, 0xff, 
  0xc0, 0x01, 0xff, 0xff, 
  0xee, 0xe3, 0xff, 0xff, 
  0xc0, 0x07, 0xff, 0xff, 
  0xdd, 0x8f, 0xff, 0xff, 
  0xfb, 0x9f, 0xff, 0xff, 
  0xc0, 0x3f, 0xff, 0xff
};

char player[][2]  = {
  0xff, 0xff, 
  0xff, 0xff, 
  0xfc, 0x3f, 
  0xf9, 0x9f, 
  0xf9, 0xdf, 
  0xfa, 0x1f, 
  0xfb, 0xdf, 
  0x7b, 0xde, 
  0x80, 0x01, 
  0xfc, 0x1f, 
  0xfb, 0xef, 
  0xf3, 0xf3, 
  0xe7, 0xfd, 
  0xe8, 0x7d, 
  0xeb, 0x74, 
  0x89, 0x6a, 
  0x9a, 0x78, 
  0xe8, 0x72, 
  0xef, 0xea, 
  0xeb, 0xe6, 
  0xe3, 0xfd, 
  0xaf, 0xfd, 
  0x0f, 0xfb, 
  0xef, 0xfb, 
  0xe7, 0xf3, 
  0xf3, 0xf3, 
  0xf8, 0x07, 
  0xff, 0x1f, 
  0xfe, 0x9f, 
  0xfe, 0x9f, 
  0xfc, 0x1f, 
  0xf8, 0x1f
};

Object ball;
char gamefield[ScreenHeight][ScreenWidth]; 
char noncollisionlayer[ScreenHeight][ScreenWidth];
int score_g = 0;
float angle_g = 2.0;
float speed_g = 7.0;
char button_CENTER_pressed = 0;
char button_LEFT_pressed = 0;
char button_RIGHT_pressed = 0;
char button_UP_pressed = 0;
char button_DOWN_pressed = 0;
char drawtrajectory = 0;
int gameStatus = 0; //0 - setup

int goal_g = 0;
int time_g = 0;
int button_CENTER_pressed_cnt = 0;
int button_DOWN_pressed_cnt = 0;
int button_UP_pressed_cnt = 0;
int button_RIGHT_pressed_cnt = 0;
int button_LEFT_pressed_cnt = 0;

int main(void)
{
	MDR_RST_CLK->PER_CLOCK = 0xFFFFFFFF;	
	__enable_irq();
	initButtons();
	initPerefirial();
	InitLcd();
	SetPage(0, 0);
	SetAddr(0, 0);
	clear_lcd();
	
	//drawScuare(screen, 20, 20, 30, 2);
	//InitObj(&ball, ball_startpos_x,  ball_startpos_y, radius_123, 8.7, 2.3, model);
	//InitObj(&ball, ball_startpos_x,  ball_startpos_y, radius_123, speed_g, angle_g, model);
//	showTime(noncollisionlayer, 123, 60, 55);
//	showScore(noncollisionlayer, 123, 110, 55);
	//PutObjOnMap(ball, gamefield);

	//drawBasketNet(basket, noncollisionlayer, basket_x, basket_y - basket_height, 1);
	//drawPlayer(player, gamefield, 16, 0, 1);
	//drawBasket(gamefield, basket_x, basket_y, basket_width);
	//mixLayers(screen, staticlayer, balllayer);
	//drawLine(noncollisionlayer, 0, 0, 20, 1);
	//drawLine(noncollisionlayer, 40, 20, 15, 2);
	//drawTrajectory(ball, noncollisionlayer);
	//ScreenUpdate(gamefield, noncollisionlayer, 1);
	initTimer_1(80, 1000);
	initTimer_2(80, 100);
	initTimer_3(8000, 1000);
	//initTimer_2(80, 1000);
	while(1){
		while(gameStatus == 0){
			time_g = 60;
			score_g = 0;
//			showTime(noncollisionlayer, time_g, time_pos_x, time_pos_y);
			InitObj(&ball, ball_startpos_x,  ball_startpos_y, radius_123, speed_g, angle_g, model);
//			drawBasketNet(basket, noncollisionlayer, basket_x, basket_y - basket_height, 1);
//			drawPlayer(player, gamefield, 16, 0, 1);
//			drawBasket(gamefield, basket_x, basket_y, basket_width);
			while(gameStatus == 0){
				goal_g = 0;
				if(button_LEFT && button_LEFT_pressed != 1){
					MDR_TIMER1 -> IE &= ~(1<<1);
					button_LEFT_pressed = 1;
					clearTrajectory(ball, noncollisionlayer);
					speed_g -= speed_step;
					if(speed_g < 0)
						speed_g = 0;
					InitObj(&ball, ball_startpos_x, ball_startpos_y, radius_123, speed_g, angle_g, model);
					MDR_TIMER1 -> IE |= (1<<1);
				}
				if(button_RIGHT && button_RIGHT_pressed != 1){
					MDR_TIMER1 -> IE &= ~(1<<1);
					button_RIGHT_pressed = 1;
					clearTrajectory(ball, noncollisionlayer);
					speed_g += speed_step;
					if(speed_g > 30)
						speed_g = 30;
					InitObj(&ball, ball_startpos_x, ball_startpos_y, radius_123, speed_g, angle_g, model);
					MDR_TIMER1 -> IE |= (1<<1);
				}
				if(button_UP && button_UP_pressed != 1){
					MDR_TIMER1 -> IE &= ~(1<<1);
					button_UP_pressed = 1;
					clearTrajectory(ball, noncollisionlayer);
					angle_g -= angle_step;
					if(angle_g > 2 * PI)
						angle_g -= 2 * PI;
					else if(angle_g < 0)
						angle_g += 2 * PI;
					InitObj(&ball, ball_startpos_x, ball_startpos_y, radius_123, speed_g, angle_g, model);
					MDR_TIMER1 -> IE |= (1<<1);
				}
				if(button_DOWN && button_DOWN_pressed != 1){
					if((MDR_PORTD -> RXTX & (1 << 11)) == (1 << 11)){
								MDR_PORTD -> RXTX &= ~(1 << 11);
							}
					else{
						MDR_PORTD -> RXTX |= (1 << 11);
					}
					MDR_TIMER1 -> IE &= ~(1<<1);
					button_DOWN_pressed = 1;
					clearTrajectory(ball, noncollisionlayer);
					angle_g += angle_step;
					if(angle_g > 2 * PI)
						angle_g -= 2 * PI;
					else if(angle_g < 0)
						angle_g += 2 * PI;
					InitObj(&ball, ball_startpos_x, ball_startpos_y, radius_123, speed_g, angle_g, model);
					MDR_TIMER1 -> IE |= (1<<1);
				}
				if(button_CENTER && button_CENTER_pressed != 1){
					button_CENTER_pressed = 1;
					if(drawtrajectory){
						drawtrajectory = 0;
					}
					else{
						drawtrajectory = 1;
					}		
				}
				if(!button_CENTER)
					button_CENTER_pressed = 0;
				if(!button_LEFT)
					button_LEFT_pressed = 0;
				if(!button_RIGHT)
					button_RIGHT_pressed = 0;
				if(!button_UP)
					button_UP_pressed = 0;
				if(!button_DOWN)
					button_DOWN_pressed = 0;
			}
		}
		while(gameStatus == 1){
			
			//while(gameStatus == 1){
				if(button_DOWN && button_DOWN_pressed != 1){// && button_DOWN_pressed != 1
					//button_DOWN_pressed = 1;
					if((MDR_PORTD -> RXTX & (1 << 11)) == (1 << 11)){
							MDR_PORTD -> RXTX &= ~(1 << 11);
						}
					else{
						MDR_PORTD -> RXTX |= (1 << 11);
					}
					clearObject(ball, gamefield);
					//InitObj(&ball, ball_startpos_x, ball_startpos_y, radius_123, speed_g, angle_g, model);
					gameStatus = 2;
				}
				if(!button_UP)
					button_DOWN_pressed = 0;
				//}
		}
		while(gameStatus == 2){
			goal_g = 0;
			speed_g = speed_start;
			angle_g = angle_start;
			InitObj(&ball, ball_startpos_x,  ball_startpos_y, radius_123, speed_g, angle_g, model);
			PutObjOnMap(ball, gamefield);
			drawBasketNet(basket, noncollisionlayer, basket_x, basket_y - basket_height, 1);
			drawPlayer(player, gamefield, 16, 0, 1);
			drawBasket(gamefield, basket_x, basket_y, basket_width);
			while(gameStatus == 2){
				if(button_LEFT && button_LEFT_pressed != 1){
					MDR_TIMER1 -> IE &= ~(1<<1);
					button_LEFT_pressed = 1;
					clearTrajectory(ball, noncollisionlayer);
					speed_g -= speed_step;
					if(speed_g < 0)
						speed_g = 0;
					InitObj(&ball, ball_startpos_x, ball_startpos_y, radius_123, speed_g, angle_g, model);
					MDR_TIMER1 -> IE |= (1<<1);
				}
				if(button_RIGHT && button_RIGHT_pressed != 1){
					MDR_TIMER1 -> IE &= ~(1<<1);
					button_RIGHT_pressed = 1;
					clearTrajectory(ball, noncollisionlayer);
					speed_g += speed_step;
					if(speed_g > 30)
						speed_g = 30;
					InitObj(&ball, ball_startpos_x, ball_startpos_y, radius_123, speed_g, angle_g, model);
					MDR_TIMER1 -> IE |= (1<<1);
				}
				if(button_UP && button_UP_pressed != 1){
					MDR_TIMER1 -> IE &= ~(1<<1);
					button_UP_pressed = 1;
					clearTrajectory(ball, noncollisionlayer);
					angle_g -= angle_step;
					if(angle_g > 2 * PI)
						angle_g -= 2 * PI;
					else if(angle_g < 0)
						angle_g += 2 * PI;
					InitObj(&ball, ball_startpos_x, ball_startpos_y, radius_123, speed_g, angle_g, model);
					MDR_TIMER1 -> IE |= (1<<1);
				}
				if(button_DOWN && button_DOWN_pressed != 1){
					if((MDR_PORTD -> RXTX & (1 << 11)) == (1 << 11)){
								MDR_PORTD -> RXTX &= ~(1 << 11);
							}
					else{
						MDR_PORTD -> RXTX |= (1 << 11);
					}
					MDR_TIMER1 -> IE &= ~(1<<1);
					button_DOWN_pressed = 1;
					clearTrajectory(ball, noncollisionlayer);
					angle_g += angle_step;
					if(angle_g > 2 * PI)
						angle_g -= 2 * PI;
					else if(angle_g < 0)
						angle_g += 2 * PI;
					InitObj(&ball, ball_startpos_x, ball_startpos_y, radius_123, speed_g, angle_g, model);
					MDR_TIMER1 -> IE |= (1<<1);
				}
				if(button_CENTER && button_CENTER_pressed != 1){

					button_CENTER_pressed = 1;
					if(drawtrajectory){
						drawtrajectory = 0;
					}
					else{
						drawtrajectory = 1;
					}		
				}
				if(!button_CENTER)
					button_CENTER_pressed = 0;
				if(!button_LEFT)
					button_LEFT_pressed = 0;
				if(!button_RIGHT)
					button_RIGHT_pressed = 0;
				if(!button_UP)
					button_UP_pressed = 0;
				if(!button_DOWN)
					button_DOWN_pressed = 0;
			}
		}
	}
}




void drawTrajectory(Object obj, char layer[ScreenHeight][ScreenWidth]){
	int x = 0;
	int y = 0;
	for(int t = 0; t < 120; t += 5){
		y = (int)round(obj.ball.y + obj.vel.y * (t / 5.0) - ((gravity * (t / 5.0) * (t / 5.0)) / 2.0));
		x = (int)round(obj.ball.x + obj.vel.x * (t / 5.0));
		if((x >= 0) && (x < ScreenWidth) && (y >= 0) && (y < ScreenHeight))
			layer[y][x] = 1;
	}
}

void clearTrajectory(Object obj, char layer[ScreenHeight][ScreenWidth]){
	int x = 0;
	int y = 0;
	for(int t = 0; t < 120; t += 5){
		y = (int)round(obj.ball.y + obj.vel.y * (t / 5.0) - ((gravity * (t / 5.0) * (t / 5.0)) / 2.0));
		x = (int)round(obj.ball.x + obj.vel.x * (t / 5.0));
		if((x >= 0) && (x < ScreenWidth) && (y >= 0) && (y < ScreenHeight))
			layer[y][x] = 0;
	}
}

void drawBasket(char layer[ScreenHeight][ScreenWidth], int xPos, int yPos, int width){
	drawScuare(layer, xPos, yPos - basket_height, 2, basket_height);
	drawScuare(layer, xPos + 2 + width, yPos - 2, 2, 2);
}

void drawBasketNet(char hex[][4], char layer[ScreenHeight][ScreenWidth], int xPos, int yPos, int inv){
	char msk = 0x80;
	for(int i = yPos; i < yPos + 16; i++){
		for(int j = xPos; j < xPos + 4; j++){
			msk = 0x80;
			for(int k = 0; k < 8; k++){
				if(inv)
					layer[2 * yPos + 16 - 1 - i][8 * (j) - xPos + k] = ((~hex[i - yPos][j - xPos]) & (msk >> k));
				else
					layer[2 * yPos + 16 - 1 - i][8 * (j) - xPos + k] = ((hex[i - yPos][j - xPos]) & (msk >> k));
			}
		}
	}
}

void drawPlayer(char hex[][2], char layer[ScreenHeight][ScreenWidth], int xPos, int yPos, int inv){
	char msk = 0x80;
	for(int i = yPos; i < yPos + 32; i++){
		for(int j = xPos; j < xPos + 2; j++){
			msk = 0x80;
			for(int k = 0; k < 8; k++){
				if(inv)
					layer[2 * yPos + 32 - 1 - i][8 * (j) - xPos + k] = ((~hex[i - yPos][j - xPos]) & (msk >> k));
				else
					layer[2 * yPos + 32 - 1 - i][8 * (j) - xPos + k] = ((hex[i - yPos][j - xPos]) & (msk >> k));
			}
		}
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

void checkScore(Object obj){
	if((obj.ball.y < basket_y - max(-1 * obj.vel.y / 2.0,1)) && (obj.ball.y > basket_y + max(-1 * obj.vel.y / 2.0, 1)) - 1 && ((obj.ball.x > basket_x + 2) && (obj.ball.x < basket_x + 2 + basket_width)) && (obj.vel.y < 0) && goal_g != 1){
		goal_g = 1;
		score_g += points;
	}
}

char isPosInMap(Object obj){
	if((obj.ball.x - obj.ball.radius >= 0) && (obj.ball.x + obj.ball.radius + 1 < ScreenWidth - 1) && (obj.ball.y - obj.ball.radius >= 0) && (obj.ball.y + obj.ball.radius + 1 < ScreenHeight - 1))
		return 1;
	else 
		return 0;
}


void InitObj(Object *obj, float xPos, float yPos, int rad, float speed, float angle, char model[2 * radius_123 + 1][2 * radius_123 + 1]){ //, char model[radius][radius]
	(*obj).ball.x = xPos;
	(*obj).ball.y = yPos;
	(*obj).ball.radius = rad;
	(*obj).vel.speed = speed;
	(*obj).vel.x = speed * cos(angle);
	(*obj).vel.y = speed * sin(angle);
	for(int i = 0; i < 2 * rad + 1; i++){
		for(int j = 0; j < 2 * rad + 1; j++){
			(*obj).ball.model[i][j] = model[i][j];//model[i*2*rad + j];
		}
	}
}

void PutObjOnMap(Object obj, char layer[ScreenHeight][ScreenWidth]){
	int ix = (int)round(obj.ball.x);
	int iy = (int)round(obj.ball.y);
	int iradius = obj.ball.radius;
	for(int i = 0; i < 2 * iradius + 1; i++){
		for(int j = 0; j < 2 * iradius + 1; j++){
			if(isPosInMap(obj))
				layer[iy + iradius - i][ix - iradius + j] = obj.ball.model[i][j];//2 * radius_123 + 1 - 
		}
	}
}

void ScreenUpdate(char array1[ScreenHeight][ScreenWidth], char array2[ScreenHeight][ScreenWidth], int theme){
	int byte = 0;
	int x = 0;//(ScreenWidth - 1);
	int y = (ScreenHeight - 1);
	for(int crystal = 0; crystal < 2; crystal++){
		for(int page = 0; page < 8; page++){
			SetAddr(0, crystal);
			SetPage(page, crystal);
			for(int width = 0; width < 64; width++){
				for(int bit = 0; bit < 8; bit++) {
					if(array1[y - (page * 8) - bit ][x + ((crystal * 64) + width)] + array2[y - (page * 8) - bit ][x + ((crystal * 64) + width)]){
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


char isCollision(Object obj, char collislayer[ScreenHeight][ScreenWidth]){
	if(!isPosInMap(obj)){
		return 1;
	}
	else{
		for(int i = 0; i < 2 * obj.ball.radius + 1; i++){
			for(int j = 0; j < 2 * obj.ball.radius + 1; j++){
				if(obj.ball.model[(int)round(obj.ball.y) - obj.ball.radius + i][(int)round(obj.ball.x) - obj.ball.radius + j]){
					if(collislayer[(int)round(obj.ball.y) - obj.ball.radius + i][(int)round(obj.ball.x) - obj.ball.radius + j])
						return 1;
					}
				}
			}
		}
	return 0;
}



void clearObject(Object obj, char layer[ScreenHeight][ScreenWidth]){
	int ix = (int)round(obj.ball.x);
	int iy = (int)round(obj.ball.y);
	for(int i = 0; i < 2 * obj.ball.radius + 1; i++){
		for(int j = 0; j < 2 * obj.ball.radius + 1; j++){
				layer[iy + obj.ball.radius - i][ix - obj.ball.radius + j] = 0; 
		}
	}
}

void mixLayers(char sumlayer[ScreenHeight][ScreenWidth], char layer1[ScreenHeight][ScreenWidth], char layer2[ScreenHeight][ScreenWidth]){
	for(int i = 0; i < ScreenHeight; i++){
		for(int j = 0; j < ScreenWidth; j++){
			sumlayer[i][j] = layer1[i][j] + layer2[i][j];
		}
	}
}

void movUpdate(Object *obj, char collislayer[ScreenHeight][ScreenWidth]){
	float yPos = (*obj).ball.y;
	float xPos = (*obj).ball.x;
	(*obj).ball.y += (*obj).vel.y;
	if(isCollision(*obj, collislayer)){
		while(isCollision(*obj, collislayer)){
			if((*obj).vel.y > 0)
				(*obj).ball.y -= 0.5;
			else
				(*obj).ball.y += 0.5;
		}
		(*obj).vel.speed *= friction;
		(*obj).vel.x *= friction;
		(*obj).vel.y *= friction;
		(*obj).vel.y = -(*obj).vel.y;
	}
	(*obj).ball.x += (*obj).vel.x;
	if(isCollision(*obj, collislayer)){
		while(isCollision(*obj, collislayer)){
			if((*obj).vel.x > 0)
				(*obj).ball.x -= 0.5;
			else
				(*obj).ball.x += 0.5;
		}
		(*obj).vel.speed *= friction;
		(*obj).vel.x *= friction;
		(*obj).vel.y *= friction;
		(*obj).vel.x = -(*obj).vel.x;
	}
	return;
}

void movGravity(Object *obj){
	//if(((*obj).vel.x > stop_speed) || ((*obj).vel.x < -stop_speed))
		(*obj).vel.y -= gravity;
//	else
//		(*obj).vel.y = 0;
		//(*obj).vel.x = 0;
}

void drawScuare(char staticlevel[ScreenHeight][ScreenWidth], int xPos, int yPos, int width, int height){
	for(int i = yPos; i < (yPos + height); i++){
		staticlevel[i][xPos] = 1;
		staticlevel[i][xPos + width - 1] = 1;
	}
	for(int i = xPos; i < (xPos + width); i++){
		staticlevel[yPos][i] = 1;
		staticlevel[yPos + height - 1][i] = 1;
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
	int razryad[3] = {0};
	razryad[0] = score % 10;
	razryad[1] = (score / 10) % 10;
	razryad[2] = (score / 100) % 10;
	for(long int i = 0; i < 3; i++){
		switch (razryad[i]) {
			case 0 : drawNum(staticlevel, num_0, xPos + 4 * (2 - i), yPos); break;
			case 1 : drawNum(staticlevel, num_1, xPos + 4 * (2 - i), yPos); break;
			case 2 : drawNum(staticlevel, num_2, xPos + 4 * (2 - i), yPos); break;
			case 3 : drawNum(staticlevel, num_3, xPos + 4 * (2 - i), yPos); break;
			case 4 : drawNum(staticlevel, num_4, xPos + 4 * (2 - i), yPos); break;
			case 5 : drawNum(staticlevel, num_5, xPos + 4 * (2 - i), yPos); break;
			case 6 : drawNum(staticlevel, num_6, xPos + 4 * (2 - i), yPos); break;
			case 7 : drawNum(staticlevel, num_7, xPos + 4 * (2 - i), yPos); break;
			case 8 : drawNum(staticlevel, num_8, xPos + 4 * (2 - i), yPos); break;
			case 9 : drawNum(staticlevel, num_9, xPos + 4 * (2 - i), yPos); break;
		}
	}
	//drawScuare(staticlevel, xPos - 2, yPos - 2, 3 * (NumWidth + 1) + 3, 1 * (NumHeight + 1) + 3);
}

void showTime(char staticlevel[ScreenHeight][ScreenWidth], int time, int xPos, int yPos){
	int razryad[4] = {0};
	int min = 0;
	int sec = 0;
	min = time / 60;
	sec = time % 60;
	razryad[0] = sec % 10;
	razryad[1] = (sec / 10) % 10;
	razryad[2] = min % 10;
	razryad[3] = (min / 10) % 10;
	for(long int i = 0; i < 2; i++){
		switch (razryad[i]) {
			case 0 : drawNum(staticlevel, num_0, xPos + 2 + 4 * (3 - i), yPos); break;
			case 1 : drawNum(staticlevel, num_1, xPos + 2 + 4 * (3 - i), yPos); break;
			case 2 : drawNum(staticlevel, num_2, xPos + 2 + 4 * (3 - i), yPos); break;
			case 3 : drawNum(staticlevel, num_3, xPos + 2 + 4 * (3 - i), yPos); break;
			case 4 : drawNum(staticlevel, num_4, xPos + 2 + 4 * (3 - i), yPos); break;
			case 5 : drawNum(staticlevel, num_5, xPos + 2 + 4 * (3 - i), yPos); break;
			case 6 : drawNum(staticlevel, num_6, xPos + 2 + 4 * (3 - i), yPos); break;
			case 7 : drawNum(staticlevel, num_7, xPos + 2 + 4 * (3 - i), yPos); break;
			case 8 : drawNum(staticlevel, num_8, xPos + 2 + 4 * (3 - i), yPos); break;
			case 9 : drawNum(staticlevel, num_9, xPos + 2 + 4 * (3 - i), yPos); break;
		}
	}
	for(long int i = 2; i < 4; i++){
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
	staticlevel[yPos + 1][xPos + 4 * 2] = 1;
	staticlevel[yPos + 3][xPos + 4 * 2] = 1;
}
/*
void drawLine(char layer[ScreenHeight][ScreenWidth], int xPos, int yPos, float length, float angle){
	int width = 0;
	if(cos(angle) >= 0.0 && sin(angle) >= 0.0){
		width = 1;
		int a_y = yPos;
		int b_y = yPos + (int)round(length * sin(angle));
		int a_x = xPos;
		int b_x = xPos + (int)round(length * cos(angle));
		for(int i = a_y; i < b_y; i++){
			for(int j = a_x; j < b_x; j++){
				if(((i - a_y) > (int)round((j + 0.5 - a_x) * tan(angle)) - width) && ((i - a_y) <= (int)round((j + 0.5 - a_x) * tan(angle)) + width))
					layer[i][j] = 1;
				else
					layer[i][j] = 0;
			}
		}
	}
	if(cos(angle) < 0.0 && sin(angle) >= 0.0){
		width = 1;
		int a_y = yPos;
		int b_y = yPos + (int)round(length * sin(angle));
		int a_x = xPos + (int)round(length * cos(angle));
		int b_x = xPos;
		for(int i = a_y; i < b_y; i++){
			for(int j = a_x; j < b_x; j++){
				if((i - a_y) >= (int)round((j - a_x) * (1/tan(angle)) - width) && ((i - a_y) < (int)round((j - a_x) * (1/tan(angle)) + width)))
					layer[i][j] = 1;
				else
					layer[i][j] = 0;
			}
		}
	}
	if(cos(angle) < 0.0 && sin(angle) < 0.0){
		width = 1;
		int a_y = yPos + (int)round(length * sin(angle));
		int b_y = yPos;
		int a_x = xPos + (int)round(length * cos(angle));
		int b_x = xPos;
		for(int i = a_y; i < b_y; i++){
			for(int j = a_x; j < b_x; j++){
				if(((i - a_y) >= (int)round((j - a_x) * (-1/tan(angle))) - width) && ((i - a_y) <= (int)round((j - a_x) * (-1/tan(angle))) + width))
					layer[i][j] = 1;
				else
					layer[i][j] = 0;
			}
		}
	}
	if(cos(angle) >= 0.0 && sin(angle) < 0.0){
		width = 1;
		int a_y = yPos + (int)round(length * sin(angle));
		int b_y = yPos;
		int a_x = xPos;
		int b_x = xPos + (int)round(length * cos(angle));
		for(int i = a_y; i < b_y; i++){
			for(int j = a_x; j < b_x; j++){
				if(((i - a_y) > (int)round((j - xPos) * tan(angle)) - width) && ((i - a_y) <= (int)round((j - xPos) * tan(angle)) + width))
					layer[i][j] = 1;
				else
					layer[i][j] = 0;
			}
		}
	}
}*/

void Timer1_IRQHandler (void){
	
	if(gameStatus == 0){
		drawBasketNet(basket, noncollisionlayer, basket_x, basket_y - basket_height, 1);
		showTime(noncollisionlayer, time_g, time_pos_x, time_pos_y);
		drawPlayer(player, gamefield, 16, 0, 1);
		drawBasket(gamefield, basket_x, basket_y, basket_width);
		if(drawtrajectory == 1){
			drawTrajectory(ball, noncollisionlayer);
		}
		else if(drawtrajectory == 0){
			clearTrajectory(ball, noncollisionlayer);			
		}
		PutObjOnMap(ball, gamefield);		
	}
	else if(gameStatus == 1){
		clearObject(ball, gamefield);
		drawBasket(gamefield, basket_x, basket_y, basket_width);
		movGravity(&ball);
		movUpdate(&ball, gamefield);
		checkScore(ball);
		showScore(noncollisionlayer, score_g, score_pos_x, score_pos_y);
		PutObjOnMap(ball, gamefield);
	}
	else if(gameStatus == 2){
		drawBasketNet(basket, noncollisionlayer, basket_x, basket_y - basket_height, 1);
		showTime(noncollisionlayer, time_g, time_pos_x, time_pos_y);
		drawPlayer(player, gamefield, 16, 0, 1);
		drawBasket(gamefield, basket_x, basket_y, basket_width);
		if(drawtrajectory == 1){
			drawTrajectory(ball, noncollisionlayer);
		}
		else if(drawtrajectory == 0){
			clearTrajectory(ball, noncollisionlayer);			
		}
		PutObjOnMap(ball, gamefield);	
	}
	
	ScreenUpdate(gamefield, noncollisionlayer, 1);
		
	MDR_TIMER1 -> STATUS = 0;//&= ~(1 << 0); //CNT_ZERO_EVENT
	NVIC->ICPR[0] &= ~(1 << 14);
}


void Timer2_IRQHandler (void){
	if(gameStatus != 1){
		if(button_CENTER)
			button_CENTER_pressed_cnt++;
		else{
			button_CENTER_pressed_cnt = 0;
			button_CENTER_pressed_cnt = 0;
		}
		if(button_CENTER_pressed_cnt == 100){
			gameStatus = 1;
			button_CENTER_pressed_cnt = 0;
			clearTrajectory(ball, noncollisionlayer);
			MDR_TIMER2 -> IE &= ~(1<<2);
		}
	}
	
	if(button_DOWN)
		button_DOWN_pressed_cnt++;
	else{
		button_DOWN_pressed_cnt = 0;
	}
	if(button_DOWN_pressed_cnt == button_press_delay){
		button_DOWN_pressed = 0;
		button_DOWN_pressed_cnt = 0;
	}
	
	if(button_UP)
		button_UP_pressed_cnt++;
	else
		button_UP_pressed_cnt = 0;
	if(button_UP_pressed_cnt == button_press_delay){
		button_UP_pressed = 0;
		button_UP_pressed_cnt = 0;
	}
	
	if(button_LEFT)
		button_LEFT_pressed_cnt++;
	else
		button_LEFT_pressed_cnt = 0;
	if(button_LEFT_pressed_cnt == button_press_delay){
		button_LEFT_pressed = 0;
		button_LEFT_pressed_cnt = 0;
	}
	
	if(button_RIGHT)
		button_RIGHT_pressed_cnt++;
	else
		button_RIGHT_pressed_cnt = 0;
	if(button_RIGHT_pressed_cnt == button_press_delay){
		button_RIGHT_pressed = 0;
		button_RIGHT_pressed_cnt = 0;
	}
	MDR_TIMER2 -> STATUS = 0;//&= ~(1 << 0); //CNT_ZERO_EVENT
	NVIC->ICPR[0] &= ~(1 << 15);
}

void Timer3_IRQHandler (void){
	if(gameStatus == 1 || gameStatus == 2){
		if(time_g == 0){
			clearObject(ball, gamefield);
			gameStatus = 0;
			MDR_TIMER3 -> IE &= ~(1<<3);
		}
		else
			time_g--;
	}
	showTime(noncollisionlayer, time_g, time_pos_x, time_pos_y);
	MDR_TIMER3 -> STATUS = 0;
	NVIC->ICPR[0] &= ~(1 << 16);
}