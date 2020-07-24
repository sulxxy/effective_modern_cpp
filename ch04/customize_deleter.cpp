#include<iostream>
#include<memory>
using namespace std;

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