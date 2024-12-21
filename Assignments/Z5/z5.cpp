#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <utility>
using namespace std;


template <typename TipKljuca, typename TipVrijednosti> class Mapa {
public:
  Mapa(){};
  virtual ~Mapa(){};
  virtual TipVrijednosti &operator[](TipKljuca k) = 0;
  virtual TipVrijednosti operator[](TipKljuca k) const = 0;
  virtual int brojElemenata() const = 0;
  virtual void obrisi() = 0;
  virtual void obrisi(const TipKljuca &k) = 0;
};
template <typename TipKljuca, typename TipVrijednosti>
class HashMapaLan : public Mapa<TipKljuca, TipVrijednosti> {
  int broj_elemenata;
  std::vector<std::list<std::pair<TipKljuca, TipVrijednosti>> *> lan;
  unsigned int (*p)(TipKljuca, unsigned int)=nullptr;

public:
  void definisiHashFunkciju(unsigned int (*p1)(TipKljuca, unsigned)) { p = p1; }
  ~HashMapaLan() {
    for (int i = 0; i < lan.size(); i++) {
      delete lan[i];
    }
  }
  HashMapaLan() : lan(100) {
    broj_elemenata = 0;
  }
  HashMapaLan(const HashMapaLan<TipKljuca, TipVrijednosti> &mp)
      : lan(mp.lan.size()) {
    broj_elemenata = mp.broj_elemenata;
    p = mp.p;
    for (int i = 0; i < lan.size(); i++) {
      if (mp.lan[i]) {
        lan[i] =
            new std::list<std::pair<TipKljuca, TipVrijednosti>>(*mp.lan[i]);
      }
    }
  }
  HashMapaLan &operator=(const HashMapaLan<TipKljuca, TipVrijednosti> &mp) {
    if (&mp == this)
      return *this;
    for (auto it = lan.begin();it!=lan.end();it++) {
      delete (*it);
       }
      lan.clear();
      lan.resize(1000);
      broj_elemenata = 0;
    lan.resize(mp.lan.size(), nullptr);
    p = mp.p;
    broj_elemenata = mp.broj_elemenata;
    for (int i = 0; i < lan.size(); i++) {
      if (mp.lan[i]) {
        lan[i] =
            new std::list<std::pair<TipKljuca, TipVrijednosti>>(*mp.lan[i]);
      }
    }
    return *this;
  }
  int brojElemenata() const { return broj_elemenata; }
  TipVrijednosti operator[](TipKljuca k) const {
    if (p == nullptr)
      throw std::logic_error("Greska");
    int vel = lan.size();
    unsigned int hash = p(k, vel);
    if (lan[hash]) {
      for (auto it = lan[hash]->begin(); it != lan[hash]->end();
           it++) {
        if (it->first == k)
          return it->second;
      }
    }
    return TipVrijednosti{};
  }
  TipVrijednosti &operator[](TipKljuca k) {
    if (p == nullptr)
      throw std::logic_error("Greska");
    int vel = lan.size();
    unsigned int hash = p(k, vel);
    if (lan[hash] != nullptr) {
      auto iter = lan[hash]->begin();
      for (auto it = lan[hash]->begin(); it != lan[hash]->end();
           it++) {
        if (it->first == k)
          return it->second;
      }
      while (iter != lan[hash]->end() && iter->first < k)
        iter++;
      broj_elemenata++;
      return (lan[hash]->insert(iter, std::pair<TipKljuca, TipVrijednosti>(
                                          k, TipVrijednosti{})))
          ->second;
    } else {
      lan[hash] = new std::list<std::pair<TipKljuca, TipVrijednosti>>{
          std::pair<TipKljuca, TipVrijednosti>(k, TipVrijednosti{})};
      broj_elemenata++;
      auto it = lan[hash]->begin();
      return it->second;
    }
  }
  
  void obrisi(const TipKljuca &k) {
    if (p == nullptr)
      throw std::logic_error("Greska");
    int vel = lan.size();
    unsigned int hash = p(k, vel);
    if (lan[hash]) {
      auto it = lan[hash]->begin();
      for (; it != lan[hash]->end(); it++) {
        if (it->first == k) {
          lan[hash]->erase(it);
          broj_elemenata--;
          return;
        }
      }
    if (lan[hash]->empty()) {
        delete lan[hash];
    }    
    }
    throw std::logic_error("Greska");
  }
  void obrisi() {
    for (auto it = lan.begin();it!=lan.end();it++) {
      delete (*it);
       }
      lan.clear();
      lan.resize(1000);
      broj_elemenata = 0;
   
  }
};
template <typename TipOznake> class Grana;
template <typename TipOznake> class Cvor;
template <typename TipOznake> class GranaIterator;
template <typename TipOznake> class ListaGraf;

