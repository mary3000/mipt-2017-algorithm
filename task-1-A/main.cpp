//Автор: Мария Феофанова, 696   

//Найдите все вхождения шаблона в строку.
//Найдите все вхождения шаблона в строку.Длина шаблона – p, длина строки – n.Время O(n + p), доп.память – O(p).
//Вариант 1. С помощью префикс - функции;
//p <= 30000, n <= 300000.

#include<iostream>
#include<vector>
#include<fstream>

//Функция обработки символа
int ProcessLetter(char text_char, std::vector<char> &pattern, bool &is_pattern,
	std::vector<int> &pi, int &prev_pi, int position) {

	if (text_char == '\n' && !is_pattern) { //пропускаем лишние пробелы
		return -1;
	}
	if (is_pattern) { //заполняем паттерн
		pattern.push_back(text_char);
		if (text_char == '\n') { //если паттерн кончился, запоминаем это
			is_pattern = false;
		}
	}

	while (pattern[prev_pi] != text_char && prev_pi != 0) { //ищем максимальный бордер
		prev_pi = pi[prev_pi - 1];
	}

	if (pattern[prev_pi] == text_char) {
		prev_pi++;
		if (!is_pattern && prev_pi == (pattern.size() - 1)) { //Если бордер равен по длине паттерну, мы нашли вхождение
			return (position - 2 * (pattern.size() - 1));
		}
	}
	else {
		prev_pi = 0;
	}

	if (is_pattern) { //Запоминаем префикс-функцию для паттерна
		pi.push_back(prev_pi);
	}
	return -1;
}

//Алгоритм КМП
//Т.к. алгоритм онлайн, пришлось ввод-вывод с файла оставить в функции, иначе бы нарушилась асимптотика.
void Prefix(const char* file_in, const char* file_out) {
	std::ifstream fin(file_in);
	std::ofstream fout(file_out);

	std::vector<char> pattern; //Здесь будет паттерн
	std::vector<int> pi; //Вычисленные значения префикс-функции

	//Так как нам не нужна префикс-функция от всего текста, 
	//будем хранить префикс функцию от паттерна и предыдущего
	int prev_pi = 0; 
	int position = 1; //Позиция в слове pattern + ' ' + text
	bool is_pattern = true; //проверка, где мы находимся сейчас, в паттерне или в тексте
	char text_char;
	int occurence = -1; //позиция вхождения, если вхождения нет, то -1

	//Начальная инициализация
	fin.get(text_char);
	pi.push_back(0);
	pattern.push_back(text_char);

	while (fin.get(text_char)) {
		occurence = ProcessLetter(text_char, pattern, is_pattern, pi, prev_pi, position);
		if (occurence >= 0) {
			fout << occurence << ' ';
		}
		position++;
	}

	fin.close();
	fout.close();
}

int main() {
	const char* file_in = "input.txt";
	const char* file_out = "output.txt";

	Prefix(file_in, file_out);

	return 0;
}