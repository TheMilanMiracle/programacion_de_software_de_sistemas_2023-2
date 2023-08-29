#include <stdio.h>

#include "suma.h"

Bcd sumaBcd(Bcd x, Bcd y) {
  Bcd mask = ~((Bcd) -1 << 4), R = ~(-1), xs, ys, sum; //mask = 0b1111, R = 0b0
  int res = 0;
  for(int i = 0; i < (sizeof(Bcd) << 3); i += 4){ //para revisar de a cada 4 bits
    xs = (x >> i) & mask; //se maskean los 4 bits del digito actual y xs/ys guardan el numero guardado en los actuales 4 bits
    ys = (y >> i) & mask; 

    if(res){//si hay resto que sumar se usa y se reinicia su var
        sum = xs + ys + res;
        res = 0;
    }
    else{//si no, no es necesario sumarlo
        sum = xs + ys;
    }
    
    if(sum >= 10){//si hay res se ajustan las vars
        sum -= 10;
        res = 1;
    }
    
    if(sum > 0){//si hay algo que sumar-
        R = R | (sum << i); //-se maskea el resultado a la respuesta
    }

  }
  if(res == 0){//si no hay desborde-
    return R;//-se retorna la respuesta calculada
  }
  return 0xffffffffffffffff; //si hay desborde se retorna el error
}

