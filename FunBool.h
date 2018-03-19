#ifndef FUNBOOL
#define FUNBOOL

#include "Fun.h"

using namespace std;
using namespace Fun;

/*
  abstraction of Bool Lambdas
  author: Su Le
  2018.3
*/
namespace FunBool {

  auto fnot = [](auto a) { return [&](auto x){return !a(x);}; };
}
#endif
