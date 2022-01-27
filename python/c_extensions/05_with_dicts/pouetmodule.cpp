#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

static PyObject* pouet_edit_the_dict(PyObject* self, PyObject* args) {
    PyObject* input_dict;
    // According to this, I never have to DECREF input_dict :
    //      https://docs.python.org/3/c-api/arg.html#other-objects
    //      The object’s reference count is not increased.
    if (!PyArg_ParseTuple(args, "O!", &PyDict_Type, &input_dict)) {
        return NULL;
    }

    // add a new key :
    PyObject* father = PyUnicode_FromString("anakin");
    if (!father) {
        return NULL;
    }

    if (PyDict_SetItemString(input_dict, "luke_father", father)) {
        Py_DECREF(father);
        return NULL;
    }
    Py_DECREF(father);  // SetItemString does not steal the reference, so we must decref here.

    // edit key (exception if missing) :
    PyObject* age = PyDict_GetItemString(input_dict, "luke_age");
    if (!age) {
        // PyDict_GetItemString does NOT set an exception
        PyErr_SetString(PyExc_ValueError, "Missing key 'luke_age'");
        return NULL;
    }

    long age_long = PyLong_AsLong(age);
    if (PyErr_Occurred()) {
        return NULL;
    }

    PyObject* age_incremented = PyLong_FromLong(age_long + 1);

    if (PyErr_Occurred()) {
        return NULL;
    }

    if (PyDict_SetItemString(input_dict, "luke_age", age_incremented)) {
        Py_DECREF(age_incremented);
        return NULL;
    }
    Py_DECREF(age_incremented);

    // delete key (exception if missing) :
    if (PyDict_DelItemString(input_dict, "luke_sister")) {
        return NULL;
    }

    return input_dict;
}

static PyObject* pouet_create_a_dict(PyObject* self, PyObject* args) {
    PyObject* newdict = PyDict_New();
    if (!newdict) {
        return NULL;
    }

    // inserting "star": "wars"
    PyObject* wars = PyUnicode_FromString("wars");
    if (!wars) {
        Py_DECREF(newdict);
        return NULL;
    }

    if (PyDict_SetItemString(newdict, "star", wars)) {
        Py_DECREF(wars);
        Py_DECREF(newdict);
        return NULL;
    }
    Py_DECREF(wars);

    // inserting "eleven squared": 121
    PyObject* squared = PyLong_FromLong(121);
    if (!squared) {
        Py_DECREF(newdict);
        return NULL;
    }

    if (PyDict_SetItemString(newdict, "eleven squared", squared)) {
        Py_DECREF(squared);
        Py_DECREF(newdict);
        return NULL;
    }
    Py_DECREF(squared);

    return newdict;
}

static PyMethodDef PouetMethods[] = {
    {"edit_the_dict", pouet_edit_the_dict, METH_VARARGS, "Mutates a dict"},
    {"create_a_dict", pouet_create_a_dict, METH_VARARGS, "Creates a dict"},
    {NULL, NULL, 0, NULL} /* Sentinel */
};

static struct PyModuleDef pouetmodule = {PyModuleDef_HEAD_INIT, "pouet", NULL, -1, PouetMethods};

PyMODINIT_FUNC PyInit_pouet(void) {
    return PyModule_Create(&pouetmodule);
}
