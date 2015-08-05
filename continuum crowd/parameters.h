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

//comment and uncomment corresponding lines to run different tests

//#define TWO_GROUP_60_PEOPLE
//#define TWO_GROUP_600_PEOPLE
#define TWO_GROUP_4000_PEOPLE
//#define FOUR_GROUP_800_PEOPLE

#ifdef TWO_GROUP_60_PEOPLE
#define GROUP_PEOPLE 30
#define PERSON_SIZE 0.3
#define MAX_DENSITY 1.5
#define GROUP_NUM 2
#endif

#ifdef TWO_GROUP_600_PEOPLE
#define GROUP_PEOPLE 300
#define PERSON_SIZE 0.15
#define MAX_DENSITY 3
#define GROUP_NUM 2
#endif

#ifdef TWO_GROUP_4000_PEOPLE
#define GROUP_PEOPLE 2000
#define PERSON_SIZE 0.09
#define MAX_DENSITY 15
#define GROUP_NUM 2
#endif

#ifdef FOUR_GROUP_800_PEOPLE
#define GROUP_PEOPLE 200
#define PERSON_SIZE 0.15
#define MAX_DENSITY 3
#define GROUP_NUM 4
#endif

#define INFI 100000 //infinity

#define RESOLUTION 15
#define MAP_WIDTH 40
#define MAP_HEIGHT 40

#define INIT_SPEED 4.5
#define INIT_CELL_SPEED 0.5

#define TIME_STEP 0.02

#define NORTH 0
#define SOUTH 1
#define WEST 2
#define EAST 3

#define KNOWN 0
#define UNKNOWN 1
#define CANDIDATE 2

#define MIN_DENSITY 0.01

#define MAX_SPEED 3.0
#define MIN_FLOW_SPEED 0.06

#define PATH_WEIGHT 1
#define TIME_WEIGHT 1
#define DISCOMFORT_WEIGHT 1

#define DISCOMFORT 1

#define LAMBDA 1


