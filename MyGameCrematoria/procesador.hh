/** @file procesador.hh
    @brief Especificació clase Procesador
*/

#ifndef PROCESADOR
#define PROCESADOR
#ifndef NO_DIAGRAM
#include <map>
#include <set>
#include <iostream>
#endif

#include "Proces.hh"

using namespace std;

/** @class Procesador
@brief Representa un processador

està compost per dos ints que ens indiquen la memòria lliure i la memòria total del processador també per 3 mapes on un actua com la memòria, per tant, la clau és la posició on comença el procés i el valor és el procés un altre que ens permet saber on estan els processos, per tant, la clau és els identificadors del procés i el valor és la posició de memòria a la qual comença i l'últim mapa ens indica els espais en buit que hi ha per això la clau és la mida del forat i el valor és un set ordenat per proximitat a l'inici de la memòria on hi ha les posicions d'inici dels forats de la mida de la clau

També tenim 2 mètodes privats que ens permeten calcular el forat a l'esquerra del procés i a la dreta per tal que si l'hem d'eliminar fusionar-ho amb un sol forat.

i 2 altres mètodes que ens permeten afegir i eliminar forats, ja que així ens evitem repetir fragments de codi
*/
class Procesador{
private:
/** @brief mètode privat per calcular si té un forat a l'esquerra
	Ens calcula si té un forat a l'esquerra i ens l'elimina del mapa de forats per tal de després fusionar amb la mida del procés eliminat i el forat dret si en té per unir-ho en un sol forat
	\pre it_act apunta a un procés que eliminarem
	\post El resultat es el forat a l'esquerra del procés al qual apunta it_act i aquest forat s'ha eliminat del mapa de forats
	\coste cost \link Eliminar_forat \endlink +log<sub>2</sub>(k) constant i logarítmic, ja que per trobar el forat és constant i per eliminar-lo crida la funció eliminar forat que té cost log<sub>2</sub>
*/
int forat_esquerra(const map<int,Proces>::iterator& it_act, int& pos_in);
/** @brief mètode privat per calcular si té un forat a la dreta
	Ens calcula si té un forat a la dreta i ens l'elimina del mapa de forats per tal de després fusionar amb la mida del procés eliminat i el forat esquerre si en té per unir-ho en un sol forat
	\pre it_act apunta a un procés que eliminarem
	\post El resultat és el forat a la dreta del procés al qual apunta it_act i aquest forat s'ha eliminat del mapa de forats
	\coste cost \link Eliminar_forat \endlink +log<sub>2</sub>(k) constant i logarítmic, ja que per trobar el forat és constant i per eliminar-lo crida la funció eliminar forat que té cost log<sub>2</sub>(k) on k és el nombre de forats de mida diferent
*/
int forat_dreta(const map<int,Proces>::iterator& it_act);
/** @brief mètode privat per Afegir un forat
Ens permet crear un forat de mida mida_forat i que tingui posició inicial a pos_in
\pre mida_forat i pos_in ens indiquen la mida i la posició inicial d'un forat que està al mapa de forats
\post Ens elimina el forat al qual fa referència del mapa de forats
\coste log<sub>2</sub>(k) té cost logarítmic on k és el nombre de forats diferents
*/
void Afegir_forat(const int& mida_forat, const int&pos_in);

/** @brief mètode privat per Eliminar un forat
Ens permet eliminar un forat de mida mida_forat i que tingui posició inicial a pos_inicial
\pre mida_forat i pos_in ens indiquen la mida i la posició inicial d'un forat que està al mapa de forats
\post Ens elimina el forat al qual fa referència del mapa de forats
\coste log<sub>2</sub>(k) té cost logarítmic on k és el nombre de forats diferents
*/

void Eliminar_forat(const int& mida_forat, const int& posicio_inicial);
/** @brief mem_lliure ens indica la memòria lliure restant del processador
*/
int mem_lliure;
/** @brief mem_total ens indica la memòria total del processador
*/
int mem_total;
/** @brief mapa on la clau és la posició de memòria on està cada procés i el valor és el procés
*/
map<int,Proces>mem;
/** @brief mapa ordenant per la clau id del procés i el valor és la posició de memòria on es troba
*/
map<int,int>On_es_proces;
/** @brief mapa ordenant per la clau que és la mida del forat i el valor és un set amb les posicions on comencen els forats de mida de la clau
*/
map<int,set<int>>Espais_buits;



public:
    //constructores
/** @brief Creadora per defecte
	\pre <em>cert</em>
	\post El resultat és un processador buit
	\coste constant
*/
    Procesador();
    


