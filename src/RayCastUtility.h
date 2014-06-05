
// Ray cast callback class helper

#pragma once
#include <Box2D/Box2D.h>

class RayCastUtility : public b2RayCastCallback
{

public:

		
		RayCastUtility();
	
		float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point,
							  const b2Vec2& normal, float32 fraction);
		
		
		// Public members
		b2Fixture*  m_fixture;
		b2Vec2		m_point;
		b2Vec2		m_normal;
		float32		m_fraction;

};
