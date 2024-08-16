#pragma once

/*
	C++函数模板

	函数模板 <= 类型不知道,不进行编译
	模板实例化 <= 函数调用点实例化
	模板函数 <= 编译器编译

	模板类型参数
	模板非类型参数(非常重要) 都是常量,只能使用,不能修改;


	模板实参推演 <= 函数调用点进行模板参数推演
	模板的特化 <= 特殊的实例化,用户提供
	非模板函数 <= (需要推演时,先调用这个函数;优先找函数地址;)
	重载(函数名相同,参数列表不同)

	模板实例化之前一定要看到模板定义的地方;
	或者直接在定义的文件中实例化;
	才能正常实例化;

*/

/*
	在函数调用点,编译器使用用户指定类型,从原模版实例化一份代码出来;
	3.函数名,函数调用点===>
	4.实例化一份(并且只实例化一份)模板函数出来;
	5.实参推演;
*/

template<typename T,typename E>//	1.模板参数列表
bool compare(T a, T b) { //	2.compare是一个函数模板
	cout << "template compare" << endl;
	return a > b; 
}

//	6.针对compare函数模板,提供了const char*类型的特化	模板函数	版本
/*
template<>
bool compare(const char* a, const char* b) { 
	cout << "compare<const char*>" << endl;
	return strcmp(a, b) > 0; 
}
*/

//	7.非模板函数
bool compare(const char* a, const char* b) {
	cout << "compare<const char*>" << endl;
	return strcmp(a, b) > 0;
}

//	8.非类型参数	==	必须是整数类型地址/引用(常量),只能使用,不能修改;
template<typename T,int SIZE>
void sort(T* arr) {
	for (int i = 0; i < SIZE - 1; ++i) {
		for (int j = 0; j < SIZE - 1 - i; ++j) {
			if (arr[j] > arr[j + 1]) {
				T tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
		}
	}
}

void test_sort_template() {
	int arr[] = { 12,5,3,4,7,9,20,35 };
	const int size = sizeof(arr) / sizeof(arr[0]);
	sort<int, size>(arr);
	for (int v : arr) { cout << v << " "; }
	cout << endl;

	return;
}

/*
	C++类模板
		类模板==>实例化==>模板类

	
	//	默认类型参数
template<typename T = int>
class SeqStack {//类模板
public://	构造析构函数名不用加<T>,其他出现的地方需要加;
	SeqStack(int size = 10)
		:_pstack(new int[size])
		, _top(0)
		, _size(size)
	{}

	~SeqStack() {
		delete[]_pstack;
		_pstack = nullptr;
	}

	//	重写拷贝构造与赋值函数
	SeqStack(const SeqStack<T>& stack)
		:_top(stack._top)
		: _size(stack._size)
	{
		_pstack = new int[_size];
		for (int i = 0; i < _top; ++i) {
			_pstack[i] = stack._pstack[i];

		}
	}

	SeqStack<T>& operator=(const SeqStack<T>& stack) {
		if (this == &stack) { return *this; }
		delete[]_pstack;
		_top = stack._top;
		_size = stack._size;
		_pstack = new int[_size];

		for (int i = 0; i < _top; ++i) { _pstack[i] = stack._pstack[i]; }

		return *this;
	}

public:
	void push(const T& val);

	void pop() {
		if (empty()) { return; }
		--_top;

	}

	T top()const {
		if (empty()) { throw"stack is empty!"; }
		return _pstack[_top - 1];
	}

	bool full()const { return _top == _size; }
	bool empty()const { return _top == 0; }


private:
	T* _pstack;
	int _top;
	int _size;

	void expand() {
		T* ptmp = new T[_size * 2];
		for (int i = 0; i < _top; ++i) { ptmp[i] = _pstack[i]; }
		delete[]_pstack;
		_pstack = ptmp;
		_size *= 2;
	}
};

template<typename T>
void SeqStack<T>::push(const T& val) {
	if (full()) { expand(); }
	_pstack[_top++] = val;
}

void test_tem_seq() {
	//	类模板的选择性实例化,使用过的方法才会进行实例化;
	SeqStack<int>s1;
	s1.push(20);
	s1.push(78);
	s1.push(23);
	s1.push(15);
	s1.push(29);

	s1.pop();

	SeqStack<>s2;
	return;
}






*/

//	2.实现一个C++的STL_vector容器
/*
template<typename T>
class vector {
private:
	T* _first;
	T* _last;
	T* _end;

	void expend() {
		int size = _last - _first;
		int length = _end - _first;

		T* ptmp = new T[2 * size];

		for (int i = 0; i < size; ++i) { ptmp[i] = _first[i]; }

		delete[]_first;

		_first = ptmp;
		_last = _first + size;
		_end = _first + length;
	}


public:
	vector(int size = 10)
		:_first(new T[size])
		, _last(_first)
		, _end(_first + size)
	{}//这里有个缺点,无法控制真正构造对象的个数;

	~vector() { 
		delete[]_first; 
		_first = _last = _end = nullptr; 
	}

	vector(vector<T>& vec) {//	拷贝构造
		_first = new T[vec._end - vec._first];
		_last = _first + vec._last - vec._first;
		_end = _first + vec._end - vec._first;

		for (int i = 0; i < vec._end - vec._first; ++i) { _first[i] = vec._first[i]; }
	}

	vector<T>& operator=(const vector<T>& vec) { //	如何优化==================左引用或者是右引用;左	使用this进行优化;右	使用move语义

		if (this == &vec) { return *this; }

		delete[]_first;
		_first = _last = _end = nullptr;

		_first = new T[vec.capacity()];
		_last = _first + vec.size();
		_end = _first + vec.capacity();

		for (int i = 0; i < vec._end - vec._first; ++i) { _first[i] = vec._first[i]; }

		return *this;

	}


public:
	void push_back(const T& val) {
		if (full()) { expend(); }

		*(_last++) = val;
	}

	void pop_back() { 
		if (empty()) { return; }
		_last--; 
	}

	T& back()const { return *(_last - 1); }

	bool empty()const { return _first == _last; }
	bool full()const { return _last == _end; }

	int size()const { return _last - _first; }
	int capacity()const { return _end - _first; }

	void show() {
		int size = _last - _first;
		for (int i = 0; i < size; ++i)
			cout << _first[i] << "\t";
		cout << endl;
	}


};

void test_tem_vec() {

	::vector<int>vec;
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(4);
	vec.push_back(5);

	vec.show();
	vec.pop_back();
	vec.show();

	cout << vec.back() << endl;
}



*/

//	3.allocator空间适配器
//	allocator内存开辟,内存释放;对象构造,对象析构;不用new与delete
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

	void expand(){
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

};

class Test{
public:
	Test() { cout << "Test()" << endl; }
	~Test() { cout << "~Test()" << endl; }
	Test(const Test&) { cout << "Test(const Test&)" << endl; }
};

void test_allocator(){
	Test t1, t2, t3;
	cout << "----------------" << endl;
	::vector<Test>vec;
	vec.push_back(t1);
	vec.push_back(t2);
	vec.push_back(t3);
	cout << "----------------" << endl;
	vec.pop_back();	//	只需要析构对象,要把对象的析构和内存释放也分离开;非常重要
	cout << "----------------" << endl;

}

void test_template() {
	compare<int, int>(10, 20);
	compare("aaa", "bbb");
	test_sort_template();

	test_allocator();
	//test_tem_seq();

	//test_tem_vec();
}
