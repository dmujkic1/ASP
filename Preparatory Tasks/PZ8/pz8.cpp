#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>

const int vel = 1000;
using namespace std;

template <typename kljuc, typename vrijednost> class Mapa {
public:
  Mapa(){};
  virtual ~Mapa(){};
  virtual vrijednost operator[](const kljuc &indeks) const = 0;
  virtual vrijednost &operator[](const kljuc &indeks) = 0;
  virtual int brojElemenata() const = 0;
  virtual void obrisi() = 0;
  virtual void obrisi(const kljuc &Kljuc) = 0;
};

template <typename kljuc, typename vrijednost>
class NizMapa : public Mapa<kljuc, vrijednost> {
  std::pair<kljuc, vrijednost> **niz;
  int velicina, kapacitet;

public:
  NizMapa<kljuc, vrijednost>() {
    velicina = 0;
    kapacitet = vel;
    niz = new std::pair<kljuc, vrijednost> *[vel];
  }
  NizMapa<kljuc, vrijednost>(const NizMapa<kljuc, vrijednost> &nova) {
    velicina = nova.velicina;
    kapacitet = velicina;
    niz = new std::pair<kljuc, vrijednost> *[vel];

    for (int i = 0; i < velicina; i++)
      this->niz[i] = new std::pair<kljuc, vrijednost>(nova.niz[i]->first, nova.niz[i]->second);
  }
  ~NizMapa() override {
    for (int i = 0; i < velicina; i++) delete niz[i];
    delete[] niz;
  }
  // operatori
  NizMapa<kljuc, vrijednost> operator=(const NizMapa<kljuc, vrijednost> &nova) {
    if (this == &nova) return *this;
    unisti();

    velicina = nova.velicina;
    kapacitet = velicina;
    niz = new std::pair<kljuc, vrijednost> *[vel];

    for (int i = 0; i < velicina; i++)
      this->niz[i] = new std::pair<kljuc, vrijednost>(nova.niz[i]->first, nova.niz[i]->second);
    return *this;
  }
  vrijednost operator[](const kljuc &indeks) const override {
    for (int i = 0; i < velicina; i++) {
      if (niz[i]->first == indeks) return niz[i]->second;
    }
    return vrijednost();
  }
  vrijednost &operator[](const kljuc &indeks) override {
    for (int i = 0; i < velicina; i++) {
      if (niz[i]->first == indeks) return niz[i]->second;
    }
    if (velicina == kapacitet) {
      std::pair<kljuc, vrijednost> **pom;
      pom = new std::pair<kljuc, vrijednost> *[2 * vel];
      for (int i = 0; i < velicina; i++) {
        pom[i] = new std::pair<kljuc, vrijednost>(niz[i]->first, niz[i]->second);
      }
      unisti();
      niz = pom;
      kapacitet = 2 * vel;
    }
    niz[velicina] = new std::pair<kljuc, vrijednost>(indeks, vrijednost());
    return niz[velicina++]->second;
  }

  void unisti() {
    for (int i = 0; i < velicina; i++) delete niz[i];
    delete[] niz;
  }
  void obrisi() override {
    for (int i = 0; i < velicina; i++) delete niz[i];
    velicina = 0;
  }
  void obrisi(const kljuc &Kljuc) override {
    for (int i = 0; i < velicina; i++) {
      if (niz[i]->first == Kljuc) {
        delete niz[i];
        if (i != velicina - 1) {
          niz[i] = niz[velicina - 1];
          niz[velicina - 1] = nullptr;
        }
        velicina--;
        return;
      }
    }
    throw std::logic_error("Nema elementa");
  }

  int brojElemenata() const override { return velicina; }
};

template <typename kljuc, typename vrijednost>
class BinStabloMapa : public Mapa<kljuc, vrijednost> {
  struct cvor {
    kljuc key;
    vrijednost val;
    cvor *roditelj, *lijevo, *desno;
    cvor(const kljuc &k, const vrijednost &v, cvor *c) {
      key = k;
      val = v;
      lijevo = nullptr;
      desno = nullptr;
      roditelj = c;
    }
  };
  int velicina;
  cvor *korijen;

  cvor *trazi(cvor *c, const kljuc &k) const {
    if (c == nullptr || c->key == k) return c;
    if (k < c->key) return trazi(c->lijevo, k);
    return trazi(c->desno, k);
  }
  cvor *dodaj(cvor *&c, const kljuc &k, cvor *roditelj) {
    if (c == nullptr) {
      c = new cvor(k, vrijednost(), roditelj);
      velicina++;
      return c;
    } else if (k < c->key) return dodaj(c->lijevo, k, c);
    else return dodaj(c->desno, k, c);
  }

public:
  BinStabloMapa<kljuc, vrijednost>() : Mapa<kljuc, vrijednost>() {
    velicina = 0;
    korijen = nullptr;
  }
  BinStabloMapa<kljuc, vrijednost>(const BinStabloMapa<kljuc, vrijednost> &nova) {
    velicina = 0;
    korijen = nullptr;
    kopirajStablo(korijen, nova.korijen, nullptr);
  }
  void kopirajStablo(cvor *&c1, cvor *c2, cvor *roditelj) {
    if (c2 == nullptr) return;
    c1 = new cvor(c2->key, c2->val, roditelj);
    kopirajStablo(c1->lijevo, c2->lijevo, c1);
    kopirajStablo(c1->desno, c2->desno, c1);
    velicina++;
  }
  ~BinStabloMapa() override { obrisiStablo(korijen); }

