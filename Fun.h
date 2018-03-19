#ifndef FUN
#define FUN

#include <algorithm>
#include <functional>
#include <string>

/*
author: suler
date: 2018.3
license: MIT
*/

// no void type
namespace Fun {

// when dealing with functions take Exp type as argument
#define expF(x) [&]() { return x; }
#define exeF(x) [&]() { x;return 0;}
#define dup(x) dupf(move(x))

using Char = char;
using Word = unsigned int;
using Int = int;
using Double = double;
using String = std::string &&;
using Bool = bool;
using Void = bool;

template <class A> using Exp = std::function<A()>;

auto dupf = [](auto &&x) { return x; };

// use dup rather than move to avoid side effect
// should only be used on rvalue varibles

/*

  all the functions just write then using side effects,
  every thing is ok
  so there is no need for a const keyword to keep pure

  usage:
  using Int = int&&

  Int func(Int a){
  eq(
  funcB(funcC(funcD(move(a))))
  )
  }
  Int func(Int a){
  eq(
  funcB(funcC(funcD(dup(a),dup(a))))
  )
  }
*/

// loop::(Word->A)->Word->()
} // namespace Fun
#endif
