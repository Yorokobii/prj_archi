#include "balles.hpp"

void Balles::lancer(Vec3 _vecteurDeform, Vec3 _vitesseBalle, float _rayonDeform){
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

vContraintes* Balles::makeArray(){
    std::list<Balle>::iterator it;
    vContraintes* contraintes = new vContraintes[balles.size()];

    it = balles.begin();
    int cpt = 0;
    while(it != balles.end()){
        contraintes[cpt] = it->makeArray();
        it++;
        cpt++;
    }

    return contraintes;
}

int Balles::tailleTab(){
    std::list<Balle>::iterator it;

    return balles.size();
}
