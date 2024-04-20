#include "Player.hh"
#include <algorithm>

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME POPSXGOATED5


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
   struct movInfo{
     int id;
     Pos pini;
     Dir d;
     UnitType tipus;
     bool escapar;
     bool compe;

  };





   typedef vector<vector<bool>> Pa;
   typedef vector<vector<int>> Pb;
   typedef vector<vector<double>> weight;
   typedef vector<vector<Pos>> Mat_pos;
   typedef vector<movInfo> Moviments;
   set<pair<int,int>> Mapa_gemes=set<pair<int,int>>();//mapa de gemes
   int posantS=40;//posicio ant del inici del sol comença 40 ja que priemra ronda no el modifiquem per tant es l ant de ronda 1
   int posiniS=0;
   unsigned long int Qfuryans;
   unsigned long int padalt;
   set<pair<int,int>> Mapa_ascenssors=set<pair<int,int>>();//mapa dels ascenssors
   bool primer=true;// bool per la primera it per fer la martriu del mapa i el set dels ascenssors
   Pa Pos_atacades_h;
   Pa Pos_atacades_n;
   Pa Pos_atacades_f;
   Pb Pos_atacades_eval;
   map<Pos,pair<int,int>> MapaPosDist;
   set<int> dvu;

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
bool gemaComp(Pos p){
  for(int i=0; i<8;++i){
      if(pos_ok(p+Dir(i)) and cell(p+Dir(i)).id!=-1 and unit(cell(p+Dir(i)).id).player!=me() and unit(cell(p+Dir(i)).id).type==Pioneer)return true;
  }
  return false;
}

bool ascComp(Pos p){
  for(int i=0; i<8;++i){
      if(pos_ok(p+Dir(i)) and cell(p+Dir(i)).type!=Rock and cell(p+Dir(i)).id!=-1 and unit(cell(p+Dir(i)).id).player!=me())return true;
  }
  return false;

}
bool furyan_mele(Pos& pini){
  for(int i=0; i<8; ++i){
    Pos sig=pini+Dir(i);
    if(pos_ok(sig) and cell(sig).type!=Rock and cell(sig).id !=-1 and unit(cell(sig).id).player!=me() and  unit(cell(sig).id).type==Furyan){
      return true;

    }
  }
  return false;

}
int fenvoltants(Pos& pini){
  int counter=0;
   for(int i=0; i<8; ++i){
    Pos sig=pini+Dir(i);
    if(pos_ok(sig) and cell(sig).type!=Rock and cell(sig).id !=-1 and unit(cell(sig).id).player!=me() and  unit(cell(sig).id).type==Furyan)++counter;
   }
   return counter;
}

bool furyan_mele_f(Pos& pini,int vidaF){
  for(int i=0; i<8; ++i){
    Pos sig=pini+Dir(i);
    if(pos_ok(sig) and cell(sig).type!=Rock and cell(sig).id !=-1 and unit(cell(sig).id).player!=me() and  unit(cell(sig).id).type==Furyan){
      int vidaEnemic=unit(cell(sig).id).health;
      if(((vidaF-vidaEnemic<=-10 and vidaF>50) or (vidaF-vidaEnemic<=-5 and vidaF<=50)) and vidaEnemic>25){
        return true;
      }else return false;
    }
  }
  return false;

}

//ens far el moviment a partir del id i de posicio ini i pos final
void fer_mov(int id,Pos& PdistMin,Pos& pini,Moviments& moviments){
  bool fet=false;
  //if(cell(PdistMin).gem)Mapa_gemes.erase(pair<int,int>(PdistMin.i,PdistMin.j));
  for(int i =0; i<8 and not fet;++i){
        Dir dir=Dir(i);
      if(pini+dir==PdistMin){
        fet=true;
        movInfo mov;
        mov.id=id;
        mov.pini=pini;
        mov.d=dir;
        if(unit(id).type==Pioneer)mov.escapar=furyan_mele(pini);
        else mov.escapar=furyan_mele_f(pini,unit(id).health);
        mov.tipus=unit(id).type;
        if(cell(pini+dir).gem)mov.compe=gemaComp(pini);
        else if(cell(pini+dir).type==Elevator)mov.compe=ascComp(pini);
        else mov.compe=false;
        moviments.push_back(mov);
      }
  }
}

Pos trobar_pos(Mat_pos& pant, Pos p,Pos p_aliat){
  int d=0;
  while(pant[p.i][p.j]!=p_aliat){
      p=pant[p.i][p.j];

  }

  return p;
}

int dist2(int j, int dist){
    if(posiniS<=j)return j-(posiniS+dist*2);
    else return j+(cols()-(posiniS+dist*2));
}

