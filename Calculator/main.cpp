#include "mytools.h"
using namespace std;


void print(PolyItem* head) {
	for (PolyItem* p = head; p != NULL; p = p->next) {
		cout << p->str << endl;
	}
}

void prompt_welcome() {
	cout << "��ӭ�������ֶ���ʽO(��_��)O������������ĵ�һ������ʽ��ʾ����(2x+8x^7)-(3x+9x^9)" << endl;
}

void prompt_select_func(Polynomia& p) {
	cout << "----------------------------------------------" << endl;
	cout << "��ǰ����ʽΪ" << endl;
	p.print();
	cout << "������ʲô�أ�" << endl;
	cout << "[1]����һ������ʽ" << endl;
	cout << "[2]��ȥһ������ʽ" << endl;
	cout << "[3]����һ������ʽ" << endl;
	cout << "[4]������" << endl;
	cout << "[5]�������ʽ��ĳ����ֵ" << endl;
	cout << "[6]���õ�ǰ����ʽ" << endl;
	cout << "[7]�˳����ֶ���ʽ" << endl;
	cout << "��������Ž��й���ѡ��" << endl;
}



void add(Polynomia &p) {
	cout << "������Ҫ���ϵĶ���ʽ��ʾ����(2x+8x^7)-(3x+9x^9)" << endl;
	string input;
	cin >> input;
	Polynomia *p2 = new Polynomia(segment(input));//�ڴ�й©�����ⲻ��
	p += *p2;
	cout << "��ӳɹ�" << endl;
}

void substract(Polynomia &p) {
	cout << "������Ҫ��ȥ�Ķ���ʽ��ʾ����(2x+8x^7)-(3x+9x^9)" << endl;
	string input;
	cin >> input;
	Polynomia *p2 = new Polynomia(segment(input));//й¶
	p -= *p2;
	cout << "����ɹ�" << endl;
	p.simplify();//debug
}

void multiply(Polynomia &p) {
	cout << "������Ҫ���ϵĶ���ʽ��ʾ����(2x+8x^7)-(3x+9x^9)" << endl;
	string input;
	cin >> input;
	Polynomia *p2 = new Polynomia(segment(input));//й¶
	p *= *p2;
	cout << "��˳ɹ�" << endl;
}

void derivation(Polynomia &p) {
	p.derivation();
	cout << "�󵼳ɹ�" << endl;
}

void evaluate(Polynomia &p) {
	cout << "�����������ȡֵ" << endl;
	string input;
	cin >> input;
	stringstream ss;
	double x;
	ss << input;
	ss >> x;
	cout << "����ʽ�ڴ˴���ֵΪ��" << p.evaluate(x) << endl;
}

void reset(Polynomia *&p) {
	cout << "�������µĶ���ʽ��ʾ����(2x+8x^7)-(3x+9x^9)" << endl;
	string input;
	cin >> input;
	delete p;
	p = new Polynomia(segment(input));
	cout << "���óɹ�" << endl;
}

int main() {
	string input;
	prompt_welcome();
	cin >> input;
	Polynomia *currentPoly=new Polynomia(segment(input));
	currentPoly->simplify();
	currentPoly->sort();
	input = "";
	input.clear();
	while (true) {
		prompt_select_func(*currentPoly);
		cin >> input;
		switch (atoi(input.c_str())) {
		case 1:
			add(*currentPoly);
			break;
		case 2:
			substract(*currentPoly);
			break;
		case 3:
			multiply(*currentPoly);
			break;
		case 4:
			derivation(*currentPoly);
			break;
		case 5:
			evaluate(*currentPoly);
			break;
		case 6:
			reset(currentPoly);
			break;
		case 7:
			delete currentPoly;
			cout << "�ټ�" << endl;
			exit(0);
		}
	}
	//string input, input2;
	//cin >> input;
	//cin >> input2;
	////print(segment(input));
	//Polynomia p(segment(input));
	//Polynomia p2(segment(input2));
	//p += p2;
	//p.simplify();
	//p.sort();
	//p.print();
}