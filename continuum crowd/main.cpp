/*******************************************************************
* PROJECT: continuum crowd
*
* DESCRIPTION: this is the final project for Module CS5343, based on the paper: 
*				Treuille A, Cooper S, Popovic Z. Continuum crowds[J]. ACM 
*				Transactions on Graphics (TOG), 2006, 25(3): 1160-1168.
*
*
* AUTHOR: Luo Yuanfu
*
* CREATED ON: 20/04/2015
********************************************************************/

#include "display.h"



//ENTRY
void main(int argc, char **argv) 
{
	 glutInit(&argc, argv); 
	 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	  
	 glutInitWindowPosition(100,0);
	 glutInitWindowSize(MAP_WIDTH*RESOLUTION, MAP_HEIGHT*RESOLUTION); 	 
	 glutCreateWindow("continuum crowd"); 
	
	 srand(time(NULL));

	 initDisplay(); 
	 glutDisplayFunc(display);
	 glutIdleFunc(display);
	 glutMainLoop(); 
}
