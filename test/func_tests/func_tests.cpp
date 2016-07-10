#include <thread>
#include <numeric>
#include <iostream>
#include <universal_handler_map/handler_map.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

using namespace boost::unit_test;
 
// Functional tests
 
BOOST_AUTO_TEST_SUITE(HandlerMapFunctionalTests);
 

// Test concurrent push/pop
BOOST_AUTO_TEST_CASE(Zero_Params_Test)
{
	std::cout << "Zero params test..." << std::endl;
	//testValuesConcurrently<int>();
	//testValuesConcurrently<long long>();
	//testValuesConcurrently<double>();
	//testValuesConcurrently<void*>();
}

 
BOOST_AUTO_TEST_SUITE_END()
 
