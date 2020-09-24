#include<iostream>
#include<chrono>
#include<thread>
#include<future>
using namespace std;

int long_compute(){
    cout << "thread 1: start computing" << endl;
    this_thread::sleep_for(chrono::seconds(6));
    // int i = 0;
    // while(i++ < 1000);
    cout << "thread 1: complete computing" << endl;
    return 100;
}

int main(){
    future<int> f = async(launch::async, long_compute);
    cout << "main thread: start computing" << endl;
    this_thread::sleep_for(chrono::seconds(6));
    cout << "main thread: complete computing" << endl;
    int res = f.get();
    cout << "Res: " << res << endl;
    return 0;
}