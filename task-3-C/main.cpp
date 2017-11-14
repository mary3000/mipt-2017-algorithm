

using namespace std;

struct Polygon {
  void Add(pair<int, int>);
  pair<int, int> StartPoint();

  vector<pair<int, int>> points;
  int start_number = 0;
};

void Polygon::Add(pair<int, int> point) {
  points.push_back(point);
}

pair<int, int> Polygon::StartPoint() {
  return points[start_number];
}

bool HasZeroPoint(Polygon p1, Polygon p2) {
  p2.Reverse();
}

int main() {
  int n, m, x, y;
  Polygon first_polygon, second_polygon;
  cin >> n;
  for (int i = 0; i < n; i++) {
    cin >> x >> y;
    if (x < first_polygon.StartPoint().first) {
      first_polygon.start_number = i;
    } else if (x == first_polygon.StartPoint().first) {
      if (y < first_polygon.StartPoint().second) {
        first_polygon.start_number = i;
      }
    }
    first_polygon.Add(pair(x, y));
  }
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
    second_polygon.Add(pair(x, y));
  }

  if (HasZeroPoint(first_polygon, second_polygon)) {
    cout << "YES";
  } else {
    cout << "NO";
  }

  return 0;
}