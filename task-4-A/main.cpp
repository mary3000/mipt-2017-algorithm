//
// Created by User on 07.12.2017.
//

#include <iostream>
#include "biginteger.h"

int main() {
  int ii = -12;
  int jj = -5;
  BigInteger i(ii);
  BigInteger j(jj);
  //cin >> j;
  cout << i << endl;
  cout << j << endl;
  i %= i + 1;
  ii %= ii + 1;
  cout << i << endl;
  cout << ii << endl;

  int aa, bb;
  aa = 302696;
  bb = -1066991;
  BigInteger a(aa);
  BigInteger b(bb);
  cin >> a;
  std::string testString = a.toString() + " " + (-b).toString();
  cout << testString << endl;
  b += ++a %= a /= a *= a -= b++;
  bb += ++aa %= aa /= aa *= aa -= bb++;
  cout << a << " " << b << endl;
  cout << aa << " " << bb << endl;
  cout << 5+a << ' ' << 1-b << endl; // 5 -1000
  cout << 5+aa << ' ' << 1-bb << endl; // 5 -1000
  cout << (b == a + 5);

  return 0;
}