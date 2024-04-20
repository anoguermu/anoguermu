/** @file Prioritat.hh
    @brief Especificació clase Prioritat
*/

#ifndef PRIORITAT_HH
#define PRIORITAT_HH

#include "Proces.hh"
#include "Cluster.hh"

#ifndef NO_DIAGRAM
#include <iostream>
#include <set>
#include <list>
#endif

using namespace std;

/** @class Prioritat
@brief Representa una prioritat de l'area d'espera.

Està composta per 2 ints que ens indiquen la quantitat de processos acceptats i rebutjats d'aquesta prioritat també un set amb tots els id dels processos que estan sota aquesta prioritat i una llista on es guarden aquest procés per orde d'antiguitat;
*/

class Prioritat{

private:
/** @brief ens permet saber el nombre de processos acceptats al clúster sota aquesta prioritat
*/
int acceptats;
/** @brief ens permet saber el nombre de processos no acceptats al clúster sota aquesta prioritat
*/
int no_acceptats;
/** @brief ens permet consultar de forma més ràpida si hi ha un procés amb un cert id
*/
set<int> id_processos;
/** @brief ens guarda els processos sota aquesta prioritat per ordre d'antiguitat
*/
list<Proces> processos;

public:
//constructores 
/** @brief Creadora per defecte.
	\pre cert
	\post El resultat és un procés buit.
	\coste Constant
*/

   Prioritat();
// modificadores

/** @brief Ens afegeix processos sota aquesta prioritat.

	S'afegirà si no hi ha cap procés amb el mateix i ens retornarà true en cas contrari ens retornarà false i no s'afegirà
	@param procés és un procés que volem afegir sota aquesta prioritat en cas que no n'hi hagi cap altre amb el mateix id
	@return ens retorna true si s'ha afegit el procés sota aquesta prioritat false en cas contrari
	\pre Ens entra un procés inicialitzat i llegit prèviament
	\post s'ha afegit un procés sota l'identificador de prioritat, si ja hi havia un procés amb el mateix id no s'afegeix i ens retorna false.
	\coste O+log2(k), ja que per mirar si existeix és cost log i per afegir és constant on O és el cost de pc i n el nombre de processos de la prioritat
*/
   bool afegir_proces(const Proces& proces);
   
/** @brief Envia processos al clúster

Ens intenta enviar n processos al clúster per cada acceptat s'ha incrementat en 1 el nombre d'acceptats i per cada rebutjat s'incrementa en 1 el nombre de rebutjats
	@param clúster és el clúster on hi ha tots els processadors
	@param n és el nombre de processos que volem enviar al clúster
	\pre ens entra un n>=0
	\post Ens elimina els n processos enviats d'aquesta prioritat al clúster si un procés no es pot enviar torna a entrar com a nou a l'àrea si aquesta prioritat no ha pogut enviar n processos ens retorna n-el nombre de processos enviats per aquesta prioritat.
	\coste té cost n*(\ref Cluster::enviar_proces_cluster "cost enviar_proces_cluster") on n és el nombre de processos que intenta enviar al clúster
*/
   void enviar_processos(Cluster& cluster, int&n);
   
   //escriptores
/** @brief Imprimeix la prioritat

ens mostra per pantalla tots els processos d'aquesta prioritat i les seves especificacions

	\pre el paràmetre implícit està inicialitzat
	\post Ens imprimeix per pantalla els processos pendents de la prioritat que hem llegit en ordre decreixent d'antiguitat
	\coste lineal O*n on n és el nombre de processos
*/
   void imprimir_prio()const;
	//consultores

/** @brief Consultora de si la prioritat té processos

Ens permet saber si una prioritat té processos adjunts
	@return ens retorna si la prioritat té processos o no
	\pre el paràmetre implícit està inicialitzat
	\post Ens retorna si la prioritat té algun procés
	\coste constant
*/
	bool te_processos()const;

};


#endif

