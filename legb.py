def func1(param=None):
    def func2():
        if not param:
            param = 'default'
        print param
    # Just return func2.
    return func2


if __name__ == '__main__':
    func1('test')()
