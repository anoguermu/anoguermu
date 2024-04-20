/**@file Area_Processos.cc
   @brief Codi de la classe Area_processos
 */
#include "Area_Processos.hh"

Area_processos::Area_processos(){
	int n; 
	cin>>n;
	area=map<string,Prioritat>();
	Prioritat prio;
	//invariant: s'han llegit i prioritats i s'han afegit al mapa area;
	//	     0<=i<n
	//funcio de cota:  n-i-1;
	for(int i=0; i<n;++i){
		string id;
		cin>>id;
		area[id]=prio;
	}

}
void Area_processos::enviar_procesos_cluster(Cluster &cluster,int&t){
	map<string,Prioritat>::iterator it = area.begin();
    //invariant s'han intentat enviar t processos al cluster de la prioritat que apunta it
    //          it itera es a dir el seu valor esta entre area.begin() i area.end()
    //funcio de cota: el min entre t i el nombre d'elements entre it i areal.end()
	while(t >0 and it != area.end()){
		it->second.enviar_processos(cluster,t);
		++it;
	}
}

void Area_processos::afegir_prio(const string& prioritat){
	map<string,Prioritat>::iterator it;
	it=area.find(prioritat);
	if(it==area.end()){
		Prioritat prio;
		area[prioritat]=prio;
	
	}else{
		cout<<"error: ya existe prioridad"<<endl;
	}
	

}


void Area_processos::eliminar_prio(const string& prioritat){
	map<string,Prioritat>::iterator it;
	it=area.find(prioritat);
	if(it==area.end()){
		cout<<"error: no existe prioridad"<<endl;
	
	}else{
		if(it->second.te_processos()){
			cout<<"error: prioridad con procesos"<<endl;
		
		}else{
			area.erase(it);
		}
	
	}

}

void Area_processos::afegir_proces(const string& prioritat, const Proces proces){
	map<string,Prioritat>::iterator it;
	it=area.find(prioritat);
	if(it!=area.end()){
		if(not it->second.afegir_proces(proces)){
			cout<<"error: ya existe proceso"<<endl;
		}
		
	}else{
		cout<<"error: no existe prioridad"<<endl;
	}

}


void Area_processos::imprimir_prio(const string& prioritat) const{
	map<string,Prioritat>::const_iterator it;
	it=area.find(prioritat);
	if(it==area.end()){
		cout<<"error: no existe prioridad"<<endl;
        }else{
		it->second.imprimir_prio();
	}

}

void Area_processos::imprimir_area_espera()const {
	//invariant: s'han impres per pantalla tots els elements entre area.begin() i it;
	//	     it itera es a dir esta entre area.begin() i area.end();
	//funcio de cota: es la quantitat d'elements entre it i area.end();
	for(map<string,Prioritat>::const_iterator it=area.begin(); it!=area.end(); ++it){
		cout<<it->first<<endl;
		it->second.imprimir_prio();
	}



}
