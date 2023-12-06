#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "pss.h"
       
// estructura que guarda el tamanno y nombre de un archivo
typedef struct fileStruct{
  int size;
  char *name;
} FileStruct;

Queue *q; // cola global para las estructuras

void fillQueue(char *name){
  struct stat st_nom;
  int rc;
  rc = stat(name, &st_nom);

  if (rc!=0) {
    printf("%s no existe\n", name);
    exit(0);
  }

  if(S_ISREG(st_nom.st_mode)){
    //archivo regular y no se hace nada
  }
  else if (S_ISDIR(st_nom.st_mode)){
    //directorio
    DIR *dir = opendir(name);
    if(dir == NULL ){
      perror(name);
      exit(1);
    }
    for(struct dirent *entry = readdir(dir); entry != NULL; entry = readdir(dir)){
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..")==0){
        continue;
      }

      char *nom_arch = malloc(strlen(name)+1+strlen(entry->d_name)+1); // pedimos memoria para el nombre/ruta del archivo
      strcpy(nom_arch, name); // añadimos la ruta
      strcat(nom_arch, "/"); 
      strcat(nom_arch, entry->d_name); // añadimos el nombre del archivo

      // estructura stat para archivo actual
      struct stat current_file;
      stat(nom_arch, &current_file);

      //estructura con nombre y tamanno
      FileStruct *file = malloc(sizeof(FileStruct)); //pedimos memoria para la estructura
      file->size = current_file.st_size; // llenamos el campo del tamanno
      file->name = malloc(strlen(nom_arch)+1); // pedimos memoria para el nombre del archivo
      strcpy(file->name, nom_arch); // llenamos el campo de la ruta/nombre el archivo

      put(q, file); // agreganmos la estructura a la cola global

      fillQueue(nom_arch); // recursivamente revisamos el siguiente archivo
      free(nom_arch); //liberamos la memoria
      }
      closedir(dir); //cerramos la carpeta luego de revisar recursivamente su contenido
    }
    else {
      //otros
      fprintf(stderr, "archivo %s es de tipo desconocido\n", name);
      exit(1);
    }
}

// definimos de que forma se comparan las estructuras creadas para los archivos
int cmpFileSize(void *ptr, int i, int j){
  FileStruct **a = ptr;
  if(a[i]->size > a[j]->size){
    return -1;
  }
  else if(a[i]->size < a[j]->size){
    return 1;
  }
  else{//si los tamannos son iguales
    return strcmp(a[i]->name, a[j]->name); // comparamos los nombres
  }
}



int main(int argc, char *argv[]) {
  if(argc != 3){ // error de argumentos del comando
    fprintf(stderr, "usage: ./mas-grandes dir_name n \n");
    exit(1);
  }

  q = makeQueue(); // creamos la cola

  fillQueue(argv[1]); // llamamos a la funcion que va a llenar la cola con los archivos

  int qlen = queueLength(q); // largo de la cola global
  FileStruct *directories[qlen]; // creamos un arreglo para almacenar el contenido de la cola
  for(int i=0; i<qlen;i++){ //llenamos el arreglo
    directories[i] = get(q);
  }
  destroyQueue(q); // destruimos la cola

  sortPtrArray(&directories, 0, qlen-1, cmpFileSize); // ordenamos el arreglo usando la funcion definida de comparar la estructura
  
  int n = atoi(argv[2]); //extraemos n de los argumentos
  for(int i=0; i < n; i++){ // para los primeros n elementos del arreglo ordenado
    FileStruct *file = directories[i]; // extraemos la estructura
    printf("%s %d\n",file->name, file->size); // imprimimos el archivo
  }

  for(int i=0; i < qlen; i++){//liberamos la memoria de los punteros en el arreglo
    FileStruct *file = directories[i]; // extraemos la estructura
    free(file->name); // liberamos la memoria usada para el campo del nombre de la estructura
    free(file); // liberamos la memoria de la estructura
  }

  return 0; // final del programa
}
