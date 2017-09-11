//Найдите все вхождения шаблона в строку.
//Найдите все вхождения шаблона в строку.Длина шаблона – p, длина строки – n.Время O(n + p), доп.память – O(p).
//Вариант 1. С помощью префикс - функции;
//p <= 30000, n <= 300000.

#include<iostream>
#include<vector>
#include<fstream>

//Алгоритм КМП
//Т.к. алгоритм онлайн, пришлось ввод-вывод с файла оставить в функции, иначе бы нарушилась асимптотика.
void prefix() {
	std::ifstream fin("input.txt");
	std::ofstream fout("output.txt");

	std::vector<char> pattern; //Здесь будет паттерн
	std::vector<int> pi; //Вычисленные значения префикс-функции
	char text_char;
	//Так как нам не нужна префикс-функция от всего текста, 
	//будем хранить префикс функцию от паттерна и предыдущего
	int prev_pi = 0; 
	int position = 1; //Позиция в слове pattern + ' ' + text
	bool is_pattern = true; //проверка, где мы находимся сейчас, в паттерне или в тексте
	fin.get(text_char);
	pi.push_back(0);
	pattern.push_back(text_char);
	while (fin.get(text_char)) {
		if (text_char == '\n' && !is_pattern) { //пропускаем лишние пробелы
			continue;
		}
		if (is_pattern) { //заполняем pattern
			pattern.push_back(text_char);
		}
		if (text_char == '\n' && is_pattern) { //разделитель паттерна и текста
			is_pattern = false;
		}
		while (pattern[prev_pi] != text_char && prev_pi != 0) { //ищем максимальный бордер
			prev_pi = pi[prev_pi - 1];
		}
		if (pattern[prev_pi] == text_char) {
			prev_pi++;
			if (!is_pattern && prev_pi == (pattern.size() - 1)) { //Если бордер равен по длине паттерну, мы нашли вхождение
				fout << position - 2 * (pattern.size() - 1) << ' ';
			}
		}
		else {
			prev_pi = 0;
		}
		if (is_pattern) { //Запоминаем префикс-функцию для паттерна
			pi.push_back(prev_pi);
		}
		position++;
	}
	fin.close();
	fout.close();
}

int main() {
	prefix();
	return 0;
}