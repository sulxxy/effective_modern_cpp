#include<iostream>
#include<vector>

using namespace std;

template<typename T>
class TD;

template<typename T>
class MyList{
public:
    MyList(const initializer_list<T>& xs){
        for(auto x : xs){
            vec.push_back(x);
        }
    }

    void print(){
        for(auto x : vec){
            cout << x << "\t";
        }
    }
private:
    vector<T> vec;
};

int main(){
    MyList<int> myList{1,2,3};
    myList.print();

}
