#include "Circle.h"
#include <math.h>
#include <GL/glew.h> 
#include <GL/freeglut.h>
#include "SOIL.h"			//	Biblioteca pentru texturare;
#include<iostream>
#include "glm/glm.hpp"		//	Bibloteci utilizate pentru transformari grafice;
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Line.h"

GLuint LoadTexture(const char* texturePath, GLuint &texture)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generarea unui obiect textura si legarea acestuia;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//	Desfasurarea imaginii pe orizontala/verticala in functie de parametrii de texturare;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Modul in care structura de texeli este aplicata pe cea de pixeli;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Incarcarea texturii si transferul datelor in obiectul textura; 
	int width, height, channels;
	unsigned char* image = SOIL_load_image(texturePath, &width, &height, 0, SOIL_LOAD_RGBA);

	if (image == nullptr) {
		std::cout << "Texture loading failed for: " << texturePath << std::endl;
		return texture;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	// Eliberarea resurselor
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

Circle::Circle() {}
Circle::Circle(Vector2 center, float radius) {
	this->center = center;
	this->radius = radius;
	this->velocity = Vector2{ 0,0 };
}

Circle::Circle(Vector2 center, float radius, Vector2 Velocity) {
	this->center = center;
	this->radius = radius;
	this->velocity = velocity;
}


Circle::Circle(Vector2 center, float radius,  const char* filePath)
{
	this->center = center;
	this->radius = radius;
	this->velocity = Vector2{ 0,0 };
	this->textureId = LoadTexture(filePath, this->textureId);
}

std::vector<Vector2> Circle::getPoints() {
	std::vector<Vector2> points;
	float edge = 2 * this->radius;
	Vector2 topLeft = { this->center.x - this->radius, this->center.y + this->radius };
	points.push_back(topLeft);
	points.push_back(topLeft + Vector2{ edge, 0 });
	points.push_back(topLeft + Vector2{ edge, -edge });
	points.push_back(topLeft + Vector2{ 0, -edge });

	return points;
}

void Circle::moveCircle( int xMin, int xMax, int yMin, int yMax, float deltaTime ) {
	
	this->center = this->center + this->velocity * deltaTime;

	if (this->center.x - this->radius < xMin)
	{
		this->center.x = xMin + this->radius;
		this->velocity.x = -this->velocity.x;
	}
	if (this->center.x + this->radius > xMax)
	{
		this->center.x = xMax - this->radius;
		this->velocity.x = -this->velocity.x;
	}
	if (this->center.y - this->radius < yMin)
	{
		this->center.y = yMin + this->radius;
		this->velocity.y = -this->velocity.y;
	}
	if (this->center.y + this->radius > yMax)
	{
		this->center.y = yMax - this->radius;
		this->velocity.y = -this->velocity.y;
	}


	float friction = 0.99f;
	this->velocity  = this->velocity * friction;

	if ( this->velocity.GetSqrMagnitude() < 1 )
		this->velocity = Vector2{ 0,0 };
}

void Circle::onHitBall(Circle& other)
{
	Vector2 dir = other.center - this->center;
	float distance = dir.GetMagnitude();
	float overlap = 2 * radius - distance;

	Vector2 normal = dir.GetNormalized();
	this->center -= normal * (overlap / 2);
	other.center += normal * (overlap / 2);

	Vector2 tangent{ normal.y * -1, normal.x };

	float componentaNormala1 = this->velocity.Dot(normal);
	float componentaTangentiala1 = this->velocity.Dot(tangent);
	float componentaNormala2 = other.velocity.Dot(normal);
	float componentaTangentiala2= other.velocity.Dot(tangent);

	float cn1Final = componentaNormala2;
	float cn2Final = componentaNormala1;

	this->velocity = tangent * componentaTangentiala1 + normal * cn1Final;
	other.velocity = tangent * componentaTangentiala2 + normal * cn2Final;

}

void Circle::drawCircle(int VboId, int myMatrixLocation, glm::mat4 myMatrix, int textureLocation)
{
	std::vector<Vector2> points = this->getPoints();
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->textureId);
	glUniform1i(textureLocation, 0);



	std::vector<float> drawpoints;
	for (auto p : points)
	{
		drawpoints.push_back(p.x);
		drawpoints.push_back(p.y);
		drawpoints.push_back(0.f);
		drawpoints.push_back(1.f);
	}
	glm::mat4 finalMatrix = myMatrix * transformMatrix;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &finalMatrix[0][0]);


	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBufferData(GL_ARRAY_BUFFER, drawpoints.size() * sizeof(float), drawpoints.data(), GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLE_FAN, 0, drawpoints.size() / 4);

}

void Circle::collisionManta( Manta &manta)
{
	//Verifying with main segment
	
	Vector2 a = manta.mainsegment1;
	Vector2 b = manta.mainsegment2;

	float distSq = center.DistanceToSegmentSqr(a, b);
	bool hit = (distSq <= radius * radius);
	if (hit)
	{
		if (a.y > 250 && b.y > 250)
		{
			// Manta sus
			if (this->velocity.y > 0)
				this->velocity.y = -this->velocity.y;
		}
		else if (b.y < 250 && a.y < 250)
		{
			// Manta jos
			if (this->velocity.y < 0)
				this->velocity.y = -this->velocity.y;
		}
		else if (a.x < 200 && b.x < 200)
		{
			// Manta stanga
			if (this->velocity.x < 0)
				this->velocity.x = -this->velocity.x;
		}
		else if (b.x > 800 && a.x > 800)
		{
			// Manta dreapta
			if (this->velocity.x > 0)
				this->velocity.x = -this->velocity.x;
		}
	}

	// Verifying with slanted segments
	std::vector<Line> segments = manta.getSlantedSegments();
	for (auto segment : segments)
	{

		float distTo = center.DistanceToSegmentSqr(segment.p1, segment.p2);
		bool hit = (distTo <= radius * radius);

		if (hit)
		{
			Vector2 segmentDist = segment.p2 - segment.p1;
			Vector2 normal = Vector2{ -segmentDist.y, segmentDist.x };
			normal.Normalize();
			float vDotN = this->velocity.Dot(normal);
			this->velocity = this->velocity - normal  * vDotN * 2;

		}
	}
}

void Circle::animate(float deltaTime)
{
	if (!animPlaying) return;

	animTime += deltaTime;
	float t = animTime / animDuration;
	if (t >= 1.0f) {
		t = 1.0f;
		animPlaying = false;
	}

	float scale = t; // este in [0,1]
	float rotationRadians = glm::radians(360.0f * t);

	glm::mat4 origineScena = glm::translate(glm::mat4(1.0f), glm::vec3(-this->center.x, -this->center.y, 0.0f));
	glm::mat4 matScale = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 1.0f));
	glm::mat4 matRotation = glm::rotate(glm::mat4(1.0f), rotationRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), glm::vec3(this->center.x, this->center.y, 0.0f));

	transformMatrix = translateBack * matRotation * matScale * origineScena;
}

void Circle::playAnimation(float durationSeconds)
{
	active = true;
	animDuration = durationSeconds;
	animTime = 0.0f;
	animPlaying = true;
	transformMatrix = glm::mat4(1.0f);
}

