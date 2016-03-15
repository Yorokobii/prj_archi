#ifndef BALLES_HPP
#define BALLES_HPP

#include <list>
#include "balle.hpp"

class Balles {
private:
    std::list<Balle> balles;

public:
    inline Balles() {}
    inline ~Balles() {}
    void lancer(Vec3, Vec3, float =1.0f);
    void avancer(Objet& , float); // Faire avancer les balles + test fin de vie
    int tailleTab();
};

#endif
