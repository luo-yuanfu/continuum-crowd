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

Map map;

void initDisplay()
{
	glClearColor(1.0,1.0,1.0,0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0,MAP_WIDTH,0,MAP_HEIGHT);
}

void drawPerson(double x, double y)
{
	
	glPushMatrix();
	glTranslatef(x,y,0);
    glBegin(GL_QUADS);//draw quad to represent a person
    glVertex2f(-PERSON_SIZE, PERSON_SIZE);//up left
    glVertex2f(PERSON_SIZE, PERSON_SIZE);//up right
    glVertex2f(PERSON_SIZE, -PERSON_SIZE);//bottom right
	glVertex2f(-PERSON_SIZE, -PERSON_SIZE);//bottom left
    glEnd();//
	glPopMatrix();
	

}

void display(void )
{
	glClear(GL_COLOR_BUFFER_BIT);

	for(int i=0;i<GROUP_NUM;i++)
	{
		glColor3f(map.groupPtr[i]->color[0],map.groupPtr[i]->color[1],map.groupPtr[i]->color[2]);
		for(int j=0;j<GROUP_PEOPLE;j++)
		{
			if(map.groupPtr[i]->people_list[j].pos[0]<MAP_WIDTH && 
				map.groupPtr[i]->people_list[j].pos[0]>0 &&
				map.groupPtr[i]->people_list[j].pos[1]<MAP_HEIGHT &&
				map.groupPtr[i]->people_list[j].pos[1]>0 )
			drawPerson(map.groupPtr[i]->people_list[j].pos[0], map.groupPtr[i]->people_list[j].pos[1]);
		}
	}

	map.update();

	glFlush();
	glutSwapBuffers();
}