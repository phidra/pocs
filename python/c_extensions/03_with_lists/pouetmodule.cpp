#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <string>
#include <iostream>

using namespace std;

static PyObject* pouet_double_the_ints(PyObject* self, PyObject* args) {
    // According to this, I never have to DECREF input_list :
    //      https://docs.python.org/3/c-api/arg.html#other-objects
    //      The object’s reference count is not increased.
    PyObject* input_list;
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &input_list)) {
        return NULL;
    }
    Py_ssize_t length = PyList_GET_SIZE(input_list);

    PyObject* my_returned_list = PyList_New(length);
    if (!my_returned_list) {
        return NULL;
    }

    for (Py_ssize_t i = 0; i < length; ++i) {
        PyObject* item = PyList_GetItem(input_list, i);  // reference is borrowed

        // is the item an integer (or convertible to it) ?
        long item_long = PyLong_AsLong(item);
        if (PyErr_Occurred()) {
            Py_DECREF(my_returned_list);
            return NULL;
        }

        // we ignore overflow errors for this POC...
        PyObject* doubled = PyLong_FromLong(2 * item_long);  // reference is strong
        if (!doubled) {
            Py_DECREF(my_returned_list);
            return NULL;
        }

        if (!PyList_SET_ITEM(my_returned_list, i, doubled)) {
            Py_DECREF(my_returned_list);
            Py_DECREF(doubled);
            return NULL;
        }
    }

    return my_returned_list;
}

static PyMethodDef PouetMethods[] = {
    {"double_the_ints", pouet_double_the_ints, METH_VARARGS, "Double ints in a list"},
    {NULL, NULL, 0, NULL} /* Sentinel */
};

static struct PyModuleDef pouetmodule = {PyModuleDef_HEAD_INIT, "pouet", NULL, -1, PouetMethods};

PyMODINIT_FUNC PyInit_pouet(void) {
    return PyModule_Create(&pouetmodule);
}
