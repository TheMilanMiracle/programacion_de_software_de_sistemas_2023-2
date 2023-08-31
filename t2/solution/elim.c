#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "elim.h"

//Esta función elimina del string str todas las apariciones del patrón pat
void eliminar(char *str, char *pat){
    int pat_len = strlen(pat); 
    char *aux = str; //se guarda una copia del puntero al string y otro a strlen(pat) espacios mas

    if(pat_len){ //si el patron es de largo 0 no hay nada que hacer
        
        while(strlen(aux) >= strlen(pat)){// mientras aun quepa el patron en lo que resta del string

            if(strncmp(aux, pat, pat_len) == 0){ // se compara un trozo del string con el patron que se quiere eliminar
                char *copy = aux + pat_len, *paste = aux; //punteros para copiar los caracteres luego del patron y moverlos
                
                while(*copy){ //mientras queden caracteres que reubicar
                    *paste = *copy; //se mueve el caracter para atras
                    copy++; // se avanza al sigte caracter 
                    paste++; // se avanza al sigte caracter
                }
                
                copy -= pat_len; //como en el while se llega al 0 final, se retrocede el puntero para mover el 0 lo que se movio del string hacia atras
                *copy = '\0'; // se marca el nuevo final del string original
                
                continue;
            }
            
            aux++; // se mueve el puntero aux del string un espacio
        }
    }
}

//Esta funcion retorna un nuevo string que es str habiendole eliminado el patron pat 
char *eliminados(char *str, char *pat){
    int pat_len = strlen(pat), pat_count = 0; 
    char *aux = str; //se crea un puntero auxiliar para no perder la referencia al puntero original

    if(strlen(str) == 0){// si el string original esta vacio hay que retornar otro string vacio
        
        char *new_string = malloc(1); //se crea una var dinamica para retornar el string
        *new_string = '\0'; //como esta vacio solo esta el cero del final

        return new_string; // se retorna el string
    }

    if(pat_len == 0){//si el patron es vacio hay que retornar el mismo string
        
        char *new_string = malloc(strlen(str) + 1); //se crea una var dinamica para el string de retorno 
        strcpy(new_string, str); //se copia el string
        
        return new_string; //se retorna el string
    }

    //while para contar las ocurrencias del patron
    while(strlen(aux) >= strlen(pat)){// mientras aun quepa el patron en lo que resta del string

        if(strncmp(aux, pat, pat_len) == 0){ // se compara un trozo del string con el patron que se quiere eliminar
            
            pat_count++; //si es que se encuentra el patron, se aumenta el contador

            aux += pat_len; // se avanzan el puntero aux mas alla del patron
            
            continue;
        }

        aux++; // se mueve el puntero aux del string un espacio
    }

    char *new_string = malloc(strlen(str) - (pat_len * pat_count) + 1); //se asigna una nueva variable dinamica
    char *aux2 = str, *ns_aux = new_string; // se crean los punteros auxs necesarios

    // nuevo while para traspasar la respuesta
    while(*aux2){ //mientras no se termine de recorrer el string original

        if(strncmp(aux2, pat, pat_len) == 0){//si se encuentra una ocurrencia del patron-

            aux2 += pat_len;//-se ignoran estos caracteres avanzando el puntero

            continue; // se pasa la siguiente iteracion
        }

        //si no hay ocurrencia del patron-
        *ns_aux = *aux2;//-se copia el caracter actual al nuevo string creado
        aux2++; //se avanza al sigte caracter para revisar
        ns_aux++; //se avanza al sigte espacio en el string de la respuesta
    }
     
    *ns_aux = '\0'; //luego del while el puntero auxiliar del nuevo string apunta a donde deberia ir el cero del final del string

    return new_string;
}