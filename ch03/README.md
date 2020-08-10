# 条款7：在创建对象时注意区分`()`和`{}`
```c++
int x(0);
int y{0};
int z = 0;
```
大括号可以统一初始化；不能窄化；

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
2. 如果构造函数形参中包含`std::initializer_list`，那么编译器会尽最大可能把调用带`std::initializer_list`的版本
   ```c++
   class A{
       public:
       A(int i, bool b);
       A(int i, double d);
       A(std::initializer_list<double> il);
   };
   A a1(10, true);
   A a2{10, true};
   A a3(10, 5.0);
   A a4{10, 5.0};
   ```

   ```c++
   class A{
       public:
       A(int i, bool b);
       A(int i, double d);
       A(A a);
       A(A&& a);
       A(std::initializer_list<double> il);
       operator float() const;
   };
   A tmp;
   A a1(tmp);
   A a2{tmp};
   A a3(std::move(tmp));
   A a4{std::move(tmp)};
   ```

   ```c++
   class A{
       public:
       A(int i, bool b);
       A(int i, double d);
       A(std::initializer_list<bool> il);
   };
   A a1(10, true);
   A a2{10, true};
   A a3(10, 5.0);
   A a4{10, 5.0};
   ```
3. 只有在找不到任何办法把大括号中的初始化物的实参转化成`std::initializer_list`模板中的型别时，编译器才会退而检查其他重载版本；
   ```c++
   class A{
       public:
       A(int i, bool b);
       A(int i, double d);
       A(std::initializer_list<std::string> il);
   };
   A a1(10, true);
   A a2{10, true};
   A a3(10, 5.0);
   A a4{10, 5.0};

# 条款8：优先选用`nullptr`，而非`0`或`NULL`
# 条款9：优先选用别名声明，而非`typedef`
# 条款10：优先选用限定作用域的枚举类型，而非不限定作用域的枚举类型
# 条款11：优先选用删除函数，而非`private`未定义函数
# 条款12：为意在改写的函数添加override声明
# 条款13：优先选用`const_iterator`，而非`iterator`
# 条款14：只要函数不发射异常，就为其添加`noexcept`声明
# 条款15：只要有可能使用`constexpr`，就使用它
# 条款16：保证`const`成员函数的线程安全性
# 条款17：理解特种成员函数的生成机制