#pragma once
#include"Public.h"

//	1.CComplex复数类实现(运算符重载)
class CComplex {
private:
	int mreal;
	int mimage;

	//	友元函数
	friend CComplex operator+(const CComplex& lhs, const CComplex& rhs);
	friend ostream& operator<<(ostream& out, const CComplex& src);
	friend istream& operator>>(istream& out, CComplex& src);//	应该是非常量运算符

public:
	CComplex(int r = 0, int i = 0)
		:mreal(r), mimage(i) {}
	~CComplex() {}

public://	运算符重载
	//	如果返回指针引用内存必须不消失;
	CComplex operator+(const CComplex& src) {
		//	当前对象与传入对象,运算后返回
		CComplex comp;
		comp.mimage = this->mimage + src.mimage;
		comp.mreal = this->mreal + src.mreal;
		return comp;

		//	return CComplex(this->mreal + src.mreal, this->mimage + src.mimage);
	}
	//	后置运算符
	CComplex operator++(int) { return CComplex(mreal++, mimage++); }
	//	前置运算符
	CComplex& operator++() {
		mreal++;
		mimage++;
		return *this; 
	}

	void operator+=(const CComplex& src) {
		mreal += src.mreal;
		mimage += src.mimage;
	}

	void show()const { cout << "real: " << mreal << "image: " << mimage << endl; }

};

CComplex operator+(const CComplex& lhs, const CComplex& rhs) { return CComplex(lhs.mreal + rhs.mreal, lhs.mimage + rhs.mimage); }

ostream& operator<<(ostream& out, const CComplex& src) {
	out << "mreal: " << src.mreal << "mimage: " << src.mimage << endl;
	return out;
}

istream& operator>>(istream& in, CComplex& src) {
	in >> src.mreal >> src.mimage;
	return in;
}

void test_complex() {
	CComplex comp1(10, 10);
	CComplex comp2(20, 20);
	//	comp1.operator+(comp2)	运算符的重载函数
	CComplex comp3 = comp1 + comp2;
	//CComplex comp3 = comp1.operator+(comp2);
	comp3.show();
	CComplex comp4 = comp1 + 20;
	comp4.show();

	//	优先调用类成员方法,然后在全局作用域寻找运算符重在函数;	::operator(30,comp1);
	CComplex comp5 = 30 + comp1;
	comp5.show();

	CComplex comp6 = comp1++;
	comp6.show();
	comp1.show();

	comp6 = ++comp1;
	comp6.show();
	comp1.show();

	//	comp1.operator+=(comp2)		::operator+=(comp1,comp2)
	comp1 += comp2;
	comp1.show();
	//	cout ::operator<<(cout,comp1)
	//	ostream& operator<<(ostream &out,const CComplex &src)
	cout << comp1 << endl;

	cin >> comp1;

	cout << comp1 << comp2 << endl;


	return;
}

template<typename T>
void show(T a) { cout << a << endl; }

//	2.String类实现
#if 0
class String {//	自己实现一个字符串对象
public:
	bool operator>(const String& str)const { return strcmp(_pstr, str._pstr) > 0; }
	bool operator<(const String& str)const { return strcmp(_pstr, str._pstr) < 0; }
	bool operator==(const String& str)const { return strcmp(_pstr, str._pstr) == 0; }

	String& operator=(const String& src) {
		if (this == &src) { return *this; }

		delete[]_pstr;
		_pstr = new char[strlen(src._pstr) + 1];
		strcpy(_pstr, src._pstr);
		return *this;
	}

	int length()const { return strlen(_pstr); }
	const char* c_str()const { return _pstr; }


	//	char ch=str6[6];	str[6]='7';
	char& operator[](int index) { return _pstr[index]; }
	const char& operator[](int index)const { return _pstr[index]; }

public:
	String(const char* p = nullptr) {
		if (p != nullptr) {
			_pstr = new char[strlen(p) + 1];//strlen计算有效位置
			strcpy(_pstr, p);
		}
		else {
			_pstr = new char[1];
			*_pstr = '\0';
		}
	}

