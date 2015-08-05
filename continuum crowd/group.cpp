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

#include "group.h"

Person::Person()
{
	speed=INIT_SPEED;
	/*
	pos[0]=(rand()%1000)/1000.0*100.0; //position will be assigned in the group class
	pos[1]=(rand()%1000)/1000.0*100.0;
	*/
}

Group::Group(int id, CostMap * map_ptr, CellSpeed *cell_speed_ptr):group_id(id)
{
	cellSpeedPtr=cell_speed_ptr;
	mapPtr=map_ptr;
	initColor(id);
	initGoals(id);
	initPeople(id);
//	setPotenAndGradient(id);
}

Group::~Group()
{
}

void Group::initColor(int id)
{
	if(id==1)
	{
		color[0]=1.0;
		color[1]=0.0;
		color[2]=0.0;
	}
	else if(id==2)
	{
		color[0]=0.0;
		color[1]=1.0;
		color[2]=0.0;
	}
	else if(id==3)
	{
		color[0]=0.0;
		color[1]=0.0;
		color[2]=1.0;
	}
	else
	{
		color[0]=0.0;
		color[1]=1.0;
		color[2]=1.0;
	}

}

/*
the initial position of the four groups:
	
	1-----------4
	|			|
	|			|
	3-----------2

the initial position of the two groups:
	
	-------------
	| 1		  2	|
	| 1		  2	|
	-------------
*/
void Group::initPeople(int id)
{
	bool two_group=false;

#ifdef TWO_GROUP_4000_PEOPLE
	two_group=true;
#endif

#ifdef TWO_GROUP_600_PEOPLE
	two_group=true;
#endif

#ifdef TWO_GROUP_60_PEOPLE
	two_group=true;
#endif

	if(two_group)
	{
		if(id==1)
		{
			for(int i=0;i<GROUP_PEOPLE;i++)
			{
				//people_list[i].pos[0]=rand()%(MAP_WIDTH/2)+(rand()%100)/100.0;
				people_list[i].pos[1]=rand()%(MAP_HEIGHT)+(rand()%1000)/1000.0;
				people_list[i].pos[0]=(MAP_WIDTH/10)+(rand()%2==0?1:-1)*(rand()%(MAP_WIDTH/10))+(rand()%1000)/1000.0;
			}
		}
		else if(id==2)
		{
			for(int i=0;i<GROUP_PEOPLE;i++)
			{
				//people_list[i].pos[0]=rand()%(MAP_WIDTH/2)+(MAP_WIDTH/2)+(rand()%100)/100.0;
				people_list[i].pos[1]=rand()%MAP_HEIGHT+(rand()%1000)/1000.0;;
				people_list[i].pos[0]=9*(MAP_WIDTH/10)+(rand()%2==0?1:-1)*(rand()%(MAP_WIDTH/10))+(rand()%1000)/1000.0;;
			}
		}

		return;
	}
	

	if(id==1)
	{
		for(int i=0;i<GROUP_PEOPLE;i++)
		{
			//people_list[i].pos[0]=rand()%(MAP_WIDTH/2)+(rand()%100)/100.0;
			people_list[i].pos[1]=rand()%(MAP_HEIGHT/2)+(MAP_HEIGHT/2)+(rand()%1000)/1000.0;
			people_list[i].pos[0]=MAP_WIDTH/2-(people_list[i].pos[1]-(MAP_HEIGHT/2))+(rand()%2==0?1:-1)*(rand()%(MAP_WIDTH/8))+(rand()%1000)/1000.0;
		}
	}
	else if(id==2)
	{
		for(int i=0;i<GROUP_PEOPLE;i++)
		{
			//people_list[i].pos[0]=rand()%(MAP_WIDTH/2)+(MAP_WIDTH/2)+(rand()%100)/100.0;
			people_list[i].pos[1]=rand()%(MAP_HEIGHT/2)+(rand()%1000)/1000.0;
			people_list[i].pos[0]=MAP_WIDTH-people_list[i].pos[1]+(rand()%2==0?1:-1)*(rand()%(MAP_WIDTH/8))+(rand()%1000)/1000.0;
		}
	}
	else if(id==3)
	{
		for(int i=0;i<GROUP_PEOPLE;i++)
		{
			//people_list[i].pos[0]=rand()%(MAP_WIDTH/2)+(rand()%100)/100.0;
			people_list[i].pos[1]=rand()%(MAP_HEIGHT/2)+(rand()%1000)/1000.0;
			people_list[i].pos[0]=people_list[i].pos[1]+(rand()%2==0?1:-1)*(rand()%(MAP_WIDTH/8))+(rand()%1000)/1000.0;
		}
	}
	else
	{
		for(int i=0;i<GROUP_PEOPLE;i++)
		{
			//people_list[i].pos[0]=rand()%(MAP_WIDTH/2)+(MAP_WIDTH/2)+(rand()%100)/100.0;
			people_list[i].pos[1]=rand()%(MAP_HEIGHT/2)+(MAP_HEIGHT/2)+(rand()%1000)/1000.0;
			people_list[i].pos[0]=people_list[i].pos[1]-(MAP_HEIGHT/2)+(MAP_WIDTH/2)+(rand()%2==0?1:-1)*(rand()%(MAP_WIDTH/8))+(rand()%1000)/1000.0;
		}
	}

}

