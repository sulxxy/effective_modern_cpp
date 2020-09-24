#include<thread>
#include <chrono>
#include<iostream>
#include<mutex>

using namespace std;
int cnt = 20;
mutex m;

void func1(){
    for(int i = cnt; cnt > 0; ){
        lock_guard<mutex> lockGuard(m);
        cout << "func1: " << cnt << endl;
        cnt--;
    }
}

void func2(){
    for(int i = cnt; cnt > 0;){
        lock_guard<mutex> lockGuard(m);
        cout << "func2: " << cnt << endl;
        cnt--;
    }
}

void func3(){
    for(int i = cnt; cnt > 0; ){
        lock_guard<mutex> lockGuard(m);
        cout << "func3: " << cnt << endl;
        cnt--;
    }
}

int main(){
    cout << "MAX CONCURRENCY: " << thread::hardware_concurrency() << endl;
    thread t1{func1};   // thread starts execution after created
    thread t2{func2};
    thread t3{func3};
    cout << "t1 joined" << endl;
    t1.join();
    cout << "t2 joined" << endl;
    t2.join();
    cout << "t3 joined" << endl;
    t3.join();
    cout << "main thread finished. " << endl;
    return 0;
}
/*output
func1: 20
func1: 19
func1: 18
func1: 17
func1: 16
func1: 15
func1: 14
func1: 13
func1: 12
t1 joined
func1: 11
func1: 10
func1: 9
func1: 8
func1: 7
func1: 6
func1: 5
func1: 4
func1: 3
func1: 2
func1: 1
func3: 0
t2 joined
func2: -1
t3 joined
main thread finished. 
*/