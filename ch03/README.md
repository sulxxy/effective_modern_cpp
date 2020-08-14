# 条款7：在创建对象时注意区分`()`和`{}`
```c++
int x(0);
int y{0};
int z = 0;
```
大括号可以统一初始化；
```c++
class A{
private:
    int x{0};
    int y = 0;
    int z(0);  // not allowed
}
```

```c++
std::atomic<int> ai1{0};   // correct
std::atomic<int> ai2(0);   // correct
std::atomic<int> ai3 = 0; // wrong
```

大括号初始化不能窄化，[例子](./init_list0.cpp)。

1. 构造函数中没有`std::initializer_list`形参时，大小括号初始化行为一致；
   ```c++
   class A{
       public:
       A(int i, bool b);
       A(int i, double d);
   };
   A a1(10, true);   // 调用第一个构造函数
   A a2{10, true};   // 同上
   A a3(10, 5.0);    // 调用第二个构造函数
   A a4{10, 5.0};    // 同上
   ```
2. 如果构造函数形参中包含`std::initializer_list`，那么编译器会尽最大可能把调用带`std::initializer_list`的版本，[例子](./init_list.cpp):
   ```c++
   class A{
       public:
       A(int i, bool b);
       A(int i, double d);
       A(std::initializer_list<double> il);
   };
   A a1(10, true);    // 1
   A a2{10, true};    // 3
   A a3(10, 5.0);     // 2
   A a4{10, 5.0};     // 3
   ```

   ```c++
   class A{
       public:
       A(int i, bool b);
       A(int i, double d);
       A(std::initializer_list<double> il);
       A(const A& a);
       A(A&& a);
       operator float() const;
   };
   A tmp;
   A a1(tmp);    //  4
   A a2{tmp};    //  3
   A a3(std::move(tmp));    // 5
   A a4{std::move(tmp)};    // 3
   ```

   ```c++
   class A{
       public:
       A(int i, bool b);
       A(int i, double d);
       A(std::initializer_list<bool> il);
   };
   A a1(10, true);  // 1
   A a2{10, true};  // 3
   A a3(10, 5.0);   // 2
   A a4{10, 5.0};   // 3 with error
   ```
