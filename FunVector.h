#ifndef FUNVECTOR
#define FUNVECTOR

#include <cmath>
#include <iostream>
#include <stack>
#include <vector>

#include "FunBits.h"
#include "FunBool.h"

using namespace std;
using namespace Fun;
using namespace FunBool;
using namespace FunBits;

/*
  abstraction of vector
  author: Su Le
  2018.3
*/
namespace FunVector {


auto smaller = [](auto a, auto b) { return a < b; };
auto larger = [](auto a, auto b) { return a > b; };

template <class A> using Vector = vector<A> &&;

template <class A> vector<A> sized(Word s) {
  vector<A> v(s);
  return move(v);
}

template <class A> Vector<A> map(Vector<A> v, function<A(A)> f) {
  transform(v.begin(), v.end(), v.begin(), f);
  return move(v);
}

template <class A> Void travel(Vector<A> v, function<Void(A)> f) {
  for_each(v.begin(), v.end(), f);
  return 0;
}

template <class A> Vector<A> print(Vector<A> v, ostream &os) {
  travel<A>(move(v), [&](A t) {
    os << t << " ";
    return 0;
  });
  os << endl;
  return move(v);
}

template <class A>
Vector<A> filterR(Vector<A> v, Word begin, Word size, function<Bool(A)> f) {
  Vector<A> rv = sized<A>(size);
  copy(v.begin() + begin, v.begin() + size, rv.begin());
  auto rtI = remove_if(rv.begin(), rv.end(), fnot(f));
  rv.erase(rtI, std::end(rv));
  v = rv;
  return move(v);
}

template <class A> Vector<A> filter(Vector<A> v, function<Bool(A)> f) {
  auto returnIterator = remove_if(v.begin(), v.end(), fnot(f));
  v.erase(returnIterator, std::end(v));
  return move(v);
}

