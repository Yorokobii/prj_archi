#ifndef BALLE_HPP
#define BALLE_HPP

#include <GL/glew.h> /* /!\ Include avant gl.h /!\ */
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <iostream>
#include "objetsOFF.h"

typedef struct Vector3 {
    float x;
    float y;
    float z;
} Vec3;

class Balle {
private:
    Vec3 vecteurDeform;
    Vec3 vitesseBalle;
    float rayonDeform;
    Vec3 centreDeform;
    Vec3 PositionBalle;

    float tabCD[3];
    float tabVD[3];

public:
    Balle(Vec3, Vec3, float =1.0);
    inline ~Balle(){ }
    bool avancer(Objet& objet, float, GLint, GLint, GLint );
    Vec3 couleur;

    inline const Vec3& getVDeform() const { return vecteurDeform; }
    inline const Vec3& getVBalle() const { return vitesseBalle; }
    inline const float& getRDeform() const { return rayonDeform; }
    inline const Vec3& getCDeform() const { return centreDeform; }
    inline const Vec3& getPBalle() const { return PositionBalle; }
};

#endif
