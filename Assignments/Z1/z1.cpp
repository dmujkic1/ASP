#include <iostream>
using namespace std;

template <typename Tip> class Lista {
private:
public:
  Lista(){};
  virtual ~Lista() {}
  virtual int brojElemenata() const = 0;
  virtual Tip trenutni() const = 0;
  virtual Tip &trenutni() = 0;
  virtual bool prethodni() = 0;
  virtual bool sljedeci() = 0;
  virtual void pocetak() = 0;
  virtual void kraj() = 0;
  virtual void obrisi() = 0;
  virtual void dodajIspred(const Tip &el) = 0;
  virtual void dodajIza(const Tip &el) = 0;
  virtual Tip &operator[](int indeks) = 0;
  virtual Tip operator[](int indeks) const = 0;
};

template <typename Tip> class DvostrukaLista : public Lista<Tip> {
private:
  struct Cvor {
    Tip el;
    Cvor *sljedeci;
    Cvor *prethodni;
  };
  Cvor *prvi;
  Cvor *posljednji;
  Cvor *trenutni_cvor;
  int brel;

public:
  template <typename Tip1>
  friend class Iterator;

  ~DvostrukaLista() {
    while (prvi != nullptr) {
      auto temp = prvi;
      prvi = prvi->sljedeci;
      temp->sljedeci = nullptr;
      delete temp;
    }
    posljednji=nullptr;
    trenutni_cvor = nullptr;
    brel = 0;
  }

  DvostrukaLista() {
    prvi = nullptr;
    posljednji=nullptr;
    trenutni_cvor = nullptr;
    brel = 0;
  }

  DvostrukaLista(DvostrukaLista &&L) // pomjerajuci
  {
    // privremeni objekat L
    posljednji=L.posljednji;
    prvi = L.prvi;
    trenutni_cvor = L.trenutni_cvor;
    brel = L.brel;
  }
  DvostrukaLista(const DvostrukaLista &L) // kopirajuci
  {
    prvi=nullptr;
    trenutni_cvor=nullptr;
    posljednji=nullptr;
    brel=0;

    if (L.brel == 0) {
      prvi = nullptr;
      posljednji=nullptr;
      trenutni_cvor = nullptr;
      brel = 0;
    }

    else {
      auto it = L.prvi;
      while (it != nullptr) {
        dodajIza(it->el);
        sljedeci();
        it = it->sljedeci;
      }
      brel=L.brel;
    }
  }

  DvostrukaLista &operator=(const DvostrukaLista &L) // kopirajuci
  {
    if (this == &L)
      return *this;

    while (prvi != nullptr) {
      trenutni_cvor = prvi;
      prvi = prvi->sljedeci;
      delete trenutni_cvor;
    }
    brel = 0;
    if (L.brel == 0) {
      prvi = nullptr;
      posljednji=nullptr;
      trenutni_cvor = nullptr;
      brel = 0;
    } else {
      auto it = L.prvi;
      while (it != nullptr) {
        dodajIza(it->el);
        sljedeci();
        it = it->sljedeci;
      }
    }
    return *this;
  }
  DvostrukaLista &operator=(DvostrukaLista &&L) // pomjerajuci
  {
    if (this == &L)
      return *this;
    std::swap(prvi, L.prvi);
    std::swap(posljednji, L.posljednji);
    std::swap(trenutni_cvor, L.trenutni_cvor);
    std::swap(brel, L.brel);
    return *this;
  }

  int brojElemenata() const override { return brel; }

  Tip &trenutni() override {
    if (brel == 0)
      throw std::range_error("Nemam šta da ti dam!\n");
    return trenutni_cvor->el;
  }

  Tip trenutni() const override {
    if (brel == 0)
      throw std::range_error("Nemam šta da ti dam!\n");
    return trenutni_cvor->el;
  }

  bool prethodni() override {
    if (brel == 0)
      throw std::range_error("Nemam šta da ti dam!\n");
    if (trenutni_cvor->prethodni==nullptr) return false;
    trenutni_cvor=trenutni_cvor->prethodni;
    return true;
  }

  bool sljedeci() override {
    if (brel == 0)
      throw std::range_error("Nemam šta da ti dam!\n");
    if (trenutni_cvor->sljedeci == nullptr)
      return false;
    trenutni_cvor = trenutni_cvor->sljedeci;
    return true;
  }

  void pocetak() override {
    if (brel == 0)
      throw std::range_error("Nemam šta da ti dam!\n");
    trenutni_cvor = prvi;
  }

  void kraj() override {
    if (brel == 0)
      throw std::range_error("Nemam šta da ti dam!\n");
    trenutni_cvor=posljednji;
  }

  void obrisi() override { //briše iz liste trenutni element
    //nakon brisanja sljedeći element u listi postaje trenutni
    //ako je obrisan posljednji element u listi, element prije njega je trenutni
    if (brel == 0) throw std::range_error("Nemam šta da ti dam!\n");

    else if (brel == 1) {
      delete prvi;
      prvi = nullptr;
      trenutni_cvor = nullptr;
      brel--;
      return;
    }

    else if (brel>1){
        if (trenutni_cvor==posljednji) //zadnji brisem
        {
            trenutni_cvor=trenutni_cvor->prethodni;
            posljednji->prethodni=nullptr;
            trenutni_cvor->sljedeci=nullptr;
            delete posljednji;
            brel--;
            posljednji=trenutni_cvor;
        }
        else if (trenutni_cvor==prvi) //prvi brisem
        {
            trenutni_cvor=trenutni_cvor->sljedeci;
            prvi->sljedeci=nullptr;
            trenutni_cvor->prethodni=nullptr;
            delete prvi;
            brel--;
            prvi=trenutni_cvor;
        }
        else //brisem u sredini negdje
        {
            auto it=trenutni_cvor;
            auto prije_trenutnog=trenutni_cvor->prethodni; 
            auto poslije_trenutnog=trenutni_cvor->sljedeci;
            trenutni_cvor=trenutni_cvor->sljedeci;
            prije_trenutnog->sljedeci=poslije_trenutnog;
            poslije_trenutnog->prethodni=prije_trenutnog;
            poslije_trenutnog=nullptr;
            prije_trenutnog=nullptr;
            it->sljedeci=nullptr;
            it->prethodni=nullptr;
            delete it;
            brel--;
        }
    }
  }

  void dodajIspred(const Tip &el) override {
    if (brel ==
        0) // dodavanje u prazan
    {
      Cvor *novi = new Cvor{el, nullptr, nullptr};
      trenutni_cvor = novi;
      prvi = novi;
      posljednji=novi;
      brel++;
      // novi=nullptr;
    }
    // dodavanje prije prvog:
    else if (trenutni_cvor == prvi) // NE MOZE "IF" JER VISE JE POSLIJE PRVOG
                                    // DODAVANJA TRENUTNI NA PRVOM
    {
      Cvor *novi = new Cvor{el, trenutni_cvor, nullptr};
      trenutni_cvor->prethodni=novi;
      prvi = novi;
      brel++;
    }
    // dodavanje u sredini:
    else {
      auto prije_trenutnog = trenutni_cvor->prethodni;
      Cvor *novi = new Cvor{el, trenutni_cvor, prije_trenutnog};
      prije_trenutnog->sljedeci = novi;
      trenutni_cvor->prethodni=novi;
      brel++;
    }
  }

  void dodajIza(const Tip &el) override {
    if (brel ==
        0) // dodavanje u prazan
    {
      Cvor *novi = new Cvor{el, nullptr, nullptr};
      trenutni_cvor = novi;
      prvi = novi;
      posljednji=novi;
      brel++;
      // novi=nullptr;
    }
    // dodavanje poslije zadnjeg:
    else if (trenutni_cvor->sljedeci ==
             nullptr) // NE MOZE "IF" JER VISE JE POSLIJE PRVOG DODAVANJA
                      // TRENUTNI NA PRVOM
    {
      Cvor *novi = new Cvor{el, nullptr, trenutni_cvor};
      trenutni_cvor->sljedeci = novi;
      posljednji=novi;
      brel++;
    }
    // dodavanje u sredini:
    else {
      auto poslije_trenutnog = trenutni_cvor->sljedeci;
      Cvor *novi = new Cvor{el, poslije_trenutnog, trenutni_cvor};
      trenutni_cvor->sljedeci = novi;
      poslije_trenutnog->prethodni=novi;
      brel++;
    }
  }

  Tip &operator[](int indeks) override {
    if (indeks < 0 || indeks >= brel)
      throw std::range_error("Premasen indeks!");

     if (indeks<=brel/2)
     {
         auto it = prvi;
        for (int i = 0; i < indeks; i++) it = it->sljedeci;
        return it->el;
     }
     else
     {
         auto it=posljednji;
         for (int i = brel-1; i > indeks; i--) it = it->prethodni;
        return it->el;
     }

  }

  Tip operator[](int indeks) const override {
    if (indeks < 0 || indeks >= brel)
      throw std::range_error("Premasen indeks!");

     if (indeks<=brel/2)
     {
         auto it = prvi;
        for (int i = 0; i < indeks; i++) it = it->sljedeci;
        return it->el;
     }
     else
     {
         auto it=posljednji;
         for (int i = brel-1; i > indeks; i--) it = it->prethodni;
        return it->el;
     }
  }
};

