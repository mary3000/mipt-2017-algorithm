//�����: ����� ���������, 696

//����� ����������������� - ����������� ������, ����������� �� ������� - �������, � �������� a - z.

#include<iostream>
#include<vector>
#include<algorithm>

//�-�, ��������� ������, �� ��������� �� ������ ���������. 
//��� ��� ���������� ������ ����������������� �����������, �� ������ ���������� ����� ������.
char NotPrefixLetter(std::vector<std::pair<int, char> > &prefix_array) {
	char letter = 'a';
	char curr_char; //������� ������-"��������".
	int i = prefix_array.size();

	while (i > 0) {
		//����� "�������" �� 1 ������, ��� ��������� �� ���������, ����� �������
		//�������� ����, ��� �� ������ ������, ������� �� ����� �� ������ �� ������ 
		//��������� �������� ��� ������� �������, �.�. �������-������� ����� ����� ����� 0.
		curr_char = prefix_array[prefix_array[i - 1].first].second + 1;
		letter = std::max(letter, curr_char);
		i = prefix_array[i - 1].first;
	}

	return letter;
}
//�-� ���� ���������� ����������������� ����������� �����.
char FindLetter(std::vector<std::pair<int, char> > &prefix_array, int prefix_number) {
	char letter;

	if (prefix_number != 0) { //���� ������ != 0, ��� ��������, ���� ����� � ������ ������ �� �����. �������.
		letter = prefix_array[prefix_number - 1].second;
	}
	else { //����� - ���� �������, ��������� ����������� ������, ������� �� ���� ��������.
		letter = NotPrefixLetter(prefix_array);
	}

	prefix_array.push_back(std::pair<int, int>(prefix_number, letter));

	return letter;
}

int main() {
	//�������� �������� �������� � ������ ��� (�������-�������, ������).
	int prefix_number;
	std::vector<std::pair<int, char> > prefix_array;

	while (std::cin >> prefix_number) {
		std::cout << FindLetter(prefix_array, prefix_number);
	}

	return 0;
}