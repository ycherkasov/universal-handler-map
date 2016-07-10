#include <stack>
#include <iostream>
#include <sstream>
#include <universal_handler_map/static_handler_map.h>
#include <universal_handler_map/dynamic_handler_map.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

using namespace boost::unit_test;
 
// Functional tests

// Push test values so that take back in reverse order
class StackChecker
{
public:
    
    static std::stack<std::string>& instance()
    {
        return checker_stack_;
    }

private:
    static std::stack<std::string> checker_stack_;
};
std::stack<std::string> StackChecker::checker_stack_;

BOOST_AUTO_TEST_SUITE(HandlerMapFunctionalTests);
 
void zero_handler()
{
    StackChecker::instance().push("zero_handler()");
}

void one_handler()
{
    StackChecker::instance().push("one_handler()");
}

class MyHandlers
{
public:
    static void class_handler_two()
    {
        StackChecker::instance().push("MyHandlers::class_handler_two()");
    }
    static void class_handler_three()
    {
        StackChecker::instance().push("MyHandlers::class_handler_three()");
    }
};

// Map 0-params handlers to integer values
DeclareUniversalMap(NonParam, std::function<void()>);
InsertUniversalMap(NonParam, 0, &zero_handler);
InsertUniversalMap(NonParam, 1, &one_handler);
InsertUniversalMap(NonParam, 2, &MyHandlers::class_handler_two);
InsertUniversalMap(NonParam, 3, &MyHandlers::class_handler_three);

BOOST_AUTO_TEST_CASE(Static_Zero_Params_Test)
{
	std::cout << "Zero params test..." << std::endl;

    // Call 0-params handlers
    UniversalMapCall(NonParam, 0)::call();
    UniversalMapCall(NonParam, 1)::call();
    UniversalMapCall(NonParam, 2)::call();
    UniversalMapCall(NonParam, 3)::call();

    BOOST_CHECK_EQUAL(StackChecker::instance().top(), std::string("MyHandlers::class_handler_three()"));
    StackChecker::instance().pop();
    BOOST_CHECK_EQUAL(StackChecker::instance().top(), std::string("MyHandlers::class_handler_two()"));
    StackChecker::instance().pop();
    BOOST_CHECK_EQUAL(StackChecker::instance().top(), std::string("one_handler()"));
    StackChecker::instance().pop();
    BOOST_CHECK_EQUAL(StackChecker::instance().top(), std::string("zero_handler()"));
    StackChecker::instance().pop();
    BOOST_CHECK_EQUAL(StackChecker::instance().empty(), true);
}

void zero_handler1(int i)
{
    std::stringstream s;
    s << "zero_handler1(" << i << ")";
    StackChecker::instance().push(s.str());
}

void one_handler1(int i)
{
    std::stringstream s;
    s << "one_handler1(" << i << ")";
    StackChecker::instance().push(s.str());
}


class MyHandlers1
{
public:
    static void class_handler_two(int i)
    {
        std::stringstream s;
        s << "MyHandlers1::class_handler_two(" << i << ")";
        StackChecker::instance().push(s.str());
    }
    static void class_handler_three(int i)
    {
        std::stringstream s;
        s << "MyHandlers1::class_handler_three(" << i << ")";
        StackChecker::instance().push(s.str());
    }
};

// Map 1-params handlers to integer values
DeclareUniversalMap(OneParam, std::function<void(int)>);
InsertUniversalMap(OneParam, 0, &zero_handler1);
InsertUniversalMap(OneParam, 1, &one_handler1);
InsertUniversalMap(OneParam, 2, &MyHandlers1::class_handler_two);
InsertUniversalMap(OneParam, 3, &MyHandlers1::class_handler_three);

