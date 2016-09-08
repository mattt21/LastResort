#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "TExaS.h"
#include "ADC.h"

//TRUE = 1 			(COLLISION!!!!)
//FALSE = 0 
	

	
int wallCollision (int a,int b){
	
	
	if ( a == b ) {
		
		return 1;
		
	}
	
	else{
		
		return 0;
		
	}
	
}




int monsterCollisionX(int marinex1, int marinex2, int monsterx1, int monsterx2){
	
	
	if (((monsterx2 - marinex1 ) >= 0 ) && ((marinex1 - monsterx1) >= 0 ) ) {
		
		return 1;
		
	}
	
	
	if (((marinex2 - monsterx1) >= 0 ) && ((monsterx1 - marinex1 ) >= 0 )) {
		
		return 1;
		
	}
	
	
	if (((marinex2 - monsterx2) >= 0 ) && ((monsterx2 - marinex1 ) >= 0 )) {
		
		return 1;
		
	}
	
	
	
	
	
	
	
	return 0;
}



int monsterCollisionY(int mariney1, int mariney2, int monstery1, int monstery2){
	
	
	if (((monstery1-mariney2) >= 0) && ((mariney2 - monstery2) >= 0) ) {
		
		return 1;
		
	}
	
	if (((mariney1-monstery2) >= 0) && ((monstery2 - mariney2) >= 0) ) {
		
		return 1;
		
	}
	
	
	if (((mariney1-monstery1) >= 0) && ((monstery1 - mariney2) >= 0) ) {
		
		return 1;
		
	}
	
	
	

		return 0;
	
	
}

















