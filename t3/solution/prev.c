#include <stddef.h>
#include <stdio.h>

#include "prev.h"

void asignarPrev(Nodo *t, Nodo **pprev) {
  if(t == NULL){//si el nodo actual es nulo, estamos en el caso base
    return;
  }

  //recorrido in orden
  asignarPrev(t -> izq, pprev); //recorrido recursivo por el subarbol izq
  
  t -> prev = *pprev; //el prev del nodo actual es el nodo que apunta pprev
  t -> prox = NULL; //momentaneamente se le asigna al nodo actual el ptr NULL como prox

  if(*pprev != NULL){ //si es que prev del nodo no es nulo
    Nodo *prev_ptr = *pprev; //se asigna el nodo previo a una variable
    prev_ptr -> prox = t; //el nodo actual es el prox del nodo previo
  }

  *pprev = t; //antes de continuar el recorrido se define el nuevo pprev

  asignarPrev(t -> der, pprev); //recorrido recursivo por el subarbol der

}//end 
