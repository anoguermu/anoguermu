/**@file Cluster.cc
   @brief Codi de la classe Cluster
 */
#include "Cluster.hh"

Cluster::Cluster(){
	arbre_cluster=BinTree<string>();
	map_cluster=map<string,Procesador>();
	
	

}
//ens entra un mapa i un arbre
//ens retorna el nou arbre i mapa llegits amb els nous procesadors
void Cluster::llegir(BinTree<string> &arbre, map<string,Procesador>& mapa){
	string id;
	cin>>id;
	if(id !="*"){
		Procesador pro;
		pro.llegir();
		mapa[id]=pro;
		BinTree<string> esquerra;
		BinTree<string> dreta;
		llegir(esquerra,mapa);
		llegir(dreta,mapa);
		arbre=BinTree<string>(id,esquerra,dreta);
		
	}

}

 void Cluster::Config_Cluster(){
 	map_cluster.clear();
 	llegir(arbre_cluster,map_cluster);
 }





void Cluster::alta_proces_Procesador(const string& id_procesador,const Proces& pro){
	map<string,Procesador>::iterator it;
	it=map_cluster.find(id_procesador);
	if(it!=map_cluster.end()){
		if(it->second.existeix_Proces(pro.Consultar_id())){
			cout<<"error: ya existe proceso"<<endl;
		}else{
			it->second.Alta_Proces(pro);
		}
	
	}else{
		cout<<"error: no existe procesador"<<endl;
	
	}


}


void Cluster::baixa_proces_Procesador(const string& id_procesador, const int& id_proces){
	map<string,Procesador>::iterator it;
	it=map_cluster.find(id_procesador);
	if(it!=map_cluster.end()){
		if(not it->second.Baixa_Proces(id_proces)){
			cout<<"error: no existe proceso"<<endl; 	
		}
	
	}else{
		cout<<"error: no existe procesador"<<endl;
	
	}
}

void Cluster::modificar_temps(const int & t){
	// invariant s'ha modificat al temps de tots els processos de tots els procesadors entre map_cluster.begin() i it;
	//	     it itera es a dir esta entre map_cluster.begin() i map_cluster.end();
	//funcio de cota: son la cuantitat d'elements entre it i map_cluster.end();
	for(map<string,Procesador>::iterator it=map_cluster.begin(); it!=map_cluster.end(); ++it){
		it->second.modificar_temps_procesos(t);
	}



}



void Cluster::imprimir_procesador(const string& id_procesador)const{
	map<string,Procesador>::const_iterator it;
	it=map_cluster.find(id_procesador);
	if(it!=map_cluster.end()){
		it->second.imprimir_Procesador();
	
	}else{
		cout<<"error: no existe procesador"<<endl;
	}

}

void Cluster::imprimir_procesadors_cluster()const{
	//invariant: s'han impres per pantalla el nom dels processadors i dels processos de cada procecador entre map_cluster.begin() i it;
	//	     it itera es a dir esta entre map_cluster.begin() i map_cluster.end();
	//funcio de cota: la cuantitat d'elements entre it i map_cluster.end();
	for(map<string,Procesador>::const_iterator it=map_cluster.begin(); it!=map_cluster.end(); ++it){
		cout<<it->first<<endl;
		it->second.imprimir_Procesador();
	}


}
 void Cluster::compactar_mem_processador(const string& id_procesador){
 	map<string,Procesador>::iterator it = map_cluster.find(id_procesador);
 	if(it != map_cluster.end()){
 		(it->second).redistribuir_mem();
 	
 	}else{
 		cout<<"error: no existe procesador"<<endl;
 	
 	}
 }
 void Cluster::compactar_mem(){
 	//invariant: s'ha redistribuit la memoria de tots els procesadors entre map_cluster.begin() i it;
 	//	    it itera es a dir esta entra map_cluster.begin() i map_cluster.end();
 	//funcio de cota: la cuantitat d'elements entre it i map_cluster.end();
 	for(map<string,Procesador>::iterator it= map_cluster.begin(); it != map_cluster.end(); ++it){
 		(it->second).redistribuir_mem();
 	}
 
 
 }
 
 
 
 // pre ens entren l arbre del parametre implicit un id d'un processador al qual volem afegir al nou arbre en el lloc del procesador
 //amb l id_procesador en el cas que no tingui procesadors auxiliars i no tingui processos
 //post en ha modificat el cluster en cas que el processador amb id tingui processos en execucio o no existeixi o no sigui una fulla ens mostra un misatge d'error
 bool Cluster::construir_nou(const string id_procesador,BinTree<string>& a,BinTree<string> arbre_aux){
 	if(not a.empty()){
 		BinTree<string> esquerra= a.left();
 		BinTree<string> dreta= a.right();
 		if(a.value()== id_procesador){
 			if(esquerra.empty() and dreta.empty()){
 				a=arbre_aux;
 				return true;
 			}else return false;
 		}
 		
		bool e=construir_nou(id_procesador,esquerra,arbre_aux);
		bool d=construir_nou(id_procesador,dreta,arbre_aux);
		string id= a.value();
		a=BinTree<string>(id,esquerra,dreta);
		return e and d;

 	
 	}
 	return true; 
 
 }
 
 void Cluster::modificar_Cluster(const string id_processador){
 	map<string,Procesador> map_aux;
 	BinTree<string>arbre_aux;
 	llegir(arbre_aux, map_aux);
 	map<string,Procesador>::iterator it = map_cluster.find(id_processador);
 	if(it!= map_cluster.end()){
 		if(not (it->second).te_procesos()){
 	
		 	if(not construir_nou(id_processador,arbre_cluster, arbre_aux)) cout<<"error: procesador con auxiliares"<<endl;
		 	else{
		 		map_cluster.erase(it);
		 	 	map_cluster.insert(map_aux.begin(), map_aux.end());
		 	 }
		 
		}else cout<<"error: procesador con procesos"<<endl;

	}else cout<<"error: no existe procesador"<<endl;
 
 }
 
