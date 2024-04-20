/**@file Procesador.cc
   @brief Codi de la classe Procesador
 */

#include "procesador.hh"

Procesador::Procesador(){
mem_total=0;
mem_lliure=0;
mem=map<int,Proces>();
On_es_proces=map<int,int>();
Espais_buits=map<int,set<int>>();

}


void Procesador::llegir(){
	int memoria;
	cin>>memoria;
	mem_total=memoria;
	mem_lliure=memoria;
	set<int> a;
	a.insert(0);
	Espais_buits[memoria]=a;


}


void Procesador::imprimir_Procesador()const{
	//invariant: s'han impres per pantalla tots els elsements entre mem.begin() i it;
	//	     it itera es a dir esta entre mem.begin() i mem.end();
	//funcio de cota: es el nombre d'elements entre it i mem.end();
	for(map<int,Proces>::const_iterator it=mem.begin(); it!=mem.end(); ++it){
		cout<<(it->first)<<" "<<(it->second).Consultar_id()<<" "<<(it->second).Consultar_MemOcupada()<<" "<<(it->second).Consultar_temps()<<endl;
		
	}


}

void Procesador::Alta_Proces(const Proces& proces){
	

	//comprovar si mem proces es mes petiti o igual que  l ultim element del mapa de forats que es el forat mes gran
	//fer nomes un upper bound i --it per mirar si es igual,
	//imprimir_forats();	
	int mem_pro =proces.Consultar_MemOcupada();
	map<int,set<int>>::iterator it=Espais_buits.lower_bound(mem_pro); //creem un it per buscar si existeix un espai exacte
	
	if(it==Espais_buits.end()){
		cout<<"error: no cabe proceso"<<endl; 
	
	}else{
		mem_lliure-=mem_pro;
		set<int>::iterator it2=it->second.begin();
		mem[(*it2)]=proces;
		On_es_proces[proces.Consultar_id()]=(*it2);
		int forat_lliure= (it->first) - mem_pro;
		if(forat_lliure!=0){
			int pos=(*it2)+mem_pro;
			Afegir_forat(forat_lliure,pos);
		}
		//tot i tenir una funcio que ens elimini el forat prefereixo fer l'errase aixi ja que la funcio treballa amb valors i aqui ja tenim 
		//els iterados calculats i penso que no es gaire eficien tornar-los a calcular en una altre funcio
		if(it->second.size()==1){
			Espais_buits.erase(it);
		}else{
			it->second.erase(it2);
		}

	}
	


}

bool Procesador::existeix_Proces(const int id_proces)const{
	map<int, int>::const_iterator it;
	it=On_es_proces.find(id_proces);
	return(it!=On_es_proces.end());
}

bool Procesador::te_procesos() const{
	return(mem.begin()!=mem.end());

}

int Procesador::forat_esquerra(const map<int,Proces>::iterator& it_act, int& pos_in){
	int mida_espai_ant=0;
	if(it_act!=mem.begin()){
		map<int,Proces>::iterator it_ant=it_act;
		--it_ant;
		int mem_oc_abans=(it_ant->first)+(it_ant->second).Consultar_MemOcupada();
		mida_espai_ant=it_act->first - mem_oc_abans;
	}else{
		mida_espai_ant=it_act->first;//cas que abans tingui un forat i sigui el primer proces a mem
	
	}
	if(mida_espai_ant!=0){
		pos_in=it_act->first - mida_espai_ant;
		//Ens permet eliminar el forat del mapa de forats en cas que existeixi
		Eliminar_forat(mida_espai_ant,pos_in);
	}
	return mida_espai_ant;

}

