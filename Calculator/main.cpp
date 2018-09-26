#include "mytools.h"
using namespace std;

class PolyItem {//多项式链表的元素，可以是括号，可以是运算符，可以是项
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
	segment函数用于扫描输入生成链表，如果出现了语法错误则返回null
*/
PolyItem* segment(string input) {
	PolyItem* head = NULL, *ptr = NULL;
	int i = 0;

	while (i < input.length()) {
		string temp = "";
		do {
			temp += input[i++];
			if (isDivider(input[i])||isDivider(input[i-1])) {//遇到分隔符就分开
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