/*
the goal position of the four groups:
	
	2-----------3
	|			|
	|			|
	4-----------1

the goal position of the two groups:
	
	2------------
	|			|
	|			|
	------------1
*/
void Group::initGoals(int id)
{
	if(id==1)
	{
		goal[0]=MAP_WIDTH-1;
		goal[1]=0;
	}
	else if(id==2)
	{
		goal[0]=0;
		goal[1]=MAP_HEIGHT-1;
	}
	else if(id==3)
	{
		goal[0]=MAP_WIDTH-1;
		goal[1]=MAP_HEIGHT-1;
	}
	else
	{
		goal[0]=0;
		goal[1]=0;
	}

}

void Group::updateVelocity()
{
	for(int i=0;i<GROUP_PEOPLE;i++)
	{
		int cell_x, cell_y;//cell index of person i's position in the map
		cell_x=int(people_list[i].pos[0]);
		cell_y=int(people_list[i].pos[1]);

		//the person is out of the map
		if(cell_x<0 || cell_y<0 || cell_x>=MAP_WIDTH || cell_y>=MAP_HEIGHT) continue;

		people_list[i].direction[0]=-poten[cell_x][cell_y].gradient[0];
		people_list[i].direction[1]=-poten[cell_x][cell_y].gradient[1];

		double vx, vy;

		if(people_list[i].direction[0]>=0) vx=cellSpeedPtr->speed[cell_x][cell_y][EAST];
		else vx=cellSpeedPtr->speed[cell_x][cell_y][WEST];

		if(people_list[i].direction[1]>=0) vy=cellSpeedPtr->speed[cell_x][cell_y][NORTH];
		else vy=cellSpeedPtr->speed[cell_x][cell_y][SOUTH];

		people_list[i].speed=abs(vx*people_list[i].direction[0])+abs(vy*people_list[i].direction[1]);
	}
}

void Group::updatePosition()
{
	for(int i=0;i<GROUP_PEOPLE;i++)
	{
		int cell_x, cell_y;//cell index of person i's position in the map
		cell_x=int(people_list[i].pos[0]);
		cell_y=int(people_list[i].pos[1]);

		//the person is out of the map
		if(cell_x<0 || cell_y<0 || cell_x>=MAP_WIDTH || cell_y>=MAP_HEIGHT) continue;

		people_list[i].pos[0]+=people_list[i].speed*people_list[i].direction[0]*TIME_STEP;
		people_list[i].pos[1]+=people_list[i].speed*people_list[i].direction[1]*TIME_STEP;
	}
}

