#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "elim.h"

//Esta función elimina del string str todas las apariciones del patrón pat
void eliminar(char *str, char *pat){
    int pat_len = strlen(pat), str_len = strlen(str);
    char *aux_str, *aux_pat; //se guarda una copia del puntero al string y otro a strlen(pat) espacios mas

    if(pat_len){ //si el patron es de largo 0 no hay nada que hacer
        int i = 0; //para el recorrido de str

        while(i < str_len){ //mientras no se haya recorrido todo el string
            aux_str = str + i; //se define donde apunta el puntero auxiliar esta vuelta
            aux_pat = pat; //lo mismo para el puntero aux del patron

            if(*aux_str == *aux_pat){ //si se encuentra una primera coicidencia entre el string y el patron-
                char *copy = aux_str + pat_len, *paste = aux_str; //se declaran los punteros necesarios reubicar hacia atras los caracteres
                char pattern_found = 1; //condicion de haber encontrado el patron
  
                aux_pat++; //-se empieza a buscar en los demas caracteres del patron
                aux_str++;
                while(*aux_pat){ //se compara el resto del patron hasta llegar al caracter \0
                    if(*aux_str != *aux_pat){ //si se encuentra un caracter que no es igual al patro-
                        pattern_found = 0; //-no se encontro el patron
                        break; //se rompe el while
                    } 
                    aux_str++; //se revisa el proxima caracter del str-
                    aux_pat++; //-y del patron
                }

                if(pattern_found){//si es que efectivamente se econtro el patron

                    while(*copy){ //mientras queden caracteres que reubicar
                        *paste = *copy; //se mueve el caracter para atras
                        copy++; // se avanza al sigte caracter 
                        paste++; // se avanza al sigte caracter
                    }
                    
                    copy -= pat_len; //como en el while se llega al 0 final, se retrocede el puntero para mover el 0 lo que se movio del string hacia atras
                    *copy = '\0'; // se marca el nuevo final del string original

                    continue; //se avanza a la siguiente vuelta con el i actualizado
                }
            }
            
            i++; //se avanza el i para leer el sigte caracter en el string
        }
    }
}

//Esta funcion retorna un nuevo string que es str habiendole eliminado el patron pat 
char *eliminados(char *str, char *pat){
    int pat_len = strlen(pat), str_len = strlen(str), pat_count = 0; 
    char *aux_str, *aux_pat; //se crean punteros auxiliares para no perder la referencia al str original y pat original 

    if(str_len == 0){// si el string original esta vacio hay que retornar otro string vacio
        
        char *new_string = malloc(1); //se crea una var dinamica para retornar el string
        *new_string = '\0'; //como esta vacio solo esta el cero del final

        return new_string; // se retorna el string
    }

    if(pat_len == 0){//si el patron es vacio hay que retornar el mismo string
        
        char *new_string = malloc(str_len + 1); //se crea una var dinamica para el string de retorno 
        strcpy(new_string, str); //se copia el string
        
        return new_string; //se retorna el string
    }

    int i = 0; //i para iterar en el string

    //while para contar las ocurrencias del patron
    while(i < str_len){// mientras aun quede string por recorrer
        aux_str = str + i; //se establecen los punteros auxiliares
        aux_pat = pat;


        if(*aux_str == *aux_pat){ // se busca una primera ocurrencia del patron
            char pattern_found = 1;
            aux_str++;//se empieza a buscar desde el proximo caracter respectivo
            aux_pat++;

            while(*aux_pat){//mientras aun haya caracteres que comparar del patron
                if(*aux_str != *aux_pat){// si se haya un caracter diferente ya no hay patron
                    pattern_found = 0;
                    break;
                }
                aux_str++;//se avanzan los respectivos caracteres
                aux_pat++;
            }

            if(pattern_found){
                pat_count++; //si es que se encuentra el patron, se aumenta el contador
                
                i += pat_len; //se lleva el i mas alla de la posicion del patron

                continue;
            }
        }

        i++; //se avanza el i para leer el proximo caracter
    }

    char *new_string = malloc(str_len - (pat_len * pat_count) + 1); //se asigna una nueva variable dinamica
    char *ns_aux = new_string; // se crean los punteros auxs necesarios


    i = 0; //se resetea la variable para iterar

    // nuevo while para traspasar la respuesta
    while(i < str_len){ //mientras no se termine de recorrer el string original
        aux_str = str + i;//se definen los punteros aux para esta iteracion
        aux_pat = pat;


        if(*aux_str == *aux_pat){//si se encuentra una primera ocurrencia del patron-
            char pattern_found = 1; //condicion de haber encontrado una ocurrencia del patron
            aux_str++; //se empieza a buscar a partir de los siguientes caracteres respectivos
            aux_pat++;

            while(*aux_pat){//mientras quede patron por checkear
                if(*aux_str != *aux_pat){ //si hay un caracter diferente ya no puede haber patron
                    pattern_found = 0;
                    break;
                }
                aux_str++; //se avanzan los respectivos punteros
                aux_pat++;
            }
            
            if(pattern_found){ //si se encontro el patron hay que avanzar el i mas alla del patron para ignorarlo
                i += pat_len;
                continue;
            }
        }

        //en caso de no haber encontrado un patron se copia el actual caracter a la respuesta
        *ns_aux = *aux_str;
        ns_aux++; //se avanza al siguiente caracter a agregar en new_string
        i++; //y se avanza el i para ver el siguiente caracter en la siguiente iteracion

    }
     
    *ns_aux = '\0'; //luego del while el puntero auxiliar del nuevo string apunta a donde deberia ir el cero del final del string

    return new_string;
}