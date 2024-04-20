#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME POPSXGOAT_V1


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */
   typedef vector<vector<bool>> Pa;
   typedef vector<vector<double>> weight;
   typedef vector<vector<Pos>> Mat_pos;
   set<pair<int,int>> Mapa_gemes=set<pair<int,int>>();//mapa de gemes
//mapa interior l'inicialitzem a la primera it
   int posantS=40;//posicio ant del inici del sol comença 40 ja que priemra ronda no el modifiquem per tant es l ant de ronda 1
   int posiniS=0;
   long unsigned int padalt;
   set<pair<int,int>> Mapa_ascenssors=set<pair<int,int>>();//mapa dels ascenssors
   bool primer=true;// bool per la primera it per fer la martriu del mapa i el set dels ascenssors
   Pa Pos_atacades_h;
   Pa Pos_atacades_n;
  Pa Pos_atacades_f;
  /**
   * Play method, invoked once per each round.
   */

  //troba les gemes noves que han spawneat per aixo necessita l a primera casella que ara te ombre es a dir la primera en tenir sol de la ronda anterior
  //com que 25% de que apareixi 1 si ja la troba no cal que continui buscant

//funcio per els elements de les claseses
void trobar_gemes(){
  int c=cols();
  int r=rows();
  bool trobat=false;
  for(int i=0;i<r and not trobat;++i){
    for(int j=0;j<2 and not trobat;++j){
      int po=posantS+j;
      if(po>=c)po-=c;
      Cell aux=cell(i,po,1);
      if(aux.gem){
        Mapa_gemes.insert(pair<int,int>(i,po));
        trobat=true;
      }
    }

  }

}

void modificar_gemes(){
  set<pair<int,int>>:: iterator it=Mapa_gemes.begin();
  while(it!=Mapa_gemes.end()){
    if(not cell(it->first,it->second,1).gem)it=Mapa_gemes.erase(it);
    else ++it;
  }
  posantS+=2;
  if(posantS>=cols())posantS-=cols();
}


void llegir_grid(){

  int r= rows();
  int c=cols();


  for(int i=0;i<r;++i){
    for(int j=0;j<c;++j){
      Cell aux=cell(i,j,1);
      if(primer and aux.type==Elevator and  Mapa_ascenssors.find(pair<int,int>(i,j))==Mapa_ascenssors.end())Mapa_ascenssors.insert(pair<int,int>(i,j));
      else if(aux.gem and Mapa_gemes.find(pair<int,int>(i,j))==Mapa_gemes.end())Mapa_gemes.insert(pair<int,int>(i,j));
    }
  }
  primer=false;

   }

///////funcions generiques

//ens far el moviment a partir del id i de posicio ini i pos final
void fer_mov(int id,Pos& PdistMin,Pos& pini){
  bool fet=false;
  for(int i =0; i<8 and not fet;++i){
        Dir dir=Dir(i);
      if(pini+dir==PdistMin){
        fet=true;
        command(id,dir);
      }
  }
}

Pos trobar_pos(Mat_pos& pant, Pos p,Pos p_aliat){

  while(pant[p.i][p.j]!=p_aliat){
      p=pant[p.i][p.j];

  }

  return p;
}



//funcio per moure els pioners per sota te en compte a la hora de conquistar si es de algu te preferencia i si es d algu que va per sobre meu te mes preferencia
//millorar funcio pq te pinta que no va be
//fer la millor moviments a la dreta sumar 1 altres sumar 2 i igualar al sol afegir per tal que es pugui escapar