    //modificadores
    
    
/** @brief Llegidora d'un processador
	Ens llegeix les característiques d'un processador i modifica els elements de la classe per tal que tinguin les característiques descrites
	el mapa de forats té un forat de tota la memòria
	\pre hi ha preparats al canal estàndard d'entrada un enter N més gran que zero.
	\post El resultat és un processador amb N espais de memòria i el mapa de forats té un de mida n a la posició 0
	\coste constant
*/
    void llegir();
    
/** @brief Afegeix un procés
	@param procés és un procés que volem donar d'alta al nostre processador
	Ens afegeix un procés en cas que no hi càpiga ens mostra un missatge d'error
	\pre rebem un procés el qual no el tenim cap amb el mateix id
	\post El resultat és el processador amb el procés afegit a memòria
	\coste log<sub>2</sub>(k) on k és el nombre de forats de mida diferent
*/
    void Alta_Proces(const Proces& proces);

/** @brief Elimina Processos de la memòria

Si aquest procés no existeix ens retorna false en cas contrari s'elimina i en cas que aquest procés tingues forats al seu voltant es fusionen en un
@param id_proces es l'id d'un procés que volem eliminar en cas que existeixi
@return ens retorna true o false depenent de si s'ha eliminat o no el procés amb id_proces
\pre rebem un identificador d'un procés
\post El resultat és true si el procés està a la memòria i ens l'eliminat de la memòria en cas que no hi sigui ens retorna false;
\coste log<sub>2</sub>(k)+ cost \link forat_esquerra \endlink + cost \link forat_dreta \endlink + cost \link Afegir_forat \endlink té cost logarítmic, ja que per trobar el procés té cost logarítmic i per calcular el forat dret i esquera té cost de la seva respectives funcions que és constant on k és el nombre de processos
*/
    bool Baixa_Proces(const int& id_proces);

/** @brief Ens avança n unitats de temps de tots els processos si un procés acaba l'elimina
	@param temps és el temps que avancem l'execució de tots els processos del processador
	\pre rebem un enter més gran que 0
	\post El resultat és el processador on tots els processos han avençat n unitats de temps si un proces s'ha acabat d'executar és eliminat del processador
	\coste O*n+ p*(O+ log<sub>2</sub>(t)) té cost lineal més n log<sub>2</sub> on n és el nombre de processos del processador p el nombre de processos que s'eliminen i t el nombre
*/
    void modificar_temps_procesos(const int& temps);

/** @brief Ens compacta la memòria
	\pre el paràmetre implícit està inicialitzat
	\post El resultat és el processador amb la memòria compactada
	\coste lineal k+n on k és el nombre de forats diferents i n el nombre de processos després del primer forat
*/
    void redistribuir_mem();
    
    //escriptores
    
/** @brief Ens permet mostrar per pantalla tots els processos del processador
	\pre el paràmetre implícit està inicialitzat
	\post Ens imprimeix per pantalla tots els processos del processador per ordre que estan en la memòria
	\coste lineal
*/
    
    void imprimir_Procesador()const;

    //consultores

/** @brief Ens diu si existeix un procés amb id n
@param id_proces és l'identificador d'un procés que volem consultar si hi és o no
@return ens retorna true o false en funció de si existeix un procés amb id_proces
\pre rebem un identificador d'un procés
\post El resultat és si hi ha un procés amb aquell id
\coste log<sub>2</sub>(k) on k és el nombre de processos
*/
    bool existeix_Proces(const int id_proces)const;


/** @brief Ens permet saber si hi ha processos en el processador
@return ens retorna true si té processos o false en cas que no
\pre el paràmetre implícit està inicialitzat
\post El resultat és un booleà que ens indica si te processos en execució
\coste constant
*/

    bool te_procesos() const;
    
/** @brief Ens permet saber el forat més ajustat pel procés

	Ens modifica el paràmetre per referència forat per la mida del forat més ajustat per afegir un procés de mida_forat en cas que no càpiga ens retorna false
	@param forat és un int o guardarem la mida del forat més ajustat a mida_forat en cas que existeixi
	@param mida_forat és el mida del forat mínim que busquem en aquest processador
	@return ens retorna true o false en funció de si ha trobat un forat &ge; mida_forat
	\pre ens entra un forat el qual modificarem el seu valor u la mida del forat que estem
	\post El resultat és un booleà que ens indica si hi ha un forat de mida més gran o igual a mida_forat i en cas que sigui true forat val el valor d'aquest forat que ha trobat
	\coste log<sub>2</sub>(k) on k és el nombre de forats de mida diferent
    */
    
    bool forat(int& forat, const int &mida_forat)const;

    
    //operadors
	/** @brief Ens Permet fer comparacions més ràpides
	Ens permet comparar si un procés té més memòria lliure que un altre
	\pre hi ha a l'esquerra i a la dreta un processador el qual esta inicialitzat
	\post El resultat és un booleà que ens indica si el procés de l'esquerra té més memòria lliure que el de la dreta
	\coste const
	*/
    bool operator>(const Procesador& pro) const;
/** @brief Ens Permet fer comparacions més ràpides
	Ens permet comparar si un procés té la mateixa memòria lliure que un altre
	\pre hi ha a l'esquerra i a la dreta un processador el qual està inicialitzat
	\post El resultat és un booleà que ens indica si el procés de l'esquerra té igual memòria lliure que el de la dreta
	\coste const
*/
    
    bool operator==(const Procesador& pro) const;

};

#endif
