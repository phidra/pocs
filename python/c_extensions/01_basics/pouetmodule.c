#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject* pouet_system(PyObject* self, PyObject* args) {
    const char* command;
    int sts;

    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;
    sts = system(command);
    return PyLong_FromLong(sts);
}

static PyMethodDef PouetMethods[] = {
    {"system", pouet_system, METH_VARARGS, "Execute a shell command."},
    {NULL, NULL, 0, NULL} /* Sentinel */
};

static struct PyModuleDef pouetmodule = {PyModuleDef_HEAD_INIT, "pouet", NULL, /* no documentation */
                                         -1, PouetMethods};

PyMODINIT_FUNC PyInit_pouet(void) {
    return PyModule_Create(&pouetmodule);
}
