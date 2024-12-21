#include <iostream>
#include <string>
using namespace std;

template <typename kljuc, typename vrijednost> class Mapa {
public:
    Mapa(){};
    virtual ~Mapa(){};
    virtual const vrijednost &operator[](const kljuc &indeks) const = 0;
    virtual vrijednost &operator[](const kljuc &indeks) = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const kljuc &Kljuc) = 0;
};

template <typename tipkljuca, typename tipvrijednosti> struct cvor {
    tipkljuca kljuc = tipkljuca();
    tipvrijednosti vrijednost = tipvrijednosti();
    cvor *lijevo = nullptr, *desno = nullptr, *roditelj = nullptr;
    int balans = 0;
};

template <typename kljuc, typename vrijednost>
class AVLStabloMapa : public Mapa<kljuc, vrijednost> {
private:
    int vel = 0;
    vrijednost blah = vrijednost();
    cvor<kljuc, vrijednost> *korijen;
    void kopirajstablo(const cvor<kljuc,vrijednost> *c){ korijen=kopirajCvor(c); }
    cvor<kljuc,vrijednost>* kopirajCvor(const cvor<kljuc,vrijednost> *c){
        if(!c) return nullptr;
        cvor<kljuc,vrijednost> *kopiraj=new cvor<kljuc,vrijednost>();
        kopiraj->kljuc = c->kljuc;
        kopiraj->vrijednost = c->vrijednost;
        kopiraj->balans=c->balans;

        kopiraj->lijevo=kopirajCvor(c->lijevo);
        if(kopiraj->lijevo) kopiraj->lijevo->roditelj=kopiraj;
        kopiraj->desno=kopirajCvor(c->desno);
        if(kopiraj->desno) kopiraj->desno->roditelj=kopiraj;
        
        return kopiraj;
    }
    int visina(cvor<kljuc,vrijednost> *c) {
        if (c == nullptr) return 0;
        else return c->balans;
    }
    void nadiketi(cvor<kljuc,vrijednost> *c){
        c->balans=std::max(visina(c->lijevo),visina(c->desno))+1;
    }
    int dajBalans(cvor<kljuc, vrijednost> *c){
        return visina(c->lijevo)-visina(c->desno);
    }
    void poveziL(cvor<kljuc, vrijednost> *pale,cvor<kljuc, vrijednost> *trenutno){
        pale->lijevo=trenutno;
        if(trenutno) trenutno->roditelj=pale;
    }
    void poveziD(cvor<kljuc, vrijednost> *pale,cvor<kljuc, vrijednost> *trenutno){
        pale->desno=trenutno;
        if(trenutno) trenutno->roditelj=pale;
    }
    cvor<kljuc, vrijednost>* azurirajBalans(cvor<kljuc, vrijednost> *Cvor) {
        nadiketi(Cvor);
        int bf=dajBalans(Cvor);
        
        if (bf>1){
            if(dajBalans(Cvor->lijevo)<0) lijevaRotacija(Cvor->lijevo);
            desnaRotacija(Cvor);
        }
        if(bf<-1){
            if(dajBalans(Cvor->desno)>0) desnaRotacija(Cvor->desno);
            lijevaRotacija(Cvor);
        }
        if (Cvor->roditelj == nullptr) return Cvor;
        return azurirajBalans(Cvor->roditelj);
        
    }
    cvor<kljuc, vrijednost> *lijevaRotacija(cvor<kljuc, vrijednost> *Cvor) {
        cvor<kljuc, vrijednost> *roditeljTemp=Cvor->roditelj;
        cvor<kljuc, vrijednost> *desnoTemp=Cvor->desno;
        cvor<kljuc, vrijednost> *lijevoTemp=desnoTemp->lijevo;
        if(!roditeljTemp) desnoTemp->roditelj=nullptr;
        else if(Cvor==Cvor->roditelj->lijevo) poveziL(roditeljTemp,desnoTemp);
        else poveziD(roditeljTemp,desnoTemp);
        poveziL(desnoTemp,Cvor);
        poveziD(Cvor,lijevoTemp);
        nadiketi(Cvor);
        nadiketi(desnoTemp);
        return desnoTemp;
    }
    cvor<kljuc, vrijednost> *desnaRotacija(cvor<kljuc, vrijednost> *Cvor) {
        cvor<kljuc, vrijednost> *roditeljTemp=Cvor->roditelj;
        cvor<kljuc, vrijednost> *lijevoTemp=Cvor->lijevo;
        cvor<kljuc, vrijednost> *desnoTemp=lijevoTemp->desno;
        if(!roditeljTemp) lijevoTemp->roditelj=nullptr;
        else if(Cvor==Cvor->roditelj->lijevo) poveziL(roditeljTemp,lijevoTemp);
        else poveziD(roditeljTemp,lijevoTemp);
        poveziD(lijevoTemp,Cvor);
        poveziL(Cvor,desnoTemp);
        nadiketi(Cvor);
        nadiketi(lijevoTemp);
        return lijevoTemp;
    }

public:
    void obrisicijelo(cvor<kljuc,vrijednost> *korijen){
        if(korijen==nullptr) return;
        obrisicijelo(korijen->lijevo);
        obrisicijelo(korijen->desno);
        delete korijen;
    }
    AVLStabloMapa() { korijen = nullptr; }
    ~AVLStabloMapa() { obrisicijelo(korijen); }
    void obrisi() {
        vel = 0;
        obrisicijelo(korijen);
        korijen = nullptr;
    }
    int brojElemenata() const { return vel; }
    
