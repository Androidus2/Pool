#pragma once
#include "Circle.h"
#include "Vector2.h"
#include <vector>
#include "glm/glm.hpp"	

struct Tac
{
	float speed=0;
	Vector2 direction;
	Vector2 mousePos;
	Vector2 whiteBallCenter;
	Vector2 leftUppermostPoint;
	Vector2 rightUppermostPoint;
	Vector2 leftLowermostPoint;
	Vector2 rightLowermostPoint;
	float thickness;
	float length;
	float incline;
	bool visible = true;
	float offset;
	
	glm::mat4 matTranslation = glm::mat4(1.f);
	glm::mat4 matRotation = glm::mat4(1.f);

	Tac() {}
	void drawTac(int VboId, int myMatrixLocation, glm::mat4 myMatrix);

	void pointToWhiteBall(Circle whiteBall);
	void moveToWhiteBall();

};
