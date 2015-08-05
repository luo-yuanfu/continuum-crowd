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
* CREATED ON: 21/04/2015
********************************************************************/

#include "map.h"

Map::Map()
{
	initCellSpeed();
//	initMap();
	for(int i=0; i< GROUP_NUM; i++)
	{
		groupPtr[i]=new Group(i+1, &cost_map, &cell_speed);
	}
}

Map::~Map()
{
	for(int i=0; i< GROUP_NUM; i++)
	{
		if(groupPtr[i]!=NULL)
		{
			delete groupPtr[i];
			groupPtr[i]=NULL;
		}
	}
}

void Map::initCellSpeed()
{
	for(int i=0;i<MAP_WIDTH;i++)
		for(int j=0;j<MAP_HEIGHT;j++)
			for(int k=0;k<4;k++)
				cell_speed.speed[i][j][k]=INIT_CELL_SPEED;
}

void Map::calcCostMap()
{
	for(int i=0;i<MAP_WIDTH;i++)
		for(int j=0;j<MAP_HEIGHT;j++)
			for(int k=0;k<4;k++)
			{
				//speed is contrait to bigger than 0 in other function
				cost_map.cost[i][j][k]=(PATH_WEIGHT*cell_speed.speed[i][j][k]+TIME_WEIGHT+DISCOMFORT_WEIGHT*DISCOMFORT)/cell_speed.speed[i][j][k];
			}
}

void Map::zeroDensity()
{
	for(int i=0;i<MAP_WIDTH;i++)
		for(int j=0;j<MAP_HEIGHT;j++)
		{
			density[i][j]=0;
		}
}

void Map::resetDensityAndAveVel()
{
	for(int i=0;i<MAP_WIDTH;i++)
	{
		for(int j=0; j<MAP_HEIGHT; j++)
		{
			if(density[i][j]==0) 
			{
				//when the density is 0, flow speed is also zero, i.e. ave_vel=0£¬
				density[i][j]=MIN_DENSITY;
			}
			else if(density[i][j]<MIN_DENSITY) 
			{
				ave_vel[i][j][0]/=density[i][j];
				ave_vel[i][j][1]/=density[i][j];
				density[i][j]=MIN_DENSITY;
			}
			else if(density[i][j]>MAX_DENSITY) 
			{
				ave_vel[i][j][0]/=density[i][j];
				ave_vel[i][j][1]/=density[i][j];
				density[i][j]=MAX_DENSITY;
			}
			else
			{
				ave_vel[i][j][0]/=density[i][j];
				ave_vel[i][j][1]/=density[i][j];
			}
		}
	}
}

void Map::zeroAveVelocity()
{
	for(int i=0;i<MAP_WIDTH;i++)
	{
		for(int j=0; j<MAP_HEIGHT; j++)
		{
			ave_vel[i][j][0]=0;
			ave_vel[i][j][1]=0;
		}
	}
}

