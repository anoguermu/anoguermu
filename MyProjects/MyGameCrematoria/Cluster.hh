/** @file Cluster.hh
    @brief Especificació clase Cluster
*/

#ifndef CLUSTER
#define CLUSTER
#ifndef NO_DIAGRAM
#include "BinTree.hh"
#include <iostream>
#include <map>
#include <queue>
using namespace std;
#endif
#include "procesador.hh"




/** @class Cluster
@brief Representa un Clúster

Està format per un arbre binari on cada node té el valor d'un identificador de processador i un mapa on la clau és l'identificador del processador i el valor és la classe processador i un mètode privat que ens permet trobar el processador amb el forat més ajustat per tal d'enviar-li un procés
*/

class Cluster{
private:
/** @brief arbre on cada node té l'id de processador 
*/
	BinTree<string> arbre_cluster;
/** @brief mapa ordenat per la clau id del processador i el valor és el processador 
*/
	map<string,Procesador> map_cluster;
	
public:
    //constructores
	/** @brief Creadora per defecte.
		\pre cert
		\post El resultat és un clúster buit.
		\coste Constant
	*/
    Cluster();

    //modificadores

/** @brief Ens configura un clúster

	si ja existia un abans el canvia per un de nou.
	\pre hi ha preparats al canal d'entrada n processadors i les seves connexions
	\post El resultat és un clúster format pels n processadors
	\coste O*n lineal en funció de n on n és el nombre de processadors nous
*/

    void Config_Cluster();

/** @brief Ens permet modificar el clúster

Ens modifica el clúster en cas que el processador no existeixi o tingui processos o tingui processadors auxiliars no es modificarà i ens mostrarà un missatge d'error
@param id_processador és l'id d'un processador que pot estar o no en el mapa
\pre hi ha preparats al canal estàndard d'entrada un id de processador i un nou clúster
\post El resultat és un clúster modificat si p no existeix o té processos en execució o té processadors aux imprimeix un missatge d'error
\coste log<sub>2</sub>(k)+ O+ O*k és un mixt entre log i lineal, ja que per comprovar si existeix té cost log<sub>2</sub>(k) on k és el nombre de processadors i constant per tal de saber si té processos i lineal per tal de modificar el clúster
*/
    void modificar_Cluster(const string id_processador);

/** @brief Ens permet afegir un procés en el processador
	@param id_procesador és l'id d'un processador que pot estar o no en el mapa
	@param pro és un procés complet el qual volem donar d'alta al processador amb id_procesador

	En cas permet afegir un procés en el processador en cas que no existeixi el processador o ja hi hagi un procés amb el mateix id o no hi càpiga ens mostra un missatge d'error
	\pre ens entra un id_procesador que és de tipus string i un procés els quals hem llegit totes les seves dades al main
	\post El resultat és un clúster amb el procés afegit en el processador si no existeix o ja té un procés amb el mateix id imprimeix un missatge d'error
	@coste log<sub>2</sub>(k) + \ref Procesador::existeix_Proces "cost existeix_Proces" + \ref Procesador::Alta_Proces "cost Alta_Proces" té cost logarítmic on k és el nombre de processadors + el cost de les altres dues funcions.

*/

    void alta_proces_Procesador(const string& id_procesador,const Proces& pro);

/** @brief Ens permet eliminar un procés d'un processador
	@param id_procesador és l'id d'un processador que pot estar o no en el mapa
	@param id_proces és l'id d'un procés que pot estar o no al processador amb id_procesador en cas que existeixi.
	Ens elimina un procés d'un processador en cas que el procés o el processador no existeixi ens mostra un missatge d'error
	\pre ens entra un id de processador de tipus string i un id d'un procés de tipus int;
	\post El resultat és un clúster amb el procés eliminat del processador si no existeix el processador o no té el procés amb aquest id imprimeix un missatge d'error
	\coste log<sub>2</sub>(k) + cost \ref Procesador::Baixa_Proces "Baixa_Proces" on k és el nombre de processadors 
*/

    void baixa_proces_Procesador(const string& id_procesador, const int& id_proces);

/** @brief Modificadora de temps

	Ens permet modificar el temps de tots els processos de tots els processadors en cas que el temps d'un procés sigui 0 s'elimina
	@param t és el temps quan ha d'avençar l'execució de tots els processos de tots els processadors del clúster
	\pre ens entra un int t que és més gran que 0
	\post El resultat és un clúster amb el temps avançat de cada procés de cada processador si ja s'ha executat és eliminat
	\coste O*n+ \ref Procesador::modificar_temps_procesos "cost modificar_temps_procesos" lineal, ja que hem de recórrer tots els processos 
*/

    void modificar_temps(const int & t);

/** @brief Compacta la memòria de tots els processadors
\pre el paràmetre implícit està inicialitzat
\post El resultat és un clúster amb la memòria de cada processador compactada
\coste t*(k<sub>t</sub>+n<sub>t</sub>) on t és el nombre de processadors i k<sub>t</sub> és el nombre de forats diferents del processador t i nt el nombre de processos després del primer forat del processador t, ja que per trobar el processador té cost log<sub>2</sub> i per compactar la memòria té cost lineal
*/

    void compactar_mem();
    