3. 只有在找不到任何办法把大括号中的初始化物的实参转化成`std::initializer_list`模板中的型别时，编译器才会退而检查其他重载版本；
   ```c++
   class A{
       public:
       A(int i, bool b);
       A(int i, double d);
       A(std::initializer_list<std::string> il);
   };
   A a1(10, true);    //  1
   A a2{10, true};    //  1
   A a3(10, 5.0);     //  2
   A a4{10, 5.0};     //  2
4. 不带参数的默认构造函数
```c++
A a(2);    // init an object of A with parameter 2
A a();     // function a with return type A
A a({});     // call the constructor with initializer_list
A a{};   //  call default constructor withno parameters
```

# 条款8：优先选用`nullptr`，而非`0`或`NULL`
```c++
void f(int);
void f(bool);
void f(ptr*);
f(0);
f(NULL); // compilation error
f(nullptr); // no problem
```
[例子](./nullptr.cpp)。

# 条款9：优先选用别名声明，而非`typedef`
别名声明：
```c++
using IterT = std::vector<int>::iterator;
```
`typedef`:
```c++
typedef std::vector<int>::iterator IterT;
```
别名声明的一个优势时可以模板化，
```c++
template<typename T>
using MyAllocList = std::list<T, MyAlloc<T>>;
MyAllocList<A> lw;
```

但是使用`typedef`会非常麻烦：
```c++
template<typename T>
struct MyAllocList{
    typedef std::list<T, MyAlloc<T>> type;
};
MyAllocList<A>::type lw;
```

```c++
template<typename T>
class Widget{
private:
    typename MyAllocList<T>::type list; //带依赖型别，编译器无法确定type是表示型别还是类成员，所以必须加typename
};
```

```c++
class Wine {...};
template<>
class MyAllocList<Wine>{
private:
    enum class WineType{
        White, Red, Rose
    };
    WineType type;  //此时type是一个成员
};
```

# 条款10：优先选用限定作用域的枚举类型，而非不限定作用域的枚举类型

1. 减少命名污染
   ```c++
   enum Color {red, green, blue};
   auto red = false; // wrong
   ```

   ```c++
   enum class Color {red, green, blue};
   auto red = false; // correct
   auto c = Color::red;
   ```
2. 强类型
3. 可以用于前置声明
   
   C++98不可以，因为底层型别是不确定的，因而编译器无法确定尺寸。在C++11中，限定作用域的枚举型别的底层型别是已知的；而对于不限范围的枚举型别，可以指定型别，[例子](./enum_test.cpp)。

# 条款11：优先选用删除函数，而非`private`未定义函数
C++98: 声明为`private`且不给出定义；
C++11: 使用delete。

好处：
1. 删除函数无法通过任何方法使用，但private函数可在成员内被使用，这样只有在链接阶段才会被诊断出来，[例子](./delete_n_private.cpp)；
2. 任何函数都可以删除，包括非成员函数和模板实现，[例子](./delete.cpp)；
3. delete可以阻止不应该进行的模板具现，[例子](./delete2.cpp)。

# 条款12：为意在改写的函数添加override声明
override条件：
1. 基类中的函数必须是虚函数
2. 基类和派生类中的函数名字必须完全一致（析构函数除外）
3. 函数形参型别必须完全一致
4. 常量性完全相同
5. 返回值和异常规格兼容
6. 引用修饰词相同

[例子](./override.cpp)。

override好处：
修改基类、能方便看出改动的代价。

成员函数引用修饰词使得对于左值和右值对象的处理能够区分开，[例子](./member_func_ref.cpp)。

# 条款13：优先选用`const_iterator`，而非`iterator`
任何时候只需要迭代器而其指向的内容没有必要修改，就应该使用`const_iterator`。
但C++98中的`const_iterator`并不好用。
例子：
1. [C++98](./const_iterator_98.cpp)
2. [C++11](./const_iterator_11.cpp)

# 条款14：只要函数不发射异常，就为其添加`noexcept`声明
# 条款15：只要有可能使用`constexpr`，就使用它
好处，减少运行时时间，[例子](./constexpr.cpp)。

# 条款16：保证`const`成员函数的线程安全性
`const`成员函数意味着其是一个只读操作。多线程场景下，即使没有同步，读操作也是安全的。但如果类中包含`mutable`成员时，会有例外。

```c++
class A{
public:
    void foo() const{
        if(!flag){
            var = compute();   // data race in multithread env
            flag = true;
        }
        retun var;
    }
private:
    mutable int var;
    int compute() const;
}
```

解决方案是加锁：
```c++
class A{
public:
    void foo() const{
        std::lock_guard<std::mutex> g(m);
        if(!flag){
            var = compute();   // data race in multithread env
            flag = true;
        }
        retun var;
    }
private:
    mutable int var;
    mutable std::mutex m;
    int compute() const;
}
```

# 条款17：理解特种成员函数的生成机制

* 默认构造函数：仅当类中不包含用户声明的构造函数时才生成；
* 析构函数：仅当基类的析构函数为虚函数时，派生类的析构函数才是虚的；
* 复制构造函数和复制赋值函数：按成员进行非静态数据成员的复制操作；如果该类声明了移动操作，则复制函数被定义为删除，[例子](./member_func.cpp)；
* 移动构造函数和移动赋值运算符：都按成员进行非静态数据成员的移动操作。仅当类中不包含用户声明的复制操作、移动操作和析构函数时才生成。

即使编译器能自动生成复制和移动操作，但最好还是：将函数自行声明，并且以`=default`作为定义，[例子](./member_func2.cpp)。

# 待解决的问题
- [ ] `constexpr`有时不生效
