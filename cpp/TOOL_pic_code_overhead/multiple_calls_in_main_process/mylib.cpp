int global_var = 42;

void __attribute__((noinline)) accumulate(int& accumulator, int const& value) {
    accumulator += 2 * value;
}