template <typename TipOznake> class UsmjereniGraf {
public:
  virtual ~UsmjereniGraf(){};
  UsmjereniGraf() = default;
  virtual int dajBrojCvorova() = 0;
  virtual void postaviBrojCvorova(int bc) = 0;
  virtual void dodajGranu(int P, int S, float tez) = 0;
  virtual void obrisiGranu(int P, int S) = 0;
  virtual void postaviTezinuGrane(int P, int S, float tez) = 0;
  virtual float dajTezinuGrane(int P, int S) = 0;
  virtual bool postojiGrana(int P, int S) = 0;
  virtual void postaviOznakuCvora(int i, TipOznake oznaka) = 0;
  virtual TipOznake dajOznakuCvora(int c) = 0;
  virtual void postaviOznakuGrane(int P, int S, TipOznake ozn) = 0;
  virtual TipOznake dajOznakuGrane(int P, int S) = 0;
  virtual Grana<TipOznake> dajGranu(int P, int S) = 0;
  virtual Cvor<TipOznake> dajCvor(int br) = 0;
  virtual GranaIterator<TipOznake> dajGranePocetak() = 0;
  virtual GranaIterator<TipOznake> dajGraneKraj() = 0;
};
template <typename TipOznake> class Cvor {
  TipOznake oznaka;
  int r;
  UsmjereniGraf<TipOznake> *pok;

public:
  Cvor(TipOznake o, int b, UsmjereniGraf<TipOznake> *p)
      : oznaka(o), r(b), pok(p){};
  Cvor() : oznaka(TipOznake{}) {
    r = 0;
    pok = nullptr;
  }
  TipOznake dajOznaku() { return oznaka; }
  void postaviOznaku(TipOznake o) { pok->postaviOznakuCvora(r, o); }
  void postaviOznakuPom(TipOznake o) { oznaka = o; }
  int dajRedniBroj() { return r; }
  friend class Grana<TipOznake>;
};
template <typename TipOznake> class Grana {
  float tez;
  TipOznake ozn;
  Cvor<TipOznake> *P;
  Cvor<TipOznake> *S;

public:
  Grana() : ozn(TipOznake{}), P(nullptr), S(nullptr), tez(0) {}
  Grana(float t, TipOznake o, Cvor<TipOznake> *p, Cvor<TipOznake> *s)
      : tez(t), ozn(o), P(p), S(s) {}
  float dajTezinu() { return tez; }
  void postaviTezinu(float t) {
    P->pok->postaviTezinuGrane(P->dajRedniBroj(), S->dajRedniBroj(), t);
  }
  void postaviTezinuPom(float t) { tez = t; }
  TipOznake dajOznaku() { return ozn; }
  void postaviOznaku(TipOznake o) {
    P->pok->postaviOznakuGrane(P->dajRedniBroj(), S->dajRedniBroj(), o);
  }
  void postaviOznakuPom(TipOznake o) { ozn = o; }
  Cvor<TipOznake> dajPolazniCvor() { return *P; }
  Cvor<TipOznake> dajDolazniCvor() { return *S; }
  bool JesuLiIsti(int p, int k) {
      if(p == P->dajRedniBroj() && k == S->dajRedniBroj()) return true;
    return false;
  }
};
template <typename TipOznake>
class ListaGraf : public UsmjereniGraf<TipOznake> {
public:
  std::vector<std::vector<Grana<TipOznake> *>> lista;
  std::vector<Cvor<TipOznake> *> c;
  int avg = 0;
  TipOznake tip= TipOznake{};
  ListaGraf(int n) {
    Risajz(*this,n);
    Alociraj(c);
  }
  void Risajz(ListaGraf<TipOznake> &l,int n){
    lista.resize(n);
    c.resize(n, nullptr);
  }
  void Alociraj(std::vector<Cvor<TipOznake> *> &c,int n=0){
      for (int i = n; i < lista.size(); i++)
      c[i] = new Cvor<TipOznake>(tip, i, this);
  }
    
  ~ListaGraf() {
    Obrisi(*this);
  }
 