	     /** @brief Ens permet compactar la memòria d'un processador
	@param id_procesador és l'id d'un processador que pot estar o no en el mapa
	Ens permet compactar la memòria del processador amb id id_procesador en cas que no existeixi ens mostra un missatge d'error
	\pre ens entra un identificador de processador de caràcter string
	\post El resultat és un clúster amb la memòria del processador amb identificador id compactada si existeix si no ens mostra un missatge d'error per pantalla
	\coste \ref Procesador::redistribuir_mem "cost redistribuir_mem"+ log<sub>2</sub>(t) on t és el nombre de processadors  ja que per trobar el processador té cost log<sub>2</sub> i per compactar la memòria té cost lineal
*/
    void compactar_mem_processador(const string& id_procesador);
    
     /** @brief Ens envia un procés al clúster
	@param proc és un procés complet que volem enviar al clúster
	@return ens retorna true si el procés s'ha afegit o false en cas contrari
	Ens intenta enviar un procés al clúster en el processador amb l'espai més ajustat en cas que hi hagi un empat es decideix pel que tingui més memòria lliure i en cas que hi hagi un altre empat es decideix pel més proper a l'arrel
	\pre ens entra un procés
	\post ens retorna true si el procés s'ha afegit al clúster o false en cas contrari
	\coste 2n+n*log<sub>2</sub>(k) té cost lineal + lineal nlog, ja que en cas que tingui que recorre l'arbre crida la funció \link in_m \endlink on n el nombre de processadors, ja que aquest és el cost en pitjor cas que te que recorre l'arbre però si féssim bfs per reduir cost tindríem un cost massa gran en memòria
*/
    
    bool enviar_proces_cluster(const Proces& proc);
    
    //escriptura
    
     /** @brief mostrar per pantalla tots els processos d'un processador

	Ens imprimeix per pantalla tots els processos del processador la posició de memòria on està i l'especificacio de cada procés en cas que el processador no existeixi ens mostra un missatge d'error
	@param id_procesador és un string que té l'identificador d'un processador que pot estar o no en el mapa
	\pre ens entra un identificador de processador de caràcter string
	\post Ens mostra per pantalla els processos per ordre de col·locació dins la memòria del processador amb identificador que hem llegit si existeix en cas contrari ens mostra un missatge d'error
	\coste O*log<sub>2</sub>(k) + O*n és un mixt entre logarítmic i lineal, ja que per trobar el processador té cost log<sub>2</sub>(k) on k és el nombre de processadors i lineal respecte on n és el nombre de processos dins del processador
*/
    
    void imprimir_procesador(const string& id_procesador)const;
    
/** @brief Ens permet mostrar per pantalla tots els processos de tots els processadors del clúster
	\pre el paràmetre implícit està inicialitzat
	\post Ens mostra per pantalla tots els processos de tots els processadors per ordre creixent d'identificador i dins de cada processador per ordre de col·locació dins de memòria.
	\coste cost quadràtic, ja que és lineal per recórrer tots els processadors i lineal per imprimir tots els processos de tots els processadors, ja que crida la funció \ref Procesador::imprimir_Procesador "imprimir_Procesador"
*/
    
    void imprimir_procesadors_cluster()const;
    
/** @brief Ens permet mostrar per pantalla l'estructura del clúster
	\pre el paràmetre implícit està inicialitzat
	\post ens mostra per pantalla l'estructura del clúster
	\coste lineal O*n on n és el nombre de processadors
*/
    
    void imprimir_estructura_cluster()const;

    private:
/** @brief Ens permet fer el desempat dels processos que tenen la mateixa mida i memòria lliure

	@param a es un arbre binari no buit
	@param prof és un int que ens indica la profunditat a la qual està el node que estem
	@param lis_proc_pos és un set amb els processadors que tenen el forat més ajustat i la mateixa memòria lliure
	@return ens retorna un pair amb el processador del set més a prop de l'arrel i la seva profunditat
	\pre hi ha un set amb més d'un id de processador que té un forat de la mateixa mida i la mateixa memori lliure
	\post Ens retorna el processador que està més a prop de l'arrel dels que estan en el set i en cas d'empat el de més a l'esquerra
	\coste n+n*log<sub>2</sub>(k) te un cost n log<sub>2</sub> i logarítmic on n és el nombre de processadors i k el nombre de strings del set 
    */
    static pair<int,string> in_m(const BinTree<string>& a, int prof, const set<string>& lis_proc_pos);
 /** @brief Ens permet imprimir l'arbre de processadors

	@param a es un arbre binari no buit

	\pre ens entra un arbre no buit
	\post Ens mostra per pantalla l'estructura de l'arbre
	\coste n té un cost lineal
*/
    static void imprimir(const BinTree<string>& a);
/** @brief Ens permet modificar el clúster

	Ens modifica el clúster en cas que el processador tingui processadors auxiliars ens retorna false i no el modifica

	@param id_procesador és l'identificador d'un processador que està en l'arbre
	@param a és un arbre binari no buit
	@param arbe_aux és un arbre binari de strings
	\pre ens entra un arbre no buit
	\post En cas que el processador amb id no tingui processadors auxiliars ens retorni true i ens ha modificat l'arbre en cas contrari ens retorna false
	\coste n té un cost lineal
*/
    static bool construir_nou(const string id_procesador,BinTree<string>& a,BinTree<string> arbre_aux);
/** @brief Operació de lectura d'un arbre de processadors
	@param arbre és un arbre que construirem de 0
	@param mapa és un mapa de strings i processador buit
	\pre el clúster i el mapa estan buits
	\post a conte un arbre de strings de processadors i mapa conte un mapa de string i processadors
	\coste n té un cost lineal
*/
    static void llegir(BinTree<string> &arbre, map<string,Procesador>& mapa);




};

#endif