//funcio per moure els pioners per sota te en compte a la hora de conquistar si es de algu te preferencia i si es d algu que va per sobre meu te mes preferencia
//millorar funcio pq te pinta que no va be
//fer la millor moviments a la dreta sumar 1 altres sumar 2 i igualar al sol afegir per tal que es pugui escapar
bool dist_gema_gemaAsc(int dist, Pos& gact){
    Pa mirat=Pa(rows(),vector<bool>(cols(),false));
    int dist_sol=dist2(gact.j,dist);
    queue<pair<Pos,int>> cua;
    cua.push(pair<Pos,int>(gact,dist_sol));

    mirat[gact.i][gact.j]=true;

    while(not cua.empty()){

      pair<Pos,int> act=cua.front();
      cua.pop();
      if((cell(act.first).gem or cell(act.first).type==Elevator) and act.first!=gact){
          return true;
      }
      for(int i=0; i<8; ++i){

        Pos sig=act.first+Dir(i);
        if(sig.i>=rows())sig.i=rows()-1;
        else if(sig.i<=0)sig.i=0;
        int dsig=distn(act.second,Dir(i));
        if(not mirat[sig.i][sig.j] and not daylight(sig) and cell(sig).id==-1 and (not Pos_atacades_n[sig.i][sig.j] or cell(sig).gem) and dsig>=0){

          mirat[sig.i][sig.j]=true;
          cua.push(pair<Pos,int>(sig,dsig));
        }

      }


    }

    return false;


}

//funcions part de dalt
double Dist_AscenssorgemaProp(pair<int,int> ascenssors, int& dist){
  int distmin=9999999;
  int dygema=cols();
  pair<int,int>g;
  for(auto gema : Mapa_gemes){
    int dx=abs(ascenssors.first- gema.first);
    int dy=abs(ascenssors.second-gema.second);
    dy=min(dy,abs(cols()-dy));
    int daux=0;
    if(dy<dx)daux=dx+dx-dy;
    else daux=dy+dy-dx;
    if(distmin>daux){
      double distSol_gema=0;
      dygema=gema.second;
      if(posiniS<=dygema)distSol_gema=dygema-posiniS;
      else distSol_gema=dygema+(cols()-posiniS);
      if((daux+dist)<=(distSol_gema)/2)distmin=daux;
      g=gema;
    }
  }
  dist+=distmin;
  return (distmin*100-30000);
}

  double Dist_AscenssorgemaProp(pair<int,int> ascenssors, int& dist,Pos & act){
  int distmin=9999999;
  int dygema=cols();
  pair<int,int>g;
  for(auto gema : Mapa_gemes){
    int dx=abs(ascenssors.first- gema.first);
    int dy=abs(ascenssors.second-gema.second);
    dy=min(dy,abs(cols()-dy));
    int daux=0;
    if(dy<dx)daux=dx+dx-dy;
    else daux=dy+dy-dx;
    if(distmin>daux){
      double distSol_gema=0;
      dygema=gema.second;
      if(posiniS<=dygema)distSol_gema=dygema-posiniS;
      else distSol_gema=dygema+(cols()-posiniS);
      if((daux+dist)>(distSol_gema)/2)daux=999999999;
      g=gema;
      auto it=MapaPosDist.find(Pos(gema.first,gema.second,1));
      if(it==MapaPosDist.end() or it->second.first>daux+dist){
        distmin=daux;
        act=Pos(gema.first,gema.second,1);
      }
    }
  }
  dist+=distmin;
  return (distmin*100-30000);

}

bool comprobarsol(const Pos& p,const int& d ){
  int distSol_p;
  if(posiniS<=p.j)distSol_p=p.j-posiniS;
  else distSol_p=p.j+(cols()-posiniS);
  return d*2<=distSol_p;
}
int dist(int j){
  if(posiniS<=j)return j-posiniS;
  else return j+(cols()-posiniS);
}


int distn(int j, Dir d){
  if(d==LB or d==Left or d==TL)return j-3;
  else if(d==Top or d==Bottom)return j-2;
  return j-1;
}

Pos bfs_trobar_ascensor(Pos p){
    bool solalcostat=daylight(p+Left);
    Pa mirat=Pa(rows(),vector<bool>(cols(),false));
    Mat_pos pant=Mat_pos(rows(),vector<Pos>(cols(),Pos(-1,-1,-1)));
    queue<pair<Pos,int>> cua;
    
    int dist_sol=dist(p.j);
    cua.push(pair<Pos,int>(p,dist_sol));
    mirat[p.i][p.j]=true;
    
    while(not cua.empty()){
      pair<Pos,int> act=cua.front();
      cua.pop();
      if(cell(act.first).type==Elevator){
          return trobar_pos(pant,act.first,p);
      }
      for(int i=0; i<8; ++i){
        Pos sig=act.first+Dir(i);
        if(sig.i==40)sig.i=39;
        else if(sig.i==-1)sig.i=0;
        int dsig=distn(act.second,Dir(i));
        if(not mirat[sig.i][sig.j] and not( daylight(sig) and solalcostat)  and (cell(sig).id==-1 ) and (not Pos_atacades_n[sig.i][sig.j] or cell(sig).type==Elevator)and dsig>=0){
          mirat[sig.i][sig.j]=true;
          cua.push(pair<Pos,int>(sig,dsig));
          pant[sig.i][sig.j]=act.first;
        }

      }



    }
    return p;
}


