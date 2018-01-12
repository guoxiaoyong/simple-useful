echo 'int main() { puts ("Hello world"); }' | gcc -x c - -c -o hello.o
echo "this is my special data" > mydata
objcopy --add-section .mydata=mydata --set-section-flags .mydata=noload,readonly hello.o hello2.o
gcc hello2.o -o hello
./hello
objdump -sj .mydata hello
