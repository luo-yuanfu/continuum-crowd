##Description
This is my final project for Module CS5343 in National University of Singapore. 
This project is based on the paper: Treuille, A., Cooper, S., & Popović, Z. (2006). Continuum crowds. ACM Transactions on Graphics (TOG), 25(3), 1160-1168.

##Prerequisites
This program is developed in Microsoft Visual Studio 2010 with OpenGL installed.

##Parameter Usages
In parameters.h file, comment and uncomment corresponding lines to run different tests. For example, you can run the test with two groups and 60 people with the following lines.

	#define TWO_GROUP_60_PEOPLE
	//#define TWO_GROUP_600_PEOPLE
	//#define TWO_GROUP_4000_PEOPLE
	//#define FOUR_GROUP_800_PEOPLE

if you want to run the test with two groups and 4000 people, you can do it with the following lines:

	//#define TWO_GROUP_60_PEOPLE
	//#define TWO_GROUP_600_PEOPLE
	#define TWO_GROUP_4000_PEOPLE
	//#define FOUR_GROUP_800_PEOPLE