void  Group::setPotenAndGradient()
{
	int known_num=0;//number of known cells
	int total_cell_num=MAP_HEIGHT*MAP_WIDTH;

	priority_queue<Potential, vector<Potential>, BiggerThan> poten_pri_queue; //minimum priority queue of candidates

	for(int i=0;i<MAP_WIDTH;i++)
		for(int j=0;j<MAP_HEIGHT;j++)
		{
			poten[i][j].potential=INFI;
			poten[i][j].id=UNKNOWN;
			poten[i][j].x=i;
			poten[i][j].y=j;
		}

	poten[goal[0]][goal[1]].potential=0;
	poten[goal[0]][goal[1]].id=CANDIDATE;
	poten[goal[0]][goal[1]].gradient[0]=1;//it does not matter what gradint the goal has
	poten[goal[0]][goal[1]].gradient[0]=0;

	poten_pri_queue.push(poten[goal[0]][goal[1]]);

	//use a priority queue to do this, to save time.this queue contains all the candidates
	while(known_num<total_cell_num && !poten_pri_queue.empty())
	{
		int x,y;//index of current potential node
		int neighbor_x,neighbor_y;//neighbor index: x,y
		
		//index of the candidate with minimum potential 
		x=poten_pri_queue.top().x;
		y=poten_pri_queue.top().y;

		//candidate with minimum potential becomes known
		poten[x][y].id=KNOWN;
		known_num++;

		//since poten[x][y] is already a known node, pop it
		poten_pri_queue.pop();

		//add its neighbors into candidates list
		//neighbor to the south
		neighbor_x=x;
		neighbor_y=y-1;
		if(neighbor_y>=0)
		{
			if(poten[neighbor_x][neighbor_y].id==UNKNOWN)
			{
				setOneCell(neighbor_x,neighbor_y);
				poten_pri_queue.push(poten[neighbor_x][neighbor_y]);
			}
		}

		//neighbor to the north
		neighbor_x=x;
		neighbor_y=y+1;
		if(neighbor_y<MAP_HEIGHT)
		{
			if(poten[neighbor_x][neighbor_y].id==UNKNOWN)
			{
				setOneCell(neighbor_x,neighbor_y);
				poten_pri_queue.push(poten[neighbor_x][neighbor_y]);
			}
		}

		//neighbor to the west
		neighbor_x=x-1;
		neighbor_y=y;
		if(neighbor_x>=0)
		{
			if(poten[neighbor_x][neighbor_y].id==UNKNOWN)
			{
				setOneCell(neighbor_x,neighbor_y);
				poten_pri_queue.push(poten[neighbor_x][neighbor_y]);
			}
		}

		//neighbor to the east
		neighbor_x=x+1;
		neighbor_y=y;
		if(neighbor_x<MAP_WIDTH)
		{
			if(poten[neighbor_x][neighbor_y].id==UNKNOWN)
			{
				setOneCell(neighbor_x,neighbor_y);
				poten_pri_queue.push(poten[neighbor_x][neighbor_y]);
			}
		}

		
	}

}

