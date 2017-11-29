#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <array>
#include <algorithm>

using namespace std;

//Для удобства есть отдельная структура точки с нужными методами и переоп. операторами.
struct Point {
  Point() : x(0), y(0), z(0) {}
  Point(double x1, double y1, double z1) : x(x1), y(y1), z(z1) {}

  double x;
  double y;
  double z;
};

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

  Point begin;
  Point end;
  Point vec;
};

double LineSegment::Len() const {
  return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
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

//Многогранник, куда будет записываться результат.
struct Polyhedron {
  void Add(const Point &p);
  vector<Point> points;
};

void Polyhedron::Add(const Point &p) {
  points.push_back(p);
}

//Функция сравнения для сортировки граней лексикографически.
bool Comparator (array<int, 3> first, array<int, 3> second) {
  for (int i = 0; i < 3; i++) {
    if (first[i] < second[i]) {
      return true;
    }
    if (first[i] > second[i]) {
      return false;
    }
  }
  return true;
}

//Ищет первую грань, с которой надо начать.
void FindFirstFace(const Polyhedron &polyhedron, array<int, 3> &answer) {
  int first_index = 0;
  //Первую точку находим минимальную по y, x.
  for (int i = 0; i < polyhedron.points.size(); i++) {
    if (polyhedron.points[i].y < polyhedron.points[first_index].y) {
      first_index = i;
    } else if (polyhedron.points[i].y == polyhedron.points[first_index].y
        && polyhedron.points[i].x < polyhedron.points[first_index].x) {
      first_index = i;
    }
  }
  //Далее "проецируем" все точки на плоскость xOy. Находим вторую точку, как в алгоритме Джарвиса 2d.
  Point first_point(polyhedron.points[first_index].x, polyhedron.points[first_index].y, 0);
  int second_index = first_index ? 0 : 1;
  for (int i = 0; i < polyhedron.points.size(); i++) {
    if (i == first_index) {
      continue;
    }
    Point second_point(polyhedron.points[second_index].x, polyhedron.points[second_index].y, 0);
    Point curr_point(polyhedron.points[i].x, polyhedron.points[i].y, 0);
    //Ищем точку, дающую самый минимальный полярный угол у вектора с началом в 1 точке и концом в текущей.
    if (CrossProduct(LineSegment(first_point, second_point), LineSegment(first_point, curr_point)).end.z < 0) {
      second_index = i;
    }
  }
  //Далее выбираем какую-либо 3-ю точку, не равную двум первым, чтобы создать временную грань.
  int tmp_index = 0;
  if (first_index == 0 || second_index == 0) {
    tmp_index++;
    if (first_index == 1 || second_index == 1) {
      tmp_index++;
    }
  }
  //Фиксируем вектор ребра временной грани, и нормаль к плоскости грани.
  LineSegment first_line(polyhedron.points[second_index], polyhedron.points[first_index]);
  LineSegment normal = CrossProduct(LineSegment(polyhedron.points[second_index], polyhedron.points[tmp_index]),
                                    first_line);
  double min_cos = 1;
  int third_index = tmp_index;
  //Сравнивая углы между плоскостями, находим плоскость, после которой нет точек, т.е. максимально отклоненную.
  for (int i = 0; i < polyhedron.points.size(); i++) {
    if (i == first_index || i == second_index || i == tmp_index) {
      continue;
    }
    LineSegment curr_line(polyhedron.points[second_index], polyhedron.points[i]);
    LineSegment curr_normal = CrossProduct(curr_line, first_line);
    LineSegment tmp = CrossProduct(normal, curr_normal);
    //Для определенности ищем плоскость только "справа" от плоскости. Тогда мы точно будем знать,
    //что получившаяся нормаль торчит наружу.
    if (DotProduct(first_line, tmp) > 0) {
      continue;
    }
    //Сравниваем по косинусу.
    double dot_product = DotProduct(normal, curr_normal);
    dot_product /= normal.Len() * curr_normal.Len();
    if (dot_product < min_cos) {
      min_cos = dot_product;
      third_index = i;
    }
  }
  answer = {first_index, second_index, third_index};
}

void ConvexHull(const Polyhedron &polyhedron, vector<array<int, 3>> &convex_hull) {
  array<int, 3> first_face;
  FindFirstFace(polyhedron, first_face);

  vector<vector<bool>> boundary(polyhedron.points.size());
  for (int i = 0; i < boundary.size(); i++) {
    boundary[i] = vector<bool>(polyhedron.points.size());
  }
  for (int i = 0; i < first_face.size(); i++) {
    boundary[first_face[i]][first_face[(i + 1) % 3]] = true;
  }
  queue<array<int, 3>> faces;
  faces.push(first_face);
  array<int, 3> face;
  while (!faces.empty()) {
    face = faces.front();
    faces.pop();
    LineSegment v1(polyhedron.points[face[0]], polyhedron.points[face[1]]);
    LineSegment v2(polyhedron.points[face[0]], polyhedron.points[face[2]]);
    LineSegment normal = CrossProduct(v1, v2);
    for (int i = 0; i < 3; i++) {
      if (!boundary[face[i]][face[(i + 1) % 3]]) {
        continue;
      }
      double max_cos = -1;
      int next_vertex_index = 0;
      for (int j = 0; j < polyhedron.points.size(); j++) {
        if (j == face[0] || j == face[1] || j == face[2]) {
          continue;
        }
        LineSegment curr_normal = CrossProduct(LineSegment(polyhedron.points[face[i]], polyhedron.points[j]),
                                               LineSegment(polyhedron.points[face[i]], polyhedron.points[face[(i + 1) % 3]]));
        double curr_cos = DotProduct(normal, curr_normal);
        curr_cos /= normal.Len() * curr_normal.Len();
        if (curr_cos >= max_cos) {
          max_cos = curr_cos;
          next_vertex_index = j;
        }
      }
      faces.push({face[i], next_vertex_index, face[(i + 1) % 3]});
      for (int j = 0; j < 3; j++) {
        if (boundary[faces.back()[(j + 1) % 3]][faces.back()[j]]) {
          boundary[faces.back()[(j + 1) % 3]][faces.back()[j]] = false;
        } else {
          boundary[faces.back()[j]][faces.back()[(j + 1) % 3]] = true;
        }
      }
    }
    int first_number = face[0] < face[1] ? 0 : 1;
    first_number = face[first_number] < face[2] ? first_number : 2;
    convex_hull.push_back({face[first_number], face[(first_number + 1) % 3], face[(first_number + 2) % 3]});
  }
  sort(convex_hull.begin(), convex_hull.end(), Comparator);
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
    vector<array<int, 3>> answer;
    ConvexHull(polyhedron, answer);
    cout << answer.size() << endl;
    for (int j = 0; j < answer.size(); j++) {
      cout << "3 ";
      for (auto vertex : answer[j]) {
        cout << vertex << ' ';
      }
      cout << endl;
    }
    //...
    polyhedron.points.clear();
  }

  return 0;
}