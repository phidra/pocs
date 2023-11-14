#include "my_code.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("testing my own code", "function check_the_truth") {
    MyClass obj;
    REQUIRE(obj.the_truth() == 42);
    /* REQUIRE(obj.the_truth() == 43);  // uncomment this to see a test failure */
}
