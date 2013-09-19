// File: Defines.h
 

#ifndef _DEFINES_H
#define _DEFINES_H

//initial window size
#define INIT_WW 800
#define INIT_WH 800

//map size
#define WORLD_X 60
#define WORLD_Y 1
#define WORLD_Z 60

//bezier surface control point order
#define SURFACE_U_ORDER 8
#define SURFACE_V_ORDER 8
#define TESSELATE_DEGREE_Z 40
#define TESSELATE_DEGREE_X 40
#define MAX_TERRAIN_ELEVATION 20
#define NUM_CONTOUR_BANDS 10

//initial view frustum
#define FRUSTUM_TOP		 3
#define FRUSTUM_BOTTOM  -3
#define FRUSTUM_LEFT	-3	
#define FRUSTUM_RIGHT	 3
#define FRUSTUM_NEAR	 6
#define FRUSTUM_FAR	   150	
#define VIEW_ANGLE	  50.0


//constants for map representation
#define CLEAR_SPACE    0xFF
#define BLOCKED_SPACE  0xFE
//other numbers represent # of agent
#define MAX_AGENTS     12

//percentage of map blocked
#define PERCENT_BLOCKED 20

//penalty for changing elevation on the path selected
//#define CLIMBING_PENALTY 1.0f


//timing constants
#define UPDATE_PERIOD	500
#define REFRESH_PERIOD  33


//select buffer size
#define SELECT_BUF_SIZE 64

//map types
enum e_EnvType
{
	CLEAR_WORLD,
	RANDOM_WORLD,
	RANDOM_2BY2_WORLD
};

//movement strategies
enum e_MoveStrategy
{
	RANDOM,
	DEPTH_FIRST	,
	BREADTH_FIRST,
	ITERATIVE_DEEPENING,
    A_STAR,
	IDA_STAR,
	GREEDY,
	QUICK

};

//math constants
#define TWO_PI			6.283185307	
#define PI				3.141592654
#define PI_OVER_TWO		1.570796327
#define MAX_TEXTURES	5
#define TEXSIZE			64
#define SQRT_TWO		1.414213562
#define HALF_SQRT_TWO	.7071067812
#define TEN_DEG			.1745

struct TripleInt
{

	int x;
	int y;
	int z;
};

struct TripleFloat
{
	float x;
	float y;
	float z;
};

extern TripleInt LogPosToReal(int space, int worldX);

#endif
