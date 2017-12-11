/*
 * Автор: Феофанова Мария
 *
 * Задача: BigInteger
 * Решение: арифметические операции реализованы столбиком.
 */

#include <vector>
#include <iostream>
#include <string>

#ifndef FIVT_BIGINTEGER_H
#define FIVT_BIGINTEGER_H

const int kDimension = 10;

class BigInteger {
 public:
  BigInteger() : number({0}), is_positive(true) {}

  BigInteger(const std::vector<char> &value, bool is_positive) : number(value), is_positive(is_positive) {}

  BigInteger(int input);
  explicit BigInteger(const std::string &input);

  //Арифметические операторы
  friend BigInteger operator+(const BigInteger &left, const BigInteger &right);
  friend BigInteger operator*(const BigInteger &left, const BigInteger &right);
  friend BigInteger operator-(const BigInteger &left, const BigInteger &right);
  friend BigInteger operator/(const BigInteger &left, const BigInteger &right);
  friend BigInteger operator%(const BigInteger &left, const BigInteger &right);

  //Состright присваивание
  BigInteger& operator+=(const BigInteger &other);
  BigInteger& operator-=(const BigInteger &other);
  BigInteger& operator*=(const BigInteger &other);
  BigInteger& operator/=(const BigInteger &other);
  BigInteger& operator%=(const BigInteger &other);

  //Унарный минус и инкременты/декременты
  BigInteger operator-() const;
  BigInteger& operator++();
  BigInteger operator++(int);
  BigInteger& operator--();
  BigInteger operator--(int);

  //Логические операции
  bool operator==(const BigInteger &other) const;
  bool operator!=(const BigInteger &other) const;
  bool operator<(const BigInteger &other) const;
  bool operator<=(const BigInteger &other) const;
  bool operator>(const BigInteger &other) const;
  bool operator>=(const BigInteger &other) const;

  //Преобразование в bool и string
  explicit operator bool() const;
  std::string toString() const;

  //Операторы ввода-вывода
  friend std::ostream& operator<<(std::ostream &os,  const BigInteger &big_int);
  friend std::istream& operator>>(std::istream &is, BigInteger &big_int);

  //Операторы для int + BigInteger
  friend BigInteger operator-(int first, const BigInteger &second);
  friend BigInteger operator+(int first, const BigInteger &second);
  friend BigInteger operator*(int first, const BigInteger &second);
  friend BigInteger operator/(int first, const BigInteger &second);
  friend BigInteger operator%(int first, const BigInteger &second);

  private:
  //Вектор, состоящий из разрядов в десятичном представлении
  std::vector<char> number;
  //Знак числа
  bool is_positive;

