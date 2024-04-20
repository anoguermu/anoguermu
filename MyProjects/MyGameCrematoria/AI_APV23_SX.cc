#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME APV23_P8


// DISCLAIMER: The following Demo player is *not* meant to do anything
// sensible. It is provided just to illustrate how to use the API.
// Please use AINull.cc as a template for your player.


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

  typedef vector<int> VI;

  map<int, int> kind; // For pioneers: 0 -> random, 1 -> cyclic.




  //////////////////////BFS GOS///////////////////////
  pair<Pos,int> BFS_gos(Pos posicio) {
    queue<pair<Pos,int>> q;
    //Cua amb els nodes
    vector<vector<bool>> visitats (40, vector<bool>(80, 0));  //Matriu de visitats
    visitats[posicio.i][posicio.j] = 1;                      //Node inical visitat

    Pos temporal;
    for(int x = 0; x < 8; ++x) {
      temporal = posicio + Dir(x);
      if(pos_ok(temporal)) { //Posició correcta i sense ningú
        pair<Pos,int> valor (temporal, x);
        q.push(valor);
      }
    }

    while(!q.empty()) {       //Mentres no estigui empty
      pair<Pos,int> valor = q.front();
      q.pop();

      int id_psc = cell(valor.first).id;  //Id de la posicio (-1 si no hi ha res)

      //Mirar que hi hagi algu, que sigui un furyen enemic o que sigui un hellhound
      if (id_psc != -1 and unit(id_psc).type == Hellhound)
        return valor;

      Pos psc_temp;
      for(int x = 0; x < 8; ++x) {
        psc_temp = valor.first + Dir(x); //Calcular nova posicio;
        if(pos_ok(psc_temp)) {
          if(!visitats[psc_temp.i][psc_temp.j]) {
            pair<Pos,int> valor2 (psc_temp, valor.second); //Copia de nova posicio i dir inicial
            q.push(valor2);
            visitats[psc_temp.i][psc_temp.j] = 1;
          }
        }
      }
    }
    return pair<Pos,int>(Pos(-1, -1, -1),10);
  }


  //////////////////////MIRAR DISTANCIA.F///////////////////////
  pair<bool, Dir> escapar_fury(Pos posicio) {  //Si distancia menor a x retornar millor direccio i bool = 1
    pair<Pos,int> Enemic = BFS_gos(posicio);
    Pos posicio2 = Enemic.first;
    int distancia;

    int difi = posicio.i - posicio2.i;
    int difj = posicio.j - posicio2.j;
    if(difi < 0) difi*=-1;
    if(difj < 0) difj*=-1;
    if(difi >= difj) distancia = difi;
    else distancia = difj;

    if(distancia < 5) { //Millorar
      int dir_cont = (Enemic.second + 4)%8;
      //Mirar si la direccio no esta disponible

      if(cell(posicio + Dir(dir_cont)).type == Rock or cell(posicio + Dir(dir_cont)).id != -1) {
        for(int x = 1; x < 4; ++x) {
          int dir_dreta = (dir_cont+x)%8;
          int dir_esque = (dir_cont-x)%8;
          if(cell(posicio + Dir(dir_dreta)).type != Rock or cell(posicio + Dir(dir_dreta)).id == -1)
            return pair<bool, Dir> (1, Dir(dir_dreta));

          else if(cell(posicio + Dir(dir_esque)).type != Rock or cell(posicio + Dir(dir_esque)).id == -1)
            return pair<bool, Dir> (1, Dir(dir_esque));
        }
        return pair<bool, Dir> (0, None);
      }
      //Direccio disponible
      else return pair<bool, Dir> (1, Dir(dir_cont));
    }
    return pair<bool, Dir> (0, None);

  }

  //////////////////////BFS FURYENS///////////////////////
  pair<Pos,Dir> BFS_furyen(Pos posicio) {
    queue<pair<Pos,Dir>> q;
    //Cua amb els nodes
    vector<vector<bool>> visitats (40, vector<bool>(80, 0));  //Matriu de visitats
    visitats[posicio.i][posicio.j] = 1;                      //Node inical visitat

    Pos temporal;
    for(int x = 0; x < 8; ++x) {
      temporal = posicio + Dir(x);
      if(cell(temporal).type != Rock) { //Posició correcta i sense ningú
        pair<Pos,Dir> valor (temporal, Dir(x));
        q.push(valor);
      }
    }
    while(!q.empty()) {       //Mentres no estigui empty
      pair<Pos,Dir> valor = q.front();
      q.pop();

      int id_psc = cell(valor.first).id;  //Id de la posicio (-1 si no hi ha res)
      if (id_psc != -1 and unit(id_psc).player != me() and (unit(id_psc).type == Pioneer or unit(id_psc).type == Furyan)) return valor;
      Pos psc_temp;
      for(int x = 0; x < 8; ++x) {
        psc_temp = valor.first + Dir(x); //Calcular nova posicio;
        if(cell(psc_temp).type != Rock) {
          if(!visitats[psc_temp.i][psc_temp.j]) {
            pair<Pos,Dir> valor2 (psc_temp, valor.second); //Copia de nova posicio i dir inicial
            q.push(valor2);
            visitats[psc_temp.i][psc_temp.j] = 1;
          }
        }
      }
    }
    return pair<Pos,Dir>(Pos(-1, -1, -1),None);
  }

  //////////////////////MOURE FURYENS///////////////////////
  void move_furyans() {
    VI F = furyans(me());
    int n = F.size();
    VI perm = random_permutation(n);
    for (int i = 0; i < n; ++i) {
      // id is an own furyan. For some reason (or not) we treat our furyans in random order.

      int id = F[perm[i]];
      Pos pos = unit(id).pos;
      pair<bool, Dir> perill = escapar_fury(pos);
      if(perill.first) {
        command(id, perill.second);
      }
      else {
        pair<Pos,Dir> atacar = BFS_furyen(pos);
        if(atacar.first != Pos(-1, -1, -1)) {
          command(id, atacar.second);
        }
      }
    }
  }

  //////////////////////BFS ENEMIC///////////////////////
  pair<Pos,int> BFS_enemic(Pos posicio) {
    queue<pair<Pos,int>> q;
    //Cua amb els nodes
    vector<vector<bool>> visitats (40, vector<bool>(80, 0));  //Matriu de visitats
    visitats[posicio.i][posicio.j] = 1;                      //Node inical visitat

    Pos temporal;
    for(int x = 0; x < 8; ++x) {
      temporal = posicio + Dir(x);
      if(pos_ok(temporal)) { //Posició correcta i sense ningú
        pair<Pos,int> valor (temporal, x);
        q.push(valor);
      }
    }

    while(!q.empty()) {       //Mentres no estigui empty
      pair<Pos,int> valor = q.front();
      q.pop();

      int id_psc = cell(valor.first).id;  //Id de la posicio (-1 si no hi ha res)

      //Mirar que hi hagi algu, que sigui un furyen enemic o que sigui un hellhound
      if (id_psc != -1 and ((unit(id_psc).player != me() and unit(id_psc).type == Furyan) or (unit(id_psc).type == Hellhound)))
        return valor;

      Pos psc_temp;
      for(int x = 0; x < 8; ++x) {
        psc_temp = valor.first + Dir(x); //Calcular nova posicio;
        if(pos_ok(psc_temp)) {
          if(!visitats[psc_temp.i][psc_temp.j]) {
            pair<Pos,int> valor2 (psc_temp, valor.second); //Copia de nova posicio i dir inicial
            q.push(valor2);
            visitats[psc_temp.i][psc_temp.j] = 1;
          }
        }
      }
    }
    return pair<Pos,int>(Pos(-1, -1, -1),10);
  }


  //////////////////////MIRAR DISTANCIA///////////////////////
  pair<bool, Dir> escapar(Pos posicio) {  //Si distancia menor a x retornar millor direccio i bool = 1
    pair<Pos,int> Enemic = BFS_enemic(posicio);
    Pos posicio2 = Enemic.first;
    int distancia;
    int difi = posicio.i - posicio2.i;
    int difj = posicio.j - posicio2.j;
    if(difi < 0) difi*=-1;
    if(difj < 0) difj*=-1;
    if(difi >= difj) distancia = difi;
    else distancia = difj;
    if(distancia < 5) { //Millorar
      int dir_cont = (Enemic.second + 4)%8;
      //Mirar si la direccio no esta disponible

      if(cell(posicio + Dir(dir_cont)).type == Rock or cell(posicio + Dir(dir_cont)).id != -1) {
        for(int x = 1; x < 4; ++x) {
          int dir_dreta = (dir_cont+x)%8;
          int dir_esque = (dir_cont-x)%8;
          if(cell(posicio + Dir(dir_dreta)).type != Rock or cell(posicio + Dir(dir_dreta)).id == -1)
            return pair<bool, Dir> (1, Dir(dir_dreta));

          else if(cell(posicio + Dir(dir_esque)).type != Rock or cell(posicio + Dir(dir_esque)).id == -1)
            return pair<bool, Dir> (1, Dir(dir_esque));
        }
        return pair<bool, Dir> (0, None);
      }
      //Direccio disponible
      else return pair<bool, Dir> (1, Dir(dir_cont));
    }
    return pair<bool, Dir> (0, None);

  }
  //////////////////////BFS CELA///////////////////////
  pair<Pos, Dir> BFS_cela(Pos posicio) {
    queue<pair<Pos,Dir>> q;
    //Cua amb els nodes
    vector<vector<bool>> visitats (40, vector<bool>(80, 0));  //Matriu de visitats
    visitats[posicio.i][posicio.j] = 1;                      //Node inical visitat

    Pos temporal;
    for(int x = 0; x < 8; ++x) {
      temporal = posicio + Dir(x);
      if(cell(temporal).type != Rock and cell(temporal).id == -1) { //Posició correcta i sense ningú
        pair<Pos,Dir> valor (temporal, Dir(x));
        q.push(valor);
      }
    }
    while(!q.empty()) {       //Mentres no estigui empty
      pair<Pos,Dir> valor = q.front();
      q.pop();
      Pos psc_temp = valor.first;
      if (cell(psc_temp).owner != me() and cell(temporal).id == -1) return valor;
      for(int x = 0; x < 8; ++x) {
        psc_temp = valor.first + Dir(x); //Calcular nova posicio;
        if(cell(psc_temp).type != Rock) {
          if(!visitats[psc_temp.i][psc_temp.j]) {
            pair<Pos,Dir> valor2 (psc_temp, valor.second); //Copia de nova posicio i dir inicial
            q.push(valor2);
            visitats[psc_temp.i][psc_temp.j] = 1;
          }
        }
      }
    }
    return pair<Pos,Dir>(Pos(-1, -1, -1),None);
  }

  //////////////////////MOURE P.COVA///////////////////////
  void move_pioneers_cova(int i) {
    VI P = pioneers(me());
    int ID = P[i];
    Pos posicio = unit(ID).pos;
    //Escaparse: posicions de distancia es lo mes gran de x.i - y.i || x.j - y.j

    pair<bool, Dir> perill = escapar(posicio);
    if(cell(posicio).type == Elevator and daylight(posicio + Up) != 1 and P.size() > 14 and nb_gems(me()) < 5) command(ID, Up);


    else if(perill.first) {
      command(ID, perill.second);
    }

    else {
      int millor_dir = 10;  //Inicialitzar amb none
      int prio = -1;        //Pitjor possibilitat
      Pos psc_temp;
      for(int x = 0; x < 8; ++x) {    //Mirar totes les direccions
        psc_temp = posicio + Dir(x);
        if(cell(psc_temp).type != Rock and cell(psc_temp).id == -1) { //Nomes moviments vàlids

          if(cell(psc_temp).type != Elevator) { //Si es cel·la de cova normal
            if(cell(psc_temp).owner == -1) {       //Casella que no es de ningu Prio=1
              if (prio < 1) {
                millor_dir = x;
                prio = 1;
              }
            }

            if(cell(psc_temp).owner != me() and cell(psc_temp).owner != -1){   //Casella que no es meva Prio=2
              if (prio < 2) {
                millor_dir = x;
                prio = 2;
              }
            }

            if (cell(psc_temp).owner == me()){  //Casella meva Prio=0;
              if (prio <0) {
                millor_dir = x;
                prio = 0;
              }
            }

          }  //final if de no ascensor

          else {                    //Casella ascencor Prio=3 !!Falta mirar llum
            if(prio < 3  and P.size() > 14 and nb_gems(me()) < 4) {
              millor_dir = x;
              prio = 3;
            }

          }  //final if ascencor
        }
      } //final miras posicions possibles


      if(prio == 0) { //en cas de nomes caselles meves, moure random
        pair<Pos,Dir> casella_propera = BFS_cela(posicio);
        if(casella_propera.first != Pos(-1, -1, -1)) command(ID, casella_propera.second);
        else command(ID, None);
      }
      else command(ID, Dir(millor_dir));
    }
  }

  //////////////////////BFS GEMA///////////////////////
  pair<Pos,Dir> BFS_gema(Pos posicio) {

    queue<pair<Pos,Dir>> q;

    //q.push(posicio);    //Cua amb els nodes
    vector<vector<bool>> visitats (40, vector<bool>(80, 0));  //Matriu de visitats
    visitats[posicio.i][posicio.j] = 1;                      //Node inical visitat
    Pos temporal;
    for(int x = 0; x < 8; ++x) {
      temporal = posicio + Dir(x);

      if(pos_ok(temporal) and cell(temporal).id == -1) { //Posició correcta i sense ningú
        pair<Pos,Dir> valor (temporal, Dir(x));
        q.push(valor);
      }
    }

    while(!q.empty()) {       //Mentres no estigui empty
      pair<Pos,Dir> valor = q.front();
      //Pos psc = q.front();
      q.pop();

      if (cell(valor.first).gem == 1) return valor;
      Pos psc_temp;
      for(int x = 0; x < 8; ++x) {
        psc_temp = valor.first + Dir(x); //Calcular nova posicio;

        if(pos_ok(psc_temp)) {
          if(!visitats[psc_temp.i][psc_temp.j]) {
            pair<Pos,Dir> valor2 (psc_temp, valor.second); //Copia de nova posicio i dir inicial
            q.push(valor2);
            visitats[psc_temp.i][psc_temp.j] = 1;
          }
        }
      }
    }
    return pair<Pos,Dir>(Pos(-1, -1, -1),None);
  }

  //////////////////////MOURE P.EXTERIOR///////////////////////
  void move_pioneers_exterior(int i) {
    int id = pioneers(me())[i];
    Pos jugador_pcs = unit(id).pos;

    pair<Pos,Dir> valors_gema = BFS_gema(jugador_pcs);
    Pos gema_pcs = valors_gema.first;
    if (gema_pcs != Pos(-1, -1, -1)) {    //Si ha trobat algna gema
      Dir direccio = valors_gema.second;
      command(id, direccio);
    }
    else command(id, Right);
  }

  //////////////////////MOURE PIONERS///////////////////////
  void move_pioneers() {
    vector<int> P = pioneers(me());
    for(long unsigned int i = 0; i < P.size(); ++i) { //Per a cada pioner
      // cerr << i << endl;
      if (unit(P[i]).pos.k == 0) move_pioneers_cova(i); //De l'interior
      else move_pioneers_exterior(i);                   //De l'exterior
    }
  }


  /**
   * Play method, invoked once per each round.
   */
  void play () {
    move_furyans();
    move_pioneers();
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

