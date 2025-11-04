#include "Circle.h"
#include <math.h>
#include <GL/glew.h> 
#include <GL/freeglut.h>
#include "SOIL.h"			//	Biblioteca pentru texturare;
#include<iostream>
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
	float latura = 2 * this->radius;
	Vector2 topLeft = { this->center.x - this->radius, this->center.y + this->radius };
	points.push_back(topLeft);
	points.push_back(topLeft + Vector2{ latura, 0 });
	points.push_back(topLeft + Vector2{ latura, -latura });
	points.push_back(topLeft + Vector2{ 0, -latura });

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


	float frecare = 0.99f;
	this->velocity  = this->velocity * frecare ;

	if ( this->velocity.GetSqrMagnitude() < 1 )
		this->velocity = Vector2{ 0,0 };
}

void Circle::onHitBall(Circle& other)
{
	Vector2 dir = other.center - this->center;
	float distance = dir.GetMagnitude();
	float overlap = 2 * radius - distance;

	if (overlap > 0) {
		// Separate balls so they don't overlap
		dir.Normalize();
		this->center -= dir * (overlap / 2);
		other.center += dir * (overlap / 2);

		// Compute normal and tangent vectors
		Vector2 normal = dir;
		Vector2 tangent{ normal.y * -1, normal.x };

		// Decompose velocities into normal and tangent components
		float v1n = this->velocity.Dot(normal);
		float v1t = this->velocity.Dot(tangent);
		float v2n = other.velocity.Dot(normal);
		float v2t = other.velocity.Dot(tangent);

		// Swap normal components for equal-mass elastic collision
		float v1nFinal = v2n;
		float v2nFinal = v1n;

		// Recombine
		this->velocity = tangent * v1t + normal * v1nFinal;
		other.velocity = tangent * v2t + normal * v2nFinal;

		// Optionally add restitution (less bouncy)
		float restitution = 0.98f; // 1.0 = perfectly elastic, <1 = loses energy
		this->velocity *= restitution;
		other.velocity *= restitution;
	}
}




void Circle::drawCircle(int VboId, int myMatrixLocation, glm::mat4 myMatrix, int textureLocation)
{
	std::vector<Vector2> points = this->getPoints();
	//points.push_back(center);
	
	//std::cout << this->textureId << " ";
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
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBufferData(GL_ARRAY_BUFFER, drawpoints.size() * sizeof(float), drawpoints.data(), GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLE_FAN, 0, drawpoints.size() / 4);

}

void Circle::collisionManta(Manta& manta)
{
	Vector2 a = manta.mainsegment1;
	Vector2 b = manta.mainsegment2;

	float distSq = center.DistanceToSegmentSqr(a, b);

	bool hit =( distSq <= radius * radius);
	
	if (hit)
	{
		if (a.y < 250 && a.x == b.x)
			this->velocity.x = -this->velocity.x;
		else if (a.x < 500 && a.y == b.y)
			this->velocity.y = -this->velocity.y;
		else if (a.y > 250 && a.x == b.x)
			this->velocity.x = -this->velocity.x;
		else if (a.x > 500 && a.y == b.y)
			this->velocity.y = -this->velocity.y;
	}
	manta.hit = hit;

}

