#define asChar(x, n) #x[n]
#define FOO(x) asChar(x, 0), asChar(x, 1), asChar(x, 2)

FOO(abc)
