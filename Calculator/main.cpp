#include "mytools.h"
using namespace std;


void print(PolyItem* head) {
	for (PolyItem* p = head; p != NULL; p = p->next) {
		cout << p->str << endl;
	}
}

void prompt_welcome() {
	cout << "欢迎来到快乐多项式O(∩_∩)O，请先输入你的第一个多项式，示例：(2x+8x^7)-(3x+9x^9)" << endl;
}

void prompt_select_func(Polynomia& p) {
	cout << "----------------------------------------------" << endl;
	cout << "当前多项式为" << endl;
	p.print();
	cout << "你想做什么呢？" << endl;
	cout << "[1]加上一个多项式" << endl;
	cout << "[2]减去一个多项式" << endl;
	cout << "[3]乘上一个多项式" << endl;
	cout << "[4]帮我求导" << endl;
	cout << "[5]计算多项式在某处的值" << endl;
	cout << "[6]重置当前多项式" << endl;
	cout << "[7]退出快乐多项式" << endl;
	cout << "请输入序号进行功能选择" << endl;
}



void add(Polynomia &p) {
	cout << "请输入要加上的多项式，示例：(2x+8x^7)-(3x+9x^9)" << endl;
	string input;
	cin >> input;
	Polynomia *p2 = new Polynomia(segment(input));//内存泄漏，问题不大
	p += *p2;
	cout << "相加成功" << endl;
}

void substract(Polynomia &p) {
	cout << "请输入要减去的多项式，示例：(2x+8x^7)-(3x+9x^9)" << endl;
	string input;
	cin >> input;
	Polynomia *p2 = new Polynomia(segment(input));//泄露
	p -= *p2;
	cout << "相减成功" << endl;
	p.simplify();//debug
}

void multiply(Polynomia &p) {
	cout << "请输入要乘上的多项式，示例：(2x+8x^7)-(3x+9x^9)" << endl;
	string input;
	cin >> input;
	Polynomia *p2 = new Polynomia(segment(input));//泄露
	p *= *p2;
	cout << "相乘成功" << endl;
}

void derivation(Polynomia &p) {
	p.derivation();
	cout << "求导成功" << endl;
}

void evaluate(Polynomia &p) {
	cout << "请输入变量的取值" << endl;
	string input;
	cin >> input;
	stringstream ss;
	double x;
	ss << input;
	ss >> x;
	cout << "多项式在此处的值为：" << p.evaluate(x) << endl;
}

void reset(Polynomia *&p) {
	cout << "请输入新的多项式，示例：(2x+8x^7)-(3x+9x^9)" << endl;
	string input;
	cin >> input;
	delete p;
	p = new Polynomia(segment(input));
	cout << "重置成功" << endl;
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
			cout << "再见" << endl;
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