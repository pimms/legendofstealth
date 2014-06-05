#include "RayCastUtility.h"


RayCastUtility::RayCastUtility() : m_fixture(NULL) {}

float32 RayCastUtility::ReportFixture(b2Fixture* fixture, const b2Vec2& point, 
									  const b2Vec2& normal, float32 fraction)


{
	m_fixture = fixture;
	m_point = point;
	m_normal = normal;
	m_fraction = fraction;
	return m_fraction;

	//GameObject *go = _fixture->GetUserData();
	//if (dynamic_cast<Player*>(go)) 
}
