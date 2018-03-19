#ifndef FUNBITS
#define FUNBITS

#include "Fun.h"

using namespace Fun;

namespace FunBits{

template<class A> Word getAtDec(Word i,A bit){
  while(i-->0)bit/=10;
  return bit%10;
}

}
#endif
