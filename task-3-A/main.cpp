/*
 * Автор: Феофанова Мария
 *
 * Задача: Расстояние между отрезками.
 * Даны два отрезка в пространстве (x1, y1, z1) - (x2, y2, z2) и (x3, y3, z3) - (x4, y4, z4).
 * Найдите расстояние между отрезками.
 *
 * Описание решения:
 *  Если один из отрезков вырождается в точку, то расстояние легко ищется с помощью формулы проекции точки на прямую,
 * если она не попадает в отрезок, то ответ - минимум из расстояний от концов отрезка до точки.
 *  Если отрезки в 1 плоскости, надо проверить попадание проекций концов отрезков на другой отрезок,
 * выбрать из них минимум, иначе расстояние - минимум из длин отрезков, соединяющих концы данных.
 *  Из предыдущего пункта легко строится расстояние в 3d: понятно, что если построить плоскость на скрещивающихся прямых,
 * то расстояние от одного отрезка до другого можно представить в виде гипотенузы треугольника,
 * в котором катеты - расстояние от 1-го отрезка до плоскости и расстояние от проекции какой-то точки 1-го отрезка
 * на плоскость до 2-го отрезка. Т.к. 1-ый катет фиксирован, надо минимизировать 2-е значение,
 * воспользуемся для этого предыдущим пунктом.
 */

#include <iostream>
#include <cmath>

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

//Проверяет, находятся для отрезки в 1 плоскости.
bool IsInOneFlat(const LineSegment& v1, const LineSegment& v2) {
  return TripleProduct(v1, v2, LineSegment(v1.begin, v2.begin)) == 0;
}

//Возвращает точку проекции точки на прямую.
Point Projection(const Point& point, const LineSegment& line) {
  double p = line.vec.x*(line.begin.x - point.x) +
      line.vec.y*(line.begin.y - point.y) + line.vec.z*(line.begin.z - point.z);
  p = p / (line.vec.Len());
  double cos_a = line.vec.x / line.vec.Len();
  double cos_b = line.vec.y / line.vec.Len();
  double cos_c = line.vec.z / line.vec.Len();
  return {line.begin.x - p*cos_a, line.begin.y - p*cos_b, line.begin.z - p*cos_c};
}

//Проверяет, находится ли данная точка прямой в отрезке, принадлежащем этой же прямой.
//Для этого достаточно, чтобы отрезок был сонаправлен с вектором, построенном на точке и конца отрезка,
//и чтобы полученный вектор был меньше.
bool IsInLineSegment(const Point& point, const LineSegment& line) {
  LineSegment seg_between(point, line.end);
  double dot = DotProduct(line, seg_between);
  return dot >= 0 && seg_between.Len() <= line.Len();
}

//Считает расстояние между отрезками в плоскости.
double FlatDistance(const LineSegment& v1, LineSegment v2) {
  if (DotProduct(v1, v2) < 0) {
    v2.Reverse(); //Делаем сонаправленными отрезки.
  }
  //Строим все варианты проекций концов отрезков на прямую, содержащую другой отрезок.
  LineSegment pr1(v1.begin, Projection(v1.begin, v2));
  LineSegment pr2(v1.end, Projection(v1.end, v2));
  LineSegment pr3(v2.begin, Projection(v2.begin, v1));
  LineSegment pr4(v2.end, Projection(v2.end, v1));
  //Для того, чтобы отрезки пересекались, достаточно, чтобы проекции обоих отрезков падали с разных сторон.
  if (DotProduct(pr1, pr2) <= 0 && DotProduct(pr3, pr4) <= 0) {
    return 0;
  }
  LineSegment seg_between = LineSegment(v1.end, v2.begin);
  double min_len = seg_between.Len();
  //Расстояние между отрезками - это минимум из проекций, попадающих в отрезок, или, если таких нет,
  //минимум из расстояний до концов отрезков.
  if (IsInLineSegment(pr1.end, v2)) {
    min_len = min(min_len, pr1.Len());
  }
  if (IsInLineSegment(pr2.end, v2)) {
    min_len = min(min_len, pr2.Len());
  }
  if (IsInLineSegment(pr3.end, v1)) {
    min_len = min(min_len, pr3.Len());
  }
  if (IsInLineSegment(pr4.end, v1)) {
    min_len = min(min_len, pr4.Len());
  }
  seg_between = LineSegment(v1.end, v2.end);
  min_len = min(min_len, seg_between.Len());
  seg_between = LineSegment(v1.begin, v2.begin);
  min_len = min(min_len, seg_between.Len());
  seg_between = LineSegment(v1.begin, v2.end);
  min_len = min(min_len, seg_between.Len());
  return min_len;
}

//Проекция точки на плоскость, построенную на 1-м отрезке + 2-м отрезке в качества вектора.
Point ProjectionOnFlat(Point point, LineSegment v1, LineSegment v2) {
  LineSegment normal = CrossProduct(v1, v2);
  double p = normal.vec.x*point.x + normal.vec.y*point.y + normal.vec.z*point.z
      - DotProduct(normal, {Point(), v1.begin});
  p = p / normal.Len();
  double cos_a = normal.vec.x / normal.Len();
  double cos_b = normal.vec.y / normal.Len();
  double cos_c = normal.vec.z / normal.Len();
  return {point.x - p*cos_a, point.y - p*cos_b, point.z - p*cos_c};
}

//Расстояние между отрезками.
double Distance(LineSegment v1, LineSegment v2) {
  //Если в одной плоскости, ищем расстояние с помощью спец. функции
  if (IsInOneFlat(v1, v2)) {
    return FlatDistance(v1, v2);
  } else {
    //Иначе прямые, содержащие отрезки, скрещивающиеся.
    //Проецируем отрезок на плоскость, построенную на прямых. Тогда расстояние - кв. корень из расстояния
    //до плоскости и "плоского" расстояния между отрезками.
    LineSegment pr(ProjectionOnFlat(v1.begin, v2, v1), ProjectionOnFlat(v1.end, v2, v1));
    double height = (v1.begin - pr.begin).Len();
    double len = FlatDistance(v2, pr);
    return sqrt(len*len + height*height);
  }
}

//Расстояние от точки до отрезка.
double DistanceToPoint(LineSegment v, Point p) {
  //Обе точки.
  if (v.Len() == 0) {
    return (v.begin - p).Len();
  }
  Point pr = Projection(p, v);
  //Проекиця попадает на отрезок.
  if (IsInLineSegment(p, v)) {
    return (p - pr).Len();
  }
  //Иначе минимум расстояний до концов отрезков.
  return min((p - v.begin).Len(), (p - v.end).Len());
}

int main() {
  //Инициализация значений.
  int x, y, z, x1, y1, z1;
  cin >> x >> y >> z;
  cin >> x1 >> y1 >> z1;
  LineSegment v1(Point(x, y, z), Point(x1, y1, z1));
  cin >> x >> y >> z;
  cin >> x1 >> y1 >> z1;
  LineSegment v2(Point(x, y, z), Point(x1, y1, z1));
  cout.precision(7);

  //Отдельная обработка случая вырождения до точки.
  if (v1.Len() == 0) {
    cout << fixed << DistanceToPoint(v2, v1.begin);
    return 0;
  }
  if (v2.Len() == 0) {
    cout << fixed << DistanceToPoint(v1, v2.begin);
    return 0;
  }

  cout << fixed << Distance(v1, v2);
  return 0;
}