//
// ================================================
// | Grafica pe calculator                        |
// ================================================
// | Laboratorul III - 03_02_animatie.cpp |
// ============================================
//
//	Program ce deseneaza un dreptunghi ce se deplaseaza stanga-dreapta si are un patrat ce orbiteaza in jurul sau, folosindu-se tehnicile MODERN OpenGL;
//	ELEMENTE DE NOUTATE:
//	- utilizeaza diverse transformari si compunerea acestora folosind biblioteca glm,
//	- functii pentru utilizarea mouse-ului;
//
//
//	Biblioteci

#include <iostream>			//	Biblioteca necesara pentru afisarea in consola;
#include <windows.h>        //	Utilizarea functiilor de sistem Windows (crearea de ferestre, manipularea fisierelor si directoarelor);
#include <stdlib.h>         //  Biblioteci necesare pentru citirea shaderelor;
#include <stdio.h>
#include <GL/glew.h>        //  Define?te prototipurile functiilor OpenGL si constantele necesare pentru programarea OpenGL moderna; 
#include <GL/freeglut.h>    //	Include functii pentru: 
							//	- gestionarea ferestrelor si evenimentelor de tastatura si mouse, 
							//  - desenarea de primitive grafice precum dreptunghiuri, cercuri sau linii, 
							//  - crearea de meniuri si submeniuri;
#include "loadShaders.h"	//	Fisierul care face legatura intre program si shadere;
#include "glm/glm.hpp"		//	Bibloteci utilizate pentru transformari grafice;
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Circle.h"
#include <vector>
#include <math.h>
#include "SOIL.h"			//	Biblioteca pentru texturare;
#include "Manta.h"
using namespace std;

//  Identificatorii obiectelor de tip OpenGL;
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
//	Dimensiunile ferestrei de afisare;
GLfloat
winWidth = 1000, winHeight = 500;
//	Variabile catre matricile de transformare;
glm::mat4
myMatrix, resizeMatrix;

glm::vec4 ecran = glm::vec4{ 1.0f, 1.0f, 1.0f, 0.0f };

//	Variabile pentru proiectia ortogonala;
float xMin = 0.f, xMax = 1000.f, yMin = 0.f, yMax = 500.f;
bool moving = false;

float radius = 20.f;
float speed = 50.f;
vector<Circle> bile;


vector<Circle> gauri;
vector<Manta> mante;


//  Crearea si compilarea obiectelor de tip shader;
//	Trebuie sa fie in acelasi director cu proiectul actual;
//  Shaderul de varfuri / Vertex shader - afecteaza geometria scenei;
//  Shaderul de fragment / Fragment shader - afecteaza culoarea pixelilor;
void CreateShaders(void)
{
	ProgramId = LoadShaders("example.vert", "example.frag");
	glUseProgram(ProgramId);
}


float xNegru = 700, yNegru = 250;

