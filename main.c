#include <lpc17xx.h>
#include <stdio.h>
#include <cmsis_os2.h> 
#include "stdlib.h" 
#include "uart.h"
#include "GLCD.h"
	 
int p1 = 0, p2 = 0, tie = 0; // scores
char s_p1[10],s_p2[10],s_tie[10]; // string scores
int matrix_size = 3; //size of matrix
int lag = 4; // delay between joystick movements by 1/lag seconds
// RESET SETTINGS
int ind_i = 0, ind_j = 0, ind_prev_i = 0, ind_prev_j = 0; // position
int matrix[3][3] = {{0,0,0},{0,0,0},{0,0,0}}; // board matrix
int turn = 1; // player turn
int count = 0; // turns taken
int win = 0; // if winner
int reset = 0; // reset the board

void turns(void *arg) {
	LPC_GPIO1->FIODIR |= 0xB0000000;
	LPC_GPIO1->FIOCLR |= 0xB0000000;
	LPC_GPIO2->FIODIR |= 0x0000007C;
	LPC_GPIO2->FIOCLR |= 0x0000007C;
	while(1) {
		if((LPC_GPIO1->FIOPIN & 0x00800000) == (!0x00800000)){ // UP BUTTON
			if(ind_j == 0){
				ind_prev_i = ind_i;
				ind_prev_j = ind_j;
				ind_j = 2;
			} else {
				ind_prev_i = ind_i;
				ind_prev_j = ind_j;
				ind_j--;
			}
			osDelay(osKernelGetTickFreq()/lag);
		}
		if((LPC_GPIO1->FIOPIN & 0x02000000) == (!0x02000000)){ // DOWN BUTTON
			if(ind_j == 2){
				ind_prev_i = ind_i;
				ind_prev_j = ind_j;
				ind_j = 0;
			} else {
				ind_prev_i = ind_i;
				ind_prev_j = ind_j;
				ind_j++;
			}
			osDelay(osKernelGetTickFreq()/lag);
		}
		if((LPC_GPIO1->FIOPIN & 0x04000000) == (!0x04000000)){ // LEFT BUTTON
			if(ind_i == 0){
				ind_prev_i = ind_i;
				ind_prev_j = ind_j;
				ind_i = 2;
			} else {
				ind_prev_i = ind_i;
				ind_prev_j = ind_j;
				ind_i--;
			}
			osDelay(osKernelGetTickFreq()/lag);
		}	
		if((LPC_GPIO1->FIOPIN & 0x01000000) == (!0x01000000)){ // RIGHT BUTTON
			if(ind_i == 2){
				ind_prev_i = ind_i;
				ind_prev_j = ind_j;
				ind_i = 0;
			} else {
				ind_prev_i = ind_i;
				ind_prev_j = ind_j;
				ind_i++;
			}
			osDelay(osKernelGetTickFreq()/lag);
		}
		if((LPC_GPIO2->FIOPIN & 0x00000400) == (!0x00000400)){ // BUTTON PRESSED
			if(matrix[ind_i][ind_j] == 0){
				matrix[ind_i][ind_j] = turn;
				turn = (turn%2)+1;
				count++;
			}
			osDelay(osKernelGetTickFreq()/lag);
		}
		osThreadYield();
	}
}

