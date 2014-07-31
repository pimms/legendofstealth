#include "PhysicsDebugDrawer.h"




/*
================
PhysicsDebugDrawer Public 
================
*/
void PhysicsDebugDrawer::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, 
					const b2Color& color)
{
	b2Color c = color;
	c.a = 0.2f;
	DrawPolygon(vertices, vertexCount, c, GL_LINE_LOOP);
}


void PhysicsDebugDrawer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, 
						const b2Color& color)
{
	b2Color c = color;
	c.a = 0.4f;
	DrawPolygon(vertices, vertexCount, c, GL_POLYGON);
}


void PhysicsDebugDrawer::DrawCircle(const b2Vec2& center, float32 radius, 
				const b2Color& color)
{
	b2Color c = color;
	c.a = 0.2f;
	DrawCircle(center, radius, c, GL_TRIANGLE_FAN);
}


void PhysicsDebugDrawer::DrawSolidCircle(const b2Vec2& center, float32 radius, 
						const b2Vec2& axis, const b2Color& color)
{
	b2Color c = color;
	c.a = 0.4f;
	DrawCircle(center, radius, c, GL_LINE_LOOP);
}


void PhysicsDebugDrawer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, 
					const b2Color& color)
{
	CommonPreRender();

	GLfloat verts[4] = {
		p1.x, p1.y,
		p2.x, p2.y
	};
	
	glColor4f(color.r, color.g, color.b, 0.4f);
	glVertexPointer(2, GL_FLOAT, 0, verts);
	glDrawArrays(GL_LINES, 0, 2);

	CommonPostRender();
}

void PhysicsDebugDrawer::DrawTransform(const b2Transform& xf)
{
	CommonPreRender();

	GLfloat verts[] = {
		0.f, 0.f,
		1.f, 0.f,

		1.f, 0.f,
		0.95f, 0.04f,

		1.f, 0.f,
		0.95f, -0.04f,
	};

	glRotatef(xf.q.GetAngle(), 0.f, 0.f, 1.f);
	
	glColor4ub(255, 0, 0, 200);
	glVertexPointer(2, GL_FLOAT, 0, verts);
	glDrawArrays(GL_LINES, 0, 6);

	CommonPostRender();
}


/*
================
PhysicsDebugDrawer Private
================
*/
void PhysicsDebugDrawer::CommonPreRender() 
{
	glPushMatrix();
	glScalef(B2_PTM, B2_PTM, 1.f);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

void PhysicsDebugDrawer::CommonPostRender()
{
	glPopMatrix();
	glDisableClientState(GL_VERTEX_ARRAY);
	glColor3ub(255, 255, 255);
}


void PhysicsDebugDrawer::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, 
					const b2Color& color, GLenum drawMode)
{
	CommonPreRender();

	glColor4f(color.r, color.g, color.b, color.a);
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays(drawMode, 0, vertexCount);

	CommonPostRender();
}


void PhysicsDebugDrawer::DrawCircle(const b2Vec2& center, float32 radius, 
				const b2Color& color, GLenum drawMode)
{
	CommonPreRender();

	glTranslatef(center.x, center.y, 0.f);

	vector<b2Vec2> pts;

	if (drawMode == GL_TRIANGLE_FAN)
		pts.push_back(b2Vec2(0.f, 0.f));

	float r = 0.f;
	const float pi2 = M_PI * 2.f;
	const int tris = 20;
	while (r < pi2) {
		b2Vec2 v(radius * cos(r), radius * sin(r));
		pts.push_back(v);
		r += (pi2 / (float)tris);
	}

	// Close the circle
	if (drawMode == GL_TRIANGLE_FAN) 
		pts.push_back(pts[1]);

	// Draw it
	glColor4f(color.r, color.g, color.b, color.a);
	glVertexPointer(2, GL_FLOAT, 0, &pts[0]);
	glDrawArrays(drawMode, 0, pts.size());

	CommonPostRender();
}
