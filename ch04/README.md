# 智能指针
2. `std::shared_ptr`: 管理具备共享所有权的资源
3. `std::weak_ptr`: 对于类似`std::shared_ptr`但有可能空悬的指针使用
1. `std::unique_ptr`: 管理具备专属所有权的资源
4. ~~`std::auto_ptr`：被`std::unique_ptr`替代~~

   
# std::shared_ptr
## 条款19：使用`std::shared_ptr`管理具备共享所有权的资源
1. 使用引用计数来判断是否需要析构资源
2. `std::shared_ptr`的构造函数（移动构造函数除外）递增引用
3. `std::shared_ptr`的析构函数递减引用
4. 复制赋值运算符在左边执行引用递增，右边执行引用递减

## 简单版实现
shared_ptr_t.cpp.

## 问题1：性能
1. 尺寸：内部包含一个指向资源的裸指针，也包含一个指向该资源的引用计数的指针
2. 引用计数的内存必须动态分配
3. 引用计数的递增、递减必须原子操作
## 问题2：环形引用
1. 例子:circle_ref.cpp
2. 打破：`std::weak_ptr`
## 自定义析构器
例子：
```c++
class A{
public:
    A(){
        cout << "Constructor A" << endl;
    }
    ~A(){
        cout << "Destructor A" << endl;
    }
};
auto deleter = [](A* pa){
    cout << "This is custimized deleter" << endl;
    delete pa;
};

int main(){
    {
        shared_ptr<A> a(new A, deleter);
    }
    return 0;
}
//output:
Constructor A
This is custimized deleter
Destructor A
```

## 控制块的创建规则
1. 调用`std::make_shared`时
3. 使用`std::unique_ptr`构造`std::shared_ptr`时
2. 使用裸指针作为实参初始化`std::shared_ptr`时
```c++
auto pA = new A;
// undefined behaviour
std::shared_ptr<A> sp1(pa); // create first control block for *pa
std::shared_ptr<A> sp2(pa); // create second control block for *pa
```
如果不可避免的需要使用裸指针（如使用自定义析构函数时），直接传递`new`表达式：
```c++
std::shared_ptr<A> sp1(new A); 
std::shared_ptr<A> sp2(sp1);
```

一个更加隐蔽的例子：
```c++
std::vector<std::shared_ptr<A>> vec;
class A{
public:
    void process(){
        //other code
        // put into vec
        vec.emplace_back(this);  // 每次都会用this指针创建一个新的std::shared_ptr对象（包含一个新的控制块）
    }
}
```
解决方案：
```c++
std::vector<std::shared_ptr<A>> vec;
class A : public std::enable_shared_from_this<A>{ // 奇异递归模板模式, CRTP
public:
    void process(){
        //processing
        // put into vec
        vec.emplace_back(shared_from_this()); 
    }
}
```

```c++
template<class _Ty>
class enable_shared_from_this
{	// provide member functions that create shared_ptr to this
public:
	using _Esft_type = enable_shared_from_this;

	_NODISCARD shared_ptr<_Ty> shared_from_this()
		{	// return shared_ptr
		return (shared_ptr<_Ty>(_Wptr));
		}
	// 成员变量是一个指向资源的弱智能指针
	mutable weak_ptr<_Ty> _Wptr;
};
```
使用`enable_from_this()`前提是，得有个指向当前资源的控制块，见例子：
```c++
class A : public enable_shared_from_this<A>{
public:
	A(){
		cout << "A()" << endl;
	}
	~A(){
		cout << "~A()" << endl;
	}
	shared_ptr<A> getSharedPtr(){
		return shared_from_this();
	}
};
int main(){
    /* 
     * terminate called after throwing an instance of 'std::bad_weak_ptr'
     * what():  bad_weak_ptr
     * [1]    2028 abort      ./a.out
    */
    // A* a = new A;
    // a->getSharedPtr(); // runtime error!
    auto a1 = make_shared<A>();
    a1->getSharedPtr(); //correct
}
```

# std::weak_ptr
## 条款20：对于类似`std::shared_ptr`但有可能空悬的指针使用`std::weak_ptr`
1. 不影响引用计数
2. 调用前需先检查对象是否被析构
   ```c++
   std::shared_ptr<A> sp1 = wp.lock(); //若wp失效，sp1为空
   //or
   std::shared_ptr<A> sp2(wp); // 若wp失效，抛出std::bad_weak_ptr异常
   ```

### 使用场景
1. 观察者模式（每个主题持有一个容器，包含所有观察者的`std::weak_ptr`，当发现失活以后，则移除，不再给其发）
2. `std::shared_ptr`的循环引用
3. 缓存
   
## 条款21：优先选用`std::make_unique`和`std::make_shared`，而非直接使用`new`
1. `std::make_shared`只会分配一次内存
2. 异常安全
   ```c++
   void process(std::shared_ptr<A> sp, int priority);
   int computePriority();
   // caller
   process(std::shared_ptr<A>(new A), computePriority());
   ```
   编译器可能会有如下执行顺序：
   1. `new A`
   2. `computePriority()`
   3. `std::shared_ptr<A>()`
   
   如果第二步出现异常，则会发生内存泄漏。
   
# std::unique_ptr
## 条款18：使用`std::unique_ptr`管理具备专属所有权的资源
1. 几乎和裸指针占用空间一致
1. 自定义deleter，见例子custimize_deleter.cpp.
1. 转化成`std::shared_ptr`

## 条款22：使用Pimpl习惯用法时，将特殊成员函数的定义放到实现文件中


## Reference
* Scott Meyers. 2014. Effective Modern C++: 42 Specific Ways to Improve Your Use of C++11 and C++14 (1st. ed.). O’Reilly Media, Inc.
* https://blog.csdn.net/QIANGWEIYUAN/article/details/88973735
* https://blog.csdn.net/peng864534630/article/details/77932574
