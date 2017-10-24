/*
 * Автор: Феофанова Мария
 *
 * Задача: Количество различных подстрок.
 * Дана строка длины n. Найти количество ее различных подстрок. Используйте суффиксный массив.
 * Построение суффиксного массива выполняйте за O(n log n). Вычисление количества различных подстрок выполняйте за O(n).
 *
 * Описание решения:
 * 1) Сортируем суффиксы. Т.к. алфавит конечен, удобно пользоваться сортировкой подсчетом.
 * В 0-й фазе сортируем по первым символам, затем по 2^k символам. Подсчитываем каждый раз классы эквивалентности.
 * Зная классы для 2^(k-1), легко отсортировать по 2^k символам: сдвигаем каждый суффикс на половину вперед,
 * тогда вторая половина уже отсортирована. А первые половины сортируем по классам сортировкой подсчетом.
 * Классов n -> сортировка O(n). Всего итераций O(logn) -> время O(nlogn).
 * 2) Считаем lcp соседних суффиксов в суфф. массиве. Благодаря природе суффиксов, мы можем проверять символы, начиная
 * с предыдущего значения lcp. Таким образом, т.к. lcp < n и уменьшается с каждой итерацией не больше, чем на 1,
 * то время работы O(2n) = O(n).
 * 3) Вычисляем кол-во различных подстрок. Если смотреть суфф. масс., то каждый i-й суффикс дает n - i строк, из которых
 * lcp[i] уже были в предыдущем, т.е. надо просуммировать n - i - lcp[i].
 */

#include <iostream>
#include <vector>

using namespace std;

const int kAlphabetLength = 27;
const char kFirstChar = 'a' - 1;

//Класс для сортировки суфф. массива.
class SuffixArray {
 public:
  //Записывает в answer отсортированный суфф. массив.
  void ToSuffixArray(const string& new_str, vector<int>& answer);

 private:
  //Сортировка по len = 2^k символам.
  void sort(int len);
  //Устойчивая сортировка подсчетом по классам.
  void sort_by_classes();

  string str;
  vector<int> sorted_array;
  vector<int> old_class;
  vector<int> new_class;
};

void SuffixArray::sort_by_classes() {
  vector<int> tmp(sorted_array.size());
  vector<int> class_count(sorted_array.size());
  for (int i = 0; i < class_count.size(); i++) {
    class_count[old_class[i]]++;
    tmp[i] = sorted_array[i];
  }
  int tmp_count = class_count[0];
  int prev_count = 0;
  class_count[0] = 0;
  for (int i = 1; i < class_count.size(); i++) {
    prev_count = tmp_count;
    tmp_count = class_count[i];
    class_count[i] = class_count[i - 1] + prev_count;
  }
  for (int i = 0; i < sorted_array.size(); i++) {
    sorted_array[class_count[old_class[tmp[i]]]++] = tmp[i];
  }
}

void SuffixArray::sort(int len) {
  //Обновляем классы, и сдвигаем на len позиций вперед.
  old_class = new_class;
  for (int &i : sorted_array) {
    i = (sorted_array.size() + i - len) % sorted_array.size();
  }
  //Тогда массив будет отсортирован по 2-й половине, и останется только отсортировать устойчивым алгоритмом 1-ю.
  sort_by_classes();
  //Считаем новые классы.
  int class_number = 0;
  int first = 0;
  int second = 0;
  new_class[sorted_array[0]] = 0;
  for (int i = 1; i < str.size(); i++) {
    first = (sorted_array[i] + len) % sorted_array.size();
    second = (sorted_array[i - 1] + len) % sorted_array.size();
    //Сравниваем классы соответственно 1-й и 2-й половины, получаем новые.
    if (old_class[sorted_array[i - 1]] != old_class[sorted_array[i]]
        || old_class[first] != old_class[second]) {
      class_number++;
    }
    new_class[sorted_array[i]] = class_number;
  }
}