BOOST_AUTO_TEST_CASE(Static_One_Params_Test)
{
    std::cout << "One params test..." << std::endl;

    // Call 1-params handlers
    UniversalMapCall(OneParam, 0)::call(0);
    UniversalMapCall(OneParam, 1)::call(1);
    UniversalMapCall(OneParam, 2)::call(2);
    UniversalMapCall(OneParam, 3)::call(3);

    BOOST_CHECK_EQUAL(StackChecker::instance().top(), std::string("MyHandlers1::class_handler_three(3)"));
    StackChecker::instance().pop();
    BOOST_CHECK_EQUAL(StackChecker::instance().top(), std::string("MyHandlers1::class_handler_two(2)"));
    StackChecker::instance().pop();
    BOOST_CHECK_EQUAL(StackChecker::instance().top(), std::string("one_handler1(1)"));
    StackChecker::instance().pop();
    BOOST_CHECK_EQUAL(StackChecker::instance().top(), std::string("zero_handler1(0)"));
    StackChecker::instance().pop();
    BOOST_CHECK_EQUAL(StackChecker::instance().empty(), true);
}

// Simple test with dynamic handler map
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
        std::stringstream s;
        s << "i1 = " << i << " d1 = " << d;
        StackChecker::instance().push(s.str());
    }

    static void handler2(int i, double d)
    {
        std::stringstream s;
        s << "i2 = " << i << " d2 = " << d;
        StackChecker::instance().push(s.str());
    }
};

BOOST_AUTO_TEST_CASE(Dynamic_Enum_Test)
{
    std::cout << "Dynamic map simple test..." << std::endl;
    // Example with enum
    HandlerMap<MyEnum, std::function<void(int, double)>> m1;
    m1
        (MyEnum::Handler1, &MyDynamicHandler1::handler1)
        (MyEnum::Handler2, &MyDynamicHandler1::handler2);

    // should call associated handlers
    m1.call(MyEnum::Handler1, 1, 1.0);
    m1.call(MyEnum::Handler2, 2, 2.0);

    BOOST_CHECK_EQUAL(StackChecker::instance().top(), std::string("i2 = 2 d2 = 2"));
    StackChecker::instance().pop();
    BOOST_CHECK_EQUAL(StackChecker::instance().top(), std::string("i1 = 1 d1 = 1"));
    StackChecker::instance().pop();
    BOOST_CHECK_EQUAL(StackChecker::instance().empty(), true);
}

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
    static void handler1(int i, std::string& s1)
    {
        std::stringstream s;
        s << "i1 = " << i << " s1 = " << s1;
        StackChecker::instance().push(s.str());
    }

    static void handler2(int i, std::string& s2)
    {
        std::stringstream s;
        s << "i2 = " << i << " s2 = " << s2;
        StackChecker::instance().push(s.str());
    }
};

BOOST_AUTO_TEST_CASE(Dynamic_Enum_Class_Exception_Test)
{
    std::cout << "Dynamic map simple test. Example with enum class..." << std::endl;
    HandlerMap<MyEnumClass, std::function<void(int, std::string&)>> m2;
    m2
        (MyEnumClass::HandlerClass1, &MyDynamicHandler2::handler1)
        (MyEnumClass::HandlerClass2, &MyDynamicHandler2::handler2);

    std::string s1("atatat1");
    std::string s2("atatat2");

    // should call associated handlers
    m2.call(MyEnumClass::HandlerClass1, 1, std::ref(s1));
    m2.call(MyEnumClass::HandlerClass2, 2, std::ref(s2));

    try {
        // exception should be thrown, no handler for such key
        m2.call(MyEnumClass::HandlerClass3, 2, std::ref(s2));
    }
    catch (const std::exception& e) {
        BOOST_CHECK_EQUAL(e.what(), "Inappropriate handler value");
        std::cerr << e.what() << '\n';
    }


    BOOST_CHECK_EQUAL(StackChecker::instance().top(), std::string("i2 = 2 s2 = atatat2"));
    StackChecker::instance().pop();
    BOOST_CHECK_EQUAL(StackChecker::instance().top(), std::string("i1 = 1 s1 = atatat1"));
    StackChecker::instance().pop();
    BOOST_CHECK_EQUAL(StackChecker::instance().empty(), true);
}


