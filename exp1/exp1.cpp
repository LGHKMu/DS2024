#include <iostream> 
#include <ctime>
#include <cmath>
#include "vector.cpp"
using namespace std;

class Complex {
    public:
    int real;
    int image;
    double Mo;
    Complex(int x = 0,int y = 0):real(x),image(y){
        Mo=sqrt(real*real+image*image);
    }
    //Complex(int x=0):Mo(x){}
    bool operator ==(const Complex& other) const {  
        return real == other.real && image == other.image;  
    }  
    bool operator !=(const Complex& other) const {  
        return real != other.real || image != other.image;  
    }  
    bool operator <=(const Complex& other) const {  
        if (Mo-other.Mo==0)
        {
            return real<=other.real; 
        }
        return Mo <= other.Mo; 
    }
    bool operator <(const Complex& other) const {  
        if (Mo-other.Mo!=0)
        {
           return Mo < other.Mo;  
        }
        return real<other.real;
    }
    bool operator >(const Complex& other) const {  
        if (Mo-other.Mo!=0)
        {
           return Mo > other.Mo;  
        }
        return real>other.real;
    }
    bool operator >=(const Complex& other) const {  
        if (Mo-other.Mo!=0)
        {
           return Mo >= other.Mo;  
        }
        return real>=other.real;
    }

    /*void print(){
        cout<<real<<"+"<<image<<"i"<<endl;
    
    }*/
    
};  
std::ostream& operator<<(std::ostream& os, const Complex & x) {  
    return os <<x.real << "+" << x.image << "i";
}
void swap(Complex& first, Complex& second) noexcept {  
    using std::swap;  
    swap(first.real, second.real);  
    swap(first.image, second.image);  
}

int main(){
    srand(static_cast<unsigned>(time(0)));
    clock_t start, end;  
    double time; 
    double m1=2;double m2=5;
    Vector<Complex> TEST;  
    for (int i = 0; i < 10; ++i) {  
        TEST.insert(Complex(int(rand()) % 5, int(rand())% 5));
    }
    TEST.print();

    TEST.unsort();
    TEST.print();
    cout<<TEST.find(Complex(1,0))<<endl;
    TEST.print();
    TEST.insert(Complex(3,4));
    TEST.print();
    TEST.remove(2);
    TEST.print();
    cout<<TEST.deduplicate()<<endl;
    TEST.print(); 


    TEST.sort();
    start = clock();  
    TEST.sort();//顺序
    end = clock();
    TEST.print();  
    time = ((double) (end - start));
    cout<<time<<endl;

    TEST.sort();
    TEST.OPsort();
    start = clock();
    TEST.sort();//逆序
    end = clock(); 
    TEST.print(); 
    time = ((double) (end - start));
    cout<<time<<endl;

    TEST.sort();
    TEST.unsort();
    start = clock();
    TEST.sort();//乱序
    end = clock();  
    TEST.print();
    time = ((double) (end - start));
    cout<<time<<endl;

    Vector<Complex> TEST2(TEST,TEST.search(Complex(0,m1)),TEST.search(Complex(m2,0)));
    TEST2.print();
    return 0;
}


