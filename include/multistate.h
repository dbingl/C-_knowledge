#pragma once
#include"Public.h"

/*
	1.继承的本质与原理
	2.派生类的构造过程
	3.重载、隐藏、覆盖	overload、override、overwrite	、visual、动态绑定、静态绑定

	5.多态 vfptr和vftable
	6.抽象类的设计原理

	7.多重继承以及问题
	8.虚基类 vbptr和vbtable

	9.RTTI
	10.C++四种类型强转
*/

#if 0
/*
1.继承的本质以及原理
	类与类的关系:组合、继承;

	a.代码复用;
	b.在基类中体出统一的虚函数接口,派生类重写;可以使用动态多态;

	public、protected、private	访问权限问题;
	默认继承与派生类的类型class struct;
*/
class A {
public:
	int ma;
protected:
	int mb;
private:
	int mc;//	自己或友元可以访问
};

class B :public A {//	继承A基类,B派生类;作用域
public:
	int md;
protected:
	int me;
private:
	int mf;
};
#endif

#if 0
/*
2.派生类的生命周期(构造过程)
	在进行构造时,会出现一点问题;不能直接对基类成员进行初始化,应当使用基类构造函数;
	可以继承所有方法,继承不了构造与析构函数;基类资源由基类管理(基类构造与析构函数);
	先构造,后析构;基类先构造;(过程叙述	balabala...)

	2.1派生类调用基类构造函数,初始化基类成员;
	2.2调用派生类自己的构造函数,初始化派生类自己的成员;
	2.3	...派生类对象的作用域到期;
		调用派生类析构函数,释放派生类资源(堆/文件);
	2.4调用基类的构造函数,释放基类资源(堆/文件);
*/

class Base {
public:
	Base(int data) :ma(data) { cout << "Base()" << endl; }
	~Base() { cout << "~Base()" << endl; }
protected:
	int ma;
};

class Derive :public Base {
public:
	Derive(int data = 10) :Base(data), mb(data) { cout << "Derive()" << endl; }
	~Derive() { cout << "~Derive()" << endl; }
private:
	int mb;
};
#endif

#if 0
/*
3.重载、隐藏、覆盖
	重载:在同一作用域
	隐藏:(不同作用域)派生类同名方法,直接将基类方法隐藏;
	覆盖:visual关键字、虚函数

==============================================================================
a.类型(相互转换)->内存之中的调用(丢失与缺少)可以丢失,不能增加

基类对象 -> 派生类对象	(不行)
派生类对象 -> 基类对象	（可以）

基类指针|引用 -> 派生类对象	 (可以)
派生类指针|引用 -> 基类对象	 (不行)
*/

class Base {
public:
	Base(int data) :ma(data) { cout << "Base()" << endl; }
	void show() { cout << "Base::show()" << endl; }
	void show(int) { cout << "Base::show(int)" << endl; }
	~Base() { cout << "~Base()" << endl; }
protected:
	int ma;
};

class Derive :public Base {
public:
	Derive(int data = 20) :Base(data), mb(data) { cout << "Derive()" << endl; }

	//同名函数,无论参数是否相同,会直接将基类隐藏,优先调用派生类;或者直接使用作用域;d.Base::show();(除非进行类型转换,才能调用基类)
	void show() { cout << "Derive::show()" << endl; }
	~Derive() { cout << "~Derive()" << endl; }
private:
	int mb;
};

void test() {
	Derive d;
	d.show();
	d.::Baseshow();

	((Derive*)pb)->show();//可以将类型转换成派生进行调用
}

#endif