//funcions part de dalt
double Dist_AscenssorgemaProp(pair<int,int> ascenssors, int& dist){
  int distmin=9999999;
  int dygema=cols();
  pair<int,int>g;
  for(auto gema : Mapa_gemes){
    int dx=abs(ascenssors.first- gema.first);
    int dy=abs(ascenssors.second-gema.second);
    dy=min(dy,abs(cols()-dy));
    int daux=sqrt(dx*dx + dy*dy);
    if(distmin>daux){
      dygema=gema.second;
      distmin=daux;
      g=gema;
    }
  }

  int distSol_gema=0;
  if(posiniS<=dygema)distSol_gema=dygema-posiniS;
  else distSol_gema=dygema+(cols()-posiniS);
  if((distmin+dist)*2<=distSol_gema)dist+=distmin;
  else dist=9999999;
  return distmin*70;

}
Pos bfs_trobar_ascensor(Pos p){
    Pa mirat=Pa(rows(),vector<bool>(cols(),false));
    Mat_pos pant=Mat_pos(rows(),vector<Pos>(cols(),Pos(-1,-1,-1)));
    queue<Pos> cua;
    cua.push(p);
    mirat[p.i][p.j]=true;

    while(not cua.empty()){
      Pos act=cua.front();
      cua.pop();
      if(cell(act).type==Elevator){

          return trobar_pos(pant,act,p);
      }
      for(int i=0; i<8; ++i){
        Pos sig=act+Dir(i);
        if(sig.i==40)sig.i=39;
        else if(sig.i==-1)sig.i=0;
        if(not mirat[sig.i][sig.j] and not daylight(sig) and cell(sig).id==-1 and not Pos_atacades_n[sig.i][sig.j]){
          mirat[sig.i][sig.j]=true;
          cua.push(sig);
          pant[sig.i][sig.j]=act;
        }

      }



    }

    return p;
}

Pos bfs_gema(Pos p){

  Pa mirat=Pa(rows(),vector<bool>(cols(),false));
    Mat_pos pant=Mat_pos(rows(),vector<Pos>(cols(),Pos(-1,-1,-1)));
    queue<Pos> cua;
    cua.push(p);
    mirat[p.i][p.j]=true;

    while(not cua.empty()){
      Pos act=cua.front();
      cua.pop();
      if(cell(act).gem){

          return trobar_pos(pant,act,p);
      }
      for(int i=0; i<8; ++i){
        Pos sig=act+Dir(i);
        if(sig.i==40)sig.i=39;
        else if(sig.i==-1)sig.i=0;
        if(not mirat[sig.i][sig.j] and not daylight(sig) and cell(sig).id==-1 and not Pos_atacades_n[sig.i][sig.j]){
          mirat[sig.i][sig.j]=true;
          cua.push(sig);
          pant[sig.i][sig.j]=act;
        }

      }


    }

    return p;


}

//fer que vagin per la gema si no poden arribar abans que els mati el sol o no hi han gemes que busqui escapatoria
void Mou_Pioners_fora(int id){
  Pos pini=unit(id).pos;
  int d=0;
  Pos p;
  pair<int,int>aux(pini.i,pini.j);
  Dist_AscenssorgemaProp(aux,d);
  if(d<120-round() and not Mapa_gemes.empty()){
    p=bfs_gema(pini);
    if(pos_ok(p)){
      fer_mov(id,p,pini);
      Pos_atacades_n[p.i][p.j]=true;
    }


  }else{//intentar tornar a baix
    if(cell(pini).type==Elevator){
      command(id,Down);
      --padalt;
    }
    else{
       p=bfs_trobar_ascensor(pini);
      if(pos_ok(p)){
        Pos_atacades_n[p.i][p.j]=true;
        fer_mov(id,p,pini);
      }

    }

  }

}

//mirar si hellhound

//funcions matirus pos atacades
void atacs(){
  vector<int>hellhound=hellhounds();
  for(int id : hellhound){
    Pos pact=unit(id).pos;
    for(int i=pact.i-2; i<=pact.i+2;++i){
      for(int j=pact.j-2; j<=pact.j+2;++j){
        int iajustada=(i+rows())%rows();
        int jajustada=(j+cols())%cols();
        if(cell(iajustada,jajustada,0).type!=Rock)Pos_atacades_h[iajustada][jajustada]=true;
      }
    }

  }
  for(int k=0;k<3;++k){
      if(k!=me()){
        vector<int>fur=furyans(k);
        for(int id : fur){
          Pos pact=unit(id).pos;
          bool fora=pact.k==1;
          for(int i=pact.i-1; i<=pact.i+1;++i){
            for(int j=pact.j-1; j<=pact.j+1;++j){
              int iajustada=(i+rows())%rows();
              int jajustada=(j+cols())%cols();
              if(not fora)Pos_atacades_f[iajustada][jajustada]=true;
              else Pos_atacades_n[iajustada][jajustada]=true;
            }
          }

        }

      }


  }



}

 void mirar_p_adalt(){
  vector<int>P=pioneers(me());
  for(int id : P){
    if(unit(id).pos.k==1)++padalt;
  }
}

