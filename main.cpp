#include <GL/glew.h> /* /!\ Include avant gl.h /!\ */
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <iostream>
//Fonction GLSL
#include "glsl_fs.h"
#include "glsl_vs.h"
#include "glsl_program.h"
//Autres
#include "fonctions.h"
#include "objetsOFF.h"
#include "texture.hpp"
#include "balles.hpp"

using namespace std;

#define ECHAP 27
GLSL_Program* basic_shader;
GLSL_Program* raffin_shader;
GLSL_Program* night_shader;

GLint locCDeform;
GLint locVDeform;
GLint locRDeform;

Texture mRaffin;
Objet monObjet;
Balles balles;

int mouseX = 0;
int mouseY = 0;

float zPlan = -200.0;
float Ratio = 0.0003f;

float angle_x = 0.0f;
float angle_y = 0.0f;

bool nightModeON = true;
float cpt = 0.0; // Compteur de nombre de balles envoyées

//variables pour la gestion des paramètres de la fenêtre
float windowRatio = 1.0f;
int windowHeight = 500;
int windowWidth = 500;
float angle = 0.0f;

GLint locDep;
float VecDep[2] = {0.0, 0.0};
float depX = 0.1f;
float depY = 0.2f;


void Deplacement(){

	if(monObjet.max.x + (monObjet.max.x - monObjet.min.x)/2>windowWidth/2-1)
		depX = -depX;
	if(monObjet.min.x - (monObjet.max.x - monObjet.min.x)/2<-(windowWidth/2-1))
		depX = -depX;

	if(monObjet.max.y + (monObjet.max.y - monObjet.min.y)/2>windowHeight/2-1)
		depY = -depY;
	if(monObjet.min.y - (monObjet.max.y - monObjet.min.y)/2<-(windowHeight/2-1))
		depY = -depY;

	monObjet.min.x -= depX;
	monObjet.min.y -= depY;

	monObjet.max.x -= depX;
	monObjet.max.y -= depY;

	VecDep[0] += depX;
	VecDep[1] += depY;

	glUniform2fv(locDep, 1, VecDep);
}

Vec3 GetMouseVec(int x, int y){
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject( winX, winY, winZ, modelview, projection,viewport, &posX, &posY, &posZ);

	Vec3 vec = {(float)posX, -(float)posY, (float)(posZ + 200)};

	vec.x *= 0.003;
	vec.y *= 0.003; // régule la vitesse
	vec.z *= 0.003;

	return vec;
}

void GeomInit(void) {
	srand(time(NULL));
	unsigned int nfaces;

	monObjet.charge_OFFUV((const char*) "100x100pointsUV.off");

	monObjet.id=glGenLists(1);

	glNewList(monObjet.id, GL_COMPILE);
	    glEnable(GL_TEXTURE_2D);
		mRaffin.charger((const char *)"texture.jpg");
		mRaffin.definir_filtrage(GL_LINEAR, GL_LINEAR);
		mRaffin.definir_bouclage(GL_CLAMP, GL_CLAMP);
		mRaffin.definir_melange(GL_DECAL);
		mRaffin.utiliser();

		glBegin(GL_TRIANGLES);
			for (nfaces = 0; nfaces<monObjet.nbfaces; nfaces++) {
				glTexCoord2f(monObjet.lpoints[monObjet.lfaces[nfaces].S1].s, monObjet.lpoints[monObjet.lfaces[nfaces].S1].t);
				glVertex3f(monObjet.lpoints[monObjet.lfaces[nfaces].S1].x, monObjet.lpoints[monObjet.lfaces[nfaces].S1].y, monObjet.lpoints[monObjet.lfaces[nfaces].S1].z);
	            glTexCoord2f(monObjet.lpoints[monObjet.lfaces[nfaces].S2].s, monObjet.lpoints[monObjet.lfaces[nfaces].S2].t);
				glVertex3f(monObjet.lpoints[monObjet.lfaces[nfaces].S2].x, monObjet.lpoints[monObjet.lfaces[nfaces].S2].y, monObjet.lpoints[monObjet.lfaces[nfaces].S2].z);
	            glTexCoord2f(monObjet.lpoints[monObjet.lfaces[nfaces].S3].s, monObjet.lpoints[monObjet.lfaces[nfaces].S3].t);
				glVertex3f(monObjet.lpoints[monObjet.lfaces[nfaces].S3].x, monObjet.lpoints[monObjet.lfaces[nfaces].S3].y, monObjet.lpoints[monObjet.lfaces[nfaces].S3].z);
			}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	glEndList();

}

