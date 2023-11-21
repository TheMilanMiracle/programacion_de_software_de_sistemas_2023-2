#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "viajante.h"

int leer(int fd, void *vbuf, int n) {
  char *buf= vbuf;
  do {
    int rc= read(fd, buf, n);
    if (rc<=0)
      return 1; /* fracaso: error o fin del archivo/pipe/socket */
    n-= rc; /* descontamos los bytes leídos */
    buf+= rc; /* avanzamos el buffer para no reescribir lo leido previamente */
  } while (n>0); /* mientras no leamos todo lo que esperamos */
  return 0; /* exito */
}

double viajante_par(int z[], int n, double **m, int nperm, int p) {
  int pids[p]; // arreglo para las ids de los p hijos 
  int fds[p][2]; // arreglo para las p pipes para los p hijos

  for(int i = 0; i < p; i++){ // creamos las p pipes
    pipe(fds[i]);
  }

  for(int i=0; i < p; i++){ // para p procesos
    pid_t pid =  fork(); // hacemos fork
    pids[i] = pid; //guardamos el id del proceso hijo


    if(pid == 0){//en un hijo
      close(fds[i][0]); // cerramos el canal de lectura
      int child_z[n+1]; // un arreglo para escribir al pipe

      srandom(getUSecsOfDay()*getpid()); //cambiar la semilla para que los procesos no arrojen las mismas rutas
      double min = viajante(child_z, n, m, nperm/n); //llamamos a viajante para nperm/n permutaciones

      write(fds[i][1], &min, sizeof(double)); //escribimos el minimo
      write(fds[i][1], &child_z, (n+1)*sizeof(int)); //escribimos el arreglo con el camino

      exit(0); // matamos el proceso
    }
    else{//en el padre
      close(fds[i][1]); //cerramos el canal de escritura
    }
  }

  double min = DBL_MAX; // para guardar la menor distancia
  for(int i = 0; i < p; i++){ // para revisar lo que mandaron los p hijos por las p pipes
    int x[n+1]; // buffer para leer el camino del pipe
    double child_min; // buffer para leer el minimo del pipe

    read(fds[i][0], &child_min, sizeof(double)); // leemos el minimo 
    read(fds[i][0], &x, (n+1)*sizeof(int)); // leemeos el arreglo con el camino
    close(fds[i][0]); // cerramos el canal de lectura para evitar "goteras"

    waitpid(pids[i], NULL, 0); // enterramos el hijo para evitar procesos _zombies_

    if(child_min < min){ // si leemos un valor menor del hijo, actualizamos el minimo...
      min = child_min;
      for(int j = 0; j <= n; j++){ //... y el camino relacionado
        z[j] = x[j];
      }
    }
  }

  return min; // retornamos el minimo extraido de los p procesos
}
