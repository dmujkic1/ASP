#include <iostream>
using namespace std;

template <typename Tip> class DvostraniRed { // red je FIFO
private:    //DvostraniRed ima osobine bibliotecne klase "deque"
  struct Cvor { //Sve O(1) osim BRISI
    Tip el;
    Cvor *sljedeci;
    Cvor *prethodni;
  };
  int brel;
  Cvor *pocetak;
  Cvor *kraj;

public:
  DvostraniRed() {
    brel = 0;
    pocetak = nullptr; // izbacuje se odavde, ČELO
    kraj = nullptr;    // ovdje se ubacuje, VRH
  }

  ~DvostraniRed() {
      while (pocetak != nullptr) {
      auto temp = pocetak;
      pocetak = pocetak->sljedeci;
      temp->sljedeci=nullptr;
      delete temp;
        }
        brel=0;
  }

  DvostraniRed(const DvostraniRed &DR){ //kopirajuci konstruktor, element po element alociraj
        brel=0;
        pocetak=nullptr;
        kraj=nullptr;
        Cvor *temp=nullptr;
        bool prvi=true;
        while(DR.pocetak!=nullptr)
        {
            Cvor *novi=new Cvor{DR.pocetak->el,nullptr,nullptr};
            if(temp!=nullptr)
            {
                temp->sljedeci=novi;
                novi->prethodni=temp;
            }
            if(prvi) pocetak=novi;
            temp=novi;
            prvi=false;
            DR.pocetak=DR.pocetak->sljedeci;
        }
        kraj=temp;
        brel=DR.brel;
  }

  DvostraniRed &operator=(const DvostraniRed &DR){ //operator dodjele (DESTR+KONSTR)
  if (this==&DR) return *this; //zastita od samododjele
        while (pocetak != nullptr)
        {
            auto temp = pocetak;
            pocetak = pocetak->sljedeci;
            temp->sljedeci=nullptr;
            delete temp;
        }
        brel=0;
//OBRISAN THIS
        brel=0;
        pocetak=nullptr;
        kraj=nullptr;
        Cvor *temp=nullptr;
        bool prvi=true;
        while(DR.pocetak!=nullptr)
        {
            Cvor *novi=new Cvor{DR.pocetak->el,nullptr,nullptr};
            if(temp!=nullptr)
            {
                temp->sljedeci=novi;
                novi->prethodni=temp;
            }
            if(prvi) pocetak=novi;
            temp=novi;
            prvi=false;
            DR.pocetak=DR.pocetak->sljedeci;
        }
        kraj=temp;
        brel=DR.brel;
        return *this;
  }

  void brisi(){
        while (pocetak != nullptr) {
            auto temp = pocetak;
            pocetak = pocetak->sljedeci;
            temp->sljedeci=nullptr;
            delete temp;
        }
        brel=0;
  }

  int brojElemenata() {return brel;}

  void staviNaVrh(const Tip &el){
      if (brel==0)
      {
          Cvor *novi = new Cvor{el, nullptr, nullptr};
          pocetak=novi;
          kraj=novi;
          brel++;
          return;
      }
      Cvor *novi = new Cvor{el, nullptr, kraj};
      kraj->sljedeci=novi;
      kraj=novi;
      brel++;
  }

  Tip skiniSaVrha(){
      if (brel == 0) throw std::logic_error("Nemam sta da vratim!");
      auto prijekraja=kraj->prethodni;
      Tip podatak=kraj->el;
      kraj->prethodni=nullptr;
      delete kraj;
      brel--;
      prijekraja->sljedeci=nullptr;
      kraj=prijekraja;
      return podatak;
  }

  void staviNaCelo(const Tip &el){
    if (brel==0)
      {
          Cvor *novi = new Cvor{el, nullptr, nullptr};
          pocetak=novi;
          kraj=novi;
          brel++;
          return;
      }
      Cvor *novi = new Cvor{el, pocetak, nullptr};
      pocetak->prethodni=novi;
      pocetak=novi;
      brel++;
  }

  Tip skiniSaCela(){
      if (brel == 0) throw std::logic_error("Nemam sta da vratim!");
      auto poslijepocetka=pocetak->sljedeci;
      Tip podatak=pocetak->el;
      pocetak->sljedeci=nullptr;
      delete pocetak;
      brel--;
      poslijepocetka->prethodni=nullptr;
      pocetak=poslijepocetka;
      return podatak;
  }

  Tip &vrh(){
    if (brel == 0) throw std::logic_error("Nemam sta da vratim!");
    return kraj->el;
  }

  Tip &celo(){
    if (brel == 0) throw std::logic_error("Nemam sta da vratim!");
    return pocetak->el;
  }
};

int main(){
    DvostraniRed<int> r;
    r.staviNaVrh(2); //2
    r.staviNaVrh(4); //2 4
    r.staviNaCelo(9); //9 2 4
    r.staviNaCelo(7); //7 9 2 4
    std::cout<<"\nElement na celu je: "<<r.celo();
    std::cout<<"\nElement na vrhu je: "<<r.vrh();
    std::cout<<"\nBroj elemenata je: "<<r.brojElemenata();
    std::cout<<"\nElement skinut sa vrha je: "<<r.skiniSaVrha();
    std::cout<<"\nElement skinut sa cela je: "<<r.skiniSaCela();
    std::cout<<"\nBroj elemenata je: "<<r.brojElemenata();
    std::cout<<std::endl;
    int brel=r.brojElemenata();
    for (int i=0;i<brel;i++) std::cout<<r.skiniSaCela()<<" ";
    r.brisi();
    std::cout<<"\nBroj elemenata je: "<<r.brojElemenata();
    return 0;
}