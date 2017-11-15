/*
 * Автор: Феофанова Мария
 *
 * Задача: Пересечение многоугольников
 * Даны два выпуклых многоугольника на плоскости. В первом n точек, во втором m.
 * Определите, пересекаются ли они за O(n + m). Указание. Используйте сумму Минковского.
 *
 * Описание решения:
 * Решение: для решения "отразим" второй многоугольник относительно начала координат. Посчитаем сумму Минковского
 * этих 2 многоугольников. Если они пересекались, что точка (0, 0) должна лежать в этой сумме,
 * т.к. многоугольники пересекаются <=> существует вектор-точка z из 1 многоугольника,
 * т.ч. -z лежит во 2 многоугольнике <=> -z + z лежит в сумме М. <=> (0, 0) лежит в сумме М.
 *  Сумма Минковского считается по алгоритму, когда берется, скажем, самая левая-нижняя для каждого мн-ка точка,
 * и от нее по часовой стрелке мы последовательно выбираем из 2-х множеств точек такую, чтобы был "наименьший" поворот
 * по часовой стрелке для след. ребра. Понятно, что получившаяся фигура и будет контуром суммы.
 *  Проверка принадлежности (0, 0) сумме: достаточно проверить, есть ли точки из суммы в каждой из четвертей,
 * т.е. пройтись по контуру, отмечая точки концов ребер, а также, проверяя, пересекает ли ребро какую-то еще четверть.
 * Т.к. сумма М. - выпуклая, этого достаточно, чтобы (0, 0) лежала в ней.
 *
 */

#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

//Многоугольник.
struct Polygon {
  //Добавить точку к многоугольнику.
  void Add(const pair<double, double>&);
  //Возвращает стартовую точку - самую левую-нижнюю.
  pair<double, double> StartPoint() const;

  //Точки многоугольника.
  vector<pair<double, double>> points;
  //Номер стартовой точки.
  int start_number = 0;
};

void Polygon::Add(const pair<double, double> &point) {
  points.push_back(point);
}

pair<double, double> Polygon::StartPoint() const {
  if (points.empty()) {
    return {0, 0};
  }
  return points[start_number];
}

//Сумма двух точек - сумма их координат.
pair<double, double> Sum(const pair<double, double> &p1, const pair<double, double> &p2) {
  return {p1.first + p2.first, p1.second + p2.second};
}

//Функция проверяет, является ли полярный угол первого вектора большим, чем полярный угол второго.
//Векторы заданы соответственно двумя первыми точками и двумя вторыми.
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

//Вычисляет сумму минковского для p1 и p2, кладет ответ в answer.
void MinkowskiSum(const Polygon &p1, const Polygon &p2, Polygon &answer) {
  int i = 0, j = 0;
  pair<double, double> p1_start, p1_end, p2_start, p2_end;
  //Пока не обойдем один мз многоугольников, добавляем углы в ответ.
  while (i <= p1.points.size() && j <= p2.points.size()) {
    //Задаем текущие точки и следующие по часовой стрелке, начиная со страртовых.
    p1_start = p1.points[(i + p1.start_number) % p1.points.size()];
    p1_end = p1.points[(i + p1.start_number + 1) % p1.points.size()];
    p2_start = p2.points[(j + p2.start_number) % p2.points.size()];
    p2_end = p2.points[(j + p2.start_number + 1) % p2.points.size()];
    answer.Add(Sum(p1_start, p2_start));
    //Каждый раз мы должны добавить точки, создающие ребро с меньшим поворотом по часовой стрелке,
    //чтобы построить именно контур (все другие точки будут внутренние).
    if (IsBiggerAngle(p1_start, p1_end, p2_start, p2_end)) {
      i++;
    }
    else if (IsBiggerAngle(p2_start, p2_end, p1_start, p1_end)) {
      j++;
    } else {
      i++;
      j++;
    }
  }
}

//Проверяет, лежит ли точка (0, 0) в сумме Минковского p1 + p2.
bool HasZeroPoint(const Polygon &p1, const Polygon &p2) {
  Polygon sum;
  MinkowskiSum(p1, p2, sum);
  //Флаги для четвертей координатной плоскости.
  bool first, second, third, fourth;
  first = second = third = fourth = false;
  //Обходим многоугольник по ребрам по часовой стрелке.
  pair<double, double> prev_point = sum.points[0];
  pair<double, double> curr_point;
  for (int i = 1; i < sum.points.size(); i++) {
    //Отмечаем флагом соответствующую четверть, если там лежит точка.
    curr_point = sum.points[i];
    if (prev_point.first >= 0 && prev_point.second >= 0) {
      first = true;
    }
    if (prev_point.first <= 0 && prev_point.second >= 0) {
      second = true;
    }
    if (prev_point.first <= 0 && prev_point.second <= 0) {
      third = true;
    }
    if (prev_point.first >= 0 && prev_point.second <= 0) {
      fourth = true;
    }
    //А также помечаем флаг, если ребро проходит сквозь четверть, т.е. точки лежат в противоположных четвертях,
    //и угол ребра-вектора больше, нежели у вектора, идущего до (0, 0).
    if (curr_point.first*prev_point.first <= 0 && curr_point.second*prev_point.second <= 0
        && IsBiggerAngle(prev_point, curr_point, prev_point, {0, 0})) {
      if (prev_point.first >= 0 && prev_point.second >= 0) {
        fourth = true;
      }
      if (prev_point.first <= 0 && prev_point.second >= 0) {
        first = true;
      }
      if (prev_point.first <= 0 && prev_point.second <= 0) {
        second = true;
      }
      if (prev_point.first >= 0 && prev_point.second <= 0) {
        third = true;
      }
    }
    prev_point = curr_point;
  }
  //Если все флаги помечены, то есть в каждой четверти есть точка из многоугольника, то и (0, 0) входит в него.
  return first && second && third && fourth;
}

int main() {
  int n, m;
  double x, y;
  Polygon first_polygon, second_polygon;
  cin >> n;
  //Считываем 1 многоугольник.
  for (int i = 0; i < n; i++) {
    cin >> x >> y;
    if (x < first_polygon.StartPoint().first) {
      first_polygon.start_number = i;
    } else if (x == first_polygon.StartPoint().first) {
      if (y < first_polygon.StartPoint().second) {
        first_polygon.start_number = i;
      }
    }
    first_polygon.Add(pair<double, double>(x, y));
  }
  //Считываем 2 перевернутый многоугольник.
  cin >> m;
  for (int i = 0; i < m; i++) {
    cin >> x >> y;
    x = -x;
    y = -y;
    if (x < second_polygon.StartPoint().first) {
      second_polygon.start_number = i;
    } else if (x == second_polygon.StartPoint().first) {
      if (y < second_polygon.StartPoint().second) {
        second_polygon.start_number = i;
      }
    }
    second_polygon.Add(pair<double, double>(x, y));
  }

  if (HasZeroPoint(first_polygon, second_polygon)) {
    cout << "YES";
  } else {
    cout << "NO";
  }


  return 0;
}