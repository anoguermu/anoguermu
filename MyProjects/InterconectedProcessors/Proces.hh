/** @file Proces.hh
    @brief Especificació clase Proces
*/

#ifndef PROCES_CL
#define PROCES_CL
#ifndef NO_DIAGRAM

#include <iostream>
#endif

using namespace std;

/** @class Proces
    @brief Representa un proces informatic.

Està compost pel temps d'execució, la memòria que requereix i el temps necessària d'execució tots són variables de tipus int
*/

class Proces {

private:
/** @brief és l'id del procés
*/
    int id;
/** @brief és el temps que li falta per acabar l'execució
*/
    int temps;
/** @brief és la memoria que ocupa el procés
*/
    int memoria_ocupada;

public:
    // Constructores

    /** @brief Creadora per defecte.
        \pre <em>cert</em>
        \post El resultat és un procés buit.
        \coste Constant
    */
    Proces();

    //Modificadores

    /** @brief Modifica característiques del procés
\pre hi ha preparats al canal estàndard d'entrada 2 enters més grans que 0 negatiu i un més grani o igual a 0.
\post El paràmetre implícit passa a tenir els atributs llegits del canal estàndard d'entrada.
\coste Constant
*/
	
    void llegir();

    /** @brief Redueix el temps del Proces
    
        ja que aquesta funcio es cridada cuan el procesador executa un proces
        
        @param Temps és un int més gran que 0 que ens indica el temps que ha avançat l'execució del procés
\pre rebem un enter >0 i més petit que el temps del paràmetre implícit
\post Es modifica el temps del paràmetre implícit.
\coste Constant
*/
    void modificar_temps(const int temps);

    //consultores

/** @brief Consultora de l'id del procés.

	@return ens retorna l'id del procés
	\pre El paràmetre implícit té identificador.
	\post El resultat és l'identificador del procés.
	\coste Constant
*/

    int Consultar_id() const;

/** @brief consultora del temps del procés.

	@return ens retorna el temps que falta perquè s acabi d'executar el procés
	\pre El paràmetre implícit té temps.
	\post El resultat és el temps necessari d'execució del procés.
	\coste Constant
*/

    int Consultar_temps() const;

/** @brief Consultora de la memòria ocupada

	@return Ens retorna la quantitat de memòria lliure necessària pel procés.
	\pre El paràmetre implícit te memoria_ocupada.
	\post El resultat és la quantitat de memòria necessària per executar el procés.
	\coste Constant
*/

    int Consultar_MemOcupada() const;

    //operadors

/** @brief Operador <= de la classe processador

	@return Ens retorna el booleà resultant de comparar si el temps del procés ≤ que un int x.
	\pre El paràmetre implícit té temps i a l esquerra de ≤ hi ha un int x.
	\post El resultat és el booleà resultant de comparar si el temps del procés ≤ que un int x.
	\coste Constant
*/
    bool operator<=(const int& x);


};
#endif
