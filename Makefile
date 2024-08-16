# 预处理
g++ -std=c++17 -Iinclude -E src/test.cpp -o lib/test.i

# 编译
g++ -std=c++17 -Iinclude -S lib/test.i -o lib/test.s

# 汇编
g++ -std=c++17 -Iinclude -c lib/test.s -o lib/test.o

# 链接
g++ -std=c++17 lib/test.o -o build/my_project

rm -rf lib/test.i lib/test.s lib/test.o