  // operatori
  BinStabloMapa<kljuc, vrijednost> &
  operator=(const BinStabloMapa<kljuc, vrijednost> &nova) {
    if (this == &nova) return *this;
    obrisiStablo(korijen);
    velicina = 0;
    korijen = nullptr;
    kopirajStablo(korijen, nova.korijen, nullptr);
    return *this;
  }
  vrijednost operator[](const kljuc &indeks) const override {
    cvor *c = trazi(korijen, indeks);
    if (c != nullptr) return c->val;
    return vrijednost();
  }

  vrijednost &operator[](const kljuc &indeks) override {
    cvor *c = trazi(korijen, indeks);
    if (c != nullptr) return c->val;
    c = dodaj(korijen, indeks, nullptr);
    return c->val;
  }

  void obrisi() override {
    obrisiStablo(korijen);
    korijen = nullptr;
    velicina = 0;
  }
  void obrisi (const kljuc &Kljuc) override {
    cvor *del = trazi(korijen, Kljuc);
    if (del == nullptr) throw std::logic_error("Kljuc ne postoji");
    if (del->lijevo == nullptr && del->desno == nullptr) {
      if (del == korijen) korijen = nullptr;
      else if (del == del->roditelj->lijevo) del->roditelj->lijevo = nullptr;
      else del->roditelj->desno = nullptr;
      delete del;

    } else if (del->lijevo != nullptr && del->desno != nullptr) {
      cvor *pom = del->lijevo;
      while (pom->desno != nullptr) pom = pom->desno;
        pom->desno=del->desno;
      if (del->desno != nullptr) del->desno->roditelj = pom;
      pom->roditelj->desno = pom->lijevo;
      if (pom->lijevo != nullptr) pom->lijevo->roditelj = pom->roditelj;
      if (del == korijen) korijen = pom;
      else if (del == del->roditelj->lijevo) {
        del->roditelj->lijevo = pom;
        pom->roditelj = del->roditelj;
      } else {
        del->roditelj->desno = pom;
        pom->roditelj = del->roditelj;
      }
      if (del == korijen) pom->lijevo = nullptr;
      else {
        pom->lijevo = del->lijevo;
        del->lijevo->roditelj = pom;
      }
      delete del;
    } else {
      cvor *dijete = del->lijevo;
      if (dijete == nullptr) dijete = del->desno;
      if (del == korijen) korijen = dijete;
      else if (del == del->roditelj->lijevo) del->roditelj->lijevo = dijete;
      else del->roditelj->desno = dijete;
      dijete->roditelj = del->roditelj;
      delete del;
    }
    velicina--;
  }
  void obrisiStablo(cvor *c) {
    if (c == nullptr) return;
    obrisiStablo(c->lijevo);
    obrisiStablo(c->desno);
    delete c;
  }

  int brojElemenata() const override { return velicina; }
};

int main() {
    const int brojElemenata = 10000;
    clock_t start, end;

    // Test NizMapa
    NizMapa<int, int> nizMapa;

    start = clock();
    for (int i = 0; i < brojElemenata; i++) nizMapa[i] = i;
    end = clock();
    std::cout << "NizMapa dodavanje trajalo: " << 1000.0 * (end - start) / CLOCKS_PER_SEC << " ms\n";

    start = clock();
    for (int i = 0; i < brojElemenata; i++) int value = nizMapa[i];
    end = clock();
    std::cout << "NizMapa pristup trajalo: " << 1000.0 * (end - start) / CLOCKS_PER_SEC << " ms\n";

    // Test BinStabloMapa
    BinStabloMapa<int, int> binStabloMapa;

    start = clock();
    for (int i = 0; i < brojElemenata; i++) binStabloMapa[i] = i;
    end = clock();
    std::cout << "BinStabloMapa dodavanje trajalo: " << 1000.0 * (end - start) / CLOCKS_PER_SEC << " ms\n";

    start = clock();
    for (int i = 0; i < brojElemenata; i++) int value = binStabloMapa[i];
    end = clock();
    std::cout << "BinStabloMapa pristup trajalo: " << 1000.0 * (end - start) / CLOCKS_PER_SEC << " ms\n";

    return 0;
}