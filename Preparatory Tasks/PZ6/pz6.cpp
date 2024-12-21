#include <iostream>
#include <string>
#include <vector>
using namespace std;

template <typename TipKljuca, typename TipVrijednosti> class Mapa {
private:
public:
  Mapa(){};
  virtual ~Mapa(){};
  virtual int brojElemenata() const = 0;
  virtual void obrisi() = 0;                       // brise sve el iz mape
  virtual void obrisi(const TipKljuca &kljuc) = 0; // brise jedan el, znaci osloboditi tu memoriju
  virtual TipVrijednosti &operator[](TipKljuca k) = 0;
  virtual TipVrijednosti operator[](TipKljuca k) const = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca, TipVrijednosti> {
private:
  std::pair<TipKljuca, TipVrijednosti> **niz;
  int kapacitet;
  int brel;

public:
  NizMapa() {
    niz = new std::pair<TipKljuca, TipVrijednosti> *[10000] {};
    kapacitet = 10000;
    brel = 0;
  }

  ~NizMapa() {
    for (int i = 0; i < brel; i++) delete niz[i];
    delete[] niz;
  }

  NizMapa(const NizMapa &n) { // kopirajuci
    kapacitet = n.kapacitet;
    brel = n.brel;
    if (kapacitet == 0) niz = nullptr;
    else
    {
      niz = new std::pair<TipKljuca, TipVrijednosti> *[kapacitet] {};
      for (int i = 0; i < brel; i++) niz[i] = new std::pair<TipKljuca, TipVrijednosti>(*n.niz[i]);
    }
  }

  NizMapa &operator=(const NizMapa &n) { // operator dodjele (DESTR+KONSTR)
    if (this == &n) return *this;
    for (int i = 0; i < brel; i++) delete niz[i];
    delete[] niz;
    kapacitet = n.kapacitet;
    brel = n.brel;
    if (kapacitet == 0) niz = nullptr;
    else
    {
      niz = new std::pair<TipKljuca, TipVrijednosti> *[kapacitet] {};
      for (int i = 0; i < brel; i++) niz[i] = new std::pair<TipKljuca, TipVrijednosti>(*n.niz[i]);
    }
    return *this;
  }

  int brojElemenata() const { return brel; }

  void obrisi() {
    for (int i = 0; i < brel; i++) delete niz[i];
    brel = 0;
  }

  void obrisi(const TipKljuca &k) {
    for (int i = 0; i < brel; i++)
    {
      if (niz[i]->first == k)
      {
        delete niz[i];
        niz[i] = nullptr;
        for (int j = i; j < brel - 1; j++)
        {
          niz[j] = niz[j + 1];
        }
        niz[brel - 1] = nullptr;
        brel--;
        return;
      }
    }
    throw std::logic_error("Proslijedjeni kljuc nije pronadjen u mapi!");
  }

  TipVrijednosti &operator[](TipKljuca k) {
    for (int i = 0; i < brel; i++)
    {
      if (niz[i]->first == k) return niz[i]->second;
    }
    niz[brel] = new std::pair<TipKljuca, TipVrijednosti>(k, TipVrijednosti{});
    brel++;
    return niz[brel - 1]->second;
  }

  TipVrijednosti operator[](TipKljuca k) const {
    for (int i = 0; i < brel; i++)
    {
      if (niz[i]->first == k) return niz[i]->second;
    }
    return TipVrijednosti{}; // ako ne nadje vrati default
  }
};

int main() {
  NizMapa<int, std::string> mapa;
  mapa[0] = "BiH";
  mapa[1] = "Srbija";
  mapa[2] = "Hrvatska";
  mapa[3] = "Crna Gora";
  mapa[4] = "Slovenija";
  mapa[5] = "Makedonija";
  std::cout << "Mapa ima " << mapa.brojElemenata() << " elemenata, a treci je: " << mapa[2]; //Hrvatska
  mapa[2]="SSSR";
  std::cout<< "\nPromijenjena drzava na trecoj poziciji je: "<< mapa[2];

  std::cout << "\n" << mapa[5];
  std::cout << "\n" << mapa[6];
  std::cout << "\n" << mapa[7]; //prazno mjesto jer je default vrijednost stringa prazan string
  mapa.obrisi(5);
  std::cout <<"\n"<< mapa.brojElemenata();

  mapa.obrisi();
  std::cout <<"\n"<< mapa.brojElemenata();
  return 0;
}