void Group::setOneCell(int neighbor_x, int neighbor_y)
{
	
	poten[neighbor_x][neighbor_y].id=CANDIDATE;

	int mx, my;
	double cost_east,cost_west,cost_north,cost_south;
	double poten_east, poten_west, poten_north, poten_south;

	cost_east=mapPtr->cost[neighbor_x][neighbor_y][EAST];
	cost_west=mapPtr->cost[neighbor_x][neighbor_y][WEST];
	cost_north=mapPtr->cost[neighbor_x][neighbor_y][NORTH];
	cost_south=mapPtr->cost[neighbor_x][neighbor_y][SOUTH];
	
	if(neighbor_x+1<MAP_WIDTH)
	{
		poten_east= poten[neighbor_x+1][neighbor_y].potential + cost_east;
	}
	else poten_east=INFI+cost_east;

	if(neighbor_x-1>=0)
	{
		poten_west= poten[neighbor_x-1][neighbor_y].potential + cost_west;
	}
	else poten_west=INFI+cost_west;
	
	if(poten_east<poten_west) mx=EAST;
	else mx=WEST;
	if(poten_east>=INFI && poten_west>=INFI) mx=-1;

	if(neighbor_y+1<MAP_HEIGHT)
	{
		poten_north= poten[neighbor_x][neighbor_y+1].potential + cost_north;
	}
	else poten_north=INFI+cost_north;

	if(neighbor_y-1>=0)
	{
		poten_south= poten[neighbor_x][neighbor_y-1].potential + cost_south;
	}
	else poten_south=INFI+cost_south;

	if(poten_north<poten_south) my=NORTH;
	else my=SOUTH;
	if(poten_north>=INFI && poten_south>=INFI) my=-1;

	if(mx==-1 && my==-1) { exit(1);}//this is impossible to happen. if it happens, cost map should be wrong
	else if(mx!=-1 && my!=-1)
	{
		//to do: calculate Equation(11) in the paper
		//use Equation (5.10) in the paper to calculate: 
		//Simulation of large and dense crowds on the GPU using OpenCL
		double PHI_mx, PHI_my,cost_mx,cost_my;
		if(mx==EAST)
		{
			PHI_mx=poten[neighbor_x+1][neighbor_y].potential;
			cost_mx=mapPtr->cost[neighbor_x][neighbor_y][EAST];
		}
		else
		{
			PHI_mx=poten[neighbor_x-1][neighbor_y].potential;
			cost_mx=mapPtr->cost[neighbor_x][neighbor_y][WEST];
		}

		if(my==NORTH)
		{
			PHI_my=poten[neighbor_x][neighbor_y+1].potential;
			cost_my=mapPtr->cost[neighbor_x][neighbor_y][NORTH];
		}
		else
		{
			PHI_my=poten[neighbor_x][neighbor_y-1].potential;
			cost_my=mapPtr->cost[neighbor_x][neighbor_y][SOUTH];
		}

		double delta=2*pow(cost_mx+cost_my,2)-pow(PHI_mx-PHI_my,2);
		if(delta<0) //this won't happen
		{
			poten[neighbor_x][neighbor_y].potential =(cost_mx+cost_my) + ( (PHI_mx<PHI_my)? PHI_mx:PHI_my );
		}
		else 
		{
			poten[neighbor_x][neighbor_y].potential=( (PHI_mx+PHI_my)+pow(delta,0.5) ) / 2.0;
		}

		double gx,gy;
		//calculate gradient
		if(mx==EAST) gx=poten[neighbor_x+1][neighbor_y].potential-poten[neighbor_x][neighbor_y].potential;
		else gx=poten[neighbor_x][neighbor_y].potential-poten[neighbor_x-1][neighbor_y].potential;

		if(my==NORTH) gy=poten[neighbor_x][neighbor_y+1].potential-poten[neighbor_x][neighbor_y].potential;
		else gy=poten[neighbor_x][neighbor_y].potential-poten[neighbor_x][neighbor_y-1].potential;

		double temp=gx*gx+gy*gy;
		temp=pow(temp, 0.5);
		if(temp==0) { exit(2);}//this should not happen
		poten[neighbor_x][neighbor_y].gradient[0]=gx/temp;
		poten[neighbor_x][neighbor_y].gradient[1]=gy/temp;					
	}
	else if(mx!=-1 && my==-1)
	{
			
		double PHI_mx,cost_mx,cost_my;
		if(mx==EAST)
		{
			PHI_mx=poten[neighbor_x+1][neighbor_y].potential;
			cost_mx=mapPtr->cost[neighbor_x][neighbor_y][EAST];
			poten[neighbor_x][neighbor_y].gradient[0]=-1;
		}
		else
		{
			PHI_mx=poten[neighbor_x-1][neighbor_y].potential;
			cost_mx=mapPtr->cost[neighbor_x][neighbor_y][WEST];
			poten[neighbor_x][neighbor_y].gradient[0]=1;
		}

		if(my==NORTH)
		{
			cost_my=mapPtr->cost[neighbor_x][neighbor_y][NORTH];
		}
		else
		{
			cost_my=mapPtr->cost[neighbor_x][neighbor_y][SOUTH];
		}

		poten[neighbor_x][neighbor_y].potential =(cost_mx+cost_my) + PHI_mx;

		poten[neighbor_x][neighbor_y].gradient[1]=0;

	}
	else if(mx==-1 && my!=-1)
	{
		double PHI_my,cost_mx,cost_my;
		if(mx==EAST)
		{
			cost_mx=mapPtr->cost[neighbor_x][neighbor_y][EAST];
		}
		else
		{
			cost_mx=mapPtr->cost[neighbor_x][neighbor_y][WEST];
		}

		if(my==NORTH)
		{
			PHI_my=poten[neighbor_x][neighbor_y+1].potential;
			cost_my=mapPtr->cost[neighbor_x][neighbor_y][NORTH];

			poten[neighbor_x][neighbor_y].gradient[1]=-1;
		}
		else
		{
			PHI_my=poten[neighbor_x][neighbor_y-1].potential;
			cost_my=mapPtr->cost[neighbor_x][neighbor_y][SOUTH];

			poten[neighbor_x][neighbor_y].gradient[1]=1;
		}

		poten[neighbor_x][neighbor_y].potential =(cost_mx+cost_my) + PHI_my;

		poten[neighbor_x][neighbor_y].gradient[0]=0;
					
	}
				
}