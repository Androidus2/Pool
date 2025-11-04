
#include <iostream>			
#include <windows.h>        
#include <stdlib.h>         
#include <stdio.h>
#include <GL/glew.h>        
#include <GL/freeglut.h>    
#include "loadShaders.h"	
#include "glm/glm.hpp"		
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Circle.h"
#include <vector>
#include <math.h>
#include "SOIL.h"			
#include "Manta.h"
#include "Tac.h"
using namespace std;

GLuint
VaoId,
VboId,
ColorBufferId,
ProgramId,
myMatrixLocation,
matrScaleLocation,
matrTranslLocation,
matrRotlLocation,
codColLocation, textureBufferId, textureLocation;
GLfloat
winWidth = 1000, winHeight = 500;
glm::mat4
myMatrix, resizeMatrix;


float xMin = 0.f, xMax = 1000.f, yMin = 0.f, yMax = 500.f;
bool moving = false, movingTac= false;

float radius = 20.f;
float speed = 50.f;
vector<Circle> bile;
int tacIndex;
glm::mat4 tacMatrix;

vector<Circle> gauri;
vector<Manta> mante;
int blackBallIndex = 4;
Tac tac;

vector<Circle> animationBackgroundHoles;
vector<Circle> animationBalls;
float xNegru = 700, yNegru = 250;
float hole_radius;
Vector2 initialWhiteBallPosition = { 250.f,250.f };

GLuint LoadTexture(const char* texturePath, GLuint& texture);

void CreateShaders(void)
{
	ProgramId = LoadShaders("example.vert", "example.frag");
	glUseProgram(ProgramId);
}


void drawLine(int VboId, int myMatrixLocation, glm::mat4 myMatrix)
{
	GLfloat points[] =
	{
		250.f, 0.f, 0.f, 1.f,
		250.f, 500.f, 0.f, 1.f, 
	};

	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glPointSize(10);
	glDrawArrays(GL_LINES, 0, 2);
};

void drawPoint(int VboId, int myMatrixLocation, glm::mat4 myMatrix)
{
	GLfloat points[] =
	{
		250.f, 250.f, 0.f, 1.f,
	};

	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glPointSize(25);
	glEnable(GL_POINT_SMOOTH);
	glDrawArrays(GL_POINTS, 0, 1);
};


bool gameover = false;
void drawGameOver(int VboId, int myMatrixLocation, glm::mat4 myMatrix)
{
	GLfloat points[] =
	{
		0.f, 500.f, 0.f, 1.f,
		1000.f, 500.f, 0.f, 1.f,
		1000.f, 0.f, 0.f, 1.f,
		0.f, 0.f, 0.f, 1.f,
	};
	GLuint gameOverText = LoadTexture("lose.png", gameOverText);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gameOverText);
	glUniform1i(textureLocation, 0);

	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}


bool win = false;
void drawWin(int VboId, int myMatrixLocation, glm::mat4 myMatrix)
{
	GLfloat points[] =
	{

		0.f, 500.f, 0.f, 1.f,
		1000.f, 500.f, 0.f, 1.f,
		1000.f, 0.f, 0.f, 1.f,
		0.f, 0.f, 0.f, 1.f,
	};
	GLuint winOverText = LoadTexture("win.png", winOverText);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, winOverText);
	glUniform1i(textureLocation, 0);

	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}