//funcio per detectar caselles atacades per Hellhound;
void atacs_h(){

  vector<int>N=necromongers();
  for(int id : N){
    Pos pact=unit(id).pos;
    for(int i=pact.i-1; i<=pact.i+1;++i){
      for(int j=pact.j-1; j<=pact.j+1;++j){
        int iajustada=i;
        if(iajustada>=rows())iajustada=rows()-1;
        if(iajustada<0)iajustada=0;
        int jajustada=(j+cols())%cols();
        Pos_atacades_n[iajustada][jajustada]=true;
      }
    }

  }


}

void Troba_ascensor_dij(Pos pos,weight& pesnodes,Mat_pos& pant){
  Pa mirat=Pa(rows(),vector<bool>(cols(),false));//matriu per saber si ja s'ha mirat
  pant=Mat_pos(rows(),vector<Pos>(cols()));//matriu de la pos ant serveix pel cami
  pesnodes=weight(rows(),vector<double>(cols(),999999999));//matriu de pesos
  priority_queue<pair<double,Pos>, vector<pair<double,Pos>>,greater<pair<double,Pos>>> q;
  int puntsme=nb_cells(me())+30*nb_gems(me());//cua de pos ant pes
  q.push(pair<double,Pos>(0,pos));
  pesnodes[pos.i][pos.j]=0;
  //fiquem el primer element i anem iterant fins que estigui buida
  while(not q.empty()){
    Pos act=q.top().second;
    q.pop();
    if(not mirat[act.i][act.j]){
      mirat[act.i][act.j]=true;
      for(int i =0; i<8;++i){
        Dir dir=Dir(i);
        //com que el fem de la planta de abaix no cal up ni down
          Pos nseg=act+dir;
          Cell cseg=cell(nseg);
          double pen=100;
          if(dir==LB or dir==Left or dir==TL)pen+=35;
          else if(dir==Top or dir==Bottom)pen+=15;
          else pen-=35;
          if(cseg.type==Cave){
            int mult=0;
             if(cseg.owner!=me()){
              if(cseg.owner==-1)mult=1;
              else if(puntsme<nb_cells(cseg.owner)+30*nb_gems(cseg.owner))mult=2.5;// si el tiu va per sobre jugua mes agresiu
              else mult=2;

            }
            pen-=20*mult;
          }
          //falten els altres atacs que es juga depenen de si et pots escapar i no et mata
          //nomes fiquem moviments legals per tant no podem anar a celes que son roques
          if(cseg.id == -1 and cseg.type != Rock and not Pos_atacades_h[nseg.i][nseg.j] and not Pos_atacades_f[nseg.i][nseg.j] and pesnodes[nseg.i][nseg.j] > pesnodes[act.i][act.j] + pen){
            pesnodes[nseg.i][nseg.j]=pesnodes[act.i][act.j] + pen;
            pant[nseg.i][nseg.j]=act;
            q.push(pair<double,Pos>(pesnodes[nseg.i][nseg.j],nseg));
          }
      }

    }

  }
}

bool comprobar_con_asc(const double& PdistMin,const double& PdistAux ,const int& distmin,const int& dist){
  return dist!=9999999 and dist<120-round() and  (PdistMin>PdistAux or(PdistMin==PdistAux and distmin>dist))  ;
}

Pos bfs_casellanc_v2(Pos p){
    Pa mirat=Pa(rows(),vector<bool>(cols(),false));
    Mat_pos pant=Mat_pos(rows(),vector<Pos>(cols(),Pos(-1,-1,-1)));
    queue<Pos>cua;
    cua.push(p);
    mirat[p.i][p.j]=true;
    while(not cua.empty()){
      Pos act=cua.front();
      cua.pop();
      Cell cact=cell(act);
      if(cact.type==Cave and cact.owner!=me() and  cact.type!=Elevator){
        return trobar_pos(pant,act,p);
      }
      for(int i=0; i<8; ++i){

        Pos sig=act+Dir(i);

        Cell cseg=cell(sig);
        if(sig.i==40)sig.i=39;
        else if(sig.i==-1)sig.i=0;
        if(not mirat[sig.i][sig.j]and cseg.id == -1 and cseg.type != Rock and not Pos_atacades_h[sig.i][sig.j] and not Pos_atacades_f[sig.i][sig.j]){
          mirat[sig.i][sig.j]=true;
          cua.push(sig);
          pant[sig.i][sig.j]=act;
        }

      }

    }
    return p;

}

