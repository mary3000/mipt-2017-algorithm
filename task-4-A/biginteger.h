//
// Created by User on 07.12.2017.
//

#include <utility>
#include <vector>

using namespace std;

#ifndef FIVT_BIGINTEGER_H
#define FIVT_BIGINTEGER_H

const int kDimension = 10;

class BigInteger {
 public:
  BigInteger(vector<char> value, bool is_positive) : number(std::move(value)), is_positive(is_positive) {}

  const BigInteger operator+(const BigInteger &other) const;
  const BigInteger operator-(const BigInteger &other) const;
  const BigInteger operator*(const BigInteger &other) const;
  const BigInteger operator%(const BigInteger &other) const;
  const BigInteger operator+=(const BigInteger &other) const;
  const BigInteger operator-=(const BigInteger &other) const;
  const BigInteger operator*=(const BigInteger &other) const;
  const BigInteger operator/=(const BigInteger &other) const;
  const BigInteger operator%=(const BigInteger &other) const;
  const BigInteger operator-(void) const;
  const BigInteger operator++(void);
  const BigInteger operator++(int);
  const BigInteger operator--(void);
  const BigInteger operator--(int);

  const BigInteger operator+(int other) const;
 //private:
  vector<char> number;
  bool is_positive;

  const BigInteger positive_addition(const BigInteger &other) const;
  const BigInteger positive_subtraction(const BigInteger &other) const;
};

const BigInteger BigInteger::operator+(const BigInteger &other) const {
  /*if (is_positive != other.is_positive) {
    if (is_positive) {
      if (*this >= -other) {
        positive_subtraction(other);
      }
      return -other.positive_subtraction(*this);
    }
    if (other >= -*this) {
      return other.positive_subtraction(*this);
    }
    return -positive_subtraction(other);
  }
  return positive_addition(other);*/
}

const BigInteger BigInteger::positive_subtraction(const BigInteger &other) const {
  vector<char> new_number;
  bool remainder = false;
  int first, second;
  first = second = 0;
  for (int i = 0; i < number.size(); i++) {
    first = number[i] - remainder;
    second = i < other.number.size() ? other.number[i] : 0;
    new_number.push_back((kDimension + first - second) % 10);
    remainder = first < second;
  }
  unsigned int end = new_number.size();
  while (end >= 0) {
    end--;
    if (new_number[end] != 0) {
      break;
    }
  }
  new_number.resize(end + 1);
  return BigInteger(new_number, true);
}
const BigInteger BigInteger::positive_addition(const BigInteger &other) const {
  vector<char> new_number;
  int max_length = number.size() > other.number.size() ? number.size() : other.number.size();
  bool remainder = false;
  int first, second;
  first = second = 0;
  for (int i = 0; i <= max_length; i++) {
    if (!remainder && i >= number.size() && i >= other.number.size()) {
      break;
    }
    first = i < number.size() ? number[i] : 0;
    second = i < other.number.size() ? other.number[i] : 0;
    new_number.push_back((first + second + remainder) % 10);
    remainder = (first + second + remainder) / 10;
  }
  return BigInteger(new_number, is_positive);
}
const BigInteger BigInteger::operator-(const BigInteger &other) const {
  return *this + BigInteger(other.number, false);
}
const BigInteger BigInteger::operator*(const BigInteger &other) const {
  //+...+ or .... ?
}
const BigInteger BigInteger::operator+=(const BigInteger &other) const {
  return *this + other;
}
const BigInteger BigInteger::operator-=(const BigInteger &other) const {
  return *this - other;
}
const BigInteger BigInteger::operator*=(const BigInteger &other) const {
  return *this*other;
}
const BigInteger BigInteger::operator-(void) const {
  return BigInteger(number, !is_positive);
}
const BigInteger BigInteger::operator++(void) {
  *this = *this + 1;
  return *this;
}
const BigInteger BigInteger::operator++(int) {
  BigInteger old_int = *this;
  ++*this;
  return old_int;
}
const BigInteger BigInteger::operator--(void) {
  *this = *this - 1;
  return *this;
}
const BigInteger BigInteger::operator--(int) {
  BigInteger old_int = *this;
  --*this;
  return old_int;
}
const BigInteger BigInteger::operator+(int other) const {
  vector<char> array = number;
  array[0] += other;
  return BigInteger(array, is_positive);
}

#endif //FIVT_BIGINTEGER_H
