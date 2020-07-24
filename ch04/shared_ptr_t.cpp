template <typename T>
class shared_ptr_t
{
public:
    shared_ptr_t(): pointer(0), counter(0)
    {
    } 
 
    shared_ptr_t(T* p): pointer(0), counter(0){
        *this = p;
    }
 
    shared_ptr_t(const shared_ptr_t<T> &p): 
    pointer(p.pointer), counter(p.counter){
        Increase();
    }
 
    ~shared_ptr_t(){
        Decrease();
    }
 
    shared_ptr_t operator=(T* p){
        Decrease();
        if (p){
            pointer = p;
            counter = new int(1);
        }
        else {
            pointer = 0;
            counter = 0;
        }
        return *this;
    }
 
    shared_ptr_t operator=(shared_ptr_t<T> &p){
        if (this != &p){
            Decrease();
            pointer = p.pointer;
            counter = p.counter;
            Increase();
        }
        return *this;
    }
 
    operator bool() const{
        return counter != 0;
    }
 
    T* operator*() const{
        return this;
    }
 
    T* operator->() const{
        return pointer;
    }
    
    int getPtrPointer() const{
        return *pointer;
    }
 
    int getPtrCounter() const{
        return *counter;
    }
 
private:
    T*      pointer;
    int*    counter;
 
    void Increase(){
        if (counter) 
            ++*counter;
    }
 
    void Decrease(){
        if (counter && --*counter == 0){
            delete pointer;
            delete counter;
            counter = 0;
            pointer = 0;
        }
    }
 
};