void Map::updateDensityAndAveVel()
{
	zeroDensity();
	zeroAveVelocity();

	for(int i=0;i<GROUP_NUM;i++)
	{
		for(int j=0; j<GROUP_PEOPLE; j++)
		{
			int cell_x, cell_y;//cell index of person i's position in the map
			cell_x=int(groupPtr[i]->people_list[j].pos[0]);
			cell_y=int(groupPtr[i]->people_list[j].pos[1]);

			//the person is out of the map
			if(cell_x<0 || cell_y<0 || cell_x>=MAP_WIDTH || cell_y>=MAP_HEIGHT) continue;

			double delta_x=groupPtr[i]->people_list[j].pos[0]-cell_x;
			double delta_y=groupPtr[i]->people_list[j].pos[1]-cell_y;

			if(delta_x<=0.5 && delta_y<=0.5)//person is at the left bottom of the cell
			{
				double delta_x_to_center=0.5-delta_x;
				double delta_y_to_center=0.5-delta_y;

				double new_density=pow(1-delta_x_to_center<1-delta_y_to_center? 1-delta_x_to_center : 1-delta_y_to_center,LAMBDA);
				density[cell_x][cell_y]+=new_density;
				ave_vel[cell_x][cell_y][0]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[0];
				ave_vel[cell_x][cell_y][1]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[1];
				
				if(cell_x-1>=0)
				{
					new_density=pow(delta_x_to_center<1-delta_y_to_center? delta_x_to_center : 1-delta_y_to_center,LAMBDA);
					density[cell_x-1][cell_y]+=new_density;
					ave_vel[cell_x-1][cell_y][0]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[0];
					ave_vel[cell_x-1][cell_y][1]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[1];
				}

				if(cell_y-1>=0)
				{
					new_density=pow(1-delta_x_to_center<delta_y_to_center? 1-delta_x_to_center : delta_y_to_center,LAMBDA);
					density[cell_x][cell_y-1]+=new_density;
					ave_vel[cell_x][cell_y-1][0]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[0];
					ave_vel[cell_x][cell_y-1][1]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[1];
				
				}

				if(cell_y-1>=0 && cell_x-1>=0) 
				{
					new_density=pow(delta_x_to_center<delta_y_to_center? delta_x_to_center : delta_y_to_center,LAMBDA);
					density[cell_x-1][cell_y-1]+=new_density;
					ave_vel[cell_x-1][cell_y-1][0]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[0];
					ave_vel[cell_x-1][cell_y-1][1]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[1];

				}
			}
			else if(delta_x<=0.5 && delta_y>0.5)//person is at the left top of the cell
			{
				double delta_x_to_center=0.5-delta_x;
				double delta_y_to_center=delta_y-0.5;

				/*
				density[cell_x][cell_y]+=pow(1-delta_x_to_center<1-delta_y_to_center? 1-delta_x_to_center : 1-delta_y_to_center,LAMBDA);
				
				if(cell_x-1>=0)density[cell_x-1][cell_y]+=pow(delta_x_to_center<1-delta_y_to_center? delta_x_to_center : 1-delta_y_to_center,LAMBDA);

				if(cell_y+1<MAP_HEIGHT)density[cell_x][cell_y+1]+=pow(1-delta_x_to_center<delta_y_to_center? 1-delta_x_to_center : delta_y_to_center,LAMBDA);

				if(cell_y+1<MAP_HEIGHT && cell_x-1>=0) density[cell_x-1][cell_y+1]+=pow(delta_x_to_center<delta_y_to_center? delta_x_to_center : delta_y_to_center,LAMBDA);
				*/
				double new_density=pow(1-delta_x_to_center<1-delta_y_to_center? 1-delta_x_to_center : 1-delta_y_to_center,LAMBDA);
				density[cell_x][cell_y]+=new_density;
				ave_vel[cell_x][cell_y][0]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[0];
				ave_vel[cell_x][cell_y][1]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[1];
				
				if(cell_x-1>=0)
				{
					new_density=pow(delta_x_to_center<1-delta_y_to_center? delta_x_to_center : 1-delta_y_to_center,LAMBDA);
					density[cell_x-1][cell_y]+=new_density;
					ave_vel[cell_x-1][cell_y][0]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[0];
					ave_vel[cell_x-1][cell_y][1]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[1];
				}

				if(cell_y+1<MAP_HEIGHT)
				{
					new_density=pow(1-delta_x_to_center<delta_y_to_center? 1-delta_x_to_center : delta_y_to_center,LAMBDA);
					density[cell_x][cell_y+1]+=new_density;
					ave_vel[cell_x][cell_y+1][0]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[0];
					ave_vel[cell_x][cell_y+1][1]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[1];
				
				}

				if(cell_y+1<MAP_HEIGHT && cell_x-1>=0) 
				{
					new_density=pow(delta_x_to_center<delta_y_to_center? delta_x_to_center : delta_y_to_center,LAMBDA);
					density[cell_x-1][cell_y+1]+=new_density;
					ave_vel[cell_x-1][cell_y+1][0]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[0];
					ave_vel[cell_x-1][cell_y+1][1]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[1];

				}
			}
			else if(delta_x>0.5 && delta_y>0.5)//person is at the right top of the cell
			{
				double delta_x_to_center=delta_x-0.5;
				double delta_y_to_center=delta_y-0.5;

				/*
				density[cell_x][cell_y]+=pow(1-delta_x_to_center<1-delta_y_to_center? 1-delta_x_to_center : 1-delta_y_to_center,LAMBDA);
				
				if(cell_x+1<MAP_WIDTH)density[cell_x+1][cell_y]+=pow(delta_x_to_center<1-delta_y_to_center? delta_x_to_center : 1-delta_y_to_center,LAMBDA);

				if(cell_y+1<MAP_HEIGHT)density[cell_x][cell_y+1]+=pow(1-delta_x_to_center<delta_y_to_center? 1-delta_x_to_center : delta_y_to_center,LAMBDA);

				if(cell_y+1<MAP_HEIGHT && cell_x+1<MAP_WIDTH) density[cell_x+1][cell_y+1]+=pow(delta_x_to_center<delta_y_to_center? delta_x_to_center : delta_y_to_center,LAMBDA);
				*/
				double new_density=pow(1-delta_x_to_center<1-delta_y_to_center? 1-delta_x_to_center : 1-delta_y_to_center,LAMBDA);
				density[cell_x][cell_y]+=new_density;
				ave_vel[cell_x][cell_y][0]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[0];
				ave_vel[cell_x][cell_y][1]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[1];
				
				if(cell_x+1<MAP_WIDTH)
				{
					new_density=pow(delta_x_to_center<1-delta_y_to_center? delta_x_to_center : 1-delta_y_to_center,LAMBDA);
					density[cell_x+1][cell_y]+=new_density;
					ave_vel[cell_x+1][cell_y][0]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[0];
					ave_vel[cell_x+1][cell_y][1]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[1];
				}

				if(cell_y+1<MAP_HEIGHT)
				{
					new_density=pow(1-delta_x_to_center<delta_y_to_center? 1-delta_x_to_center : delta_y_to_center,LAMBDA);
					density[cell_x][cell_y+1]+=new_density;
					ave_vel[cell_x][cell_y+1][0]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[0];
					ave_vel[cell_x][cell_y+1][1]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[1];
				
				}

				if(cell_y+1<MAP_HEIGHT && cell_x+1<MAP_WIDTH) 
				{
					new_density=pow(delta_x_to_center<delta_y_to_center? delta_x_to_center : delta_y_to_center,LAMBDA);
					density[cell_x+1][cell_y+1]+=new_density;
					ave_vel[cell_x+1][cell_y+1][0]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[0];
					ave_vel[cell_x+1][cell_y+1][1]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[1];

				}
			}
			else if(delta_x>0.5 && delta_y<=0.5)//person is at the right bottom of the cell
			{
				double delta_x_to_center=delta_x-0.5;
				double delta_y_to_center=0.5-delta_y;

				/*
				density[cell_x][cell_y]+=pow(1-delta_x_to_center<1-delta_y_to_center? 1-delta_x_to_center : 1-delta_y_to_center,LAMBDA);
				
				if(cell_x+1<MAP_WIDTH)density[cell_x+1][cell_y]+=pow(delta_x_to_center<1-delta_y_to_center? delta_x_to_center : 1-delta_y_to_center,LAMBDA);

				if(cell_y-1>=0)density[cell_x][cell_y-1]+=pow(1-delta_x_to_center<delta_y_to_center? 1-delta_x_to_center : delta_y_to_center,LAMBDA);

				if(cell_y-1>=0 && cell_x+1<MAP_WIDTH) density[cell_x+1][cell_y-1]+=pow(delta_x_to_center<delta_y_to_center? delta_x_to_center : delta_y_to_center,LAMBDA);
				*/
				double new_density=pow(1-delta_x_to_center<1-delta_y_to_center? 1-delta_x_to_center : 1-delta_y_to_center,LAMBDA);
				density[cell_x][cell_y]+=new_density;
				ave_vel[cell_x][cell_y][0]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[0];
				ave_vel[cell_x][cell_y][1]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[1];
				
				if(cell_x+1<MAP_WIDTH)
				{
					new_density=pow(delta_x_to_center<1-delta_y_to_center? delta_x_to_center : 1-delta_y_to_center,LAMBDA);
					density[cell_x+1][cell_y]+=new_density;
					ave_vel[cell_x+1][cell_y][0]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[0];
					ave_vel[cell_x+1][cell_y][1]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[1];
				}

				if(cell_y-1>=0)
				{
					new_density=pow(1-delta_x_to_center<delta_y_to_center? 1-delta_x_to_center : delta_y_to_center,LAMBDA);
					density[cell_x][cell_y-1]+=new_density;
					ave_vel[cell_x][cell_y-1][0]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[0];
					ave_vel[cell_x][cell_y-1][1]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[1];
				
				}

				if(cell_y-1>=0 && cell_x+1<MAP_WIDTH) 
				{
					new_density=pow(delta_x_to_center<delta_y_to_center? delta_x_to_center : delta_y_to_center,LAMBDA);
					density[cell_x+1][cell_y-1]+=new_density;
					ave_vel[cell_x+1][cell_y-1][0]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[0];
					ave_vel[cell_x+1][cell_y-1][1]+=new_density*groupPtr[i]->people_list[j].speed*groupPtr[i]->people_list[j].direction[1];

				}
			}

		}
		
	}

	//check if they are more than or less than the max and min density, if yes, reset them. at the same time, calculate the average velocity
	resetDensityAndAveVel();
}

