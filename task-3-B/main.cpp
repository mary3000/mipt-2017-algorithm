#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

struct Point {
  Point() : x(0), y(0), z(0) {}
  Point(double x1, double y1, double z1) : x(x1), y(y1), z(z1) {}
  double Len() const;

  double x;
  double y;
  double z;
};

double Point::Len() const {
  return sqrt(x*x + y*y + z*z);
}

Point operator + (const Point& v1, const Point& v2) {
  return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

Point operator - (const Point& v1, const Point& v2) {
  return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

Point operator - (const Point& v1) {
  return {-v1.x, -v1.y, -v1.z};
}

struct Polyhedron {
  void Add(const Point &p);
  vector<Point> points;
};

void Polyhedron::Add(const Point &p) {
  points.push_back(p);
}

bool IsBiggerAngle(const pair<double, double> &a1, const pair<double, double> &a2,
                   const pair<double, double> &b1, const pair<double, double> &b2) {
  pair<double, double> v1 = Sum(a2, pair<double, double>(-a1.first, -a1.second));
  pair<double, double> v2 = Sum(b2, pair<double, double>(-b1.first, -b1.second));
  double len1 = sqrt(v1.first*v1.first + v1.second*v1.second);
  double len2 = sqrt(v2.first*v2.first + v2.second*v2.second);
  //Нормируем векторы.
  v1.first = v1.first / len1;
  v1.second = v1.second / len1;
  v2.first = v2.first / len2;
  v2.second = v2.second / len2;
  //Дальше идет разбор случаев, основанный на взаимном расположении векторов,
  //и том, что x - это косинус, а y - это синус.
  if (v1.second >= 0 && v2.second >= 0) {
    return v1.first <= v2.first;
  }
  if (v1.second <= 0 && v2.second <= 0) {
    return v1.first >= v2.first;
  }
  if (v1.second >= 0) {
    if (v2.first >= 0) {
      return v1.first >= 0 || v1.second >= -v2.second;
    }
    return v1.first >= 0 && v1.second <= -v2.second;
  }
  if (v1.second <= 0) {
    return v2.first <= 0 && v2.second <= -v1.second;
  }
}

void ConvexHull(const Polyhedron &polyhedron) {
  int min_index = 0;
  for (int i = 0; i < polyhedron.points.size(); i++) {
    if (polyhedron.points[i].y < polyhedron.points[min_index].y) {
      min_index = i;
    } else if (polyhedron.points[i].y == polyhedron.points[min_index].y
        && polyhedron.points[i].x < polyhedron.points[min_index].x) {
      min_index = i;
    }
  }

  for (int i = 0; i < polyhedron.points.size(); i++) {
    if (i == min_index) {
      continue;
    }

  }

}

int main() {
  int m = 0, n = 0;
  double x, y, z;
  Polyhedron polyhedron;
  cin >> m;
  for (int i = 0; i < m; i++) {
    cin >> n;
    for (int j = 0; j < n; j++) {
      cin >> x >> y >> z;
      polyhedron.Add(Point(x, y, z));
    }
    ConvexHull(polyhedron);
    //...
    polyhedron.points.clear();
  }

  return 0;
}