#ifndef FUNDEBUGGER
#define FUNDEBUGGER

#include <iostream>

#include "Fun.h"

using namespace Fun;
using std::forward;
using std::function;
using std::ostream;
using std::move;

/*
  abstraction of debugger
  author: Su Le
  2018.3
*/
namespace FunDebugger {

// impure, can be modified
template <class A> A trace(Exp<A> f, function<Void(A &&)> st) {
  A &&temp = f();
  st(temp);
  return move(temp);
}

// pure
template <class A> A inspect(ostream &os, Exp<A> f) {
  A &&temp = f();
  os << temp<<std::endl;
  return move(temp);
}

  // pure
  template <class A> A info(String enter,String out,ostream &os, Exp<A> f) {
    os << enter<<std::endl;
    A &&temp = f();
    os << out<<std::endl;
    return move(temp);
  }
} // namespace FunDebugger
#endif
