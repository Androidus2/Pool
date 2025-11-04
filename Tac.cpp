#include "Tac.h"
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
void Tac::drawTac(int VboId, int myMatrixLocation, glm::mat4 myMatrix)
{

	std::vector<Vector2> points;

	points.push_back(leftUppermostPoint);
	points.push_back(rightUppermostPoint);
	points.push_back(rightLowermostPoint);
	points.push_back(leftLowermostPoint);

	std::vector<GLfloat> tacVertices;
	for (auto pct : points)
	{
		tacVertices.push_back(pct.x);
		tacVertices.push_back(pct.y);
		tacVertices.push_back(0.0f);
		tacVertices.push_back(1.0f);
	}


	myMatrix = myMatrix * matTranslation  *matRotation ;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBufferData(GL_ARRAY_BUFFER, tacVertices.size() * sizeof(GLfloat), tacVertices.data(), GL_STATIC_DRAW);

	glPointSize(10);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


};

void Tac::moveToWhiteBall()
{
	Vector2 center = { (rightUppermostPoint.x + rightLowermostPoint.x) / 2, (rightUppermostPoint.y + rightLowermostPoint.y) / 2 };
	Vector2 distanceToBall = { whiteBallCenter.x - center.x, whiteBallCenter.y - center.y };

	this->matTranslation = glm::translate(glm::mat4(1.f), glm::vec3(distanceToBall.x  , distanceToBall.y , 0.f));
}


void Tac::pointToWhiteBall(Circle whiteBall) {

	//float offset = 25.f + speed * 2.f;
	float offset = this->offset ;

	Vector2 pivot = { (rightUppermostPoint.x + rightLowermostPoint.x) / 2, (rightUppermostPoint.y + rightLowermostPoint.y) / 2 };

	float dx = this->mousePos.x - whiteBall.center.x;
	float dy = this->mousePos.y - whiteBall.center.y ;
	float angle = atan2(dy, dx);

	
	this->matRotation = glm::translate(glm::mat4(1.f), glm::vec3(pivot.x, pivot.y, 0.f)) *
		glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 0, 1)) *
		glm::translate(glm::mat4(1.f), glm::vec3(-1 * pivot.x, -1 * pivot.y,0.f)) *
		glm::translate(glm::mat4(1.f), glm::vec3(-offset, 0.f, 0.f));
}