void AddBileInitiale()
{
	bile.clear();
	bile.push_back(Circle(initialWhiteBallPosition, radius, "16bun.png"));// alba

	bile.push_back(Circle({ xNegru - radius * 4, yNegru }, radius, "5bun.png")); // 5

	bile.push_back(Circle({ xNegru - radius * 2, yNegru + radius }, radius, "11bun.png")); // 11
	bile.push_back(Circle({ xNegru - radius * 2, yNegru - radius }, radius, "3bun.png")); // 3


	bile.push_back(Circle({ xNegru, yNegru}, radius, "8bun.png")); // 8
	bile.push_back(Circle({ xNegru, yNegru - radius*2}, radius, "2bun.png")); // 2
	bile.push_back(Circle({ xNegru, yNegru + radius*2}, radius, "10bun.png")); // 10

	bile.push_back(Circle({ xNegru + radius * 2, yNegru + radius }, radius, "9bun.png")); // 9
	bile.push_back(Circle({ xNegru + radius * 2, yNegru - radius }, radius, "7bun.png")); // 7
	bile.push_back(Circle({ xNegru + radius * 2, yNegru + 3*radius }, radius, "14bun.png")); // 14
	bile.push_back(Circle({ xNegru + radius * 2, yNegru -3* radius }, radius, "4bun.png")); // 4


	bile.push_back(Circle({ xNegru + radius * 4, yNegru  }, radius, "13bun.png")); // 13
	bile.push_back(Circle({ xNegru + radius * 4, yNegru +2 *radius }, radius, "12bun.png")); // 12
	bile.push_back(Circle({ xNegru + radius * 4, yNegru - 2 * radius }, radius, "6bun.png")); // 6
	bile.push_back(Circle({ xNegru + radius * 4, yNegru - 4 * radius }, radius, "15bun.png")); // 15
	bile.push_back(Circle({ xNegru + radius * 4, yNegru + 4 * radius }, radius, "1bun.png")); // 1


	


}

void AddGauriInitiale() {
		hole_radius = radius + radius / 4;
		float radius = hole_radius;
		gauri.push_back(Circle({ radius,radius }, hole_radius, "gaura.png")); 
		gauri.push_back(Circle({ winWidth / 2,radius }, hole_radius, "gaura.png"));
		gauri.push_back(Circle({ winWidth-radius ,radius}, hole_radius, "gaura.png"));
		gauri.push_back(Circle({ radius,winHeight-radius }, hole_radius, "gaura.png"));
		gauri.push_back(Circle({ winWidth / 2,winHeight -radius}, hole_radius, "gaura.png"));
		gauri.push_back(Circle({ winWidth -radius,winHeight -radius}, hole_radius, "gaura.png"));

}

void AddMante()
{
	float radius = hole_radius;	
	float incline = 40;
	float grosime = 2 * radius - 2.5;

	mante.push_back( Manta({2*radius, winHeight }, {winWidth/2-radius, winHeight }, { winWidth / 2 - radius -incline, winHeight -grosime}, {2*radius + incline, winHeight -grosime},
					{ winWidth / 2 - radius - incline, winHeight - grosime }, { 2 * radius + incline, winHeight - grosime } ));
	mante.push_back(Manta({ winWidth / 2 + radius, winHeight }, { winWidth - 2 * radius, winHeight }, { winWidth - 2 * radius - incline, winHeight - grosime }, { winWidth / 2 + radius + incline, winHeight - grosime },
		{ winWidth - 2 * radius - incline, winHeight - grosime }, { winWidth / 2 + radius + incline, winHeight - grosime }));
	mante.push_back(Manta({ 2 * radius, 0 }, { winWidth / 2 - radius, 0 }, { winWidth / 2 - radius - incline, grosime }, { 2 * radius + incline, grosime },
			{ winWidth / 2 - radius - incline, grosime }, { 2 * radius + incline, grosime }	));
	mante.push_back(Manta({ winWidth / 2 + radius, 0 }, { winWidth - 2 * radius, 0 }, { winWidth - 2 * radius - incline, grosime }, { winWidth / 2 + radius + incline, grosime },
	{ winWidth - 2 * radius - incline, grosime }, { winWidth / 2 + radius + incline, grosime }));

	mante.push_back(Manta({ 0, 2 * radius }, { 0, winHeight - 2 * radius }, { grosime, winHeight - 2 * radius - incline }, { grosime, 2 * radius + incline },
		{ grosime, winHeight - 2 * radius - incline }, { grosime, 2 * radius + incline } ));
	mante.push_back(Manta({ winWidth, 2 * radius }, { winWidth, winHeight - 2 * radius }, { winWidth - grosime, winHeight - 2 * radius - incline }, { winWidth - grosime, 2 * radius + incline },
		{ winWidth - grosime, winHeight - 2 * radius - incline }, { winWidth - grosime, 2 * radius + incline }));
}

