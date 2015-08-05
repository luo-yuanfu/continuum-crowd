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

class Map
{
public:
	Group *groupPtr[GROUP_NUM];
	double density[MAP_WIDTH][MAP_HEIGHT];
	double ave_vel[MAP_WIDTH][MAP_HEIGHT][2];
	CostMap cost_map;
	CellSpeed cell_speed;
	double min_density;
	double max_density;

	Map();
	~Map();
	void calcCostMap();//calculate cost map;
	void initCellSpeed();
	void zeroDensity();//assign 0 to Density
	void zeroAveVelocity();//assign 0 to the average velocity
	void resetDensityAndAveVel();

	void updateDensityAndAveVel();
	void updateCellSpeed();

	void update();
};