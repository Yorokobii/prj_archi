#include <stdlib.h>
#include <time.h>
#include "balle.hpp"


Balle::Balle(Vec3 _vecteurDeform, Vec3 _vitesseBalle, unsigned int _rayonDeform)
    :vecteurDeform(_vecteurDeform), vitesseBalle(_vitesseBalle), rayonDeform(_rayonDeform)
{
    PositionBalle.x = 0.0;
    PositionBalle.y = 0.0;
    PositionBalle.z = 0.0;

    centreDeform.x = 0.0;
    centreDeform.y = 0.0;
    centreDeform.z = 0.0;

    couleur.x = (float(rand()%100))/100.0;
    couleur.y = (float(rand()%100))/100.0;
    couleur.z = (float(rand()%100))/100.0;

}

bool Balle::avancer(float zPlan){

    PositionBalle.x -= vitesseBalle.x;
    PositionBalle.y -= vitesseBalle.y;
    PositionBalle.z -= vitesseBalle.z;

    centreDeform.x -= vitesseBalle.x;
    centreDeform.y -= vitesseBalle.y;
    centreDeform.z -= vitesseBalle.z;

    glPushMatrix();
        glDisable(GL_TEXTURE_2D);
            glColor3f(couleur.x, couleur.y, couleur.z);
            glTranslatef(PositionBalle.x, PositionBalle.y, PositionBalle.z);
            glutSolidSphere(rayonDeform, 20, 20);
        glEnable(GL_TEXTURE_2D);
    glPopMatrix();

    if(PositionBalle.z <= zPlan*2) return true; else return false;
}