void AddTac()
{
	tac.leftUppermostPoint = { 0, 250 };
	tac.length = 500;
	tac.incline = 13;
	tac.thickness = 35;
	tac.leftLowermostPoint = { tac.leftUppermostPoint.x, tac.leftUppermostPoint.y - tac.thickness };
	tac.rightUppermostPoint = { tac.leftUppermostPoint.x + tac.length, tac.leftUppermostPoint.y - tac.incline };
	tac.rightLowermostPoint = { tac.leftLowermostPoint.x + tac.length, tac.leftLowermostPoint.y + tac.incline };
	tac.speed = speed;
	tac.whiteBallCenter = initialWhiteBallPosition;
}

void AddAnimation() {
	animationBackgroundHoles.clear();
	animationBalls.clear();

	animationBackgroundHoles.push_back({ {110.f, 25.f}, 20.f, "gaura.png"});
	animationBackgroundHoles.push_back({ {160.f, 25.f}, 20.f, "gaura.png" });
	animationBackgroundHoles.push_back({ {210.f, 25.f}, 20.f, "gaura.png" });
	animationBackgroundHoles.push_back({ {260.f, 25.f}, 20.f, "gaura.png" });
	animationBackgroundHoles.push_back({ {310.f, 25.f}, 20.f, "gaura.png" });
	animationBackgroundHoles.push_back({ {360.f, 25.f}, 20.f, "gaura.png" });
	animationBackgroundHoles.push_back({ {410.f, 25.f}, 20.f, "gaura.png" });

	animationBackgroundHoles.push_back({ {590.f, 25.f}, 20.f, "gaura.png" });
	animationBackgroundHoles.push_back({ {640.f, 25.f}, 20.f, "gaura.png" });
	animationBackgroundHoles.push_back({ {690.f, 25.f}, 20.f, "gaura.png" });
	animationBackgroundHoles.push_back({ {740.f, 25.f}, 20.f, "gaura.png" });
	animationBackgroundHoles.push_back({ {790.f, 25.f}, 20.f, "gaura.png" });
	animationBackgroundHoles.push_back({ {840.f, 25.f}, 20.f, "gaura.png" });
	animationBackgroundHoles.push_back({ {890.f, 25.f}, 20.f, "gaura.png" });

	// 5 11 3 2 10 9 7 14 4 13 12 6 15 1
	animationBalls.push_back({ {310.f, 25.f}, 20.f, "5bun.png" });
	animationBalls.push_back({ {690.f, 25.f}, 20.f, "11bun.png" });
	animationBalls.push_back({ {210.f, 25.f}, 20.f, "3bun.png" });
	animationBalls.push_back({ {160.f, 25.f}, 20.f, "2bun.png" });
	animationBalls.push_back({ {640.f, 25.f}, 20.f, "10bun.png" });
	animationBalls.push_back({ {590.f, 25.f}, 20.f, "9bun.png" });
	animationBalls.push_back({ {410.f, 25.f}, 20.f, "7bun.png" });
	animationBalls.push_back({ {840.f, 25.f}, 20.f, "14bun.png" });
	animationBalls.push_back({ {260.f, 25.f}, 20.f, "4bun.png" });
	animationBalls.push_back({ {790.f, 25.f}, 20.f, "13bun.png" });
	animationBalls.push_back({ {740.f, 25.f}, 20.f, "12bun.png" });
	animationBalls.push_back({ {360.f, 25.f}, 20.f, "6bun.png" });
	animationBalls.push_back({ {890.f, 25.f}, 20.f, "15bun.png" });
	animationBalls.push_back({ {110.f, 25.f}, 20.f, "1bun.png" });


	for (auto& ball : animationBalls)
		ball.active = false;
}

void CreateVBO(void)
{
	GLfloat Vertices[] = {
		//	Varfuri pentru axe;
			100.0f, 0.0f, 0.0f, 1.0f,
	};

	GLfloat Colors[10] ;

	glGenVertexArrays(1, &VaoId);                                                   
	glBindVertexArray(VaoId);

	glGenBuffers(1, &VboId);                                                        
	glBindBuffer(GL_ARRAY_BUFFER, VboId);                                           
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);      
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &ColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	
	GLfloat TextureCoords[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};
	glGenBuffers(1, &textureBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, textureBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TextureCoords), TextureCoords, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}

