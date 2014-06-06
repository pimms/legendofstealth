#include "Entity.h"

#ifdef _WIN32
#include <res/Texture.h>
#else
#include <trutle/res/Texture.h>
#endif

Vec2 ToVec2(b2Vec2 bvec)
{
	return Vec2(bvec.x * PTM, bvec.y * PTM);
}

b2Vec2 Tob2Vec2(Vec2 vec)
{
	return b2Vec2(vec.x / PTM, vec.y / PTM);
}

//push
float Deg2Rad(float deg)
{
	return deg * (M_PI / 180.f);
}

float Rad2Deg(float rad)
{
	return rad * (180.f / M_PI);
}


/*
================
Entity Public 
================
 */
Entity::Entity(b2World *world)
	:	_world(world),
		_body(NULL)
{
	
}

Entity::~Entity()
{
	if (_body) {
		_world->DestroyBody(_body);
	}
}


void Entity::Update(const DeltaTime &dt)
{
	GameObject::Update(dt);

	if (_body) {
		_body->SetTransform(_body->GetPosition(), Deg2Rad(Rotation()));

		Vec2 bpos = ToVec2(_body->GetPosition());
		//bpos.x -= GetTexture()->GetDimensions().x * (0.5f - Pivot().x);
		//bpos.y -= GetTexture()->GetDimensions().y * (0.5f - Pivot().y);
		Position() = bpos;
	}
}

void Entity::Render(Renderer *renderer)
{
	if (_body && false) {
		b2Fixture *fix = _body->GetFixtureList();
		b2PolygonShape *shape = (b2PolygonShape*)fix->GetShape();

		renderer->PushTransform();
		glLoadIdentity();
		
		Vec2 pos = ToVec2(_body->GetPosition());
		glTranslatef(pos.x, pos.y, 0.f);

		glColor4ub(255, 0, 0, 128);
		glBegin(GL_TRIANGLE_FAN);

		for (int i=0; i<shape->GetVertexCount(); i++) {
			b2Vec2 b2v = shape->GetVertex(i);
			Vec2 v = ToVec2(b2v);
			glVertex2f(v.x, v.y);
		}

		glEnd();

		renderer->PopTransform();
		glColor4ub(255, 255, 255, 255);
	}


	GameObject::Render(renderer);
}

Vec2 Entity::GetScreenPosition()
{
	Vec2 layerPos = GetParentLayer()->Position();
	Vec2 pos = Position();

	pos.x += layerPos.x;
	pos.y += layerPos.y;
	return pos;
}

void Entity::MoveB2Body(Vec2 velocity)
{
	_body->SetLinearVelocity(Tob2Vec2(velocity));
}

void Entity::SetPosition(Vec2 position)
{
	_body->SetTransform(Tob2Vec2(position), Deg2Rad(Rotation()));
}


b2Body* Entity::GetBody()
{
	return _body;
}

b2World* Entity::GetWorld()
{
	return _world;
}


/*
================
Entity Protected
================
*/
void Entity::CreateSquareBody(b2BodyType type)
{
	Vec2 dim = GetTexture()->GetDimensions();
	dim.x *= Scale().x;
	dim.y *= Scale().y;

	Vec2 pos = Position();
	//pos.x += (0.5f - Pivot().x) * dim.x;
	//pos.y += (0.5f - Pivot().y) * dim.y;

	b2BodyDef bd;
	bd.position = Tob2Vec2(Position());
	bd.type = type;
	bd.userData = this;
	bd.position = Tob2Vec2(pos);


	b2Vec2 b2dim = Tob2Vec2(dim);
	b2PolygonShape shape;
	shape.SetAsBox(b2dim.x/2.f, b2dim.y/2.f);
	
	b2FixtureDef fd;
	fd.isSensor = false;
	fd.density = 3.5f;
	fd.userData = this;
	fd.shape = &shape;
	fd.friction = 1.f;

	_body = _world->CreateBody(&bd);
	_body->CreateFixture(&fd);
}