  //Вспомогательные функции
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

//Сложение столбиком
BigInteger operator+(const BigInteger& left, const BigInteger& right) {
  if (left.is_positive != right.is_positive) {
    if (left.is_positive) {
      if (left >= -right) {
        return left.positive_subtraction(right);
      }
      return -right.positive_subtraction(left);
    }
    if (right >= -left) {
      return right.positive_subtraction(left);
    }
    return -left.positive_subtraction(right);
  }
  return left.positive_addition(right);
}

//Вычитает большее число из меньшего
BigInteger BigInteger::positive_subtraction(const BigInteger &other) const {
  std::vector<char> new_number;
  bool remainder = false;
  int first, second;
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

//Складывает числа с одинаковым знаком
BigInteger BigInteger::positive_addition(const BigInteger &other) const {
  std::vector<char> new_number;
  size_t max_length = number.size() >= other.number.size() ? number.size() : other.number.size();
  bool remainder = false;
  int first, second;
  for (size_t i = 0; i <= max_length; i++) {
    if (!remainder && i == max_length) {
      break;
    }
    first = i < number.size() ? number[i] : 0;
    second = i < other.number.size() ? other.number[i] : 0;
    new_number.push_back((first + second + remainder) % kDimension);
    remainder = (first + second + remainder) / kDimension;
  }
  return BigInteger(new_number, is_positive);
}

BigInteger operator-(const BigInteger& left, const BigInteger& right) {
  return left + BigInteger(right.number, !right.is_positive);
}

//Умножение столбиком
BigInteger operator*(const BigInteger& left, const BigInteger& right) {
  if ((left.number.size() == 1 && left.number[0] == 0) ||
      (right.number.size() == 1 && right.number[0] == 0)) {
    return BigInteger(0);
  }
  BigInteger result;
  std::vector<char> curr_number;
  char remainder = 0;
  int first, second;
  for (size_t i = 0; i < right.number.size(); i++) {
    first = right.number[i];
    for (size_t j = 0; j < i; j++) {
      curr_number.push_back(0);
    }
    remainder = 0;
    for (char j : left.number) {
      second = j;
      curr_number.push_back((first * second + remainder) % kDimension);
      remainder = (first * second + remainder) / kDimension;
    }
    if (remainder != 0) {
      curr_number.push_back(remainder);
    }
    result += BigInteger(curr_number, true);
    curr_number.resize(0);
  }
  result.is_positive = (left.is_positive == right.is_positive);
  return result;
}

BigInteger& BigInteger::operator+=(const BigInteger &other) {
  *this = *this + other;
  return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger &other) {
  *this = *this - other;
  return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger &other) {
  *this = *this * other;
  return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger &other) {
  *this = *this / other;
  return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger &other) {
  *this = *this % other;
  return *this;
}

BigInteger BigInteger::operator-() const {
  return BigInteger(number, !is_positive);
}

BigInteger& BigInteger::operator++() {
  *this = *this + BigInteger(1);
  return *this;
}

BigInteger BigInteger::operator++(int) {
  BigInteger old_int = *this;
  ++*this;
  return old_int;
}

BigInteger& BigInteger::operator--() {
  *this = *this - BigInteger(1);
  return *this;
}

BigInteger BigInteger::operator--(int) {
  BigInteger old_int = *this;
  --*this;
  return old_int;
}

//Вместо std::reverse
std::vector<char> Reverse(std::vector<char> vec) {
  char tmp = 0;
  for (size_t i = 0; i < vec.size()/2; i++) {
    tmp = vec[i];
    vec[i] = vec[vec.size() - i - 1];
    vec[vec.size() - i - 1] = tmp;
  }
  return vec;
}

//Деление столбиком
BigInteger operator/(const BigInteger& left, const BigInteger &right) {
  BigInteger denomirator = right;
  bool is_positive = left.is_positive == denomirator.is_positive;
  denomirator.is_positive = true;
  std::vector<char> reversed_result;
  std::vector<char> number_to_div;
  std::vector<char> reversed_numerator = Reverse(left.number);
  BigInteger integer_to_div;

  for (char ch : reversed_numerator) {
    if (number_to_div.size() == 1 && number_to_div[0] == 0) {
      number_to_div.resize(0);
    }
    number_to_div.push_back(ch);
    integer_to_div = BigInteger(Reverse(number_to_div), true);
    if (integer_to_div < denomirator) {
      if (!reversed_result.empty()) {
        reversed_result.push_back(0);
      }
      continue;
    }
    reversed_result.push_back(integer_to_div.small_division(denomirator));
    number_to_div = Reverse(integer_to_div.number);
  }

  if (reversed_result.empty()) {
    reversed_result.push_back(0);
  }
  return BigInteger(Reverse(reversed_result), is_positive);
}

//Деление, дающее < 10 в результате. В bigint остается остаток.
int BigInteger::small_division(const BigInteger &other) {
  int count = 0;
  //Т.к. count < 10, работает за O(n).
  while (*this >= other) {
    count++;
    *this -= other;
  }
  return count;
}

BigInteger operator%(const BigInteger& left, const BigInteger& right) {
  BigInteger div = left / right;
  return left - div * right;
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
  return !this->operator==(other);
}

bool BigInteger::operator<(const BigInteger &other) const {
  if (number.size() == 1 && number[0] == 0) {
    if (other.number.size() == 1 && other.number[0] == 0) {
      return false;
    }
    return other.is_positive;
  }
  if (!is_positive && other.is_positive) {
    return true;
  }
  if (is_positive && !other.is_positive) {
    return false;
  }
  if (other.number.size() > number.size()) {
    return is_positive;
  }
  if (other.number.size() < number.size()) {
    return !is_positive;
  }
  for (int i = number.size() - 1; i >= 0; i--) {
    if (number[i] < other.number[i]) {
      return is_positive;
    } else if (number[i] > other.number[i]) {
      return !is_positive;
    }
  }
  return !is_positive;
}

bool BigInteger::operator<=(const BigInteger &other) const {
  return this->operator==(other) || this->operator<(other);
}

bool BigInteger::operator>(const BigInteger &other) const {
  return other < *this;
}

bool BigInteger::operator>=(const BigInteger &other) const {
  return other <= *this;
}

std::ostream& operator<<(std::ostream &os, const BigInteger &big_int) {
  os << big_int.toString();
  return os;
}
std::istream& operator>>(std::istream &is, BigInteger &big_int) {
  std::string input;
  is >> input;
  std::vector<char> result;
  big_int.is_positive = true;
  bool signed_int = false;
  if (input[0] == '-') {
    signed_int = true;
    big_int.is_positive = false;
  } else if (input[0] == '+') {
    signed_int = true;
  }
  for (auto i = input.rbegin(); i != input.rend() - signed_int; i++) {
    result.push_back(*i - '0');
  }
  big_int.number = result;
  return is;
}

std::string BigInteger::toString() const {
  if (number.size() == 1 && number[0] == 0) {
    return "0";
  }
  std::vector<char> result;
  for (auto i = number.rbegin(); i != number.rend(); i++) {
    result.push_back(*i + '0');
  }
  std::string str = {result.begin(), result.end()};
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

BigInteger::BigInteger(const std::string &input) {
  std::vector<char> result;
  is_positive = true;
  bool signed_int = false;
  if (input[0] == '-') {
    signed_int = true;
    is_positive = false;
  } else if (input[0] == '+') {
    signed_int = true;
  }
  for (auto i = input.rbegin(); i != input.rend() - signed_int; i++) {
    result.push_back(*i - '0');
  }
  number = result;
}

#endif //FIVT_BIGINTEGER_H
