//
// Created by User on 07.12.2017.
//

#include <vector>
#include <iostream>
#include <string>

using namespace std;

#ifndef FIVT_BIGINTEGER_H
#define FIVT_BIGINTEGER_H

const int kDimension = 10;

class BigInteger {
 public:
  BigInteger() : number({0}), is_positive(true) {}
  BigInteger(vector<char> value, bool is_positive) : number(std::move(value)), is_positive(is_positive) {}
  BigInteger(int input);

  BigInteger operator+(const BigInteger &other) const;
  BigInteger operator-(const BigInteger &other) const;
  BigInteger operator*(const BigInteger &other) const;
  BigInteger operator/(BigInteger other) const;
  BigInteger operator%(const BigInteger &other) const;
  BigInteger operator+=(const BigInteger &other);
  BigInteger operator-=(const BigInteger &other);
  BigInteger operator*=(const BigInteger &other);
  BigInteger operator/=(const BigInteger &other);
  BigInteger operator%=(const BigInteger &other);
  BigInteger operator-() const;
  BigInteger& operator++();
  BigInteger operator++(int);
  BigInteger& operator--();
  BigInteger operator--(int);
  bool operator==(const BigInteger &other) const;
  bool operator!=(const BigInteger &other) const;
  bool operator<(const BigInteger &other) const;
  bool operator<=(const BigInteger &other) const;
  bool operator>(const BigInteger &other) const;
  bool operator>=(const BigInteger &other) const;

  explicit operator bool() const;
  string toString() const;

  //private:
  vector<char> number;
  bool is_positive;

  friend ostream& operator<<(ostream& os,  const BigInteger &big_int);
  friend istream& operator>>(istream& is, BigInteger &big_int);

  friend BigInteger operator-(int first, const BigInteger &second);
  friend BigInteger operator+(int first, const BigInteger &second);
  friend BigInteger operator*(int first, const BigInteger &second);
  friend BigInteger operator/(int first, const BigInteger &second);
  friend BigInteger operator%(int first, const BigInteger &second);

  BigInteger positive_addition(const BigInteger &other) const;
  BigInteger positive_subtraction(const BigInteger &other) const;
  int small_division(const BigInteger &other);
};

BigInteger::BigInteger(int input) {
  is_positive = input >= 0;
  if (input < 0) {
    input = -input;
  }
  if (input == 0) {
    number.push_back(0);
    return;
  }
  while (input != 0) {
    number.push_back(input % kDimension);
    input /= kDimension;
  }
}

BigInteger BigInteger::operator+(const BigInteger &other) const {
  if (is_positive != other.is_positive) {
    if (is_positive) {
      if (*this >= -other) {
        return positive_subtraction(other);
      }
      return -other.positive_subtraction(*this);
    }
    if (other >= -*this) {
      return other.positive_subtraction(*this);
    }
    return -positive_subtraction(other);
  }
  return positive_addition(other);
}

BigInteger BigInteger::positive_subtraction(const BigInteger &other) const {
  vector<char> new_number;
  bool remainder = false;
  int first, second;
  first = second = 0;
  for (size_t i = 0; i < number.size(); i++) {
    first = number[i] - remainder;
    second = i < other.number.size() ? other.number[i] : 0;
    new_number.push_back((kDimension + first - second) % kDimension);
    remainder = first < second;
  }
  unsigned int end = new_number.size();
  while (end > 0) {
    end--;
    if (new_number[end] != 0) {
      break;
    }
  }
  new_number.resize(end + 1);
  return BigInteger(new_number, true);
}

BigInteger BigInteger::positive_addition(const BigInteger &other) const {
  vector<char> new_number;
  size_t max_length = number.size() > other.number.size() ? number.size() : other.number.size();
  bool remainder = false;
  int first, second;
  first = second = 0;
  for (size_t i = 0; i <= max_length; i++) {
    if (!remainder && i >= number.size() && i >= other.number.size()) {
      break;
    }
    first = i < number.size() ? number[i] : 0;
    second = i < other.number.size() ? other.number[i] : 0;
    new_number.push_back((first + second + remainder) % kDimension);
    remainder = (first + second + remainder) / kDimension;
  }
  return BigInteger(new_number, is_positive);
}

BigInteger BigInteger::operator-(const BigInteger &other) const {
  return *this + BigInteger(other.number, !other.is_positive);
}

BigInteger BigInteger::operator*(const BigInteger &other) const {
  BigInteger new_number;
  vector<char> tmp_number;
  char remainder = 0;
  int first, second;
  first = second = 0;
  for (size_t i = 0; i < other.number.size(); i++) {
    first = other.number[i];
    for (size_t j = 0; j < i; j++) {
      tmp_number.push_back(0);
    }
    remainder = 0;
    for (size_t j = 0; j < number.size(); j++) {
      second = number[j];
      tmp_number.push_back((first * second + remainder) % kDimension);
      remainder = (first * second + remainder) / kDimension;
    }
    if (remainder != 0) {
      tmp_number.push_back(remainder);
    }
    new_number += BigInteger(tmp_number, true);
    tmp_number.resize(0);
  }
  new_number.is_positive = is_positive == other.is_positive;
  return new_number;
}

BigInteger BigInteger::operator+=(const BigInteger &other) {
  *this = *this + other;
  return *this;
}