	~String() {
		delete[]_pstr;
		_pstr = nullptr;
	}

	String(const String& str) {
		_pstr = new char[strlen(str._pstr) + 1];
		strcpy(_pstr, str._pstr);
	}

private:
	char* _pstr;

	friend ostream& operator<<(ostream& out, const String& str);
	friend String operator+(const String& lhs, const String& rhs);


};

String operator+(const String& lhs, const String& rhs) {
	char* ptmp = new char[strlen(lhs._pstr) + strlen(rhs._pstr) + 1];
	strcpy(ptmp, lhs._pstr);
	strcat(ptmp, rhs._pstr);

	String tmp(ptmp);
	delete[]ptmp;

	//String tmp;
	//tmp._pstr = new char[strlen(lhs._pstr) + strlen(rhs._pstr) + 1];
	//strcpy(tmp._pstr, lhs._pstr);
	//strcat(tmp._pstr, rhs._pstr);
	return tmp;

	//return String(ptmp);	有问题move语义就可以
}

ostream&operator<<(ostream&out,const String&str){
	out << str._pstr;
	return out;
}

void test_String() {
	String str1;
	String str2 = "aaa";//	string(const char*)
	String str3 = "bbb";
	String str4 = str2 + str3;

	String str5 = str2 + "ccc";
	String str6 = "ddd" + str2;

	cout << "str6: " << str6 << endl;
	if (str5 > str6) { cout << str5 << " > " << str6 << endl; }
	else { cout << str5 << " < " << str6 << endl; }

	int len = str6.length();
	//	char& str6.operator[](i)
	for (int i = 0; i < len; ++i) { cout << str6[i] << " "; }
	cout << endl;

	// string -> char*
	char buf[1024] = { 0 };
	strcpy(buf, str6.c_str());
	cout << "buf: " << buf << endl;

}

#endif

//	3.String的iterator实现
#include<string>

//	迭代器是容器的嵌套类型,可以透明访问容器内部元素的值;
//  提供统一的方式透明遍历容器;
//  泛型算法,全局函数,给所有容器使用;统一遍历所有元素;

class String {//	自己实现一个字符串对象
public:
	bool operator>(const String& str)const { return strcmp(_pstr, str._pstr) > 0; }
	bool operator<(const String& str)const { return strcmp(_pstr, str._pstr) < 0; }
	bool operator==(const String& str)const { return strcmp(_pstr, str._pstr) == 0; }

	String& operator=(const String& src) {
		if (this == &src) { return *this; }

		delete[]_pstr;
		_pstr = new char[strlen(src._pstr) + 1];
		strcpy(_pstr, src._pstr);
		return *this;
	}

	int length()const { return strlen(_pstr); }
	const char* c_str()const { return _pstr; }


	//	char ch=str6[6];	str[6]='7';
	char& operator[](int index) { return _pstr[index]; }
	const char& operator[](int index)const { return _pstr[index]; }

public:
	String(const char* p = nullptr) {
		if (p != nullptr) {
			_pstr = new char[strlen(p) + 1];//strlen计算有效位置
			strcpy(_pstr, p);
		}
		else {
			_pstr = new char[1];
			*_pstr = '\0';
		}
	}

	~String() {
		delete[]_pstr;
		_pstr = nullptr;
	}

	String(const String& str) {
		_pstr = new char[strlen(str._pstr) + 1];
		strcpy(_pstr, str._pstr);
	}

private:
	char* _pstr;
	friend ostream& operator<<(ostream& out, const String& str);
	friend String operator+(const String& lhs, const String& rhs);

public:
	class iterator {
	private:
		char* _p;

	public:
		iterator(char* p = nullptr) :_p(p) {}
		bool operator!=(const iterator& it) { return _p != it._p; }
		//后置++
		void operator++(int) { _p++; }
		//前置++
		void operator++() { ++_p; }

