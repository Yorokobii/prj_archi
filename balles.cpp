#include "balles.hpp"

void Balles::lancer(Vec3 _vecteurDeform, Vec3 _vitesseBalle, unsigned int _rayonDeform){
    Balle balle(_vecteurDeform, _vitesseBalle, _rayonDeform);
    balles.push_back(balle);
}

void Balles::avancer(Objet& objet, float zPlan){
    std::list<Balle>::iterator it;

    it = balles.begin();
    while(it != balles.end()){
        if(it->avancer(zPlan)) it = balles.erase(it);
        else it++;
    }
}