  int dajBrojCvorova() { return c.size(); }
  void provjera(int n){
      if(n>c.size())
      throw std::logic_error("Greska");
  }
  void postaviBrojCvorova(int n) {
    if (n < c.size())
      throw std::logic_error("Greska");
    int temp = c.size();
    Risajz(*this,  n);
    Alociraj(c,temp);
  }
  void dodajGranu(int P, int S, float t) {
    provjera(P);
    auto pom=new Grana<TipOznake>(t, tip, c[P], c[S]);
    lista[P].push_back(pom);
  }
  void obrisiGranu(int P, int S) {
    provjera(P);
    provjera(S);
    for (int i = 0; i < lista[P].size(); i++){      
      if (lista[P][i]->JesuLiIsti(P, S)) {
        delete lista[P][i];
        lista[P].erase(lista[P].begin() + i);
        return;
      }
      
    }
     throw std::logic_error("Greska");
  }
  void postaviTezinuGrane(int P, int S, float t) {
    for (int i = 0; i < lista[P].size(); i++) {
      if (lista[P][i]->JesuLiIsti(P, S)) {
        lista[P][i]->postaviTezinuPom(t);
        return;
      }
    }
  }
   void Obrisi(ListaGraf<TipOznake> &l){
      for (int i = 0; i < lista.size(); i++)
      for (int j = 0; j < lista[i].size(); j++)
        delete lista[i][j];
    for (int i = 0; i < c.size(); i++)
      delete c[i];
  }
  void nadiketiOznaku(vector<Grana<TipOznake>*> &l, int P, int S, TipOznake o){
      for (int i = 0; i < lista[P].size(); i++) {
      if (lista[P][i]->JesuLiIsti(P, S)) {
        lista[P][i]->postaviOznakuPom(o);
        return;
      }
    }
    throw std::logic_error("Greska");
  }
  float dajTezinuGrane(int P, int S) {
    provjera(P);
    provjera(S);
    for (int i = 0; i < lista[P].size(); i++) {
      if (lista[P][i]->JesuLiIsti(P, S)) {
        return lista[P][i]->dajTezinu();
      }
    }
    throw std::logic_error("Greska");
  }
  bool postojiGrana(int P, int S) {
    if (P > c.size() || S > c.size())
      return false;
    for (int i = 0; i < lista[P].size(); i++) {
      if (lista[P][i]->JesuLiIsti(P, S)) {
        return true;
      }
    }
    return false;
  }
  void postaviOznakuCvora(int n, TipOznake o) {
    if (c[n] == nullptr)
      throw std::logic_error("Greska");
    c[n]->postaviOznakuPom(o);
  }
  TipOznake dajOznakuCvora(int n) {
    if (c[n] == nullptr)
     throw std::logic_error("Greska");
    return c[n]->dajOznaku();
  }
  void postaviOznakuGrane(int P, int S, TipOznake o) {
    provjera(P);
    provjera(S);
    nadiketiOznaku(lista[P],  P,  S, o);

  }
  
  TipOznake dajOznakuGrane(int P, int S) {
    provjera(P);
    provjera(S);
    for (int i = 0; i < lista[P].size(); i++) {
      if (lista[P][i]->JesuLiIsti(P, S)) {
        return lista[P][i]->dajOznaku();
      }
    }
    throw std::logic_error("Greska");
  }
  Grana<TipOznake> dajGranu(int P, int S) {
    for (int i = 0; i < lista[P].size(); i++) {
      if (lista[P][i]->JesuLiIsti(P, S)) {
        return *lista[P][i];
      }
    }
  }
  Cvor<TipOznake> dajCvor(int n) {
    if (c[n] == nullptr)
     throw std::logic_error("Greska");
    return *c[n];
  }
  virtual GranaIterator<TipOznake> dajGranePocetak() {
    for (int i = 0; i < lista.size(); i++) {
      for (int j = 0; j < lista[i].size(); j++) {
        if (lista[i][j] != nullptr)
          return GranaIterator<TipOznake>(this, i, j);
      }
    }
    return GranaIterator<TipOznake>(this, 0, 0);
  }
  virtual GranaIterator<TipOznake> dajGraneKraj() {
    for (int i = lista.size() - 1; i > 0; i--) {
      for (int j = lista[i].size() - 1; j > 0; j--) {
        if (lista[i][j] != nullptr) {

          if (j < lista[i].size() - 1)
            return GranaIterator<TipOznake>(this, i, j + 1);
          else
            return GranaIterator<TipOznake>(this, i + 1, 0);
        }
      }
    }
    return GranaIterator<TipOznake>(this, lista.size() - 1,
                                    lista[lista.size() - 1].size() - 1);
  }
  friend class GranaIterator<TipOznake>;
  template <typename TipOznake1>
  friend void dfs(UsmjereniGraf<TipOznake1> *g,
                  std::vector<Cvor<bool>> &obilazak, Cvor<TipOznake> cvor);
  template <typename TipOznake1>
  friend void bfs(UsmjereniGraf<TipOznake1> *g,
                  std::vector<Cvor<bool>> &obilazak, Cvor<TipOznake1> cvor,
                  int Sedeci);
};
template <typename TipOznake1>
void dfs(UsmjereniGraf<TipOznake1> *g, std::vector<Cvor<bool>> &obilazak,
         Cvor<TipOznake1> cvor) {
  ListaGraf<TipOznake1> *temp = dynamic_cast<ListaGraf<TipOznake1> *>(g);
  auto pos = TipOznake1(1);
  temp->c[cvor.dajRedniBroj()]->postaviOznaku(pos);
  obilazak.push_back(cvor);
  temp->avg++;
  for (int j = 0; j < temp->lista[cvor.dajRedniBroj()].size(); j++) {
    if (temp->lista[cvor.dajRedniBroj()][j]->dajDolazniCvor().dajOznaku() != pos) {
      dfs(g, obilazak,
          temp->lista[cvor.dajRedniBroj()][j]->dajDolazniCvor());
    }
  }
  if (temp->avg < temp->dajBrojCvorova()) {
    for (int i = 0; i < temp->lista.size(); i++) {
      for (int j = 0; j < temp->lista[i].size(); j++) {
        if (temp->lista[i][j] != nullptr &&
            temp->lista[i][j]->dajDolazniCvor().dajOznaku() != pos) {
          if (temp->c[i]->dajOznaku() != pos) {
            temp->c[i]->postaviOznaku(pos);
            obilazak.push_back(*temp->c[i]);
          }
          dfs(g, obilazak, temp->lista[i][j]->dajDolazniCvor());
        }
      }
    }
  }
  if (temp->avg == temp->dajBrojCvorova())
    temp->avg = 0;
}

