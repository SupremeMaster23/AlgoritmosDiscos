//
//  main.cpp
//  TareaDiscos
//
//  Created by Gabriela Diaz Reynaud on 11/3/22.
//

#include <iostream>
#include <vector>
#include <pthread.h>
#include <mutex>
#include <unistd.h>
#include <time.h>
#include "Disco.cpp"
using namespace std;
pthread_mutex_t  permiso1;
Disco disc;
int indice;
bool terminar= true;
void * InsertarPila(void*){
    srand((unsigned)time(0));
    int pistaN=0;
    while(terminar){
    pthread_mutex_lock(&permiso1);
        pistaN= rand()%disc.nPistas + 1;
        disc.planificacion.push_back(pistaN);
    pthread_mutex_unlock(&permiso1);
    usleep(1000000);
    }
    return NULL;
}

void * planificarFIFO(void*){
    while (terminar) {
        
        usleep(6000000);
        pthread_mutex_lock(&permiso1);
        vector<int> ordenando = disc.planificacion;
        disc.planificacion.clear();
        disc.terminado.clear();
        pthread_mutex_unlock(&permiso1);
        for(int x=0; x<ordenando.size(); x++){
            disc.terminado.push_back(ordenando[x]);
        }
    }
    return NULL;
}
bool containsF(vector<int> used , int num){
    for(int x=0; x<used.size(); x++){
        if(num == used.at(x)){
            return true;
        }
    }
    return false;
}
void * planificarSSTF(void*){
    while (terminar) {
        usleep(6000000);
          pthread_mutex_lock(&permiso1);
          vector<int> ordenando = disc.planificacion;
          vector<int> usados;
          disc.planificacion.clear();
          disc.terminado.clear();
          pthread_mutex_unlock(&permiso1);
          int indiceInicio = ordenando.at(ordenando.size()-1);
          int min = disc.nPistas+1;
        int cabexa=indiceInicio;
          int y=0;
        usados.push_back(indiceInicio);
        while( y<ordenando.size()){
       
            for(int x=0; x<ordenando.size(); x++){
                int tmp = indiceInicio-ordenando.at(x);
                if(tmp<0){
                    tmp=-1*tmp;
                }
                if(ordenando.at(x)==indiceInicio){
                    disc.terminado.push_back(ordenando.at(x));
                    y++;
                } else  {
                    if(tmp<=min && containsF(usados,ordenando.at(x))==false){
                        cabexa =ordenando.at(x);
                        min=tmp;
                    }
                }
            }     
            indiceInicio=cabexa;
            usados.push_back(cabexa);
            min = disc.nPistas+1;
           
    }
  
}
    return NULL;
}


void * planificarSCAN(void*){
    while(terminar){
     usleep(6000000);
      pthread_mutex_lock(&permiso1);
      vector<int> ordenando = disc.planificacion;
      vector<int> usados;
      bool contrario = false;
      disc.terminado.clear();
      disc.planificacion.clear();
      pthread_mutex_unlock(&permiso1);
        int indiceInicio = ordenando.at(ordenando.size()-1);
        int originial = indiceInicio;
        int y=0;
        usados.push_back(indiceInicio);
      while(y<ordenando.size()){
      
          for(int x=0; x<ordenando.size(); x++){
              if(indiceInicio==ordenando.at(x)){
                  disc.terminado.push_back(indiceInicio);
                  
                  y++;
              }
              
          }
          if(indiceInicio==1){
              int min= disc.nPistas+1;
              int cercano=indiceInicio;
              for(int x=0; x<ordenando.size(); x++){
                  int tmp = originial-ordenando.at(x);
                  if(tmp<0){
                      tmp=-1*tmp;
                  }
                  if(tmp<=min && containsF(usados,ordenando.at(x))==false){
                          min=tmp;
                          cercano =ordenando.at(x);
                      }
                  }
              indiceInicio=cercano;
              contrario=true;
          } else if(contrario!=true){
              indiceInicio--;
              usados.push_back(indiceInicio);
          } else if (contrario==true){
              indiceInicio++;
              usados.push_back(indiceInicio);
          }
      }
    }
    return NULL;
}


void * print(void*){
    while (terminar) {
        pthread_mutex_lock(&permiso1);
        vector<int> ordenando = disc.terminado;
        printf("Ingresos : ");
        vector<int> plani = disc.planificacion;
        pthread_mutex_unlock(&permiso1);
        for(int x=0; x<plani.size() ; x++){
            printf("%d ", plani.at(x));
        }
        printf("\n");
        
        printf("Planificacion :");
        for(int x=0; x<ordenando.size() ; x++){
            printf("%d ", ordenando.at(x));
        }
     
        printf("\n\n");
        usleep(1000000);
    }
    
    return NULL;
}

void * temporizador (void *){
    usleep(60000000);
    terminar = false;
    return NULL;
}

void iniciarDisco(){
    int nPistas=0;
    int opcionA=0;
    cout<<"Numero de Pistas"<<endl;
    cin>>nPistas;
    cout<<"Algoritmos------\n1.FIFO\n2.SSTF\n3.SCAN\n";
    cout<<"Algoritmo"<<endl;
    cin>>opcionA;
    disc.nPistas= nPistas;
    disc.opcionA = opcionA;
}
int main(int argc, const char * argv[]) {
    iniciarDisco();
    if(pthread_mutex_init(&permiso1,NULL)){
        
    }
    pthread_t imprimir;
    pthread_t injection;
    pthread_t algoritmo;
    pthread_t temporizar;

    switch(disc.opcionA){
        case 1:
            pthread_create(&temporizar, NULL, temporizador, NULL);
            pthread_create(&injection, NULL, InsertarPila, NULL);
            pthread_create(&algoritmo, NULL, planificarFIFO, NULL);
            pthread_create(&imprimir, NULL, print, NULL);
            pthread_join(imprimir, NULL);
            pthread_join(injection, NULL);
            pthread_join(algoritmo, NULL);
            pthread_join(temporizar, NULL);
            
            break;
        case 2:
            pthread_create(&temporizar, NULL, temporizador, NULL);
            pthread_create(&injection, NULL, InsertarPila, NULL);
            pthread_create(&algoritmo, NULL, planificarSSTF, NULL);
            pthread_create(&imprimir, NULL, print, NULL);
            pthread_join(imprimir, NULL);
            pthread_join(injection, NULL);
            pthread_join(algoritmo, NULL);
            pthread_join(temporizar, NULL);
            
            break;
        case 3:
            pthread_create(&temporizar, NULL, temporizador, NULL);
            pthread_create(&injection, NULL, InsertarPila, NULL);
            pthread_create(&algoritmo, NULL, planificarSCAN, NULL);
            pthread_create(&imprimir, NULL, print, NULL);
            pthread_join(imprimir, NULL);
            pthread_join(injection, NULL);
            pthread_join(algoritmo, NULL);
            pthread_join(temporizar, NULL);
            
            break;
        default :
   
        break;
            
    }

}
