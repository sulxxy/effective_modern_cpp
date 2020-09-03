# return value optimization
[example](./test_new.cpp)

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
[example](./lambda_findif.cpp)。

## 条款31：避免使用默认捕获模式
默认捕获包括默认值捕获`[=]`和引用捕获`[&]`，
1. 避免空悬引用
   ```c++
   using FilterContainer = std::vector<std::function<bool(int)>>;
   FilterContainer filters;
   void addDivisorFilter(int param){
      auto divisor = computeDisivor(param);
      filters.emplace_back(
         [&](int value) {return value % divisor == 0; // reference to divisor might be empty
         }
      );
   }
   ```

   一种更好的实践是，显示写出捕获的变量：
   ```c++
   filters.emplace_back(
      [&divisor](int value) {return value % divisor == 0; 
      }
   );
   ```

   值捕获也可能导致空悬引用。
   ```c++
   void Widget::addFilter() const{
      filters.emplace_back(
         [=](int value) { return value % divisor == 0; }
      );
   }
   ```
   捕获仅针对于在创建lambda表达式的作用域内可见的**非静态局部**变量（包括形参）。
   上式等价于：
   ```c++
   void Widget::addFilter() const{
      auto currentObjectPtr = this;
      filters.emplace_back(
         [currentObjectPtr](int value) { return value % currentObjectPtr->divisor == 0; }
      );
   }
   ```

   解决方案：
   ```c++
   void Widget::addFilter() const{
      auto divisorCopy = divisor;
      filters.emplace_back(
         [divisorCopy](int value) { return value % divisorCopy == 0; }
      );
   }
   ```

2. 容易让用户产生误会
   `static`变量可以在lambda内部使用，但不能被捕获，[例子](./lambda_static.cpp)。

## 条款32：使用初始化捕获将对象移入闭包
有时希望将一个只移对象（例如`std::unique_ptr`）放入捕获列表中，C++11不直接支持。C++14中提出了全新的捕获机制，叫初始化捕获（init capture）。使用初始化捕获可以指定：
1. 由lambda生成的闭包类中的成员变量的名字；
2. 一个表达式，用以初始化该成员变量。

例子：
```c++
class Widget {
public:
   bool isValidated() const;
   bool isProcessed() const;
   bool isArchived() const;
};

auto pw = std::make_unique<Widget>();
auto func = [pw = std::move(pw)]{     //初始化捕获
   return pw->isValidated() && pw->isArchived();
};
```
上例初始化捕获列表中，等号左右两边的`pw`拥有不同的作用域，左边是闭包`func`类成员变量的名字，作用域则是闭包的作用域；右边是初始化表达式，与lambda表达式定义之处的作用域相同。

如果需要用C++11实现，则是：
1. 把需要捕获的对象移动到`std::bind`产生的函数对象中
2. 给到lambda表达式一个指涉到欲捕获的对象的引用
```c++
std::vector<double> data;
// ...
auto func = [data = std::move(data)] { /*processing data*/ }; // C++14
```
在C++11中，则需要：
```c++
std::vector<double> data;
// ...
auto func = std::bind([](const std::vector<double>& data){/* processing data */}, std::move(data));
```

## 条款33：对`auto&&`型别的形参使用`decltype`，以`std::forward`之
C++14中，lambda表达式的形参可以使用`auto`，例如
```c++
auto f = [val](auto x) {return foo(x); };
```

lambda表达式对`x`的唯一动作就是将其转发给`foo`。如果`foo`函数区分左值和右值，那么该lambda表达式会有问题，因为lambda表达式总是传递左值给`foo`，[例子](./lambda_lrv.cpp)。
解决方案是使用完美转发：
```c++
auto f = [val](auto&& x) {return foo(std::forward<decltype(x)>(x)); };
```

## 条款34：优先选用lambda表达式，而非`std::bind`
1. 更高的可读性
   ```c++
   using Time = std::chrono::steday_clock::time_point;
   enum class Sound {Beep, Siren, Whistle};
   using Duration = std::chrono::steady_clock::duration;
   void setAlarm(Time t, Sound s, Duration d);
   ```

   需要在程序某处设置在一个小时后发出警报并持续30s。警报的具体声音未确定。因此可以编写lambda表达式，修改`setAlarm`的接口，新接口只需指定声音即可。

   ```c++
   auto setSoundL = [] (Sound s) {
      using namespace std::chrono;
      using namespace std::literals;
      setAlarm(steady_clock::now()+1h, s,30s);
   }
   ```

   如果使用`std::bind`，代码形式如下：

   ```c++
   using namespace std::chrono;
   using namespace std::literals;
   using namespace std::placeholders;

   auto setSoundB = std::bind(setAlarm, steady_clock::now()+1h, _1, 30s);
   ```

   但是这段代码是错误的，因为警报被设定的启动时间是创建`setSoundB`后的一小时，而不是`setAlarm`调用之后的一小时。解决这个问题需要延迟评估表达式的值，则有：
   
   ```c++
   // https://stackoverflow.com/questions/42207520/why-nested-bind-can-defer-evaluation-of-the-expression
   auto setSoundB = std::bind(setAlarm, 
                              std::bind(std::plus<steady_clock::time_point>(), 
                                       std::bind(steady_clock::now),
                                       hours(1)),
                              _1,
                              seconds(30));
   ```

   如果`setAlarm`有重载版本，那么`setSoundB`会报错，因为编译器不知道该传哪个版本的参数。