//Сортировка подсчетом для 0-й фазы.
void CountingSort(const string& str, vector<int>& sorted_array) {
  vector<int> count(kAlphabetLength);
  for (char curr_char : str) {
    count[curr_char - kFirstChar]++;
  }
  for (int i = 1; i < count.size(); i++) {
    count[i] += count[i - 1];
  }
  for (int i = 0; i < str.size(); i++) {
    sorted_array[--count[str[i] - kFirstChar]] = i;
  }
}

//Сортируем циклические сдвиги, засчет вспомогательного символа kFirstChar суффиксы сортируются корректно.
void SuffixArray::ToSuffixArray(const string& new_str, vector<int>& answer) {
  str = new_str;
  sorted_array.resize(str.size());
  old_class.resize(str.size());
  new_class.resize(str.size());
  //Сортируем по первым буквам и вычисляем классы.
  CountingSort(str, sorted_array);
  int class_number = 0;
  for (int i = 1; i < str.size(); i++) {
    if (str[sorted_array[i - 1]] != str[sorted_array[i]]) {
      class_number++;
    }
    new_class[sorted_array[i]] = class_number;
  }
  //Сортируем по 2^k первым символам. Благодаря природе суффиксов,
  //длины 2^k нам будет достаточно, ведь по индукции мы уже отсортировали префиксы длиной 2^(k-1).
  //Благодаря цилкических сдвигам мы получим всю информацию про обе половинки.
  for (int len = 1; len <= str.size(); len *= 2) {
    sort(len);
  }
  answer = sorted_array;
  sorted_array.clear();
  old_class.clear();
  new_class.clear();
}

//Записывает в lcp наибольшие общие префиксы в упорядоченном массиве суффиксов.
void LCP(const string &str, vector<long long>& lcp) {
  lcp.resize(str.size());
  SuffixArray suff;
  vector<int> suff_array;
  //С помощью специального класса получим отсортированный массив суффиксов.
  suff.ToSuffixArray(str, suff_array);
  //suff_pos - обратный к suff_array массив.
  vector<int> suff_pos(suff_array.size());
  for (int i = 0; i < suff_array.size(); i++) {
    suff_pos[suff_array[i]] = i;
  }

  int max_prefix = 0;
  int second_pos = 0;
  for (int i = 0; i < str.size(); i++) {
    if (max_prefix > 0) {
      max_prefix--;
    }
    //Позиция лексикографически следующего за текущим суффикса.
    second_pos = suff_pos[i] + 1;
    //Крайний случай: если суффикс последний, следующего нет.
    if (second_pos >= suff_array.size()) {
      lcp[i] = 0;
      max_prefix = 0;
      continue;
    }
    //Проверяем на выход за границы.
    int max_value = i > suff_array[second_pos] ? i : suff_array[second_pos];
    max_value += max_prefix;
    //По свойству суфф. массива и lcp, lcp следующего суффикса в строке не меньше, чем lcp предыдущего
    //(иначе бы нарушалось своейство суфф. массива).
    //Поэтому мы просто запоминаем значение предыдущего lcp в max_prefix и сравниваем остальное.
    while (max_value < str.size()) {
      if (str[i + max_prefix] != str[suff_array[second_pos] + max_prefix]) {
        break;
      }
      max_prefix++;
      max_value = i > suff_array[second_pos] ? i : suff_array[second_pos];
      max_value += max_prefix;
    }
    lcp[i] = max_prefix;
  }
}

//Функция, находящая количество различных подстрок.
long long UniqueSubstringCount(const string& str) {
  vector<long long> lcp_array;
  LCP(str, lcp_array);

  //Каждый суффикс i дает str.size() - i строк, но т.к. в лексикографически предыдущем суффиксе уже были учтены lcp[i]
  //строк, для нахождения только уникальных, надо отнять и их.
  long long count = 0;
  for (int i = 0; i < str.size(); i++) {
    count += str.size() - i - lcp_array[i];
  }
  //Строки от вспомогательного символа нам не нужны.
  return count - str.size();
}

int main() {
  string input_string;
  cin >> input_string;

  cout << UniqueSubstringCount(input_string + kFirstChar);
  return 0;
}