void Map::updateCellSpeed()
{
	double fT=MAX_SPEED;//only consider that the terrain is flat
	for(int i=0;i<MAP_WIDTH;i++)
		for(int j=0;j<MAP_HEIGHT;j++)
		{
			for(int k=0;k<4;k++)
			{
				switch(k)
				{
				case EAST:
					{
						if(i+1<MAP_WIDTH)
						{
							cell_speed.speed[i][j][EAST]=fT+(density[i][j]-MIN_DENSITY)/(MAX_DENSITY-MIN_DENSITY)*(ave_vel[i+1][j][0]-fT);
						}
						else cell_speed.speed[i][j][EAST]=fT+(density[i][j]-MIN_DENSITY)/(MAX_DENSITY-MIN_DENSITY)*(ave_vel[i][j][0]-fT);
						if(cell_speed.speed[i][j][EAST]<MIN_FLOW_SPEED) cell_speed.speed[i][j][EAST]=MIN_FLOW_SPEED;
						 break;
					}
				case WEST:
					{
						if(i-1>=0)
						{
							cell_speed.speed[i][j][WEST]=fT+(density[i][j]-MIN_DENSITY)/(MAX_DENSITY-MIN_DENSITY)*(ave_vel[i-1][j][0]-fT);
						}
						else cell_speed.speed[i][j][WEST]=fT+(density[i][j]-MIN_DENSITY)/(MAX_DENSITY-MIN_DENSITY)*(ave_vel[i][j][0]-fT);
						if(cell_speed.speed[i][j][WEST]<MIN_FLOW_SPEED) cell_speed.speed[i][j][WEST]=MIN_FLOW_SPEED;
						 break;
					}
				case NORTH:
					{
						if(j+1<MAP_HEIGHT)
						{
							cell_speed.speed[i][j][NORTH]=fT+(density[i][j]-MIN_DENSITY)/(MAX_DENSITY-MIN_DENSITY)*(ave_vel[i][j+1][1]-fT);
						}
						else cell_speed.speed[i][j][NORTH]=fT+(density[i][j]-MIN_DENSITY)/(MAX_DENSITY-MIN_DENSITY)*(ave_vel[i][j][1]-fT);
						if(cell_speed.speed[i][j][NORTH]<MIN_FLOW_SPEED) cell_speed.speed[i][j][NORTH]=MIN_FLOW_SPEED;
						 break;
					}
				case SOUTH:
					{
						if(j-1>=0)
						{
							cell_speed.speed[i][j][SOUTH]=fT+(density[i][j]-MIN_DENSITY)/(MAX_DENSITY-MIN_DENSITY)*(ave_vel[i][j-1][1]-fT);
						}
						else cell_speed.speed[i][j][SOUTH]=fT+(density[i][j]-MIN_DENSITY)/(MAX_DENSITY-MIN_DENSITY)*(ave_vel[i][j][1]-fT);
						if(cell_speed.speed[i][j][SOUTH]<MIN_FLOW_SPEED) cell_speed.speed[i][j][SOUTH]=MIN_FLOW_SPEED;
						 break;
					}
				}
			}
		}

}

void Map::update()
{
	calcCostMap();
	for(int i=0;i<GROUP_NUM;i++)
	{
		groupPtr[i]->setPotenAndGradient();
		groupPtr[i]->updateVelocity();
		groupPtr[i]->updatePosition();
	}

	updateDensityAndAveVel();

	updateCellSpeed();

}