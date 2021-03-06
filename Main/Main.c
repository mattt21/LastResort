
// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "TExaS.h"
#include "ADC.h"
#include "Collision_Detect.h"
#include "Timer0.h"
#include "Timer1.h"

#define UP											(*((volatile uint32_t *)0x40024004)) // PE-0
#define DOWN										(*((volatile uint32_t *)0x40024008)) // PE-1
#define LEFT										(*((volatile uint32_t *)0x40024010)) // PE-2		
#define RIGHT										(*((volatile uint32_t *)0x40024020)) // PE-3	
#define SHOOT										(*((volatile uint32_t *)0x40024040)) // PE-4
#define E												(*((volatile uint32_t *)0x40024080)) // PE-5
#define SLIDE										(*((volatile uint32_t *)0x40007004)) // PD-0
#define RESET 									(*((volatile uint32_t *)0x40024100)) // PE-6
#define PAUSE										(*((volatile uint32_t *)0x40024200)) // PE-7



int marinehealth = 60;
int monster1health = 0;
int monster2health = 0;
int monster3health = 0;
int m1 = 0;   						// 1 = small --  2 = medium --  3 = big
int m2 = 0;
int m3 = 0;
int marineDir = 1;
int monster1Dir = 0;
int monster2Dir = 0;
int monster3Dir = 0;
int monster1y = 0;
int monster1x = 0;
int monster2y = 0;
int monster2x = 0;
int monster3y = 0;
int monster3x = 0;
int marinex = 58;
int mariney = 72;
int rocknum = 1;
int rock1y = 0;
int rock2y = 0;
int rock3y = 0;
int rock4y = 0;
int rock1x = 0;
int rock2x = 0;
int rock3x = 0;
int rock4x = 0;
int gap1 = 0;
int gap2 = 0;
int gap3 = 0;

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds

void GameOver (void){
	
	DisableInterrupts();
	ST7735_FillScreen(0x0000);  
	
	while(1){
	ST7735_DrawBitmap(0, 95, dead, 128, 30);
	Delay100ms(1000);
	}
	
}
void drawmarinebulletdown(int x, int y){
		
		int marinebulletdownx = x ;
		int marinebulletdowny = y + 3;
		ST7735_DrawBitmap(marinebulletdownx, marinebulletdowny, BulletDown, 1, 4);
		while(marinebulletdowny < 165){
			marinebulletdowny ++;
			ST7735_DrawBitmap(marinebulletdownx, marinebulletdowny, BulletDown, 1, 4);
			Delay100ms(5);
				if ((wallCollision(marinebulletdowny, 145)) == 1) {
					marinebulletdowny-=4;
					ST7735_DrawFastVLine(marinebulletdownx,marinebulletdowny,5,0x7BEF);
					marinebulletdowny = (166);
				}
				if (((monsterCollisionX(marinebulletdownx, marinebulletdownx+1, rock1x, rock1x+6)) == 1) && (wallCollision(marinebulletdowny, rock1y-6) == 1)){
					marinebulletdowny-=4;
					ST7735_DrawFastVLine(marinebulletdownx,marinebulletdowny,5,0x7BEF);
					marinebulletdowny = (166);
				}
				if (((monsterCollisionX(marinebulletdownx, marinebulletdownx+1, rock2x, rock2x+6)) == 1) && (wallCollision(marinebulletdowny, rock2y-6) == 1)){
					marinebulletdowny-=4;
					ST7735_DrawFastVLine(marinebulletdownx,marinebulletdowny,5,0x7BEF);
					marinebulletdowny = (166);
				}
				if (((monsterCollisionX(marinebulletdownx, marinebulletdownx+1, rock3x, rock3x+6)) == 1) && (wallCollision(marinebulletdowny, rock3y-6) == 1)){
					marinebulletdowny-=4;
					ST7735_DrawFastVLine(marinebulletdownx,marinebulletdowny,5,0x7BEF);
					marinebulletdowny = (166);
				}
				if (((monsterCollisionX(marinebulletdownx, marinebulletdownx+1, rock4x, rock4x+6)) == 1) && (wallCollision(marinebulletdowny, rock4y-6) == 1)){
					marinebulletdowny-=4;
					ST7735_DrawFastVLine(marinebulletdownx,marinebulletdowny,5,0x7BEF);
					marinebulletdowny = (166);
				}
				if (m1 == 1){
					if (((monsterCollisionX(marinebulletdownx, marinebulletdownx+1, monster1x, monster1x+15)) == 1) && (wallCollision(marinebulletdowny, monster1y-15) == 1)){
					marinebulletdowny-=4;
					ST7735_DrawFastVLine(marinebulletdownx,marinebulletdowny,5,0x7BEF);
					marinebulletdowny = (166);
					monster1health --;
				}
				}
				if (m2 == 1){
					if (((monsterCollisionX(marinebulletdownx, marinebulletdownx+1, monster2x, monster2x+15)) == 1) && (wallCollision(marinebulletdowny, monster2y-15) == 1)){
					marinebulletdowny-=4;
					ST7735_DrawFastVLine(marinebulletdownx,marinebulletdowny,5,0x7BEF);
					marinebulletdowny = (166);
					monster2health --;
				}
				}
				if (m3 == 1){
					if (((monsterCollisionX(marinebulletdownx, marinebulletdownx+1, monster3x, monster3x+15)) == 1) && (wallCollision(marinebulletdowny, monster3y-15) == 1)){
					marinebulletdowny-=4;
					ST7735_DrawFastVLine(marinebulletdownx,marinebulletdowny,5,0x7BEF);
					marinebulletdowny = (166);
					monster3health --;
				}
				}
				if (m1 == 2){
					if (((monsterCollisionX(marinebulletdownx, marinebulletdownx+1, monster1x, monster1x+18)) == 1) && (wallCollision(marinebulletdowny, monster1y-18) == 1)){
					marinebulletdowny-=4;
					ST7735_DrawFastVLine(marinebulletdownx,marinebulletdowny,5,0x7BEF);
					marinebulletdowny = (166);
					monster1health --;
				}
				}
				if (m2 == 2){
					if (((monsterCollisionX(marinebulletdownx, marinebulletdownx+1, monster2x, monster2x+18)) == 1) && (wallCollision(marinebulletdowny, monster2y-18) == 1)){
					marinebulletdowny-=4;
					ST7735_DrawFastVLine(marinebulletdownx,marinebulletdowny,5,0x7BEF);
					marinebulletdowny = (166);
					monster2health --;
				}
				}
				if (m3 == 2){
					if (((monsterCollisionX(marinebulletdownx, marinebulletdownx+1, monster3x, monster3x+18)) == 1) && (wallCollision(marinebulletdowny, monster3y-18) == 1)){
					marinebulletdowny-=4;
					ST7735_DrawFastVLine(marinebulletdownx,marinebulletdowny,5,0x7BEF);
					marinebulletdowny = (166);
					monster3health --;
				}
				}
				if (m1 == 3){
					if (((monsterCollisionX(marinebulletdownx, marinebulletdownx+1, monster1x, monster1x+24)) == 1) && (wallCollision(marinebulletdowny, monster1y-25) == 1)){
					marinebulletdowny-=4;
					ST7735_DrawFastVLine(marinebulletdownx,marinebulletdowny,5,0x7BEF);
					marinebulletdowny = (166);
					monster1health --;
				}
				}
				if (m2 == 3){
					if (((monsterCollisionX(marinebulletdownx, marinebulletdownx+1, monster2x, monster2x+24)) == 1) && (wallCollision(marinebulletdowny, monster2y-25) == 1)){
					marinebulletdowny-=4;
					ST7735_DrawFastVLine(marinebulletdownx,marinebulletdowny,5,0x7BEF);
					marinebulletdowny = (166);
					monster2health --;
				}
				}
				if (m3 == 3){
					if (((monsterCollisionX(marinebulletdownx, marinebulletdownx+1, monster3x, monster3x+24)) == 1) && (wallCollision(marinebulletdowny, monster3y-25) == 1)){
					marinebulletdowny-=4;
					ST7735_DrawFastVLine(marinebulletdownx,marinebulletdowny,5,0x7BEF);
					marinebulletdowny = (166);
					monster3health --;
				}
				}
			
		}
		
}

void drawmarinebulletup(int x, int y){
		int marinebulletupx = x + 12;
		int marinebulletupy = y - 15;
		ST7735_DrawBitmap(marinebulletupx, marinebulletupy, BulletUp, 1, 4);
		while(marinebulletupy > 0){
			marinebulletupy --;
			ST7735_DrawBitmap(marinebulletupx, marinebulletupy, BulletUp, 1, 4);
			Delay100ms(5);
			if ((wallCollision(marinebulletupy-4, 11)) == 1) {
					marinebulletupy -=4;
					ST7735_DrawFastVLine(marinebulletupx,marinebulletupy,4,0x7BEF);
					marinebulletupy = (0);
				}
			if (((monsterCollisionX(marinebulletupx, marinebulletupx+1, rock1x, rock1x+6)) == 1) && (wallCollision(marinebulletupy-4, rock1y) == 1)){
					marinebulletupy -= 4;
					ST7735_DrawFastVLine(marinebulletupx,marinebulletupy,4,0x7BEF);
					marinebulletupy = (0);
				}
			if (((monsterCollisionX(marinebulletupx, marinebulletupx+1, rock2x, rock2x+6)) == 1) && (wallCollision(marinebulletupy-4, rock2y) == 1)){
					marinebulletupy -=4;
					ST7735_DrawFastVLine(marinebulletupx,marinebulletupy,4,0x7BEF);	
					marinebulletupy = (0);
			}
			if (((monsterCollisionX(marinebulletupx, marinebulletupx+1, rock3x, rock3x+6)) == 1) && (wallCollision(marinebulletupy-4, rock3y) == 1)){
					marinebulletupy -=4;
					ST7735_DrawFastVLine(marinebulletupx,marinebulletupy,4,0x7BEF);
					marinebulletupy = (0);
				}
			if (((monsterCollisionX(marinebulletupx, marinebulletupx+1, rock4x, rock4x+6)) == 1) && (wallCollision(marinebulletupy-4, rock4y) == 1)){
					marinebulletupy -=4;
					ST7735_DrawFastVLine(marinebulletupx,marinebulletupy,4,0x7BEF);
					marinebulletupy = (0);
				}
				
				if (m1 == 1){
				if (((monsterCollisionX(marinebulletupx, marinebulletupx+1, monster1x, monster1x+15)) == 1) && (wallCollision(marinebulletupy-4, monster1y) == 1)){
					marinebulletupy -=4;
					ST7735_DrawFastVLine(marinebulletupx,marinebulletupy,4,0x7BEF);
					marinebulletupy = 0;
					monster1health --;
				}
			}
				if (m2 == 1){
				if (((monsterCollisionX(marinebulletupx, marinebulletupx+1, monster2x, monster2x+15)) == 1) && (wallCollision(marinebulletupy-4, monster2y) == 1)){
					marinebulletupy -=4;
					ST7735_DrawFastVLine(marinebulletupx,marinebulletupy,4,0x7BEF);
					marinebulletupy = (0);
					monster2health --;
				}
			}
			if (m3 == 1){
				if (((monsterCollisionX(marinebulletupx, marinebulletupx+1, monster3x, monster3x+15)) == 1) && (wallCollision(marinebulletupy-4, monster3y) == 1)){
					marinebulletupy -=4;
					ST7735_DrawFastVLine(marinebulletupx,marinebulletupy,4,0x7BEF);
					marinebulletupy = (0);
					monster3health --;
				}
			}
			if (m1 == 2){	
				if (((monsterCollisionX(marinebulletupx, marinebulletupx+1, monster1x, monster1x+18)) == 1) && (wallCollision(marinebulletupy-4, monster1y) == 1)){
					marinebulletupy -=4;
					ST7735_DrawFastVLine(marinebulletupx,marinebulletupy,4,0x7BEF);
					marinebulletupy = (0);
					monster1health --;
				}
			}
			if (m2 == 2){	
				if (((monsterCollisionX(marinebulletupx, marinebulletupx+1, monster2x, monster2x+18)) == 1) && (wallCollision(marinebulletupy-4, monster2y) == 1)){
					marinebulletupy -=4;
					ST7735_DrawFastVLine(marinebulletupx,marinebulletupy,4,0x7BEF);
					marinebulletupy = (0);
					monster2health --;
				}
			}
			if (m3 == 2){	
				if (((monsterCollisionX(marinebulletupx, marinebulletupx+1, monster3x, monster3x+18)) == 1) && (wallCollision(marinebulletupy-4, monster3y) == 1)){
					marinebulletupy -=4;
					ST7735_DrawFastVLine(marinebulletupx,marinebulletupy,4,0x7BEF);
					marinebulletupy = (0);
					monster3health --;
				}
			}
			if (m1 == 3){
				if (((monsterCollisionX(marinebulletupx, marinebulletupx+1, monster1x, monster1x+24)) == 1) && (wallCollision(marinebulletupy-4, monster1y) == 1)){
					marinebulletupy -=4;
					ST7735_DrawFastVLine(marinebulletupx,marinebulletupy,4,0x7BEF);
					marinebulletupy = (0);
					monster1health --;
				}
			}
			if (m2 == 3){
				if (((monsterCollisionX(marinebulletupx, marinebulletupx+1, monster2x, monster2x+24)) == 1) && (wallCollision(marinebulletupy-4, monster2y) == 1)){
					marinebulletupy -=4;
					ST7735_DrawFastVLine(marinebulletupx,marinebulletupy,4,0x7BEF);
					marinebulletupy = (0);
					monster2health --;
				}
			}
			if (m3 == 3){
				if (((monsterCollisionX(marinebulletupx, marinebulletupx+1, monster3x, monster3x+24)) == 1) && (wallCollision(marinebulletupy-4, monster3y) == 1)){
					marinebulletupy -=4;
					ST7735_DrawFastVLine(marinebulletupx,marinebulletupy,4,0x7BEF);
					marinebulletupy = (0-10);
					monster3health --;
				}
			}					
		}
}

void drawmarinebulletleft(int x, int y){
		int marinebulletleftx = x - 2;
		int marinebulletlefty = y - 12;
		ST7735_DrawBitmap(marinebulletleftx, marinebulletlefty, BulletLeft, 4, 1);
		while(marinebulletleftx > 10){
			marinebulletleftx --;
			ST7735_DrawBitmap(marinebulletleftx, marinebulletlefty, BulletLeft, 4,1);
			Delay100ms(5);
			if ((wallCollision(marinebulletleftx, 11)) == 1) {
					ST7735_DrawFastHLine(marinebulletleftx,marinebulletlefty,4,0x7BEF);
					marinebulletleftx = (10);
				}
			if (((monsterCollisionY(marinebulletlefty, marinebulletlefty-1, rock1y, rock1y-6)) == 1) && (wallCollision(marinebulletleftx, rock1x+6) == 1)){
					ST7735_DrawFastHLine(marinebulletleftx,marinebulletlefty,4,0x7BEF);
					marinebulletleftx = (10);
				}
			if (((monsterCollisionY(marinebulletlefty, marinebulletlefty-1, rock2y, rock2y-6)) == 1) && (wallCollision(marinebulletleftx, rock2x+6) == 1)){
					ST7735_DrawFastHLine(marinebulletleftx,marinebulletlefty,4,0x7BEF);
					marinebulletleftx = (10);				}
			if (((monsterCollisionY(marinebulletlefty, marinebulletlefty-1, rock3y, rock3y-6)) == 1) && (wallCollision(marinebulletleftx, rock3x+6) == 1)){
					ST7735_DrawFastHLine(marinebulletleftx,marinebulletlefty,4,0x7BEF);
					marinebulletleftx = (10);
				}
			if (((monsterCollisionY(marinebulletlefty, marinebulletlefty-1, rock4y, rock4y-6)) == 1) && (wallCollision(marinebulletleftx, rock4x+6) == 1)){
					ST7735_DrawFastHLine(marinebulletleftx,marinebulletlefty,4,0x7BEF);
					marinebulletleftx = (10);
				}
			if (m1 == 1){
				if (((monsterCollisionY(marinebulletlefty, marinebulletlefty-1, monster1y, monster1y-15)) == 1) && (wallCollision(marinebulletleftx, monster1x+15) == 1)){
					ST7735_DrawFastHLine(marinebulletleftx,marinebulletlefty,4,0x7BEF);
					marinebulletleftx = (10);
					monster1health--;
				}
			}
			if (m2 == 1){
				if (((monsterCollisionY(marinebulletlefty, marinebulletlefty-1, monster2y, monster2y-15)) == 1) && (wallCollision(marinebulletleftx, monster2x+15) == 1)){
					ST7735_DrawFastHLine(marinebulletleftx,marinebulletlefty,4,0x7BEF);
					marinebulletleftx = (10);
					monster2health--;
				}
			}
			if (m3 == 1){
				if (((monsterCollisionY(marinebulletlefty, marinebulletlefty-1, monster3y, monster3y-15)) == 1) && (wallCollision(marinebulletleftx, monster3x+15) == 1)){
					ST7735_DrawFastHLine(marinebulletleftx,marinebulletlefty,4,0x7BEF);
					marinebulletleftx = (10);
					monster3health--;
				}
			}
			if (m1 == 2){
				if (((monsterCollisionY(marinebulletlefty, marinebulletlefty-1, monster1y, monster1y-18)) == 1) && (wallCollision(marinebulletleftx, monster1x+18) == 1)){
					ST7735_DrawFastHLine(marinebulletleftx,marinebulletlefty,4,0x7BEF);
					marinebulletleftx = (10);
					monster1health--;
				}
			}
			if (m2 == 2){
				if (((monsterCollisionY(marinebulletlefty, marinebulletlefty-1, monster2y, monster2y-18)) == 1) && (wallCollision(marinebulletleftx, monster2x+18) == 1)){
					ST7735_DrawFastHLine(marinebulletleftx,marinebulletlefty,4,0x7BEF);
					marinebulletleftx = (10);
					monster2health--;
				}
			}
			if (m3 == 2){
				if (((monsterCollisionY(marinebulletlefty, marinebulletlefty-1, monster3y, monster3y-18)) == 1) && (wallCollision(marinebulletleftx, monster3x+18) == 1)){
					ST7735_DrawFastHLine(marinebulletleftx,marinebulletlefty,4,0x7BEF);
					marinebulletleftx = (10);
					monster3health--;
				}
			}
			if (m1 == 3){
				if (((monsterCollisionY(marinebulletlefty, marinebulletlefty-1, monster1y, monster1y-25)) == 1) && (wallCollision(marinebulletleftx, monster1x+25) == 1)){
					marinebulletleftx ++ ;
					ST7735_DrawFastHLine(marinebulletleftx,marinebulletlefty,4,0x7BEF);
					marinebulletleftx = (10);
					monster1health--;
				}
			}
			if (m2 == 3){
				if (((monsterCollisionY(marinebulletlefty, marinebulletlefty-1, monster2y, monster2y-25)) == 1) && (wallCollision(marinebulletleftx, monster2x+25) == 1)){
					marinebulletleftx ++;
					ST7735_DrawFastHLine(marinebulletleftx,marinebulletlefty,4,0x7BEF);
					marinebulletleftx = (10);
					monster2health--;
				}
			}
				if (m3 == 3){
				if (((monsterCollisionY(marinebulletlefty, marinebulletlefty-1, monster3y, monster3y-25)) == 1) && (wallCollision(marinebulletleftx, monster3x+25) == 1)){
					marinebulletleftx ++;
					ST7735_DrawFastHLine(marinebulletleftx,marinebulletlefty,4,0x7BEF);
					marinebulletleftx = (10);
					monster3health--;
				}
			}
		}
	}

