#pragma once
#include"Public.h"

/*
	stl	6大组件;
	容器
		array	forward_list

		vector	deque	list
		stack	queue	priority_queue

		unorder_map				map
		unorder_set				set
		unorder_multimap		multimap
		unorder_multiset		multiset

	迭代器
		iterator		const_iterator
		reverse_iterator		const_reverse_iterator

	函数对象	函数器	lambda(重点掌握)

	泛型算法(重点掌握)

*/

/*
		底层数据结构(扩容)			增加|删除(连续)复杂度			查询(随机访问、迭代器)		内存使用(效率)
vector	size().empty().reserve().resize().swap
deque	区别
list	区别

						为什么使用某种底层容器(封装实现)					迭代器(没有)
stack					效率、内存
queue				
priority_queue			动态数组构建堆


*/

#include<vector>
#include<deque>
#include<list>

#include<stack>
#include<queue>

#include<set>
#include<map>

#include<functional>
#include<algorithm>

//	1.vector deque list	stack queue priority_queue	map|set
void test_vector() {
	vector<int>vec;
	vec.reserve(20);
	//	vec.resize(20);全部是0
	//	vector<int>old_vec;
	//	vec.swap(old_vec);

	for (int i = 0; i < 20; ++i) { vec.push_back(rand() % 100 + 1); }
	for (vector<int>::iterator it = vec.begin(); it != vec.end(); ++it) { cout << *it << " "; }
	cout << endl;

	//	删除所有偶数
	for (vector<int>::iterator it = vec.begin(); it != vec.end();) { 
		if (*it % 2 == 0) { it = vec.erase(it); }
		else { ++it; }
	}

	for (vector<int>::iterator it = vec.begin(); it != vec.end(); ++it) { cout << *it << " "; }
	cout << endl;

	//	在前面插入小于1的奇数
	for (vector<int>::iterator it = vec.begin(); it != vec.end(); ++it) {
		if (*it % 2 != 0) { 
			it = vec.insert(it, *it - 1); 
			++it;
		}
	}

	for (vector<int>::iterator it = vec.begin(); it != vec.end(); ++it) { cout << *it << " "; }
	cout << endl;
}

template<typename T,typename Container=deque<T>>
class Stack {
public:
	void push(const T& val) { con.push_back(val); }
private:
	Container con;
};

class Student {
private:
	int _id;
	string _name;
	//	输出函数声明
	friend ostream& operator<<(ostream& out, const Student& stu);

public:
	Student(int id = 0, string name = "")
		:_id(id)
		, _name(name)
	{}

	//	内部运算符重载,比较大小,小在前
	bool operator<(const Student& stu)const { return _id < stu._id; }
};

ostream& operator<<(ostream& out, const Student& stu) {
	out << "id:" << stu._id << " name:" << stu._name << endl;
	return out;
}

void test_set() {
	set<int> set1;
	for (int i = 0; i < 20; ++i) { set1.insert(rand() % 10 + 1); }
	for (int v : set1) { cout << v << " "; }
	cout << endl;

	//	定义如何排序,需要在类内部定义排序方法;
	set<Student>set2;
	set2.insert(Student(1000, "zhangwen"));
	set2.insert(Student(1020, "liguang"));

	for (set<Student>::iterator it = set2.begin(); it != set2.end(); ++it) { cout << *it << endl; }

	//	使用map,使用第一个进行比较大小
	map<int, Student>stuMap;
	stuMap.insert({ 1000,Student(1000,"zhangwen") });
	stuMap.insert({ 1020,Student(1020,"liguang") });
	stuMap.insert({ 1030,Student(1030,"gaoyang") });

	//	stuMap.erase(it)	stuMap.erase(1020)
	cout << stuMap[1020] << endl;

	for (map<int, Student>::iterator it = stuMap.begin(); it != stuMap.end(); ++it) { 
		cout << "key:" << it->first << "value:" << it->second << endl; 
	}
	cout << endl;
}

//	2.iterator const_iterator reverse_iterator const_reverse_iterator
void test_iterator() {
	/*
		class const_iterator {
		public://	常引用
			const T& operator*() { return *_ptr; }

		};

		class iterator :public const_iterator {
		public://	普通引用
			T& operator*() { return *_ptr; }

		};
	*/
	vector<int>vec;
	vec.reserve(20);

	for (int i = 0; i < 20; ++i) { vec.push_back(rand() % 100 + 1); }
	for (vector<int>::reverse_iterator it = vec.rbegin(); it != vec.rend(); ++it) { cout << *it << " "; }
	cout << endl;

	//	删除所有偶数
	for (vector<int>::iterator it = vec.begin(); it != vec.end();) {
		if (*it % 2 == 0) { it = vec.erase(it); }
		else { ++it; }
	}

	for (vector<int>::reverse_iterator it = vec.rbegin(); it != vec.rend(); ++it) { cout << *it << " "; }
	cout << endl;

	//	在前面插入小于1的奇数
	for (vector<int>::iterator it = vec.begin(); it != vec.end(); ++it) {
		if (*it % 2 != 0) {
			it = vec.insert(it, *it - 1);
			++it;
		}
	}

	for (vector<int>::reverse_iterator it = vec.rbegin(); it != vec.rend(); ++it) { cout << *it << " "; }
	cout << endl;
}

//	3.functor
/*
	函数对象(有()重载函数的对象)=>函数指针

	class sum {//	调用()重载方法
	public:
		int operator()(int a, int b) { return a + b; }
	};

	//	调用地址
	int sum(int a, int b) { return a + b; }

*/

//	function<bool(int, int)>函数封装器;functor;*func;lambda;

//	使用函数指针会有函数地址调用开销,没法内联
template<typename T>
bool less(T a, T b) { return a < b; }

template<typename T>
bool greater(T a, T b) { return a > b; }

//	函数对象,可以内联省略调用开销
template<typename T>
class mygreater {
public:
	bool operator()(T a, T b) { return a > b; }
};

template<typename T>
class myless {
public:
	bool operator()(T a, T b) { return a < b; }
};

//	函数模板,类型参数Compare为参数类型(可以放入指针、仿函数、lambda表达式)
template<typename T,typename Compare>
bool compare(T a, T b, Compare comp) {  return comp(a, b); }

void test_functor() {
	cout << compare(10, 20, ::mygreater<int>()) << endl;
	cout << compare(10, 20, ::mygreater<int>()) << endl;
	cout << compare('a', 'b', ::mygreater<int>()) << endl;
}

//	4.algorithm
/*
	template + iterator + functor

	sort find find_if binary_search for_each
	1.接收的参数都是迭代器
	2.还可以接收functor、*func、function<bool(int, int)>、lambda;

	绑定器 + 二元函数对象	=	一元函数对象
	封装器

	bind1st(greater<int>(), 55);
	[](T val)->{return asdf;}

	bind1st:将二元函数对象的operator()第1个形参绑定起来
	bind2st:将二元函数对象的operator()第2个形参绑定起来


	C++prime
*/

void test_stl() {
	test_vector();
	test_set();
	test_iterator();

	test_functor();
}
