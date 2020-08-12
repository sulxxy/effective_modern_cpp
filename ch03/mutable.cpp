class A{
public:
    void foo() const{
        var = compute();
    }
private:
    mutable int var;
    int compute() const { return 1; }
};

int main(){
    A a;
    a.foo();
}