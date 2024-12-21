#include <deque>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
using namespace std;

template <typename tip> class GranaIterator;
template <typename tip> class Grana;
template <typename tip> class Cvor;
template <typename tip> class UsmjereniGraf {
private:
  int br_c;
public:
  UsmjereniGraf(int br) {}
  virtual ~UsmjereniGraf() {}
  virtual int dajBrojCvorova() const = 0;
  virtual void postaviBrojCvorova(int br) = 0;
  virtual void dodajGranu(int p, int d, float tezina) = 0;
  virtual void obrisiGranu(int p, int d) = 0;
  virtual void postaviTezinuGrane(int p, int d, float tezina) = 0;
  virtual void postaviOznakuGrane(int p, int d, tip oznaka) = 0;
  virtual float dajTezinuGrane(int p, int d) const = 0;
  virtual bool postojiGrana(int p, int d) const = 0;
  virtual void postaviOznakuCvora(int cvor, tip oznaka) = 0;
  virtual tip dajOznakuCvora(int cvor) const=0;
  virtual tip dajOznakuGrane(int p, int d) const = 0;
  Grana<tip> dajGranu(int p, int d) { return Grana<tip>(this, p, d); }
  Cvor<tip> dajCvor(int cvor) { return Cvor<tip>(this, cvor); }
  virtual GranaIterator<tip> dajGranePocetak() = 0;
  virtual GranaIterator<tip> dajGraneKraj() = 0;
  virtual GranaIterator<tip> dajSljedecuGranu(int p, int d) = 0;
  virtual void ispisiGraf() const=0;
};

template <typename tip> class MatricaGraf : public UsmjereniGraf<tip> {
  struct OpisGrane {
    tip oznaka;
    float tezina;
    OpisGrane(float t) : oznaka(tip()), tezina(t) {}
    OpisGrane(tip oznaka, float tezina) : oznaka(oznaka), tezina(tezina) {}
  };
  vector<vector<OpisGrane *>> matrica;
  vector<tip> oznake;

public:
  void ispisiGraf() const{
      for(int i=0;i<matrica.size();i++){
          for(int j=0;j<matrica.size();j++){
              if(matrica[i][j]!=nullptr)
                std::cout<<setw(6)<<matrica[i][j]->tezina;
            else std::cout<<setw(6)<<0;
          }
          std::cout<<"\n";
      }
  }

  MatricaGraf(int broj) : UsmjereniGraf<tip>(broj) {
    int vel = broj;
    matrica.resize(vel);
    for (vector<OpisGrane *> &x : matrica)
      x.resize(broj, nullptr);
    oznake.resize(vel);
  }
  ~MatricaGraf() {
    for (vector<OpisGrane *> &x : matrica) {
      for (OpisGrane *&r : x) {
        if (r != nullptr)
          delete r;
      }
    }
  }
  MatricaGraf(const MatricaGraf<tip> &mg) {
    oznake = mg.oznake;
    matrica.resize(mg.matrica.size());
    for (vector<OpisGrane *> &x : matrica) {
      x.resize(mg.matrica.size(), nullptr);
    }
    for (int i = 0; i < mg.matrica.size(); i++) {
      for (int j = 0; j < mg.matrica.size(); j++) {
        if (mg.matrica[i][j] != nullptr)
          matrica[i][j] =
              new OpisGrane(mg.matrica[i][j]->oznaka, mg.matrica[i][j]->tezina);
        else
          matrica[i][j] = nullptr;
      }
    }
  }

  MatricaGraf<tip> &operator=(const MatricaGraf<tip> &mg) {
    if (this == &mg)
      return *this;
    for (vector<OpisGrane *> &x : matrica) {
      for (OpisGrane *&r : x) {
        if (r != nullptr)
          delete r;
      }
    }
    oznake = mg.oznake;
    matrica.resize(mg.matrica.size());
    for (vector<OpisGrane *> &x : matrica)
      x.resize(mg.matrica.size(), nullptr);
    for (int i = 0; i < mg.matrica.size(); i++) {
      for (int j = 0; j < mg.matrica.size(); j++) {
        if (mg.matrica[i][j] != nullptr)
          matrica[i][j] =
              new OpisGrane(mg.matrica[i][j]->oznaka, mg.matrica[i][j]->tezina);
        else
          matrica[i][j] = nullptr;
      }
    }
    return *this;
  }
  int dajBrojCvorova() const { return matrica.size(); }
  void obrisiGranu(int p, int d) {
    delete matrica[p][d];
    matrica[p][d] = nullptr;
  }
  void dodajGranu(int p, int d, float tezina) {
    matrica[p][d] = new OpisGrane(tezina);
  }
  void postaviTezinuGrane(int p, int d, float tezina) {
    matrica[p][d]->tezina = tezina;
  }
  float dajTezinuGrane(int p, int d) const { return matrica[p][d]->tezina; }
  bool postojiGrana(int p, int d) const { return matrica[p][d] != nullptr; }
  void postaviOznakuGrane(int p, int d, tip oznaka) {
    matrica[p][d]->oznaka = oznaka;
  }
  tip dajOznakuGrane(int p, int d) const { return matrica[p][d]->oznaka; }
  GranaIterator<tip> dajGranePocetak() {
    GranaIterator<tip> gi(this, 0, -1);
    return ++gi;
  }
  GranaIterator<tip> dajGraneKraj() {
      int kraj=-1;
    GranaIterator<tip> gi(this, kraj, kraj);
    return gi;
  }
  GranaIterator<tip> dajSljedecuGranu(int p, int d) {
    for (int i = p; i < matrica.size(); i++) {
      for (int j = 0; j < matrica.size(); j++) {
        if (i == p && j <= d)
          continue;
        if (matrica[i][j] != nullptr)
          return GranaIterator<tip>(this, i, j);
      }
    }
    int k=-1;
    return GranaIterator<tip>(this, k, k);
  }
  void postaviBrojCvorova(int broj) {
    if (broj < matrica.size())
      throw "NE!";
    for (vector<OpisGrane *> &x : matrica) {
      x.resize(broj, nullptr);
    }
    vector<OpisGrane *> prazan(broj, nullptr);
    matrica.resize(broj, prazan);
  }
  tip dajOznakuCvora(int cvor)const{
      return oznake[cvor];
  }
  void postaviOznakuCvora(int cvor, tip oznaka) { oznake[cvor] = oznaka; }
};

