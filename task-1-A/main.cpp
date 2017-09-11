//������� ��� ��������� ������� � ������.
//������� ��� ��������� ������� � ������.����� ������� � p, ����� ������ � n.����� O(n + p), ���.������ � O(p).
//������� 1. � ������� ������� - �������;
//p <= 30000, n <= 300000.

#include<iostream>
#include<vector>
#include<fstream>

//�������� ���
//�.�. �������� ������, �������� ����-����� � ����� �������� � �������, ����� �� ���������� �����������.
void prefix() {
	std::ifstream fin("input.txt");
	std::ofstream fout("output.txt");

	std::vector<char> pattern; //����� ����� �������
	std::vector<int> pi; //����������� �������� �������-�������
	char text_char;
	//��� ��� ��� �� ����� �������-������� �� ����� ������, 
	//����� ������� ������� ������� �� �������� � �����������
	int prev_pi = 0; 
	int position = 1; //������� � ����� pattern + ' ' + text
	bool is_pattern = true; //��������, ��� �� ��������� ������, � �������� ��� � ������
	fin.get(text_char);
	pi.push_back(0);
	pattern.push_back(text_char);
	while (fin.get(text_char)) {
		if (text_char == '\n' && !is_pattern) { //���������� ������ �������
			continue;
		}
		if (is_pattern) { //��������� pattern
			pattern.push_back(text_char);
		}
		if (text_char == '\n' && is_pattern) { //����������� �������� � ������
			is_pattern = false;
		}
		while (pattern[prev_pi] != text_char && prev_pi != 0) { //���� ������������ ������
			prev_pi = pi[prev_pi - 1];
		}
		if (pattern[prev_pi] == text_char) {
			prev_pi++;
			if (!is_pattern && prev_pi == (pattern.size() - 1)) { //���� ������ ����� �� ����� ��������, �� ����� ���������
				fout << position - 2 * (pattern.size() - 1) << ' ';
			}
		}
		else {
			prev_pi = 0;
		}
		if (is_pattern) { //���������� �������-������� ��� ��������
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