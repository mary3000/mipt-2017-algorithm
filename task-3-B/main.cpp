#include <iostream>

using namespace std;



int main() {
  int m = 0, n = 0;
  int x, y, z;
  cin >> m;
  for (int i = 0; i < m; i++) {
    cin >> n;
    for (int j = 0; j < n; j++) {
      
    }
  }

  return 0;
}

/*
#include <iostream>

class CBase {
 protected:
  int counter = 0;
};

class CDerived1 : public CBase {
 public:
  virtual void  Inc() { counter++; }
 private:
  int data = 0;
};

class CDerived2 : virtual public CBase {
 public:
  virtual void Inc() { counter++; }
 private:
  int data = 0;
};

int main() {
  CDerived1 d1;
  CDerived2 d2;

  return 0;
}*/