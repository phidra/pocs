#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <string>
#include <iostream>

using namespace std;

static PyObject* pouet_count_nb_chars(PyObject* self, PyObject* args) {
    const char* input_c_str;

    if (!PyArg_ParseTuple(args, "s", &input_c_str))
        return NULL;
    string input_str(input_c_str);
    return PyLong_FromLong(input_str.size());
}

static PyMethodDef PouetMethods[] = {
    {"count_nb_chars", pouet_count_nb_chars, METH_VARARGS, "Count number of chars in a string"},
    {NULL, NULL, 0, NULL} /* Sentinel */
};

static struct PyModuleDef pouetmodule = {PyModuleDef_HEAD_INIT, "pouet", NULL, -1, PouetMethods};

PyMODINIT_FUNC PyInit_pouet(void) {
    return PyModule_Create(&pouetmodule);
}
