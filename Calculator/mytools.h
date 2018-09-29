#pragma once
#include <iostream>
#include <string>
#include <regex>
#include <sstream>
using namespace std;

class PolyItem {//����ʽ�����Ԫ�أ����������ţ����������������������൱��ԭʼ����
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

class Term {//����ʽ��һ��
public:
	double coefficient;//ϵ��
	string variable;//����
	double exp;//����
	Term(double, string, double);
	Term* next = NULL;
};

class Polynomia {//����ʽ
public:
	Term *head=NULL,*tail=NULL;
	int size=0;

	Polynomia(PolyItem*);
	~Polynomia();

	void sort();//����ָ����������
	void del(int pos);//ɾ���±�Ϊpos��Ԫ��
	void add(Term*);//���Ԫ��
	void simplify();//����
	double evaluate(double x);//�����x����ֵ
	void derivation();//��
	void print();
	
	void operator+=(const Polynomia &a);
	void operator-=(const Polynomia &b);
	void operator*=(const Polynomia &c);
};

template<typename T>
class Stack {
private:
	int size;//ջ�������
	int curr=0;//ջ��Ԫ�ظ���
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