Pos bfs_Casellanc(Pos p){
    int puntsme=nb_cells(me())+30*nb_gems(me());
    Pa mirat=Pa(rows(),vector<bool>(cols(),false));
    Mat_pos pant=Mat_pos(rows(),vector<Pos>(cols(),Pos(-1,-1,-1)));
    priority_queue<pair<double,Pos>, vector<pair<double,Pos>>,greater<pair<double,Pos>>> cua;
    cua.push(pair<double,Pos>(0,p));
    mirat[p.i][p.j]=true;

    while(not cua.empty()){
      Pos act=cua.top().second;
      double penact=cua.top().first;
      cua.pop();
      Cell cact=cell(act);
      if(cact.type==Cave and cact.owner!=me()and  cact.type!=Elevator){

          return trobar_pos(pant,act,p);
      }
      for(int i=0; i<8; ++i){
        Dir dir=Dir(i);
        Pos sig=act+dir;

        Cell cseg=cell(sig);
        if(sig.i==40)sig.i=39;
        else if(sig.i==-1)sig.i=0;
        if(not mirat[sig.i][sig.j]and cseg.id == -1 and cseg.type != Rock and not Pos_atacades_h[sig.i][sig.j] and not Pos_atacades_f[sig.i][sig.j]){
            double pen=100;
            if(dir==LB or dir==Left or dir==TL)pen-=15;
            else if(dir==Top or dir==Bottom)pen+=5;
            else pen+=15;
            if(cseg.type==Cave){
              int mult=0;
              if(cseg.owner!=me()){
                if(cseg.owner==-1)mult=1;
                else if(puntsme<nb_cells(cseg.owner)+30*nb_gems(cseg.owner))mult=2.5;// si el tiu va per sobre jugua mes agresiu
                else mult=2;

                }
                pen-=20*mult;
              }

          mirat[sig.i][sig.j]=true;
          cua.push(pair<double,Pos>(pen+penact,sig));
          pant[sig.i][sig.j]=act;
        }

      }


    }


    return p;


}


//si trobar un ascenssor mirar la gema que te mes aprop per saber dist affegir lo de npc a fora
void Mou_Pioners_Cova(int id){
  Pos pini=unit(id).pos;


  //fa el dijkstra per cada un fe nomes amb asenssors de la dreta
  //falta fer cout del camir amb menys penalitzacio al asccensors mes proper

  bool prova=false;
  int d=0;
  pair<int,int>aux(pini.i,pini.j);
  Dist_AscenssorgemaProp(aux,d);
  cerr<<padalt<<" mida_mg"<<Mapa_gemes.size()<<endl;
  if(d<120-round() and cell(pini).type==Elevator and not daylight(pini+Up)){
    command(id,Up);//mirar que pugui pujar be
    ++padalt;
  }else if(padalt<Mapa_gemes.size() and not Mapa_gemes.empty() and cell(pini).type!=Elevator){//jugar a pujar si hi han gemes si no son mes utils abaix;
      weight pesnodes;
      Mat_pos pant;

      Troba_ascensor_dij(pini,pesnodes,pant);
      double PdistMin=9999999999;
      int distmin=999999999;
      Pos segmin(-1,-1,-1);
      for(pair<int,int> ascensor : Mapa_ascenssors){
        pair<int,int>xy=ascensor;
        double PdistAux=pesnodes[xy.first][xy.second];
        if(pesnodes[xy.first][xy.second]!=999999999 and PdistMin>=PdistAux){

          int dist=0;
          while(pant[xy.first][xy.second]!=pini ){
              pair<int,int>aux=xy;
              xy.first=pant[aux.first][aux.second].i;
              xy.second=pant[aux.first][aux.second].j;
              ++dist;
          }

          int ascGem=Dist_AscenssorgemaProp(ascensor,dist);
          if(comprobar_con_asc(PdistMin,PdistAux+ascGem,distmin,dist)){//falta afegir qua al arribar no hi hagi sol o si no suda i ja s'en adona mes endevant
            segmin.i=xy.first;
            segmin.j=xy.second;
            segmin.k=0;
            PdistMin=PdistAux;
            distmin=dist;
          }//falta cas igual dretarentheses around left hand side expression to silence this warning




        }
      }
      if(pos_ok(segmin)){
        fer_mov(id,segmin,pini);
        Pos_atacades_h[segmin.i][segmin.j]=true;
      }
      else{
        Pos p=bfs_casellanc_v2(pini);
        if(pos_ok(p)){
          fer_mov(id,p,pini);
          Pos_atacades_h[p.i][p.j]=true;
        }
        //dijkstra per moures fins a trobar la casella que no es meva i amb costos tal i com esta estipulat l unic que aquest cop cap a l'esquera meys penalitzacio i cap a la dreta mes
      }
  }else{
      int x=(posiniS-pini.j + cols())%cols();
      if(x<=20){
          //busquin conquerir / ascensor cap a la dreta per tal que cuan el sol els atrapi pujar a pillar gema

          Pos p=bfs_casellanc_v2(pini);
          if(pos_ok(p)){
            fer_mov(id,p,pini);
            Pos_atacades_h[p.i][p.j]=true;
          }
      }else{
        //dijkstra per moures fins a trobar la casella que no es meva i amb costos tal i com esta estipulat l unic que aquest cop cap a l'esquera meys penalitzacio i cap a la dreta mes
        Pos p=bfs_casellanc_v2(pini);
        if(pos_ok(p)){
          fer_mov(id,p,pini);
          Pos_atacades_h[p.i][p.j]=true;
        }
      }


    //provar dijkstra fer cert n - pconc (depenent del tipus) - distancia a ascenssor mes proper al nou sol
      //2 casos que estiguin molt a la dreta per tant van cap a l'esquerra o cas 2 que estiguin sota el sol pero a la esquerra aixi van cap a la dreta si tenen algun ascensor aprop

  }
  //fer dikstra o bfs per trobar asccensors i mirar el que esta mes aprop de la gema;
  //si no ferlos moure cap a on apareixera la ombre per tal de que pilli gemes





}




