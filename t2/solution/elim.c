#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "elim.h"

//Esta función elimina del string str todas las apariciones del patrón pat
void eliminar(char *str, char *pat){
    int pat_len = strlen(pat), steps;
    char *aux_str = str, *resp = str, *aux_pat = pat;
    
    if(pat_len){//si el patron fuera de largo cero no hay nada que hacer
        while(*aux_str){//mientras se recorra el string
            steps = 0; //steps contara la cantidad de pasos que dio aux_str que sera util en caso de no encontrar el patron finalmente
            if(*aux_str == *aux_pat){//se compara el caracter al que se apunta de str con el primero del pat
                while(*aux_pat){//se compara el resto de pat para ver si es una ocurrencia
                    if(*aux_str != *aux_pat){//si es que se encuentra un caracter diferente-
                        break;//por tanto, no es necesario seguir buscando
                    }
                    aux_pat++;//se avanza a comparar los siguiente caracter
                    aux_str++;
                    steps++;
                }

                aux_pat = pat; //se resetea el puntero aux de pat pues se movio para hacer las comparaciones

                //si el patron fue encontrado, el puntero aux_str quedara apuntando al caracter despues de
                //la aparicion del patron y sera copiado mas adelante

                if(steps < pat_len){//si el patron no fue encontrado se debe devolver a donde miraba despues de empezar a buscar
                    aux_str -= steps;
                    *resp = *aux_str; //se copia el caracter debido
                    resp++; //se avanza la respuesta
                    aux_str++; //se avanza el puntero aux
                }
            }

            else{//si es que no se encuentra una coincidencia de caracteres
            *resp = *aux_str; //se pone el caracter en donde corresponde
            resp++; //se avanza la respuesta
            aux_str++; //se avanza el puntero aux a str
            }
        }
        *resp = '\0'; //una vez se termina de revisar, el puntero resp apunta a donde deberia ir el 0 que termina el string
    }
}


//Esta funcion retorna un nuevo string que es str habiendole eliminado el patron pat 
char *eliminados(char *str, char *pat){
    int str_len = strlen(str); // pat_len = strlen(pat);

    char* str_copy = malloc(str_len + 1); //hacemos espacio para una copia del string original pues no queremos modificarlo
    strcpy(str_copy, str); //copiamos str en str_copy
    eliminar(str_copy, pat); //eliminamos de la copia el patron que se quiere eliminar

    char* new_string = malloc(strlen(str_copy) + 1);//creamos el nuevo espacio que usara el string a retornar
    strcpy(new_string, str_copy); //traspasamos el string sin el patron

    free(str_copy); //liberamos el espacio pedido para la copia del string original

    return new_string; //se retorna el nuevo string sin las ocurrencias del patron
}