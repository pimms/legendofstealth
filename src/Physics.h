#pragma once 


#include <Box2D/Box2D.h>
#include <trutle/Trutle.h>


/* Pixel To b2Meter Definitions
 * Box2D operates in meters, whereas rendering and game logic operates
 * in pixels. One b2Meter corresponds to 32 pixels. Each tile is therefore
 * 2 by 2 meters.
 */
#define MAP_TILE_SIZE 			64
#define B2METER_PER_TILE 		2
#define B2_PTM 					((float)(MAP_TILE_SIZE / B2METER_PER_TILE))

/* Collision Filter Definitions
 * Each b2Body must have defined what IT is (collision category), and
 * what it CAN collide with (collision mask). 
 */
enum ColliderGroups {
	WALL 				= 0x01,
	VENTILATION 		= 0x02,
	TERM_AREA 			= 0x04,

	PLAYER				= 0xF0,

	ALL 				= ~0,
};


inline b2Vec2 ToB2Vec2(Vec2 vec2) 
{
	return b2Vec2(vec2.x / (float)B2_PTM, vec2.y / (float)B2_PTM);
}

inline Vec2 ToVec2(b2Vec2 vec)
{
	return Vec2(vec.x * (float)B2_PTM, vec.y * (float)B2_PTM);
}
