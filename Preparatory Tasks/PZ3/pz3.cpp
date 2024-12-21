#include <iostream>
#include <stdexcept>
using namespace std;

template <typename Tip>
class Stek{
    private:
    struct Cvor{
        Tip el;
        Cvor *sljedeci;
    };
    Cvor *top;
    int brel;
    public:
    Stek(){
        brel=0;
        top=nullptr;
    } //kreira prazan stek

    ~Stek(){
        while (top != nullptr) {
      auto temp = top;
      top = top->sljedeci;
      temp->sljedeci=nullptr;
      delete temp;
        }
        top = nullptr;
        brel=0;
    }

    Stek(const Stek &s){
        brel=0;
        top=nullptr;
        if (s.brel==0) top=nullptr;
        else
        {
            auto it=s.top;
            while (it!=nullptr)
            {
                stavi(it->el);
                it=it->sljedeci;
            }
            it=s.top;
            auto trenutni=top;
            while (it!=nullptr)
            {
                trenutni->el=it->el;
                it=it->sljedeci;
                trenutni=trenutni->sljedeci;
            }
        }

    } //kopirajuci

    Stek &operator =(const Stek &s){
        if (this==&s) return *this;
        while (top!=nullptr)
        {
            auto trenutni=top;
            top=top->sljedeci;
            delete trenutni;
        }
        brel=0;
        if (s.brel==0) top=nullptr;
        else
        {
            auto it=s.top;
            while (it!=nullptr)
            {
                stavi(it->el);
                it=it->sljedeci;
            }
            it=s.top;
            auto trenutni=top;
            while (it!=nullptr)
            {
                trenutni->el=it->el;
                it=it->sljedeci;
                trenutni=trenutni->sljedeci;
            }
            return *this;
        }
    }

    void brisi(){
        while (top!=nullptr)
        {
            auto it=top;
            top=top->sljedeci;
            delete it;
            it=nullptr;
            brel--;
        }
    } //brise sve elemente

    void stavi(const Tip &el){
        Cvor *novi = new Cvor {el, top};
        top=novi;
        novi=nullptr;
        brel++;
    } //stavlja el na vrh steka

    Tip skini(){
        if (brel==0) throw std::logic_error("Ne mogu brisat iz praznog steka!\n");
        Cvor *uljez=top;
        Tip element=uljez->el;
        top=top->sljedeci;
        delete uljez;
        uljez=nullptr;
        brel--;
        return element;
    } //vraca el na vrhu i brise ga, izuzetak ako je prazan

    Tip &vrh(){
        if (brel==0) throw std::logic_error("Ne mogu brisat iz praznog steka!\n");
        return top->el;
    } //vraca ref na el na vrhu ali ga ne brise, izuzetak ako je prazan

    int brojElemenata() const {return brel;}
};

int main(){
    Stek<int> s;
    s.stavi(2);
    s.stavi(4);
    s.stavi(9);
    s.stavi(7);
    std::cout<<"\nElement na vrhu je: "<<s.vrh();
    std::cout<<"\nBroj elemenata je: "<<s.brojElemenata();
    std::cout<<"\nElement skinut sa vrha je: "<<s.skini();
    std::cout<<"\nBroj elemenata je: "<<s.brojElemenata();
    std::cout<<std::endl;
    int brel=s.brojElemenata();
    for (int i=0;i<brel;i++) std::cout<<s.skini()<<" ";
    s.brisi();
    std::cout<<"\nBroj elemenata je: "<<s.brojElemenata();

}