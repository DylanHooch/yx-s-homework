#include "mytools.h"
using namespace std;

class PolyItem {//����ʽ�����Ԫ�أ����������ţ����������������������
public:
	string str;
	PolyItem* next = NULL;
	PolyItem(string str) {
		this->str = str;
	}
	bool isBracket() {
		return str == "(" || str == ")";
	}
	bool isOperator() {
		return str == "+" || str == "-" || str == "*" || str == "/";
	}
};

/*
	segment��������ɨ������������������������﷨�����򷵻�null
*/
PolyItem* segment(string input) {
	PolyItem* head = NULL, *ptr = NULL;
	int i = 0;

	while (i < input.length()) {
		string temp = "";
		do {
			temp += input[i++];
			if (isDivider(input[i])||isDivider(input[i-1])) {//�����ָ����ͷֿ�
				break;
			}
		} while (i<input.length());
		if (head == NULL) {
			head = new PolyItem(temp);
			ptr = head;
		}
		else {
			ptr->next = new PolyItem(temp);
			ptr = ptr->next;
		}
	}
	return head;
}

void print(PolyItem* head) {
	for (PolyItem* p = head; p != NULL; p = p->next) {
		cout << p->str << endl;
	}
}

int main() {
	string input;
	cin >> input;
	print(segment(input));
}