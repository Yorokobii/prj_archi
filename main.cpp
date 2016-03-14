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

using namespace std;


#define ECHAP 27
GLSL_Program * mes_shaders;

Texture mRaffin;
Objet monObjet;

float angle_x = 0.0f;
float angle_y = 0.0f;

//variables pour la gestion des paramètres de la fenêtre
float windowRatio = 1.0f;
int windowHeight = 500;
int windowWidth = 500;
float angle = 0.0f;

void GeomInit(void) {
	unsigned int nfaces;

	//monObjet.charge_OFF((const char*) "100x100points.off");
	monObjet.charge_OFFUV((const char*) "100x100pointsUV.off");
	//monObjet.affiche();

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
	glEndList();

	glDisable(GL_TEXTURE_2D);
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
	angle += 0.001f;
	//angle_x += 0.01f;
	//angle_y -= 0.01f;
	glutPostRedisplay();
}

void RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0,0.0,0.0,1.0);

	//Modification de la matrice de projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //remise à 0 (identité)
	gluPerspective(90.0, windowRatio, 0.1, 500); //définition d'une perspective (angle d'ouverture 130°,rapport L/H=1.0, near=0.1, far=100)
    glTranslatef(0.0,0.0,-200.0);
	glRotatef(180,0.0,0.0,1.0);

	//Modification de la matrice de modélisation de la scène
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

	//cf le callback_clavier
	glRotatef(angle_x,1,0,0);
	glRotatef(angle_y,0,1,0);

	//Parce qu'on avait pas vu encore les dsiplay List...
	glCallList(monObjet.id);

	glutSwapBuffers();
}

void callback_Keyboard(unsigned char key, int x, int y) {
switch (key) {

	case 'z':
		angle_x++;
		break;
	case 's':
		angle_x--;
		break;

	case 'q':
		angle_y--;
		break;
	case 'd':
		angle_y++;
		break;

	case ECHAP:
		delete mes_shaders;
		cout << "callback_Keyboard - " << "sortie de la boucle de rendu" << endl;
		glutLeaveMainLoop(); //retour au main()
		break;

	default:
		cerr << "La touche " << int(key) << " est non active." << endl;
		cerr << "\t -> Z S Q D pour faire tourner l'objet" << endl;
		break;
	}
}

static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderScene);
	glutIdleFunc(callback_Idle);
	glutKeyboardFunc(callback_Keyboard);
	//glutMotionFunc(callback_Mouse);
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
	glutCreateWindow("TP2 deformation VS FS");

	//attachement des fonctions de l'OpenGL
	//toujours après l'initialisation de glut
	GlewInit();

	InitializeGlutCallbacks();
	InitializeGL();
	//SetShaders();

	GeomInit();


	glutMainLoop();

	delete mes_shaders;

	return (EXIT_SUCCESS);
}
