//Автор: Мария Феофанова, 696 

//Шаблон поиска задан строкой длины m, в которой кроме обычных символов могут встречаться символы “ ? ”.
//Найти позиции всех вхождений шаблона в тексте длины n.
//Каждое вхождение шаблона предполагает, что все обычные символы совпадают с соответствующими из текста, 
//а вместо символа “ ? ” в тексте встречается произвольный символ.
//Время работы - O(n + m + Z), где Z - общее - число вхождений подстрок шаблона “между вопросиками” в исходном тексте. 
//(Ахо - Корасик)
//m ≤ 5000, n ≤ 2000000.

#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<memory>

#define ALPHABET_LENGTH 26 //длина алфавита.

//узел в будущем боре.
struct Node {
	char curr_char; //символ, по которому пришли в этот узел.
	std::vector<size_t> position; //т.к. шаблонов в рег. выражении может быть много, заводим массив позиций в шаблоне.
	size_t len = 0; //длина шаблона, заполняется только если вершина терминальная.
	bool is_terminal = false; //конечная ли вершина.

	std::shared_ptr<Node> parent; //родитель.
	std::map<char, std::shared_ptr<Node>> childs; //дети по каждому символу.

	std::map<char, std::shared_ptr<Node>> delta; //значение дельта-функции (функции перехода) для каждого символа.
	std::shared_ptr<Node> pi; //суффиксная ссылка.
	std::shared_ptr<Node> good_pi; //хорошая (сжатая) суфф. ссылка.
};

//класс бора, основной для решения.
class Trie {
public:
	Trie();

	void Add(std::string input_str); //строит по тексту-регекспу бор.
	void Find(std::string text); //находит вхождения регекспа в текст.

private:
	std::shared_ptr<Node> get_delta(std::shared_ptr<Node> node, char curr_char); //находит дельта-функцию (ф-бю перехода).
	std::shared_ptr<Node> get_pi(std::shared_ptr<Node> node); //находит суфф. ссылку.
	std::shared_ptr<Node> get_good_pi(std::shared_ptr<Node> node); //сжимает суфф. ссылку.
	void find_occurence(std::shared_ptr<Node> node, std::vector<int>& patterns, size_t pos); //находит вхождения регекспа в позиции pos

	std::shared_ptr<Node> root_; //корень.
	size_t pattern_count_ = 0; //количество паттернов, выделенных из регекспа.
	size_t regexp_len_ = 0; //общая длина регекспа.
};

Trie::Trie() {
	root_ = std::shared_ptr<Node>(new Node);
}

void Trie::Add(std::string str) {
	std::shared_ptr<Node> node = root_;
	size_t position = 0;
	size_t len = 0;
	regexp_len_ = str.length();

	for (size_t i = 0; i < str.length(); i++) {

		if (str[i] != '?') {
			len++;
			//если узел - корень, то мы рассматриваем новый паттерн, запоминаем позицию.
			if (node == root_) {
				position = i;
			}
			//создаем новый узел.
			if (node->childs.find(str[i]) == node->childs.end()){
				std::shared_ptr<Node> next_node(new Node);
				next_node->parent = node;
				next_node->curr_char = str[i];
				node->childs[str[i]] = next_node;
			}
			node = node->childs[str[i]];

		}

		//Если предыдущий паттерн кончился, вершина была терминальной. Заполняем нужные поля.
		if ((i >= 1 && str[i] == '?' && str[i - 1] != '?') || (i == str.length() - 1 && str[i] != '?')) {
			pattern_count_++;
			node->position.push_back(position);
			node->is_terminal = true;
			node->len = len;
			len = 0;
			node = root_;
		}

	}
}

//Суффиксные ссылки и дельта-функции находятся с помощью т.н. "ленивой рекурсии".

std::shared_ptr<Node> Trie::get_delta(std::shared_ptr<Node> node, char curr_char) {
	if (node->delta.find(curr_char) == node->delta.end()) {
		//если есть переход по данному символу, дельта-функция равна этому узлу-сыну.
		if (node->childs.find(curr_char) != node->childs.end()) {
			node->delta[curr_char] = node->childs[curr_char];
		}
		//если узел - корень без перехода по символу, то это сам корень.
		else if (node == root_) {
			node->delta[curr_char] = root_;
		}
		//иначе рекурсивно спускаемся дальше по суффиксной ссылке.
		else {
			node->delta[curr_char] = get_delta(get_pi(node), curr_char);
		} 
	}
	return node->delta[curr_char];
}

std::shared_ptr<Node> Trie::get_pi(std::shared_ptr<Node> node) {
	if (node->pi == nullptr) {
		//задаем начальные значения рекурсии для корня и сына корня.
		if (node == root_ || node->parent == root_) {
			node->pi = root_;
		}
		//остальное вычисляется с помощью рекурсии.
		else {
			node->pi = get_delta(get_pi(node->parent), node->curr_char);
		}
	}
	return node->pi;
}

std::shared_ptr<Node> Trie::get_good_pi(std::shared_ptr<Node> node) {
	if (node->good_pi == nullptr) {
		//если узел терминальный, ссылка уже сжата.
		if (get_pi(node)->is_terminal) {
			node->good_pi = get_pi(node);
		}
		//корень - сам себе суфф. ссылка.
		else if (node == root_) {
			node->good_pi = node;
		}
		//иначе рекурсивно ищем ее дальше по суфф. ссылкам.
		else {
			node->good_pi = get_good_pi(get_pi(node));
		}
	}
	return node->good_pi;
}

void Trie::Find(std::string text) {
	//массив позиций вхождений паттернов в текст. 
	//если значение в его ячейке вдруг станет равно количеству паттернов, мы нашли вхождение всего регекспа.
	std::vector<int> patterns(text.length());
	std::shared_ptr<Node> node = root_;

	//посимвольно с помощью функции перехода ищем вхождения.
	for (size_t i = 0; i < text.length(); i++) {
		node = get_delta(node, text[i]);
		find_occurence(node, patterns, i);
	}
}

void Trie::find_occurence(std::shared_ptr<Node> node, std::vector<int>& patterns, size_t pos_in_text) {
	int index = 0;

	//пока не спустились до корня по сжатым суфф. ссылкам, ищем терминальные вершины.
	while (node != root_) {
		if (!node->is_terminal) {
			node = get_good_pi(node);
			continue;
		}
		for (size_t i = 0; i < node->position.size(); i++) {
			//Индекс - предполагаемся позиция регекспа в тексте.
			index = pos_in_text - node->len - node->position[i] + 1;
			//Если она вмещается в границы текста + все паттерны встали на свои места, выводим ответ.
			if (index >= 0 && ++patterns[index] == pattern_count_ && (index + regexp_len_ <= patterns.size())) {
				std::cout << index << ' ';
			}
		}
		node = get_good_pi(node);
	}
}

int main() {
	Trie trie;
	std::string input_str;

	//строим бор.
	std::cin >> input_str;
	trie.Add(input_str);

	//находим вхождения регекспа в текст.
	std::cin >> input_str;
	trie.Find(input_str);

	return 0;
}