Pos Calcular_dist_enemic(Pos p_aliat){
    Pa mirat=Pa(rows(),vector<bool>(cols(),false));
    Mat_pos pant=Mat_pos(rows(),vector<Pos>(cols(),Pos(-1,-1,-1)));
    queue<Pos> cua;
    cua.push(p_aliat);
    mirat[p_aliat.i][p_aliat.j]=true;

    while(not cua.empty()){
      Pos act=cua.front();
      cua.pop();
      if(cell(act).id !=-1 and unit(cell(act).id).player!=me() and unit(cell(act).id).player>=0 and (unit(cell(act).id).type==Pioneer or unit(cell(act).id).type==Furyan)){
          return trobar_pos(pant,act,p_aliat);
      }

      for(int i=0; i<8; ++i){
        Pos sig=act+Dir(i);
        if(not mirat[sig.i][sig.j] and cell(sig).type!=Rock and not Pos_atacades_h[sig.i][sig.j]){
          mirat[sig.i][sig.j]=true;
          cua.push(sig);
          pant[sig.i][sig.j]=act;
        }

      }

    }

    return p_aliat;


}


void Mou_Furyans(){
  vector<int>Furyans=furyans(me());
  for(int furyan : Furyans){
      Pos pini=unit(furyan).pos;
       Pos a=Calcular_dist_enemic(pini);
        fer_mov(furyan,a,pini);
        Pos_atacades_h[a.i][a.j]=true;
  }


}

void Mou_Pioners(){
  vector<int>Pioners=pioneers(me());//retorna un vector amb els meus pioners
  int mida=Pioners.size();
  for(int id : Pioners){

    if(unit(id).pos.k==0)Mou_Pioners_Cova(id);
    else Mou_Pioners_fora(id);
  }

}



virtual void play () {
  modificar_gemes();
  llegir_grid();
  padalt=0;
  posiniS+=2;
  if(posiniS>=cols())posiniS-=cols();
  mirar_p_adalt();
  Pos_atacades_h=Pa(rows(),vector<bool>(cols(),false));
  Pos_atacades_n=Pa(rows(),vector<bool>(cols(),false));
  Pos_atacades_f=Pa(rows(),vector<bool>(cols(),false));
  atacs_h();

  atacs();
  Mou_Pioners();
  Mou_Furyans();


}

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
