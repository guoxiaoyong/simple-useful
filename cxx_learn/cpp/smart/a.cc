class NonCopyable {

public:
    NonCopyable():Base(0) {}
    NonCopyable(int InBase):Base(InBase) {}

private:
    NonCopyable(const NonCopyable& In);
    NonCopyable& operator=(const NonCopyable& In);
    int Base;
};

int main() {

    NonCopyable nc(1);
    return 0;
}
