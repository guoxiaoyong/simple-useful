#include <Python.h>

int main(int argc, char *argv[]) {
  Py_Initialize();
  if (!Py_IsInitialized()) {
    return -1;
  }

  PyRun_SimpleString("import sys");
  PyRun_SimpleString("sys.path.append('')");
  PyObject *pName, *pModule, *pDict, *pFunc, *pArgs;

  pName = PyString_FromString("pytest");
  pModule = PyImport_Import(pName);
  if (!pModule) {
    printf("Failed to load pytest");
    return -1;
  }
  pDict = PyModule_GetDict(pModule);
  if (!pDict) {
    return -1;
  }

  pFunc = PyDict_GetItemString(pDict, "foo");
  if (!pFunc || !PyCallable_Check(pFunc)) {
    printf("can't find function [foo]");
    return -1;
  }

  pArgs = PyTuple_New(2);
  PyTuple_SetItem(pArgs, 0, Py_BuildValue("l", 3));
  PyTuple_SetItem(pArgs, 1, Py_BuildValue("l", 4));
  PyObject_CallObject(pFunc, pArgs);

  pFunc = PyDict_GetItemString(pDict, "bar");
  if (!pFunc || !PyCallable_Check(pFunc)) {
    printf("can't find function [bar]");
    return -1;
  }

  PyObject_CallObject(pFunc, NULL);
  Py_DECREF(pName);
  Py_DECREF(pArgs);
  Py_DECREF(pModule);
  Py_Finalize();
  return 0;
}
