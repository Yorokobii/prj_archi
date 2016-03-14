#ifndef BALLES_HPP
#define BALLES_HPP

#include <list>
#include "balle.hpp"
#include "objetsOFF.h"

class Balles {
private:
    std::list<Balle> balles;

public:
    inline Balles() {}
    inline ~Balles() {}
    void lancer(Vec3, Vec3, unsigned int =1);
    void avancer(Objet& , float); // Faire avancer les balles + test fin de vie
};

#endif
