class ExcA {};
class ExcB {};
class ExcC {};
class ExcD {};
class ExcE {};

void ext_func();

void filter_only() throw (ExcA, ExcB, ExcC, ExcD, ExcE) {
    ext_func();
}


