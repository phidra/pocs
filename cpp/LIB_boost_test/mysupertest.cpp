#define BOOST_TEST_MODULE this is an example of test module please note that spaces are ok
#include <boost/test/included/unit_test.hpp>

#include <iostream>

using namespace std;

constexpr int default_age = 42;

BOOST_AUTO_TEST_CASE(no_problem) {
    cout << endl << "NEW TEST" << endl;
    BOOST_WARN(true);
    BOOST_CHECK(true);
    BOOST_REQUIRE(true);
    cout << "OK : we see this step :-)" << endl;
}

BOOST_AUTO_TEST_CASE(test_dont_fail_even_if_warn_fails) {
    cout << endl << "NEW TEST" << endl;
    BOOST_WARN(false);  // test won't fail
    cout << "OK : we still see this step, and the test won't be marked as 'failed' :-)" << endl;
}

BOOST_AUTO_TEST_CASE(test_fails_when_check_fails_but_execution_continues) {
    cout << endl << "NEW TEST" << endl;
    BOOST_CHECK(false);  // test will fail but execution continues
    cout << "OK : we still see this step, BUT the test WILL be marked as 'failed' !" << endl;
}

BOOST_AUTO_TEST_CASE(test_fails_when_require_fails_and_execution_aborts) {
    cout << endl << "NEW TEST" << endl;
    cout << "We won't see any more step as execution will abort :-(" << endl;
    BOOST_REQUIRE(false);  // test will fail and execution aborts
    cout << "KO : we shouldn't see this step !" << endl;
}