// Test with return values
class MyDynamicHandler3
{
public:
    static int handler1(int i1, int i2)
    {
        std::stringstream s;
        s << "i1 + i2 = " << i1 + i2;
        StackChecker::instance().push(s.str());
        return i1 + i2;
    }

    static int handler2(int i1, int i2)
    {
        std::stringstream s;
        s << "i1 - i2 = " << i1 - i2;
        StackChecker::instance().push(s.str());
        return i1 - i2;
    }
};

BOOST_AUTO_TEST_CASE(Dynamic_Int_Null_Test)
{
    std::cout << "Example with enum int, nullptr handler and return params..." << std::endl;
    HandlerMap<int, std::function<int(int, int)>> m3;
    m3
        (0, &MyDynamicHandler3::handler1)
        (1, &MyDynamicHandler3::handler2)
        (2, nullptr);

    // should call associated handlers
    int i1 = m3.call(0, 3, 2);
    int i2 = m3.call(1, 3, 2);

    BOOST_CHECK_EQUAL(StackChecker::instance().top(), std::string("i1 - i2 = 1"));
    StackChecker::instance().pop();
    BOOST_CHECK_EQUAL(StackChecker::instance().top(), std::string("i1 + i2 = 5"));
    StackChecker::instance().pop();
    BOOST_CHECK_EQUAL(StackChecker::instance().empty(), true);

    // default value returned, handler is nullptr
    int i3 = m3.call(2, 3, 2);

    BOOST_CHECK_EQUAL(i1, 5);
    BOOST_CHECK_EQUAL(i2, 1);
    BOOST_CHECK_EQUAL(i3, 0);
}


// Test with non-static members
class MyDynamicHandler4
{
    int val_;
public:

    MyDynamicHandler4(int val) : val_(val) {}

    void handler1(int i)
    {
        std::stringstream s;
        s << "i + val = " << i + val_;
        StackChecker::instance().push(s.str());

    }

    void handler2(int i)
    {
        std::stringstream s;
        s << "i - val = " << i - val_;
        StackChecker::instance().push(s.str());
    }
};

BOOST_AUTO_TEST_CASE(Dynamic_Int_NonStatic_Test)
{
    std::cout << "Example with non-static members..." << std::endl;

    MyDynamicHandler4 my_handler1(10);
    HandlerMap<int, std::function<void(MyDynamicHandler4&, int)>> m4;
    m4
        (0, &MyDynamicHandler4::handler1)
        (1, &MyDynamicHandler4::handler2);

    m4.call(0, my_handler1, 10);
    m4.call(1, my_handler1, 20);

    BOOST_CHECK_EQUAL(StackChecker::instance().top(), std::string("i - val = 10"));
    StackChecker::instance().pop();
    BOOST_CHECK_EQUAL(StackChecker::instance().top(), std::string("i + val = 20"));
    StackChecker::instance().pop();
    BOOST_CHECK_EQUAL(StackChecker::instance().empty(), true);

}


// Test calling from the class itself
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
        std::stringstream s;
        s << "i + val = " << i + val_;
        StackChecker::instance().push(s.str());
    }

    void handler2(int i)
    {
        std::stringstream s;
        s << "i - val = " << i - val_;
        StackChecker::instance().push(s.str());
    }
};

BOOST_AUTO_TEST_CASE(Dynamic_Int_Within_Class_Test)
{
    std::cout << "Example with calling from within the class..." << std::endl;
    MyDynamicHandler5 my_handler2(5);
    my_handler2.call();

    BOOST_CHECK_EQUAL(StackChecker::instance().top(), std::string("i - val = 15"));
    StackChecker::instance().pop();
    BOOST_CHECK_EQUAL(StackChecker::instance().top(), std::string("i + val = 15"));
    StackChecker::instance().pop();
    BOOST_CHECK_EQUAL(StackChecker::instance().empty(), true);
}

BOOST_AUTO_TEST_SUITE_END()
 