template<typename Tip1>
class Iterator{
    private:
        typename DvostrukaLista<Tip1>::Cvor *trenutni;
        const DvostrukaLista<Tip1> *lista;
    public:
        Iterator(const DvostrukaLista<Tip1> &l){
        lista=&l;
        trenutni=l.prvi;
    }
    Tip1 dajTrenutni(){
        if(lista->brojElemenata()==0) throw std::domain_error("Nemam šta da ti dam!\n");
        return trenutni->el;
    }
    bool sljedeci(){
        if(lista->brojElemenata()==0) throw std::domain_error("Nemam šta da ti dam!\n");
        if(trenutni->sljedeci==nullptr) return false;
        trenutni=trenutni->sljedeci;
        return true;
    }

};

template<typename Tip>
Tip dajMaksimum(const Lista<Tip>& n){
    if (n.brojElemenata()==0) std::range_error("Nemam šta da ti dam!\n");
    Iterator<Tip> it=dynamic_cast<const DvostrukaLista<Tip>&>(n); //lista sa trenutnim na prvom
    Tip najveci=it.dajTrenutni();
    while (it.sljedeci())
    {
        if (it.dajTrenutni()>najveci) najveci=it.dajTrenutni();
    }
    return najveci;
}

int main(){
  DvostrukaLista<int> DvostrukaLista;
  DvostrukaLista.dodajIspred(1);
  DvostrukaLista.dodajIza(3);
  DvostrukaLista.kraj();
  DvostrukaLista.dodajIza(5);
  DvostrukaLista.obrisi();
  int brel=DvostrukaLista.brojElemenata();
  std::cout<<"Elementi u Dvostrukoj Listi su: ";
  for (int i=0;i<brel;i++) std::cout<<DvostrukaLista[i]<<" ";
  std::cout<<"\nTrenutni element je: "<<DvostrukaLista.trenutni()<<" a prethodni je: "<<DvostrukaLista.prethodni();
  std::cout<<"\nNajveci clan u listi je: "<<dajMaksimum(DvostrukaLista);  
    return 0;
}