Pos bfs_trobar_ascensor2(Pos p){
    bool solalcostat=daylight(p+Left);
    Pa mirat=Pa(rows(),vector<bool>(cols(),false));
    Mat_pos pant=Mat_pos(rows(),vector<Pos>(cols(),Pos(-1,-1,-1)));
    queue<pair<Pos,int>> cua;

    int dist_sol=dist(p.j);
    cua.push(pair<Pos,int>(p,dist_sol));
    mirat[p.i][p.j]=true;

    while(not cua.empty()){
      pair<Pos,int> act=cua.front();
      cua.pop();
      if(cell(act.first).type==Elevator or cell(act.first).gem){
          return trobar_pos(pant,act.first,p);
      }
      for(int i=0; i<8; ++i){
        Pos sig=act.first+Dir(i);
        if(sig.i==40)sig.i=39;
        else if(sig.i==-1)sig.i=0;
        int dsig=distn(act.second,Dir(i));
        if(not mirat[sig.i][sig.j] and not( daylight(sig) and solalcostat)  and (cell(sig).id==-1 ) and dsig>=0){
          mirat[sig.i][sig.j]=true;
          cua.push(pair<Pos,int>(sig,dsig));
          pant[sig.i][sig.j]=act.first;
        }

      }



    }
    return p;
}
//falta mirar que la gema que busquem poguem arribar fe lo mateix que abaix cua amb ints que es distancia respecte el sol
void recalcular(int id,Moviments& moviments){
  bool trobat=false;
  for(int i=0; i<moviments.size() and not trobat; ++i ){
    if(moviments[i].id==id)swap(moviments[i],moviments[moviments.size()-1]);
  }
 Pos pseg=moviments[moviments.size()-1].pini + moviments[moviments.size()-1].d;
  Pos_atacades_n[pseg.i][pseg.j]=false;
  moviments.pop_back();
  Mou_Pioners_fora(id,moviments);
}

Pos bfs_gema(Pos p, int& idrec){

  Pa mirat=Pa(rows(),vector<bool>(cols(),false));
    Mat_pos pant=Mat_pos(rows(),vector<Pos>(cols(),Pos(-1,-1,-1)));
    vector<vector<int>> distpa=vector<vector<int>>(rows(),vector<int>(cols(),0));
    int dist_sol=dist(p.j);
    queue<pair<Pos,int>> cua;
    cua.push(pair<Pos,int>(p,dist_sol));
    
    mirat[p.i][p.j]=true;

    while(not cua.empty()){

      pair<Pos,int> act=cua.front();
      cua.pop();
      if(cell(act.first).gem ){
        auto it=MapaPosDist.find(act.first);
        if(it==MapaPosDist.end() or it->second.first > distpa[act.first.i][act.first.j]){
          if(it!=MapaPosDist.end()){
            idrec=it->second.second;
            it->second=pair<int,int>(distpa[act.first.i][act.first.j],cell(p).id);
        }else MapaPosDist[act.first]=pair<int,int>(distpa[act.first.i][act.first.j],cell(p).id);

          return trobar_pos(pant,act.first,p);

        }

      }
      for(int i=0; i<8; ++i){
        int distact=distpa[act.first.i][act.first.j];
        Pos sig=act.first+Dir(i);
        if(sig.i>=rows())sig.i=rows()-1;
        else if(sig.i<=0)sig.i=0;
        int dsig=distn(act.second,Dir(i));
        if(not mirat[sig.i][sig.j] and not daylight(sig) and cell(sig).id==-1 and (not Pos_atacades_n[sig.i][sig.j] or cell(sig).gem) and dsig>=0){
          distpa[sig.i][sig.j]=distact+1;
          mirat[sig.i][sig.j]=true;
          cua.push(pair<Pos,int>(sig,dsig));
          pant[sig.i][sig.j]=act.first;
        }

      }


    }
	
    return p;


}



void seguirviu(Pos & pini,int id, Moviments& moviments){
  vector<Dir> esqu={Left, TL,LB};
  bool fet=false;
  for(int i=0; i<3 and not fet; ++i){
    Pos sig=pini+esqu[i];
    if(pos_ok(sig) and not daylight(sig) and not Pos_atacades_n[sig.i][sig.j]){
      fet=true;
      movInfo a;
      a.id=id;
      a.pini=pini;
      a.escapar=false;
      a.tipus=Pioneer;
      a.d=esqu[i];
      a.compe=false;
      moviments.push_back(a);
    }
  }
  if(not fet){
    vector<Dir>dre={Top,Bottom,Right,RT,BR};
    for(int i=0; i<5 and not fet; ++i){
      Pos sig=pini+dre[i];
      if(pos_ok(sig) and not daylight(sig) and not Pos_atacades_n[sig.i][sig.j]){
        fet=true;
        movInfo a;
        a.id=id;
        a.pini=pini;
        a.escapar=false;
        a.tipus=Pioneer;
        a.d=dre[i];
        a.compe=false;
        moviments.push_back(a);
      }
    }
  }


}