		char& operator*() { return *_p; }

	};

	iterator begin() { return iterator(_pstr); }//首元素
	iterator end() { return iterator(_pstr + length()); }//末尾元素后继位置

};

String operator+(const String& lhs, const String& rhs) {
	char* ptmp = new char[strlen(lhs._pstr) + strlen(rhs._pstr) + 1];
	strcpy(ptmp, lhs._pstr);
	strcat(ptmp, rhs._pstr);

	String tmp(ptmp);
	delete[]ptmp;

	//String tmp;
	//tmp._pstr = new char[strlen(lhs._pstr) + strlen(rhs._pstr) + 1];
	//strcpy(tmp._pstr, lhs._pstr);
	//strcat(tmp._pstr, rhs._pstr);
	return tmp;

	//return String(ptmp);	有问题move语义就可以
}

ostream& operator<<(ostream& out, const String& str) {
	out << str._pstr;
	return out;
}

void test_String_iterator() {
	String str1 = "hello world!";
	for (String::iterator it = str1.begin(); it != str1.end(); ++it) { cout << *it << " "; }
	cout << endl;
	for (auto it = str1.begin(); it != str1.end(); ++it) { cout << *it << " "; }
	cout << endl;

	//	C++11	for_each底层通过begin end实现
	for (char ch : str1) { cout << ch << " "; }
	cout << endl;
}

//	4.实现一个C++的STL_vector容器
#if 0

//	4.1allocator空间适配器
//	4.2allocator内存开辟,内存释放;对象构造,对象析构;不用new与delete

/*	迭代器失效;
		1.增加以及删除点以及之后的全部失效,如果扩容就全部失效;
		2.对插入删除点的迭代器进行更新操作;
		3.不同容器的迭代器是不能进行比较运算的;

*/
template<typename T>
struct Allocator {
	//	负责内存开辟,size_t,unsigned int;malloc与free进行堆上内存的开辟以及释放;
	//	负责内存释放,void*可以与任意指针类型转换

	T* allocate(size_t size) { return (T*)malloc(sizeof(T) * size); }
	void deallocate(void* p) { free(p); }

	//	定位new,在指定的内存空间上进行构造
	//	代表T类型的析构函数,负责对象析构
	void construct(T* p, const T& val) { new (p) T(val); }
	void destroy(T* p) { p->~T(); }

};

template<typename T, typename Alloc = Allocator<T>>	//默认参数类型
class vector {
private:
	T* _first;
	T* _last;
	T* _end;

	Alloc _allocator;	//	定义容器的空间配置器对象

	void expand() {
		int size = _end - _first;
		T* ptmp = _allocator.allocate(2 * size);
		for (int i = 0; i < size; ++i) { _allocator.construct(ptmp + i, _first[i]); }

		for (T* p = _first; p != _last; ++p) { _allocator.destroy(p); }

		_allocator.deallocate(_first);
		_first = ptmp;
		_last = _first + size;
		_end = _first + 2 * size;
	}


public:
	vector(int size = 10) {// const Alloc& alloc = Allocator<T>();可以传一个自定义的空间配置器
		//	需要把内存开辟和对象构造分开处理
		//	_first = new T[size];//	挨个开辟空间,而不是构造对象
		_first = _allocator.allocate(size);
		_last = _first;
		_end = _first + size;
	}

	~vector() {
		//	析构容器有效的元素,然后释放整个容器底层的内存空间,释放_first指针指向的堆内存
		//	delete[] _first;//	挨个析构对象,并且删除内存
		for (T* p = _first; p != _last; ++p)		//	for_each算法,进行析构
			_allocator.destroy(p);	//	把_first指针指向数组的有效元素进行析构操作

		_allocator.deallocate(_first);	//	释放堆上的数组内存
		_first = _last = _end = nullptr;
	}

	vector(const vector<T>& rhs) {
		int size = rhs._end - rhs._first;

		_first = _allocator.allocate(size);
		int len = rhs._last - rhs._first;
		for (int i = 0; i < len; ++i) { _allocator.construct(_first + i, rhs._first[i]); }

		_last = _first + len;
		_end = _first + size;
	}

