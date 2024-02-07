#pragma once
#include <iostream>
#include <stdexcept>

const double eps = 1e-6;
const size_t VARS = 3;

class Monom;

template <typename T>
class List {
public:
	friend void merging(List<Monom> &list, size_t l, size_t m, size_t r);
	friend class Polynom;
	List() : head(nullptr), tail(nullptr), size(0) {}
	~List();
	List(const List<T> &c);
	//List(List<T> &&c) = default;
	List &operator=(const List<T> &c);
	//List &operator=(List<T> &&c) = default;
	//T operator[](size_t index) { return get_node(index)->value; }
	T &operator[](size_t index) const { return get_node(index)->value; }
	size_t get_size() const noexcept { return size; }
	void push_back(const T &value) {
		if (size == 0) {
			tail = new Node(value, nullptr, nullptr);
			head = tail;
		}
		else {
			tail->next = new Node(value, nullptr, tail);
			tail = tail->next;
		}
		size++;
	}
	void push_front(const T &value) {
		if (size == 0) {
			head = new Node(value, nullptr, nullptr);
			tail = head;
		}
		else {
			head->prev = new Node(value, head, nullptr);
			head = head->prev;
		}
		size++;
	}
	T pop_back() {
		T res;
		if (size == 0) {
			throw std::runtime_error("Pop empty list");
		}
		else if (size == 1) {
			res = tail->value;
			delete tail;
			head = nullptr;
			tail = nullptr;
		}
		else {
			res = tail->value;
			Node *prevtail = tail->prev;
			prevtail->next = nullptr;
			delete tail;
			tail = prevtail;
		}
		size--;
		return res;
	}
	T pop_front() {
		T res;
		if (size == 0) {
			throw std::runtime_error("Pop empty list");
		}
		else if (size == 1) {
			res = head->value;
			delete head;
			head = nullptr;
			tail = nullptr;
		}
		else {
			res = head->value;
			Node *nexthead = head->next;
			nexthead->prev = nullptr;
			delete head;
			head = nexthead;
		}
		size--;
		return res;
	}
	void push(size_t index, const T &value);
	T pop(size_t index);

private:
	class Node {
	public:
		Node(T inpvalue, Node *inpnext, Node *inpprev) : value(inpvalue), next(inpnext), prev(inpprev) {}
		T value;
		Node *next;
		Node *prev;
	};
	Node *get_node(size_t index) const {
		if (index >= size) {
			throw std::out_of_range("List out of bounds");
		}
		if (2 * index > size) {
			Node *curnode = tail;
			for (size_t i = size - 1; i > index; i--) {
				curnode = tail->prev;
			}
			return curnode;
		}
		else {
			Node *curnode = head;
			for (size_t i = 0; i < index; i++) {
				curnode = curnode->next;
			}
			return curnode;
		}
	}
	Node *head;
	Node *tail;
	size_t size;
};

// Monom - const*x^n*y^m*z^p

class Monom {
public:
	Monom() : coef(0.0) {
		for (size_t i = 0; i < VARS; i++) {
			degs[i] = 0;
		}
	}
	Monom(double inpcoef, char *inpdegs);
	friend std::ostream &operator<<(std::ostream &stream, const Monom &m);
	bool can_sum(const Monom &sec) const noexcept;
	int get_degs() const noexcept {
		int res = 0;
		for (size_t i = 0; i < VARS; i++) {
			res *= 10;
			res += static_cast<int>(degs[i]);
		}
		return res;
	}
	double get_coef() const noexcept { return coef; }
	void set_coef(double num) noexcept { coef = num; }
	Monom operator*(const Monom &sec) const;
	Monom operator*(double num) const { return Monom(coef * num, const_cast<char*>(degs)); }
	bool operator==(const Monom &sec) const noexcept;
	bool operator!=(const Monom &sec) const noexcept { return !(*this == sec); }
private:
	double coef;
	char degs[VARS];
};

class Polynom {
public:
	Polynom() : monoms() {}
	Polynom(const List<Monom> &inpmonoms);
	Polynom operator+(const Polynom &sec) const;
	Polynom operator-(const Polynom &sec) const {
		return (*this + sec * (-1.0));
	}
	Polynom operator*(const Polynom &sec) const;
	Polynom operator*(const Monom &sec) const {
		Polynom res(*this);
		List<Monom>::Node *curnode = res.monoms.head;
		for (size_t i = 0; i < res.monoms.size; i++) {
			curnode->value = curnode->value * sec;
			curnode = curnode->next;
		}
		return res;
	}
	Polynom operator*(double num) const noexcept {
		Polynom res(*this);
		List<Monom>::Node *curnode = res.monoms.head;
		for (size_t i = 0; i < res.monoms.size; i++) {
			curnode->value = curnode->value * num;
			curnode = curnode->next;
		}
		return res;
	}
	friend std::ostream &operator<<(std::ostream &stream, const Polynom &poly);
private:
	List<Monom> monoms;
};

template <typename T>
List<T>::List(const List<T> &c) : size(c.size) {
	if (size == 0) {
		head = nullptr;
		tail = nullptr;
	}
	else {
		head = new Node(c.head->value, nullptr, nullptr);
		Node *curnode = c.head->next;
		Node *prevnode = head;
		while (curnode != nullptr) {
			prevnode->next = new Node(curnode->value, nullptr, prevnode);
			prevnode = prevnode->next;
			curnode = curnode->next;
		}
		tail = prevnode;
	}
}

template <typename T>
List<T>::~List() {
	if (size > 0) {
		Node *curnode = head;
		while (curnode->next != nullptr) {
			curnode = curnode->next;
			delete curnode->prev;
		}
		delete curnode;
		head = nullptr;
		tail = nullptr;
		size = 0;
	}
}

template <typename T>
List<T> &List<T>::operator=(const List<T> &c) {
	if (&c == this) {
		return *this;
	}
	if (size > 0) {
		Node *curnode = head;
		while (curnode->next != nullptr) {
			curnode = curnode->next;
			delete curnode->prev;
		}
		delete curnode;
	}
	size = c.size;
	if (size == 0) {
		head = nullptr;
		tail = nullptr;
	}
	else {
		head = new Node(c.head->value, nullptr, nullptr);
		Node *curnode = c.head->next;
		Node *prevnode = head;
		while (curnode != nullptr) {
			prevnode->next = new Node(curnode->value, nullptr, prevnode);
			prevnode = prevnode->next;
			curnode = curnode->next;
		}
		tail = prevnode;
	}
	return *this;
}

template <typename T>
void List<T>::push(size_t index, const T &value) {
	if (index == 0) {
		push_front(value);
	}
	else if (index == size) {
		push_back(value);
	}
	else {
		Node *curnode = get_node(index);
		Node *newnode = new Node(value, curnode, curnode->prev);
		curnode->prev->next = newnode;
		curnode->prev = newnode;
		size++;
	}
}

template <typename T>
T List<T>::pop(size_t index) {
	T res;
	if (index == 0) {
		res = pop_front();
	}
	else if (size != 0 && index == size-1) {
		res = pop_back();
	}
	else {
		Node *curnode = get_node(index);
		res = curnode->value;
		curnode->prev->next = curnode->next;
		curnode->next->prev = curnode->prev;
		delete curnode;
		size--; 
	}
	return res;
}

void merging(List<Monom> &list, size_t l, size_t m, size_t r);
void merge_sorting(List<Monom> &list, size_t l, size_t r);
