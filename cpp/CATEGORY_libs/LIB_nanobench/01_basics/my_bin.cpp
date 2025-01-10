#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>

#include "nanobench.h"

void bench_to_csv(std::filesystem::path out_name) {
    std::ofstream out(out_name);
    auto bench = ankerl::nanobench::Bench();
    bench.output(nullptr);
    bench.run("sleep 100 ms", [&]() { std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
    bench.render(ankerl::nanobench::templates::csv(), out);
}

void bench_to_stdout() {
    ankerl::nanobench::Bench().run("sleep 100 ms",
                                   [&]() { std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
}

void bench_to_html(std::filesystem::path out_name) {
    std::ofstream out(out_name);
    auto bench = ankerl::nanobench::Bench();
    bench.output(nullptr);
    bench.run("sleep 100 ms", [&]() { std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
    bench.render(ankerl::nanobench::templates::htmlBoxplot(), out);
}

int main(void) {
    std::filesystem::path csv_file = "/tmp/nanobench.csv";
    std::cout << ">>> BENCH TO CSV = " << csv_file << std::endl;
    bench_to_csv(csv_file);
    std::cout << std::endl;

    std::filesystem::path html_file = "/tmp/nanobench.html";
    std::cout << ">>> BENCH TO HTML = " << html_file << std::endl;
    bench_to_html(html_file);
    std::cout << std::endl;

    std::cout << ">>> BENCH TO STDOUT :" << std::endl;
    bench_to_stdout();
    std::cout << std::endl;

    return 0;
}
