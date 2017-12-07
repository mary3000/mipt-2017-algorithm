//
// Created by User on 07.12.2017.
//

#include <iostream>
#include "biginteger.h"

int main() {
  vector<char> v = {0};
  BigInteger i(v, true);
  BigInteger j = i++;
  cout << (int)i.number[0] << endl;
  cout << (int)j.number[0] << endl;
  return 0;
}