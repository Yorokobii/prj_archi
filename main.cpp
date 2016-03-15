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
GLSL_Program* mes_shaders;

GLint loc;
GLint tailleTab;
GLint locCDeform;

Texture mRaffin;
Objet monObjet;
Balles balles;

int mouseX = 0;
int mouseY = 0;

float zPlan = -200.0;
float Ratio = 0.0003f;

float angle_x = 0.0f;
float angle_y = 0.0f;

//variables pour la gestion des paramètres de la fenêtre
float windowRatio = 1.0f;
int windowHeight = 500;
int windowWidth = 500;
float angle = 0.0f;

void GeomInit(void) {
	srand(time(NULL));
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
	angle += 0.001f;
	glutPostRedisplay();
}

void RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0,0.0,0.0,1.0);

	//Modification de la matrice de projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //remise à 0 (identité)
	gluPerspective(90.0, windowRatio, 0.1, 500); //définition d'une perspective (angle d'ouverture 130°,rapport L/H=1.0, near=0.1, far=100)

	balles.avancer(monObjet, zPlan);
    glTranslatef(0.0,0.0,zPlan);
	glRotatef(180,0.0,0.0,1.0);

	//Modification de la matrice de modélisation de la scène
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

	//cf le callback_clavier
	glRotatef(angle_x,1,0,0);
	glRotatef(angle_y,0,1,0);

	//std::cerr << balles.tailleTab() << std::endl;

	//transmission des valeurs de la structure Contrainte
	/*glUniform3fv(where_centre, 1, maContrainte.centre);
	glUniform3fv(where_vecteur, 1, maContrainte.vecteur);
	glUniform1f(where_rayon, maContrainte.rayon);*/
	/*contr = balles.makeArray();
	glUniform1f(tailleTab, balles.tailleTab());
	glUniform3fv(loc, balles.tailleTab(), contr->vecD);
	glUniform3fv(loc, balles.tailleTab(), contr->vecC); */

	//Parce qu'on avait pas vu encore les dsiplay List...
	glCallList(monObjet.id);

	glutSwapBuffers();
}

void callback_Keyboard(unsigned char key, int x, int y) {
switch (key) {

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

GLvoid callback_Mouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		mouseX = x; mouseY = y;

		float xf = x - windowWidth/2;
		float yf = y - windowHeight/2;
		float zf = - zPlan * Ratio;
		xf *= Ratio;
		yf *= Ratio;

		Vec3 vecDef = {0.0, 0.0, 0.0};
		Vec3 vitBalle = {-xf, yf, zf};

		balles.lancer(vecDef, vitBalle);
	}
}



void SetShaders(void) {
	GLSL_VS le_vertex_shader;
	GLSL_FS le_fragment_shader;

	le_vertex_shader.ReadSource("vert.vert");
	le_vertex_shader.Compile();

	le_fragment_shader.ReadSource("frag.frag");
	le_fragment_shader.Compile();

	PrintShaderInfo(le_vertex_shader.idvs);
	PrintShaderInfo(le_fragment_shader.idfs);
	mes_shaders = new GLSL_Program();

	mes_shaders -> Use_VertexShader(le_vertex_shader);
	mes_shaders -> Use_FragmentShader(le_fragment_shader);

	mes_shaders -> Link_Shaders();
	mes_shaders -> Activate();

	//on aura besoin de localiser ces données dans la mémoire de la carte graphique
	//(pour pouvoir les modifier)
	loc = glGetUniformLocation(mes_shaders -> idprogram, "vcontraintes");
	tailleTab = glGetUniformLocation(mes_shaders -> idprogram, "lenght");
	/*where_vecteur = glGetUniformLocation(mes_shaders -> idprogram, "Uv3vecteur_deformation");
	where_rayon = glGetUniformLocation(mes_shaders -> idprogram, "Ufrayon_deformation");*/

	PrintProgramInfo(mes_shaders -> idprogram);

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
	//SetShaders();

	GeomInit();


	glutMainLoop();

	delete mes_shaders;

	return (EXIT_SUCCESS);
}
