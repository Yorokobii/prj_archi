#include "balles.hpp"

void Balles::lancer(Vec3 _vecteurDeform, Vec3 _vitesseBalle, float _rayonDeform){
    Balle balle(_vecteurDeform, _vitesseBalle, _rayonDeform);
    balles.push_back(balle);
}

void Balles::avancer(Objet& objet, float zPlan){
    std::list<Balle>::iterator it;

    it = balles.begin();
    while(it != balles.end()){
        if(it->avancer(objet, zPlan)) it = balles.erase(it);


        else it++;
    }
}

int Balles::tailleTab(){
    std::list<Balle>::iterator it;

    return balles.size();
}
