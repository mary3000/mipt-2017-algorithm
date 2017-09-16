//�����: ����� ���������, 696

//����� ����������������� - ����������� ������, ����������� �� z - �������, � �������� a - z.

#include<iostream>
#include<vector>
#include<algorithm>

//����� ������ ����� �� ���������� ������, ��� ��� � ����� ������� ��������� z-������� � �������-�������.

//�-�, ��������� ������, �� ��������� �� ������ ���������. 
//��� ��� ���������� ������ ����������������� �����������, �� ������ ���������� ����� ������.
char NotPrefixLetter(std::vector<std::pair<int, char> > &prefix_array, int pos) {
	char letter = 'a';
	char curr_char; //������� ������-"��������".

	while (pos > 0) {
		//����� "�������" �� 1 ������, ��� ��������� �� ���������, ����� �������
		//�������� ����, ��� �� ������ ������, ������� �� ����� �� ������ �� ������ 
		//��������� �������� ��� ������� �������, �.�. �������-������� ����� ����� ����� 0.
		curr_char = prefix_array[prefix_array[pos - 1].first].second + 1;
		letter = std::max(letter, curr_char);
		pos = prefix_array[pos - 1].first;
	}

	return letter;
}
//�-� ���� ���������� ����������������� ����������� �����.
char FindLetter(std::vector<std::pair<int, char> > &prefix_array, int pos) {
	char letter;
	int prefix_number = prefix_array[pos].first;

	if (prefix_number != 0) { //���� ������ != 0, ��� ��������, ���� ����� � ������ ������ �� �����. �������.
		letter = prefix_array[prefix_number - 1].second;
	}
	else { //����� - ���� �������, ��������� ����������� ������, ������� �� ���� ��������.
		letter = NotPrefixLetter(prefix_array, pos);
	}

	prefix_array[pos].second = letter;

	return letter;
}

//�-�, ����������� z-������� � �������-�������.
void ToPrefix(std::vector<std::pair<int, char> > &prefix_array, size_t pos, int z_number) {
	//����������� �� ���� ������ � ��������.
	for (int i = z_number; i >= 0; i--) {
		//��� ���, ��������, �������������� ����� ��� ��������� ��� �� �������� � �������, ��������.
		if (pos + i - 1 >= prefix_array.size()) {
			prefix_array.resize(pos + i);
		}
		if (pos >= prefix_array.size()) {
			prefix_array.resize(pos + 1);
		}

		//���� �� �����-�� ���� �������-������� ��� ���������, �� ��� ��������� (�.�. ��� ������ ������, ��� ������
		//�� ���������� � ������� z-�������), � ������ ���� �� ����.
		if (prefix_array[pos + i - 1].first != 0) {
			return;
		}
		//����� - �� ����� ��������.
		prefix_array[pos + i - 1].first = i;
	}
}

int main() {
	//������� �������� z-������� � �������������� ������ ������.
	int z_number;
	size_t str_size;

	//�.�. 1-� ����� - ������ ����� ���� ������, ������ ���, � �������������� ������(������), 
	//� ������� ����� ����������� �������-������� � ������.
	std::cin >> str_size;
	std::vector<std::pair<int, char> > prefix_array(str_size);
	if (str_size == 0) { //�� ���� ������, �� ������ ������ �������� �������� 1 ������� � ��������� ������.
		//����� ����� �������� ��������� ���������.
		prefix_array.push_back(std::pair<int, char>(0, 0));
	}

	//����������� ����� ������������ � ������� �������, ����������� �� � �������-�������.
	int i = 1;
	while(std::cin >> z_number) {
		ToPrefix(prefix_array, i, z_number);
		i++;
	}

	//�������� ��������� ������ �� �������-�������.
	for (int i = 0; i < prefix_array.size(); i++) {
		std::cout << FindLetter(prefix_array, i);
	}

	return 0;
}