void board(void *arg){
	GLCD_Init();
	GLCD_Clear(Blue);
	GLCD_SetBackColor(255);
	GLCD_SetTextColor(65535);
	
	char *name_score = "SCORES";
	char *name_p1 = "X:";
	char *name_p2 = "O:";
	char *name_tie = "Tie:";
	char *h = "_";
	char *v = "|";

	//------------------ AXIS ------------------
	GLCD_DisplayString(0,0,1,(unsigned char*)name_score);
	GLCD_DisplayString(1,0,1,(unsigned char*)name_p1);
	GLCD_DisplayString(2,0,1,(unsigned char*)name_p2);
	GLCD_DisplayString(3,0,1,(unsigned char*)name_tie);
	
	//------------------ BOARD ------------------
	//Left border
	GLCD_DisplayString(0,11,1,(unsigned char*)v);
	GLCD_DisplayString(1,11,1,(unsigned char*)v);
	GLCD_DisplayString(2,11,1,(unsigned char*)v);
	GLCD_DisplayString(3,11,1,(unsigned char*)v);
	GLCD_DisplayString(4,11,1,(unsigned char*)v);
	GLCD_DisplayString(5,11,1,(unsigned char*)v);
	GLCD_DisplayString(6,11,1,(unsigned char*)v);
	GLCD_DisplayString(7,11,1,(unsigned char*)v);
	GLCD_DisplayString(8,11,1,(unsigned char*)v);

	//Right border
	GLCD_DisplayString(0,15,1,(unsigned char*)v);
	GLCD_DisplayString(1,15,1,(unsigned char*)v);
	GLCD_DisplayString(2,15,1,(unsigned char*)v);
	GLCD_DisplayString(3,15,1,(unsigned char*)v);
	GLCD_DisplayString(4,15,1,(unsigned char*)v);
	GLCD_DisplayString(5,15,1,(unsigned char*)v);
	GLCD_DisplayString(6,15,1,(unsigned char*)v);
	GLCD_DisplayString(7,15,1,(unsigned char*)v);
	GLCD_DisplayString(8,15,1,(unsigned char*)v);
	
	//Top border
	GLCD_DisplayString(2,8,1,(unsigned char*)h);
	GLCD_DisplayString(2,9,1,(unsigned char*)h);
	GLCD_DisplayString(2,10,1,(unsigned char*)h);
	//***skip line 11***
	GLCD_DisplayString(2,12,1,(unsigned char*)h);
	GLCD_DisplayString(2,13,1,(unsigned char*)h);
	GLCD_DisplayString(2,14,1,(unsigned char*)h);
	//***skip line 15**
	GLCD_DisplayString(2,16,1,(unsigned char*)h);
	GLCD_DisplayString(2,17,1,(unsigned char*)h);
	GLCD_DisplayString(2,18,1,(unsigned char*)h);
	
	//Bottom border
	GLCD_DisplayString(5,8,1,(unsigned char*)h);
	GLCD_DisplayString(5,9,1,(unsigned char*)h);
	GLCD_DisplayString(5,10,1,(unsigned char*)h);
	//***skip line 11***
	GLCD_DisplayString(5,12,1,(unsigned char*)h);
	GLCD_DisplayString(5,13,1,(unsigned char*)h);
	GLCD_DisplayString(5,14,1,(unsigned char*)h);
	//***skip line 15***
	GLCD_DisplayString(5,16,1,(unsigned char*)h);
	GLCD_DisplayString(5,17,1,(unsigned char*)h);
	GLCD_DisplayString(5,18,1,(unsigned char*)h);
	
	char *player = "";
	while(1) {
		// SCORES
		sprintf(s_p1,"%d",p1);
		sprintf(s_p2,"%d",p2);
		sprintf(s_tie,"%d",tie);
		GLCD_DisplayString(1,5,1,(unsigned char*)s_p1);
		GLCD_DisplayString(2,5,1,(unsigned char*)s_p2);
		GLCD_DisplayString(3,5,1,(unsigned char*)s_tie);
		
		// UPDATED BOARD 
		for (int i = 0; i < matrix_size; i++) {
			for (int j = 0; j < matrix_size; j++) {
				if(matrix[i][j] != 0){
					if(matrix[i][j] == 1){
						player = "X";
					} else if (matrix[i][j] == 2){
						player = "O";
					}
					GLCD_DisplayString((j*3+1),(i*4+9),1,(unsigned char*)player);
				}
			}
		}
		
		// CURSOR
		if(matrix[ind_prev_i][ind_prev_j] == 0){
			GLCD_DisplayString((ind_prev_j*3+1),(ind_prev_i*4+9),1,(unsigned char*)" ");
		}
		GLCD_DisplayString((ind_j*3+1),(ind_i*4+9),1,(unsigned char*)"#");
		
		if(reset == 1){
			turn = 1;
			count = 0;
			win = 0;
			reset = 0;
			for(int i = 0; i < matrix_size; i++){ // resets matrix and display
				for(int j = 0; j < matrix_size; j++){
					matrix[i][j] = 0;
					GLCD_DisplayString((j*3+1),(i*4+9),1,(unsigned char*)" ");
				}
			}
		}
		osThreadYield();
	}
}

void check_winner(void *arg){
	int val = 0;
	while(1){
		for(int i = 0; i < matrix_size; i++){ // check lines
			if(matrix[i][0] == matrix[i][1] && matrix[i][1] == matrix[i][2] && matrix[i][0] != 0){ // check row
				val = matrix[i][0];
				win = 1;
			}
			if(matrix[0][i] == matrix[1][i] && matrix[1][i] == matrix[2][i]&& matrix[0][i] != 0){ // check column
				val = matrix[0][i];
				win = 1;
			}
		}
		for(int i = 0; i < 2; i++){ // check diagnols
			if(matrix[0][i*2] == matrix[1][1] && matrix[1][1] == matrix[2][(i+1)%2*2] && matrix[0][i*2] != 0){ 
				val = matrix[0][i*2];
				win = 1;
			}
		}
		if(count == 9){
			tie++;
			reset = 1;
		}
    if(win == 1){
			if(val == 1){
				p1++;
			} else if(val == 2){
				p2++;
			}
			val = 0;
			osDelay(osKernelGetTickFreq()/4);
			reset = 1;
		}
		osThreadYield();
	}
}

int main(void) {
	osKernelInitialize();
	osThreadNew(board,NULL,NULL);
	osThreadNew(turns,NULL,NULL);	
	osThreadNew(check_winner,NULL,NULL);
	osKernelStart();
	while(1);
}
