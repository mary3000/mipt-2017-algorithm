//Автор: Мария Феофанова, 696

//Найти лексикографически - минимальную строку, построенную по z - функции, в алфавите a - z.

#include<iostream>
#include<vector>
#include<algorithm>

//Далее многое будет из предыдущей задачи, так как я решаю текущую переводом z-функции в префикс-функцию.

//Ф-я, находящая символ, не следующий за концом префиксов. 
//Так как предыдущая строка лексикографически минимальная, мы найдем наименьший такой символ.
char NotPrefixLetter(std::vector<std::pair<int, char> > &prefix_array, int pos) {
	char letter = 'a';
	char curr_char; //Текущий символ-"кандидат".

	while (pos > 0) {
		//Берем "больший" на 1 символ, чем следующий за префиксом, таким образом
		//достигая того, что мы найдем символ, который не равен ни одному из концов 
		//возможных бордеров для текущей позиции, т.е. префикс-функция точно будет равна 0.
		curr_char = prefix_array[prefix_array[pos - 1].first].second + 1;
		letter = std::max(letter, curr_char);
		pos = prefix_array[pos - 1].first;
	}

	return letter;
}
//Ф-я ищет подходящую лексикографически минимальную букву.
char FindLetter(std::vector<std::pair<int, char> > &prefix_array, int pos) {
	char letter;
	int prefix_number = prefix_array[pos].first;

	if (prefix_number != 0) { //Если бордер != 0, все очевидно, надо взять в строке символ на соотв. позиции.
		letter = prefix_array[prefix_number - 1].second;
	}
	else { //Иначе - есть функция, находящая минимальный символ, который не даст бордеров.
		letter = NotPrefixLetter(prefix_array, pos);
	}

	prefix_array[pos].second = letter;

	return letter;
}

//Ф-я, переводящая z-функцию в префикс-функцию.
void ToPrefix(std::vector<std::pair<int, char> > &prefix_array, size_t pos, int z_number) {
	//Пробегаемся по всем буквам в префиксе.
	for (int i = z_number; i >= 0; i--) {
		//Так как, возможно, предполагаемые места для обработки еще не выделены в векторе, выделяем.
		if (pos + i - 1 >= prefix_array.size()) {
			prefix_array.resize(pos + i);
		}
		if (pos >= prefix_array.size()) {
			prefix_array.resize(pos + 1);
		}

		//Если на каком-то шаге префикс-функция уже заполнена, то она корректна (т.к. чем больше бордер, тем раньше
		//он встретится в массиве z-функций), и дальше идти не надо.
		if (prefix_array[pos + i - 1].first != 0) {
			return;
		}
		//Иначе - мы нашли значение.
		prefix_array[pos + i - 1].first = i;
	}
}

int main() {
	//Текущее значение z-функции и предполагаемый размер строки.
	int z_number;
	size_t str_size;

	//Т.к. 1-е число - обычно длина всей строки, читаем его, и инициализируем массив(нулями), 
	//в котором будет вычисленная префикс-функция и строка.
	std::cin >> str_size;
	std::vector<std::pair<int, char> > prefix_array(str_size);
	if (str_size == 0) { //Не знаю почему, но бывают случаи нулевого значения 1 символа и ненулевой строки.
		//Тогда нужно запушить начальное состояние.
		prefix_array.push_back(std::pair<int, char>(0, 0));
	}

	//Последующие числа обрабатываем с помощью функции, переводящей их в префикс-функцию.
	int i = 1;
	while(std::cin >> z_number) {
		ToPrefix(prefix_array, i, z_number);
		i++;
	}

	//Осталось построить строку по префикс-функции.
	for (int i = 0; i < prefix_array.size(); i++) {
		std::cout << FindLetter(prefix_array, i);
	}

	return 0;
}