template <typename TipOznake1>
void bfs(UsmjereniGraf<TipOznake1> *g, std::vector<Cvor<bool>> &obilazak,
         Cvor<TipOznake1> cvor, int Sedeci = 0) {
  ListaGraf<TipOznake1> *temp = dynamic_cast<ListaGraf<TipOznake1> *>(g);
  if (temp->avg == temp->dajBrojCvorova()) {
    temp->avg = 0;
    return;
  }
  auto pos = TipOznake1(1);
  if (temp->c[cvor.dajRedniBroj()]->dajOznaku() != pos) {
    temp->c[cvor.dajRedniBroj()]->postaviOznaku(pos);
    obilazak.push_back(cvor);
    temp->avg++;
  }
  for (int i = 0; i < temp->lista[cvor.dajRedniBroj()].size(); i++) {
    if (temp->lista[cvor.dajRedniBroj()][i]->dajDolazniCvor().dajOznaku() !=
        pos) {
      obilazak.push_back(temp->lista[cvor.dajRedniBroj()][i]->dajDolazniCvor());
      temp->lista[cvor.dajRedniBroj()][i]->dajDolazniCvor().postaviOznaku(pos);
      temp->avg++;
    }
  }
  bfs(g, obilazak, obilazak[++Sedeci], Sedeci);
}

template <typename TipOznake> class GranaIterator {
  ListaGraf<TipOznake> *lista;
  int P;
  int koordinata;

public:
  GranaIterator(ListaGraf<TipOznake> *pok, int P, int kraj)
      : lista(pok), P(P), koordinata(kraj) {}
  Grana<TipOznake> operator*() {
    return *lista->lista[P][koordinata];
    throw std::logic_error("Greska");
  }
  bool operator==(const GranaIterator &iter) const {
      if(lista == iter.lista && P == iter.P &&
            koordinata == iter.koordinata) return true;
    return false;
  }
  bool operator!=(const GranaIterator &iter) const {
    if (!(lista == iter.lista && P == iter.P &&
             koordinata == iter.koordinata)) return true;
    return false;
  }
  GranaIterator &operator++() {
    if (P == lista->lista.size() - 1 &&
        koordinata == lista->lista[lista->lista.size() - 1].size() - 1)
      return *this;
    if (koordinata != lista->lista[P].size() - 1) {
      koordinata++;

    } else {
      koordinata = 0;
      P++;
    }
    return *this;
  }
  GranaIterator operator++(int) {
    GranaIterator<TipOznake> temp(lista, P, koordinata);
    if (P == lista->lista.size() - 1 &&
        koordinata == lista->lista[lista->lista.size() - 1].size() - 1)
      return *this;
    if (koordinata != lista->lista[P].size() - 1) {
      koordinata++;
    } else {
      koordinata = 0;
      P++;
    }
    return temp;
  }
};
int main() {
/*UsmjereniGraf<std::string> *g = new ListaGraf<std::string>(4);
  g->dodajGranu(0, 1, 2.5);
  g->dodajGranu(1, 2, 1.2);
  g->dodajGranu(1, 3, 0.1);
  g->dodajGranu(3, 3, -4.0);
  g->postaviOznakuGrane(1, 3, "aa");
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      if (g->postojiGrana(i,j))
        std::cout << "(" << i << "," << j << ") -> '" << g->dajOznakuGrane(i, j) << "'; ";
  delete g;*/
  return 0;
}