#include "fibinacci.h"

unsigned int fibinacci(const unsigned int n) {
    if (n < 2) {
        return n;
    }
    return fibinacci(n - 1) + fibinacci(n - 2);
}