void AddBileInitiale()
{
	bile.clear();
	bile.push_back(Circle({ 250.f,250.f }, radius, "16bun.png"));// alba

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
float hole_radius;
void AddGauriInitiale() {
		hole_radius = radius + radius / 4;
		float radius = hole_radius;
		gauri.push_back(Circle({ radius,radius }, hole_radius, "gaura.png")); // st sus
		gauri.push_back(Circle({ winWidth / 2,radius }, hole_radius, "gaura.png"));//
		gauri.push_back(Circle({ winWidth-radius ,radius}, hole_radius, "gaura.png"));
		gauri.push_back(Circle({ radius,winHeight-radius }, hole_radius, "gaura.png"));
		gauri.push_back(Circle({ winWidth / 2,winHeight -radius}, hole_radius, "gaura.png"));
		gauri.push_back(Circle({ winWidth -radius,winHeight -radius}, hole_radius, "gaura.png"));

}

void AddMante()
{
	float radius = hole_radius;	float incline = 40;
	float grosime = 2*radius - 2.5;

	mante.push_back( Manta({2*radius, yMax}, {winWidth/2-radius, yMax}, { winWidth / 2 - radius -incline, yMax -grosime}, {2*radius + incline, yMax-grosime},
					{ winWidth / 2 - radius - incline, yMax - grosime }, { 2 * radius + incline, yMax - grosime } ));
	mante.push_back(Manta({ winWidth / 2 + radius, yMax }, { winWidth - 2 * radius, yMax }, { winWidth - 2 * radius - incline, yMax - grosime }, { winWidth / 2 + radius + incline, yMax - grosime },
		{ winWidth - 2 * radius - incline, yMax - grosime }, { winWidth / 2 + radius + incline, yMax - grosime }));
	mante.push_back(Manta({ 2 * radius, 0 }, { winWidth / 2 - radius, 0 }, { winWidth / 2 - radius - incline, grosime }, { 2 * radius + incline, grosime },
			{ winWidth / 2 - radius - incline, grosime }, { 2 * radius + incline, grosime }	));
	mante.push_back(Manta({ winWidth / 2 + radius, 0 }, { winWidth - 2 * radius, 0 }, { winWidth - 2 * radius - incline, grosime }, { winWidth / 2 + radius + incline, grosime },
	{ winWidth - 2 * radius - incline, grosime }, { winWidth / 2 + radius + incline, grosime }));

	mante.push_back(Manta({ 0, 2 * radius }, { 0, winHeight - 2 * radius }, { grosime, winHeight - 2 * radius - incline }, { grosime, 2 * radius + incline },
		{ grosime, winHeight - 2 * radius - incline }, { grosime, 2 * radius + incline } ));
	mante.push_back(Manta({ winWidth, 2 * radius }, { winWidth, winHeight - 2 * radius }, { winWidth - grosime, winHeight - 2 * radius - incline }, { winWidth - grosime, 2 * radius + incline },
		{ winWidth - grosime, winHeight - 2 * radius - incline }, { winWidth - grosime, 2 * radius + incline }));


}

//  Se initializeaza un Vertex Buffer Object (VBO) pentru tranferul datelor spre memoria placii grafice (spre shadere);
//  In acesta se stocheaza date despre varfuri (coordonate, culori, indici, texturare etc.);
void CreateVBO(void)
{
	//  Coordonatele varfurilor;
	GLfloat Vertices[] = {
		//	Varfuri pentru axe;
			100.0f, 0.0f, 0.0f, 1.0f,
	};

	//  Culorile axelor;
	GLfloat Colors[10] ;

	//  Transmiterea datelor prin buffere;

	//  Se creeaza / se leaga un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO;
	glGenVertexArrays(1, &VaoId);                                                   //  Generarea VAO si indexarea acestuia catre variabila VaoId;
	glBindVertexArray(VaoId);

	//  Se creeaza un buffer pentru VARFURI;
	glGenBuffers(1, &VboId);                                                        //  Generarea bufferului si indexarea acestuia catre variabila VboId;
	glBindBuffer(GL_ARRAY_BUFFER, VboId);                                           //  Setarea tipului de buffer - atributele varfurilor;
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);      //  Punctele sunt "copiate" in bufferul curent;
	//  Se asociaza atributul (0 = coordonate) pentru shader;
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//  Se creeaza un buffer pentru CULOARE;
	glGenBuffers(1, &ColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	//  Se asociaza atributul (1 =  culoare) pentru shader;
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

//  Elimina obiectele de tip shader dupa rulare;
void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}

//  Eliminarea obiectelor de tip VBO dupa rulare;
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

//  Functia de eliberare a resurselor alocate de program;
void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}

//  Setarea parametrilor necesari pentru fereastra de vizualizare;
void Initialize(void)
{


	glClearColor(0.0f, 0.588f, 0.196f, 1.0f);		//  Culoarea de fond a ecranului;
	CreateVBO();								//  Trecerea datelor de randare spre bufferul folosit de shadere;
	CreateShaders();							//  Initilizarea shaderelor;
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	textureLocation = glGetUniformLocation(ProgramId, "myTexture");
	AddBileInitiale();
	AddGauriInitiale();
	AddMante();
}