	vector<T>& operator=(const vector<T>& rhs) {
		if (this == &rhs)
			return *this;

		// 释放当前对象的内存
		for (T* p = _first; p != _last; ++p) { _allocator.destroy(p); }
		_allocator.deallocate(_first);

		// 分配新的内存并复制 rhs 中的元素
		int size = rhs._end - rhs._first;
		_first = _allocator.allocate(size);
		for (int i = 0; i < size; ++i) { _allocator.construct(_first + i, rhs._first[i]); }

		_last = _first + size;
		_end = _first + size;

		return *this;
	}

	void push_back(const T& val) {	//	向容器末尾添加元素

		if (full()) { expand(); }

		_allocator.construct(_last, val);
		_last++;
	}

	void pop_back() {//	不仅要把_last指针--,还需要析构删除元素
		if (empty()) { return; }

		--_last;
		_allocator.destroy(_last);
	}

	T back()const { return *(_last - 1); }
	bool full()const { return _last == _end; }
	bool empty()const { return _first == _last; }
	int size()const { return _last - _first; }
	T& operator[](int index) { 
		if (index < 0 || index >= size()) { throw"index outofrangeException"; }
		return _first[index]; 
	}

	//	#1迭代器一般实现成容器的嵌套类型
	class iterator {
	private:
		T* _ptr;

	public:
		iterator(T* ptr = nullptr)
			:_ptr(ptr) {}

		bool operator!=(const iterator& it)const { return _ptr != it._ptr; }
		void operator++() { _ptr++; }
		T& operator*()const { return *_ptr; }
		//const T& operator*()const { return *_ptr; }


	};
	//	begin()以及end()方法;
	iterator begin() { return iterator(_first); }
	iterator end() { return iterator(_last); }
};

void test_allocator_iterator() {
	::vector<int>vec;
	for (int i = 0; i < 20; ++i) { vec.push_back(rand() % 5 + 1); }
	for (auto it = vec.begin(); it != vec.end(); ++it) { cout << *it << " "; }
	cout << endl;

	for (auto v : vec) { cout << v << " "; }
	cout << endl;

}

#endif

#if 0

template<typename T>
struct Allocator {
	T* allocate(size_t size) { return (T*)malloc(sizeof(T) * size); }
	void deallocate(void* p) { free(p); }

	void construct(T* p, const T& val) { new (p) T(val); }
	void destroy(T* p) { p->~T(); }
};

template<typename T, typename Alloc = Allocator<T>>
class vector {
public:
	vector(int size = 10) {
		_first = _allocator.allocate(size);
		_last = _first;
		_end = _first + size;
	}

	~vector() {
		for (T* p = _first; p != _last; ++p) { _allocator.destroy(p); }

		_allocator.deallocate(_first);
		_first = _last = _end = nullptr;
	}

	vector(const vector<T>& rhs) {
		int size = rhs._end - rhs._first;

		_first = _allocator.allocate(size);
		int len = rhs._last - rhs._first;
		for (int i = 0; i < len; ++i) { _allocator.construct(_first + i, rhs._first[i]); }

		_last = _first + len;
		_end = _first + size;
	}

	vector<T>& operator=(const vector<T>& rhs) {
		if (this == &rhs)
			return *this;

		for (T* p = _first; p != _last; ++p) { _allocator.destroy(p); }
		_allocator.deallocate(_first);

		int size = rhs._end - rhs._first;
		_first = _allocator.allocate(size);
		for (int i = 0; i < size; ++i) { _allocator.construct(_first + i, rhs._first[i]); }

		_last = _first + size;
		_end = _first + size;

		return *this;
	}

	void push_back(const T& val) {
		if (full()) { expand(); }

		_allocator.construct(_last, val);
		_last++;
	}

