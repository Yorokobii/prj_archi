#include "balles.hpp"

void Balles::lancer(Vec3 _vecteurDeform, Vec3 _vitesseBalle, float _rayonDeform){
    Balle balle(_vecteurDeform, _vitesseBalle, _rayonDeform);
    balles.push_back(balle);
}

void Balles::avancer(Objet& objet, float zPlan, GLint locCDeform, GLint locVDeform, GLint locRDeform, GLint locVibration){
    std::list<Balle>::iterator it;

    it = balles.begin();
    while(it != balles.end()){
        if(it->avancer(objet, zPlan, locCDeform, locVDeform, locRDeform, locVibration))
            it = balles.erase(it);
        else it++;
    }

}

int Balles::tailleTab(){ return balles.size(); }