void DestroyVBO(void)
{
	//  Eliberarea atributelor din shadere (pozitie, culoare, texturare etc.);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	//  Stergerea bufferelor pentru varfuri, culori;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &ColorBufferId);
	glDeleteBuffers(1, &VboId);

	//  Eliberaea obiectelor de tip VAO;
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);
}

void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}

void Initialize(void)
{
	glClearColor(0.0f, 0.588f, 0.196f, 1.0f);		
	CreateVBO();								
	CreateShaders();							
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	textureLocation = glGetUniformLocation(ProgramId, "myTexture");
	AddBileInitiale();
	AddGauriInitiale();
	AddMante();
	AddTac();
	AddAnimation();
}

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT);			
	
	resizeMatrix = glm::ortho(xMin, xMax, yMin, yMax);

	glUniform1i(glGetUniformLocation(ProgramId, "color"), 0);
	if (gameover)
	{
		glUniform1i(glGetUniformLocation(ProgramId, "withTexture"), 1);
		glUniform1i(glGetUniformLocation(ProgramId, "color"), 0);
		drawGameOver(VboId, myMatrixLocation, resizeMatrix);
		glutSwapBuffers();
		glFlush();
		return;
	}

	if (win)
	{
		glUniform1i(glGetUniformLocation(ProgramId, "withTexture"), 1);
		glUniform1i(glGetUniformLocation(ProgramId, "color"), 0);
		drawWin(VboId, myMatrixLocation, resizeMatrix);
		glutSwapBuffers();
		glFlush();
		return;
	}

	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &resizeMatrix[0][0]);
	glPointSize(5);


	glUniform1i(glGetUniformLocation(ProgramId, "color"), 1);
	drawLine(VboId, myMatrixLocation, resizeMatrix);
	drawPoint(VboId, myMatrixLocation, resizeMatrix);

	glUniform1i(glGetUniformLocation(ProgramId, "color"), 0);
	glUniform1i(glGetUniformLocation(ProgramId, "withTexture"), 0);
	for(Manta& manta:mante)
	{
		manta.drawManta(VboId, myMatrixLocation, resizeMatrix);
	}


	glUniform1i(glGetUniformLocation(ProgramId, "withTexture"), 1);
	for (auto gaura : gauri)
	{
		gaura.drawCircle(VboId, myMatrixLocation, resizeMatrix, textureLocation);
	}
	for (auto bila : bile)
	{
		if (bila.active)
			bila.drawCircle(VboId, myMatrixLocation, resizeMatrix, textureLocation);
	}

	glUniform1i(glGetUniformLocation(ProgramId, "withTexture"), 1);
	glUniform1i(glGetUniformLocation(ProgramId, "color"), 0);
	for (auto& animationHole : animationBackgroundHoles) {
		animationHole.drawCircle(VboId, myMatrixLocation, resizeMatrix, textureLocation);
	}
	for (auto& animationBall : animationBalls) {
		if (animationBall.active) {
			animationBall.animate(0.03f);
			animationBall.drawCircle(VboId, myMatrixLocation, resizeMatrix, textureLocation);
		}
	}

	glUniform1i(glGetUniformLocation(ProgramId, "withTexture"), 0);
	glUniform1i(glGetUniformLocation(ProgramId, "color"), 2);
	if (tac.visible)
	{
		tac.whiteBallCenter = bile[0].center;
		tac.moveToWhiteBall();
		tac.speed = speed;
		tac.drawTac(VboId, myMatrixLocation, resizeMatrix);
	}

	glutSwapBuffers();	
	glFlush();	
}

Vector2 transformScreenToWorld(int x, int y,
	float xMin, float xMax,
	float yMin, float yMax)
{
	int screenWidth = glutGet(GLUT_WINDOW_WIDTH);
	int screenHeight = glutGet(GLUT_WINDOW_HEIGHT);

	float nx = (float)x / (float)screenWidth;
	float ny = 1.0f - (float)y / (float)(screenHeight); 

	Vector2 world;
	world.x = xMin + nx * (xMax - xMin);
	world.y = yMin + ny * (yMax - yMin);

	return world;
}


void UseMouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
	{
		if (moving)
			break;
		Vector2 screenCoord = transformScreenToWorld(x, y,
			xMin, xMax,
			yMin, yMax);
		Vector2 dir = screenCoord - bile[0].center;
		dir.Normalize();
		dir = dir * speed;
		bile[0].velocity = dir;
		moving = true;
		movingTac = true;
		tacMatrix = tac.matTranslation * tac.matRotation;
		break;
	}
	default:
		break;
	}
}

void update(int value)
{
	if (movingTac)
	{
		if (speed == 0)
			movingTac = false;
		else {
			tac.offset -= speed * 40 / 100;
			tac.pointToWhiteBall(bile[0]);

			if (tac.offset < 15.0f)
			{
				movingTac = false;
			}
		}
	}
	else
	{
		tac.offset = 25.f + speed * 2.f;
		moving = false;
		for (auto& bila : bile)
			if (bila.active && bila.velocity.GetMagnitude() != 0)
			{
				moving = true;
			}
		if (bile[0].active == false && moving == false)
		{
			bile[0].center = Vector2{ 250.f,250.f };
			bile[0].velocity = Vector2{ 0,0 };
			bile[0].active = true;
		}

		for (auto& bila : bile)
		{
			if (bila.active)
				bila.moveCircle(xMin, xMax, yMin, yMax);
		}
		for (int i = 0; i < bile.size(); i++)
		{
			for (int j = i + 1; j < bile.size(); j++)
			{
				if (bile[i].active && bile[j].active && (bile[i].center - bile[j].center).GetMagnitude() < 2 * radius)
				{
					bile[i].onHitBall(bile[j]);
				}
			}

			for (auto& manta : mante)
			{
				if (bile[i].active)
					bile[i].collisionManta(manta);
			}

			for (auto& gaura : gauri)
			{
				if (bile[i].active && (bile[i].center - gaura.center).GetMagnitude() < (bile[i].radius + gaura.radius) / 2)
				{
					bile[i].active = false;
					if (i != 0 && i != 4) {
						int ind = i - 1;
						if (i > 4)
							ind--;
						animationBalls[ind].playAnimation();
					}
					if (i == blackBallIndex) 
					{
						gameover = false;
						for (int i = 0; i < bile.size(); i++)
						{
							if (i != 0 && bile[i].active)
							{
								gameover = true;
								break;
							}
						}
					}
				}
			}
		}

		win = true;
		for(int i=1;i<bile.size();i++)
			if(bile[i].active == true)
				win = false;

		if (win)
			win = bile[0].active;
		tac.pointToWhiteBall(bile[0]);

		if (moving == false)
			tac.visible = true;
		else
		{
			tac.visible = false;
		}

	}

	glutPostRedisplay();
	glutTimerFunc(16, update, 0);


}


void ProcessSpecialKeys(int key, int xx, int yy)
{
	switch (key)			
	{						
	case GLUT_KEY_UP:
		if (!movingTac) {
			speed += 1;
			speed = speed > 100 ? 100 : speed;
			cout << speed << " " << endl;
		}
		break;
	case GLUT_KEY_DOWN:
		if (!movingTac) {
			speed -= 1;
			speed = speed < 0 ? 0 : speed;
			cout << speed << " " << endl;
		}
		break;
	case GLUT_KEY_F4:
		moving = false;
		gameover = false;
		win = false;
		AddBileInitiale();
		AddAnimation();
	}
}

void MouseView(int x, int y)
{
	Vector2 worldPos = transformScreenToWorld(x, y,
		xMin, xMax,
		yMin, yMax);
	tac.mousePos = { worldPos.x, worldPos.y };
	tac.direction = (bile[0].center - tac.mousePos).GetNormalized();

}


int main(int argc, char* argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);					
	glutInitWindowSize(winWidth, winHeight);									
	glutCreateWindow("Biliard");	

	glewInit();

	Initialize();						
	glutDisplayFunc(RenderFunction);	
	glutCloseFunc(Cleanup);				
	glutMouseFunc(UseMouse);

	glutPassiveMotionFunc(MouseView);
	glutTimerFunc(16, update, 0);
	glutSpecialFunc(ProcessSpecialKeys);
	
	glutMainLoop();


	return 0;
}

