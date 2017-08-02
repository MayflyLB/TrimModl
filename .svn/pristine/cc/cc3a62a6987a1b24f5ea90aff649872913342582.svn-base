#include <vector>
#include <iostream>
class B
{
public:
    B(const char *x)
    {
        strcpy(flag, x);
        std::cout << "B(char*)===" << flag << "\n";
    }
    B()
    {
        std::cout << "===B()===" << flag << "\n";
    }
    B(const B &x)
    {
        strcpy(flag, x.flag);
        std::cout << "B(B&)===" << flag << "----copy" << "\n";
    }
    ~B()
    {
        std::cout << "~B()===" << flag << "\n";
        strcpy(flag, "Flag is disconstruct!!!");
    }
    void printValue()
    {
        std::cout <<"printValue()"<< flag << "\n";
    }

    void setValue(const char *x)
    {
        strcpy(flag, x);
    }
private:
    char flag[64];
};

B fun(char * x)
{
    B a(x);
    a.setValue("fun");
    return a;
}
void test()
{
//    fun("void");
//     std::cout << "111111111111111111111111111111\n";
//     B aa = fun("aa");
//     aa.printValue();
//    aa.setValue("new aa");
//    std::cout << "222222222222222222222222222222\n";
    B& xx = fun("bb");
    xx.printValue();
    xx.setValue("new bb");
//    std::cout << "333333333333333333333333333333\n";

}

int main()
{
    test();
    getchar();
}

