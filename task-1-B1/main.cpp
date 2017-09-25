//Автор: Мария Феофанова, 696

//Найти лексикографически - минимальную строку, построенную по префикс - функции, в алфавите a - z.

#include<iostream>
#include<vector>
#include<algorithm>

//Ф-я, находящая символ, не следующий за концом префиксов. 
//Так как предыдущая строка лексикографически минимальная, мы найдем наименьший такой символ.
char NotPrefixLetter(std::vector<std::pair<int, char> > &prefix_array) {
	char letter = 'a';
	char curr_char; //Текущий символ-"кандидат".
	int i = prefix_array.size();

	while (i > 0) {
		//Берем "больший" на 1 символ, чем следующий за префиксом, таким образом
		//достигая того, что мы найдем символ, который не равен ни одному из концов 
		//возможных бордеров для текущей позиции, т.е. префикс-функция точно будет равна 0.
		curr_char = prefix_array[prefix_array[i - 1].first].second + 1;
		letter = std::max(letter, curr_char);
		i = prefix_array[i - 1].first;
	}

	return letter;
}
//Ф-я ищет подходящую лексикографически минимальную букву.
char FindLetter(std::vector<std::pair<int, char> > &prefix_array, int prefix_number) {
	char letter;

	if (prefix_number != 0) { //Если бордер != 0, все очевидно, надо взять в строке символ на соотв. позиции.
		letter = prefix_array[prefix_number - 1].second;
	}
	else { //Иначе - есть функция, находящая минимальный символ, который не даст бордеров.
		letter = NotPrefixLetter(prefix_array);
	}

	prefix_array.push_back(std::pair<int, int>(prefix_number, letter));

	return letter;
}

int main() {
	//Значение текущего префикса и вектор пар (префикс-функция, символ).
	int prefix_number;
	std::vector<std::pair<int, char> > prefix_array;

	while (std::cin >> prefix_number) {
		std::cout << FindLetter(prefix_array, prefix_number);
	}

	return 0;
}