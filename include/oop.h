#pragma once
#include"Public.h"

#if 0

/*
	oop编程(抽象 封装 继承 多态)	struct(public) class(private)
		1.类可以定义无数对象,共用一套成员方法(在代码段上),都有自己的成员变量;
		2.对象内存大小只与数据有关,与方法无关,(内存对齐,与编译器以及硬件有关,cpu读取内存方式减少IO次数;最小对齐数)
		3.类体内实现的函数自动inline,friend func(友元函数);类外定义方法不可以;

		（1）CPU周期
			 WIN vs qt 默认8字节对齐
			 Linux 32位 默认4字节对齐，64位默认8字节对齐
		（2）结构体最大成员(基本数据类型变量)
		（3）预编译指令#pragma pack(n)手动设置 n只能填1 2 4 8
	
	this指针
		1.以类为载体,调用成员方法,编译|调用阶段通过加入this指针来指定数据以及成员方法;

	构造|析构函数
		1.定义对象:开辟内存、调用构造函数(可以重载);	出作用域自动调用析构函数(只有1个);	先构造的后析构;
		1.1构造函数定义后,不会出现默认构造;		//0xcccccccc
		2.	.data	.heap(new	delete	空间以及构造)	.stack
		3.	初始化列表(定义时初始化对于类更有效果,没有栈帧开辟)是以定义的顺序开始的,与初始化列表顺序无关(构造函数体);

	成员(对象调用,传递地址)
		1.普通成员方法
			类作用域、依赖对象(常对象无法调用)、成员方法任意访问私有成员;
		2.静态成员方法
			类作用域、类名作用域调用方法、任意访问私有成员(仅限于不依赖对象的成员,只能调用其他静态成员);CGoods::countNum();没有this指针;
			static成员变量,所有对象都拥有;相当于全局变量属于类作用域,用类名作用域调用,一定在类外定义并且初始化;
		3.常成员方法
			属于类作用域,依赖普通或者常对象,可以任意访问私有成员(只读不写);
			传递的对象地址是常地址;因此只能调用常方法,或者被其他方法调用;
			只读方法实现成const;

	指向类成员(成员变量、成员方法)的指针
		1.外部指针,指向成员(需要指定)作用域;静态不依赖对象,因此不用加入作用域;

	深|浅拷贝
		浅拷贝就是只有栈上的拷贝;
		深拷贝是堆上的拷贝;

*/

class Test {
public:
	void func() { cout << "call Test::func" << endl; }	//	void (Test::*pfunc)()=&Test::func;
	static void static_func() { cout << "call Test::static_func" << endl; }	//	void (*pstatic_func)()=&Test::static_func;

	int ma;	//	int Test::*
	static int mb;	//	不依赖于对象	int *p1 = &Test::mb;
};


void test_Test() {
	Test t1;
	Test* t2 = new Test();


	int Test::* p = &Test::ma;	//	指向作用域中的指针
	t1.*p = 20;
	cout << t1.*p << endl;

	t2->*p = 30;
	cout << t2->*p << endl;


	delete t2;

}



//	6.string实现
#if 0

#include <cstring> // 为了使用 strlen, strcpy, strcat
//std也包括ostream,istream;作用域调用需要研究一下,名空间的问题
using namespace std;//std之中会有string这样就回产生自定义冲突,所以应该包括作用域

class string {
public:
	string() : _pstr(nullptr) {}

	string(const char* str) {
		if (str) {
			_pstr = new char[strlen(str) + 1];
			strcpy(_pstr, str);
		}
		else {
			_pstr = nullptr;
		}
	}

	~string() {
		delete[] _pstr;
	}

	string(const string& other) {
		if (other._pstr) {
			_pstr = new char[strlen(other._pstr) + 1];
			strcpy(_pstr, other._pstr);
		}
		else {
			_pstr = nullptr;
		}
	}

	/*
	//	const 的具体用法,不改变类对象的数据,常量在编译阶段就已经处理完毕
	//	strcmp	小中大,会具体变化为负数、0、正数
	//	strlen(str)字符串长度,不包括最后的null
	//strcpy就是从第一个空字符开始复制,最后会有null 结束符
	//strcat 是一个用于将一个字符串（源字符串）连接到另一个字符串（目标字符串）的标准C库函数。
	//它会将源字符串的内容追加到目标字符串的末尾，并且会在目标字符串的末尾添加一个 null 结束符，以确保生成的字符串仍然是以 null 结束的
	*/
	string& operator=(const string& other) {
		if (this != &other) {
			delete[] _pstr;
			if (other._pstr) {
				_pstr = new char[strlen(other._pstr) + 1];
				strcpy(_pstr, other._pstr);
			}
			else {
				_pstr = nullptr;
			}
		}
		return *this;
	}

	bool operator<(const string& str) const {
		if (!_pstr || !str._pstr) return false;
		return strcmp(_pstr, str._pstr) < 0;
	}

	bool operator>(const string& str) const {
		if (!_pstr || !str._pstr) return false;
		return strcmp(_pstr, str._pstr) > 0;
	}

	bool operator==(const string& str) const {
		if (!_pstr || !str._pstr) return false;
		return strcmp(_pstr, str._pstr) == 0;
	}

	char operator[](int index) const {
		if (_pstr && index >= 0 && index < strlen(_pstr)) {
			return _pstr[index];
		}
		return '\0'; // 或者抛出异常
	}

	int length() const {
		if (_pstr) {
			return strlen(_pstr);
		}
		return 0;
	}

	const char* c_str() const {
		return _pstr;
	}

	class iterator {
	public:
		iterator(char* p = nullptr) : _p(p) {}

		bool operator!=(const iterator& it) {
			return _p != it._p;
		}

		void operator++() {
			++_p;
		}

