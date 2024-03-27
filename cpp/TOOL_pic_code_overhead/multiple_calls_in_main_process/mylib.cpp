int global_var = 42;

void __attribute__((noinline)) accumulate(int& accumulator, const int& value) {
    accumulator += 2 * value;
}
