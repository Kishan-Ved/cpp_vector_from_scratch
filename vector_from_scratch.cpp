#include <algorithm>

template <typename Object>
class Vector
{

private:
    int theSize;
    int theCapacity;
    Object *objects; // pointer to the memory location containing the array. Think of this like int* arr =  new int[10] --> arr is the pointer here to the memory location

public:
    static const int SPARE_CAPACITY = 16; // static as one copy is shared across all instances

    // explicit keyword prevents implicit conversions like: Vector v = 10;
    explicit Vector(int initSize = 0) : theSize{initSize},
                                        theCapacity{initSize + SPARE_CAPACITY}, objects{nullptr}
    {
        objects = new Object[theCapacity];
    }

    // objects{nullptr} can be used in the above function also, but it is unnecessary there
    // It is a good practice to use objects{nullptr} in a copy constructor (function below)
    // This is a copy constructor:
    // Usage 1: Vector<int> b = a;  or Vector<int> b(a);
    // Usage 2: function parameter passed by value: void func(Vector<int> v) { ... }  (here, v is created by copying the argument)
    // IMPORTANT: &rhs is required (ie; keep it pass by reference), if we pass by value (only rhs), then there will be infinite recursion (as for pass by value to any function (here the copy constructor), the copy constructor is invoked again)
    Vector(const Vector &rhs) : theSize{rhs.theSize},
                                theCapacity{rhs.theCapacity}, objects{nullptr}
    {
        objects = new Object[theCapacity]; // We can also use this->objects. In case of name conflicts, using this-> is mandatory to distinguish. Here it is optional as there are no name conflicts
        for (int k = 0; k < theSize; ++k)
            objects[k] = rhs.objects[k];
    }

    Vector &operator=(const Vector &rhs) // used in a = b;
    {
        // Usage:

        // Vector<int> a(3);        // a has capacity 19, size 3
        // a.push_back(10);
        // a.push_back(20);
        // a.push_back(30);
        // // a now contains: [10, 20, 30]

        // Vector<int> b(2);        // b has capacity 18, size 2
        // b.push_back(100);
        // b.push_back(200);
        // // b now contains: [100, 200]

        // a = b;  // this calls the copy assignment operator (current function)

        //-----------------------------------------------------------------------

        Vector copy = rhs;      // this calls the copy constructor and creates a new Vector object 'copy'
        std::swap(*this, copy); // The current pointer ('a' in the above example)'s data is swapped with 'copy'
        return *this;           //  copy goes out of scope and its destructor is called, cleaning up the old data of 'a' (which is now in 'copy')
    }

    ~Vector()
    {
        delete[] objects;
    }

    // Move constructor
    // this steals the resources from rhs (instead of copying)
    // Usage 1: Vector v2 = std::move(v1);
    // Usage 2 - when a function returns a Vector (return by value) that has to be stored in another Vector:
    // Vector<int> createVector() {
    //     Vector<int> temp(5);
    //     temp.push_back(10);
    //     temp.push_back(20);
    //     return temp;  // temp is about to be destroyed
    // }
    // Vector<int> v = createVector(); // Move constructor is called
    // Note that Vector v2 = v1 (wihtout std::move) calls the copy constructor, not the move constructor. Here, we can use v1 after this line.
    Vector(Vector &&rhs) : theSize{rhs.theSize},
                           theCapacity{rhs.theCapacity}, objects{rhs.objects}
    {
        rhs.objects = nullptr;
        rhs.theSize = 0;
        rhs.theCapacity = 0;
    }

    // Move assignment operator
    // Usage 1: a = std::move(b); (Note that a is created beforehand, so move constructor is not called here)
    // Usage 2: a = createVector(); (a is existing, so move constructor is not called here)
    Vector &operator=(Vector &&rhs)
    {
        std::swap(theSize, rhs.theSize);
        std::swap(theCapacity, rhs.theCapacity);
        std::swap(objects, rhs.objects);

        return *this;
    }

    // TODO: Read about: lvalue, rvalue and move semantics

    void resize(int newSize)
    {
        if (newSize > theCapacity)
            reserve(newSize * 2); // capacity is double the size
        theSize = newSize;
    }

    // The following function could be written using a move assignment like this:
    // Vector temp(newCapacity);                    // New object
    // --- Move original elements to temp ---
    // *this = std::move(temp);
    // However, this is less efficient because it involves creating a temporary object and then moving it
    void reserve(int newCapacity)
    {
        if (newCapacity < theSize)
            return;

        Object *newArray = new Object[newCapacity];
        for (int k = 0; k < theSize; ++k)
            newArray[k] = std::move(objects[k]);

        theCapacity = newCapacity;
        std::swap(objects, newArray);
        delete[] newArray;
    }

    Object &operator[](int index)
    {
        return objects[index];
    }
    const Object &operator[](int index) const
    {
        return objects[index];
    }

    bool empty() const
    {
        return size() == 0;
    }
    int size() const
    {
        return theSize;
    }
    int capacity() const
    {
        return theCapacity;
    }

    void push_back(const Object &x)
    {
        if (theSize == theCapacity)
            reserve(2 * theCapacity + 1);
        objects[theSize++] = x;
    }

    void push_back(Object &&x)
    {
        if (theSize == theCapacity)
            reserve(2 * theCapacity + 1);
        objects[theSize++] = std::move(x);
    }

    void pop_back()
    {
        --theSize;
    }

    const Object &back() const
    {
        return objects[theSize - 1];
    }

    typedef Object *iterator;
    typedef const Object *const_iterator;

    iterator begin()
    {
        return &objects[0];
    }
    const_iterator begin() const
    {
        return &objects[0];
    }

    iterator end()
    {
        return &objects[size()];
    }
    const_iterator end() const
    {
        return &objects[size()];
    }
};