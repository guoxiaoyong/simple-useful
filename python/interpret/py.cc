struct python_interpreter {
  python_interpreter() {
    Py_InitializeEx(false):
    PyEval_InitThreads();
    thread_state = PyEval_SaveThread();
  }

  ~python_interpreter() {
    PyEval_RestoreThread(thread_state);
    Py_Finalize();
  }
}

struct global_interpreter_lock {
  global_interpreter_lock() {
    gil_state = PyGILState_Ensure();
  }

  PyGILState_STATE gil_state;
}
