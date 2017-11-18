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