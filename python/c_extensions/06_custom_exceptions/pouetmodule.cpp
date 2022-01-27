#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <string>
#include <iostream>

using namespace std;

static PyObject* TeuopError;  // custom exception

static PyObject* pouet_fail_with_ValueError(PyObject* self, PyObject* args) {
    PyErr_SetString(PyExc_ValueError, "I am a dreadful ValueError, fear me !");
    return NULL;
}

static PyObject* pouet_fail_with_PouetError(PyObject* self, PyObject* args) {
    PyErr_SetString(TeuopError, "I am a not less dreadful custom error !");
    return NULL;
}

static PyMethodDef PouetMethods[] = {
    {"fail_with_ValueError", pouet_fail_with_ValueError, METH_VARARGS, "raises ValueError"},
    {"fail_with_PouetError", pouet_fail_with_PouetError, METH_VARARGS, "raises PouetERror"},
    {NULL, NULL, 0, NULL} /* Sentinel */
};

static struct PyModuleDef pouetmodule = {PyModuleDef_HEAD_INIT, "pouet", NULL, -1, PouetMethods};

PyMODINIT_FUNC PyInit_pouet(void) {
    PyObject* m = PyModule_Create(&pouetmodule);
    if (m == NULL) {
        return NULL;
    }
    TeuopError = PyErr_NewExceptionWithDoc("pouet.TeuopError", "This is my custom exception", NULL, NULL);
    Py_XINCREF(TeuopError);
    if (PyModule_AddObject(m, "TeuopError", TeuopError) < 0) {
        Py_XDECREF(TeuopError);
        Py_CLEAR(TeuopError);
        Py_DECREF(m);
        return NULL;
    }
    return m;
}
