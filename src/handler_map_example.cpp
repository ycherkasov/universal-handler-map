#include <universal_handler_map/handler_map.h>
#include <iostream>
#include <map>



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


int main()
{
    UniversalMapCall(NonParam, 0)::call();
    UniversalMapCall(NonParam, 1)::call();
    UniversalMapCall(NonParam, 2)::call();
    UniversalMapCall(NonParam, 3)::call();

    UniversalMapCall(OneParam, 0)::call(0);
    UniversalMapCall(OneParam, 1)::call(1);

    return 0;
}
