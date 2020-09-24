#include<iostream>
#include<future>
#include<chrono>
#include<thread>
using namespace std;

int long_compute(){
    cout << "thread 1: start computing" << endl;
    this_thread::sleep_for(chrono::seconds(6));
    cout << "thread 1: complete computing" << endl;
    return 0;
}

int main(){
    cout << "Main thread started" << endl;
    {
        future<int> f = async(launch::async, long_compute);
    }
    cout << "Main thread finished" << endl;
    return 0;
}