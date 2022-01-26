#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

static PyObject* pouet_upper_the_string(PyObject* self, PyObject* args) {
    const char* input_string_c;
    if (!PyArg_ParseTuple(args, "s", &input_string_c)) {
        return NULL;
    }
    string cpp_str(input_string_c);
    transform(cpp_str.begin(), cpp_str.end(), cpp_str.begin(), ::toupper);
    return PyUnicode_FromString(cpp_str.c_str());
}

static PyMethodDef PouetMethods[] = {
    {"upper_the_string", pouet_upper_the_string, METH_VARARGS, "Double ints in a list"},
    {NULL, NULL, 0, NULL} /* Sentinel */
};

static struct PyModuleDef pouetmodule = {PyModuleDef_HEAD_INIT, "pouet", NULL, -1, PouetMethods};

PyMODINIT_FUNC PyInit_pouet(void) {
    return PyModule_Create(&pouetmodule);
}
