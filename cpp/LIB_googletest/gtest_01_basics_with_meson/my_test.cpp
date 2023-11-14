#include "my_code.h"

#include <gtest/gtest.h>

TEST(MyCode, check_the_truth) {
    MyClass obj;
    EXPECT_EQ(obj.the_truth(), 42);
    // EXPECT_EQ(obj.the_truth(), 43);  // uncomment this to see a test failure
}