//pre a es un arbre 
//post ens mostra per pantalla la estructura del arbre
//cas base l'arbre esta  buit ens imprimeix un espai en blanc
//cas recursius l'arbre no esta buir ens imprimeix  
//la funcio de cota es el tamany ja que al pasar a.left() i a.right() el temany va decreixent
void Cluster::imprimir(const BinTree<string>& a){
	if(not a.empty()){
		cout<<"("<<a.value();
		imprimir(a.left());
		imprimir(a.right());
		cout<<")";
	
	}else{
		cout<<" ";
	}

}

void Cluster::imprimir_estructura_cluster()const{
	imprimir(arbre_cluster);
	cout<<endl;


}





pair<int,string> Cluster::in_m(const BinTree<string>& a, int prof, const set<string>& lis_proc_pos){
	if(not a.empty()){
		set<string>::const_iterator it = lis_proc_pos.find(a.value());
		if(it!= lis_proc_pos.end()){
			pair<int,string>b;
			b.first=prof;
			b.second=*(it);
			return b;
		}else{
			
			pair<int,string>Esquerra=in_m(a.left(), prof+1,lis_proc_pos);
			pair<int,string>Dreta=in_m(a.right(), prof+1,lis_proc_pos);
			//fem la comperació dels pair dreta i esquerra per determinar quin processador dels que hem rebut
			//esta a menor profunditat o esta mes a l'esquerra
			if(Esquerra.second == "" and Dreta.second=="")return Esquerra;
			else if(Esquerra.second != "" and Dreta.second != ""){
				if(Dreta.first<Esquerra.first)return Dreta;
				else return Esquerra;
			
			}else if(Dreta.second != "") return Dreta;
			else return Esquerra;
		}
	
	
	}
	pair<int,string>b;
	b.first=prof;
	b.second="";
	return b;

}

bool Cluster::enviar_proces_cluster(const Proces& proc){
	
	int mida_forat= proc.Consultar_MemOcupada();
	int id= proc.Consultar_id();
	set<string> lis_proc_pos;
	int forat_max=0;
	//invariant forat max te el forat mes a proper a mida_forat per sobre o igual lis_proc_pos hi han els strings
	//dels processadors que tenen de forat forat_max i la mateixa mem lliure
	//it itera es a dir esta entre map_cluster.begin() i map_cluster.end()
	//funció de cota els elements entre it i map_cluster.end()
	for(map<string,Procesador>::iterator it = map_cluster.begin(); it!=map_cluster.end(); ++it){
		int forat;
		if(it->second.forat(forat, mida_forat) and not it->second.existeix_Proces(id)){
			if(lis_proc_pos.empty() or forat<forat_max){
				lis_proc_pos.clear();
				lis_proc_pos.insert(it->first);
				forat_max=forat;
            //he sobre caregat l'operador > per tal que quan esta entre 2 elements de la clase processador ens retorna true si l espai lliure de la dreta es estrictament mes gran que el de l esquerra
			}else if(forat==forat_max){
				map<string,Procesador>::iterator it2=map_cluster.find(*(lis_proc_pos.begin()));
				if(it->second > it2->second){			
					lis_proc_pos.clear();
					lis_proc_pos.insert(it->first);
				}else if(it->second ==it2->second){
					lis_proc_pos.insert(it->first);
				}
			}
		}
	}
	//si no hem trobat cap processador que tingui un forat lo soficientment gran pq capiga el proces retornem false
	if(lis_proc_pos.empty()){
		return false;
	//si n'hem trobat nomes un ja podem donar d'alta el proces en aquell processador
	}else if(lis_proc_pos.size()== 1){
		map<string,Procesador>::iterator it;
		it=map_cluster.find(*(lis_proc_pos.begin()));
		(it)->second.Alta_Proces(proc);
		return true;
	//en cas que n'hagim trobat varis hem de recorre l'arbre per tal de determinar quin te menor profunditat
	//i en cas d'empat el que esta mes a l'esquerra
	}else{
		pair<int,string>a=in_m(arbre_cluster,0,lis_proc_pos);
		map<string,Procesador>::iterator it=map_cluster.find(a.second);
		it->second.Alta_Proces(proc);
	}
	return true;
	
}





