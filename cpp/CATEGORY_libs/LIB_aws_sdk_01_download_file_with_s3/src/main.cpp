#include <iostream>

#include "s3.h"

int main(void) {
    std::string BUCKET = "MYBUCKET";
    std::string FILE = "my_super_test.txt";
    std::cout << "Getting content of :" << std::endl;
    std::cout << "BUCKET = " << BUCKET << std::endl;
    std::cout << "FILE = " << FILE << std::endl;

    auto content = s3::read_file(BUCKET, FILE);
    std::cout << "CONTENT = " << content << std::endl;
    return 0;
}