		char& operator*() {
			return *_p;
		}

	private:
		char* _p;
	};

	iterator begin() { return iterator(_pstr); }

	iterator end() { return iterator(_pstr + length()); }

	friend string operator+(const string& lhs, const string& rhs);

	friend std::ostream& operator<<(std::ostream& out, const string& str);

private:
	char* _pstr;
};

::string operator+(const ::string& lhs, const ::string& rhs) {
	::string tmp;
	tmp._pstr = new char[lhs.length() + rhs.length() + 1];
	strcpy(tmp._pstr, lhs._pstr);
	strcat(tmp._pstr, rhs._pstr);
	return tmp;
}

std::ostream& operator<<(std::ostream& out, const ::string& str) {
	out << str._pstr;
	return out;
}

int main() {
	::string s1("你好");
	::string s2("，世界！");
	::string s3 = s1 + s2;

	cout << "s1: " << s1 << std::endl;
	cout << "s2: " << s2 << std::endl;
	cout << "s3: " << s3 << std::endl;

	std::cout << "遍历 s3:" << std::endl;
	for (auto it = s3.begin(); it != s3.end(); ++it) {
		std::cout << *it;
	}
	std::cout << std::endl;

	return 0;
}

#endif


//	7.对象池Queue
#if 0

//	改变自定义对象的内存管理方式,对象池

#include<iostream>
using namespace std;

template<typename T>
class Queue {
public:
	Queue() { _front = _rear = new QueueItem(); }
	~Queue() {

		QueueItem* cur = _front;
		while (cur != nullptr) {

			_front = _front->_next;
			delete cur;		//	调用内部设置的dalete操作符
			cur = _front;
		}
	}

	void push(const T& val) {	//	入队操作
		QueueItem* item = new QueueItem(val);
		_rear->_next = item;
		_rear = item;
	}

	void pop() {
		if (empty())
			return;

		QueueItem* first = _front->_next;	//	头删法
		_front->_next = first->_next;

		if (_front->_next == nullptr)
			_rear = _front;

		delete first;
	}

	T front()const { return _front->_next->_data; }

	bool empty()const { return _front == _rear; }

private:
	//	产生一个QueueItem的对象池(n个QueueItem的节点)
	struct QueueItem {
		QueueItem(T data = T()) :_data(data), _next(nullptr) {}//	对于单个节点的构造初始化

		//	对于单个节点自定义内存管理方式
		//	给QueueItem提供自定义内存管理,通过不同的内存开辟方式进行内存效率开辟
		void* operator new(size_t size) {

			if (_itemPool == nullptr) {

				_itemPool = (QueueItem*)new char[POOL_ITEM_SIZE * sizeof(QueueItem)];
				QueueItem* p = _itemPool;

				for (; p < _itemPool + POOL_ITEM_SIZE - 1; ++p)
					p->_next = p + 1;	//	对象池元素产生相关联指针

				p->_next = nullptr;
			}

			QueueItem* p = _itemPool;
			_itemPool = _itemPool->_next;
			return p;//	operator new	主动返回指针,并且再改地址上进行对象构造
		}

		void operator delete(void* ptr) {

			QueueItem* p = (QueueItem*)ptr;
			p->_next = _itemPool;
			_itemPool = p;
		}

		T _data;
		QueueItem* _next;

		/*
		静态成员变量在类的所有对象中是共享的，
		它们不依赖于类的任何对象而存在，因此可以在不创建类对象的情况下访问和修改

		1.内存池的初始化： 在第一次需要分配 QueueItem 对象时，会通过 operator new 方法来分配一块连续的内存，
		  将其划分成多个 QueueItem 对象，并将它们链接成一个自由链表。

		2.内存的重复利用： 每当需要创建新的 QueueItem 对象时，都会先检查 _itemPool 是否有空闲的对象。
		  如果有，则直接从 _itemPool 中取出一个对象；如果没有，则从堆上分配新的内存块来扩展内存池。

		3.提升性能： 使用内存池可以减少频繁的动态内存分配和释放操作，从而提高程序的性能。
		  这在需要高效处理大量对象创建和销毁的情况下特别有效。

		*/

		static QueueItem* _itemPool;
		static const int POOL_ITEM_SIZE = 100000;//	表示池化的数量

	};

	QueueItem* _front;//头节点
	QueueItem* _rear;//指向队尾
};


template<typename T>
typename Queue<T>::QueueItem* Queue<T>::QueueItem::_itemPool = nullptr;		//	没有实例化,因此会有问题

int main()
{
	Queue<int> que;
	for (int i = 0; i < 100; ++i)
	{
		que.push(i);
	}

	while (!que.empty())
	{
		cout << que.front() << " ";
		que.pop();
	}
	cout << endl;

	return 0;
}

#endif



/*

class CGoods {
public:
	CGoods(const char* name, int amount) : _amount(amount) {
		_name = new char[strlen(name) + 1];
		strcpy(_name, name);
	}

	void show();

private:
	char* _name;
	int _amount;

	// 友元函数声明
	friend void show1(const CGoods& obj);
};

void CGoods::show() { cout << _name << "\t" << _amount << endl; }

void show1(const CGoods& obj) { cout << obj._name << "\t" << obj._amount << endl; }

int main() {
	CGoods good("cakes", 100);
	good.show(); // 输出：cakes   100
	show1(good); // 输出：cakes   100

	//外部指针,指向成员(需要指定)作用域;静态不依赖对象,因此不用加入作用域;
	//成员变量初始化是以定义的顺序开始的,与初始化列表无关;
	//static所有对象都拥有;作用域来调用;只能调用static成员;
	//常成员方法(参数指针化);调用常对象或者普通对象,只能读不能写;


	return 0;
}
*/

#endif
