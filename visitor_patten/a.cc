#include <iostream>
#include <vector>

class AbstractDispatcher; // Forward declare AbstractDispatcher

class File { // Parent class for the elements (ArchivedFile, SplitFile and
             // ExtractedFile)
public:
  // This function accepts an object of any class derived from
  // AbstractDispatcher and must be implemented in all derived classes
  virtual void accept(AbstractDispatcher &dispatcher) = 0;
};

// Forward declare specific elements (files) to be dispatched
class ArchivedFile;
class SplitFile;
class ExtractedFile;

class AbstractDispatcher { // Declares the interface for the dispatcher
public:
  // Declare overloads for each kind of a file to dispatch
  virtual void dispatch(ArchivedFile &file) = 0;
  virtual void dispatch(SplitFile &file) = 0;
  virtual void dispatch(ExtractedFile &file) = 0;
};

class ArchivedFile : public File { // Specific element class #1
public:
  // Resolved at runtime, it calls the dispatcher's overloaded function,
  // corresponding to ArchivedFile.
  void accept(AbstractDispatcher &dispatcher) override {
    dispatcher.dispatch(*this);
  }
};

class SplitFile : public File { // Specific element class #2
public:
  // Resolved at runtime, it calls the dispatcher's overloaded function,
  // corresponding to SplitFile.
  void accept(AbstractDispatcher &dispatcher) override {
    dispatcher.dispatch(*this);
  }
};

class ExtractedFile : public File { // Specific element class #3
public:
  // Resolved at runtime, it calls the dispatcher's overloaded function,
  // corresponding to ExtractedFile.
  void accept(AbstractDispatcher &dispatcher) override {
    dispatcher.dispatch(*this);
  }
};

class Dispatcher : public AbstractDispatcher { // Implements dispatching of all
                                               // kind of elements (files)
public:
  void dispatch(ArchivedFile &file) override {
    std::cout << "dispatching ArchivedFile" << std::endl;
  }

  void dispatch(SplitFile &file) override {
    std::cout << "dispatching SplitFile" << std::endl;
  }

  void dispatch(ExtractedFile &file) override {
    std::cout << "dispatching ExtractedFile" << std::endl;
  }
};

int main() {
  ArchivedFile archivedFile;
  SplitFile splitFile;
  ExtractedFile extractedFile;

  std::vector<File *> files;
  files.push_back(&archivedFile);
  files.push_back(&splitFile);
  files.push_back(&extractedFile);

  Dispatcher dispatcher;

  for (File *file : files) {
    file->accept(dispatcher);
  }
}