int Procesador::forat_dreta(const map<int,Proces>::iterator& it_act){
	int mida_espai_post=0;
	int mida_proces=(it_act->second).Consultar_MemOcupada();
	map<int,Proces>::iterator it_post=it_act;
		++it_post;
		//mirar si hi han processos despres
		if(it_post!=mem.end()){
		
			int mem_oc_post=(it_act->first)+mida_proces;
			mida_espai_post=it_post->first - mem_oc_post;
		
		}else{
			mida_espai_post=mem_total-(it_act->first + mida_proces );
			//cas que sigui l'ultim proces i tingui un espai buit despres;
		}
		//Ens permet eliminar el forat del mapa de forats en cas que existeixi
		if(mida_espai_post!=0 )Eliminar_forat(mida_espai_post,it_act->first + mida_proces);
	return mida_espai_post;
}

void Procesador::Afegir_forat(const int& mida_forat, const int&pos_in){
	map<int,set<int>>::iterator it_colocar=Espais_buits.find(mida_forat);
	if(it_colocar!=Espais_buits.end()){
		it_colocar->second.insert(pos_in);
	
	}else{
		set<int> a;
		a.insert(pos_in);
		Espais_buits[mida_forat]=a;
	
	}

}

void Procesador::Eliminar_forat(const int& mida_forat, const int& posicio_inicial){
	map<int,set<int>>::iterator it=Espais_buits.find(mida_forat);
	if(it->second.size()==1){
		Espais_buits.erase(it);
	}else{
		it->second.erase(posicio_inicial);
	}

}



bool Procesador::Baixa_Proces(const int& id){
	map<int, int>::iterator it;
	it=On_es_proces.find(id);
	if(it==On_es_proces.end())return false;
	int posmem=it->second;
	
	map<int,Proces>::iterator it_2=mem.find(posmem);
	int pos_in=it_2->first;
	//calculem el forat que ha deixat el proces al ser eliminat per tal de saber-ho mira si te forat a la dreta i a l'esquerra 
	//per aixo crida la funcio dorat_esquerra i forat_dreta
	int mida_forat=forat_esquerra(it_2, pos_in)+forat_dreta(it_2)+(it_2->second).Consultar_MemOcupada();
	//ens permet afegir el forat del tamany que hem calculat abans
	Afegir_forat(mida_forat, pos_in);
	mem_lliure+=it_2->second.Consultar_MemOcupada();
	mem.erase(it_2);
	On_es_proces.erase(it);
	//imprimir_forats();
	return true;

}

void Procesador::modificar_temps_procesos(const int& temps){
    map<int,Proces>::iterator it=mem.begin();
    bool anterior_eliminat=false;
    int mida_forat=0;
    int pos=0;
    //invariant: it itera es a dir esta entre mem.begin() i mem.end(); 
    //		en el mapa mem s'han eliminat tots els procesos que s'han acabat d'executar entre mem.begin() i it;
    //		en cas que no s'hagi acabat d'executar es modifica el temps d'aquests processos entre mem.begin() i it;
    //funcio de cota els elements entre it i mem.end();
    //aquest algoritme fa una finestra ja que si ja hem eliminat el proces anterior no cal consultar si hi havia forat abans ja que es el forat que ha deixat al eliminar el proces que tenia just abans per tant acomulant el tamany del forat cada cop que s'eliminen processos de forma seguida al primer cop que no s'elimini un podem fusionar tot amb un forat en canvi si en lloc de fer la finestra cridesim la funcio baja proceso ens mirarira el forat de devant cada cop quan ja el podem saber
    while(it!=mem.end()){
        //he sobre carregat l'operador <= per tal que cuan a l'esquera te un proces ens retorni true si el temps del proces es mes petit que el int de la dreta en cas contrari retorna false
        if(it->second<=temps){
        	//en el cas que l anterior no hagi sigut eliminat com que eliminem aquest cal mirar si hi havia forat abans i despres del proces
        	if(not anterior_eliminat){
        		pos=it->first;
        		mida_forat=forat_esquerra(it,pos) + (it->second).Consultar_MemOcupada() + forat_dreta(it);
        		anterior_eliminat=true;
        	//com que ja sabem que s'ha eliminat com a minim el proces de just abans que aquest nomes cal mirar el forat posterior ja que l anterior es de tamany mida_forat
        	}else{
        		mida_forat+=(it->second).Consultar_MemOcupada() + forat_dreta(it);
        	
        	}
        	mem_lliure+=it->second.Consultar_MemOcupada();
		On_es_proces.erase(it->second.Consultar_id());
		it=mem.erase(it);
  		
	}else{
		(it->second).modificar_temps(temps);
		//si els processos anteriors s'han eliminat i aquest no creem el forat del tamany que ha quedat al eliminar els processos
		if(anterior_eliminat){
			Afegir_forat(mida_forat,pos);
			anterior_eliminat=false;
			mida_forat=0;
		}
		++it;
			
	}

    }
	if(anterior_eliminat)Afegir_forat(mida_forat,pos);



}