	void pop_back() {
		if (empty()) { return; }

		// erase(it);	verify(it._pstr, _last);
		// insert(it, val);		verify(it._pstr, _last);

		verify(_last - 1, _last);
		--_last;
		_allocator.destroy(_last);
	}

	T back()const { return *(_last - 1); }
	bool full()const { return _last == _end; }
	bool empty()const { return _first == _last; }
	int size()const { return _last - _first; }
	T& operator[](int index) {
		if (index < 0 || index >= size()) { throw"index outofrangeException"; }
		return _first[index];
	}

	class iterator {
	public:
		friend class vector<T, Alloc>;
		//	底层为一个链表结构
		iterator(vector<T, Alloc>* pvec = nullptr, T* ptr = nullptr)
			:_ptr(ptr), _pVec(pvec) 
		{
			Iterator_Base* itb = new Iterator_Base(this, _pVec->_head._next);
			_pVec->_head._next = itb;
		}

		bool operator!=(const iterator& it)const { 
			if (_pVec == nullptr || _pVec != it._pVec) {
				throw"iterator incompatable!";
				return false;
			}
			return _ptr != it._ptr; 
		}

		void operator++() { 
			if (_pVec == nullptr) { throw"iterator invalid!"; }
			_ptr++; 
		}

		const T& operator*()const {
			if (_pVec == nullptr) { throw"iterator invalid!"; }
			return *_ptr; 
		}

	private:
		T* _ptr;
		vector<T, Alloc>* _pVec;
	};

	iterator begin() { return iterator(this, _first); }
	iterator end() { return iterator(this, _last); }

	void verify(T* first, T* last) {
		Iterator_Base* pre = &this->_head;
		Iterator_Base* it = this->_head._next;
		while (it != nullptr) {
			if (it->_cur->_ptr > first && it->_cur->_ptr <= last) {
				// 迭代器失效,把iterator持有的容器置nullptr;
				it->_cur->_pVec = nullptr;
				// 删除当前迭代器节点,继续判断后面迭代器节点是否失效;
				pre->_next = it->_next;
				delete it;
				it = pre->_next;
			}
			else {
				pre = it;
				it = it->_next;
			}
		}
	}

	//	自定义vector容器insert方法实现
	iterator insert(iterator it, const T& val) {
		//1.不考虑扩容;verify(_first - 1, _last);2.不考虑it._ptr指针的合法性;
		verify(it._ptr - 1, _last);
		T* p = _last;
		while (p > it._ptr) {
			_allocator.construct(p, *(p - 1));
			_allocator.destory(p - 1);
			--p;
		}

		_allocator.construct(p, val);
		_last++;

		return iterator(this, p);
	}

	iterator erase(iterator it) {
		verify(it._ptr - 1, _last);
		T* p = it._ptr;
		while (p < _last - 1) {
			_allocator.destory(p);
			_allocator.construct(p, *(p + 1));
			++p;
		}

		_allocator.destory(p);
		_last--;

		return iterator(this, it._ptr);
	}


private:
	T* _first;
	T* _last;
	T* _end;

	Alloc _allocator;

	//	容器迭代器失效增加代码
	struct Iterator_Base {
		Iterator_Base(iterator* c = nullptr, Iterator_Base* n = nullptr)
			:_cur(c), _next(n) {}

		iterator* _cur;
		Iterator_Base* _next;
	};

	Iterator_Base _head;

	void expand() {
		int size = _end - _first;
		T* ptmp = _allocator.allocate(2 * size);
		for (int i = 0; i < size; ++i) { _allocator.construct(ptmp + i, _first[i]); }

		for (T* p = _first; p != _last; ++p) { _allocator.destroy(p); }

		_allocator.deallocate(_first);
		_first = ptmp;
		_last = _first + size;
		_end = _first + 2 * size;
	}


};