#if 0
#include<typeinfo>
/*
4.覆盖(重写)、visual、动态绑定、静态绑定
  a0.覆盖|重写:
	函数声明相同,直接与virtual作用,产生vftable,将相同函数进行覆盖;

  绑定(函数调用)
	  a1.静态绑定:编译时期(编译为汇编码)的绑定,直接调用地址
	  a2.动态绑定:编译时期,发现某个函数是virtual,就进行动态绑定(函数地址放入寄存器,再调用寄存器中地址)->在运行时期才知道寄存器具体调用的地址
	  a3.指针的具体作用(有无虚函数会对指针解引用的类型产生影响);

  有虚函数的影响
	  b.类中存在虚函数,在编译阶段,编译器会给每个 类 产生唯一一个vftable;
		vftable主要存储RTTI(Run-time type information),指向类型字符串,以及偏移量(vfptr在哪个位置);虚函数地址|指针;
	  c.程序运行时,vftable加载到.rodata; 对象内容(vfptr(虚函数起始位置) + 数据信息);一个类型定义的多个对象,vfptr指向vftable的同一个位置;
	  d.虚函数个数不影响对象大小(vftable和数据),只与vftable有关;
	  e.函数声明(返回值、函数名、参数)相同,直接与virtual作用,自动处理成虚函数,产生vftable,将相同函数进行	地址	覆盖	;

	  a0.发现某个函数是普通函数,进行静态绑定;call Base::show
	  a1.发现函数是虚函数,进行动态绑定;mov eax,dword ptr[pb]派生类的前四个字节,虚函数表的地址放在寄存器中;mov ecx,dword ptr[eax];call ecx(不知某个函数地址)
	  a2.	Base* pb=&d;	*pb识别的是编译或者运行的类型(与虚函数有关);

 5.虚析构函数
  a.哪些函数不能实现为虚函数
	虚函数依赖:(1.产生地址;2.必须存在对象使用vfptr进行调用);
	构造函数调用的任何函数都是静态绑定,不能成为虚函数(没有对象没有this指针的成员函数);
	static静态成员方法，不依赖于this;

  b.虚析构函数(可以)this存在;	(析构函数本身就同名,这样就调用派生类的析构！)
	在对象指针转换时,(这样就可以通过虚函数进行覆盖调用,因为有了虚函数表,这样就可以保证)可以保证将资源释放掉,避免内存泄漏;
	基类析构是虚函数,派生类自动成为虚析构函数;
	基类的指针|引用指向heap上new出来的派生类对象的时候,必须把基类的析构函数实现为虚析构函数;这样会动态绑定,否则派生类无法调用;

 6.真实的动态绑定
  a.虚函数的调用在什么情况下对函数地址的调用可以变成动态绑定
	1.构造函数调用方法一定是静态绑定;
	2.本对象调用一定是静态(作用域);只用在指针&调用才可以是动态;

*/

class Base {
public:
	Base(int data) :ma(data) { cout << "Base()" << endl; }

	//virtual方法,使用虚函数进行覆盖
	virtual void show() { cout << "Base::show()" << endl; }
	virtual void show(int) { cout << "Base::show(int)" << endl; }
	virtual ~Base() { cout << "~Base()" << endl; }
protected:
	int ma;
};

class Derive :public Base {
public:
	Derive(int data = 20) :Base(data), mb(data) { cout << "Derive()" << endl; }

	void show() { cout << "Derive::show()" << endl; }	//	函数声明相同,直接与virtual作用,产生vftable;

	~Derive() { cout << "~Derive()" << endl; }
private:
	int mb;
};

#endif

#if 0
/*
7.什么是多态
静态(编译时期, 得知调用函数的地址)的多态;
	a.重载、模板(函数 | 类)

动态(运行时期)的多态;
	通过动态绑定实现;
	a.基类 指针|引用指向派生类, 调用同名覆盖方法;(动态绑定)
*/

class Animal {
public:
	Animal(string name) : _name(name) {}
	virtual void bark() {}  // 声明为虚函数，需要定义才可以

protected:
	string _name;
};

class Cat : public Animal {
public:
	Cat(string name) : Animal(name) {}
	void bark() { cout << _name << " bark: miao miao!" << endl; }
};

class Dog : public Animal {
public:
	Dog(string name) : Animal(name) {}
	void bark() { cout << _name << " bark: wang wang!" << endl; }
};

class Pig : public Animal {
public:
	Pig(string name) : Animal(name) {}
	void bark() { cout << _name << " bark: heng heng!" << endl; }
};

//具体方法的API接口实现,增加功能可以复用	1.模板函数(外部函数api)		2.使用方法类

// 模板函数，接受 Animal 或其派生类的引用
//template<typename T>
//void bark(T& ami) { ami.bark(); }
void bark(Animal& ami) { ami.bark(); }//指针先访问前四个字节

int main() {
	Cat cat("猫咪");
	Dog dog("二哈");
	Pig pig("佩奇");

	bark(cat);
	bark(dog);
	bark(pig);

	return 0;
}

#endif

#if 0
/*
8.抽象类
	a.把什么类设计为抽象类:
		拥有纯虚函数;不可以实例化对象,可以定义指针以及引用变量;
		相同属性,相同的被复用的成员方法;
*/

