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

#include "parameters.h"
#include <windows.h>
#include <time.h>
#include <iostream>
#include <queue>
#include <math.h>

using namespace std;

class CostMap
{
public:
	double cost[MAP_WIDTH][MAP_HEIGHT][4];
};

class CellSpeed
{
public:
	double speed[MAP_WIDTH][MAP_HEIGHT][4];//the cell speed in the four directions
};

class Person
{
public:
	double pos[2]; //x, y
	double speed;
	double direction[2];

	Person();
};


class Potential
{
public:
	double potential;
	double gradient[2];//x, y
	int id;//known, uknown, or candidates
	int x;//x index in the map
	int y;//y index in the map
};

struct BiggerThan
{
	bool operator()(const Potential lhs, const Potential rhs) const
	{
		return lhs.potential>rhs.potential;
	}
};

class Group
{
public:
	int group_id;
	Person people_list[GROUP_PEOPLE];
	double color[3]; //r,g,b
	Potential poten[MAP_WIDTH][MAP_HEIGHT];
	CostMap *mapPtr;//the pointer to costmap
	CellSpeed *cellSpeedPtr;
	int goal[2];//x, y

	Group(int id,  CostMap * map_ptr, CellSpeed * cell_speed_ptr);
	void initColor(int id);
	void setPotenAndGradient();//set potential and its gradient
	void setOneCell(int neighbor_x, int neighbor_y);//set the potential and its gradient for one cell
	void initPeople(int id);
	void initGoals(int id);
	void updateVelocity();
	void updatePosition();
	~Group();
};


