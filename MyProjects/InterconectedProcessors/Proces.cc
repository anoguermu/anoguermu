/**@file Proces.cc
   @brief Codi de la classe Proces
 */
#include "Proces.hh"

Proces::Proces(){
    id=0;
    temps=0;
    memoria_ocupada=0;
}

void Proces::llegir(){
    cin>>id>>memoria_ocupada>>temps;
}

void Proces::modificar_temps(const int temps){
    this-> temps-=temps;
}

int Proces::Consultar_id() const{
    return id;
}
int Proces::Consultar_temps() const{
    return temps;

}
int Proces::Consultar_MemOcupada() const{
    return memoria_ocupada;
}

bool Proces::operator<=(const int& x){
    return temps<=x;

}