void drawmarinebulletright(int x, int y){
		int marinebulletrightx = x + 15;
		int marinebulletrighty = y - 1;
		ST7735_DrawBitmap(marinebulletrightx, marinebulletrighty, BulletRight, 4, 1);
		while(marinebulletrightx < 135){
			marinebulletrightx ++;
			ST7735_DrawBitmap(marinebulletrightx, marinebulletrighty, BulletRight, 4,1);
			Delay100ms(5);
			if ((wallCollision(marinebulletrightx, 113)) == 1) {
					ST7735_DrawFastHLine(marinebulletrightx,marinebulletrighty,4,0x7BEF);
					marinebulletrightx = (136);
				}	
			if (((monsterCollisionY(marinebulletrighty, marinebulletrighty-1, rock1y, rock1y-6)) == 1) && ((wallCollision(marinebulletrightx+1, rock1x)) == 1) ){
					ST7735_DrawFastHLine(marinebulletrightx,marinebulletrighty,4,0x7BEF);
					marinebulletrightx = (136);
				}
			if (((monsterCollisionY(marinebulletrighty, marinebulletrighty-1, rock2y, rock2y-6)) == 1) && ((wallCollision(marinebulletrightx+1, rock2x)) == 1) ){
					ST7735_DrawFastHLine(marinebulletrightx,marinebulletrighty,4,0x7BEF);
					marinebulletrightx = (136);
				}
			if (((monsterCollisionY(marinebulletrighty, marinebulletrighty-1, rock3y, rock3y-6)) == 1) && ((wallCollision(marinebulletrightx+1, rock3x)) == 1) ){
					ST7735_DrawFastHLine(marinebulletrightx,marinebulletrighty,4,0x7BEF);
					marinebulletrightx = (136);
				}
			if (((monsterCollisionY(marinebulletrighty, marinebulletrighty-1, rock4y, rock4y-6)) == 1) && ((wallCollision(marinebulletrightx+1, rock4x)) == 1) ){
					ST7735_DrawFastHLine(marinebulletrightx,marinebulletrighty,4,0x7BEF);
					marinebulletrightx = (136);
				}
			if( m1 == 1){
				if (((monsterCollisionY(marinebulletrighty, marinebulletrighty-1, monster1y, monster1y-15)) == 1) && ((wallCollision(marinebulletrightx+1, monster1x)) == 1) ){
					ST7735_DrawFastHLine(marinebulletrightx,marinebulletrighty,4,0x7BEF);
					marinebulletrightx = (136);
					monster1health --;
				}
			}
			if( m2 == 1){
				if (((monsterCollisionY(marinebulletrighty, marinebulletrighty-1, monster2y, monster2y-15)) == 1) && ((wallCollision(marinebulletrightx+1, monster2x)) == 1) ){
					ST7735_DrawFastHLine(marinebulletrightx,marinebulletrighty,4,0x7BEF);
					marinebulletrightx = (136);
					monster2health --;
				}
			}
			if( m3 == 1){
				if (((monsterCollisionY(marinebulletrighty, marinebulletrighty-1, monster3y, monster3y-15)) == 1) && ((wallCollision(marinebulletrightx+1, monster3x)) == 1) ){
					ST7735_DrawFastHLine(marinebulletrightx,marinebulletrighty,4,0x7BEF);
					marinebulletrightx = (136);
					monster3health --;
				}
			}
			if( m1 == 2){
				if (((monsterCollisionY(marinebulletrighty, marinebulletrighty-1, monster1y, monster1y-18)) == 1) && ((wallCollision(marinebulletrightx+1, monster1x)) == 1) ){
					ST7735_DrawFastHLine(marinebulletrightx,marinebulletrighty,4,0x7BEF);
					marinebulletrightx = (136);
					monster1health --;
				}
			}
			if( m2 == 2){
				if (((monsterCollisionY(marinebulletrighty, marinebulletrighty-1, monster2y, monster2y-18)) == 1) && ((wallCollision(marinebulletrightx+1, monster2x)) == 1) ){
					ST7735_DrawFastHLine(marinebulletrightx,marinebulletrighty,4,0x7BEF);
					marinebulletrightx = (136);
					monster2health --;
				}
			}
			if( m3 == 2){
				if (((monsterCollisionY(marinebulletrighty, marinebulletrighty-1, monster3y, monster3y-18)) == 1) && ((wallCollision(marinebulletrightx+1, monster3x)) == 1) ){
					ST7735_DrawFastHLine(marinebulletrightx,marinebulletrighty,4,0x7BEF);
					marinebulletrightx = (136);
					monster3health --;
				}
			}
			if( m1 == 3){
				if (((monsterCollisionY(marinebulletrighty, marinebulletrighty-1, monster1y, monster1y-25)) == 1) && ((wallCollision(marinebulletrightx+1, monster1x)) == 1) ){
					ST7735_DrawFastHLine(marinebulletrightx,marinebulletrighty,4,0x7BEF);
					marinebulletrightx = (136);
					monster1health --;
				}
			}
			if( m2 == 3){
				if (((monsterCollisionY(marinebulletrighty, marinebulletrighty-1, monster2y, monster2y-25)) == 1) && ((wallCollision(marinebulletrightx+1, monster2x)) == 1) ){
					ST7735_DrawFastHLine(marinebulletrightx,marinebulletrighty,4,0x7BEF);
					marinebulletrightx = (136);
					monster2health --;
				}
			}
			if( m3 == 3){
				if (((monsterCollisionY(marinebulletrighty, marinebulletrighty-1, monster3y, monster3y-25)) == 1) && ((wallCollision(marinebulletrightx+1, monster3x)) == 1) ){
					ST7735_DrawFastHLine(marinebulletrightx,marinebulletrighty,4,0x7BEF);
					marinebulletrightx = (136);
					monster3health --;
				}
			}	
		}
	}








void drawmonster2bulletdown (int x, int y){
		int monster2bulletdown1x = x + 1;
		int monster2bulletdown1y = y + 6;
		int monster2bulletdown2x = x + 16;
		int monster2bulletdown2y = y + 6;
		ST7735_DrawBitmap(monster2bulletdown1x, monster2bulletdown1y, BulletMonsterDown, 1, 6);
		ST7735_DrawBitmap(monster2bulletdown2x, monster2bulletdown2y, BulletMonsterDown, 1, 6);
		while(monster2bulletdown1y < 165 || monster2bulletdown2y < 165)
			{
				monster2bulletdown1y ++;
				monster2bulletdown2y ++;
				ST7735_DrawBitmap(monster2bulletdown1x, monster2bulletdown1y, BulletMonsterDown, 1, 6);
				ST7735_DrawBitmap(monster2bulletdown2x, monster2bulletdown2y, BulletMonsterDown, 1, 6);
				Delay100ms(5);
				if ((wallCollision(monster2bulletdown1y, 143)) == 1) {
					monster2bulletdown1y-=6;
					ST7735_DrawFastVLine(monster2bulletdown1x,monster2bulletdown1y,7,0x7BEF);
					monster2bulletdown1y = (166);
				}
				if ((wallCollision(monster2bulletdown2y, 143)) == 1) {
					monster2bulletdown2y-=6;
					ST7735_DrawFastVLine(monster2bulletdown2x,monster2bulletdown2y,7,0x7BEF);
					monster2bulletdown2y = (166);
				}
				if (((monsterCollisionX(monster2bulletdown1x, monster2bulletdown1x+1, rock1x, rock1x+6)) == 1) && (wallCollision(monster2bulletdown1y, rock1y-6) == 1)){
					monster2bulletdown1y-=6;
					ST7735_DrawFastVLine(monster2bulletdown1x,monster2bulletdown1y,7,0x7BEF);
					monster2bulletdown1y = (166);
				}
				if (((monsterCollisionX(monster2bulletdown2x, monster2bulletdown2x+1, rock1x, rock1x+6)) == 1) && (wallCollision(monster2bulletdown2y, rock1y-6) == 1)){
					monster2bulletdown2y-=6;
					ST7735_DrawFastVLine(monster2bulletdown2x,monster2bulletdown2y,7,0x7BEF);
					monster2bulletdown2y = (166);
				}
				if (((monsterCollisionX(monster2bulletdown1x, monster2bulletdown1x+1, rock2x, rock2x+6)) == 1) && (wallCollision(monster2bulletdown1y, rock2y-6) == 1)){
					monster2bulletdown1y-=6;
					ST7735_DrawFastVLine(monster2bulletdown1x,monster2bulletdown1y,7,0x7BEF);
					monster2bulletdown1y = (166);
				}
				if (((monsterCollisionX(monster2bulletdown2x, monster2bulletdown2x+1, rock2x, rock2x+6)) == 1) && (wallCollision(monster2bulletdown2y, rock2y-6) == 1)){
					monster2bulletdown2y-=6;
					ST7735_DrawFastVLine(monster2bulletdown2x,monster2bulletdown2y,7,0x7BEF);
					monster2bulletdown2y = (166);
				}
				if (((monsterCollisionX(monster2bulletdown1x, monster2bulletdown1x+1, rock3x, rock3x+6)) == 1) && (wallCollision(monster2bulletdown1y, rock3y-6) == 1)){
					monster2bulletdown1y-=6;
					ST7735_DrawFastVLine(monster2bulletdown1x,monster2bulletdown1y,7,0x7BEF);
					monster2bulletdown1y = (166);
				}
				if (((monsterCollisionX(monster2bulletdown2x, monster2bulletdown2x+1, rock3x, rock3x+6)) == 1) && (wallCollision(monster2bulletdown2y, rock3y-6) == 1)){
					monster2bulletdown2y-=6;
					ST7735_DrawFastVLine(monster2bulletdown2x,monster2bulletdown2y,7,0x7BEF);
					monster2bulletdown2y = (166);
				}
				if (((monsterCollisionX(monster2bulletdown1x, monster2bulletdown1x+1, rock4x, rock4x+6)) == 1) && (wallCollision(monster2bulletdown1y, rock4y-6) == 1)){
					monster2bulletdown1y-=6;
					ST7735_DrawFastVLine(monster2bulletdown1x,monster2bulletdown1y,7,0x7BEF);
					monster2bulletdown1y = (166);
				}
				if (((monsterCollisionX(monster2bulletdown2x, monster2bulletdown2x+1, rock4x, rock4x+6)) == 1) && (wallCollision(monster2bulletdown2y, rock4y-6) == 1)){
					monster2bulletdown2y-=6;
					ST7735_DrawFastVLine(monster2bulletdown2x,monster2bulletdown2y,7,0x7BEF);
					monster2bulletdown2y = (166);
				}
				if (((monsterCollisionX(monster2bulletdown1x, monster2bulletdown1x+1, marinex, marinex+12)) == 1) && (wallCollision(monster2bulletdown1y, mariney-12) == 1)){
					monster2bulletdown1y-=6;
					ST7735_DrawFastVLine(monster2bulletdown1x,monster2bulletdown1y,7,0x7BEF);
					monster2bulletdown1y = (166);
					marinehealth -= 10;
				}
				if (((monsterCollisionX(monster2bulletdown2x, monster2bulletdown2x+1, marinex, marinex+12)) == 1) && (wallCollision(monster2bulletdown2y, mariney-12) == 1)){
					monster2bulletdown2y-=6;
					ST7735_DrawFastVLine(monster2bulletdown2x,monster2bulletdown2y,7,0x7BEF);
					monster2bulletdown2y = (166);
					marinehealth -= 10;
				}
			}			
}

void drawmonster2bulletup (int x, int y){
		int monster2bulletup1x = x + 1;
		int monster2bulletup1y = y - 20;
		int monster2bulletup2x = x + 16;
		int monster2bulletup2y = y - 20;
		ST7735_DrawBitmap(monster2bulletup1x, monster2bulletup1y, BulletMonsterUp, 1, 6);
		ST7735_DrawBitmap(monster2bulletup2x, monster2bulletup2y, BulletMonsterUp, 1, 6);
		while(monster2bulletup1y > 0 || monster2bulletup2y > 0)
			{
				monster2bulletup1y --;
				monster2bulletup2y --;
				ST7735_DrawBitmap(monster2bulletup1x, monster2bulletup1y, BulletMonsterUp, 1, 6);
				ST7735_DrawBitmap(monster2bulletup2x, monster2bulletup2y, BulletMonsterUp, 1, 6);
				Delay100ms(5);
				if ((wallCollision(monster2bulletup1y-6, 11)) == 1) {
					monster2bulletup1y -= 6;
					ST7735_DrawFastVLine(monster2bulletup1x,monster2bulletup1y,6,0x7BEF);
					monster2bulletup1y = (0);
				}
				if ((wallCollision(monster2bulletup2y-6, 11)) == 1) {
					monster2bulletup2y -= 6;
					ST7735_DrawFastVLine(monster2bulletup2x,monster2bulletup2y,6,0x7BEF);
					monster2bulletup2y = (0);
				}
				if (((monsterCollisionX(monster2bulletup1x, monster2bulletup1x+1, rock1x, rock1x+6)) == 1) && (wallCollision(monster2bulletup1y-6, rock1y) == 1)){
					monster2bulletup1y -= 6;
					ST7735_DrawFastVLine(monster2bulletup1x,monster2bulletup1y,6,0x7BEF);
					monster2bulletup1y = (0);
				}
				if (((monsterCollisionX(monster2bulletup2x, monster2bulletup2x+1, rock1x, rock1x+6)) == 1) && (wallCollision(monster2bulletup2y-6, rock1y) == 1)){
					monster2bulletup2y -= 6;
					ST7735_DrawFastVLine(monster2bulletup2x,monster2bulletup2y,6,0x7BEF);
					monster2bulletup2y = (0);
				}
				if (((monsterCollisionX(monster2bulletup1x, monster2bulletup1x+1, rock2x, rock2x+6)) == 1) && (wallCollision(monster2bulletup1y-6, rock2y) == 1)){
					monster2bulletup1y -= 6;
					ST7735_DrawFastVLine(monster2bulletup1x,monster2bulletup1y,6,0x7BEF);
					monster2bulletup1y = (0);
				}
				if (((monsterCollisionX(monster2bulletup2x, monster2bulletup2x+1, rock2x, rock2x+6)) == 1) && (wallCollision(monster2bulletup2y-6, rock2y) == 1)){
					monster2bulletup2y -= 6;
					ST7735_DrawFastVLine(monster2bulletup2x,monster2bulletup2y,6,0x7BEF);
					monster2bulletup2y = (0);
				}
				if (((monsterCollisionX(monster2bulletup1x, monster2bulletup1x+1, rock3x, rock3x+6)) == 1) && (wallCollision(monster2bulletup1y-6, rock3y) == 1)){
					monster2bulletup1y -= 6;
					ST7735_DrawFastVLine(monster2bulletup1x,monster2bulletup1y,6,0x7BEF);
					monster2bulletup1y = (0);
				}
				if (((monsterCollisionX(monster2bulletup2x, monster2bulletup2x+1, rock3x, rock3x+6)) == 1) && (wallCollision(monster2bulletup2y-6, rock3y) == 1)){
					monster2bulletup2y -= 6;
					ST7735_DrawFastVLine(monster2bulletup2x,monster2bulletup2y,6,0x7BEF);
					monster2bulletup2y = (0);
				}
				if (((monsterCollisionX(monster2bulletup1x, monster2bulletup1x+1, rock4x, rock4x+6)) == 1) && (wallCollision(monster2bulletup1y-6, rock4y) == 1)){
					monster2bulletup1y -= 6;
					ST7735_DrawFastVLine(monster2bulletup1x,monster2bulletup1y,6,0x7BEF);
					monster2bulletup1y = (0);
				}
				if (((monsterCollisionX(monster2bulletup2x, monster2bulletup2x+1, rock4x, rock4x+6)) == 1) && (wallCollision(monster2bulletup2y-6, rock4y) == 1)){
					monster2bulletup2y -= 6;
					ST7735_DrawFastVLine(monster2bulletup2x,monster2bulletup2y,6,0x7BEF);
					monster2bulletup2y = (0);
				}
				if (((monsterCollisionX(monster2bulletup1x, monster2bulletup1x+1, marinex, marinex+12)) == 1) && (wallCollision(monster2bulletup1y-6, mariney) == 1)){
					monster2bulletup1y -=6;
					ST7735_DrawFastVLine(monster2bulletup1x,monster2bulletup1y,6,0x7BEF);
					monster2bulletup1y = 0;
					marinehealth -= 10;
				}
				if (((monsterCollisionX(monster2bulletup2x, monster2bulletup2x+1, marinex, marinex+12)) == 1) && (wallCollision(monster2bulletup2y-6, mariney) == 1)){
					monster2bulletup2y -=6;
					ST7735_DrawFastVLine(monster2bulletup2x,monster2bulletup2y,6,0x7BEF);
					monster2bulletup2y = 0;
					marinehealth -= 10;
				}
			}			
}