void Procesador::redistribuir_mem(){
	//mirem que el mapa de Espais_buits no sigui empty per tal de asegurarnos que tenim com a minim un forat
	if(not Espais_buits.empty()){
		map<int,set<int>>::iterator it= Espais_buits.begin();
		// pos_primer_forat existeix ja que el mapa no esta empty i com que en el set estan ordenats de forma decreixent agafem el begin que
		//es el mes petit i el primer es el primer mes petit del mapa
		int pos_primer_forat= *((it-> second).begin());
		++ it;
		//invariant: pos_primer_forat te la posicio del forat mes proxim a la posicio 0 de mem entre Espais_buits.begin() i it
		//	     it itera es a dir el seu valor esta entre Espais_buits.begin() i Espais_buits.end()
		//funcio de cota: els elements entre it i Espais_buits.end()
		while(it != Espais_buits.end()){
			int pos_forat_seg= *((it-> second).begin());
			//mirem si la posicio del forat del seguent tamany del mapa esta a una posicio mes proxima al inici que la variable 
			//que ens indica on esta el primer forat	
			if(pos_forat_seg < pos_primer_forat){
				pos_primer_forat= pos_forat_seg;
			}
			++it;
		}
		//aconseguim el proces i la posicio que hi ha despres del forat end en cas que el primer forat no tingui procesos derrera
		map<int,Proces>::iterator it2= mem.upper_bound(pos_primer_forat);


		//comprobem que aquest primer forat tingui procesos a derrera per tal de compactar mem en el cas que sigui end no te forats entre processos per tant ja esta compactada
        if(it2 != mem.end()){
            int pos_inici_proces=pos_primer_forat;
            //si no te que compactar ja no entra en el while;
            //invariant: s'ha compactat la memoria de tots els elements entre it2 i mem.end()
            // 	     it itera es a dir esta entra mem.begin() i mem.end()
            //funcio de cota: els elements entre mem.end() i it2
            while(it2 != mem.end()){
                //l'inserim a la nova posicio en el mapa
                mem[pos_inici_proces]=it2->second;
                //modifiquem on es troba aquest proces del mapa On_es_proces
                On_es_proces[(it2->second).Consultar_id()]=pos_inici_proces;
                //modifiquem pos_inici_proces per tal que ens indiqui la posicio d'inici del seguent proces;
                pos_inici_proces+= it2->second.Consultar_MemOcupada();
                //al igualar it2 al errase it2 ens apuntara al seguent element del mapa mem
                it2=mem.erase(it2);
            }
            Espais_buits.clear();
            set<int> a;
            a.insert(pos_inici_proces);
            Espais_buits[mem_lliure]=a;
            //imprimir_forats();
        }
	
	}
	
	
	


}

 bool Procesador::forat(int& forat, const int &mida_forat)const{
 	map<int,set<int>>::const_iterator it = Espais_buits.lower_bound(mida_forat);
 	if(it!= Espais_buits.end()){
 		forat = it->first;
 		return true;
 	}
 	return false;
 
 }

bool Procesador::operator>(const Procesador& pro) const{
	return mem_lliure> pro.mem_lliure;
}

bool Procesador::operator==(const Procesador& pro) const{
	return mem_lliure==pro.mem_lliure;
}


