#include<iostream>
#include<memory>
using namespace std;

class B;

class A{
    public:
    A(){
        cout << "Constructor A" << endl;
    }
    void pointToB(shared_ptr<B> pb){
        b = pb;
    }
    ~A(){
        cout << "Destructor A" << endl;
    }
    private:
    shared_ptr<B> b;
};

class B{
    public:
    B(){
        cout << "Constructor B" << endl;
    }
    void pointToA(shared_ptr<A> pa){
        a = pa;
    }
    ~B(){
        cout << "Destructor B" << endl;
    }

    private:
    weak_ptr<A> a;
};

int main(){
    {
        auto a = make_shared<A>();
        auto b = make_shared<B>();
        a->pointToB(b);
        b->pointToA(a);
    }
    return 0;
}