//fer que vagin per la gema si no poden arribar abans que els mati el sol o no hi han gemes que busqui escapatoria
void Mou_Pioners_fora(int id,Moviments& moviments){
  Pos pini=unit(id).pos;
  int d=0;
  Pos p=pini;
  pair<int,int>aux(pini.i,pini.j);
  Dist_AscenssorgemaProp(aux,d);
  if(d<120-round() and not Mapa_gemes.empty()){
    int idrec=-1;
    p=bfs_gema(pini,idrec);


    if(pos_ok(p) and p!=pini){

      fer_mov(id,p,pini,moviments);
      if(idrec!=-1)recalcular(idrec,moviments);

      Pos_atacades_n[p.i][p.j]=true;
    }else{
	    if(cell(pini).type==Elevator){
        movInfo a;
        a.id=id;
        a.pini=pini;
        a.escapar=false;
        a.tipus=Pioneer;
        a.d=Down;
        a.compe=false;
        moviments.push_back(a);
	      --padalt;
	    }else{
		Pos q=bfs_trobar_ascensor(pini);

        if(q!=pini){
          Pos_atacades_n[q.i][q.j]=true;
          fer_mov(id,q,pini,moviments);
        }else{
          Pos q=bfs_trobar_ascensor2(pini);
          fer_mov(id,q,pini,moviments);
          Pos_atacades_n[q.i][q.j]=true;
        }

	    }

    }


  }else{//intentar tornar a baix
    if(cell(pini).type==Elevator){
        movInfo a;
        a.id=id;
        a.pini=pini;
        a.escapar=false;
        a.tipus=Pioneer;
        a.d=Down;
        a.compe=false;
        moviments.push_back(a);
//       command(id,Down);
      --padalt;
    }
    else{
       p=bfs_trobar_ascensor(pini);
      if(p!=pini){
        Pos_atacades_n[p.i][p.j]=true;
        fer_mov(id,p,pini,moviments);
      }else{
        Pos q=bfs_trobar_ascensor2(pini);
        fer_mov(id,q,pini,moviments);
        Pos_atacades_n[q.i][q.j]=true;
      }

    }

  }

}



//mirar si hellhound

//funcions matirus pos atacades
//ferla be que no va

