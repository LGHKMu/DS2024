#ifndef FIB_H
#define FIB_H

class Fib {
private:
    int a, b;

public:
    // 构造函数，初始化斐波那契数列的前两个数
    Fib(int n = 1) : a(0), b(1) {
        while (b < n) {
            next();
        }
    }

    // 获取当前的斐波那契数
    int get() const {
        return b;
    }

    // 获取下一个斐波那契数
    void next() {
        int c = a + b;
        a = b;
        b = c;
    }

    // 获取前一个斐波那契数
    void prev() {
        int c = b - a;
        b = a;
        a = c;
    }
};

#endif // FIB_H