BigInteger BigInteger::operator-=(const BigInteger &other) {
  *this = *this - other;
  return *this;
}

BigInteger BigInteger::operator*=(const BigInteger &other) {
  *this = *this * other;
  return *this;
}

BigInteger BigInteger::operator/=(const BigInteger &other) {
  *this = *this / other;
  return *this;
}

BigInteger BigInteger::operator%=(const BigInteger &other) {
  *this = *this % other;
  return *this;
}

BigInteger BigInteger::operator-() const {
  return BigInteger(number, !is_positive);
}

BigInteger& BigInteger::operator++() {
  *this = *this + 1;
  return *this;
}

BigInteger BigInteger::operator++(int) {
  BigInteger old_int = *this;
  ++*this;
  return old_int;
}

BigInteger& BigInteger::operator--() {
  *this = *this - 1;
  return *this;
}

BigInteger BigInteger::operator--(int) {
  BigInteger old_int = *this;
  --*this;
  return old_int;
}

vector<char> Reverse(vector<char> vec) {
  char tmp = 0;
  for (size_t i = 0; i < vec.size()/2; i++) {
    tmp = vec[i];
    vec[i] = vec[vec.size() - i - 1];
    vec[vec.size() - i - 1] = tmp;
  }
  return vec;
}

BigInteger BigInteger::operator/(BigInteger other) const {
  //TODO: do smth with 0
  if (other == 0) {
    return {0}; //wtf
  }
  bool is_other_positive = other.is_positive;
  other.is_positive = true;
  vector<char> reversed_result;
  vector<char> tmp_number;
  vector<char> reversed_number = Reverse(number);
  for (char ch : reversed_number) {
    tmp_number.push_back(ch);
    BigInteger tmp_int(Reverse(tmp_number), true);
    if (tmp_int < other) {
      continue;
    }
    reversed_result.push_back(tmp_int.small_division(other));
    tmp_number = Reverse(tmp_int.number);
    if (tmp_number.size() == 1 && tmp_number[0] == 0) {
      tmp_number.resize(0);
    }
  }
  if (reversed_result.empty()) {
    reversed_result.push_back(0);
  }
  return BigInteger(Reverse(reversed_result), is_positive == is_other_positive);
}

int BigInteger::small_division(const BigInteger &other) {
  int count = 0;
  while (*this >= other) {
    count++;
    *this -= other;
  }
  return count;
}

BigInteger BigInteger::operator%(const BigInteger &other) const {
  BigInteger div = *this / other;
  return *this - div * other;
}

bool BigInteger::operator==(const BigInteger &other) const {
  if (other.number.size() != number.size()) {
    return false;
  }
  if (number.size() == 1 && number[0] == 0 && other.number[0] == 0) {
    return true;
  }
  if (is_positive != other.is_positive) {
    return false;
  }
  for (size_t i = 0; i < number.size(); i++) {
    if (number[i] != other.number[i]) {
      return false;
    }
  }
  return true;
}

bool BigInteger::operator!=(const BigInteger &other) const {
  return !(*this == other);
}

bool BigInteger::operator<(const BigInteger &other) const {
  if (other.number.size() > number.size()) {
    return true;
  }
  if (other.number.size() < number.size()) {
    return false;
  }
  for (int i = number.size() - 1; i >= 0; i--) {
    if (number[i] < other.number[i]) {
      return true;
    } else if (number[i] > other.number[i]) {
      return false;
    }
  }
  return false;
}

bool BigInteger::operator<=(const BigInteger &other) const {
  return *this < other || *this == other;
}

bool BigInteger::operator>(const BigInteger &other) const {
  return other < *this;
}

bool BigInteger::operator>=(const BigInteger &other) const {
  return other <= *this;
}

ostream& operator<<(ostream &os, const BigInteger &big_int) {
  os << big_int.toString();
  return os;
}
istream& operator>>(istream &is, BigInteger &big_int) {
  string input;
  is >> input;
  vector<char> result;
  for (auto i = input.rbegin(); i != input.rend(); i++) {
    result.push_back(*i - '0');
  }
  big_int.number = result;
  return is;
}

string BigInteger::toString() const {
  if (number.size() == 1 && number[0] == 0) {
    return "0";
  }
  vector<char> result;
  for (auto i = number.rbegin(); i != number.rend(); i++) {
    result.push_back(*i + '0');
  }
  string str = {result.begin(), result.end()};
  if (!is_positive) {
    return "-" + str;
  }
  return str;
}

BigInteger::operator bool() const {
  return !(number.empty() || (number.size() == 1 && number[0] == 0));
}

BigInteger operator-(int first, const BigInteger &second) {
  return BigInteger(first) - second;
}

BigInteger operator+(int first, const BigInteger &second) {
  return BigInteger(first) + second;
}

BigInteger operator*(int first, const BigInteger &second) {
  return BigInteger(first) * second;
}

BigInteger operator/(int first, const BigInteger &second) {
  return BigInteger(first) / second;
}

BigInteger operator%(int first, const BigInteger &second) {
  return BigInteger(first) % second;
}
/*
BigInteger &operator%=(const BigInteger &first, const BigInteger &second) {
  BigInteger& first_1 = const_cast<BigInteger&>(first);
  first_1 = first_1 % second;
  return first_1;
}
*/
#endif //FIVT_BIGINTEGER_H
