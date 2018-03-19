#ifndef FUNTIMER
#define FUNTIMER

#include <chrono>
#include <iostream>

#include "Fun.h"

using namespace std;
using namespace Fun;

/*
  abstraction of timer
  author: Su Le
  2018.3
*/
namespace FunTimer {

template <class A> using Timer = A (*)(Exp<A> f, function<Void(Double)> t);

// nano level execution time recorder
// execTimeNS::Exp A->(Double->())->A
template <class A> A execTimeNS(Exp<A> f, function<Void(Double)> t) {
  auto start = std::chrono::high_resolution_clock::now();
  A &&temp = f();
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> dur = end - start;
  t(dur.count());
  return move(temp);
}

// execTimes::Word->Timer->(Word->A)->(A->())->(Word->Time->())->()
template <class Arg>
Void execTimes(Word time, Timer<Void> t, function<Arg(Word)> h,
               function<Void(Arg&&)> f, function<Void(Word, Double)> p) {
  for (Word i = 0; i < time; i++) {
    t(exec(f(h(i))), [&](Double t0) { p(i, t0);return 0; });
  }
  return 0;
}
} // namespace FunTimer
#endif
