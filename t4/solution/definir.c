#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "pss.h"

//variables globales
FILE *f; //puntero a un archivo
int line_size;// cada linea del archivo tiene 80 caracteres mas un salto de linea
int n; //entero que representa el indice de las lineas del archivo diccionario


int main(int argc, char *argv[]) {
  //argv[1]: nombre del archivo txt
  //argv[2]: llave a ingresar en el hash
  //argv[3]: descripcion de la llave

  if(argc != 4){//si es que se ingresaron mas o menos de tres argumentos (lo correcto son 3)
    fprintf(stderr, "Uso: ./definir <diccionario> <llave> <definicion>\n"); //se imprime el error en la salida estandar de erorres
    exit(1); //se sale del programa con codigo 1
  }

  f = fopen(argv[1], "r+"); //se abre el archivo con el diccionario

  if(!f){// si hay algun error con el archivo del diccionario
    perror(argv[1]); //se le muestra el error al usuario
    exit(1); // se sale del programa con codigo 1
  }

  //para calcular el tamaño de una linea
  line_size = 1; //se parte desde uno, pues
  char c[1]; //buffer para los caracteres
  fread(c, 1, 1, f); //se lee un primer caracter
  while(c[0] != '\n'){ //mientras no se llegue al salto de linea
    fread(c, 1, 1, f); //se van leyendo caracteres
    line_size++;//se aumenta el contador
  }

  fseek(f, 0, SEEK_END);//se pone el puntero al archivo en su final
  long int file_size = ftell(f);//se determina el tamaño del archivo
  int file_lines = (file_size / line_size); //la cantidad de lineas se calcula de esta manera

  n = hash_string(argv[2]) % file_lines; //se calcula la linea en la que estara la definicion segun la f. de hash
  
  char buff[line_size]; //buffer para guardar las lineas del archivo
  fseek(f, n * line_size, SEEK_SET); //se lleva el puntero del archivo a la linea donde debiese estar la definicion
  fread(buff, line_size, 1, f); //se lee la linea, con '\n' incluido,se copia en el buffer y el puntero avanza a la sigte linea

  int j = file_lines;//variable util para saber cuando ya se leyo todo el archivo
  while(--j){//mientras aun no se recorra todo el diccionario
    if(buff[0] == ' '){//si la linea se encuentra vacia (basta con el primer caracter de la linea que sea un espacio), significa que se puede agregar aqui la definicion
      fseek(f, n* line_size, SEEK_SET); //se lleva el puntero del archivo al inicio de la linea

      char new[line_size]; //string para la nueva linea que se reemplazara en el archivo
      char *key = argv[2], *desc = argv[3]; //strings a copiar en la linea en formato "key:desc   \n"
      int key_len = strlen(key), desc_len = strlen(desc); //largos de estos strings
      
      int i = 0;//variable que recorrera todo el nuevo string para escribirlo

      for(i = 0; i < key_len; i++){//ciclo for para pegar la key en el nuevo string
        new[i] = *key; //se pega el caracter
        key++; //se avanza para pegar el sigte caracter
      }

      new[i] = ':'; //se agregan los dos puntos

      for(i = key_len + 1; i < key_len + desc_len + 1; i++){//ciclo for para pegar la desc en el nuevo string
        new[i] = *desc;//se pega el caracter
        desc++;//se avanza para pegar el sigte caracter
      }

      while(i < line_size-1){//ciclo while para llenar el resto de los 80 caracteres con espacios
        new[i] = ' '; //se pega el espacio
        i++; //se avanza el iterador que recorre el string
      }

      new[i] = '\n'; //agregamos el salto de linea en el ultimo espacio del string

      fwrite(new, line_size, 1, f); //pegamos la nueva linea en el archivo
    
      break;//se rompe el while, pues ya se hizo la definicion
    }
    else{//si la linea ya esta ocupada hay que revisar la siguiente
      if(strncmp(buff, argv[2], strlen(argv[2])) == 0){//si ya se encuentra la llave en el diccionario
        fprintf(stderr, "La llave %s ya se encuentra en el diccionario\n", argv[2]); //se imprime el error en la salida estandar de erorres
        exit(1); //se sale del programa con codigo 1
      }
      
      n++; //para saltar a la siguiente linea del archivo
      n %= file_lines; //se asegura que el archivo se navega circularmente
      
      fread(buff, line_size, 1, f); //se lee la siguiente linea en el buffer y se avanza el puntero
    }
  }

  if(j == 0){//en caso de que se haya recorrido todo el dicc (pues estaba lleno)
    fprintf(stderr, "%s: el diccionario esta lleno\n", argv[1]); //se imprime el error en la salida estandar de erorres
    exit(1); //se sale del programa con codigo 1
  }

  fclose(f);//se cierra el archivo
  return 0;//final del programa
}
