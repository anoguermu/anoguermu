/** @file Area_Processos.hh
    @brief Especificació clase Area_processos
*/

#ifndef AREA_PROCESSOS
#define AREA_PROCESSOS
#ifndef NO_DIAGRAM

#include <iostream>
#include <map>
#endif

#include "Prioritat.hh"


using namespace std;

/** @class Area_processos
	@brief Representa l'àrea de processos pendents

	Està compost per un mapa on la clau és l'identificador de la prioritat i el valor és la classe prioritat
*/

class Area_processos{
private:

  map<string,Prioritat> area;


public:
    // Constructores
/** @brief Creadora d'una àrea de processos amb N prioritats.
	\pre hi ha preparats al canal estàndard d'entrada un enter N més gran que zero i N identificadors de prioritat que són de tipus string.
	\post El resultat és una Àrea de processos buida amb N prioritats.
	\coste lineal en funció de O*n on n és el nombre de prioritats que volem afegir
*/
    Area_processos();

    //modificadores

/** @brief Ens afegeix una prioritat

	Ens permet afegir una prioritat en cas que no n'hi hagi cap amb el mateix id en cas contrari ens mostra un missatge d'error.

	@param prioritat és un id de prioritat la qual volem donar d'alta a l'àrea d'espera en cas que no n'hi hagi cap amb el mateix id
	\pre ens entra un identificador de prioritat de tipus String.
	\post s'afegeix la nova prioritat a l'àrea de processos si no existia en cas contrari ens mostra un error.
	\coste O+log<sub>2</sub>(n) és logarítmic i log<sub>2</sub>, ja que per mirar si ja existeix una prioritat té cost log<sub>2</sub> i per afegir és constant
*/
    void afegir_prio(const string& prioritat);

/** @brief Ens permet eliminar prioritats.

Ens permet eliminar la prioritat en cas que no tingués processos en cas contrari ens mostri un missatge d'error
	@param prioritat és un string que fa referència a l'id d'una prioritat i la qual volem eliminar en cas que no tingui processos i existeixi
	\pre ens entra un identificador de prioritat de tipus String.
	\post s'ha eliminat la prioritat en cas que no existís o que tingués processos associats ens mostra un error.
	\coste O+log<sub>2</sub>(n) te cost logarítmic, ja que per mirar si existeix la prio a eliminar te cost log<sub>2</sub> i eliminar-la te cost constant on n és el nombre de prioritats
*/

    void eliminar_prio(const string& prioritat);

/** @brief Ens permet afegir processos a l'àrea de processos sota una prioritat.

	Ens afegeix processos sota una prioritat en cas que aquesta prioritat existeixi o no tingui cap procés amb el mateix id en cas contrari ens mostra un error
	@param prioritat és l'id d'una prioritat la qual volem afegir el procés en cas que existeixi
	@param proces és un procés que volem afegir sota la prioritat amb id prioritat
	\pre ens entra un procés amb totes les seves dades ja llegides i un identificador de prioritat de tipus String.
	\post s'ha afegit un procés sota l'identificador de prioritat, si no existeix la prioritat o ja hi havia un procés amb el mateix id i prioritat ens mostra un error.
	\coste log<sub>2</sub>(n) + \ref Prioritat::afegir_proces "cost afegir_proces" és un mix entre log i const, ja que hem de fer un find(cost log<sub>2</sub>) per si existeix la prio i li hem de sumar el cost d'afegil a la classe prio que es cost constant + log<sub>2</sub> on n és el nombre de prioritats i k el de processos sota aquesta prioritat
*/

    void afegir_proces(const string& prioritat, const Proces proces);

/** @brief Ens permet enviar processos al clúster

	ens intenta enviar t processos al clúster en cas que ja hagi intentat enviar tots els processos del clúster i encara quedin per arribar a t processos ja no intenta enviar més
	@param cluster és el clúster on hi ha tots els processadors
	@param int t és el nombre de processos que volem enviar al clúster
	\pre ens entra un enter t més gran que 0 i el clúster al qual enviarem els t processos
	\post Ens elimina els n processos enviats al clúster si un procés no es pot enviar torna a entrar com a nou a l'àrea amb la mateixa prioritat
	\coste té cost n*(\ref Prioritat::enviar_processos "cost enviar_processos") on n s'on les prioritats necessàries per enviar t processos la qual està entre 1- i el nombre de prioritats
*/
    

    void enviar_procesos_cluster(Cluster &cluster,int&t);
    
    
     //escriptores
/** @brief mostra per pantalla els processos d'una prioritat

Ens mostra tots els processos de la prioritat en cas que no existeixi la prioritat ens mostra un missatge d'error
	
@param prioritat és l'id d'una prioritat la qual volem mostrar per pantalla en cas que existeixi
	
\pre ens entra un identificador de prioritat de tipus String.
\post Ens imprimeix per pantalla els processos pendents de la prioritat que hem llegit en ordre decreixent d'antiguitat si aquesta prioritat existeix si no ens mostra un missatge d'error

\coste  O*log<sub>2</sub>(n)+ O*k és un mix entre log<sub>2</sub> i lineal, ja que per mirar si existeix la prio té cost log<sub>2</sub>(n) on n és el nombre de prioritats i ens ha d'imprimir k el nombre de processos sota la prioritat
*/
    
    void imprimir_prio(const string& prioritat) const;
    
   
    
/** @brief Ens mostra per pantalla l'àrea d'espera.

	Ens mostra per pantalla totes les prioritats i els processos sota cada prioritat
	\pre el paràmetre implícit està inicialitzat
	\post Ens imprimeix per pantalla els processos pendents de totes les prioritats en ordre creixent de prioritat i dins d'una prioritat es mostren en ordre decreixent d'antiguitat.
	\coste quadràtic, ja que per recórrer les prioritats és lineal i a cada prioritat s'ha de recórrer tots els processos d'aquella prioritat, ja que usa la funció \ref Prioritat::imprimir_prio "imprimir_prio" la classe prioritat que també té cost lineal
*/
    	
    void imprimir_area_espera() const;


};
#endif
