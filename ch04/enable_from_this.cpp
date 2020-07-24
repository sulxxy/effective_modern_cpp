#include <iostream>
#include <memory>
using namespace std;
// 智能指针测试类，继承enable_shared_from_this类
class A : public enable_shared_from_this<A>
{
public:
	A()
	{
		cout << "A()" << endl;
	}
	~A()
	{
		cout << "~A()" << endl;
	}

	shared_ptr<A> getSharedPtr()
	{
		return shared_from_this();
	}
};

int main(){
    // A* a = new A;
    // a->getSharedPtr(); // runtime error!
    auto a1 = make_shared<A>();
    a1->getSharedPtr();
}