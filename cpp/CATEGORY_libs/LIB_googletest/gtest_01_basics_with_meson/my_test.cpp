#include <gtest/gtest.h>

#include "my_code.h"

TEST(MyCode, check_the_truth) {
    MyClass obj;
    EXPECT_EQ(obj.the_truth(), 42);
    // EXPECT_EQ(obj.the_truth(), 43);  // uncomment this to see a test failure
}
