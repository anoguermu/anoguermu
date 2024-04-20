/**@file Prioritat.cc
   @brief Codide la classe Prioritat
 */

#include "Prioritat.hh"


Prioritat::Prioritat(){
	acceptats=0;
	no_acceptats=0;
	id_processos=set<int>();
	processos=list<Proces>();

}

bool Prioritat::afegir_proces(const Proces& proces){
	int id=proces.Consultar_id();
	if(id_processos.find(id)== id_processos.end()){
	
		processos.push_back(proces);
		id_processos.insert(id);
		return true;
	}
	return false;
}

void Prioritat::imprimir_prio()const{
	//invariant: s'han impres per pantalla tots els elements de la llista
	//	         it itera es a dir el seu valor esta entre processos.begin() i processos.end()
	//funcio de cota: es el nombre d'elements entre it i processos.end()
	for(list<Proces>::const_iterator it= processos.begin(); it!= processos.end(); ++it){
		cout<<(*it).Consultar_id()<<" "<<(*it).Consultar_MemOcupada()<<" "<<(*it).Consultar_temps()<<endl;
	}
	cout<<acceptats<<" "<<no_acceptats<<endl;
}


bool Prioritat::te_processos()const{
	return not processos.empty();

}

void Prioritat::enviar_processos(Cluster& cluster, int&n){
	list<Proces>::iterator it= processos.begin();
	int mida=processos.size();
	bool itf_modificat= false;
    //invariant: s'han intentat enviar tots els processos entre processos.begin() i it
    //          it itera es a dir el seu valor esta entre processos.begin() i it_final que es processos.end() en cas que no s'hagi rebutjat cap proces en cas contrari es el primer element rebutjat
    //          a la variable acceptats s ha incrementat en 1 per cada proces acceptat entre processos.begin() i it
    //          la variable no_acceptats s'ha incrementat en 1 per cada proces no acceptat entre processos.begin() i it
    //          funcio de cota: es el min entre n i els elements entre it i it_final
	while( n!= 0 and mida!=0){
		if(cluster.enviar_proces_cluster(*it)){
			++acceptats;
			--n;
			id_processos.erase((*it).Consultar_id());
		}else{
			++no_acceptats;
			processos.push_back(*it);
			if(not itf_modificat){
				itf_modificat=true;
			}
		}
		--mida;
		it=processos.erase(it);
	
	}

}
