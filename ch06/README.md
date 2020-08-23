# lambda表达式
## 基本介绍
```
[capture list](parameter list) -> return type { function body }
```
其中，`capture list`是一个lambda所在函数中定义的局部变量的列表；`return type`、`parameter list`、 `function body`与任何普通函数一样，分别表示返回类型、参数列表和函数体。与普通函数不同的是，lambda必须使用尾置返回来指定返回类型。

### 尾置返回类型
插播介绍下尾置返回类型（trailing return type）：
C++11中，支持使用尾置返回类型，即将返回类型跟在形参列表后边并以一个`->`符号开头。为了表示函数真正的返回类型是跟在形参列表之后，在本应该出现返回类型的地方放置一个`auto`：
```c++
// func接受一个int类型的实参，返回一个指针，该指针指向含有10个整数的数组
auto func(int i) -> int(*)[10];
```

尾置返回类型这种形式，对于返回类型比较复杂的函数最为有效，比如返回类型是数组的指针或者数组的引用。在上例中，如果不想使用尾置返回类型，而是使用传统的语法，则需写成下面的样子：
```c++
int (*func(int i))[10];
```
或者简洁点，使用类型声明：
```c++
typedef int arrT[10];
using arrT = int[10];
arrT* func(int i);
```

回到lambda表达式，我们可以忽略参数列表和返回类型，但必须永远包含捕获列表和函数体：
```c++
auto f = [] { return 42; };
```

### lambda捕获与返回
1. 值捕获
   
   与传值参数类似，采用值捕获的前提是变量可以拷贝；与参数不同，被捕获的变量是在lambda**创建时**拷贝，而不是调用时拷贝：
   ```c++
   void func1(){
       size_t v1 = 42;
       auto f = [v1] { return v1; };
       v1 = 0;
       auto j = f(); // j == 42; f保存了创建时v1的拷贝
   }
   ```
2. 引用捕获
   ```c++
   void func2(){
       size_t v1 = 42;
       auto f = [&v1] { return v1; };
       v1 = 0;
       auto j = f(); // j == 0; f保存了创建时v1的引用，而非拷贝
   }
   ```
   如果采用引用方式捕获一个变量，就必须确保被引用的对象在lambda**执行**的时候时存在的。lambda捕获的都是局部变量，而这些变量在函数结束后就不存在了。如果lambda可能在函数结束后才执行，捕获的引用指向的局部变量已经消失。
3. 隐式捕获
   
   在捕获列表中使用`=`或`&`，编译器则可以推断捕获列表；
   ```c++
   auto f = [=] { return 42; };  //所有局部变量皆是值捕获
   auto f = [&] { return 42; };  //所有局部变量皆是引用捕获
   auto f = [&, val] { return 42; };  //除了val，所有局部变量皆是引用捕获
   auto f = [=, &val] { return 42; };  //除了val，所有局部变量皆是值捕获
   ```
4. 可变lambda
   
   默认情况下，对于一个值被拷贝的变量，lambda不会改变其值。如果希望改变一个被捕获的变量的值，则需要在参数列表后加上`mutable`。
   ```c++
   auto f1 = [val] () mutable { return ++val; };
   ```
   而对于引用捕获，则不需要`mutable`，主要取决于此引用指向的是一个`const`类型还是非`const`类型。
   [例子](./lambda_mutable.cpp)。
5. 指定lambda的返回类型
   
   默认情况下，如果一个lambda体包含`return`之外的任何语句，则编译器假定此lambda返回`void`。
   ```c++
   auto f = [](int v) {return i < 0? -i : i; }; // correct, return int
   auto f2 = [](int v) { if (v < 0) return -i; else return i; }; // wrong, 编译器推断返回void，但实际返回了int
   auto f2 = [](int v) -> int { if (v < 0) return -i; else return i; }; // correct, return int
   ```

### 参数绑定


## 条款31：避免使用默认捕获模式
## 条款32：使用初始化捕获将对象移入闭包
## 条款33：对`auto&&`型别的形参使用`decltype`，以`std::forward`之
## 条款34：优先选用lambda表达式，而非`std::bind`