class Car {
public:
	Car(string name, double oil) :_name(name), _oil(oil) {}
	double getLeftMiles() { return _oil * this->getMilesPerGallon(); }//	发生动态绑定
	string& getName() const{ return _name; }

protected:
	string _name;
	double _oil;
	virtual double getMilesPerGallon() = 0;//纯虚函数
};

class Bnze :public Car {
public:
	Bnze(string name, double oil) :Car(name, oil) {}
	double getMilesPerGallon() { return 20.0; }
};

class Audi :public Car {
public:
	Audi(string name, double oil) :Car(name, oil) {}
	double getMilesPerGallon() { return 18.0; }
};

class BMW :public Car {
public:
	BMW(string name, double oil) :Car(name, oil) {}
	double getMilesPerGallon() { return 19.0; }
};

//剩余路程的API
template<typename T>
void showCarLeftMiles(T& car) { cout << car.getName() << "\t" << "剩余公里" << "\t" << car.getLeftMiles() << endl; }


int main() {
	Bnze b1("奔驰", 20.0);
	Audi a("奥迪", 20.0);
	BMW b2("宝马", 20.0);

	showCarLeftMiles(b1);
	showCarLeftMiles(a);
	showCarLeftMiles(b2);

	return 0;
}

#endif

#if 0
/*
9.单|多重继承(一个基类被多次继承)
	a0.可以代码复用
	a1.菱形继承(间接继承)
10.虚基类 vbptr和vbtable===========(弄清楚内存布局)
	b.虚继承使用虚基类,virtual,虚继承放在最后;vbptr,向下偏移;这样就需要最后一个继承者进行初始化;
	  vbtable,(向上偏移,向下偏移量...2行),编译时期生成,运行放在.rodata,一个class一个vbtable;多个vbptr;

	  vfptr(第一个出现虚函数的首位)|vbptr(派生首位)存储位置;
	  内存变化,偏移的作用;
	  vftable|vbtable (RTTI、偏移量,指针)

	  基类指针指向派生类对象,永远指向基类起始位置;
*/

class A {
public:
	A(int data) :ma(data) { cout << "A()" << endl; }
	~A() { cout << "~A()" << endl; }
protected:
	int ma;
};

class B :virtual public A {
public:
	B(int data) :A(data), mb(data) { cout << "B()" << endl; }
	~B() { cout << "~B()" << endl; }

protected:
	int mb;
};

class C :virtual public A {
public:
	C(int data) :A(data), mc(data) { cout << "C()" << endl; }
	~C() { cout << "~C()" << endl; }

protected:
	int mc;
};

class D :public B, public C {
public:
	D(int data) :A(data), B(data), C(data), md(data) { cout << "D()" << endl; }
	~D() { cout << "~D()" << endl; }

protected:
	int md;
};

int main() {
	D d(10);

	return 0;
}

#endif

#if 0
/*
10.C++类型强转(4种)
	C. a=(T)b//可以转换为任意类型
	C++
		const_cast:去掉常量属性(只能),只能是指针|&
		static_cast:提供编译器安全的转换(需要有关系),基类|派生类===最常用
		reinterpret_cast:类似于C风格的强制类型转换
		dynamic_cast:继承结构,识别RTTI上下转换
*/

class Base {
public:
	virtual void func() = 0;
};

class Drive1 :public Base {
public:
	void func() { cout << "call Derive1::func" << endl; }
};

class Drive2 :public Base {
public:
	void func() { cout << "call Derive2::func" << endl; }
};

template<typename T>
void show(T* base) { (*base).func(); }

int main() {
	const int a = 10;
	int* p1 = (int*)&a;//可以转换为任意类型
	int* p2 = const_cast<int*>(&a);//去掉常量属性(只能),只能是指针|&

	double* b = reinterpret_cast<double*>(p1);

	Drive2 d1;
	Drive2 d2;

	//Base& _p1 = d1;
	//Base& _p2 = d2;
	Base* _p1 = dynamic_cast<Base*>(&d1);
	Base* _p2 = dynamic_cast<Base*>(&d2);

	//	dynamic_cast进行类型转换,会检查是否成功,如果不成功返回nullptr
	if (_p1 != nullptr && _p2 != nullptr) {
		show(_p1);
		show(_p2);
	}

	show(&d1);
	show(&d2);

	return 0;
}

#endif
