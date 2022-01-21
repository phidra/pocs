import time
import pybind_11_example as pbe


def fibinacci_py(x: int) -> int:
    if x < 2:
        return x
    return fibinacci_py(x - 1) + fibinacci_py(x - 2)


def main() -> None:
    print(
        """
This POC is a working usage of pygind11.
Source = https://www.blopig.com/blog/2021/03/c-python-bindings-in-5-minutes/

To run POC :
    pip install pybind11
    pip install -e .
    python main.py

TYPICAL OUTPUT :
    python main.py
    Python:
    Answer: 102334155
    Time: 31.751701339 s

    C++:
    Answer: 102334155
    Time: 0.274786535 s

    Both answers are equal ?  True
    py/cpp ratio =  115.55042658476697
"""
    )
    n = 40

    print("Python:")
    start_time = time.perf_counter_ns()
    answer_py = fibinacci_py(n)
    time_py = (time.perf_counter_ns() - start_time) / 1e9
    print("Answer:", answer_py)
    print("Time:", time_py, "s")
    print()

    print("C++:")
    start_time = time.perf_counter_ns()
    answer_cpp = pbe.fibinacci_cpp(n)
    time_cpp = (time.perf_counter_ns() - start_time) / 1e9
    print("Answer:", answer_cpp)
    print("Time:", time_cpp, "s")

    print()
    print("Are both answers equal ? ", answer_py == answer_cpp)
    print("py/cpp ratio = ", time_py / time_cpp)


if __name__ == "__main__":
    main()