static GLvoid callback_Window(GLsizei width, GLsizei height)
{
	//au cas où h soit nul et entraîne une division par 0
	if(height == 0)
		height = 1;

	glViewport(0, 0, width, height);

	windowWidth = width;
	windowHeight = height;

	windowRatio = float(windowWidth) / float(windowHeight);

	cout << "callback_Window - " << "new width " << windowWidth << " new height " << windowHeight << endl;
}

void callback_Idle() {
	glutPostRedisplay();
}

void RenderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0,0.0,0.0,1.0);

	//Modification de la matrice de projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //remise à 0 (identité)

	gluPerspective(90.0, windowRatio, 0.1, 206); //définition d'une perspective (angle d'ouverture 130°,rapport L/H=1.0, near=0.1, far=100)

	//night_shader->Activate();
	//glColor3f(1.0, 1.0, 1.0);

	if(nightModeON) raffin_shader->Deactivate();
	else night_shader->Deactivate();

	balles.avancer(monObjet, zPlan, locCDeform, locVDeform, locRDeform);

	if(nightModeON) raffin_shader->Activate();
	else night_shader->Activate();

    glTranslatef(0.0,0.0,zPlan);
	glRotatef(180,0.0,0.0,1.0);

	//Modification de la matrice de modélisation de la scène
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

	//cf le callback_clavier
	glRotatef(angle_x,1,0,0);
	glRotatef(angle_y,0,1,0);

	Deplacement();

	//Parce qu'on avait pas vu encore les dsiplay List...
	glCallList(monObjet.id);


	glutSwapBuffers();
}

void callback_Keyboard(unsigned char key, int x, int y) {
switch (key) {

	case ECHAP:
		delete basic_shader;
		cout << "callback_Keyboard - " << "sortie de la boucle de rendu" << endl;
		glutLeaveMainLoop(); //retour au main()
		break;

	case 'n':
		if(nightModeON) nightModeON = false;
		else nightModeON = true;
	break;

	default:
		cerr << "La touche " << int(key) << " est non active." << endl;
		cerr << "\t -> Z S Q D pour faire tourner l'objet" << endl;
		break;
	}
}

GLvoid callback_Mouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		mouseX = x; mouseY = y;

		Vec3 vecDef = {0.0, 0.0, -1.0};
		Vec3 vecBalle = GetMouseVec(x,y);
		balles.lancer(vecDef, vecBalle);
		cpt++;

		float aim = 0.0f;
		//if( balles.cpt != 0)
		 aim = balles.cpt / cpt ;
		std::cerr <<"Precision : "<< aim*100 <<" %"<<std::endl;
	}
}

