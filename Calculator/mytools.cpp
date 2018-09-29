#include"mytools.h"




/*
segment��������ɨ��������������
*/
PolyItem* segment(string input) {
	PolyItem* head = NULL, *ptr = NULL;
	int i = 0;

	while (i < input.length()) {
		string temp = "";
		bool flag = true;//����һ�����������ָ��Ϊ����ʱ�����ָ������
		do {
			temp += input[i++];
			if (input[i - 1] == '^' && (input[i] == '-' || input[i] == '+')) {//����ָ���ķ���
				temp += input[i++];
				flag = false;
			}
			if (isDivider(input[i]) || (isDivider(input[i-1])&&flag)) {//�����ָ����ͷֿ�,i-1��Ϊ�˶������ָ�����ʱ��֪���Լ��Ǹ��ָ�����Ҫֱ�ӷֿ�
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
	//��������ƥ�䣬��ȡϵ����ָ��������Ĭ��Ϊx�����ܽ�������ʲôһ����Ϊx����ΪҪ����һԪ�ģ�
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
				//�������
				double coefficient;
				ss << str;
				ss >> coefficient;
				return new Term(coefficient, "x", 0);
			}
			//ָ��Ϊ1
			double coefficient;
			ss << m3[1];
			ss >> coefficient;
			return new Term(coefficient, "x", 1);
		}
		//ϵ��Ϊ1
		double exp;
		ss << m2[1];
		ss >> exp;
		return new Term(1, "x", exp);
	}
	double coefficient;
	double exp;
	ss << m[1];
	ss >> coefficient;
	ss.clear();//���stringstream
	ss.str("");
	ss << m[4];
	ss >> exp;
	return new Term(coefficient, "x", exp);
}

void Polynomia::operator+=(const Polynomia& a) {
	//������
	Term* p1 = this->head;
	for (; p1->next != NULL; p1 = p1->next);
	p1->next = a.head;
	size += a.size;
	//��������
	simplify();
	sort();
}

void Polynomia::operator-=(const Polynomia& a) {
	//����ȫ��ȡ��
	for (Term* p = a.head; p != NULL; p = p->next) {
		p->coefficient = -p->coefficient;
	}
	//���������+=��ͬ
	operator+=(a);
}

void Polynomia::operator*=(const Polynomia& a) {//��Ҫ�ϴ��������������´���
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
	//ɾ��������
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

void Polynomia::simplify() {//�ҳ�������ͬ������ӣ�ȥ��ϵ��Ϊ0����
	Term *temp, *temp4del, *prev=new Term(0,"x",0);
	int curr_exp;
	prev->next = head;
	for (Term* p = head; p != NULL; p = p->next) {
		curr_exp = p->exp;
		for (temp = p; temp->next != NULL; temp = temp->next) {
			if (temp->next->exp == curr_exp) {
				p->coefficient += temp->next->coefficient;
				//ɾ���ڵ�
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
		if (p->coefficient == 0) {//ϵ��Ϊ0��ɾ��
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
	if (term->coefficient == 0) {//�߿�
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

//��ԭʼ����Ū����
Polynomia::Polynomia(PolyItem* phead) {
	PolyItem* pstart = new PolyItem("+");
	pstart->next = phead;
	bool minus_flag = false;//��ʶ�ò���ȡ������ȥ���ţ�
	bool bracket_flag = false;//��ʶ�Ƿ�������֮��
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
			if (p->isBracket()) {//����ֻ��һ�����ţ�������Ƕ�׵������ʱ������
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

//��������
void Polynomia:: sort() {
	if (head == NULL || head->next == NULL)return;
	Term *p = head->next, *pstart = new Term(0,"x",0), *pend = head;
	pstart->next = head; //Ϊ�˲������㣬���һ��ͷ���
	while (p != NULL)
	{
		Term *tmp = pstart->next, *pre = pstart;
		while (tmp != p && p->exp <= tmp->exp) //�ҵ�����λ��
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
	cout << '\b' << '\b' << "  " << endl;//ȥ�����Ķ���
}

double Polynomia::evaluate(double x) {
	double result = 0;
	for (Term* p = head; p != NULL; p = p->next) {
		result += pow(x, p->exp)*p->coefficient;
	}
	return result;
}