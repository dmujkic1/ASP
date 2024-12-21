#include <iostream>
#include <vector>
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

    bool prazan() const {if (top==nullptr && brel==0) return true; return false;}
};


void pretraga(Stek<vector<int>> &s, int trazeni) { // stek vektora
  static int brojel=s.brojElemenata();
  /*std::vector<int> sortirani;                      // valjda su vec takvi
  int vrh;     // zadnja pozicija u preostalom dijelu niza nakon polovljenja
  int dno = 0; // prva pozicija u preostalom dijelu niza nakon polovljenja
  int srednji; // srednja pozicija u nizu
  bool nadjen = false;*/
  if(brojel==0) {
      std::cout<<"Nema elementa";
      return;
  }
  if (s.brojElemenata() == 0) {
    return;
  }
  std::vector<int>sortirani=s.skini();
  pretraga(s,trazeni);
  s.stavi(sortirani);
  int dno=0;
  int vrh=sortirani.size()-1;
  int srednji=(dno-vrh)/2;
  static int indeks=-1;

  while (vrh >= dno) {
      srednji = (dno + vrh) / 2;
      if (sortirani[srednji] == trazeni) {
        trazeni = srednji;
        indeks=srednji;
        std::cout << srednji << " " << s.brojElemenata()-1 << std::endl;
        break;
      } else if (sortirani[srednji] > trazeni)
        vrh = srednji - 1;
      else
        dno = srednji + 1;
    }

    if (indeks==-1 && s.brojElemenata()==brojel)
    {
        std::cout << "Nema elementa" << std::endl;
        return;
    }


    //if (s.brojElemenata()==brojel) {std::cout<<"Nema elementa"<<std::endl; return;}
  /*while (!s.prazan()) {
    sortirani = s.skini();
    dno = 0;
    vrh = sortirani.size() - 1;
    while (vrh >= dno) {
      srednji = (dno + vrh) / 2;
      if (sortirani[srednji] == trazeni) {
        trazeni = srednji;
        nadjen = true;
        break;
      } else if (sortirani[srednji] > trazeni)
        vrh = srednji - 1;
      else
        dno = srednji + 1;
    }
    if (nadjen == true)
      break;
  }*/



  /*if (nadjen == false) {
    std::cout << "Nema elementa" << std::endl;
    return;
  }
  if (nadjen == true) // napisi index elementa u pronadjenom vektoru i koliko
                      // ima preostalih vektora u steku
  {
    std::cout << srednji << " " << s.brojElemenata() << std::endl;
  }*/

}

int main(){
    Stek<std::vector<int>> s;
    std::vector<int>a{3,5,7,9,11};
    std::vector<int>b{12,14,16};
    std::vector<int>c{20,25};
    s.stavi(a);
    s.stavi(b);
    s.stavi(c);
    Stek<std::vector<int>> s1=s;
    pretraga(s, 25); // 1 2
    pretraga(s1, 9); // 3 0

    return 0;
}