void test_allocator_iterator() {
	::vector<int>vec(200);
	for (int i = 0; i < 20; ++i) { vec.push_back(rand() % 20 + 1); }
	for (auto it = vec.begin(); it != vec.end(); ++it) { cout << *it << " "; }
	cout << endl;
	
	/*
	auto it1 = vec.end();
	vec.pop_back();	//	verify(_last-1,_last);
	auto it2 = vec.end();
	cout << (it1 != it2) << endl;
	*/

	/*
	for (auto it = vec.begin(); it != vec.end(); ++it) { 
		if (*it % 2 == 0) {
			it = vec.insert(it, *it - 1);
			++it;
		}
	}
	cout << endl;


	for (auto it = vec.begin(); it != vec.end(); ++it) { cout << *it << " "; }
	cout << endl;
	*/
}

#endif


//	5.new与delete操作符进行重载以及实现

//	内存管理的实现方法,内存池,这样会自己构建一个new以及delete
//	内存、对象、链接、线程、进程(提升效率)
//	内存、对象:(减少频繁的动态内存分配和释放操作，从而提高程序的性能;这在需要高效处理大量对象创建和销毁的情况下特别有效)

/*
1.new delete的实质
	new -> operator new;delete -> operator delete;		===========>	new以及delete都是自动调用构造与析构函数
2.malloc与new的区别
	a.malloc是库函数,负责开辟连续内存，返回类型为void*,失败返回nullptr;
	  new是操作符,开辟内存需要指定类型,并且调用该类的构造函数,失败返回bad_alloc异常;
3.free与delete的关系
	a.free是库函数,释放内存;
	  delete是操作符,并且调用该类的析构函数,释放内存;

4.对于C++的内置类型,因为没有构造函数以及析构函数,数据在内存中的位置结构是类似的;
  因此可以混用new|delete[]、new[]|delete;
5.对于C++的需要构造函数以及析构函数调用的类型,
  数据在内存的存储会再最前面多出4字节的个数纪录(为了正确调用析构函数),因此不能混用;new|delete、new[]|delete[];
  但是依然会将指针指向第一个对象的起始地址;
*/



//	====本身就是静态方法====

//	首先调用operator new开辟内存空间,然后调用对象的构造函数(初始化);
void* operator new(size_t size) {
	void* p = malloc(size);
	if (p == nullptr)
		throw bad_alloc();
	cout << "operator new addr:" << p << endl;
	return p;
}

//  delete p;调用p指向对象的析构函数、在调用operator delete释放内存空间
void operator delete(void* ptr) {
	cout << "operator delete addr:" << ptr << endl;
	free(ptr);
}

void* operator new[](size_t size) {
	void* p = malloc(size);
	if (p == nullptr)
		throw bad_alloc();
	cout << "operator new[] addr:" << p << endl;//	输出为全部起始地址
	return p;//	返回首个元素的起始位置
}

void operator delete[](void* ptr) {
	cout << "operator delete[] addr:" << ptr << endl;
	free(ptr);
}

class Test {
public:
	Test(int data = 10) { cout << "Test()" << endl; }
	~Test() { cout << "~Test()" << endl; }
private:
	int ma;
};

//	混用与否
int test_new_delete() {

	try {
		int* p = new int;
		delete p;
		p = nullptr;

		int* q = new int[10];
		delete[]q;
		q = nullptr;
	}
	catch (const bad_alloc& err)//将bad_alloc的消息传递给cerr
	{
		cerr << err.what() << endl;//看到内存分配错误的具体信息
	}

	Test* p1 = new Test();  // 只有一个对象
	delete p1;	//	delete[] 从个数的位置进行复位

	Test* p2 = new Test[5];		//===========,对象个数+对象的内存,返回的是对象的起点位置
	cout << "p2:" << p2 << endl;

	//	delete p2---->表示将一个对象进行析构,直接free(p2),从对象起始位置,
	//也就是说因为有个数纪录,因此会从起始位置进行复位
	delete[] p2;

	return 0;
}

void test_operator() {
	//test_complex();
	//test_String();
	//test_String_iterator();

	//test_allocator_iterator();

	test_new_delete();
}
