# 并发API

## 基础
C++11开始从语言层面支持多线程。[例子](./bootcamp.cpp)。
### `std::thread`
1. `std::thread::join()`：主线程会等待子线程执行结束。
2. `std::thread::detach()`：将当前线程对象所代表的执行实例与该线程对象分离，使得线程的执行可以单独进行。

### `std::async`
用来异步执行一个任务。

### `std::future`
用来获取异步调用的结果，[例子](./future.cpp)。

## 条款35：优先选用基于任务而非基于线程的程序设计
1. 基于线程
2. 基于task
3. 基于task的好处
   
   由标准库来管理，解决超订问题，因为调用`std::async`时，系统不保证会创建一个新的软件线程。最高水平的线程调度器会使用全系统范围内的线程池来避免超订，而且还会通过运用[工作窃取算法](https://blog.csdn.net/pange1991/article/details/80944797)来提高硬件内核间的负载均衡。

4. 何时基于线程编程
   1. 需要访问底层线程实现的API
   2. 需要且有能力为你的用用优化线程用法
   3. 需要实现超越C++并发API的线程技术
## 条款36：如果异步是必要的，则指定`std::launch::async`
`std::async`时，并非一定会达到异步运行的结果，该调用仅仅是让该函数以符合`std::async`的启动策略来运行。有两个启动策略：
1. `std::launch::async`: 函数`f`必须以异步方式运行，亦即，在另一线程之上执行
2. `std::launch::deferred`: 函数`f`只会在`std::async`所返回的期值的`get`或`wait`时才调用。当调用`get`或者`wait`时，`f`会同步运行。即，调用方会阻塞至`f`运行结束为止。如果`get`或者`wait`都没有得到调用，`f`是不会被执行的。

`std::async`的默认启动策略是两者皆可：
```c++
auto fut1 = std::async(f);
auto fut2 = std::async(std::launch::async | std::launch::deferred, f); // 等价
```
这种弹性是的`std::async`与标准库的线程管理组件能够承担得起线程的创建和销毁、避免超订，以及负载均衡的责任。

但以默认方式启动，会触及一些问题。给定线程`t`，执行：
```C++
auto fut = std::async(f);
```
则：
1. 无法预知`f`是否和`t`并发执行，因为`f`可能会被调度为推迟执行；
2. 无法预知`f`是否运行在与`t`不同的线程之上；
3. 连`f`是否会被调用都是未知的，因为无法保证每条路径上，`fut`的`get()`或者`wait()`都会得到调用。

默认启动策略在调度上的弹性会在使用thread_local变量时导致不明不白的混淆。因为这意味着如果`f`读或写此线程级局部存储（thread-local storage, TLS）时，无法预知会取到哪个线程的局部存储：
```c++
auto fut = std::async(f); // f的TLS可能时和一个独立线程有关，也可能是和调用fut.get()或fut.wait()的线程有关
```
同时也会影响那些基于`wait`的循环中以超时为条件者，举个例子。
```c++
using namespace std::literals;
void f(){
    std::this_thread::sleep_for(1s);
}
auto fut = std::async(f);
while(fut.wait_for(100ms) != std::future::ready){  // 循环至f运行结束，但此事可能永不发生
    ...
}
```
如果`std::async`选择的时`std::launch::async`启动策略，那么上述代码没有问题。但如果选择的时`std::launch::deferred`启动策略，那么则意味着`fut.wait_for()`将总是返回`std::future_status::deferred`，循环永不终止。
这一类缺陷在单元测试中很难发现，因为只有当负载很重是才会出现。修正上面的缺陷只需加个判断，任务是否被延迟即可：
```c++
using namespace std::literals;
void f(){
    std::this_thread::sleep_for(1s);
}
auto fut = std::async(f);
if (fut.wait_for(0s) == std::future_status::deferred) {
    fut.get();
}
else {
    while (fut.wait_for(100ms) != std::future::ready) {  // 循环至f运行结束，但此事可能永不发生
        ...
    }
}
```
总体结论是：如果要使用默认启动策略，需满足以下几点条件：
1. 任务不需要与调用get或者wait的线程并发执行；
2. 读写哪个线程的thread_local变量并无影响；
3. 保证get或者wait一定能执行到，或者接受其永不执行；
4. 使用wait_for或者wait_until时将任务被延迟的可能性纳入考量。

## 条款37：使`std::thread`型别对象在所有路径皆不可联接
如果可联结的线程对象的析构函数被调用，则程序的执行就终止了，[例子](./destructor.cpp)。
```c++
bool Foo::work(){
    std::thread t(f);
    if(conditionSatisfied()){
        t.join();
        return true;
    }
    return false;
}
Foo foo;
foo.work();
test();
```
C++11标准：
>30.3.1.3 thread destructor [thread.thread.destr] ~thread();
>
>If `joinable()`, calls `std::terminate()`. Otherwise, has no effects.
>[ Note: Either implicitly detaching or joining a joinable() thread in its destructor could result in difficult to debug correctness (for detach) or performance (for join) bugs encountered only when an exception is raised. Thus the pro grammer must ensure that the destructor is never executed while the thread is still joinable. — end note ]

为什么要这样设计呢？考虑下除了这样设计还有什么方案：
1. 隐式`join()`: 这种情况下，std::thread的析构函数会等底层异步执行的线程完成。这会导致潜在性能异常。上例中，如果conditionSatisfied已经返回false了， 依然把f执行完，这违反直觉。
2. 隐式`detach()`: 隐式detach之后，假设f通过引用捕获捕获了一个局部变量，然后由于conditionSatisfied()返回fasle, work()直接返回，其局部变量会被销毁。但由于调用了detach，f依然以线程的方式在运行。之后某个时刻，test()被调用，test()可能会使用一部分或者全部work()栈占用过的内存。这时候，被detach的线程也在用这个栈。那么从test()的视角看，就是自己的栈被无缘无故篡改。
3. 直接cancel，不执行：posix不支持。
更具体的可以参考[这里](https://akrzemi1.wordpress.com/2012/11/14/not-using-stdthread/)。
## 条款38：对变化多端的线程句柄析构函数行为保持关注
## 条款39：考虑针对一次性事件通信使用以`void`为模板型别实参的期望
## 条款40：对并发使用`std::atomic`，对特种内存使用`volatile`