    void obrisi(const kljuc &k) {
        if (vel == 0) return;
        cvor<kljuc, vrijednost> *p = korijen;
        cvor<kljuc, vrijednost> *r = nullptr;
        while (p != nullptr && k != p->kljuc) {
            r = p;
            if (k < p->kljuc) p = p->lijevo;
            else p = p->desno;
        }
        if (p == nullptr) return;
        vel--;
        if (vel == 0) {
            obrisicijelo(korijen);
            korijen = nullptr;
            return;
        }
        cvor<kljuc, vrijednost> *rp = nullptr;
        cvor<kljuc, vrijednost> *stari_r = nullptr;
        if (p->lijevo == nullptr) rp = p->desno;
        else if (p->desno == nullptr) rp = p->lijevo;
        else
        {
            cvor<kljuc, vrijednost> *pp = p;
            rp = p->lijevo;
            cvor<kljuc, vrijednost> *tmp = rp->desno;
            while (tmp != nullptr)
            {
                pp = rp;
                rp = tmp;
                tmp = rp->desno;
            }
            stari_r = rp->roditelj;
            if (pp != p)
            {
                poveziD(pp, rp->lijevo);
                poveziL(rp, p->lijevo);
            }
            poveziD(rp, p->desno);
        }
        if (r == nullptr) {
            rp->roditelj = nullptr;
            korijen = rp;
        }
        else if (p == r->lijevo) poveziL(r, rp);
        else poveziD(r, rp);

        if (stari_r && stari_r != p) korijen = azurirajBalans(stari_r);
        delete p;
    }
    const vrijednost& operator[](const kljuc &indeks) const  {
        if (!vel) return blah; 
        cvor<kljuc, vrijednost> *w = korijen;
        for (;;)
        {
            if(w->kljuc==indeks) return w->vrijednost;
            if(indeks<w->kljuc)
            {
                if(!w->lijevo) break;
                w = w->lijevo;
            }
            else
            {
                if(!w->desno) break;
                w = w->desno;
            }
        }

        return blah;
    }
    vrijednost &operator[](const kljuc &indeks)  {
        cvor<kljuc, vrijednost> *temp = korijen;
        cvor<kljuc, vrijednost> *pokk = nullptr;
        while (temp != nullptr && indeks != temp->kljuc) {
            pokk = temp;
            if (indeks > temp->kljuc) temp = temp->desno;
            else temp = temp->lijevo;
        }
        if (temp == nullptr)
        {
            temp = new cvor<kljuc, vrijednost>();
            temp->vrijednost = vrijednost();
            temp->kljuc = indeks;
            temp->lijevo = nullptr;
            temp->desno = nullptr;
            temp->balans = 0;

            if (korijen == nullptr) korijen = temp;
            else
            {
                if (indeks < pokk->kljuc) poveziL(pokk, temp);
                else poveziD(pokk, temp);
                korijen = azurirajBalans(pokk);
            }
            vel++;
        }
        return temp->vrijednost;
    }
    
    AVLStabloMapa(AVLStabloMapa<kljuc, vrijednost> &&stablo) {
        vel = stablo.vel;
        korijen = stablo.korijen;
    }
    AVLStabloMapa &operator=(const AVLStabloMapa<kljuc, vrijednost> &stablo) {
        if (&stablo == this) return *this;
        obrisicijelo(korijen);
        vel = stablo.vel;
        kopirajstablo(stablo.korijen);
        return *this;
    }
    AVLStabloMapa(const AVLStabloMapa<kljuc, vrijednost> &m ): AVLStabloMapa(){
        *this=m;
    }
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
    if (c == nullptr) 
    {
      c = new cvor(k, vrijednost(), roditelj);
      velicina++;
      return c;
    }
    else if (k < c->key) return dodaj(c->lijevo, k, c);
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
  const vrijednost &operator[](const kljuc &indeks) const override {
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
      else if (del == del->roditelj->lijevo)
      {
        del->roditelj->lijevo = pom;
        pom->roditelj = del->roditelj;
      }
      else
      {
        del->roditelj->desno = pom;
        pom->roditelj = del->roditelj;
      }
      if (del == korijen) pom->lijevo = nullptr;
      else
      {
        pom->lijevo = del->lijevo;
        del->lijevo->roditelj = pom;
      }
      delete del;
    }
    else
    {
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
AVLStabloMapa <int, int> m;
m[100] = 200;
const AVLStabloMapa <int, int>& m2(m);
m[200] = 300;
cout << m2.brojElemenata() << " "; // 2
cout << m2[200] << " "; // 300

cout << m2[300] << " "; // 0
cout << m2.brojElemenata() << " "; // 2
m[300] = 400;
cout << m2[300] << " "; // 400
cout << m2.brojElemenata(); // 3
    return 0;
}
