#include <string.h>
int countWords(char *str){
  char *aux = str;
  int count = 0;

  char back = ' ';
  while(*aux){
    if(*aux != ' ' && back == ' '){
      count++;
    }
    back = *aux;
    aux++;
  }

  return count;
}

int strCmp(char *s1, char *s2) {
    char c1;
    char c2;

    for (;;) {
      c1= *s1;
      c2= *s2;

      if (c1==0)
        break;

      if (c1!=c2)
        break;

      s1++;
      s2++;
    }

    return c1 - c2;
}

void sort(char **a, int n) {
  char **ult= &a[n-1];
  char **p= a;

  while(p < ult){ //ordenar por palabras
    int t1 = countWords(p[0]); //cantidad de palabras de la actual palabra
    int t2 = countWords(p[1]); //cantidad de palabras de la siguiente palabra

    if(t1 > t2){
      char *tmp = p[0];

      p[0] = p[1];
      p[1] = tmp;

      p = a;
    }
    else{
      p++;
    }
  }
}