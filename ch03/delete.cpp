#include<iostream>
using namespace std;

bool isEven(int a){
    return a % 2;
}

bool isEven(bool a) = delete;
bool isEven(char c) = delete;

int main(){
    cout << isEven(13) << endl;
    // cout << isEven(true) << endl;
    // cout << isEven('a') << endl;
}

