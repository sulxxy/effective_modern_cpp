# C++中的型别推导

## auto和模板的关系
```c++
auto x = 3;
```
可以认为等价于：
```c++
template<typename T>
void function_for_x(T param);
function_for_x(3);
```
在上面的模板函数中，编译器推导实参`3`的规则基本同样适用于`auto`赋值语句中的推导, 因此讨论`auto`之前，先讨论模板的型别推导。

## 模板中的型别推导 

```c++
template<typename T>
void f(ParamType param);
f(expr);
```
编译器会推导出`T`和`ParamType`, 后者除了类型信息，还会包含一些限定词，如`const`或者引用、指针符号等。

分3种情况：
1. `ParamType`具有指针或者引用；
2. `ParamType`是个万能引用；
3. `ParamType`既非指针也非引用。


### 情况一： `ParamType`具有指针或者引用
模式如下：
```c++
template<typename T>
void f(T& param);
f(expr);
```
规则：
1. 如果`expr`具有引用类型，先忽略引用部分；
2. 对`expr`类型和`ParamType`类型进行模式匹配，以此推导出`T`的类型。

```c++
void case1(){
    int x = 131;
    int& rx = x;
    const int cx = 10;
    const int& crx = cx;
    int* px = &x;
    f(x);     // T=int, ParamType=int&
    f(rx);    // T=int, ParamType=int&
    f(cx);    // T=const int, ParamType=const int&
    f(crx);   // T=const int, ParamType=const int&
    f(px);    // T=int*, ParamType=int*&
}
```
指针的情况也是类似：
```c++
template<typename T>
void f1pt(T* param);
void case1pt(){
    int x = 131;
    int& rx = x;
    int* px = &x;
    // f1pt(x);   // template argument deduction failed
    // f1pt(rx);    // template argument deduction failed
    // f1pt(px);      // int*
    // f1pt(&x);         // int*
}
```

### 情况二：`ParamType`是个万能引用
模式如下：
```c++
template<typename T>
void f(T&& param);
f(expr);
```
规则：
1. 如果`expr`是左值，`T`和`ParamType`都会被推导为左值引用；
2. 如果`expr`是右值，应用情况一中的讨论。

```c++
void case2(){
    int x = 131;
    int& rx = x;
    const int cx = 10;
    const int& crx = cx;
    f(x);     // int&
    f(rx);    // int&
    f(cx);    // const int&
    f(crx);   // const int&
    f(233);   // int&&
```

### 情况三：`ParamType`既非指针也非引用
模式如下：
```c++
template<typename T>
void f(T param);
f(expr);
```
此时完全按值传递，无论传入什么，`param`都会是一个副本。具体规则如下：
1. 若`expr`具有引用类型，则忽略引用部分；
2. 忽略`expr`的引用性之后，如果`expr`是个`const`对象，则同样忽略`const`。
```c++
void case3(){
    int x = 131;
    int* px = &x;
    const int* cpx = &x;
    const int* const cpcx = &x;
    int& rx = x;
    const int cx = 10;
    const int& crx = cx;
    f(x);       // int
    f(rx);      // int
    f(cx);      // int
    f(crx);     // int
    f(px);      // int*
    f(cpx);     // const int*
    f(cpcx);    // const int*
}
```

### 数组实参
如果实参是数组，且`ParamType`包含引用，那么就可以看到如下场景：
```c++
template<typename T>
void f(T& param)；
void caseArray(){
    int arr[] = {1,2,3};
    f1(arr);       // int (&)[3]
}
```
上例推导出的类型直接包含了数组的大小，由此可有以下奇技淫巧来获取一个数组的大小：

```c++
template<typename T, std::size_t N>
std::size_t arraySize(T (&)[N]){
    return N;
}
int arr[] = {1,2,3};
std::size_t n_arr = arraySize(arr);
```

## auto
### 规则
```c++
auto x = 3;
```
几乎可以认为等价于：
```c++
template<typename T>
void function_for_x(T param);
function_for_x(3);
```
所以模板中型别推导的结论基本可以直接应用在`auto`上，例如：
```c++
auto x = 131; // case 3, x is int
const auto cx = 131; // case 3, cx is const int
const auto& crx = x; // case 1, crx is const int&
auto ccx = cx; // ccx is int
auto& rcx = cx; // rcx is const int&

int arr[] = {1,2,3};
auto aarr = arr;    // aarr int*
auto& ararr = arr;  // ararr is int (&)[3]
```

`auto`与模板型别推导唯一不同之处在于对大括号`{}`的处理。`auto`会把大括号推导为`std::initializer_list<T>`，而`T`则不会。
```c++
auto xs = {23, 12};  // xs is std::initilizer_list<int>

template<typename T>
void f(T param);
f({23, 12}); // error

template<typename T>
void f(std::initializer_list<T> param);
f({23, 12}); // correct, T is int
```

### Trick: 查看auto的推导结果
```c++
template<typename T>
TypeDisplayer;

auto x = 12;
TypeDisplayer<decltype(x)> x_type;   // 编译器报错信息会显示推导的类型
```
### 尽量使用auto
例子：unordered_map。


### 特殊情况：隐形代理类
[TODO] 此种情况下`auto`会推导错误，如`std::vector<bool>`。

## 待解决的问题
1. [ ] unordered_map内存空间分配
2. [ ] 隐形代理类
3. [ ] const的用法

## 参考书目
* Scott Meyers. 2014. Effective Modern C++: 42 Specific Ways to Improve Your Use of C++11 and C++14 (1st. ed.). O’Reilly Media, Inc.