#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "elim.h"

//Esta función elimina del string str todas las apariciones del patrón pat
void eliminar(char *str, char *pat){
    int pat_len = strlen(pat); 
    char *aux = str, *back = str + pat_len, backup; //se guarda una copia del puntero al string y otro a strlen(pat) espacios mas

    while(strlen(aux) >= strlen(pat)){// mientras aun quepa el patron en lo que resta del string
        backup = *back; // se guarda el caracter que exece el largo de pat
        *back = 0; // se reemplaza por cero para marcar el final de un str y poder aplicar strcmp

        if(strcmp(aux,pat) == 0){ // se compara un trozo del string con el patron que se quiere eliminar
            *back = backup; // se devuelve el caracter guardado
            char *copy = back + pat_len - 1, *paste = aux; //puntero para copiar los caracteres luego del patron y moverlos
            
            while(*copy){
                *paste = *copy;
                copy++;
                paste++;
            }
            
            copy--;
            *copy = 0; // se marca el nuevo final del string original
            
            continue;
        }
        
        *back = backup; // luego de hacer la revision se vuelve a asignar el caracter guardado para no perderlo
        aux++; // se mueve el puntero aux del string un espacio
        back++; // tambien se mueve el puntero al caracter que excede el largo de pat
    }

}

//Esta funcion retorna un nuevo string que es str habiendole eliminado el patron pat 
char *eliminados(char *str, char *pat){
    int pat_len = strlen(pat), pat_count = 0; 
    char *aux = str, *back = str + pat_len, backup; //se guarda una copia del puntero al string y otro a strlen(pat) espacios mas

    while(strlen(aux) >= strlen(pat)){// mientras aun quepa el patron en lo que resta del string
        backup = *back; // se guarda el caracter que exece el largo de pat
        *back = 0; // se reemplaza por cero para marcar el final de un str y poder aplicar strcmp

        if(strcmp(aux,pat) == 0){ // se compara un trozo del string con el patron que se quiere eliminar
            pat_count++; //si es que se encuentra el patron, se aumenta el contador
            *back = backup; // se devuelve el caracter guardado
            aux += pat_len;     // se avanzan los punteros hasta despues del patron
            back += pat_len;
            continue;
        }
        
        *back = backup; // luego de hacer la revision se vuelve a asignar el caracter guardado para no perderlo
        aux++; // se mueve el puntero aux del string un espacio
        back++; // tambien se mueve el puntero al caracter que excede el largo de pat
    }
    char *new_string = malloc(strlen(str) - (pat_len * pat_count) +1); //se crea el nuevo string 
    char *aux2 = str, *back2 = str + pat_len, *ns_aux = new_string; // se reinician las variables y se crea un puntero aux para new_string

    // nuevo while para traspasar la respuesta
    while(strlen(aux2) >= strlen(pat)){
        backup = *back2;
        *back2 = 0;

        if(strcmp(aux2,pat) == 0){
            *back2 = backup;
            aux2 += pat_len;
            back2 += pat_len;
            continue;
        }

        *back2 = backup;
        *ns_aux = *aux2;
        aux2++;
        back2++;
        ns_aux++;
    }

    return new_string;
}