void drawmonster2bulletleft (int x, int y){
		int monster2bulletleft1x = x - 8;
		int monster2bulletleft1y = y - 1;
		int monster2bulletleft2x = x - 8;
		int monster2bulletleft2y = y - 16;
		ST7735_DrawBitmap(monster2bulletleft1x, monster2bulletleft1y, BulletMonsterLeft, 6, 1);
		ST7735_DrawBitmap(monster2bulletleft2x, monster2bulletleft2y, BulletMonsterLeft, 6, 1);
		while(monster2bulletleft1x > 10 || monster2bulletleft2x > 10)
			{
				monster2bulletleft1x --;
				monster2bulletleft2x --;
				ST7735_DrawBitmap(monster2bulletleft1x, monster2bulletleft1y, BulletMonsterLeft, 6, 1);
				ST7735_DrawBitmap(monster2bulletleft2x, monster2bulletleft2y, BulletMonsterLeft, 6, 1);
				Delay100ms(5);
				if ((wallCollision(monster2bulletleft1x, 11)) == 1) {
					ST7735_DrawFastHLine(monster2bulletleft1x,monster2bulletleft1y,7,0x7BEF);
					monster2bulletleft1x = (10);
				}
				if ((wallCollision(monster2bulletleft2x, 11)) == 1) {
					ST7735_DrawFastHLine(monster2bulletleft2x,monster2bulletleft2y,7,0x7BEF);
					monster2bulletleft2x = (10);
				}
				if (((monsterCollisionY(monster2bulletleft1y, monster2bulletleft1y-1, rock1y, rock1y-6)) == 1) && (wallCollision(monster2bulletleft1x, rock1x+6) == 1)){
					ST7735_DrawFastHLine(monster2bulletleft1x,monster2bulletleft1y,7,0x7BEF);
					monster2bulletleft1x = (10);
				}
				if (((monsterCollisionY(monster2bulletleft2y, monster2bulletleft2y-1, rock1y, rock1y-6)) == 1) && (wallCollision(monster2bulletleft2x, rock1x+6) == 1)){
					ST7735_DrawFastHLine(monster2bulletleft2x,monster2bulletleft2y,7,0x7BEF);
					monster2bulletleft2x = (10);
				}
				if (((monsterCollisionY(monster2bulletleft1y, monster2bulletleft1y-1, rock2y, rock2y-6)) == 1) && (wallCollision(monster2bulletleft1x, rock2x+6) == 1)){
					ST7735_DrawFastHLine(monster2bulletleft1x,monster2bulletleft1y,7,0x7BEF);
					monster2bulletleft1x = (10);
				}
				if (((monsterCollisionY(monster2bulletleft2y, monster2bulletleft2y-1, rock2y, rock2y-6)) == 1) && (wallCollision(monster2bulletleft2x, rock2x+6) == 1)){
					ST7735_DrawFastHLine(monster2bulletleft2x,monster2bulletleft2y,7,0x7BEF);
					monster2bulletleft2x = (10);
				}
				if (((monsterCollisionY(monster2bulletleft1y, monster2bulletleft1y-1, rock3y, rock3y-6)) == 1) && (wallCollision(monster2bulletleft1x, rock3x+6) == 1)){
					ST7735_DrawFastHLine(monster2bulletleft1x,monster2bulletleft1y,7,0x7BEF);
					monster2bulletleft1x = (10);
				}
				if (((monsterCollisionY(monster2bulletleft2y, monster2bulletleft2y-1, rock3y, rock3y-6)) == 1) && (wallCollision(monster2bulletleft2x, rock3x+6) == 1)){
					ST7735_DrawFastHLine(monster2bulletleft2x,monster2bulletleft2y,7,0x7BEF);
					monster2bulletleft2x = (10);
				}
				if (((monsterCollisionY(monster2bulletleft1y, monster2bulletleft1y-1, rock4y, rock4y-6)) == 1) && (wallCollision(monster2bulletleft1x, rock4x+6) == 1)){
					ST7735_DrawFastHLine(monster2bulletleft1x,monster2bulletleft1y,7,0x7BEF);
					monster2bulletleft1x = (10);
				}
				if (((monsterCollisionY(monster2bulletleft2y, monster2bulletleft2y-1, rock4y, rock4y-6)) == 1) && (wallCollision(monster2bulletleft2x, rock4x+6) == 1)){
					ST7735_DrawFastHLine(monster2bulletleft2x,monster2bulletleft2y,7,0x7BEF);
					monster2bulletleft2x = (10);
				}
				if (((monsterCollisionY(monster2bulletleft1y, monster2bulletleft1y-1, mariney, mariney-12)) == 1) && (wallCollision(monster2bulletleft1x, marinex+12) == 1)){
					ST7735_DrawFastHLine(monster2bulletleft1x,monster2bulletleft1y,7,0x7BEF);
					monster2bulletleft1x = (10);
					marinehealth -= 10;
				}
				if (((monsterCollisionY(monster2bulletleft2y, monster2bulletleft2y-1, mariney, mariney-12)) == 1) && (wallCollision(monster2bulletleft2x, marinex+12) == 1)){
					ST7735_DrawFastHLine(monster2bulletleft2x,monster2bulletleft2y,7,0x7BEF);
					monster2bulletleft2x = (10);
					marinehealth -= 10;
				}
			}			
}

void drawmonster2bulletright (int x, int y){
		int monster2bulletright1x = x + 20;
		int monster2bulletright1y = y - 1;
		int monster2bulletright2x = x + 20;
		int monster2bulletright2y = y - 16;
		ST7735_DrawBitmap(monster2bulletright1x, monster2bulletright1y, BulletMonsterRight, 6, 1);
		ST7735_DrawBitmap(monster2bulletright2x, monster2bulletright2y, BulletMonsterRight, 6, 1);
		while(monster2bulletright1x < 138 || monster2bulletright2x < 138)
			{
				monster2bulletright1x ++;
				monster2bulletright2x ++;
				ST7735_DrawBitmap(monster2bulletright1x, monster2bulletright1y, BulletMonsterRight, 6, 1);
				ST7735_DrawBitmap(monster2bulletright2x, monster2bulletright2y, BulletMonsterRight, 6, 1);
				Delay100ms(5);
				if ((wallCollision(monster2bulletright1x, 110)) == 1) {
					ST7735_DrawFastHLine(monster2bulletright1x,monster2bulletright1y,6,0x7BEF);
					monster2bulletright1x = (138);
				}	
				if ((wallCollision(monster2bulletright2x, 110)) == 1) {
					ST7735_DrawFastHLine(monster2bulletright2x,monster2bulletright2y,6,0x7BEF);
					monster2bulletright2x = (138);
				}	
				if (((monsterCollisionY(monster2bulletright1y, monster2bulletright1y-1, rock1y, rock1y-6)) == 1) && ((wallCollision(monster2bulletright1x+1, rock1x)) == 1) ){
					ST7735_DrawFastHLine(monster2bulletright1x,monster2bulletright1y,6,0x7BEF);
					monster2bulletright1x = (136);
				}
				if (((monsterCollisionY(monster2bulletright2y, monster2bulletright2y-1, rock1y, rock1y-6)) == 1) && ((wallCollision(monster2bulletright2x+1, rock1x)) == 1) ){
					ST7735_DrawFastHLine(monster2bulletright2x,monster2bulletright2y,6,0x7BEF);
					monster2bulletright2x = (136);
				}
				if (((monsterCollisionY(monster2bulletright1y, monster2bulletright1y-1, rock2y, rock2y-6)) == 1) && ((wallCollision(monster2bulletright1x+1, rock2x)) == 1) ){
					ST7735_DrawFastHLine(monster2bulletright1x,monster2bulletright1y,6,0x7BEF);
					monster2bulletright1x = (136);
				}
				if (((monsterCollisionY(monster2bulletright2y, monster2bulletright2y-1, rock2y, rock2y-6)) == 1) && ((wallCollision(monster2bulletright2x+1, rock2x)) == 1) ){
					ST7735_DrawFastHLine(monster2bulletright2x,monster2bulletright2y,6,0x7BEF);
					monster2bulletright2x = (136);
				}
				if (((monsterCollisionY(monster2bulletright1y, monster2bulletright1y-1, rock3y, rock3y-6)) == 1) && ((wallCollision(monster2bulletright1x+1, rock3x)) == 1) ){
					ST7735_DrawFastHLine(monster2bulletright1x,monster2bulletright1y,6,0x7BEF);
					monster2bulletright1x = (136);
				}
				if (((monsterCollisionY(monster2bulletright2y, monster2bulletright2y-1, rock3y, rock3y-6)) == 1) && ((wallCollision(monster2bulletright2x+1, rock3x)) == 1) ){
					ST7735_DrawFastHLine(monster2bulletright2x,monster2bulletright2y,6,0x7BEF);
					monster2bulletright2x = (136);
				}
				if (((monsterCollisionY(monster2bulletright1y, monster2bulletright1y-1, rock4y, rock4y-6)) == 1) && ((wallCollision(monster2bulletright1x+1, rock4x)) == 1) ){
					ST7735_DrawFastHLine(monster2bulletright1x,monster2bulletright1y,6,0x7BEF);
					monster2bulletright1x = (136);
				}
				if (((monsterCollisionY(monster2bulletright2y, monster2bulletright2y-1, rock4y, rock4y-6)) == 1) && ((wallCollision(monster2bulletright2x+1, rock4x)) == 1) ){
					ST7735_DrawFastHLine(monster2bulletright2x,monster2bulletright2y,6,0x7BEF);
					monster2bulletright2x = (136);
				}
				if (((monsterCollisionY(monster2bulletright1y, monster2bulletright1y-1, mariney, mariney-12)) == 1) && ((wallCollision(monster2bulletright1x+1, marinex)) == 1) ){
					ST7735_DrawFastHLine(monster2bulletright1x,monster2bulletright1y,6,0x7BEF);
					monster2bulletright1x = (136);
					marinehealth -= 10;
				}
				if (((monsterCollisionY(monster2bulletright2y, monster2bulletright2y-1, mariney, mariney-12)) == 1) && ((wallCollision(monster2bulletright2x+1, marinex)) == 1) ){
					ST7735_DrawFastHLine(monster2bulletright2x,monster2bulletright2y,6,0x7BEF);
					monster2bulletright2x = (136);
					marinehealth -= 10;
				}
			}			
}

void drawmonster3bulletdown (int x, int y){
		int monster3bulletdown1x = x + 3;
		int monster3bulletdown1y = y + 6;
		int monster3bulletdown2x = x + 20;
		int monster3bulletdown2y = y + 6;
		ST7735_DrawBitmap(monster3bulletdown1x, monster3bulletdown1y, BulletMonsterDown, 1, 6);
		ST7735_DrawBitmap(monster3bulletdown2x, monster3bulletdown2y, BulletMonsterDown, 1, 6);
		while(monster3bulletdown1y < 165 || monster3bulletdown2y < 165)
			{
				monster3bulletdown1y ++;
				monster3bulletdown2y ++;
				ST7735_DrawBitmap(monster3bulletdown1x, monster3bulletdown1y, BulletMonsterDown, 1, 6);
				ST7735_DrawBitmap(monster3bulletdown2x, monster3bulletdown2y, BulletMonsterDown, 1, 6);
				Delay100ms(5);
				if ((wallCollision(monster3bulletdown1y, 143)) == 1) {
					monster3bulletdown1y-=6;
					ST7735_DrawFastVLine(monster3bulletdown1x,monster3bulletdown1y,7,0x7BEF);
					monster3bulletdown1y = (166);
				}
				if ((wallCollision(monster3bulletdown2y, 143)) == 1) {
					monster3bulletdown2y-=6;
					ST7735_DrawFastVLine(monster3bulletdown2x,monster3bulletdown2y,7,0x7BEF);
					monster3bulletdown2y = (166);
				}
				if (((monsterCollisionX(monster3bulletdown1x, monster3bulletdown1x+1, rock1x, rock1x+6)) == 1) && (wallCollision(monster3bulletdown1y, rock1y-6) == 1)){
					monster3bulletdown1y-=6;
					ST7735_DrawFastVLine(monster3bulletdown1x,monster3bulletdown1y,7,0x7BEF);
					monster3bulletdown1y = (166);
				}
				if (((monsterCollisionX(monster3bulletdown2x, monster3bulletdown2x+1, rock1x, rock1x+6)) == 1) && (wallCollision(monster3bulletdown2y, rock1y-6) == 1)){
					monster3bulletdown2y-=6;
					ST7735_DrawFastVLine(monster3bulletdown2x,monster3bulletdown2y,7,0x7BEF);
					monster3bulletdown2y = (166);
				}
				if (((monsterCollisionX(monster3bulletdown1x, monster3bulletdown1x+1, rock2x, rock2x+6)) == 1) && (wallCollision(monster3bulletdown1y, rock2y-6) == 1)){
					monster3bulletdown1y-=6;
					ST7735_DrawFastVLine(monster3bulletdown1x,monster3bulletdown1y,7,0x7BEF);
					monster3bulletdown1y = (166);
				}
				if (((monsterCollisionX(monster3bulletdown2x, monster3bulletdown2x+1, rock2x, rock2x+6)) == 1) && (wallCollision(monster3bulletdown2y, rock2y-6) == 1)){
					monster3bulletdown2y-=6;
					ST7735_DrawFastVLine(monster3bulletdown2x,monster3bulletdown2y,7,0x7BEF);
					monster3bulletdown2y = (166);
				}
				if (((monsterCollisionX(monster3bulletdown1x, monster3bulletdown1x+1, rock3x, rock3x+6)) == 1) && (wallCollision(monster3bulletdown1y, rock3y-6) == 1)){
					monster3bulletdown1y-=6;
					ST7735_DrawFastVLine(monster3bulletdown1x,monster3bulletdown1y,7,0x7BEF);
					monster3bulletdown1y = (166);
				}
				if (((monsterCollisionX(monster3bulletdown2x, monster3bulletdown2x+1, rock3x, rock3x+6)) == 1) && (wallCollision(monster3bulletdown2y, rock3y-6) == 1)){
					monster3bulletdown2y-=6;
					ST7735_DrawFastVLine(monster3bulletdown2x,monster3bulletdown2y,7,0x7BEF);
					monster3bulletdown2y = (166);
				}
				if (((monsterCollisionX(monster3bulletdown1x, monster3bulletdown1x+1, rock4x, rock4x+6)) == 1) && (wallCollision(monster3bulletdown1y, rock4y-6) == 1)){
					monster3bulletdown1y-=6;
					ST7735_DrawFastVLine(monster3bulletdown1x,monster3bulletdown1y,7,0x7BEF);
					monster3bulletdown1y = (166);
				}
				if (((monsterCollisionX(monster3bulletdown2x, monster3bulletdown2x+1, rock4x, rock4x+6)) == 1) && (wallCollision(monster3bulletdown2y, rock4y-6) == 1)){
					monster3bulletdown2y-=6;
					ST7735_DrawFastVLine(monster3bulletdown2x,monster3bulletdown2y,7,0x7BEF);
					monster3bulletdown2y = (166);
				}
				if (((monsterCollisionX(monster3bulletdown1x, monster3bulletdown1x+1, marinex, marinex+12)) == 1) && (wallCollision(monster3bulletdown1y, mariney-12) == 1)){
					monster3bulletdown1y-=6;
					ST7735_DrawFastVLine(monster3bulletdown1x,monster3bulletdown1y,7,0x7BEF);
					monster3bulletdown1y = (166);
					marinehealth -= 10;
				}
				if (((monsterCollisionX(monster3bulletdown2x, monster3bulletdown2x+1, marinex, marinex+12)) == 1) && (wallCollision(monster3bulletdown2y, mariney-12) == 1)){
					monster3bulletdown2y-=6;
					ST7735_DrawFastVLine(monster3bulletdown2x,monster3bulletdown2y,7,0x7BEF);
					monster3bulletdown2y = (166);
					marinehealth -= 10;
				}
			}			
}

void drawmonster3bulletup (int x, int y){
		int monster3bulletup1x = x + 3;
		int monster3bulletup1y = y - 28;
		int monster3bulletup2x = x + 20;
		int monster3bulletup2y = y - 28;
		ST7735_DrawBitmap(monster3bulletup1x, monster3bulletup1y, BulletMonsterUp, 1, 6);
		ST7735_DrawBitmap(monster3bulletup2x, monster3bulletup2y, BulletMonsterUp, 1, 6);
		while(monster3bulletup1y > 10 || monster3bulletup2y > 10)
			{
				monster3bulletup1y --;
				monster3bulletup2y --;
				ST7735_DrawBitmap(monster3bulletup1x, monster3bulletup1y, BulletMonsterUp, 1, 6);
				ST7735_DrawBitmap(monster3bulletup2x, monster3bulletup2y, BulletMonsterUp, 1, 6);
				Delay100ms(5);
				if ((wallCollision(monster3bulletup1y-6, 11)) == 1) {
					monster3bulletup1y -= 6;
					ST7735_DrawFastVLine(monster3bulletup1x,monster3bulletup1y,6,0x7BEF);
					monster3bulletup1y = (0);
				}
				if ((wallCollision(monster3bulletup2y-6, 11)) == 1) {
					monster3bulletup2y -= 6;
					ST7735_DrawFastVLine(monster3bulletup2x,monster3bulletup2y,6,0x7BEF);
					monster3bulletup2y = (0);
				}
				if (((monsterCollisionX(monster3bulletup1x, monster3bulletup1x+1, rock1x, rock1x+6)) == 1) && (wallCollision(monster3bulletup1y-6, rock1y) == 1)){
					monster3bulletup1y -= 6;
					ST7735_DrawFastVLine(monster3bulletup1x,monster3bulletup1y,6,0x7BEF);
					monster3bulletup1y = (0);
				}
				if (((monsterCollisionX(monster3bulletup2x, monster3bulletup2x+1, rock1x, rock1x+6)) == 1) && (wallCollision(monster3bulletup2y-6, rock1y) == 1)){
					monster3bulletup2y -= 6;
					ST7735_DrawFastVLine(monster3bulletup2x,monster3bulletup2y,6,0x7BEF);
					monster3bulletup2y = (0);
				}
				if (((monsterCollisionX(monster3bulletup1x, monster3bulletup1x+1, rock2x, rock2x+6)) == 1) && (wallCollision(monster3bulletup1y-6, rock2y) == 1)){
					monster3bulletup1y -= 6;
					ST7735_DrawFastVLine(monster3bulletup1x,monster3bulletup1y,6,0x7BEF);
					monster3bulletup1y = (0);
				}
				if (((monsterCollisionX(monster3bulletup2x, monster3bulletup2x+1, rock2x, rock2x+6)) == 1) && (wallCollision(monster3bulletup2y-6, rock2y) == 1)){
					monster3bulletup2y -= 6;
					ST7735_DrawFastVLine(monster3bulletup2x,monster3bulletup2y,6,0x7BEF);
					monster3bulletup2y = (0);
				}
				if (((monsterCollisionX(monster3bulletup1x, monster3bulletup1x+1, rock3x, rock3x+6)) == 1) && (wallCollision(monster3bulletup1y-6, rock3y) == 1)){
					monster3bulletup1y -= 6;
					ST7735_DrawFastVLine(monster3bulletup1x,monster3bulletup1y,6,0x7BEF);
					monster3bulletup1y = (0);
				}
				if (((monsterCollisionX(monster3bulletup2x, monster3bulletup2x+1, rock3x, rock3x+6)) == 1) && (wallCollision(monster3bulletup2y-6, rock3y) == 1)){
					monster3bulletup2y -= 6;
					ST7735_DrawFastVLine(monster3bulletup2x,monster3bulletup2y,6,0x7BEF);
					monster3bulletup2y = (0);
				}
				if (((monsterCollisionX(monster3bulletup1x, monster3bulletup1x+1, rock4x, rock4x+6)) == 1) && (wallCollision(monster3bulletup1y-6, rock4y) == 1)){
					monster3bulletup1y -= 6;
					ST7735_DrawFastVLine(monster3bulletup1x,monster3bulletup1y,6,0x7BEF);
					monster3bulletup1y = (0);
				}
				if (((monsterCollisionX(monster3bulletup2x, monster3bulletup2x+1, rock4x, rock4x+6)) == 1) && (wallCollision(monster3bulletup2y-6, rock4y) == 1)){
					monster3bulletup2y -= 6;
					ST7735_DrawFastVLine(monster3bulletup2x,monster3bulletup2y,6,0x7BEF);
					monster3bulletup2y = (0);
				}
				if (((monsterCollisionX(monster3bulletup1x, monster3bulletup1x+1, marinex, marinex+12)) == 1) && (wallCollision(monster3bulletup1y-6, mariney) == 1)){
					monster3bulletup1y -=6;
					ST7735_DrawFastVLine(monster3bulletup1x,monster3bulletup1y,6,0x7BEF);
					monster3bulletup1y = 0;
					marinehealth -= 10;
				}
				if (((monsterCollisionX(monster3bulletup2x, monster3bulletup2x+1, marinex, marinex+12)) == 1) && (wallCollision(monster3bulletup2y-6, mariney) == 1)){
					monster3bulletup2y -=6;
					ST7735_DrawFastVLine(monster3bulletup2x,monster3bulletup2y,6,0x7BEF);
					monster3bulletup2y = 0;
					marinehealth -= 10;
				}
			}			
}

