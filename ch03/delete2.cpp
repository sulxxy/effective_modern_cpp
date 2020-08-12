#include<iostream>
using namespace std;


template<typename T>
void processPtr(T* ptr){
    cout << "address: " << ptr;
}

template<>
void processPtr<void>(void*) = delete;

// template<>
// void processPtr<char>(char*) = delete;

int main(){
    int a[] = {1, 2, 4};
    processPtr(a);
    char* str = "sdf";
    processPtr(str);
}