void atacs_hellhounds(int id){
  Pos pact=unit(id).pos;
  Pa mirat=Pa(rows(),vector<bool>(cols(),false));
  queue<pair<Pos,int>>q;
  q.push(pair<Pos,int>(pact,0));
  mirat[pact.i][pact.j]=true;
  while(not q.empty()){
    Pos act=q.front().first;
    int dact=q.front().second;
    q.pop();
    if(dact<2){
      for(int i=0; i<8;++i){
        Pos seg=act+Dir(i);
        if(pos_ok(seg) and not mirat[seg.i][seg.j] and cell(act).type!=Rock){
          Pos_atacades_h[seg.i][seg.j]=true;

          if(dact==0)Pos_atacades_eval[seg.i][seg.j]=9999999;
          else if(dact==1)Pos_atacades_eval[seg.i][seg.j]=9999900;
          mirat[seg.i][seg.j]=true;
          q.push(pair<Pos,int>(seg,dact+1));
        }
      }
    }

  }
}
void atacs(){
  vector<int>hellhound=hellhounds();
  for(int id : hellhound){
    atacs_hellhounds(id);
  }
  for(int k=0;k<=3;++k){
      if(k!=me()){
        vector<int>fur=furyans(k);
        for(int id : fur){
          Pos pact=unit(id).pos;
          bool fora=pact.k==1;
          for(int i=pact.i-1; i<=pact.i+1;++i){
            for(int j=pact.j-1; j<=pact.j+1;++j){
              int iajustada=i;
              if(iajustada>=rows())iajustada=rows()-1;
              else if(iajustada<=0)iajustada=0;
              int jajustada=(j+cols())%cols();
              if(not fora){
                Pos_atacades_f[iajustada][jajustada]=true;
                if((i==pact.i-1 or i==pact.i+1) and (i==pact.j-1 or i==pact.j+1))Pos_atacades_eval[iajustada][jajustada]=50000;
                else Pos_atacades_eval[iajustada][jajustada]=52000;
              }
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
    if(unit(id).pos.k==1 and cell(unit(id).pos).type!=Elevator)++padalt;
  }
}

//funcio per detectar caselles atacades per Hellhound;
void atacs_n(){

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
//           if(dir==LB or dir==Left or dir==TL)pen+=35;
//           else if(dir==Top or dir==Bottom)pen+=15;
//           else pen-=35;
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



//mira si surt rentable
bool dreta(int ini, int act){
  int distanciaDreta=(act-ini+cols())%cols();
  int distancuaEsquerra=(ini-act+cols())%cols();
  return distanciaDreta<=distancuaEsquerra;
}



bool comprobar_con_asc(const double& PdistMin,const double& PdistAux ,const int& distmin,const int& dist){
  return dist<120-round()  and  (PdistMin>PdistAux or(PdistMin==PdistAux and distmin>dist));
}





Pos bfs_Casellanc(Pos p){
    int puntsme=nb_cells(me())+30*nb_gems(me());
    Pa mirat=Pa(rows(),vector<bool>(cols(),false));
    Mat_pos pant=Mat_pos(rows(),vector<Pos>(cols(),Pos(-1,-1,-1)));
    priority_queue<pair<double,Pos>, vector<pair<double,Pos>>,greater<pair<double,Pos>>> cua;
    cua.push(pair<double,Pos>(Pos_atacades_eval[p.i][p.j],p));
    mirat[p.i][p.j]=true;

    while(not cua.empty()){
      Pos act=cua.top().second;
      double penact=cua.top().first;
      cua.pop();
      Cell cact=cell(act);
      if(cact.type==Cave and cact.owner!=me() and  cact.type!=Elevator){

          return trobar_pos(pant,act,p);
      }
      for(int i=0; i<8; ++i){
        Dir dir=Dir(i);
        Pos sig=act+dir;

        Cell cseg=cell(sig);
        if(sig.i==40)sig.i=39;
        else if(sig.i==-1)sig.i=0;
        if(not mirat[sig.i][sig.j] and cseg.id == -1 and cseg.type != Rock and not Pos_atacades_h[sig.i][sig.j]){
            double pen=5;
            pen+=Pos_atacades_eval[sig.i][sig.j];
            if(cseg.type==Cave){
              int mult=0;
              if(cseg.owner!=me()){
                if(cseg.owner==-1)mult=1;
                else if(puntsme<nb_cells(cseg.owner)+30*nb_gems(cseg.owner))mult=3;// si el tiu va per sobre jugua mes agresiu
                else mult=2;

                }
                pen-=mult;
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
void Mou_Pioners_Cova(int id,Moviments& moviments){
  Pos pini=unit(id).pos;


  //fa el dijkstra per cada un fe nomes amb asenssors de la dreta
  //falta fer cout del camir amb menys penalitzacio al asccensors mes proper

  bool prova=false;
  int d=0;
  pair<int,int>aux(pini.i,pini.j);
  Dist_AscenssorgemaProp(aux,d);

  if(d<120-round() and cell(pini).type==Elevator and not daylight(pini+Up)){
    movInfo a;
    a.id=id;
    a.pini=pini;
    a.d=Up;
    a.tipus=Pioneer;
    a.escapar=false;
    a.compe=false;
    moviments.push_back(a);
    //command(id,Up);//mirar que pugui pujar be
    ++padalt;
  }else if( not Mapa_gemes.empty() and cell(pini).type!=Elevator){//jugar a pujar si hi han gemes si no son mes utils abaix;
      weight pesnodes;
      Mat_pos pant;

      Troba_ascensor_dij(pini,pesnodes,pant);
      double PdistMin=9999999999;
      int distmin=999999999;
      Pos segmin=pini;
      for(pair<int,int> ascensor : Mapa_ascenssors){
        pair<int,int>xy=ascensor;
        int dist=0;
        int daux=0;
        double PdistAux=pesnodes[xy.first][xy.second]+Dist_AscenssorgemaProp(ascensor,dist);
        if( pesnodes[xy.first][xy.second]!=999999999 and PdistMin>=PdistAux and dist<120-round()){

          while(pant[xy.first][xy.second]!=pini ){
              pair<int,int>aux=xy;
              xy.first=pant[aux.first][aux.second].i;
              xy.second=pant[aux.first][aux.second].j;
              ++dist;
              ++daux;
          }
          Pos paux;
          Dist_AscenssorgemaProp(ascensor,daux,paux);
          bool arriba=false;
          if(round()>70 or dist_gema_gemaAsc(daux,paux))arriba=true;
          if(comprobar_con_asc(PdistMin,PdistAux,distmin,daux) and arriba){//falta afegir qua al arribar no hi hagi sol o si no suda i ja s'en adona mes endevant
            segmin.i=xy.first;
            segmin.j=xy.second;
            segmin.k=0;
            PdistMin=PdistAux;
            distmin=dist;
          }//falta cas igual dretarentheses around left hand side expression to silence this warning




        }
      }
      if(pini!=segmin){
        fer_mov(id,segmin,pini,moviments);
        Pos_atacades_h[segmin.i][segmin.j]=true;
      }
      else{
        Pos p=bfs_Casellanc(pini);
        if(pos_ok(p)){
          fer_mov(id,p,pini,moviments);
          Pos_atacades_h[p.i][p.j]=true;
        }
        //dijkstra per moures fins a trobar la casella que no es meva i amb costos tal i com esta estipulat l unic que aquest cop cap a l'esquera meys penalitzacio i cap a la dreta mes
      }
  }else{
        //dijkstra per moures fins a trobar la casella que no es meva i amb costos tal i com esta estipulat l unic que aquest cop cap a l'esquera meys penalitzacio i cap a la dreta mes
      Pos p=bfs_Casellanc(pini);
      if(pos_ok(p)){
        fer_mov(id,p,pini,moviments);
        Pos_atacades_h[p.i][p.j]=true;
      }



    //provar dijkstra fer cert n - pconc (depenent del tipus) - distancia a ascenssor mes proper al nou sol
      //2 casos que estiguin molt a la dreta per tant van cap a l'esquerra o cas 2 que estiguin sota el sol pero a la esquerra aixi van cap a la dreta si tenen algun ascensor aprop

  }
  //fer dikstra o bfs per trobar asccensors i mirar el que esta mes aprop de la gema;
  //si no ferlos moure cap a on apareixera la ombre per tal de que pilli gemes





}

int eval(Pos &p,int vida){
Unit aqui=unit(cell(p).id);
int evaluation=0;
if(aqui.type==Furyan){
    if(aqui.health<=25)evaluation=12700;
    else if(aqui.health<=50 and aqui.health<vida)evaluation =600+(51-aqui.health)*(vida-aqui.health);
    else evaluation =10*(vida-aqui.health);
    if(dvu.find(cell(p).id)!=dvu.end())evaluation+=evaluation*1.5;
}else{
    if(aqui.health<=25)evaluation=12500;
    else evaluation=700+(51-aqui.health);
    if(dvu.find(cell(p).id)!=dvu.end())evaluation+=evaluation*1.25;
}

return evaluation;




}

void trobarmillor(Pos& act, queue<pair<Pos,int>>& cua, int vida,int dist){
  int milloreval=eval(act,vida);
  int millorpos=0;
  while(not cua.empty()){
    Pos seg=cua.front().first;
    int diseg=cua.front().second;
    cua.pop();
    if(dist>=diseg and cell(seg).id !=-1 and unit(cell(seg).id).player!=me() and unit(cell(seg).id).player>=0 and (unit(cell(seg).id).type==Pioneer or unit(cell(seg).id).type==Furyan)){
        int auxeval=eval(seg,vida);
        if(auxeval>milloreval){
          milloreval=auxeval;
          act=seg;
          millorpos=nb_cells(unit(cell(seg).id).player)+30*nb_gems(unit(cell(seg).id).player);
        }else if(auxeval==milloreval and millorpos< nb_cells(unit(cell(seg).id).player)+30*nb_gems(unit(cell(seg).id).player)){
          milloreval=auxeval;
          act=seg;
          millorpos=nb_cells(unit(cell(seg).id).player)+30*nb_gems(unit(cell(seg).id).player);
        }
      }

  }
  dvu.insert(cell(act).id);
}




Pos Calcular_dist_enemic(Pos p_aliat){

    int cuantitat=furyans(me()).size();
    Pa mirat=Pa(rows(),vector<bool>(cols(),false));
    Mat_pos pant=Mat_pos(rows(),vector<Pos>(cols(),Pos(-1,-1,-1)));
    queue<pair<Pos,int>> cua;
    cua.push(pair<Pos,int>(p_aliat,0));
    int vida=unit(cell(p_aliat).id).health;
    mirat[p_aliat.i][p_aliat.j]=true;
    int idini=cell(p_aliat).id;
    while(not cua.empty()){
      Pos act=cua.front().first;
      int distAct=cua.front().second;
      cua.pop();
      if(cell(act).id !=-1 and unit(cell(act).id).player!=me() and unit(cell(act).id).player>=0 and (unit(cell(act).id).type==Pioneer or (unit(cell(act).id).type==Furyan))){
          trobarmillor(act,cua,vida,distAct);

          return trobar_pos(pant,act,p_aliat);
      }

      for(int i=0; i<8; ++i){
        Pos sig=act+Dir(i);

        //es pot afegir condicio de que no l ataqui cap furyan en la cela que esta si no el pot matar ja que si mata es trade i worth i si no no
        if(not mirat[sig.i][sig.j] and cell(sig).type!=Rock and not(cell(sig).type==Elevator and cell(sig+Up).id!=-1 and unit(cell(sig+Up).id).type!=Necromonger and unit(cell(sig+Up).id).player==me()) and not Pos_atacades_h[sig.i][sig.j] and not furyan_mele_f(sig,vida)   and not(Pos_atacades_h[act.i][act.j] and cell(sig).id !=-1)  and (fenvoltants(sig)<2 or (cell(sig).id!=-1 and unit(cell(sig).id).player!=me())) and not(furyan_mele_f(act,vida) and cell(sig).id!=-1) and not(cell(sig).id!=-1 and unit(cell(sig).id).player==me())){
          mirat[sig.i][sig.j]=true;
          cua.push(pair<Pos,int>(sig,distAct+1));
          pant[sig.i][sig.j]=act;
        }

      }

    }

    return p_aliat;


}

bool atacarmele(Pos& pini, Moviments& moviments, int id){
  int vida=unit(id).health;
  if(vida<40){
      movInfo mov;
      mov.id=id;
      mov.pini=pini;
      mov.tipus=Furyan;
      mov.escapar=false;
      mov.compe=false;
      bool trobat=false;
      Dir millordir;
      int milloreval=0;

      for(int i=0; i<8; ++i){
      Pos sig=pini+Dir(i);
      if(pos_ok(sig) and cell(sig).type!=Rock and cell(sig).id !=-1 and unit(cell(sig).id).player!=me() and unit(cell(sig).id).type==Furyan and unit(cell(sig).id).health<=40 and not Pos_atacades_h[pini.i][pini.j]){
        int auxeval=eval(sig,vida);
        if(not trobat){
          millordir=Dir(i);
          milloreval=auxeval;
          trobat=true;
        }else if(auxeval>milloreval){
          millordir=Dir(i);
          milloreval=auxeval;
        }

      }
    }
    if(trobat){
      mov.d=millordir;
      moviments.push_back(mov);

    }
    return trobat;
  }
  return false;

}

void ultimaOpcio(Pos& pini, Moviments& moviments, int id){
  movInfo mov;
  mov.id=id;
  mov.pini=pini;
  mov.tipus=Furyan;
  mov.escapar=false;
  mov.compe=false;
  bool trobat=false;
  Dir millordir;
  int milloreval=0;
  int vida=unit(id).health;
  for(int i=0; i<8; ++i){
    Pos sig=pini+Dir(i);
    if(cell(sig).type!=Rock and cell(sig).id !=-1 and unit(cell(sig).id).player!=me() and  (unit(cell(sig).id).type==Furyan or unit(cell(sig).id).type==Pioneer )){
      int auxeval=eval(sig,vida);
      if(not trobat){
        millordir=Dir(i);
        milloreval=auxeval;
        trobat=true;
      }else if(auxeval>milloreval){
        millordir=Dir(i);
        milloreval=auxeval;
      }

    }
  }
  if(trobat)mov.d=millordir;
  else{
    //fer que no es suicidi i ja randpr_permutation i la primera
    mov.d=Dir(random(0,7));
  }
  moviments.push_back(mov);
}

void escaparEnv(Pos& pini,int id, Moviments & moviments){
  bool trobat=false;
  for(int i=0; i<8 and not trobat;++i){
    Pos seg=pini+Dir(i);
    Cell cseg=cell(seg);
    if(cseg.id!=-1 and unit(cseg.id).type==Furyan and unit(cseg.id).player!=me() and unit(cseg.id).health<=25){
     movInfo mov;
     mov.id=id;
     mov.pini=pini;
     mov.d=Dir(i);
     mov.tipus=Furyan;
     mov.escapar=false;
     mov.compe=false;
     trobat=true;
     moviments.push_back(mov);
    }
  }
  //no hi ha easy trade s'escapa
  if(not trobat){
     movInfo mov;
     mov.id=id;
     mov.pini=pini;
     mov.tipus=Furyan;
     mov.escapar=true;
     mov.compe=false;
    bool trobatesc=false;
    int minenv=8;
      for(int i=0; i<8;++i){
    Pos seg=pini+Dir(i);
    Cell cseg=cell(seg);
    if(cseg.id==-1 and fenvoltants(pini)<minenv and cseg.type!=Rock){
     mov.d=Dir(i);
     trobatesc=true;
    }
  }
  if(trobatesc)moviments.push_back(mov);
  }

}


void Mou_Furyans(Moviments& moviments){
  vector<int>Furyans=furyans(me());
  for(int furyan : Furyans){
      Pos pini=unit(furyan).pos;
      if(fenvoltants(pini)>1)escaparEnv(pini,furyan,moviments);
      else if(not atacarmele(pini, moviments,furyan)){
        Pos a=Calcular_dist_enemic(pini);

        if(a!=pini)fer_mov(furyan,a,pini,moviments);
        else ultimaOpcio(pini,moviments,furyan);
        if(cell(a).id==-1 or (cell(a).id!=-1 and unit(cell(a).id).health<=25))Pos_atacades_h[a.i][a.j]=true;
      }



  }


}

void Mou_Pioners(Moviments& moviments){
  vector<int>Pioners=pioneers(me());//retorna un vector amb els meus pioners
  vector<int>Pcova;
  int mida=Pioners.size();
  for(int id : Pioners){

    if(unit(id).pos.k==1)Mou_Pioners_fora(id,moviments);
    else Pcova.push_back(id);
  }
  for(int id : Pcova){
    Mou_Pioners_Cova(id,moviments);
  }

}


//prespectiva a pioner b Furyan
//pre : a es un pioner i b es un Furayan
 bool cmpPiFu(const movInfo& a, const movInfo& b){
    Cell finala=cell(a.pini+a.d);
    Cell finalb=cell(b.pini+b.d);
  if(120-round()<=40){
    if(finala.gem and a.compe)return true;
    else if(finalb.id!=-1 and unit(finalb.id).health<=30)return false;
  }else{
    if(finalb.id!=-1 and unit(finalb.id).health<=30)return false;
    else if(finala.gem and a.compe)return true;
  }
  if(a.escapar or b.escapar) return a.escapar;
  else if(finalb.id!=-1)return false;
  else if(a.compe)return true;
  return true;
}



//millorar per tenir en conte de quina unitat ens escapem ja que els Hellhounds es mouen al final
//fer que pegui el que te menys vida abans
 bool comparar(const movInfo& a, const movInfo& b){
//fer totes les comperacions necessaries
    if(a.escapar and b.escapar){
    	if(unit(a.id).health==unit(b.id).health){
//           if(unit(a.id).type==unit(b.id).type) fer que s'escapi primer del que es mou mes abans que jo
          return unit(a.id).type==Furyan;
        }
    	return unit(a.id).health<unit(b.id).health;
    } //si els 2 son de escapar que s'escapi primer el que te menys vida mirar si s'escapen de f o h o n
    Cell finala=cell(a.pini+a.d);
    Cell finalb=cell(b.pini+b.d);


    if(a.tipus==Furyan and b.tipus==Furyan){
      //cas que els 2 son de pegar-se
      if(finala.id!=-1 and finalb.id!=-1){
        Unit unitata=unit(finala.id);
        Unit unitatb=unit(finala.id);
        if(unitata.health<=50 and unitatb.health<=50){
          if((unitata.health<=30 and unitatb.health<=30)){
            if(unitata.type == unitatb.type ){
              if(unitata.type ==Furyan)return(unit(a.id).health<unit(b.id).health) or (unit(b.id).health>=50 and finala.id<finalb.id);
              else return finala.id<finalb.id;
            }else return unitata.type==Furyan;
          }else if( unitata.type ==unitatb.type){
            if(unitata.type==Furyan){
              if(unit(a.id).health>50 and unit(b.id).health>50)return unitata.health<unitatb.health;
              return (unit(b.id).health>50) or (unit(a.id).health<50 and finala.id<finalb.id);
            }return unitata.health<unitatb.health;

          }else{
            if((unit(a.id).health<50 and  unitata.type==Furyan) or (unit(b.id).health<50 and  unitatb.type==Furyan))return unit(a.id).health<50 and  unitata.type==Furyan;
            else return unitata.health<unitatb.health;

          }
        }else if(unitata.type == unitatb.type){
          if(unitata.type==Furyan){
             if(unit(a.id).health>50 and unit(b.id).health>50)return unitata.health<unitatb.health;
             return (unit(b.id).health>50) or (unit(a.id).health<50 and finala.id<finalb.id);
          }return unitata.health<unitatb.health;
        }else return unitata.health<unitatb.health or(unitata.health==unitatb.health and unitata.type==Furyan);
        //cuan son del mateix tipus pioneer pioneer o fuaryan furaryan anar per el del tiu que va millor per putjar-lo
                //cas contrari anem a perunitata.health<unitatb.health or(unitata.health==unitatb.health and  el furaryan ja que ens ajudara a matar mes i evitem que ens pegui
      }else if(a.escapar!=b.escapar ){
        if(finala.id!=-1 or finalb.id!=-1){
          if(finala.id!=-1 and unit(finala.id).health<=30 and (unit(finala.id).type==Furyan or unit(b.id).health>50))return true;
          else if(finalb.id!=-1 and unit(finalb.id).health<=30 and (unit(finalb.id).type==Furyan or unit(a.id).health>50))return false;//or (cell(finala).id!=-1 and )//si son escapar i un altre de matar nomes te prio el de matar si es insta kill
        }
        return a.escapar;
      }else{//arriben aqui quan un es de pegar i l'altre es default te prefernncia el de pegar o son els 2 defaults
        if(finala.id!=-1 or finalb.id!=-1)return finala.id!=-1;
        return true;
      }
    }else if(a.tipus==Pioneer and b.tipus==Pioneer){
      if(finala.gem and finalb.gem){
        return a.compe;
      }else if(a.escapar!=b.escapar){
        if(120-round()<=40 and ((a.compe and finala.gem) or (b.compe and finalb.gem))){
          return a.compe and finala.gem;
        }else return a.escapar;
        return a.escapar;
      }else{
        if((a.compe and finala.gem)or (b.compe and finalb.gem))return a.compe and finala.gem;
        if(a.compe or b.compe)return a.compe;
        return true;
      }
    }else{
      if(a.tipus==Pioneer)return cmpPiFu(a,b);
      return not cmpPiFu(b,a);

      //ficar tots els casos de furrryan pioneer i pioneer furrryan

    }
  }
// void coutmovs(const Moviments& moviments){
//
//      int id;
//      Pos pini;
//      Dir d;
//      UnitType tipus;
//      bool escapar;
//      bool compe;
//   for(movInfo a : moviments){
//     cerr<<"pini "<<a.id<<a.pini<<" tipus "<<a.tipus<<" escapar "<<a.escapar<<" compe "<<a.compe <<" pifnal "<<a.pini+a.d<<endl;
//
//   }
//   cerr<<"acaba"<<endl;
// }

void insSort(Moviments& moviments){
  int n= moviments.size();
  for(int i=1;i<n; ++i){
    for(int j=i; j>0 and comparar(moviments[j],moviments[j-1]);--j){
      swap(moviments[j-1],moviments[j]);
    }
  }
}
void fer(Moviments & moviments){

  insSort(moviments);//mirar be com va
//   cerr<<"despres"<<endl;

  for(movInfo a : moviments){
    command(a.id,a.d);
  }

}



virtual void play () {
  Moviments moviments=Moviments();
  MapaPosDist=map<Pos,pair<int,int>>();
  modificar_gemes();
  llegir_grid();
  padalt=0;
  dvu=set<int>();
  Qfuryans=furyans(me()).size();
  mirar_p_adalt();
  Pos_atacades_eval=Pb(rows(),vector<int>(cols(),1));
  Pos_atacades_h=Pa(rows(),vector<bool>(cols(),false));
  Pos_atacades_n=Pa(rows(),vector<bool>(cols(),false));
  Pos_atacades_f=Pa(rows(),vector<bool>(cols(),false));
  atacs_n();
  atacs();
  Mou_Furyans(moviments);
  Mou_Pioners(moviments);
  fer(moviments);
  posiniS+=2;
  posiniS=posiniS%cols();



}

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
