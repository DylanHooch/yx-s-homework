#pragma once
#include <iostream>
#include <string>
#include <regex>
#include <sstream>
using namespace std;

class PolyItem {//多项式链表的元素，可以是括号，可以是运算符，可以是项，相当于原始数据
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

class Term {//多项式的一项
public:
	double coefficient;//系数
	string variable;//变量
	double exp;//次数
	Term(double, string, double);
	Term* next = NULL;
};

class Polynomia {//多项式
public:
	Term *head=NULL,*tail=NULL;
	int size=0;

	Polynomia(PolyItem*);
	~Polynomia();

	void sort();//按照指数降序排列
	void del(int pos);//删除下标为pos的元素
	void add(Term*);//添加元素
	void simplify();//化简
	double evaluate(double x);//获得在x处的值
	void derivation();//求导
	void print();
	
	void operator+=(const Polynomia &a);
	void operator-=(const Polynomia &b);
	void operator*=(const Polynomia &c);
};

template<typename T>
class Stack {
private:
	int size;//栈最大容量
	int curr=0;//栈中元素个数
	T*  arr;
public:
	Stack(int size=100);
	~Stack();
	bool push(T&);
	T& top();
	void pop();
	bool empty();
};



bool isDivider(char);

Term* getTerm(string);

PolyItem* segment(string);