template <typename tip> class GranaIterator {
  UsmjereniGraf<tip> *graf;
  int p, d;
  bool postoji;

public:
  GranaIterator(UsmjereniGraf<tip> *g, int p, int d) : graf(g), p(p), d(d) {}
  Grana<tip> operator*() { return Grana<tip>(graf, p, d); }
  friend class Grana<tip>;
  bool operator==(const GranaIterator<tip> &gi) const {
    return (graf == gi.graf && p == gi.p && d == gi.d);
  }
  bool operator!=(const GranaIterator<tip> &gi) const { return !(*this == gi); }
  GranaIterator<tip> &operator++() {
    if (p == -1 && d == -1)
      throw std::logic_error("Pozicija losaaa");
    GranaIterator<tip> g = graf->dajSljedecuGranu(p, d);
    p = g.p;
    d = g.d;
    return *this;
  }
  GranaIterator<tip> operator++(int) {
    GranaIterator<tip> pom(*this);
    ++(*this);
    return pom;
  }
};

template <typename tip> class Grana {
  UsmjereniGraf<tip> *graf;
  int p, d;

public:
  Grana(UsmjereniGraf<tip> *g, int p, int d) : graf(g), p(p), d(d) {}
  float dajTezinu() const { return graf->dajTezinuGrane(p, d); }
  void postaviTezinu(float tezina) { graf->postaviTezinuGrane(p, d, tezina); }
  tip dajOznaku() const { return graf->dajOznakuGrane(p, d); }
  void postaviOznaku(tip oznaka) { graf->postaviOznakuGrane(p, d, oznaka); }
  Cvor<tip> dajPolazniCvor() const { return graf->dajCvor(p); }
  Cvor<tip> dajDolazniCvor() const { return graf->dajCvor(d); }
};

template <typename tip> class Cvor {
private:
  UsmjereniGraf<tip> *graf;
  int rb;
public:
  Cvor(UsmjereniGraf<tip> *g, int redni) : graf(g), rb(redni) {}
  tip dajOznaku() const { return graf->dajOznakuCvora(rb); }
  void postaviOznaku(tip oznaka) { graf->postaviOznakuCvora(rb, oznaka); }
  int dajRedniBroj() const { return rb; }
};

template<typename tip>
void dfs (UsmjereniGraf<tip> *graf,vector<Cvor<tip>> &dfsobilazak,Cvor<tip> cvor){
    cvor.postaviOznaku(1);
    dfsobilazak.push_back(cvor);
    for(GranaIterator<tip> it=graf->dajGranePocetak();it!=graf->dajGraneKraj();it++){
        Cvor<tip> d=*it.dajDolazniCvor();
        if(*it.dajPolazniCvor().dajRedniBroj()==cvor.dajRedniBroj()&&d.dajOznaku()!=1){
            dfs(graf,dfsobilazak,d);
        }
    }
}

template<typename tip>
void bfs (UsmjereniGraf<tip> *graf,vector<Cvor<tip>> &bfsobilazak,Cvor<tip> cvor){
    cvor.postaviOznaku(1);
    bfsobilazak.push_back(cvor);
    queue<Cvor<tip>> red;
    red.push(cvor);
    while(!red.empty()){
        Cvor<tip> pom=red.front();
        red.pop();
        for(GranaIterator<tip> it=graf->dajGranePocetak();it!=graf->dajGraneKraj();it++){
        Cvor<tip> p=*it.dajPolazniCvor();
        Cvor<tip> d=*it.dajDolazniCvor();
        if(*it.dajDolazniCvor().dajOznaku()!=1&&p.dajOznaku()==pom.dajRedniBroj()){
            d.postaviOznaku(1);
            bfsobilazak.push_back(d);
            red.push(d);
        }
    }
    }
}
int main() {
 /*UsmjereniGraf<bool> *g = new MatricaGraf<bool>(6);
    g->dodajGranu(0, 1, 2.5);
    g->dodajGranu(1, 0, 1.2);
    g->dodajGranu(1, 2, 0.1);
    g->dodajGranu(0, 0, 3.14);
    for (GranaIterator<bool> iter = g->dajGranePocetak();
         iter != g->dajGraneKraj(); ++iter)
      std::cout << "(" << (*iter).dajPolazniCvor().dajRedniBroj() << ","
           << (*iter).dajDolazniCvor().dajRedniBroj() << ") -> "
           << (*iter).dajTezinu() << "; ";
  delete g;*/
  return 0;
}