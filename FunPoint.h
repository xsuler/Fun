#ifndef FUNPOINT
#define FUNPOINT

#include <cmath>
#include <limits>

#include "FunDebugger.h"
#include "FunVector.h"

using namespace std;
using namespace Fun;
using namespace FunVector;
using namespace FunDebugger;

/*
  abstraction of Point
  author: Suler
  2018.3
*/
namespace FunPoint {

class Point {
private:
  Double x, y;

public:
  friend Double &fst(Point &p);
  friend Double &snd(Point &p);
  Bool operator==(Point pt);
  friend ostream &operator<<(ostream &os, Point p);

public:
  Point(Double x, Double y) : x(x), y(y) {}
  Point() {}
};

Double &fst(Point &p) { return p.x; }
Double &snd(Point &p) { return p.y; }
Double eulerDist(Point a, Point b) {
  return sqrt((fst(a) - fst(b)) * (fst(a) - fst(b)) +
              (snd(a) - snd(b)) * (snd(a) - snd(b)));
}

  Bool Point::operator==(Point pt) { return eulerDist(*this, pt)<=0.1;}

ostream &operator<<(ostream &os, Point p) {
  os << "(" << fst(p) << "," << snd(p) << ")";
  return os;
}

// normal algorithm
Double shortestDistanceN(Vector<Point> pts, Word begin, Word size) {
  return foldlR<Point, Double>(
      begin, size, move(pts), numeric_limits<Double>::max(),
      [&](Double rs, Point a) {
        return min(rs, foldlR<Point, Double>(begin, size, move(pts),
                                         numeric_limits<Double>::max(),
                                         [&](Double ps, Point b) {
                                           if (a == b)
                                             return ps;
                                           else
                                             return min(ps, eulerDist(a, b));
                                             }));
      });
}

auto smallerY = [](Point a, Point b) { eq(snd(a) <= snd(b)) };
auto smallerX = [](Point a, Point b) { eq(fst(a) <= fst(b)) };

// O(nlogn) algorithm, x pre-sorted vector
Double shortestDistanceDH(Vector<Point> pts, int begin, int size, int limit) {
  if (size <= limit) {
    eq(shortestDistanceN(
        mergeSort<Point>(move(pts), begin, size, smallerY, size), begin, size))
  }
  Double midx = fst(pts[begin + size / 2]);
  Double mind = min(
      shortestDistanceDH(move(pts), begin, size / 2, limit),
      shortestDistanceDH(move(pts), begin + size / 2, size - size / 2, limit));

  auto lambda = [&](Vector<Point> ptsy) {
    for (Word i = 0; i < ptsy.size(); i++) {
      for (Word j = i + 1; j <= i + 7 && j < ptsy.size(); j++) {
        int dist = eulerDist(ptsy[i], ptsy[j]);
        if (mind > dist) {
          mind = dist;
        }
      }
    }
  };

  lambda(filterR<Point>(dup(mergeV<Point>(move(pts), begin, size, smallerY)),
                        begin, size,
                        [&](Point t) { return fabs(fst(t) - midx) <= mind; }));

  return mind;
}

Double shortestDistanceD(Vector<Point> v) {
  return shortestDistanceDH(
      mergeSort<Point>(move(v), 0, v.size(), smallerX, v.size()), 0, v.size(),
      1024);
}

vector<Point> randomP(Word s, Word r) {
  vector<Point> v(s);
  for (Word i = 0; i < s; i++) {
    fst(v[i]) = rand() % r;
    snd(v[i]) = rand() % r;
  }
  return v;
}

} // namespace FunPoint
#endif
