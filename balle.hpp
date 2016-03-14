#ifndef BALLE_HPP
#define BALLE_HPP

#include <GL/glew.h> /* /!\ Include avant gl.h /!\ */
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <iostream>

typedef struct Vector3 {
    float x;
    float y;
    float z;
} Vec3;

class Balle {
private:
    Vec3 vecteurDeform;
    Vec3 vitesseBalle;
    unsigned int rayonDeform;
    Vec3 centreDeform;
    Vec3 PositionBalle;

public:
    Balle(Vec3, Vec3, unsigned int =1);
    inline ~Balle(){ }
    bool avancer(float);
    Vec3 couleur;

};

#endif
