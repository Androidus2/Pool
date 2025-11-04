#pragma once
#include "Vector2.h"
#include <vector>
#include "glm/glm.hpp"
#include <GL/glew.h>       
#include <GL/freeglut.h> 
#include "Manta.h"

struct Circle {

	Vector2 center;
	float radius;
	Vector2 velocity;
	GLuint textureId;
	bool active = true;


	Circle(Vector2 center, float radius);
	Circle(Vector2 center, float radius, Vector2 velocity);
	Circle(Vector2 center, float radius, const char* filePath);


	std::vector<Vector2> getPoints();

	void moveCircle( int xMin, int xMax, int yMin, int yMax, float deltaTime = 1);
	void drawCircle(int VboId, int myMatrixLocation, glm::mat4 myMatrix, int textureLocation);
	void onHitBall(Circle& other);
	void onHitManta(Manta& manta);
	void collisionManta(Manta& manta);

};
