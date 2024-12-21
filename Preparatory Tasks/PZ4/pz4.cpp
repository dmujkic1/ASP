#include <iostream>
#include <stdexcept>
using namespace std;

template <typename Tip> class Red { // FIFO
private:
  struct Cvor {
    Tip el;
    Cvor *sljedeci;
  };
  int brel;
  Cvor *pocetak;
  Cvor *kraj;

public:
  Red() {
    brel = 0;
    pocetak = nullptr; // izbacuje se odavde
    kraj = nullptr;    // ovdje se ubacuje
  }

  ~Red() {
      while (pocetak != nullptr) {
      auto temp = pocetak;
      pocetak = pocetak->sljedeci;
      temp->sljedeci=nullptr;
      delete temp;
        }
        //kraj=nullptr;
        brel=0;
  }

  Red(const Red &r) {
      brel=0;
      pocetak=nullptr;
      if (r.brel==0) {pocetak=nullptr; kraj=nullptr;}
      else
      {
          auto it=r.pocetak;
          while (it!=nullptr)
          {
              stavi(it->el);
              it=it->sljedeci;
          }
      }
  }

  Red &operator=(const Red &r) {
      if (this==&r) return *this;
      while (pocetak!=kraj)
      {
          auto trenutni=pocetak;
          pocetak=pocetak->sljedeci;
          delete trenutni;
      }
      brel=0;
      if (r.brel==0) pocetak=nullptr;
      else
      {
          auto it=r.pocetak;
          while (it!=nullptr)
          {
              stavi(it->el);
              it=it->sljedeci;
          }
          return *this;
      }
  }

  void brisi() {
    while (pocetak != nullptr) {
      auto it = pocetak;
      pocetak = pocetak->sljedeci;
      it->sljedeci = nullptr;
      delete it;
    }
    kraj = nullptr;
    brel = 0;
  }

  void stavi(const Tip &el) {
    Cvor *novi = new Cvor{el, nullptr};
    if (brel == 0) {
      pocetak = novi;
      kraj = novi;
      brel++;
      return;
    }

    kraj->sljedeci = novi;
    kraj = novi;
    brel++;
  }

  Tip skini() {
    if (brel == 0)
      throw std::logic_error("Nemam sta da vratim!");
    Tip podatak = pocetak->el;
    auto it = pocetak;
    pocetak = pocetak->sljedeci;
    it->sljedeci = nullptr;
    delete it;
    brel--;
    return podatak;
  }

  Tip &celo() {
    if (brel == 0)
      throw std::logic_error("Nemam sta da vratim!");
    return pocetak->el;
  }

  int brojElemenata() const { return brel; }
};

int main() {
  Red<int> r;
    r.stavi(2);
    r.stavi(4);
    r.stavi(9);
    r.stavi(7);
    std::cout<<"\nElement na vrhu je: "<<r.celo();
    std::cout<<"\nBroj elemenata je: "<<r.brojElemenata();
    std::cout<<"\nElement skinut sa vrha je: "<<r.skini();
    std::cout<<"\nBroj elemenata je: "<<r.brojElemenata();
    std::cout<<std::endl;
    int brel=r.brojElemenata();
    for (int i=0;i<brel;i++) std::cout<<r.skini()<<" ";
    r.brisi();
    std::cout<<"\nBroj elemenata je: "<<r.brojElemenata();
}
