#include<bits/stdc++.h>
using namespace std;

template<typename Object>
class Vector {
    private:
    int size, capacity;
    Object *objects;

    public:
    static const int SIZE_BUFFER = 16; // static as one copy is shared across all instances

    // constructor
    explicit Vector(int newSize): size{newSize}, capacity{newSize + SIZE_BUFFER} {
        objects = new Object[capacity];
    }

    ~Vector(){
        delete[] objects;
    }

    Object &operator[](int index){
        return objects[index];
    }
    // to return constants
    const Object &operator[](int index) const {
        return objects[index];
    }

    void resize(int newSize) {
        if(newSize > this->capacity) { // we need more memory only if it exceeds current capacity
            reserve(newSize*2);
        }
        this->size = newSize;
    }

    private:
    void reserve(int newCapacity) {
        if(newCapacity <= capacity) return;
        Object *newObjects = new Object[newCapacity];
        for(int i=0;i<this->size;i++){
            newObjects[i] = move(this->objects[i]);
        }
        delete[] this->objects;
        this->capacity = newCapacity;
        this->objects = newObjects;
    }

    public:
    void push_back(Object ob) {
        if(this->size == this->capacity){
            reserve(2*this->capacity);
        }
        this->objects[this->size++] = ob;
    }
    void getInfo(){
        cout<<this->size<<" "<<this->capacity<<endl;
    }
};

int main(){
    Vector<int> v(3);
    v[0] = 1;
    v[1] = 2;
    v[2] = 3;
    cout<<v[0]<<" "<<v[1]<<" "<<v[2]<<endl;
    v.getInfo();
    // resize
    v.resize(100);
    v.getInfo();
    cout<<v[0]<<" "<<v[1]<<" "<<v[2]<<endl;
    cout<<v[10]<<endl;

    // // change SIZE_BUFFER to 0 for code below
    // Vector<int> v2(1);
    // v2[0] = 1;
    // v2.getInfo();
    // v2.push_back(2);
    // v2.getInfo();
    // cout<<v2[0]<<" "<<v2[1]<<endl;
}
