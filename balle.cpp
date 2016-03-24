#include <stdlib.h>
#include <time.h>
#include "balle.hpp"


Balle::Balle(Vec3 _vecteurDeform, Vec3 _vitesseBalle, float _rayonDeform)
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

bool Balle::avancer(Objet& objet, float zPlan, GLint locCDeform, GLint locVDeform, GLint locRDeform, float& compteur){
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
            glutSolidSphere(rayonDeform, 50, 50);
        glEnable(GL_TEXTURE_2D);
    glPopMatrix();

    if(PositionBalle.z <= (zPlan + rayonDeform)){
        if( PositionBalle.x >= (objet.min.x)-rayonDeform && PositionBalle.x <= (objet.max.x)+rayonDeform ){
            if( PositionBalle.y >= (objet.min.y)-rayonDeform && PositionBalle.y <= (objet.max.y)+rayonDeform ){
                compteur++;
                //std::cerr<<"Mr. Raffin est touche : "<< compteur <<std::endl;

                tabCD[0] = centreDeform.x;
                tabCD[1] = centreDeform.y;
                tabCD[2] = centreDeform.z;

                tabVD[0] = vecteurDeform.x;
                tabVD[1] = vecteurDeform.y;
                tabVD[2] = vecteurDeform.z;

            	glUniform3fv(locCDeform, 1, tabCD);
            	glUniform3fv(locVDeform, 1, tabVD);
                glUniform1f(locRDeform, rayonDeform);
            }
            else{
                tabCD[0] = 0.0;
                tabCD[1] = 0.0;
                tabCD[2] = 0.0;

                tabVD[0] = 0.0;
                tabVD[1] = 0.0;
                tabVD[2] = 0.0;

                glUniform3fv(locCDeform, 1, tabCD);
                glUniform3fv(locVDeform, 1, tabVD);
                glUniform1f(locRDeform, rayonDeform);
            }
        }


        return true;
    }


    if(PositionBalle.z <= zPlan*3/2) return true;
    else return false;
}
