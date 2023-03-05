#include "figures.h"

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

char movDown(char staticlevel[20][10], struct figure *fig){ //if 0 - at the bottom
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
		}
	}
}

char isCollision(char staticlevel[20][10], struct figure fig){
	if(((fig.y + fig.bottompoint) < 0) || ((fig.x + fig.leftpoint) < 0) || ((fig.x + fig.rightpoint) > 9))
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

char movDown(char staticlevel[20][10], struct figure *fig){ //if 0 - at the bottom
	(*fig).y--;
	if(isCollision(staticlevel, (*fig))){
		(*fig).y++;
		return 0 ;
	}
	return 1;
}

void generateFigure(struct figure *fig){
	int n = 1; //kakoyto timer po osnovaniu 6
	switch (n) {
		case 0 : initFigure(fig, fig_I, nextFigure_x, nextFigure_y - minBottom(fig_I)); break;
		case 1 : initFigure(fig, fig_J, nextFigure_x, nextFigure_y); break;
		case 2 : initFigure(fig, fig_L, nextFigure_x, nextFigure_y); break;
		case 3 : initFigure(fig, fig_O, nextFigure_x, nextFigure_y); break;
		case 4 : initFigure(fig, fig_S, nextFigure_x, nextFigure_y); break;
		case 5 : initFigure(fig, fig_T, nextFigure_x, nextFigure_y); break;
		case 6 : initFigure(fig, fig_Z, nextFigure_x, nextFigure_y); break;	
	}
}

void deleteLines(char gamefield[20][10], int line_amount, int line_num[]){
	gameIsOn = 0;
	for(int k = 0; k < 5; k++){ //for animation
		for(int i = 0; i < line_amount; i++){
			gamefield[line_num[i]][4 - k] = 0;
			gamefield[line_num[i]][5 + k] = 0;
		}
		for(long int i = 0; i < 100000; i++);
	}
	for(int i = 0; i < line_amount; i++){
		for(int k = (line_num[line_amount - 1 - i] + 1); k < 20; k++){
			for(int j = 0; j < 10; j++){
				gamefield[k - 1][j] = gamefield[k][j];
			}
		}
	}
	for(int i = 16; i < 20; i++){
		for(int j = 0; j < 10; j++){
			gamefield[i][j] = 0; 
		}
	}
	gameIsOn = 1;
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
	return lines;
}