void drawmonster3bulletleft (int x, int y){
		int monster3bulletleft1x = x - 8;
		int monster3bulletleft1y = y - 3;
		int monster3bulletleft2x = x - 8;
		int monster3bulletleft2y = y - 19;
		ST7735_DrawBitmap(monster3bulletleft1x, monster3bulletleft1y, BulletMonsterLeft, 6, 1);
		ST7735_DrawBitmap(monster3bulletleft2x, monster3bulletleft2y, BulletMonsterLeft, 6, 1);
		while(monster3bulletleft1x > 10 || monster3bulletleft2x > 10)
			{
				monster3bulletleft1x --;
				monster3bulletleft2x --;
				ST7735_DrawBitmap(monster3bulletleft1x, monster3bulletleft1y, BulletMonsterLeft, 6, 1);
				ST7735_DrawBitmap(monster3bulletleft2x, monster3bulletleft2y, BulletMonsterLeft, 6, 1);
				Delay100ms(5);
				if ((wallCollision(monster3bulletleft1x, 11)) == 1) {
					ST7735_DrawFastHLine(monster3bulletleft1x,monster3bulletleft1y,7,0x7BEF);
					monster3bulletleft1x = (10);
				}
				if ((wallCollision(monster3bulletleft2x, 11)) == 1) {
					ST7735_DrawFastHLine(monster3bulletleft2x,monster3bulletleft2y,7,0x7BEF);
					monster3bulletleft2x = (10);
				}
				if (((monsterCollisionY(monster3bulletleft1y, monster3bulletleft1y-1, rock1y, rock1y-6)) == 1) && (wallCollision(monster3bulletleft1x, rock1x+6) == 1)){
					ST7735_DrawFastHLine(monster3bulletleft1x,monster3bulletleft1y,7,0x7BEF);
					monster3bulletleft1x = (10);
				}
				if (((monsterCollisionY(monster3bulletleft2y, monster3bulletleft2y-1, rock1y, rock1y-6)) == 1) && (wallCollision(monster3bulletleft2x, rock1x+6) == 1)){
					ST7735_DrawFastHLine(monster3bulletleft2x,monster3bulletleft2y,7,0x7BEF);
					monster3bulletleft2x = (10);
				}
				if (((monsterCollisionY(monster3bulletleft1y, monster3bulletleft1y-1, rock2y, rock2y-6)) == 1) && (wallCollision(monster3bulletleft1x, rock2x+6) == 1)){
					ST7735_DrawFastHLine(monster3bulletleft1x,monster3bulletleft1y,7,0x7BEF);
					monster3bulletleft1x = (10);
				}
				if (((monsterCollisionY(monster3bulletleft2y, monster3bulletleft2y-1, rock2y, rock2y-6)) == 1) && (wallCollision(monster3bulletleft2x, rock2x+6) == 1)){
					ST7735_DrawFastHLine(monster3bulletleft2x,monster3bulletleft2y,7,0x7BEF);
					monster3bulletleft2x = (10);
				}
				if (((monsterCollisionY(monster3bulletleft1y, monster3bulletleft1y-1, rock3y, rock3y-6)) == 1) && (wallCollision(monster3bulletleft1x, rock3x+6) == 1)){
					ST7735_DrawFastHLine(monster3bulletleft1x,monster3bulletleft1y,7,0x7BEF);
					monster3bulletleft1x = (10);
				}
				if (((monsterCollisionY(monster3bulletleft2y, monster3bulletleft2y-1, rock3y, rock3y-6)) == 1) && (wallCollision(monster3bulletleft2x, rock3x+6) == 1)){
					ST7735_DrawFastHLine(monster3bulletleft2x,monster3bulletleft2y,7,0x7BEF);
					monster3bulletleft2x = (10);
				}
				if (((monsterCollisionY(monster3bulletleft1y, monster3bulletleft1y-1, rock4y, rock4y-6)) == 1) && (wallCollision(monster3bulletleft1x, rock4x+6) == 1)){
					ST7735_DrawFastHLine(monster3bulletleft1x,monster3bulletleft1y,7,0x7BEF);
					monster3bulletleft1x = (10);
				}
				if (((monsterCollisionY(monster3bulletleft2y, monster3bulletleft2y-1, rock4y, rock4y-6)) == 1) && (wallCollision(monster3bulletleft2x, rock4x+6) == 1)){
					ST7735_DrawFastHLine(monster3bulletleft2x,monster3bulletleft2y,7,0x7BEF);
					monster3bulletleft2x = (10);
				}
				if (((monsterCollisionY(monster3bulletleft1y, monster3bulletleft1y-1, mariney, mariney-12)) == 1) && (wallCollision(monster3bulletleft1x, marinex+12) == 1)){
					ST7735_DrawFastHLine(monster3bulletleft1x,monster3bulletleft1y,7,0x7BEF);
					monster3bulletleft1x = (10);
					marinehealth -= 10;
				}
				if (((monsterCollisionY(monster3bulletleft2y, monster3bulletleft2y-1, mariney, mariney-12)) == 1) && (wallCollision(monster3bulletleft2x, marinex+12) == 1)){
					ST7735_DrawFastHLine(monster3bulletleft2x,monster3bulletleft2y,7,0x7BEF);
					monster3bulletleft2x = (10);
					marinehealth -= 10;
				}
			}			
}

