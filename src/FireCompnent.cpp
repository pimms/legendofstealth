#include "FireCompnent.h"


FireComponent::FireComponent() {}


void FireComponent::update(){}

/* SOMETHING LIKE THIS?
bool lineOfSight(Vec2 PlayerPosition, Vec2 EnemyPosition)
{
	Vec2 pointA = EnemyPosition;
	Vec2 pointB = PlayerPosition;
	for (int i = 0; i<NbrObstacles; i++){
		// Check linexObstacle(if your using rectangle's look up linexRectangle, 
		// for all simple shapes, their's simple formula, for complex shapes, 
		// you simple loop though all the edges, and check linexPlane(in 2D, it's basically linexline collision).)
		
		if (IntersectionWithObstacleI) 
			PointB = Intesect location; //(if we just care if the other tank can't see the player, you could return false for a line of sight function here.
	}
	
	return (PointB == PlayerPosition) ? 1 : 0;
}
*/