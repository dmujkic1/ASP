#include <iostream>
using namespace std;

/*int fib_petlja(int n){
    if (n<=1) return n; // 0 i 1
    int pretprosli(0), prosli(1), rezultat;
    for (int i(2); i<=n; i++) {
        rezultat = pretprosli+prosli;
        pretprosli = prosli;
        prosli = rezultat;
    }
    return rezultat;
}*/

 // 0 1 1 2 3 5 8 13 21 34 55
int fib2_0(int n, int trenutni=1, int prosli=0){
    if (n==0) return prosli;
    return fib2_0(n-1, trenutni+prosli, trenutni);
}

int nzd(int x, int y){
    if (y==0) return x;
    return nzd(y, x%y);
}

int main(){
    int rez=fib2_0(9);
    int a=6, b=561;
    std::cout<<"34 = "<<rez<<std::endl;
    if (rez==34) std::cout<<"BRAVO LEGENDO!";
    else std::cout<<"Malo sutra jarane...";
    std::cout<<"\nNZD brojeva "<<a<<" i "<<b<<" je: "<<nzd(a, b);
}