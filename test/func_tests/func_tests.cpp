#include <stack>
#include <iostream>
#include <universal_handler_map/handler_map.h>

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

BOOST_AUTO_TEST_CASE(Zero_Params_Test)
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

BOOST_AUTO_TEST_CASE(One_Params_Test)
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


BOOST_AUTO_TEST_SUITE_END()
 
