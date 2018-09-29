#include"mytools.h"




/*
segment函数用于扫描输入生成链表
*/
PolyItem* segment(string input) {
	PolyItem* head = NULL, *ptr = NULL;
	int i = 0;

	while (i < input.length()) {
		string temp = "";
		bool flag = true;//处理一个复杂情况，指数为负数时跳过分隔符检查
		do {
			temp += input[i++];
			if (input[i - 1] == '^' && (input[i] == '-' || input[i] == '+')) {//跳过指数的符号
				temp += input[i++];
				flag = false;
			}
			if (isDivider(input[i]) || (isDivider(input[i-1])&&flag)) {//遇到分隔符就分开,i-1是为了读单个分隔符的时候知道自己是个分隔符，要直接分开
				flag = true;
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

Term::Term(double coefficient, string variable, double exp) {
	this->coefficient = coefficient;
	this->variable = variable;
	this->exp = exp;
}

template<typename T>
Stack<T>::Stack(int size=100) {
	this->size = size;
	this->arr = new T[size];
}

template<typename T>
Stack<T>::~Stack() {
	delete[] arr;
	arr = NULL;
}

template<typename T>
void Stack<T>::pop() {
	if(curr)curr--;
}

template<typename T>
bool Stack<T>::push(T& data) {
	if (curr == size ) return false;
	arr[curr++] = data;
	return true;
}

template<typename T>
T& Stack<T>::top() {
	return arr[curr - 1];
}

template<typename T>
bool Stack<T>::empty() {
	return curr==0;
}

bool isDivider(char c) {
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
}

Term* getTerm(string str) {
	//进行正则匹配，提取系数和指数（变量默认为x，不管进来的是什么一律视为x，因为要求是一元的）
	regex reg("(^(-?\\d+)(\\.\\d+)?)x\\^((-?\\d+)(\\.\\d+)?)");
	smatch m;
	regex_match(str, m, reg);
	stringstream ss;
	if (m.size() == 0) {
		regex reg2("-?x\\^((-?\\d+)(\\.\\d+)?)");
		smatch m2;
		regex_match(str, m2, reg2);
		if (m2.size() == 0) {
			regex reg3("(^(-?\\d+)(\\.\\d+)?)x");
			smatch m3;
			regex_match(str, m3, reg3);
			if (m3.size() == 0) {
				if (str == "x") return new Term(1, "x", 1);
				//常数情况
				double coefficient;
				ss << str;
				ss >> coefficient;
				return new Term(coefficient, "x", 0);
			}
			//指数为1
			double coefficient;
			ss << m3[1];
			ss >> coefficient;
			return new Term(coefficient, "x", 1);
		}
		//系数为1
		double exp;
		ss << m2[1];
		ss >> exp;
		return new Term(1, "x", exp);
	}
	double coefficient;
	double exp;
	ss << m[1];
	ss >> coefficient;
	ss.clear();//清空stringstream
	ss.str("");
	ss << m[4];
	ss >> exp;
	return new Term(coefficient, "x", exp);
}

void Polynomia::operator+=(const Polynomia& a) {
	//连起来
	Term* p1 = this->head;
	for (; p1->next != NULL; p1 = p1->next);
	p1->next = a.head;
	size += a.size;
	//化简排序
	simplify();
	sort();
}

void Polynomia::operator-=(const Polynomia& a) {
	//减数全部取负
	for (Term* p = a.head; p != NULL; p = p->next) {
		p->coefficient = -p->coefficient;
	}
	//其余操作与+=相同
	operator+=(a);
}

void Polynomia::operator*=(const Polynomia& a) {//需要较大开销，整个表重新创建
	Term* oldHead = head;
	head = NULL;
	tail = NULL;
	size = 0;
	double coefficient;
	double exp;
	for (Term* p = oldHead; p != NULL; p = p->next) {
		for (Term* p2 = a.head; p2 != NULL; p2 = p2->next) {
			coefficient = p->coefficient * p2->coefficient;
			exp = p->exp + p2->exp;
			add(new Term(coefficient, "x", exp));
		}
	}
	//删除老链表
	Term* temp;
	for (Term* p = oldHead; p->next != NULL; p = p->next){
		temp = p->next;
		p->next = p->next->next;
		delete temp;
		temp = NULL;
	}
	delete oldHead;
	oldHead = NULL;

	simplify();
	sort();
}

void Polynomia::derivation() {
	for (Term* p = head; p != NULL; p = p->next) {
		p->coefficient *= p->exp;
		p->exp -= 1;
	}
	simplify();
}

void Polynomia::simplify() {//找出次数相同的项相加，去除系数为0的项
	Term *temp, *temp4del, *prev=new Term(0,"x",0);
	int curr_exp;
	prev->next = head;
	for (Term* p = head; p != NULL; p = p->next) {
		curr_exp = p->exp;
		for (temp = p; temp->next != NULL; temp = temp->next) {
			if (temp->next->exp == curr_exp) {
				p->coefficient += temp->next->coefficient;
				//删除节点
				temp4del = temp->next;
				temp->next = temp->next->next;
				delete temp4del;
				temp4del = NULL;
				size--;
				if (temp->next == NULL)break;
			}
		}
	}
	for (Term* p = head; p != NULL; p = p->next, prev=prev->next) {
		if (p->coefficient == 0) {//系数为0，删掉
			if (p == head) {
				temp4del = head;
				head = head->next;
			}
			else {
				temp4del = p;
				prev->next = p->next;
				p = p->next;
			}
			delete temp4del;
			temp4del = NULL;
			size--;
			if (p == NULL)break;
		}
	}
}

void Polynomia::add(Term* term) {
	if (term->coefficient == 0) {//走开
		delete term;
		return;
	}
	if (head == NULL) {
		head = term;
		tail = head;
	}
	else {
		tail->next = term;
		tail = tail->next;
	}
	size++;
}

void Polynomia::del(int pos) {
	if (pos>=size)return;
	if (size == 1) {
		delete head;
		head = NULL;
		size = 0;
		tail = NULL;
	}
	Term *p=head, *temp;
	for (int i = 0; i < pos-1 ; i++) {
		p = p->next;
	}
	temp = p->next;
	p->next = p->next->next;
	delete temp;
	size--;
}

//将原始数据弄成项
Polynomia::Polynomia(PolyItem* phead) {
	PolyItem* pstart = new PolyItem("+");
	pstart->next = phead;
	bool minus_flag = false;//标识用不用取负数（去括号）
	bool bracket_flag = false;//标识是否在括号之中
	for (PolyItem* p = pstart; p != NULL; p = p->next) {
		if (p->str == "+") {
			if (p->next->str == "-")continue;
			p = p->next;
			while (p->isBracket())p = p->next;
			Term* term = getTerm(p->str);
			if (minus_flag) {
				term->coefficient = -term->coefficient;
			}
			add(term);
		}
		else if(p->str=="-"){
			p = p->next;
			minus_flag = true;
			if (p->isBracket()) {//假设只有一个括号，多括号嵌套的情况暂时不考虑
				bracket_flag = true;
				p = p->next;
			}
			Term* term = getTerm(p->str);
			term->coefficient = -term->coefficient;
			add(term);
			if (!bracket_flag)minus_flag = false;
		}
		else if (p->str == ")") {
			bracket_flag = false;
			minus_flag = false;
		}
	}
}

Polynomia::~Polynomia() {
	Term* temp;
	for (int i = 0; i < size - 1; i++) {
		temp = head->next;
		head->next = head->next->next;
		delete temp;
		temp = NULL;
	}
	delete head;
	head = NULL;
}

//插入排序
void Polynomia:: sort() {
	if (head == NULL || head->next == NULL)return;
	Term *p = head->next, *pstart = new Term(0,"x",0), *pend = head;
	pstart->next = head; //为了操作方便，添加一个头结点
	while (p != NULL)
	{
		Term *tmp = pstart->next, *pre = pstart;
		while (tmp != p && p->exp <= tmp->exp) //找到插入位置
		{
			tmp = tmp->next; pre = pre->next;
		}
		if (tmp == p)pend = p;
		else
		{
			pend->next = p->next;
			p->next = tmp;
			pre->next = p;
		}
		p = pend->next;
	}
	head = pstart->next;
	delete pstart;
	return;
}

void Polynomia::print() {
	if (head == NULL) {
		cout << "nothing to print" << endl;
		return;
	}
	cout << size << ", ";
	for (Term* p = head; p != NULL; p = p->next) {
		cout << p->coefficient << ", ";
		cout << p->exp<<", ";
	}
	cout << '\b' << '\b' << "  " << endl;//去掉最后的逗号
}

double Polynomia::evaluate(double x) {
	double result = 0;
	for (Term* p = head; p != NULL; p = p->next) {
		result += pow(x, p->exp)*p->coefficient;
	}
	return result;
}