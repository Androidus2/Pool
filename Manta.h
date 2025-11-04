#pragma once
#include "Vector2.h"
#include "glm/glm.hpp"

#include <vector>

struct Manta
{
	Vector2 point1; 
	Vector2 point2; 
	Vector2 point3; 
	Vector2 point4; 
	bool hit = false;
	Vector2 mainsegment1, mainsegment2;


	void  drawManta(int VboId, int myMatrixLocation, glm::mat4 myMatrix);

	//constructor wiht all params
	Manta(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, Vector2 mainsegment1, Vector2 mainsegment2 )
		: point1(p1), point2(p2), point3(p3), point4(p4), mainsegment1(mainsegment1), mainsegment2(mainsegment2)
	{
	}
	
	std::vector<Vector2> getPoints();

};