  template <class A> Bool ordered(Vector<A> v,function<Bool(A,A)> f){
    for(Word i=0;i<v.size()-1;i++){
      if(!f(v[i],v[i+1])){
        return 0;
      }
    }
    return 1;
  }

template <class A, class Z>
Z foldlR(Word begin, Word size, Vector<A> v, Z z, function<Z(Z, A)> f) {
  for (Word i = begin; i < begin + size; i++) {
    z = f(z, v[i]);
  }
  return z;
}

// cheaper than foldl, used for rvalue
template <class A, class Z>
Z &&foldlF(Vector<A> v, Z &&z, function<Void(Z &&, A)> f) {
  for (auto t : v) {
    f(move(z), t);
  }
  return move(z);
}

template <class A, class Z> Z foldl(Vector<A> v, Z z, function<Z(Z, A)> f) {
  for (auto t : v) {
    z = f(z, t);
  }
  return move(z);
}

template <class A> Vector<A> fswap(Vector<A> v, Word i, Word j) {
  if (i != j) {
    A temp = v[i];
    v[i] = v[j];
    v[j] = temp;
  }
  return move(v);
}

template <class A> A most(Vector<A> v, function<Bool(A, A)> f) {
  return foldl<A,A>(move(v), v[0], [&](A z, A a) {
    if (!f(z, a))
      return a;
    else
      return z;
  });
}

template <class A> Word LIS(Vector<A> v, function<Bool(A, A)> f) {
  Vector<Word> dp = sized<Word>(v.size());
  for(Word i=0;i<v.size();i++) dp[i]=1;
  for (Word i = 1; i < v.size(); i++) {
    for (Word j = 0; j < i; j++) {
      if (f(v[j], v[i])) {
        if (dp[i] < dp[j] + 1) {
          dp[i] = dp[j] + 1;
        }
      } else {
        if (dp[i] < dp[j]) {
          dp[i] = dp[j];
        }
      }
    }
  }
  return most<A>(move(dp), larger);
}

template <class A>
Vector<A> insertionSort(Vector<A> v, Word begin, Word size,
                        function<Bool(A, A)> f, Word step = 1) {
  int j;
  for (Word i = begin + step; i < begin + size; i += step) {
    j = i;
    while (j > begin && f(v[j], v[j - step])) {
      fswap<A>(move(v), j - step, j);
      j -= step;
    }
  }
  return move(v);
}

inline Vector<Word> radixSort(Vector<Word> v) {
  stack<pair<pair<Word, Word>, Word>> st;
  st.push(make_pair(make_pair(0, v.size()), 9));
  Word begin, size;
  Int s;
  while (!st.empty()) {
    begin = st.top().first.first;
    size = st.top().first.second;
    s = st.top().second;
    st.pop();
    if (s <= 0 || size <= 10) {
      insertionSort<Word>(move(v), begin, size, smaller);
      continue;
    }
    Int is[10];
    for (Int i = 0; i < 10; is[i++] = begin - 1)
      ;
    Word k;
    for (Word i = begin; i < begin + size; i++) {
      k = getAtDec(s, v[i]);
      is[9]++;
      for (Word j = 9; j > k; j--) {
        fswap(move(v), is[j], ++is[j - 1]);
      }
    }
    if (is[0] + 1 - begin > 1)
      st.push(make_pair(make_pair(begin, is[0] + 1 - begin), s - 1));
    for (Word i = 0; i < 9; i++) {
      if (is[i + 1] - is[i] > 1)
        st.push(make_pair(make_pair(is[i] + 1, is[i + 1] - is[i]), s - 1));
    }
  }
  return move(v);
}

template <class A> Vector<A> shellSort(Vector<A> v, function<Bool(A, A)> f) {
  Word gap = v.size();
  while (1) {
    gap = floor(gap / 2);
    for (Word i = 0; i < gap; i++) {
      insertionSort(move(v), i, v.size() - i, f, gap);
    }
    if (gap <= 1)
      return move(v);
  }
}

template <class A>
Vector<A> mergeV(Vector<A> v, Word begin, Word size, function<Bool(A, A)> f) {
  Vector<A> vec = sized<A>(size);
  Word s = 0, i = begin, j = begin + size / 2;
  while (i < begin + size / 2 && j < begin + size) {
    if (f(v[i], v[j])) {
      vec[s++] = v[i++];
    } else {
      vec[s++] = v[j++];
    }
  }
  while (i < begin + size / 2) {
    vec[s++] = v[i++];
  }
  while (j < begin + size) {
    vec[s++] = v[j++];
  }
  copy(vec.begin(), vec.end(), v.begin() + begin);
  return move(v);
}

template <class A>
Vector<A> mergeSort(Vector<A> v, Word begin, Word size, function<Bool(A, A)> f,
                    Word limit) {
  if (size < log2(limit)) {
    return insertionSort(move(v), begin, size, f);
  }
  return mergeV<A>(
      mergeSort<A>(mergeSort<A>(move(v), begin, size / 2, f, limit),
                   begin + size / 2, size - size / 2, f, limit),
      begin, size, f);
}

template <class A>
Word partitionL(Vector<A> v, Word begin, Word size, function<Bool(A, A)> f) {
  if (size <= 1) {
    return 0;
  }
  Int i = begin - 1, j = begin;
  while (j < begin + size - 1) {
    if (f(v[j], v[begin + size - 1])) {
      fswap<A>(move(v), ++i, j);
    }
    j++;
  }
  fswap<A>(move(v), ++i, begin + size - 1);
  return i;
}

template <class A> Vector<A> quickSort(Vector<A> v, function<Bool(A, A)> f) {
  stack<pair<Int, Int>> st;
  st.push(make_pair(0, v.size()));
  Int mid, begin, size;
  while (!st.empty()) {
    begin = st.top().first;
    size = st.top().second;
    st.pop();
    if (size <= 1) {
      continue;
    }
    if (size <= 10) {
      insertionSort(move(v), begin, size, f);
      continue;
    }
    mid = partitionL(move(v), begin, size, f);
    st.push(make_pair(begin, mid - begin));
    st.push(make_pair(mid + 1, begin + size - mid - 1));
  }
  return move(v);
}

} // namespace FunVector
#endif
