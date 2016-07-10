#include <universal_handler_map/static_handler_map.h>
#include <universal_handler_map/dynamic_handler_map.h>
#include <iostream>
#include <map>
#include <string>



void zero_handler()
{
    std::cout << "zero_handler()\n";
}

void one_handler()
{
    std::cout << "one_handler()\n";
}

class MyHandlers
{
public:
    static void class_handler_two()
    {
        std::cout << "MyHandlers::class_handler_two()\n";
    }
    static void class_handler_three()
    {
        std::cout << "MyHandlers::class_handler_three()\n";
    }
};

DeclareUniversalMap(NonParam, std::function<void()>);
InsertUniversalMap(NonParam, 0, &zero_handler);
InsertUniversalMap(NonParam, 1, &one_handler);
InsertUniversalMap(NonParam, 2, &MyHandlers::class_handler_two);
InsertUniversalMap(NonParam, 3, &MyHandlers::class_handler_three);

class MyHandlers1
{
public:
    static void class_handler_zero(int i)
    {
        std::cout << "MyHandlers::class_handler_two() " << i << "\n";
    }
    static void class_handler_one(int i)
    {
        std::cout << "MyHandlers::class_handler_three() " << i << "\n";
    }
};

DeclareUniversalMap(OneParam, std::function<void(int)>);
InsertUniversalMap(OneParam, 0, &MyHandlers1::class_handler_zero);
InsertUniversalMap(OneParam, 1, &MyHandlers1::class_handler_one);

void test_static_handler_map()
{
    UniversalMapCall(NonParam, 0)::call();
    UniversalMapCall(NonParam, 1)::call();
    UniversalMapCall(NonParam, 2)::call();
    UniversalMapCall(NonParam, 3)::call();

    UniversalMapCall(OneParam, 0)::call(0);
    UniversalMapCall(OneParam, 1)::call(1);
}

//////////////////////////////////////////////////////////////////////////

// Test with enum key
enum MyEnum
{
    Handler1,
    Handler2,
};

class MyDynamicHandler1
{
public:
    static void handler1(int i, double d)
    {
        std::cout << "i1 = " << i << " d1 = " << d << '\n';
    }

    static void handler2(int i, double d)
    {
        std::cout << "i2 = " << i << " d2 = " << d << '\n';
    }
};

// Test with enum class key
enum class MyEnumClass
{
    HandlerClass1,
    HandlerClass2,
    HandlerClass3
};

class MyDynamicHandler2
{
public:
    static void handler1(int i, std::string& s)
    {
        std::cout << "i1 = " << i << " s1 = " << s << '\n';
    }

    static void handler2(int i, std::string& s)
    {
        std::cout << "i2 = " << i << " s2 = " << s << '\n';
    }
};

// Test with return values
class MyDynamicHandler3
{
public:
    static int handler1(int i1, int i2)
    {
        std::cout << "i1 + i2 = " << i1 + i2 << '\n';
        return i1 + i2;
    }

    static int handler2(int i1, int i2)
    {
        std::cout << "i1 - i2 = " << i1 - i2 << '\n';
        return i1 - i2;
    }
};

// Test with non-static members
class MyDynamicHandler4
{
    int val_;
public:

    MyDynamicHandler4(int val) : val_(val) {}

    void handler1(int i)
    {
        std::cout << "i + val = " << i + val_ << '\n';

    }

    void handler2(int i)
    {
        std::cout << "i - val = " << i - val_ << '\n';
    }
};

// Test from the class itself
class MyDynamicHandler5
{
    HandlerMap<int, std::function<void(MyDynamicHandler5*, int)>> m_;
    int val_;
public:

    MyDynamicHandler5(int val) : val_(val) 
    {
        m_
            (0, &MyDynamicHandler5::handler1)
            (1, &MyDynamicHandler5::handler2);
    }

    void call() 
    {
        m_.call(0, this, 10);
        m_.call(1, this, 20);
    }

protected:
    void handler1(int i)
    {
        std::cout << "i + val = " << i + val_ << '\n';

    }

    void handler2(int i)
    {
        std::cout << "i - val = " << i - val_ << '\n';
    }
};


void test_dynamic_handler_map()
{
    // Example with enum
    HandlerMap<MyEnum, std::function<void(int, double)>> m1;
    m1  
        (MyEnum::Handler1, &MyDynamicHandler1::handler1)
        (MyEnum::Handler2, &MyDynamicHandler1::handler2);

    m1.call(MyEnum::Handler1, 1, 1.0);
    m1.call(MyEnum::Handler2, 2, 2.0);

    // Example with enum class
    HandlerMap<MyEnumClass, std::function<void(int, std::string&)>> m2;
    m2  
        (MyEnumClass::HandlerClass1, &MyDynamicHandler2::handler1)
        (MyEnumClass::HandlerClass2, &MyDynamicHandler2::handler2);

    std::string s1("atatat1");
    std::string s2("atatat2");
    m2.call(MyEnumClass::HandlerClass1, 1, std::ref(s1));
    m2.call(MyEnumClass::HandlerClass2, 2, std::ref(s2));

    try{
        // exception should be thrown, no handler for such key
        m2.call(MyEnumClass::HandlerClass3, 2, std::ref(s2));
    }
    catch (const std::exception& e){
        std::cerr << e.what() << '\n';
    }

    // Example with enum int, nullptr handler and return params
    HandlerMap<int, std::function<int(int, int)>> m3;
    m3  
        (0, &MyDynamicHandler3::handler1)
        (1, &MyDynamicHandler3::handler2)
        (2, nullptr);

    int i1 = m3.call(0, 3, 2);
    int i2 = m3.call(1, 3, 2);
    
    // default value returned
    int i3 = m3.call(2, 3, 2);
    std::cout << "i1 = " << i1 << '\n';
    std::cout << "i2 = " << i2 << '\n';
    std::cout << "i3 = " << i3 << '\n';

    // Example with non-static members
    MyDynamicHandler4 my_handler1(10);
    HandlerMap<int, std::function<void(MyDynamicHandler4&, int)>> m4;
    m4  
        (0, &MyDynamicHandler4::handler1)
        (1, &MyDynamicHandler4::handler2);
    m4.call(0, my_handler1, 10);
    m4.call(1, my_handler1, 20);

    // Example with calling from within the class
    MyDynamicHandler5 my_handler2(5);
    my_handler2.call();
}

int main()
{
#if 0
    test_static_handler_map();
#else
    test_dynamic_handler_map();
#endif
    return 0;
}