void SetShaders(void) {
	// Déclaration :
	// Vertex :
	GLSL_VS basic_vert;
	GLSL_VS raffin_vert;
	GLSL_VS night_vert;

	// Fragment :
	GLSL_FS basic_frag;
	GLSL_FS raffin_frag;
	GLSL_FS night_frag;

	//____
	// Lecture des fichiers
	basic_vert.ReadSource("basic.vert");
	basic_frag.ReadSource("basic.frag");
	raffin_vert.ReadSource("raffin.vert");
	raffin_frag.ReadSource("raffin.frag");
	night_vert.ReadSource("raffin.vert");
	night_frag.ReadSource("nuit.frag");

	// Compilation :
	basic_vert.Compile();
	basic_frag.Compile();
	raffin_vert.Compile();
	raffin_frag.Compile();
	night_vert.Compile();
	night_frag.Compile();

	// Info Shader :
	PrintShaderInfo(basic_vert.idvs);
	PrintShaderInfo(basic_frag.idfs);
	PrintShaderInfo(raffin_vert.idvs);
	PrintShaderInfo(raffin_frag.idfs);
	PrintShaderInfo(night_vert.idvs);
	PrintShaderInfo(night_frag.idfs);

	//_____
	// Utilisation :
	basic_shader = new GLSL_Program();
	basic_shader -> Use_VertexShader(basic_vert);
	basic_shader -> Use_FragmentShader(basic_frag);
	basic_shader -> Link_Shaders();
	basic_shader -> Activate(); // Activation de ce basic_shader.

	raffin_shader = new GLSL_Program();
	raffin_shader -> Use_VertexShader(raffin_vert);
	raffin_shader -> Use_FragmentShader(raffin_frag);
	raffin_shader -> Link_Shaders();

	night_shader = new GLSL_Program();
	night_shader -> Use_VertexShader(night_vert);
	night_shader -> Use_FragmentShader(night_frag);
	night_shader -> Link_Shaders();

	//____
	// Définition des variables à viser dans le GPU
	locDep = glGetUniformLocation(raffin_shader->idprogram, "VecDeplac");
	locCDeform = glGetUniformLocation(raffin_shader->idprogram, "vecCDeform");
	locVDeform = glGetUniformLocation(raffin_shader->idprogram, "vecVDeform");
	locRDeform = glGetUniformLocation(raffin_shader->idprogram, "rayonDeform");

	//___
	// Info Program :
	PrintProgramInfo(raffin_shader -> idprogram);
	PrintProgramInfo(basic_shader -> idprogram);
}


static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderScene);
	glutIdleFunc(callback_Idle);
	glutKeyboardFunc(callback_Keyboard);
	glutMouseFunc(callback_Mouse);
	glutReshapeFunc(callback_Window);
	//glutSpecialFunc(&callback_SpecialKeys);

}

void InitializeGL() {

	GLfloat lightPosition [] = { 0.0f, 10.0f, 0.0f, 0.0 };

	//Crée une source de lumière directionnelle
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//Définit un modèle d'ombrage
	glShadeModel(GL_SMOOTH);

	//prise en compte des faces avant et arrières
	//glEnable(GL_CULL_FACE);

	//Z Buffer pour la suppression des parties cachées
	glEnable(GL_DEPTH_TEST);

	//La variable d'état de couleur GL_AMBIENT_AND_DIFFUSE
	//est définie par des appels à glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

}

void GlewInit() {
GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		cerr << "Erreur: " << glewGetErrorString(err) << endl;
	}
	cerr << "initGLEW_ok, ref GLEW " << glewGetString(GLEW_VERSION) << endl;

	if (glewIsSupported("GL_VERSION_2_0"))
		cerr << "ok OpenGL 2.0\n";
	else {
		cerr << "OpenGL 2.0 impossible\n";
		exit(EXIT_FAILURE);
	}

	cout << "OpenGL Vendor: " << (char*) glGetString(GL_VENDOR) << "\n";
	cout << "OpenGL Renderer: " << (char*) glGetString(GL_RENDERER) << "\n";
	cout << "OpenGL Version: " << (char*) glGetString(GL_VERSION) << "\n";
      //cout << "OpenGL Extensions:\n" << (char*) glGetString(GL_EXTENSIONS) << "\n";

}

int main(int argc, char **argv) {

	//on crée le contexte OpenGL et la fenêtre
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);

	glutInitWindowPosition(100,100);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("RaffinSplash");

	//attachement des fonctions de l'OpenGL
	//toujours après l'initialisation de glut
	GlewInit();

	InitializeGlutCallbacks();
	InitializeGL();
	SetShaders();

	GeomInit();


	glutMainLoop();

	delete basic_shader;

	return (EXIT_SUCCESS);
}
