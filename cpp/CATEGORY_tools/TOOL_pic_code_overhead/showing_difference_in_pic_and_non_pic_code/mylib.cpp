void __attribute__((noinline)) accumulate(int& accumulator, int const& value) {
    accumulator += 2 * value;
}

void __attribute__((noinline)) process(int& input) {
    accumulate(input, 42);
    accumulate(input, 43);
    accumulate(input, 44);
}