void drawmonster3bulletright (int x, int y){
		int monster3bulletright1x = x + 25;
		int monster3bulletright1y = y - 3;
		int monster3bulletright2x = x + 25;
		int monster3bulletright2y = y - 19;
		ST7735_DrawBitmap(monster3bulletright1x, monster3bulletright1y, BulletMonsterRight, 6, 1);
		ST7735_DrawBitmap(monster3bulletright2x, monster3bulletright2y, BulletMonsterRight, 6, 1);
		while(monster3bulletright1x < 138 || monster3bulletright2x < 138)
			{
				monster3bulletright1x ++;
				monster3bulletright2x ++;
				ST7735_DrawBitmap(monster3bulletright1x, monster3bulletright1y, BulletMonsterRight, 6, 1);
				ST7735_DrawBitmap(monster3bulletright2x, monster3bulletright2y, BulletMonsterRight, 6, 1);
				Delay100ms(5);
				if ((wallCollision(monster3bulletright1x, 110)) == 1) {
					ST7735_DrawFastHLine(monster3bulletright1x,monster3bulletright1y,6,0x7BEF);
					monster3bulletright1x = (138);
				}	
				if ((wallCollision(monster3bulletright2x, 110)) == 1) {
					ST7735_DrawFastHLine(monster3bulletright2x,monster3bulletright2y,6,0x7BEF);
					monster3bulletright2x = (138);
				}	
				if (((monsterCollisionY(monster3bulletright1y, monster3bulletright1y-1, rock1y, rock1y-6)) == 1) && ((wallCollision(monster3bulletright1x+1, rock1x)) == 1) ){
					ST7735_DrawFastHLine(monster3bulletright1x,monster3bulletright1y,6,0x7BEF);
					monster3bulletright1x = (136);
				}
				if (((monsterCollisionY(monster3bulletright2y, monster3bulletright2y-1, rock1y, rock1y-6)) == 1) && ((wallCollision(monster3bulletright2x+1, rock1x)) == 1) ){
					ST7735_DrawFastHLine(monster3bulletright2x,monster3bulletright2y,6,0x7BEF);
					monster3bulletright2x = (136);
				}
				if (((monsterCollisionY(monster3bulletright1y, monster3bulletright1y-1, rock2y, rock2y-6)) == 1) && ((wallCollision(monster3bulletright1x+1, rock2x)) == 1) ){
					ST7735_DrawFastHLine(monster3bulletright1x,monster3bulletright1y,6,0x7BEF);
					monster3bulletright1x = (136);
				}
				if (((monsterCollisionY(monster3bulletright2y, monster3bulletright2y-1, rock2y, rock2y-6)) == 1) && ((wallCollision(monster3bulletright2x+1, rock2x)) == 1) ){
					ST7735_DrawFastHLine(monster3bulletright2x,monster3bulletright2y,6,0x7BEF);
					monster3bulletright2x = (136);
				}
				if (((monsterCollisionY(monster3bulletright1y, monster3bulletright1y-1, rock3y, rock3y-6)) == 1) && ((wallCollision(monster3bulletright1x+1, rock3x)) == 1) ){
					ST7735_DrawFastHLine(monster3bulletright1x,monster3bulletright1y,6,0x7BEF);
					monster3bulletright1x = (136);
				}
				if (((monsterCollisionY(monster3bulletright2y, monster3bulletright2y-1, rock3y, rock3y-6)) == 1) && ((wallCollision(monster3bulletright2x+1, rock3x)) == 1) ){
					ST7735_DrawFastHLine(monster3bulletright2x,monster3bulletright2y,6,0x7BEF);
					monster3bulletright2x = (136);
				}
				if (((monsterCollisionY(monster3bulletright1y, monster3bulletright1y-1, rock4y, rock4y-6)) == 1) && ((wallCollision(monster3bulletright1x+1, rock4x)) == 1) ){
					ST7735_DrawFastHLine(monster3bulletright1x,monster3bulletright1y,6,0x7BEF);
					monster3bulletright1x = (136);
				}
				if (((monsterCollisionY(monster3bulletright2y, monster3bulletright2y-1, rock4y, rock4y-6)) == 1) && ((wallCollision(monster3bulletright2x+1, rock4x)) == 1) ){
					ST7735_DrawFastHLine(monster3bulletright2x,monster3bulletright2y,6,0x7BEF);
					monster3bulletright2x = (136);
				}
				if (((monsterCollisionY(monster3bulletright1y, monster3bulletright1y-1, mariney, mariney-12)) == 1) && ((wallCollision(monster3bulletright1x+1, marinex)) == 1) ){
					ST7735_DrawFastHLine(monster3bulletright1x,monster3bulletright1y,6,0x7BEF);
					monster3bulletright1x = (136);
					marinehealth -= 10;
				}
				if (((monsterCollisionY(monster3bulletright2y, monster3bulletright2y-1, mariney, mariney-12)) == 1) && ((wallCollision(monster3bulletright2x+1, marinex)) == 1) ){
					ST7735_DrawFastHLine(monster3bulletright2x,monster3bulletright2y,6,0x7BEF);
					monster3bulletright2x = (136);
					marinehealth -= 10;
				}
			}			
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------


void drawmonster1up(int x, int y){
	
	ST7735_DrawBitmap(x , y, monster1up , 15,15);
}
void drawmonster1down(int x, int y){
	
	ST7735_DrawBitmap(x , y, monster1down , 15,15);
	
	
}
void drawmonster1left(int x, int y){
	
	ST7735_DrawBitmap(x , y, monster1left , 15,15);
	
	
}
void drawmonster1right(int x, int y){
	
	ST7735_DrawBitmap(x , y, monster1right , 15,15);
	
	
}

void drawmonster2up(int x, int y){
	
	ST7735_DrawBitmap(x , y, monster2up , 18,18);
}
void drawmonster2down(int x, int y){
	
	ST7735_DrawBitmap(x , y, monster2down , 18,18);
	
	
}
void drawmonster2left(int x, int y){
	
	ST7735_DrawBitmap(x , y, monster2left , 18,18);
	
	
}
void drawmonster2right(int x, int y){
	
	ST7735_DrawBitmap(x , y, monster2right , 18,18);
	
	
}

void drawmonster3up(int x, int y){
	
	ST7735_DrawBitmap(x , y, monster3up , 24,25);
}


void drawmonster3down(int x, int y){
	
	ST7735_DrawBitmap(x , y, monster3down , 24,25);
	
	
}
void drawmonster3left(int x, int y){
	
	ST7735_DrawBitmap(x , y, monster3left , 25,24);
	
	
}
void drawmonster3right(int x, int y){
	
	ST7735_DrawBitmap(x , y, monster3right , 25,24);
	
	
}
void drawmarineup(int x, int y){
		
		ST7735_DrawBitmap(x , y, marineup, 12,12);
		
		}

void drawmarinedown(int x, int y){
		
		ST7735_DrawBitmap(x , y, marinedown, 12,12);
		
		}
		
void drawmarineright(int x, int y){
		
		ST7735_DrawBitmap(x , y, marineright, 12,12);
		
		}

void drawmarineleft(int x, int y){
		
		ST7735_DrawBitmap(x , y, marineleft, 12,12);
		
		}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

		
	
void RockUpDown (void) {
	
					
					if (rocknum == 1) {
						ST7735_DrawBitmap( rock1x , rock1y , RockUp , 6,6);
						rock1y++;
						if (gap1 == 0)
							rocknum++;
						gap1 --;
						Delay100ms(10);
					}
					if (rocknum == 2) {
						ST7735_DrawBitmap( rock1x , rock1y , RockUp , 6,6);
						ST7735_DrawBitmap( rock2x , rock2y , RockUp , 6,6);
						rock1y++;
						rock2y++;
						if (gap2 == 0)
							rocknum++;
						gap2 --;
						Delay100ms(10);
					}
					if (rocknum == 3) {
						ST7735_DrawBitmap( rock1x , rock1y , RockUp , 6,6);
						ST7735_DrawBitmap( rock2x , rock2y , RockUp , 6,6);
						ST7735_DrawBitmap( rock3x , rock3y , RockUp , 6,6);
						rock1y++;
						rock2y++;
						rock3y++;
						if (gap3 == 0)
							rocknum++;
						gap3 --;
						Delay100ms(10);
					}
					if (rocknum == 4) {
						ST7735_DrawBitmap( rock1x , rock1y , RockUp , 6,6);
						ST7735_DrawBitmap( rock2x , rock2y , RockUp , 6,6);
						ST7735_DrawBitmap( rock3x , rock3y , RockUp , 6,6);
						ST7735_DrawBitmap( rock4x , rock4y , RockUp , 6,6);
						rock1y++;
						rock2y++;
						rock3y++;
						rock4y++;	
						Delay100ms(10);
					}
					
				
			}


void RockDownUp (void){
	
		int rocknum = 1;
		int rock1y = 167;
		int rock2y = 167;
		int rock3y = 167;
		int rock4y = 167;
	
		int rock1x = ((Random())%52) + 12;
		int rock2x = ((Random())%52) + 12;
		int rock3x = ((Random())%52) + 66;
		int rock4x = ((Random())%52) + 66;
		
		int gap1 = ((Random())%50) + 10;
		int gap2 = ((Random())%50) + 10;
		int gap3 = ((Random())%50) + 10;

					
					if (rocknum == 1) {
						ST7735_DrawBitmap( rock1x , rock1y , RockUp , 6,6);
						rock1y--;
						if (gap1 == 0)
							rocknum++;
						gap1 --;
						Delay100ms(5);
					}
					if (rocknum == 2) {
						ST7735_DrawBitmap( rock1x , rock1y , RockUp , 6,6);
						ST7735_DrawBitmap( rock2x , rock2y , RockUp , 6,6);
						rock1y--;
						rock2y--;
						if (gap2 == 0)
							rocknum++;
						gap2 --;
						Delay100ms(5);
					}
					if (rocknum == 3) {
						ST7735_DrawBitmap( rock1x , rock1y , RockUp , 6,6);
						ST7735_DrawBitmap( rock2x , rock2y , RockUp , 6,6);
						ST7735_DrawBitmap( rock3x , rock3y , RockUp , 6,6);
						rock1y--;
						rock2y--;
						rock3y--;
						if (gap3 == 0)
							rocknum++;
						gap3 --;
						Delay100ms(5);
					}
					if (rocknum == 4) {
						ST7735_DrawBitmap( rock1x , rock1y , RockUp , 6,6);
						ST7735_DrawBitmap( rock2x , rock2y , RockUp , 6,6);
						ST7735_DrawBitmap( rock3x , rock3y , RockUp , 6,6);
						ST7735_DrawBitmap( rock4x , rock4y , RockUp , 6,6);
						rock1y--;
						rock2y--;
						rock3y--;
						rock4y--;	
						Delay100ms(5);
					}
				}
			

void RockLeftRight(void){
		
					
					if (rocknum == 1) {
						ST7735_DrawBitmap( rock1x , rock1y , RockLeft , 7,5);
						rock1x++;
						if(gap1 == 0)
							rocknum ++;
						gap1 --;
						Delay100ms(5);
					}
					if (rocknum == 2) {
						ST7735_DrawBitmap( rock1x , rock1y , RockLeft , 7,5);
						ST7735_DrawBitmap( rock2x , rock2y , RockLeft , 7,5);
						rock1x++;
						rock2x++;
						if(gap2 == 0)
							rocknum++;
						gap2 --;
						Delay100ms(5);
					}
					if (rocknum == 3) {
						ST7735_DrawBitmap( rock1x , rock1y , RockLeft , 7,5);
						ST7735_DrawBitmap( rock2x , rock2y , RockLeft , 7,5);
						ST7735_DrawBitmap( rock3x , rock3y , RockLeft , 7,5);
						rock1x++;
						rock2x++;
						rock3x++;
						if(gap3 == 0)
							rocknum++;
						gap3 --;
						Delay100ms(5);
					}
					if (rocknum == 4) {
						ST7735_DrawBitmap( rock1x , rock1y , RockLeft , 7,5);
						ST7735_DrawBitmap( rock2x , rock2y , RockLeft , 7,5);
						ST7735_DrawBitmap( rock3x , rock3y , RockLeft , 7,5);
						ST7735_DrawBitmap( rock4x , rock4y , RockLeft , 7,5);
						rock1x++;
						rock2x++;
						rock3x++;
						rock4x++;	
						Delay100ms(5);
					}
					
					
	
}

void RockRightLeft(void){
			
					
					if (rocknum == 1) {
						ST7735_DrawBitmap( rock1x , rock1y , RockRight , 7,5);
						rock1x--;
						if(gap1 == 0)
							rocknum++;
						gap1 --;
						Delay100ms(5);
					}
					if (rocknum == 2) {
						ST7735_DrawBitmap( rock1x , rock1y , RockRight , 7,5);
						ST7735_DrawBitmap( rock2x , rock2y , RockRight , 7,5);
						rock1x--;
						rock2x--;
						if(gap2 == 0)
							rocknum++;
						gap2 --;
						Delay100ms(5);
					}
					if (rocknum == 3) {
						ST7735_DrawBitmap( rock1x , rock1y , RockRight , 7,5);
						ST7735_DrawBitmap( rock2x , rock2y , RockRight , 7,5);
						ST7735_DrawBitmap( rock3x , rock3y , RockRight , 7,5);
						rock1x--;
						rock2x--;
						rock3x--;
						if(gap3 == 0)
							rocknum++;
						gap3 --;
						Delay100ms(5);
					}
					if (rocknum == 4) {
						ST7735_DrawBitmap( rock1x , rock1y , RockRight , 7,5);
						ST7735_DrawBitmap( rock2x , rock2y , RockRight , 7,5);
						ST7735_DrawBitmap( rock3x , rock3y , RockRight , 7,5);
						ST7735_DrawBitmap( rock4x , rock4y , RockRight , 7,5);
						rock1x--;
						rock2x--;
						rock3x--;
						rock4x--;						
						Delay100ms(5);
					}
}

	
		
		
		
		
		
		
		
		
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
																					//VERTICAL MAP
		
void vMap(void){
	
	for (int i = 0; i <= 10 ; i++){
	ST7735_DrawFastVLine(i, 0, 160, 0x0011);	
	}
		
		
		
		
		
	
	for (int i = 127; i >= 117 ; i--){
	ST7735_DrawFastVLine(i, 0, 160, 0x0011);
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
																					//HORIZONTAL MAP

void hMap (void) {
	
	for (int i = 0; i <= 10 ; i++){
	ST7735_DrawFastHLine(0, i, 128, 0x0011);	
	}
			
		
	
	for (int i = 160 ; i >= 150 ; i--){
	ST7735_DrawFastHLine(0, i, 128, 0x0011);
	}
}
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
																					//UPPER LEFT CORNER MAP

void ULTransitionIn(void){
	
	for (int i = 0; i <= 150 ; i++){
	ST7735_DrawFastHLine(117, i, 11, 0x7BEF);	
	

		if ( i >= 140 ){
			ST7735_DrawFastHLine(0, (i - 140), 128, 0x0011);
		}
		
	
		while ((UP == 0) || (mariney > 70 )) {
	

			if (DOWN != 0 ) {
				marineDir = 2;
				drawmarinedown(marinex, mariney);
				mariney++;
				if ((wallCollision(mariney, 159)) == 1) {
					mariney--;
				}
				marineDir = 2;
				drawmarinedown(marinex, mariney);
				Delay100ms(5);
			
			}
			if (LEFT != 0) {
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				marinex--;
				if ((wallCollision(marinex, 11)) == 1) {
					marinex++;
				}
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				Delay100ms(5);
			
			}
			if (RIGHT != 0) {
				marineDir = 4;
				drawmarineright(marinex, mariney);
				marinex++;
				if ((wallCollision(marinex, 106)) == 1) {
					marinex--;
				}
				marineDir = 4;
				drawmarineright(marinex, mariney);
				Delay100ms(5);
				
			}
			
			if (UP != 0) {
				marineDir = 1;
				drawmarineup(marinex, mariney);
				mariney--;
				marineDir = 1;
				drawmarineup(marinex, mariney);
				Delay100ms(5);
				
			}
			
			
			
		}
		marineDir = 1;
		drawmarineup(marinex,mariney);
		Delay100ms(10);

	}
	
}
void ULCornerMap(void){									
	
	
	for (int i = 0; i <= 10 ; i++){																												
	ST7735_DrawFastVLine(i, 0, 160, 0x0011);	
	}
	
	for (int i = 0; i <= 10 ; i++){
	ST7735_DrawFastHLine(0, i, 128, 0x0011);	
	}
			
		
	for (int i = 160 ; i >= 150 ; i--){
	ST7735_DrawFastHLine(117, i, 128, 0x0011);
	}
	
}

void ULTransitionOut(void){
	int a = 11;
	for (int i = 117; i >= 0 ; i--){
	

		while ((RIGHT == 0) || (marinex < 60)){
	

			if (DOWN != 0 ) {
				marineDir = 2;
				drawmarinedown(marinex, mariney);
				mariney++;
				if ((wallCollision(mariney, 150)) == 1) {
					mariney--;
				}
				marineDir = 2;
				drawmarinedown(marinex, mariney);
				Delay100ms(5);
			
			}
			if (LEFT != 0) {
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				marinex--;
				if (marinex <= a ) {
	
					marinex++;
					}
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				Delay100ms(5);
			
			}
			if (UP != 0) {
				marineDir = 1;
				drawmarineup(marinex, mariney);
				mariney--;
				if ((wallCollision(mariney-11 ,11 )) == 1) {
					mariney++;
				}
				marineDir = 1;
				drawmarineup(marinex, mariney);
				Delay100ms(5);
			
			}
			if (RIGHT != 0) {
				marineDir = 4;
				drawmarineright(marinex, mariney);
				marinex++;
				marineDir = 4;
				drawmarineright(marinex, mariney);
				Delay100ms(5);
			}
		
		}
		ST7735_DrawFastVLine(i, 150, 11, 0x0011);	
	
		if( i >= 107){
			ST7735_DrawFastVLine((i-107), 11 , 160, 0x7BEF);
			a--;
		}
	marineDir = 4;
	drawmarineright(marinex, mariney);
	Delay100ms(10);
		
	}
	
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
																					//LOWER RIGHT CORNER MAP

void LRTransitionIn(void){
	
	int a = 117;
	
	for (int i = 127; i > 10 ; i--){
	ST7735_DrawFastVLine(i, 0, 11, 0x7BEF);	
	

	if ( i <= 20 ){
	ST7735_DrawFastVLine(108+i, 0, 160, 0x0011);
	a--;
	}
	while ((RIGHT == 0) || (marinex < 60)){
	

			if (DOWN != 0 ) {
				marineDir = 2;
				drawmarinedown(marinex, mariney);
				mariney++;
				if ((wallCollision(mariney, 150)) == 1) {
					mariney--;
				}
				marineDir = 2;
				drawmarinedown(marinex, mariney);
				Delay100ms(5);
			
			}
			if (LEFT != 0) {
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				marinex--;
				if (marinex <= 0 ) {
	
					marinex++;
					}
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				Delay100ms(5);
			
			}
			if (UP != 0) {
				marineDir = 1;
				drawmarineup(marinex, mariney);
				mariney--;
				if ((wallCollision(mariney-11 ,11 )) == 1) {
					mariney++;
				}
				marineDir = 1;
				drawmarineup(marinex, mariney);
				Delay100ms(5);
			
			}
			
			if (RIGHT != 0) {
				marineDir = 4;
				drawmarineright(marinex, mariney);
				marinex++;
				marineDir = 4;
				drawmarineright(marinex, mariney);
				Delay100ms(5);
			}
		
		}
	marineDir = 4;
	drawmarineright(marinex, mariney);
	Delay100ms(10);
	}
	
	
}
	
void LRCornerMap(void){																														
	
	for (int i = 160 ; i >= 150 ; i--){
	ST7735_DrawFastHLine(0, i, 128, 0x0011);
	}
	
	for (int i = 127; i >= 117 ; i--){
	ST7735_DrawFastVLine(i, 0, 160, 0x0011);
	}
	
	for (int i = 0; i <= 10 ; i++){
	ST7735_DrawFastHLine(0, i, 10, 0x0011);	
	}
}

void LRTransitionOut(void){
	
	for (int i = 10 ; i <= 160 ; i++) {
		
	ST7735_DrawFastHLine(0, i, 11, 0x0011);
	

		if ( i <= 20){
			ST7735_DrawFastHLine(0, (i+140), 117, 0x7BEF);
		}
		
		while ((UP == 0) || (mariney > 58 )) {
	

			if (DOWN != 0 ) {
				marineDir = 2;
				drawmarinedown(marinex, mariney);
				mariney++;
				if ((wallCollision(mariney, 159)) == 1) {
					mariney--;
				}
				marineDir = 2;
				drawmarinedown(marinex, mariney);
				Delay100ms(5);
			
			}
			if (LEFT != 0) {
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				marinex--;
				if ((wallCollision(marinex, 11)) == 1) {
					marinex++;
				}
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				Delay100ms(5);
			
			}
			if (RIGHT != 0) {
				marineDir = 4;
				drawmarineright(marinex, mariney);
				marinex++;
				if (marinex >= 104) {
					marinex--;
				}
				marineDir = 4;
				drawmarineright(marinex, mariney);
				Delay100ms(5);
			
			}
			
			if (UP != 0) {
				marineDir = 1;
				drawmarineup(marinex, mariney);
				mariney--;
				drawmarineup(marinex, mariney);
				Delay100ms(5);
				marineDir =1;
				
			}
			
			
			
		}
	marineDir = 1;	
	drawmarineup(marinex,mariney);
	Delay100ms(10);
	}
	
	
}



//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
																					//UPPER RIGHT CORNER MAP

void URTransitionIn(void){									
	
	for (int i = 0; i <= 150 ; i++){
	ST7735_DrawFastHLine(0, i, 11, 0x7BEF);	
		
	

		
		if (i >= 140 ) {
			
			ST7735_DrawFastHLine(0, (i - 140), 128, 0x0011);
			
		}
		
		while ((UP == 0) || (mariney > 70 )) {
	

			if (DOWN != 0 ) {
				marineDir = 2;
				drawmarinedown(marinex, mariney);
				mariney++;
				if ((wallCollision(mariney, 159)) == 1) {
					mariney--;
				}
				marineDir = 2;
				drawmarinedown(marinex, mariney);
				Delay100ms(5);
			
			}
			if (LEFT != 0) {
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				marinex--;
				if ((wallCollision(marinex, 11)) == 1) {
					marinex++;
				}
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				Delay100ms(5);
			
			}
			if (RIGHT != 0) {
				marineDir = 4;
				drawmarineright(marinex, mariney);
				marinex++;
				if ((wallCollision(marinex, 106)) == 1) {
					marinex--;
				}
				marineDir = 4;
				drawmarineright(marinex, mariney);
				Delay100ms(5);
			
			}
			
			if (UP != 0) {
				marineDir = 1;
				drawmarineup(marinex, mariney);
				mariney--;
				marineDir = 1;
				drawmarineup(marinex, mariney);
				Delay100ms(5);
				
			}
			
			
			
		}
		marineDir = 1;
		drawmarineup(marinex, mariney);
		Delay100ms(10);

}
}

void URCornerMap(void){									
	
	for (int i = 0; i <= 10 ; i++){
	ST7735_DrawFastHLine(0, i, 128, 0x0011);	
	}
	
	
	for (int i = 127; i >= 117 ; i--){
	ST7735_DrawFastVLine(i, 0, 160, 0x0011);
	}
	
	for (int i = 160 ; i >= 150 ; i--){
	ST7735_DrawFastHLine(0, i, 10, 0x0011);
	}
	
	
}

void URTransitionOut(void){									
	int a = 106;
	for (int i = 10 ; i <= 160 ; i++) {
	

		
		while ((LEFT == 0) || (marinex > 60)){
	

		
			if (DOWN != 0 ) {
				marineDir = 2;
				drawmarinedown(marinex, mariney);
				mariney++;
				if ((wallCollision(mariney, 150)) == 1) {
					mariney--;
				}
				marineDir = 2;
				drawmarinedown(marinex, mariney);
				Delay100ms(5);
			
			}
			
			if (RIGHT != 0) {
				marineDir = 4;
				drawmarineright(marinex, mariney);
				marinex++;
				if (marinex >= a ) {
					marinex--;
					}
				marineDir = 4;
				drawmarineright(marinex, mariney);
				Delay100ms(5);
			
			}
			if (UP != 0) {
				marineDir = 1;
				drawmarineup(marinex, mariney);
				mariney--;
				if ((wallCollision(mariney-11 ,11 )) == 1) {
					mariney++;
				}
				marineDir = 1;
				drawmarineup(marinex, mariney);
				Delay100ms(5);
			
			}
			if (LEFT != 0) {
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				marinex--;
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				Delay100ms(5);
			
			}
		
		}
		marineDir = 3;
		drawmarineleft(marinex, mariney);
	ST7735_DrawFastVLine(i, 150, 11, 0x0011);
	
		if ( i <= 20 ){
			ST7735_DrawFastVLine(107+i , 11 , 150, 0x7BEF);
			a++;
		}
	marineDir = 3;
	drawmarineleft(marinex,mariney);
	Delay100ms(10);
		
	}
	
	
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
																					//LOWER LEFT CORNER MAP

void LLTransitionIn(void){									
	
	for (int i = 0; i <= 117 ; i++){
	ST7735_DrawFastVLine(i, 0, 11, 0x7BEF);	
	

		
		if (i >= 107 ) {
			
			ST7735_DrawFastVLine((i-107), 0, 160, 0x0011);
			
		}
		
		while ((LEFT == 0) || (marinex > 60)){
	

			if (DOWN != 0 ) {
				marineDir = 2;
				drawmarinedown(marinex, mariney);
				mariney++;
				if ((wallCollision(mariney, 150)) == 1) {
					mariney--;
				}
				marineDir  = 2;
				drawmarinedown(marinex, mariney);
				Delay100ms(5);
			
			}
			if (LEFT != 0) {
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				marinex--;
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				Delay100ms(5);
			
			}
			if (UP != 0) {
				marineDir = 1;
				drawmarineup(marinex, mariney);
				mariney--;
				if ((wallCollision(mariney-11 ,11 )) == 1) {
					mariney++;
				}
				marineDir = 1;
				drawmarineup(marinex, mariney);
				Delay100ms(5);
			
			}
			
			if (RIGHT != 0) {
				marineDir = 4;
				drawmarineright(marinex, mariney);
				marinex++;
				if (marinex >= 116 ) {
	
					marinex--;
				}
				marineDir = 4;
				drawmarineright(marinex, mariney);
				Delay100ms(5);
			}
		
		}
		marineDir = 3;
		drawmarineleft(marinex,mariney);
	Delay100ms(10);
}
}


void LLCornerMap(void){										
	

	for (int i = 0; i <= 10 ; i++){
	ST7735_DrawFastVLine(i, 0, 160, 0x0011);	
	}
		
	for (int i = 160 ; i >= 150 ; i--){
	ST7735_DrawFastHLine(0, i, 128, 0x0011);
	}
	
	for (int i = 0; i <= 10 ; i++){
	ST7735_DrawFastHLine(117, i, 10, 0x0011);	
	}
	
	
}

void LLTransitionOut(void){
	int a = 148;
	for (int i = 10 ; i <= 160 ; i++) {
	

	
	while ((UP == 0) || (mariney > 58 )) {
	

			if (DOWN != 0 ) {
				marineDir = 2;
				drawmarinedown(marinex, mariney);
				mariney++;
				if ((wallCollision(mariney, a)) == 1) {
					mariney--;
				}
				marineDir = 2;
				drawmarinedown(marinex, mariney);
				Delay100ms(5);
			
			}
			if (LEFT != 0) {
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				marinex--;
				if ((wallCollision(marinex, 11)) == 1) {
					marinex++;
				}
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				Delay100ms(5);
			
			}
			if (RIGHT != 0) {
				marineDir = 4;
				drawmarineright(marinex, mariney);
				marinex++;
				if ((wallCollision(marinex, 106)) == 1) {
					marinex--;
				}
				marineDir = 4;
				drawmarineright(marinex, mariney);
				Delay100ms(5);
			
			}
			
			if (UP != 0) {
				marineDir = 1;
				drawmarineup(marinex, mariney);
				mariney--;
				marineDir =1;
				drawmarineup(marinex, mariney);
				Delay100ms(5);
				
			}
			
			
			
		}
	
	
	
	
	
		
	ST7735_DrawFastHLine(117, i, 11, 0x0011);
	
		if ( i <= 20){
			
			ST7735_DrawFastHLine(11, (i+140), 117, 0x7BEF);
			a++;
		
		}
	marineDir = 1;
	drawmarineup(marinex, mariney);
	Delay100ms(10);
	}
	
	
}


//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
int monster1ShiftX (int x, int y){
	
		if (x > marinex+14){
			drawmonster1left(x, y);
			monster1Dir = 3;
			x --;
			if (((monsterCollisionY(y, y-15, rock1y, rock1y-6)) == 1) && (wallCollision(x, rock1x+6) == 1)){
				x++;
			}
			if (((monsterCollisionY(y, y-15, rock2y, rock2y-6)) == 1) && (wallCollision(x, rock2x+6) == 1)){
				x++;
			}
			if (((monsterCollisionY(y, y-15, rock3y, rock3y-6)) == 1) && (wallCollision(x, rock3x+6) == 1)){
				x++;
			}
			if (((monsterCollisionY(y, y-15, rock4y, rock4y-6)) == 1) && (wallCollision(x, rock4x+6) == 1)){
				x++;
			}
			drawmonster1left(x, y);
			monster1Dir = 3;
		}
		if (x+14 < marinex){
			
			drawmonster1right(x, y);
			x++;
			monster1Dir = 4;
			
			if (((monsterCollisionY(y, y-15, rock1y, rock1y-6)) == 1) && ((wallCollision(x+15, rock1x)) == 1) ){
				x--;
			}
			if (((monsterCollisionY(y, y-15, rock2y, rock2y-6)) == 1) && ((wallCollision(x+15, rock2x)) == 1) ){
				x--;
			}
			if (((monsterCollisionY(y, y-15, rock3y, rock3y-6)) == 1) && ((wallCollision(x+15, rock3x)) == 1) ){
				x--;
			}
			if (((monsterCollisionY(y, y-15, rock4y, rock4y-6)) == 1) && ((wallCollision(x+15, rock4x)) == 1) ){
				x--;
			}
			drawmonster1right(x, y);
			monster1Dir = 4;
		}
		
		
		if ( (x == mariney+14) || (y+14 == mariney)){
		if ((monsterCollisionY(mariney, mariney - 15, x , x-15) == 1) && marinehealth > 0 ) {
			marinehealth--;
			
			
		}
			
	}
		
		return x;
	}
 
int monster1ShiftY (int x, int y){
	
		if (y-14 > mariney ){
			drawmonster1up(x, y);
			monster1Dir = 1;
			y--;
			if (((monsterCollisionX(x, x+15, rock1x, rock1x+6)) == 1) && (wallCollision(y-15, rock1y) == 1)){
				y++;
			}
			if (((monsterCollisionX(x, x+15, rock2x, rock2x+6)) == 1) && (wallCollision(y-15, rock2y) == 1)){
				y++;
			}
			if (((monsterCollisionX(x, x+15, rock3x, rock3x+6)) == 1) && (wallCollision(y-15, rock3y) == 1)){
				y++;
			}
			if (((monsterCollisionX(x, x+15, rock4x, rock4x+6)) == 1) && (wallCollision(y-15, rock4y) == 1)){
				y++;
			}
			drawmonster1up(x, y);
			monster1Dir = 1;
		}
		if (y < mariney-14) {
			drawmonster1down(x, y);
			monster1Dir = 2;
			y++;
		
			if (((monsterCollisionX(x, x+15, rock1x, rock1x+6)) == 1) && (wallCollision(y, rock1y-6) == 1)){
				y--;
			}
			if (((monsterCollisionX(x, x+15, rock2x, rock2x+6)) == 1) && (wallCollision(y, rock2y-6) == 1)){
				y--;
			}
			if (((monsterCollisionX(x, x+15, rock3x, rock3x+6)) == 1) && (wallCollision(y, rock3y-6) == 1)){
				y--;
			}
			if (((monsterCollisionX(x, x+15, rock4x, rock4x+6)) == 1) && (wallCollision(y, rock4y-6) == 1)){
				y--;
			}
			drawmonster1down(x, y);
			monster1Dir = 2;
		}
		if ( (y == mariney-14) || (y-14 == mariney)){
		if ((monsterCollisionX(marinex, marinex+ 15, x , x+15) == 1) && marinehealth > 0 ) {
			marinehealth--;
			
		}
			
	}
		
	
	
	
		return y;
}

int monster2ShiftX (int x, int y){
	
		if (x > marinex+14){
			drawmonster2left(x, y);
			monster2Dir = 3;
			x --;
			if (((monsterCollisionY(y, y-18, rock1y, rock1y-6)) == 1) && (wallCollision(x, rock1x+6) == 1)){
				x++;
			}
			if (((monsterCollisionY(y, y-18, rock2y, rock2y-6)) == 1) && (wallCollision(x, rock2x+6) == 1)){
				x++;
			}
			if (((monsterCollisionY(y, y-18, rock3y, rock3y-6)) == 1) && (wallCollision(x, rock3x+6) == 1)){
				x++;
			}
			if (((monsterCollisionY(y, y-18, rock4y, rock4y-6)) == 1) && (wallCollision(x, rock4x+6) == 1)){
				x++;
			}
			drawmonster2left(x, y);
			monster2Dir = 3;
		}
		if (x+17 < marinex){
			
			drawmonster2right(x, y);
			monster2Dir = 4;
			x++;
			if (((monsterCollisionY(y, y-18, rock1y, rock1y-6)) == 1) && ((wallCollision(x+18, rock1x)) == 1) ){
				x--;
			}
			if (((monsterCollisionY(y, y-18, rock2y, rock2y-6)) == 1) && ((wallCollision(x+18, rock2x)) == 1) ){
				x--;
			}
			if (((monsterCollisionY(y, y-18, rock3y, rock3y-6)) == 1) && ((wallCollision(x+18, rock3x)) == 1) ){
				x--;
			}
			if (((monsterCollisionY(y, y-18, rock4y, rock4y-6)) == 1) && ((wallCollision(x+18, rock4x)) == 1) ){
				x--;
			}
			drawmonster2right(x, y);
			monster2Dir = 4;
			
		}
		return x;
	}
 
int monster2ShiftY (int x, int y){
		if (y - 17 > mariney){
			drawmonster2up(x, y);
			monster2Dir = 1;
			y--;
			if (((monsterCollisionX(x, x+18, rock1x, rock1x+6)) == 1) && (wallCollision(y-18, rock1y) == 1)){
				y++;
			}
			if (((monsterCollisionX(x, x+18, rock2x, rock2x+6)) == 1) && (wallCollision(y-18, rock2y) == 1)){
				y++;
			}
			if (((monsterCollisionX(x, x+18, rock3x, rock3x+6)) == 1) && (wallCollision(y-18, rock3y) == 1)){
				y++;
			}
			if (((monsterCollisionX(x, x+18, rock4x, rock4x+6)) == 1) && (wallCollision(y-18, rock4y) == 1)){
				y++;
			}
			drawmonster2up(x, y);
			monster2Dir = 1;
		}
		
			if (y < mariney-14){
			
			drawmonster2down(x, y);
			monster2Dir = 2;
			y++;
			if (((monsterCollisionX(x, x+18, rock1x, rock1x+6)) == 1) && (wallCollision(y, rock1y-6) == 1)){
				y--;
			}
			if (((monsterCollisionX(x, x+18, rock2x, rock2x+6)) == 1) && (wallCollision(y, rock2y-6) == 1)){
				y--;
			}
			if (((monsterCollisionX(x, x+18, rock3x, rock3x+6)) == 1) && (wallCollision(y, rock3y-6) == 1)){
				y--;
			}
			if (((monsterCollisionX(x, x+18, rock4x, rock4x+6)) == 1) && (wallCollision(y, rock4y-6) == 1)){
				y--;
			}
			drawmonster2down(x, y);
			monster2Dir = 2;
			
		}
		return y;
}

int monster3ShiftX (int x, int y){
	
		if (x > marinex+14){
			drawmonster3left(x, y);
			monster3Dir = 3;
			x --;
			if (((monsterCollisionY(y, y-25, rock1y, rock1y-6)) == 1) && (wallCollision(x, rock1x+6) == 1)){
				x++;
			}
			if (((monsterCollisionY(y, y-25, rock2y, rock2y-6)) == 1) && (wallCollision(x, rock2x+6) == 1)){
				x++;
			}
			if (((monsterCollisionY(y, y-25, rock3y, rock3y-6)) == 1) && (wallCollision(x, rock3x+6) == 1)){
				x++;
			}
			if (((monsterCollisionY(y, y-25, rock4y, rock4y-6)) == 1) && (wallCollision(x, rock4x+6) == 1)){
				x++;
			}
			drawmonster3left(x, y);
			monster3Dir = 3;
		}
		if (x+24 < marinex){
			
			drawmonster3right(x, y);
			monster3Dir = 4;
			x++;
			if (((monsterCollisionY(y, y-25, rock1y, rock1y-6)) == 1) && ((wallCollision(x+24, rock1x)) == 1) ){
				x--;
			}
			if (((monsterCollisionY(y, y-25, rock2y, rock2y-6)) == 1) && ((wallCollision(x+24, rock2x)) == 1) ){
				x--;
			}
			if (((monsterCollisionY(y, y-25, rock3y, rock3y-6)) == 1) && ((wallCollision(x+24, rock3x)) == 1) ){
				x--;
			}
			if (((monsterCollisionY(y, y-25, rock4y, rock4y-6)) == 1) && ((wallCollision(x+24, rock4x)) == 1) ){
				x--;
			}
			drawmonster3right(x, y);
			monster3Dir = 4;
			
		}
		return x;
	}
 
int monster3ShiftY (int x, int y){
		if (y-24 > mariney){
			drawmonster3up(x, y);
			monster3Dir = 1;
			y--;
			if (((monsterCollisionX(x, x+24, rock1x, rock1x+6)) == 1) && (wallCollision(y-25, rock1y) == 1)){
				y++;
			}
			if (((monsterCollisionX(x, x+24, rock2x, rock2x+6)) == 1) && (wallCollision(y-25, rock2y) == 1)){
				y++;
			}
			if (((monsterCollisionX(x, x+24, rock3x, rock3x+6)) == 1) && (wallCollision(y-25, rock3y) == 1)){
				y++;
			}
			if (((monsterCollisionX(x, x+24, rock4x, rock4x+6)) == 1) && (wallCollision(y-25, rock4y) == 1)){
				y++;
			}
			drawmonster3up(x, y);
			monster3Dir = 1;
		}
		if (y < mariney-14){
			
			drawmonster3down(x, y);
			monster3Dir = 2;
			y++;
			if (((monsterCollisionX(x, x+24, rock1x, rock1x+6)) == 1) && (wallCollision(y, rock1y-6) == 1)){
				y--;
			}
			if (((monsterCollisionX(x, x+24, rock2x, rock2x+6)) == 1) && (wallCollision(y, rock2y-6) == 1)){
				y--;
			}
			if (((monsterCollisionX(x, x+24, rock3x, rock3x+6)) == 1) && (wallCollision(y, rock3y-6) == 1)){
				y--;
			}
			if (((monsterCollisionX(x, x+24, rock4x, rock4x+6)) == 1) && (wallCollision(y, rock4y-6) == 1)){
				y--;
			}
			drawmonster3down(x, y);
			monster3Dir = 2;
			
		}
		return y;
}
void SysTick_Init(void){
	  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
		NVIC_ST_RELOAD_R = 500000000;  //80M/40
		NVIC_ST_CURRENT_R = 0; // 3) any write to CURRENT clears it 
		NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF) | 0x20000000; 	// Priority 1
		NVIC_ST_CTRL_R = 0x00000007; // 4) enable SysTick with core clock with interrupt set
}

void SysTick_Handler(void){
	
	if ( marinehealth <= 0 ){
		
		GameOver();
		
	}
	
	if (monster1health <= 0){
		
		if(m1==1){
			
		ST7735_DrawBitmap(monster1x , monster1y, monster1death , 15,15);
		Delay100ms(30);
		ST7735_FillRect(monster1x, monster1y-15, 16, 16,  0x7BEF);
		m1 = 0;
		
		
		}
		if(m1==2){
		
		ST7735_DrawBitmap(monster1x , monster1y, monster2death , 18,18);
		Delay100ms(30);
		ST7735_FillRect(monster1x, monster1y-18, 19, 19,  0x7BEF);
		m1 = 0;
		
		}
		if(m1==3){
		
		ST7735_DrawBitmap(monster1x , monster1y, monster3death , 24,25);
		Delay100ms(30);
		ST7735_FillRect(monster1x, monster1y-25, 25, 26,  0x7BEF);
		m1 = 0;
		
		}
		
		
		
	}
	
		if (monster2health <= 0){
		
		if(m2==1){
			
		ST7735_DrawBitmap(monster2x , monster2y, monster1death , 15,15);
		Delay100ms(30);
		ST7735_FillRect(monster2x, monster2y-15, 16, 16,  0x7BEF);
		m2 = 0;
		
		
		}
		if(m2==2){
		
		ST7735_DrawBitmap(monster2x , monster2y, monster2death , 18,18);
		Delay100ms(30);
		ST7735_FillRect(monster2x, monster2y-18, 19, 19,  0x7BEF);
		m2 = 0;
		
		}
		if(m2==3){
		
		ST7735_DrawBitmap(monster2x , monster2y, monster3death , 24,25);
		Delay100ms(30);
		ST7735_FillRect(monster2x, monster2y-25, 25, 26,  0x7BEF);
		m2 = 0;
		
		}
		
		
		
	}
		
		if (monster3health <= 0){
		
		if(m3==1){
			
		ST7735_DrawBitmap(monster3x , monster3y, monster1death , 15,15);
		Delay100ms(30);
		ST7735_FillRect(monster3x, monster3y-15, 16, 16,  0x7BEF);
		m3 = 0;
		
		
		}
		if(m3==2){
		
		ST7735_DrawBitmap(monster3x , monster3y, monster2death , 18,18);
		Delay100ms(30);
		ST7735_FillRect(monster3x, monster3y-18, 19, 19,  0x7BEF);
		m3 = 0;
		
		}
		if(m3==3){
		
		ST7735_DrawBitmap(monster3x , monster3y, monster3death , 24,25);
		Delay100ms(30);
		ST7735_FillRect(monster3x, monster3y-25, 25, 26,  0x7BEF);
		m3 = 0;
		
		}
		
		
		
	}
	
		if(m1 == 2){
			int i = monster1x;
			monster1x = monster2ShiftX(monster1x, monster1y);
			if (i < monster1x){
				monster1Dir = 4;
			}
			else if ( i > monster1x) {
				monster1Dir = 3;
			}
			i = monster1y;
			monster1y = monster2ShiftY(monster1x, monster1y);
			if (i < monster1y){
				monster1Dir = 2;
			}
			else if (i > monster1y) {
				monster1Dir = 1;
			}

		}	
		if(m1 == 3){
			int i = monster1x;
			monster1x = monster3ShiftX(monster1x, monster1y);
			if (i < monster1x){
				monster1Dir = 4;
			}
			else if (i > monster1x) {
				monster1Dir = 3;
			}
			i = monster1y;
			monster1y = monster3ShiftY(monster1x, monster1y);
			if (i < monster1y){
				monster1Dir = 2;
			}
			else if ( monster1y) {
				monster1Dir = 1;
			}
		}	
		
		if(m2 == 2){
			
			int i = monster2x;
			monster2x = monster2ShiftX(monster2x, monster2y);
			if (i < monster2x){
				monster2Dir = 4;
			}
			else if (i > monster2x) {
				monster2Dir = 3;
			}
			i = monster2y;
			monster2y = monster2ShiftY(monster2x, monster2y);
			if (i < monster2y){
				monster2Dir = 2;
			}
			else if (i > monster2y) {
				monster2Dir = 1;
			}

		}	
		if(m2 == 3){
			int i = monster2x;
			monster2x = monster3ShiftX(monster2x, monster2y);
			if (i < monster2x){
				monster2Dir = 4;
			}
			else if (i > monster2x) {
				monster2Dir = 3;
			}
			i = monster2y;
			monster2y = monster3ShiftY(monster2x, monster2y);
			if (i < monster2y){
				monster2Dir = 2;
			}
			else if ( i > monster2y) {
				monster2Dir = 1;
			}

		}	
		
		if(m3 == 2){
			int i = monster3x;
			monster3x = monster2ShiftX(monster3x, monster3y);
			if (i < monster3x){
				monster3Dir = 4;
			}
			if ( i > monster3x) {
				monster3Dir = 3;
			}
			i = monster3y;
			monster3y = monster2ShiftY(monster3x, monster3y);
			if (i < monster3y){
				monster3Dir = 2;
			}
			if ( i > monster3y) {
				monster3Dir = 1;
			}

		}	
		if(m3 == 3){
			int i = monster3x;
			monster3x = monster3ShiftX(monster3x, monster3y);
			if (i < monster3x){
				monster3Dir = 4;
			}
			else if ( i > monster3x){
				monster3Dir = 3;
			}
			i = monster3y;
			monster3y = monster3ShiftY(monster3x, monster3y);
			if (i < monster3y){
				monster3Dir = 2;
			}
			else if ( i> monster3y){
				monster3Dir = 1;
			}

		}	
		
	
		
		
}

void Shoot(void){


				if (m1 == 2){
			if(monster1Dir == 1)
				drawmonster2bulletup(monster1x,monster1y);
			if(monster1Dir == 2)
				drawmonster2bulletdown(monster1x,monster1y);
			if(monster1Dir == 3)
				drawmonster2bulletleft(monster1x,monster1y);
			if(monster1Dir == 4)
				drawmonster2bulletright(monster1x,monster1y);
		
			
		}
		
		if (m1 == 3){
			if(monster1Dir == 1)
				drawmonster3bulletup(monster1x,monster1y);
			if(monster1Dir == 2)
				drawmonster3bulletdown(monster1x,monster1y);
			if(monster1Dir == 3)
				drawmonster3bulletleft(monster1x,monster1y);
			if(monster1Dir == 4)
				drawmonster3bulletright(monster1x,monster1y);
		
			
			
		}
		if (m2 == 2){
			if(monster2Dir == 1)
				drawmonster2bulletup(monster2x,monster2y);
			if(monster2Dir == 2)
				drawmonster2bulletdown(monster2x,monster2y);
			if(monster2Dir == 3)
				drawmonster2bulletleft(monster2x,monster2y);
			if(monster2Dir == 4)
				drawmonster2bulletright(monster2x,monster2y);
		
			
			
			
		}
		if (m2 == 3){
			
			if(monster2Dir == 1)
				drawmonster3bulletup(monster2x,monster2y);
			if(monster2Dir == 2)
				drawmonster3bulletdown(monster2x,monster2y);
			if(monster2Dir == 3)
				drawmonster3bulletleft(monster2x,monster2y);
			if(monster2Dir == 4)
				drawmonster3bulletright(monster2x,monster2y);
		
			
			
		}
		if (m3 == 2){
			if(monster3Dir == 1)
				drawmonster2bulletup(monster3x,monster3y);
			if(monster3Dir == 2)
				drawmonster2bulletdown(monster3x,monster3y);
			if(monster3Dir == 3)
				drawmonster2bulletleft(monster3x,monster3y);
			if(monster3Dir == 4)
				drawmonster2bulletright(monster3x,monster3y);
		
			
			
		}
		if (m3 == 3){
			if(monster3Dir == 1)
				drawmonster3bulletup(monster3x,monster3y);
			if(monster3Dir == 2)
				drawmonster3bulletdown(monster3x,monster3y);
			if(monster3Dir == 3)
				drawmonster3bulletleft(monster3x,monster3y);
			if(monster3Dir == 4)
				drawmonster3bulletright(monster3x,monster3y);
			
			
		}		

		
		
		
		
		
		
}




void mon1(void){
	if(m1 == 1){
		
		int i = monster1x;
			monster1x = monster1ShiftX(monster1x, monster1y);
			if (i < monster1x){
				monster1Dir = 4;
			}
			else if (i > monster1x) {
				monster1Dir = 3;
			}
			i = monster1y;
			monster1y = monster1ShiftY(monster1x, monster1y);
			if (i < monster1y){
				monster1Dir = 2;
			}
			else if (i > monster1y) {
				monster1Dir = 1;
			}
			
		
		
		}		
	if(m2 == 1){
			int i = monster2x;
			monster2x = monster1ShiftX(monster2x, monster2y);
			if (i < monster2x){
				monster2Dir = 4;
			}
			else if ( i > monster2x) {
				monster2Dir = 3;
			}
			i = monster2y;
			monster2y = monster1ShiftY(monster2x, monster2y);
			if (i < monster2y){
				monster2Dir = 2;
			}
			else if (i > monster2y){
				monster2Dir = 1;
			}

		}		
	
		if(m3 == 1){
			int i = monster3x;
			monster3x = monster1ShiftX(monster3x, monster3y);
			if (i < monster3x){
				monster3Dir = 4;
			}
			else if ( i > monster3x){
				monster3Dir = 3;
			}
			i = monster3y;
			monster3y = monster1ShiftY(monster3x, monster3y);
			if (i < monster3y){
				monster3Dir = 2;
			}
			else if ( i >monster3y){
				monster3Dir = 1;
			}

		}		
		
		

		
	
	
	
	
}





int main(void){
	Random_Init(1);
	int open = 0;
	ADC_Init();
		
	 rock1x = ((Random())%52) + 12;
	 rock2x = ((Random())%52) + 12;
	 rock3x = ((Random())%52) + 66;
	 rock4x = ((Random())%52) + 66;
		
	 gap1 = ((Random())%50) + 10;
	 gap2 = ((Random())%50) + 10;
	 gap3 = ((Random())%50) + 10;
	
	SysTick_Init();
	Timer0_Init(&Shoot, (300000000));
	Timer1_Init(&mon1, (5000000));
	EnableInterrupts();
	
	

	
	volatile unsigned long delay;
  SYSCTL_RCGCGPIO_R |= 0x00000010;   // 1) activate clock for Port E
  delay = SYSCTL_RCGCGPIO_R;         //    allow time for clock to stabilize
  GPIO_PORTE_DIR_R &= ~0xFF;      // 2) make PE2 input
  GPIO_PORTE_AFSEL_R |= 0xFF;     // 3) enable alternate function on PE2
  GPIO_PORTE_DEN_R |= 0xFF;      // 4) disable digital I/O on PE2
  GPIO_PORTE_AMSEL_R &= ~0xFF;     // 5) enable analog function on PE2

  TExaS_Init();  // set system clock to 80 MHz
  Random_Init(1);
  Output_Init();
	
	while ( E <= 0 ){
		ST7735_DrawBitmap(0, 160, background, 128, 160);
		
		
	}
	ST7735_FillScreen(0x7BEF);
	Delay100ms(100);
	ST7735_FillScreen(0x0000);
	Delay100ms(100);
	ST7735_FillScreen(0x7BEF);
	Delay100ms(100);
	ST7735_FillScreen(0x0000);
	Delay100ms(100);
	
	ST7735_FillScreen(0x7BEF);
	drawmarineup(marinex, mariney);
	vMap();
		
	

	while(rock4y <= 165){
	
					if (SHOOT != 0)
		{
			if(marineDir == 1)
				drawmarinebulletup(marinex,mariney);
			if(marineDir == 2)
				drawmarinebulletdown(marinex,mariney);
			if(marineDir == 3)
				drawmarinebulletleft(marinex,mariney);
			if(marineDir == 4)
				drawmarinebulletright(marinex,mariney);
		}
		
			while ((UP == 0) || (mariney > 72 )) {
	
				
					if (SHOOT != 0)
		{
			if(marineDir == 1)
				drawmarinebulletup(marinex,mariney);
			if(marineDir == 2)
				drawmarinebulletdown(marinex,mariney);
			if(marineDir == 3)
				drawmarinebulletleft(marinex,mariney);
			if(marineDir == 4)
				drawmarinebulletright(marinex,mariney);
		}
		
			if (DOWN != 0 ) {
				drawmarinedown(marinex, mariney);
				marineDir = 2;
				mariney++;
				Delay100ms(5);
				if ((wallCollision(mariney, 159)) == 1) {
					mariney--;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock1x, rock1x+6)) == 1) && (wallCollision(mariney, rock1y-6) == 1)){
					mariney--;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock2x, rock2x+6)) == 1) && (wallCollision(mariney, rock2y-6) == 1)){
					mariney--;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock3x, rock3x+6)) == 1) && (wallCollision(mariney, rock3y-6) == 1)){
					mariney--;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock4x, rock4x+6)) == 1) && (wallCollision(mariney, rock4y-6) == 1)){
					mariney--;
				}
				
				
				drawmarinedown(marinex, mariney);
				marineDir = 2;
				Delay100ms(5);
			
			}
			
			if (LEFT != 0) {
				drawmarineleft(marinex, mariney);
				marineDir = 3;
				marinex--;
				Delay100ms(5);
				if ((wallCollision(marinex, 11)) == 1) {
					marinex++;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock1y, rock1y-6)) == 1) && (wallCollision(marinex, rock1x+6) == 1)){
					marinex++;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock2y, rock2y-6)) == 1) && (wallCollision(marinex, rock2x+6) == 1)){
					marinex++;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock3y, rock3y-6)) == 1) && (wallCollision(marinex, rock3x+6) == 1)){
					marinex++;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock4y, rock4y-6)) == 1) && (wallCollision(marinex, rock4x+6) == 1)){
					marinex++;
				}
				drawmarineleft(marinex, mariney);
				marineDir = 3;
				Delay100ms(5);
			
			}
			
			if (RIGHT != 0) {
				drawmarineright(marinex, mariney);
				marineDir = 4;
				marinex++;
				Delay100ms(5);
				if ((wallCollision(marinex, 105)) == 1) {
					marinex--;
				}	
				if (((monsterCollisionY(mariney, mariney-12, rock1y, rock1y-6)) == 1) && ((wallCollision(marinex+12, rock1x)) == 1) ){
					marinex--;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock2y, rock2y-6)) == 1) && ((wallCollision(marinex+12, rock2x)) == 1) ){
					marinex--;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock3y, rock3y-6)) == 1) && ((wallCollision(marinex+12, rock3x)) == 1) ){
					marinex--;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock4y, rock4y-6)) == 1) && ((wallCollision(marinex+12, rock4x)) == 1) ){
					marinex--;
				}
				drawmarineright(marinex, mariney);
				marineDir = 4;
				Delay100ms(5);
			
			}
			
				if (UP != 0 ) {
				drawmarineup(marinex, mariney);
				marineDir = 1;
				mariney--;
				Delay100ms(5);
				if (((monsterCollisionX(marinex, marinex+12, rock1x, rock1x+6)) == 1) && (wallCollision(mariney-12, rock1y) == 1)){
					mariney++;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock2x, rock2x+6)) == 1) && (wallCollision(mariney-12, rock2y) == 1)){
					mariney++;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock3x, rock3x+6)) == 1) && (wallCollision(mariney-12, rock3y) == 1)){
					mariney++;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock4x, rock4x+6)) == 1) && (wallCollision(mariney-12, rock4y) == 1)){
					mariney++;
				}
				
				marineDir = 1;
				drawmarineup(marinex, mariney);
				Delay100ms(5);
			
			}
		
		
		}
			
		
		marineDir = 1;
		drawmarineup(marinex, mariney);	
		if((((monsterCollisionX(marinex, marinex+12, rock1x, rock1x+6)) == 0) || (wallCollision(mariney-12, rock1y) == 0)) &&  (((monsterCollisionX(marinex, marinex+12, rock2x, rock2x+6)) == 0) || (wallCollision(mariney-12, rock2y) == 0)) &&   (((monsterCollisionX(marinex, marinex+12, rock3x, rock3x+6)) == 0) || (wallCollision(mariney-12, rock3y) == 0)) &&  (((monsterCollisionX(marinex, marinex+12, rock4x, rock4x+6)) == 0) || (wallCollision(mariney-12, rock4y) == 0)) ) { 
		RockUpDown();
		mon1();
		Delay100ms(10);
	
			if ( (rocknum == 1) && (gap1 == 0) ){
				m1 = 1;
				monster1health = 2;
				monster1Dir = 2;
				monster1x = rock2x+10;
				monster1y = rock2y;
				drawmonster1down(monster1x , monster1y);
				
			}
			if ( (rocknum == 2) && (gap2 == 0)){
				m2 = 1;
				monster2health = 2;
				monster2Dir = 2;
				monster2x = rock3x+10;
				monster2y = rock3y;
				drawmonster1down(monster2x , monster2y);
				
			}
			if ( (rocknum == 3) && (gap3 == 0)){
				m3 = 1;
				monster3health = 2;
				monster3Dir = 2;
				monster3x = rock4x+10;
				monster3y = rock4y;
				drawmonster1down(monster3x , monster3y);
				
				
			}
		}
		}
	

	
	
	
	ULTransitionIn();
	m1=0;
	m2=0;
	m3=0;
	ST7735_FillRect(monster1x, monster1y-15, 16, 16,  0x7BEF);
	ST7735_FillRect(monster2x, monster2y-15, 16, 16,  0x7BEF);
	ST7735_FillRect(monster3x, monster3y-15, 16, 16,  0x7BEF);
	ULCornerMap();
	int armorx = 0;
	int armory = 0;
	for (int i = 50; i <= 70 ; i++){
		ST7735_DrawFastVLine(i, 0, 11, 0x0000);	
	}
	
	while( open == 0 ){
		
	
				if (DOWN != 0 ) {
				drawmarinedown(marinex, mariney);
				marineDir = 2;
				mariney++;
				Delay100ms(5);
				if ((wallCollision(mariney, 150)) == 1) {
					mariney--;
				}
				drawmarinedown(marinex, mariney);
				marineDir = 2;
				Delay100ms(5);
			
			}
			if (LEFT != 0) {
				drawmarineleft(marinex, mariney);
				marineDir = 3;
				marinex--;
				Delay100ms(5);
				if ((wallCollision(marinex, 11)) == 1) {
					marinex++;
				}
				drawmarineleft(marinex, mariney);
				marineDir = 3;
				Delay100ms(5);
			
			}
			
			if (RIGHT != 0) {
				drawmarineright(marinex, mariney);
				marineDir = 4;
				marinex++;
				Delay100ms(5);
				if ((wallCollision(marinex, 105)) == 1) {
					marinex--;
				}	
				drawmarineright(marinex, mariney);
				marineDir = 4;
				Delay100ms(5);
			
			}
			
				if (UP != 0 ) {
				drawmarineup(marinex, mariney);
				marineDir = 1;
				mariney--;
				Delay100ms(5);
				if ((wallCollision(mariney-15, 11)) == 1) {
					mariney++;
				}
				marineDir = 1;
				drawmarineup(marinex, mariney);
				Delay100ms(5);
			
			}
	
		if( (((monsterCollisionX(marinex, marinex+12, 50, 70)) == 1) && (mariney <= 29)) && (E >= 1) ){
		
		ST7735_DrawBitmap(0, 160, door, 128, 160);
		Delay100ms(1000);
		int masterunlock = 0;
		int unlock1 = 0;
		int unlock2 = 0;
		int unlock3 = 0;
		int unlock4 = 0;
		int data = ADC_In();
		ST7735_FillScreen(0x0000);
		ST7735_DrawBitmap(0, 100, zero, 127, 41);
		while (masterunlock == 0){
			
			data = ADC_In();
			
			while(unlock1 ==0){
			data = ADC_In();
				if ( (data < 2000 ) &&( E > 0) ){
				unlock1 = 1;
				ST7735_DrawBitmap(0, 100, one, 127, 41);
				}
				
			}
			while(unlock2 ==0){
			data = ADC_In();	
				if ( (data > 2000 ) &&( E > 0) ){
				unlock2 = 1;
				ST7735_DrawBitmap(0, 100, two, 127, 41);
				Delay100ms(100);
				}
				
				
			}
			while(unlock3 ==0){
			data = ADC_In();	
				if ( (data > 2000 ) &&( E > 0) ){
				unlock3 = 1;
				ST7735_DrawBitmap(0, 100, three, 127, 41);
				}
				
				
			}
			while(unlock4 ==0){
			data = ADC_In();
				if ( (data < 2000 ) &&( E > 0) ){
				unlock4 = 1;
				ST7735_DrawBitmap(0, 100, four, 127, 41);
				Delay100ms(500);
				masterunlock = 1;
					
				}
				
				
			}
			
			
			
			
			
			
			
		}
			
			
		mariney = 145;
		ST7735_FillScreen(0x7BEF);
		Delay100ms(50);
		ST7735_FillScreen(0X0011);
		Delay100ms(50);
		ST7735_FillScreen(0x7BEF);
		vMap();
		hMap();
	
		for (int i = 50; i <= 70 ; i++){
		ST7735_DrawFastVLine(i, 150, 11, 0x0000);	
		}
		ST7735_DrawBitmap(60, 30, armor, 13, 13);
		drawmarineup(marinex, mariney);		
		armorx = 60;
		armory = 30;
	}
	
	if ((((monsterCollisionX(marinex, marinex+12, armorx, armorx+13)) == 1) && (wallCollision(mariney-15, armory) == 1))){
		
		marinehealth += 20;
		ST7735_FillRect(armorx, armory-13, 13, 14,  0x7BEF);
		
	}
	
	if ((((monsterCollisionX(marinex, marinex+12, 50, 70)) == 1) && (wallCollision(mariney, 149) == 1)) && (E >= 1) ){
		
	
	open = 1;
	
	
}
		

		
		
	}
	
	mariney = 40;
	ST7735_FillScreen(0x7BEF);
	ULCornerMap();
	drawmarinedown(marinex, mariney);
	
	ULTransitionOut();
	
	hMap();
		
		rocknum = 1;
		rock1x = 127;
		rock2x = 127;
		rock3x = 127;
		rock4x = 127;
		rock1y = ((Random())%60) + 20;
		rock2y = ((Random())%60) + 20;
		rock3y = ((Random())%60) + 84;
		rock4y = ((Random())%60) + 84;
		gap1 = ((Random())%50) + 10;
		gap2 = ((Random())%50) + 10;
		gap3 = ((Random())%50) + 10;
		
		
	while(rock4x >= -6){
	
							if (SHOOT != 0)
		{
			if(marineDir == 1)
				drawmarinebulletup(marinex,mariney);
			if(marineDir == 2)
				drawmarinebulletdown(marinex,mariney);
			if(marineDir == 3)
				drawmarinebulletleft(marinex,mariney);
			if(marineDir == 4)
				drawmarinebulletright(marinex,mariney);
		}
		
		while ((RIGHT == 0) || (marinex < 58 )) {
								if (SHOOT != 0)
		{
			if(marineDir == 1)
				drawmarinebulletup(marinex,mariney);
			if(marineDir == 2)
				drawmarinebulletdown(marinex,mariney);
			if(marineDir == 3)
				drawmarinebulletleft(marinex,mariney);
			if(marineDir == 4)
				drawmarinebulletright(marinex,mariney);
		}
			
			
			if (DOWN != 0 ) {
				marineDir = 2;
				drawmarinedown(marinex, mariney);
				mariney++;
				Delay100ms(5);
				if ((wallCollision(mariney, 149)) == 1) {
					mariney--;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock1x, rock1x+6)) == 1) && (wallCollision(mariney, rock1y-6) == 1)){
					mariney--;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock2x, rock2x+6)) == 1) && (wallCollision(mariney, rock2y-6) == 1)){
					mariney--;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock3x, rock3x+6)) == 1) && (wallCollision(mariney, rock3y-6) == 1)){
					mariney--;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock4x, rock4x+6)) == 1) && (wallCollision(mariney, rock4y-6) == 1)){
					mariney--;
				}
				marineDir = 2;
				drawmarinedown(marinex, mariney);
				Delay100ms(5);
			
			}
			
			if (LEFT != 0) {
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				marinex--;
				Delay100ms(5);
				if ((wallCollision(marinex, 0)) == 1) {
					marinex++;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock1y, rock1y-6)) == 1) && (wallCollision(marinex, rock1x+6) == 1)){
					marinex++;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock2y, rock2y-6)) == 1) && (wallCollision(marinex, rock2x+6) == 1)){
					marinex++;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock3y, rock3y-6)) == 1) && (wallCollision(marinex, rock3x+6) == 1)){
					marinex++;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock4y, rock4y-6)) == 1) && (wallCollision(marinex, rock4x+6) == 1)){
					marinex++;
				}
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				Delay100ms(5);
			
			}
			
			if (RIGHT != 0) {
				marineDir = 4;
				drawmarineright(marinex, mariney);
				marinex++;
				Delay100ms(5);
				if (((monsterCollisionY(mariney, mariney-12, rock1y, rock1y-6)) == 1) && ((wallCollision(marinex+12, rock1x)) == 1) ){
					marinex--;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock2y, rock2y-6)) == 1) && ((wallCollision(marinex+12, rock2x)) == 1) ){
					marinex--;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock3y, rock3y-6)) == 1) && ((wallCollision(marinex+12, rock3x)) == 1) ){
					marinex--;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock4y, rock4y-6)) == 1) && ((wallCollision(marinex+12, rock4x)) == 1) ){
					marinex--;
				}
				marineDir = 4;
				drawmarineright(marinex, mariney);
				Delay100ms(5);
			
			}
			
				if (UP != 0 ) {
				marineDir = 1;
				drawmarineup(marinex, mariney);
				mariney--;
					Delay100ms(5);
				if ((wallCollision(mariney-12, 11)) == 1) {
					mariney++;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock1x, rock1x+6)) == 1) && (wallCollision(mariney-12, rock1y) == 1)){
					mariney++;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock2x, rock2x+6)) == 1) && (wallCollision(mariney-12, rock2y) == 1)){
					mariney++;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock3x, rock3x+6)) == 1) && (wallCollision(mariney-12, rock3y) == 1)){
					mariney++;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock4x, rock4x+6)) == 1) && (wallCollision(mariney-12, rock4y) == 1)){
					mariney++;
				}
				
				marineDir = 1;
				drawmarineup(marinex, mariney);
				Delay100ms(5);
			
			}
		
		
		}
			
		
		marineDir = 4;
		drawmarineright(marinex, mariney);	
		if((((monsterCollisionY(mariney, mariney-12, rock1y, rock1y-6)) == 0) || ((wallCollision(marinex+12, rock1x)) == 0) ) &&  (((monsterCollisionY(mariney, mariney-12, rock2y, rock2y-6)) == 0) || ((wallCollision(marinex+12, rock2x)) == 0) ) &&   (((monsterCollisionY(mariney, mariney-12, rock3y, rock3y-6)) == 0) || ((wallCollision(marinex+12, rock3x)) == 0) ) && (((monsterCollisionX(marinex, marinex+12, rock4x, rock4x+6)) == 0) || (wallCollision(mariney-12, rock4y) == 0))  ) { 
		RockRightLeft();
		Delay100ms(10);
		if ( (rocknum == 1) && (gap1 == 0) ){
				m1 = 1;
				monster1health = 2;
				monster1Dir = 3;
				monster1x = rock2x+10;
				monster1y = rock2y;
				drawmonster1down(monster1x , monster1y);
				
			}
			if ( (rocknum == 2) && (gap2 == 0)){
				m2 = 1;
				monster2health = 2;
				monster2Dir = 3;
				monster2x = rock3x+10;
				monster2y = rock3y;
				drawmonster1down(monster2x , monster2y);
				
			}
			if ( (rocknum == 3) && (gap3 == 0)){
				m3 = 2;
				monster3health = 4;
				monster3Dir = 3;
				monster3x = rock4x+10;
				monster3y = rock4y;
				drawmonster2down(monster3x , monster3y);
				
				
			}
	}
		
	}
	LRTransitionIn();
	m1=0;
	m2=0;
	m3=0;
	ST7735_FillRect(monster1x, monster1y-15, 16, 16,  0x7BEF);
	ST7735_FillRect(monster2x, monster2y-15, 16, 16,  0x7BEF);
	ST7735_FillRect(monster3x, monster3y-18, 19, 19,  0x7BEF);
	LRCornerMap();
	LRTransitionOut();
	vMap();
	
		rocknum = 1;
		rock1y = 0;
		rock2y = 0;
		rock3y = 0;
		rock4y = 0;
		rock1x = ((Random())%52) + 12;
		rock2x = ((Random())%52) + 12;
		rock3x = ((Random())%52) + 66;
		rock4x = ((Random())%52) + 66;
		gap1 = ((Random())%50) + 10;
		gap2 = ((Random())%50) + 10;
		gap3 = ((Random())%50) + 10;
	
	
	
	
	
	while(rock4y <= 165){

									if (SHOOT != 0)
		{
			if(marineDir == 1)
				drawmarinebulletup(marinex,mariney);
			if(marineDir == 2)
				drawmarinebulletdown(marinex,mariney);
			if(marineDir == 3)
				drawmarinebulletleft(marinex,mariney);
			if(marineDir == 4)
				drawmarinebulletright(marinex,mariney);
		} 
		
			while ((UP == 0) || (mariney > 72 )) {

										if (SHOOT != 0)
		{
			if(marineDir == 1)
				drawmarinebulletup(marinex,mariney);
			if(marineDir == 2)
				drawmarinebulletdown(marinex,mariney);
			if(marineDir == 3)
				drawmarinebulletleft(marinex,mariney);
			if(marineDir == 4)
				drawmarinebulletright(marinex,mariney);
		}
		
			if (DOWN != 0 ) {
				marineDir = 2;
				drawmarinedown(marinex, mariney);
				mariney++;
				if ((wallCollision(mariney, 159)) == 1) {
					mariney--;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock1x, rock1x+6)) == 1) && (wallCollision(mariney, rock1y-6) == 1)){
					mariney--;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock2x, rock2x+6)) == 1) && (wallCollision(mariney, rock2y-6) == 1)){
					mariney--;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock3x, rock3x+6)) == 1) && (wallCollision(mariney, rock3y-6) == 1)){
					mariney--;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock4x, rock4x+6)) == 1) && (wallCollision(mariney, rock4y-6) == 1)){
					mariney--;
				}
				
				marineDir = 2;
				drawmarinedown(marinex, mariney);
				Delay100ms(5);
			
			}
			
			if (LEFT != 0) {
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				marinex--;
				Delay100ms(5);
				if ((wallCollision(marinex, 11)) == 1) {
					marinex++;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock1y, rock1y-6)) == 1) && (wallCollision(marinex, rock1x+6) == 1)){
					marinex++;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock2y, rock2y-6)) == 1) && (wallCollision(marinex, rock2x+6) == 1)){
					marinex++;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock3y, rock3y-6)) == 1) && (wallCollision(marinex, rock3x+6) == 1)){
					marinex++;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock4y, rock4y-6)) == 1) && (wallCollision(marinex, rock4x+6) == 1)){
					marinex++;
				}
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				Delay100ms(5);
			
			}
			
			if (RIGHT != 0) {
				marineDir = 4;
				drawmarineright(marinex, mariney);
				marinex++;
				Delay100ms(5);
				if ((wallCollision(marinex, 105)) == 1) {
					marinex--;
				}	
				if (((monsterCollisionY(mariney, mariney-12, rock1y, rock1y-6)) == 1) && ((wallCollision(marinex+12, rock1x)) == 1) ){
					marinex--;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock2y, rock2y-6)) == 1) && ((wallCollision(marinex+12, rock2x)) == 1) ){
					marinex--;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock3y, rock3y-6)) == 1) && ((wallCollision(marinex+12, rock3x)) == 1) ){
					marinex--;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock4y, rock4y-6)) == 1) && ((wallCollision(marinex+12, rock4x)) == 1) ){
					marinex--;
				}
				marineDir = 4;
				drawmarineright(marinex, mariney);
				Delay100ms(5);
			
			}
			
				if (UP != 0 ) {
				marineDir = 1;
				drawmarineup(marinex, mariney);
				mariney--;
				if (((monsterCollisionX(marinex, marinex+12, rock1x, rock1x+6)) == 1) && (wallCollision(mariney-12, rock1y) == 1)){
					mariney++;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock2x, rock2x+6)) == 1) && (wallCollision(mariney-12, rock2y) == 1)){
					mariney++;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock3x, rock3x+6)) == 1) && (wallCollision(mariney-12, rock3y) == 1)){
					mariney++;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock4x, rock4x+6)) == 1) && (wallCollision(mariney-12, rock4y) == 1)){
					mariney++;
				}
				
				marineDir = 1;
				drawmarineup(marinex, mariney);
				Delay100ms(5);
			
			}
		
		
		}
			
		
		marineDir = 1;
		drawmarineup(marinex, mariney);	
		if((((monsterCollisionX(marinex, marinex+12, rock1x, rock1x+6)) == 0) || (wallCollision(mariney-12, rock1y) == 0)) &&  (((monsterCollisionX(marinex, marinex+12, rock2x, rock2x+6)) == 0) || (wallCollision(mariney-12, rock2y) == 0)) &&   (((monsterCollisionX(marinex, marinex+12, rock3x, rock3x+6)) == 0) || (wallCollision(mariney-12, rock3y) == 0)) &&  (((monsterCollisionX(marinex, marinex+12, rock4x, rock4x+6)) == 0) || (wallCollision(mariney-12, rock4y) == 0)) ) { 
		RockUpDown();
		Delay100ms(10);
		if ( (rocknum == 1) && (gap1 == 0) ){
				m1 = 1;
				monster1health = 2;
				monster1Dir = 2;
				monster1x = rock2x+10;
				monster1y = rock2y - 8;
				drawmonster1down(monster1x , monster1y);
				
			}
			if ( (rocknum == 2) && (gap2 == 0)){
				m2 = 2;
				monster2health = 4;
				monster2Dir = 2;
				monster2x = rock3x+10;
				monster2y = rock3y;
				drawmonster2down(monster2x , monster2y);
				
			}
			if ( (rocknum == 3) && (gap3 == 0)){
				m3 = 2;
				monster3health = 4;
				monster3Dir = 2;
				monster3x = rock4x+10;
				monster3y = rock4y-10;
				drawmonster2down(monster3x , monster3y);
				
				
			}
		}
		}
	

	
	
	

	URTransitionIn();
	m1=0;
	m2=0;
	m3=0;
	ST7735_FillRect(monster1x, monster1y-15, 16, 16,  0x7BEF);
	ST7735_FillRect(monster2x, monster2y-18, 19, 19,  0x7BEF);
	ST7735_FillRect(monster3x, monster3y-18, 19, 19,  0x7BEF);
	URCornerMap();
	URTransitionOut();
	hMap();
	
		rocknum = 1;
		rock1x = 0;
		rock2x = 0;
		rock3x = 0;
		rock4x = 0;
		rock1y = ((Random())%60) + 20;
		rock2y = ((Random())%60) + 20;
		rock3y = ((Random())%60) + 84;
		rock4y = ((Random())%60) + 84;	
		gap1 = ((Random())%50) + 10;
		gap2 = ((Random())%50) + 10;
		gap3 = ((Random())%50) + 10;
			
		
	while(rock4x <= 134){

											if (SHOOT != 0)
		{
			if(marineDir == 1)
				drawmarinebulletup(marinex,mariney);
			if(marineDir == 2)
				drawmarinebulletdown(marinex,mariney);
			if(marineDir == 3)
				drawmarinebulletleft(marinex,mariney);
			if(marineDir == 4)
				drawmarinebulletright(marinex,mariney);
		} 
		
		while ((LEFT == 0) || (marinex > 58 )) {

		
												if (SHOOT != 0)
		{
			if(marineDir == 1)
				drawmarinebulletup(marinex,mariney);
			if(marineDir == 2)
				drawmarinebulletdown(marinex,mariney);
			if(marineDir == 3)
				drawmarinebulletleft(marinex,mariney);
			if(marineDir == 4)
				drawmarinebulletright(marinex,mariney);
		} 
		
			if (DOWN != 0 ) {
				marineDir = 2;
				drawmarinedown(marinex, mariney);
				mariney++;
				if ((wallCollision(mariney, 149)) == 1) {
					mariney--;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock1x, rock1x+6)) == 1) && (wallCollision(mariney, rock1y-6) == 1)){
					mariney--;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock2x, rock2x+6)) == 1) && (wallCollision(mariney, rock2y-6) == 1)){
					mariney--;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock3x, rock3x+6)) == 1) && (wallCollision(mariney, rock3y-6) == 1)){
					mariney--;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock4x, rock4x+6)) == 1) && (wallCollision(mariney, rock4y-6) == 1)){
					mariney--;
				}
				marineDir = 2;
				drawmarinedown(marinex, mariney);
				Delay100ms(5);
			
			}
			
			if (LEFT != 0) {
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				marinex--;
				Delay100ms(5);
				if (((monsterCollisionY(mariney, mariney-12, rock1y, rock1y-6)) == 1) && (wallCollision(marinex, rock1x+6) == 1)){
					marinex++;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock2y, rock2y-6)) == 1) && (wallCollision(marinex, rock2x+6) == 1)){
					marinex++;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock3y, rock3y-6)) == 1) && (wallCollision(marinex, rock3x+6) == 1)){
					marinex++;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock4y, rock4y-6)) == 1) && (wallCollision(marinex, rock4x+6) == 1)){
					marinex++;
				}
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				Delay100ms(5);
			
			}
			
			if (RIGHT != 0) {
				marineDir = 4;
				drawmarineright(marinex, mariney);
				marinex++;
				if ((wallCollision(marinex, 116)) == 1) {
					marinex--;
				}
				Delay100ms(5);
				if (((monsterCollisionY(mariney, mariney-12, rock1y, rock1y-6)) == 1) && ((wallCollision(marinex+12, rock1x)) == 1) ){
					marinex--;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock2y, rock2y-6)) == 1) && ((wallCollision(marinex+12, rock2x)) == 1) ){
					marinex--;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock3y, rock3y-6)) == 1) && ((wallCollision(marinex+12, rock3x)) == 1) ){
					marinex--;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock4y, rock4y-6)) == 1) && ((wallCollision(marinex+12, rock4x)) == 1) ){
					marinex--;
				}
				marineDir = 4;
				drawmarineright(marinex, mariney);
				Delay100ms(5);
			
			}
			
				if (UP != 0 ) {
				marineDir = 1;
				drawmarineup(marinex, mariney);
				mariney--;
				if ((wallCollision(mariney-12, 11)) == 1) {
					mariney++;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock1x, rock1x+6)) == 1) && (wallCollision(mariney-12, rock1y) == 1)){
					mariney++;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock2x, rock2x+6)) == 1) && (wallCollision(mariney-12, rock2y) == 1)){
					mariney++;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock3x, rock3x+6)) == 1) && (wallCollision(mariney-12, rock3y) == 1)){
					mariney++;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock4x, rock4x+6)) == 1) && (wallCollision(mariney-12, rock4y) == 1)){
					mariney++;
				}
				
				marineDir = 1;
				drawmarineup(marinex, mariney);
				Delay100ms(5);
			
			}
		
		
		}
			
		
		marineDir = 3;
		drawmarineleft(marinex, mariney);	
		if((((monsterCollisionY(mariney, mariney-12, rock1y, rock1y-6)) == 0) || ((wallCollision(marinex, rock1x+6)) == 0) ) &&  (((monsterCollisionY(mariney, mariney-12, rock2y, rock2y-6)) == 0) || ((wallCollision(marinex, rock2x+6)) == 0) ) &&   (((monsterCollisionY(mariney, mariney-12, rock3y, rock3y-6)) == 0) || ((wallCollision(marinex, rock3x+6)) == 0) ) && (((monsterCollisionX(marinex, marinex+12, rock4x, rock4x-6)) == 0) || (wallCollision(mariney, rock4y+6) == 0))  ) { 
		RockLeftRight();
		Delay100ms(10);
				if ( (rocknum == 1) && (gap1 == 0) ){
				m1 = 2;
				monster1health = 4;
				monster1Dir = 4;
				monster1x = rock2x-10;
				monster1y = rock2y+10;
				drawmonster2down(monster1x , monster1y);
				
			}
			if ( (rocknum == 2) && (gap2 == 0)){
				m2 = 2;
				monster2health = 4;
				monster2Dir = 4;
				monster2x = rock3x;
				monster2y = rock3y+10;
				drawmonster2down(monster2x , monster2y);
				
			}
			if ( (rocknum == 3) && (gap3 == 0)){
				m3 = 2;
				monster3health = 4;
				monster3Dir = 4;
				monster3x = rock4x - 10 ;
				monster3y = rock4y-40;
				drawmonster2down(monster3x , monster3y);
				
				
			}
	}
		
	}

	LLTransitionIn();
	m1=0;
	m2=0;
	m3=0;
	ST7735_FillRect(monster1x, monster1y-18, 19, 19,  0x7BEF);
	ST7735_FillRect(monster2x, monster2y-18, 19, 19,  0x7BEF);
	ST7735_FillRect(monster3x, monster3y-18, 19, 19,  0x7BEF);
	LLCornerMap();
	LLTransitionOut();
	vMap();
	
		rocknum = 1;
		rock1y = 0;
		rock2y = 0;
		rock3y = 0;
		rock4y = 0;
		rock1x = ((Random())%52) + 12;
		rock2x = ((Random())%52) + 12;
		rock3x = ((Random())%52) + 66;
		rock4x = ((Random())%52) + 66;
		gap1 = ((Random())%50) + 10;
		gap2 = ((Random())%50) + 10;
		gap3 = ((Random())%50) + 10;
	
	
	
	
	
	while(rock4y <= 165){

											if (SHOOT != 0)
		{
			if(marineDir == 1)
				drawmarinebulletup(marinex,mariney);
			if(marineDir == 2)
				drawmarinebulletdown(marinex,mariney);
			if(marineDir == 3)
				drawmarinebulletleft(marinex,mariney);
			if(marineDir == 4)
				drawmarinebulletright(marinex,mariney);
		} 
		
			while ((UP == 0) || (mariney > 72 )) {

											if (SHOOT != 0)
		{
			if(marineDir == 1)
				drawmarinebulletup(marinex,mariney);
			if(marineDir == 2)
				drawmarinebulletdown(marinex,mariney);
			if(marineDir == 3)
				drawmarinebulletleft(marinex,mariney);
			if(marineDir == 4)
				drawmarinebulletright(marinex,mariney);
		} 
		
			if (DOWN != 0 ) {
				marineDir = 2;
				drawmarinedown(marinex, mariney);
				mariney++;
				if ((wallCollision(mariney, 159)) == 1) {
					mariney--;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock1x, rock1x+6)) == 1) && (wallCollision(mariney, rock1y-6) == 1)){
					mariney--;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock2x, rock2x+6)) == 1) && (wallCollision(mariney, rock2y-6) == 1)){
					mariney--;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock3x, rock3x+6)) == 1) && (wallCollision(mariney, rock3y-6) == 1)){
					mariney--;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock4x, rock4x+6)) == 1) && (wallCollision(mariney, rock4y-6) == 1)){
					mariney--;
				}
				
				marineDir = 2;
				drawmarinedown(marinex, mariney);
				Delay100ms(5);
			
			}
			
			if (LEFT != 0) {
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				marinex--;
				Delay100ms(5);
				if ((wallCollision(marinex, 11)) == 1) {
					marinex++;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock1y, rock1y-6)) == 1) && (wallCollision(marinex, rock1x+6) == 1)){
					marinex++;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock2y, rock2y-6)) == 1) && (wallCollision(marinex, rock2x+6) == 1)){
					marinex++;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock3y, rock3y-6)) == 1) && (wallCollision(marinex, rock3x+6) == 1)){
					marinex++;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock4y, rock4y-6)) == 1) && (wallCollision(marinex, rock4x+6) == 1)){
					marinex++;
				}
				marineDir = 3;
				drawmarineleft(marinex, mariney);
				Delay100ms(5);
			
			}
			
			if (RIGHT != 0) {
				marineDir = 4;
				drawmarineright(marinex, mariney);
				marinex++;
				Delay100ms(5);
				if ((wallCollision(marinex, 105)) == 1) {
					marinex--;
				}	
				if (((monsterCollisionY(mariney, mariney-12, rock1y, rock1y-6)) == 1) && ((wallCollision(marinex+12, rock1x)) == 1) ){
					marinex--;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock2y, rock2y-6)) == 1) && ((wallCollision(marinex+12, rock2x)) == 1) ){
					marinex--;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock3y, rock3y-6)) == 1) && ((wallCollision(marinex+12, rock3x)) == 1) ){
					marinex--;
				}
				if (((monsterCollisionY(mariney, mariney-12, rock4y, rock4y-6)) == 1) && ((wallCollision(marinex+12, rock4x)) == 1) ){
					marinex--;
				}
				marineDir = 4;
				drawmarineright(marinex, mariney);
				Delay100ms(5);
			
			}
			
				if (UP != 0 ) {
				marineDir = 1;
				drawmarineup(marinex, mariney);
				mariney--;
				if (((monsterCollisionX(marinex, marinex+12, rock1x, rock1x+6)) == 1) && (wallCollision(mariney-12, rock1y) == 1)){
					mariney++;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock2x, rock2x+6)) == 1) && (wallCollision(mariney-12, rock2y) == 1)){
					mariney++;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock3x, rock3x+6)) == 1) && (wallCollision(mariney-12, rock3y) == 1)){
					mariney++;
				}
				if (((monsterCollisionX(marinex, marinex+12, rock4x, rock4x+6)) == 1) && (wallCollision(mariney-12, rock4y) == 1)){
					mariney++;
				}
				
				marineDir = 1;
				drawmarineup(marinex, mariney);
				Delay100ms(5);
			
			}
		
		
		}
			
		
		marineDir = 1;
		drawmarineup(marinex, mariney);	
		if((((monsterCollisionX(marinex, marinex+12, rock1x, rock1x+6)) == 0) || (wallCollision(mariney-12, rock1y) == 0)) &&  (((monsterCollisionX(marinex, marinex+12, rock2x, rock2x+6)) == 0) || (wallCollision(mariney-12, rock2y) == 0)) &&   (((monsterCollisionX(marinex, marinex+12, rock3x, rock3x+6)) == 0) || (wallCollision(mariney-12, rock3y) == 0)) &&  (((monsterCollisionX(marinex, marinex+12, rock4x, rock4x+6)) == 0) || (wallCollision(mariney-12, rock4y) == 0)) ) { 
		RockUpDown();
		Delay100ms(10);
				if ( (rocknum == 1) && (gap1 == 0) ){
				m1 = 1;
				monster1health = 2;
				monster1Dir = 2;
				monster1x = rock2x+10;
				monster1y = rock2y -10;
				drawmonster1down(monster1x , monster1y);
				
			}
			if ( (rocknum == 2) && (gap2 == 0)){
				m2 = 3;
				monster2health = 8;
				monster2Dir = 2;
				monster2x = rock3x-40;
				monster2y = rock3y;
				drawmonster3down(monster2x , monster2y);
				
			}
			if ( (rocknum == 3) && (gap3 == 0)){
				m3 = 3;
				monster3health = 8;
				monster3Dir = 2;
				monster3x = rock4x-30;
				monster3y = rock4y-5;
				drawmonster3down(monster3x , monster3y);
				
				
			}
		}
		}
	

	
	
	

	
		
		
		
	Delay100ms(50);              // delay 5 sec at 80 MHz

	DisableInterrupts();
  ST7735_FillScreen(0x0000);            // set screen to black
  
  while(1){
	ST7735_DrawBitmap(0, 110, end, 128, 67);
	Delay100ms(1000);
  }

}





void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 7272;  // 0.1sec at 80 MHz    727240
    while(time){
	  	time--;
    }
    count--;
  }
}
