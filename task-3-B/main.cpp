#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

//Для удобства есть отдельная структура точки с нужными методами и переоп. операторами.
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

//Отрезок. Т.к. он же - вектор, к нему поставляются и векторные операции.
struct LineSegment {
  LineSegment(const Point& first, const Point& second) {
    begin = first;
    end = second;
    vec = end - begin;
  }
  double Len() const;
  void Reverse();

  Point begin;
  Point end;
  Point vec;
};

double LineSegment::Len() const {
  return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

//Переворачивает отрезок в другую сторону. Нужно, чтобы рассматривать меньше случаев.
void LineSegment::Reverse() {
  begin = vec + begin;
  vec = -vec;
  end = vec + begin;
}

//Скалярное произведение.
double DotProduct(const LineSegment& v1, const LineSegment& v2) {
  return v1.vec.x*v2.vec.x + v1.vec.y*v2.vec.y + v1.vec.z*v2.vec.z;
}

//Векторное произведение.
LineSegment CrossProduct(const LineSegment& v1, const LineSegment& v2) {
  return {v1.begin,
          v1.begin + Point(v1.vec.y*v2.vec.z - v1.vec.z*v2.vec.y,
                           v1.vec.z*v2.vec.x - v1.vec.x*v2.vec.z,
                           v1.vec.x*v2.vec.y - v1.vec.y*v2.vec.x)};
}

//Смешанное произведение.
double TripleProduct(const LineSegment& v1, const LineSegment& v2, const LineSegment& v3) {
  return DotProduct(v1, CrossProduct(v2, v3));
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

  Point min_point(polyhedron.points[min_index].x, polyhedron.points[min_index].y, 0);
  int second_index = min_index ? 0 : 1;
  for (int i = 0; i < polyhedron.points.size(); i++) {
    if (i == min_index) {
      continue;
    }
    Point second_point(polyhedron.points[second_index].x, polyhedron.points[second_index].y, 0);
    Point new_point(polyhedron.points[i].x, polyhedron.points[i].y, 0);
    if (CrossProduct(LineSegment(min_point, second_point), LineSegment(min_point, new_point)).end.z < 0) {
      second_index = i;
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