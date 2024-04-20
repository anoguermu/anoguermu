/** @mainpage En aquesta pràctica hem creat una simulació del funcionament d'una arquitectura multiprocessador, on cada processador treballa en paral·lel, ja que té processos en la seva pròpia memòria i pot executar diversos a la vegada.

El programa principal comença inicialitzant un primer clúster sense processos i a continuació inicialitza una àrea d'espera amb n prioritats
un cop ja tot inicialitzat començarà a executar una sèrie de comandes fins que amb el comando fi acabi l'execució.

*/

/**@file program.cc
   @brief Programa principal
 */

#include "Cluster.hh"
#include "Area_Processos.hh"

#ifndef NO_DIAGRAM
#include <iostream>
#endif
using namespace std;


int main (){
    Cluster cluster;
    cluster.Config_Cluster();
    Area_processos area;
    string comanda;
    cin >> comanda;
    while (comanda != "fin") {
    	
        if (comanda == "cc" or comanda == "configurar_cluster") {
            cout<<"#"<<comanda<<endl;
            cluster.Config_Cluster();
            
        }

        else if (comanda == "mc" or comanda == "modificar_cluster") {
       	    string id_processador;
       	    cin>>id_processador;
       	    cout<<"#"<<comanda<<" "<<id_processador<<endl;
            cluster.modificar_Cluster(id_processador);
        }

        else if (comanda == "ap" or comanda == "alta_prioridad") {
            string prio;
            cin>>prio;
            cout<<"#"<<comanda<<" "<<prio<<endl;
            area.afegir_prio(prio);
        }

        else if (comanda == "bp" or comanda == "baja_prioridad") {
            string prio;
            cin>>prio;	
            cout<<"#"<<comanda<<" "<<prio<<endl;
            area.eliminar_prio(prio);
        }

        else if (comanda == "ape" or comanda == "alta_proceso_espera") {
            string prioritat;
            cin>>prioritat;
            Proces pro;
            pro.llegir();
            cout<<"#"<<comanda<<" "<<prioritat<<" "<<pro.Consultar_id()<<endl;

            
            
            area.afegir_proces(prioritat, pro);
        }

        else if (comanda == "app" or comanda == "alta_proceso_procesador") {
            string id_processador;
            cin>>id_processador;
            Proces pro;
            pro.llegir();
            cout<<"#"<<comanda<<" "<<id_processador<<" "<<pro.Consultar_id()<<endl;
            cluster.alta_proces_Procesador(id_processador,pro);
        }

        else if (comanda == "bpp" or comanda == "baja_proceso_procesador") {
            string id_processador;
            int id_proces;
            cin>>id_processador>>id_proces;
            cout<<"#"<<comanda<<" "<<id_processador<<" "<<id_proces<<endl;
            cluster.baixa_proces_Procesador(id_processador,id_proces);
        }

        else if (comanda == "epc" or comanda == "enviar_procesos_cluster") {
            int n;
            cin>>n;
            cout<<"#"<<comanda<<" "<<n<<endl;
            area.enviar_procesos_cluster(cluster,n);
        }

        else if (comanda == "at" or comanda == "avanzar_tiempo") {
            int n;
            cin>>n;
            cout<<"#"<<comanda<<" "<<n<<endl;
            cluster.modificar_temps(n);
        }

        else if (comanda == "ipri" or comanda == "imprimir_prioridad") {
            string prio;
            cin>>prio;
            cout<<"#"<<comanda<<" "<<prio<<endl;
            area.imprimir_prio(prio);
        }

        else if (comanda == "iae" or comanda == "imprimir_area_espera") {
            cout<<"#"<<comanda<<endl;
            area.imprimir_area_espera();
        }

        else if (comanda == "ipro" or comanda == "imprimir_procesador") {
            string id_procesador;
            cin>>id_procesador;
              cout<<"#"<<comanda<<" "<<id_procesador<<endl;
            cluster.imprimir_procesador(id_procesador);
        }
        else if (comanda == "ipc" or comanda == "imprimir_procesadores_cluster") {
              cout<<"#"<<comanda<<endl;
            cluster.imprimir_procesadors_cluster();
        }

        else if (comanda == "iec" or comanda == "imprimir_estructura_cluster") {
            cout<<"#"<<comanda<<endl;
            cluster.imprimir_estructura_cluster();
        }

        else if (comanda == "cmp" or comanda == "compactar_memoria_procesador") {
            string id_procesador;
            cin>>id_procesador;
            cout<<"#"<<comanda<<" "<<id_procesador<<endl;
            cluster.compactar_mem_processador(id_procesador);
        }

        else if (comanda == "cmc" or comanda == "compactar_memoria_cluster") {
            cout<<"#"<<comanda<<endl;
            cluster.compactar_mem();
        }

        cin >> comanda;
    }
}

