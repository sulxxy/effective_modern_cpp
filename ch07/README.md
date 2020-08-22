# 并发API

## 条款35：优先选用基于任务而非基于线程的程序设计
## 条款36：如果异步是必要的，则指定`std::launch::async`
## 条款37：使`std::thread`型别对象在所有路径皆不可联接
## 条款38：对变化多端的线程句柄析构函数行为保持关注
## 条款39：考虑针对一次性事件通信使用以`void`为模板型别实参的期望
## 条款40：对并发使用`std::atomic`，对特种内存使用`volatile`