//  Functia de desenarea a graficii pe ecran;
void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT);			//  Se curata ecranul OpenGL pentru a fi desenat noul continut;
	
	resizeMatrix = glm::ortho(xMin, xMax, yMin, yMax);
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &resizeMatrix[0][0]);
	glPointSize(5);

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

	glutSwapBuffers();	//	Inlocuieste imaginea deseneata in fereastra cu cea randata; 
	glFlush();	//  Asigura rularea tuturor comenzilor OpenGL apelate anterior;
}

Vector2 transformScreenToWorld(int x, int y,
	float xMin, float xMax,
	float yMin, float yMax)
{
	int screenWidth = glutGet(GLUT_WINDOW_WIDTH);
	int screenHeight = glutGet(GLUT_WINDOW_HEIGHT);

	// Normalize to [0,1]
	float nx = static_cast<float>(x) / static_cast<float>(screenWidth);
	float ny = 1.0f - static_cast<float>(y) / static_cast<float>(screenHeight); // flip Y

	// Map to world coords
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
		break;
	}
	case GLUT_RIGHT_BUTTON:			//	CLICK dreapta => dreptunghiurile se misca spre drepta;
		break;
	default:
		break;
	}
}

void update(int value)
{
	moving = false;
	for (auto& bila:bile)
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
	
	for(auto &bila:bile)
	{
		if ( bila.active )
			bila.moveCircle(xMin, xMax, yMin, yMax);
	}

	for(int i = 0;i<bile.size();i++)
	{
		for (int j = i+1; j < bile.size(); j++)
		{
			if ( bile[i].active && bile[j].active  && (bile[i].center - bile[j].center).GetMagnitude() < 2 * radius )
			{
				bile[i].onHitBall(bile[j]);
			}
		}
	}
	

	for (auto &bila:bile)
		for (auto& manta : mante)
		{
			if (bila.active)
				bila.collisionManta(manta);
		}

	for (int i = 0; i < bile.size(); i++)
	{
		for (auto& gaura : gauri)
		{
			if ((bile[i].center - gaura.center).GetMagnitude() < (bile[i].radius + gaura.radius)/2)
			{
				bile[i].active = false;
			}
		}
	}



	// aici se pot face animatii
	glutPostRedisplay();
	glutTimerFunc(16, update, 0);

}


void ProcessSpecialKeys(int key, int xx, int yy)
{
	switch (key)			//	Procesarea tastelor 'LEFT', 'RIGHT', 'UP', 'DOWN'
	{						//	duce la deplasarea patratului pe axele Ox si Oy;
	case GLUT_KEY_UP:
		speed += 1;
		speed = speed > 100 ? 100 : speed;
		cout << speed << " " << endl;
		break;
	case GLUT_KEY_DOWN:
		speed -= 1;
		speed = speed < 0 ? 0 : speed;
		cout << speed << " " << endl;
		break;
	case GLUT_KEY_F4:
		moving = false;
		AddBileInitiale();
	}
}


int main(int argc, char* argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);					//	Se folosesc 2 buffere (unul pentru afisare si unul pentru randare => animatii cursive) si culori RGB;
	glutInitWindowPosition(100, 100);								//  Pozitia initiala a ferestrei;
	glutInitWindowSize(winWidth, winHeight);									//  Dimensiunea ferestrei;
	glutCreateWindow("Dreptunghi cu satelit - OpenGL <<nou>>");		//	Creeaza fereastra de vizualizare, indicand numele acesteia;

	//	Se initializeaza GLEW si se verifica suportul de extensii OpenGL modern disponibile pe sistemul gazda;
	//  Trebuie initializat inainte de desenare;

	glewInit();

	Initialize();						//  Setarea parametrilor necesari pentru fereastra de vizualizare; 
	glutDisplayFunc(RenderFunction);	//  Desenarea scenei in fereastra;
	glutCloseFunc(Cleanup);				//  Eliberarea resurselor alocate de program;
	glutMouseFunc(UseMouse);
	glutTimerFunc(16, update, 0);
	glutSpecialFunc(ProcessSpecialKeys);
	glutMainLoop();

	return 0;
}

