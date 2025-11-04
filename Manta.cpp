#include "Manta.h";
#include <GL/glew.h>       
#include <GL/freeglut.h> 
#include "Line.h"

void Manta::drawManta(int VboId, int myMatrixLocation, glm::mat4 myMatrix)
{
	GLfloat mantaVertices[] = {
		point1.x, point1.y, 0.0f, 1.0f,
		point2.x, point2.y, 0.0f, 1.0f,
		point3.x, point3.y, 0.0f, 1.0f,
		point4.x, point4.y, 0.0f, 1.0f,
	};
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mantaVertices), mantaVertices, GL_STATIC_DRAW);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

std::vector <Vector2> Manta::getPoints()
{
	std::vector<Vector2> points;
	points.push_back(point1);
	points.push_back(point2);
	points.push_back(point3);
	points.push_back(point4);
	return points;
}

std::vector<Line> Manta::getSlantedSegments()
{
	std::vector<Line> segments;
	Line line1 = { point2, point3 };
	Line line2 = { point1, point4 };
	segments.push_back(line1);
	segments.push_back(line2);
	return segments;
}